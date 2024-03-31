#include "qt_endpoint_handler.h"
#include <chrono>
#include <iostream>

#include <QString>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QThread>
#include <QAbstractButton>
#include <memory>
#include <qnamespace.h>
#include <qobjectdefs.h>


std::unique_ptr<EndpointHandlerI> create()
{
    return std::make_unique<QtEndpointHandler>();
}

QWidget* findWidget(const QString& qname)
{
    for (QWidget *widget : QApplication::allWidgets()) {
        if (widget->objectName() == qname) {
            std::cout << "FOUND NAME" << std::endl;
            return widget;
        }
    }
    return nullptr;
}


bool GuiSyncher::event(QEvent* event)
{
    if (event->type() == m_event_type) {
        TaskEvent* te = static_cast<TaskEvent*>(event);
        std::cout << "some event in thread " << std::this_thread::get_id() <<std::endl;
        (*te)();
        return true;
    }
    return false;
}

bool QtEndpointHandler::init()
{
    std::cout << "QtEndpointHandler::init()" << std::endl;
    int event_type = QEvent::registerEventType();
    m_gui_syncher.reset(new GuiSyncher{event_type});
    return true;
}

std::string QtEndpointHandler::getText(const std::string& name)
{
    std::cout << "post event in thread " << std::this_thread::get_id() <<std::endl;

    std::string res;
    auto f = [&res](const std::string& name)
    {
        QString qname = QString::fromStdString(name);
        if (QWidget* w = findWidget(qname)) {
            QVariant v = w->property("text");
            res = v.toString().toStdString();
        }
        else {
            std::cout << "Widget for name " << name << " not found" << std::endl;
        }
    };

    auto task = std::make_shared<TaskEvent::TaskType>(std::bind(f, name));
    QEvent* te = new TaskEvent(m_gui_syncher->get_event_type(), task);
    QCoreApplication::postEvent(m_gui_syncher.get(), te);
    task->get_future().wait();
    std::cout << "NOW RETURN RES" << std::endl;
    return res;
}
