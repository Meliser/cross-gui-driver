#include "elf_parser.h"

// std
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <variant>


#define EI_NIDENT 16

/* 32-bit ELF base types. */
typedef uint32_t Elf32_Addr;
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Off;
typedef int32_t  Elf32_Sword;
typedef uint32_t Elf32_Word;

/* 64-bit ELF base types. */
typedef	int32_t  Elf64_Sword;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Xword;

#define ELFMAG "\177ELF"

struct  Elf32_Ehdr
{
  unsigned char e_ident[EI_NIDENT];
  Elf32_Half    e_type;
  Elf32_Half    e_machine;
  Elf32_Word    e_version;
  Elf32_Addr    e_entry;  /* Entry point */
  Elf32_Off     e_phoff;
  Elf32_Off     e_shoff;
  Elf32_Word    e_flags;
  Elf32_Half    e_ehsize;
  Elf32_Half    e_phentsize;
  Elf32_Half    e_phnum;
  Elf32_Half    e_shentsize;
  Elf32_Half    e_shnum;
  Elf32_Half    e_shstrndx;
};

struct  Elf32_Shdr
{
  Elf32_Word  sh_name;
  Elf32_Word  sh_type;
  Elf32_Word  sh_flags;
  Elf32_Addr  sh_addr;
  Elf32_Off   sh_offset;
  Elf32_Word  sh_size;
  Elf32_Word  sh_link;
  Elf32_Word  sh_info;
  Elf32_Word  sh_addralign;
  Elf32_Word  sh_entsize;
};

struct  Elf64_Ehdr
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
  Elf64_Half    e_type;             /* Object file type */
  Elf64_Half    e_machine;          /* Architecture */
  Elf64_Word    e_version;          /* Object file version */
  Elf64_Addr    e_entry;            /* Entry point virtual address */
  Elf64_Off     e_phoff;            /* Program header table file offset */
  Elf64_Off     e_shoff;            /* Section header table file offset */
  Elf64_Word    e_flags;            /* Processor-specific flags */
  Elf64_Half    e_ehsize;           /* ELF header size in bytes */
  Elf64_Half    e_phentsize;        /* Program header table entry size */
  Elf64_Half    e_phnum;            /* Program header table entry count */
  Elf64_Half    e_shentsize;        /* Section header table entry size */
  Elf64_Half    e_shnum;            /* Section header table entry count */
  Elf64_Half    e_shstrndx;         /* Section header string table index */
};

