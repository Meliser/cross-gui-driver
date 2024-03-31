#include <boost/dll/import.hpp>
#include <iostream>

using HandlerType = bool();
std::function<HandlerType> getInit()
{
  std::function<HandlerType> init = boost::dll::import_alias<HandlerType>(
      "src/cross-gui-driver", "init_driver",
      boost::dll::load_mode::append_decorations);
  return init;
}

class Initializer
{
public:
    Initializer()
    {
        std::cout << "Construct injector" << std::endl;
        m_init = getInit();
        m_init();
    }
    ~Initializer()
    {
        std::cout << "Destruct injector" << std::endl;
    }
private:
    std::function<HandlerType> m_init;
};

static Initializer auto_init;