//#include <iostream>
//#include "app.h"
#include "dialog.h"
#include <QApplication>
#include <thread>
#include <iostream>
#include <boost/dll/import.hpp>

using HandlerType = bool();

int main(int argc, char *argv[])
{
    std::cout << "gui thread " << std::this_thread::get_id() <<std::endl;
    QApplication a(argc, argv);

    std::function<HandlerType> init;
    if (argc == 1) {
        std::cout << "DONT LOAD PLUGIN" << std::endl;
    }
    else {
        std::cout << "LOAD PLUGIN" << std::endl;
        init = boost::dll::import_alias<HandlerType>(
            "src/cross-gui-driver", "init_driver",
            boost::dll::load_mode::append_decorations);
        init();
    }

    CustomDialog dlg;
    dlg.exec();
 
    return a.exec();
}