struct  Elf64_Shdr
{
  Elf64_Word  sh_name;      /* Section name (string tbl index) */
  Elf64_Word  sh_type;      /* Section type */
  Elf64_Xword sh_flags;     /* Section flags */
  Elf64_Addr  sh_addr;      /* Section virtual addr at execution */
  Elf64_Off   sh_offset;    /* Section file offset */
  Elf64_Xword sh_size;      /* Section size in bytes */
  Elf64_Word  sh_link;      /* Link to another section */
  Elf64_Word  sh_info;      /* Additional section information */
  Elf64_Xword sh_addralign; /* Section alignment */
  Elf64_Xword sh_entsize;   /* Entry size if section holds table */
};



    template <int bits> struct HdrTraits { };

    template<>
    struct HdrTraits<32>
    {
      typedef Elf32_Ehdr EhdrT;
      typedef Elf32_Shdr ShdrT;
      enum { Type = ElfParser::FileFormat::elf_x86_32 };
    };

    template<>
    struct HdrTraits<64>
    {
      typedef Elf64_Ehdr EhdrT;
      typedef Elf64_Shdr ShdrT;
      enum { Type = ElfParser::FileFormat::elf_x86_64 };
    };

    // =========================================================================
    class  ElfParser::Impl
    {
    public:
      Impl(const std::string& file_name, bool throw_unknown) :
        m_fs(file_name.c_str(), std::ios::in | std::ios::binary),
        m_format(ElfParser::FileFormat::unknown),
        m_string_section(nullptr),
        m_custom_section(nullptr)
      {

          if (!m_fs.good())
            throw std::runtime_error("Невозможно открыть файл.");
          m_fs.exceptions(std::ios::badbit);

          char buf[sizeof(Elf64_Ehdr)];
          m_format = checkFormat(buf);
          switch (m_format)
          {
            case ElfParser::FileFormat::elf_x86_32:
              loadHeader<32>(buf);
              loadStringSection<32>();
              break;
            case ElfParser::FileFormat::elf_x86_64:
              loadHeader<64>(buf);
              loadStringSection<64>();
              break;
            default:
              if (throw_unknown)
                throw std::runtime_error("Неверный формат ELF-файла.");
          }
      }

      std::optional<ElfParser::SectionLoc> findSection(std::string_view section_name)
      {
        switch (m_format)
        {
          case ElfParser::FileFormat::elf_x86_32:
            return internalFindSection<32>(section_name);
          case ElfParser::FileFormat::elf_x86_64:
            return internalFindSection<64>(section_name);
          default:
            throw std::runtime_error("BADD!!!!!");
        }
      }

      ElfParser::SectionDesc getSectionContent(const std::string& section_name)
      {
        switch (m_format)
        {
          case ElfParser::FileFormat::elf_x86_32:
            return internalGetSectionContent<32>(section_name);
          case ElfParser::FileFormat::elf_x86_64:
            return internalGetSectionContent<64>(section_name);
          default:
           throw std::runtime_error("BEFEGW");
        }
      }

      ElfParser::FileFormat::Type getFileFormat() const
      {
        return m_format;
      }

      ~Impl()
      {
        if (m_string_section)
          free(m_string_section);
        if (m_custom_section)
          free(m_custom_section);
      }

    private:

      // проверка формата
      ElfParser::FileFormat::Type checkFormat(char* buf)
      {
        m_fs.read(buf, sizeof(Elf64_Ehdr));
        if (m_fs.fail())
          return ElfParser::FileFormat::unknown;
        m_fs.exceptions(std::ios::failbit | std::ios::badbit);
        if (0 == strncmp(&buf[0], ELFMAG, 4)) {
          if (sizeof(Elf32_Shdr) == ((Elf32_Ehdr*) buf)->e_shentsize)
            return ElfParser::FileFormat::elf_x86_32;
          if (sizeof(Elf64_Shdr) == ((Elf64_Ehdr*) buf)->e_shentsize)
            return ElfParser::FileFormat::elf_x86_64;
        }
        return ElfParser::FileFormat::unknown;
      }

      // чтение заголовка
      template<int bits>
      void loadHeader(const char* buf)
      {
        typedef HdrTraits<bits> TraitsT;
        m_ehdr = typename TraitsT::EhdrT();
        memcpy(&std::get<typename TraitsT::EhdrT>(m_ehdr), buf, sizeof(typename TraitsT::EhdrT));
      }

      // чтение строковой секции
      template<int bits>
      void loadStringSection()
      {
        typedef HdrTraits<bits> TraitsT;
        m_fs.seekg(
          std::get<typename TraitsT::EhdrT>(m_ehdr).e_shoff +
          sizeof(typename TraitsT::ShdrT) * std::get<typename TraitsT::EhdrT>(m_ehdr).e_shstrndx);
        typename TraitsT::ShdrT shdr;
        m_fs.read((char*) (&shdr), sizeof(typename TraitsT::ShdrT));
        if (shdr.sh_size > 0) {
          m_string_section = (char*) malloc(shdr.sh_size);
          m_fs.seekg(shdr.sh_offset);
          m_fs.read(m_string_section, shdr.sh_size);
        }
      }

      // поиск позиции секции в файле
      template<int bits>
      std::optional<ElfParser::SectionLoc> internalFindSection(std::string_view section_name)
      {
        if (!m_string_section)
          throw std::runtime_error("kajgksgd");

        typedef HdrTraits<bits> TraitsT;
        if (0 != std::get<typename TraitsT::EhdrT>(m_ehdr).e_shnum) {
          // поиск секции
          m_fs.seekg(std::get<typename TraitsT::EhdrT>(m_ehdr).e_shoff);
          for (int i = 0; i < std::get<typename TraitsT::EhdrT>(m_ehdr).e_shnum; ++i) {
            typename TraitsT::ShdrT shdr;
            m_fs.read((char*) (&shdr), sizeof(typename TraitsT::ShdrT));

            const std::string cur_section_name = m_string_section + shdr.sh_name;
            if (section_name == cur_section_name) {
              if (shdr.sh_size > 0)
                return SectionLoc { shdr.sh_offset, shdr.sh_size };
              else
                return { };
            }

          }
        }
        return { };
      }

      // чтение произвольной секции
      template<int bits>
      ElfParser::SectionDesc internalGetSectionContent(const std::string& section_name)
      {
        const std::optional<ElfParser::SectionLoc> loc = internalFindSection<bits>(section_name);
        if (!loc)
          return SectionDesc { 0, nullptr };
        if (m_custom_section) {
          free(m_custom_section);
          m_custom_section = nullptr;
        }
        // загрузка секции
        m_custom_section = malloc(loc.value().size);
        m_fs.seekg(loc.value().offset);
        m_fs.read((char*) m_custom_section, loc.value().size);
        return SectionDesc { loc.value().size, m_custom_section };
      }

      std::ifstream m_fs;
      ElfParser::FileFormat::Type m_format;
      std::variant<Elf32_Ehdr, Elf64_Ehdr> m_ehdr;
      char* m_string_section;
      void* m_custom_section;
    };

    // =========================================================================

    ElfParser::ElfParser(const std::string& file_name): pimpl(new Impl(file_name, true))
    { }

    ElfParser::ElfParser(ElfParser&& other): pimpl(other.pimpl)
    {
      other.pimpl = nullptr;
    }

    ElfParser& ElfParser::operator = (ElfParser&& e)
    {
      if (this != &e) {
        Impl* p = pimpl;
        pimpl = e.pimpl;
        e.pimpl = nullptr;
        delete p;
      }
      return *this;
    }

    ElfParser::FileFormat::Type ElfParser::checkFileFormat(const std::string& file_name)
    {
      return Impl(file_name, false).getFileFormat();
    }

    ElfParser::FileFormat::Type ElfParser::getFileFormat() const
    {
      return pimpl->getFileFormat();
    }

    std::optional<ElfParser::SectionLoc> ElfParser::findSection(std::string_view section_name)
    {
      return pimpl->findSection(section_name);
    }

    ElfParser::SectionDesc ElfParser::getSectionContent(const std::string& section_name)
    {
      return pimpl->getSectionContent(section_name);
    }

    ElfParser::~ElfParser()
    {
      delete pimpl;
    }
