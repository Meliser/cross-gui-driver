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
    if (argc != 2) {
        std::cout << "Pass 2 args" << std::endl;
        return -1;
    }

    std::cout << "gui thread " << std::this_thread::get_id() <<std::endl;
    QApplication a(argc, argv);

    std::function<HandlerType> init;
    if (!strcmp(argv[1],"auto")) {
        std::cout << "DONT LOAD PLUGIN" << std::endl;
    }
    else if (!strcmp(argv[1],"load")){
        std::cout << "LOAD PLUGIN" << std::endl;
        init = boost::dll::import_alias<HandlerType>(
            "src/cross-gui-driver", "init_driver",
            boost::dll::load_mode::append_decorations);
        init();
    }
    else {
        std::cout << "Unknown load type" << std::endl;
        return -1;
    }

    CustomDialog dlg;
    //dlg.walkTree();
    dlg.exec();
 
    return a.exec();
}