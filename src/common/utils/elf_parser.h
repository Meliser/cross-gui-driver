#pragma once

// std
#include <string>
#include <optional>


class  ElfParser
{
public:
  ElfParser(const std::string& file_name);
  ElfParser(ElfParser&&);
  ElfParser(const ElfParser&) = delete;
  ~ElfParser();
  ElfParser& operator = (ElfParser&&);
  ElfParser& operator = (const ElfParser&) = delete;
  /// Формата файла (см. также checkFileFormat).
  struct FileFormat {
    enum Type { unknown, elf_x86_32, elf_x86_64 };
  };
  /// Проверка формата файла.
  static FileFormat::Type checkFileFormat(const std::string& file_name);
  /// Получение формата файла.
  FileFormat::Type getFileFormat() const;
  /// Расположение секции в файле.
  struct SectionLoc
  {
    size_t offset;
    size_t size;
  };
  /// Поиск секции в файле.
  std::optional<SectionLoc> findSection(std::string_view section_name);
  /// Содержимое секции ELF-файла (см. также getSectionContent).
  struct SectionDesc
  {
    size_t size;
    void* content;
  };
  /**
   * Запрос содержимого секции.
   *
   * Данные предыдущей запрошенной секции удаляются.
   * При отсутствии секции возвращается { 0, NULL }.
   */
  SectionDesc getSectionContent(const std::string& section_name);
  /// Поток с содержимым секции.
  /// Не использовать после нового вызова getSectionContent() или getSectionStream().
//   inline boost::iostreams::stream<boost::iostreams::array_source> getSectionStream(const std::string& section_name)
//   {
//     SectionDesc desc = getSectionContent(section_name);
//     return boost::iostreams::stream<boost::iostreams::array_source>((char*) desc.content, desc.size);
//   }
private:
  class Impl;
  Impl* pimpl;
};
