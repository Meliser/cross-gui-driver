#include "qt_endpoint_handler.h"
#include "endpoint_handler_i.h"
#include <chrono>
#include <iostream>

#include <QString>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <QThread>
#include <QAbstractButton>
#include <QTextStream>
#include <memory>
#include <qnamespace.h>
#include <qobjectdefs.h>


std::unique_ptr<EndpointHandlerI> create(void* config)
{
    return std::make_unique<QtEndpointHandler>(config);
}

QWidget* findWidget(const std::string& name)
{
    QString qname = QString::fromStdString(name);
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

QtEndpointHandler::QtEndpointHandler(void*)
{
    std::cout << "QtEndpointHandler" << std::endl;
    int event_type = QEvent::registerEventType();
    m_gui_syncher.reset(new GuiSyncher{event_type});
}

void QtEndpointHandler::post_and_wait(std::function<void(void)> f)
{
    auto task = std::make_shared<TaskEvent::TaskType>(f);
    QEvent* te = new TaskEvent(m_gui_syncher->get_event_type(), task);
    QCoreApplication::postEvent(m_gui_syncher.get(), te);
    task->get_future().wait();
}

std::string QtEndpointHandler::getText(const std::string& name)
{
    std::cout << "post event in thread " << std::this_thread::get_id() <<std::endl;

    std::string res;
    auto f = [&res, &name]()
    {
        if (QWidget* w = findWidget(name)) {
            QVariant v = w->property("text");
            res = v.toString().toStdString();
        }
        else {
            std::cout << "Widget for name " << name << " not found" << std::endl;
        }
    };

    post_and_wait(f);
    return res;
}

std::string QtEndpointHandler::getTitle(const std::string& name)
{
    std::string res;
    auto f = [&res, &name]()
    {
        if (QWidget* w = findWidget(name); w && w->isWindow()) {
            res = w->windowTitle().toStdString();
            return;
        }
    };

    post_and_wait(f);
    //post_and_wait(std::bind(&QtEndpointHandler::nameAll, this));
    return res;
}

std::optional<WidgetRect> QtEndpointHandler::getWidgetRect(const std::string& name)
{
    std::optional<WidgetRect> opt_rect;
    auto f = [&opt_rect, &name]()
    {
        if (QWidget* w = findWidget(name); w) {
            QRect qrect = w->geometry();
            WidgetRect rect;
            rect.height = qrect.height();
            rect.width = qrect.width();
            rect.x = qrect.x();
            rect.y = qrect.y();
            opt_rect = std::move(rect);
            return;
        }
    };

    post_and_wait(f);
    return opt_rect;
}

void QtEndpointHandler::nameAll()
{
    static unsigned level_ctr[100] = {};
    static unsigned preset_name_ctr = 0;
    const QString root_prefix = "auto_";
    for (QWidget *widget : QApplication::allWidgets()) {
        QWidget* top = widget;
        for (QWidget* parent = widget->parentWidget(); parent; parent = parent->parentWidget()) {
            top = parent;
        }

        //std::cout << "Take top widget with name " << top->objectName().toStdString() << std::endl;
        int level = 0;
        QWidgetList wl = { top };
        while (true) {
            QWidgetList ch_wl;
            for (QWidget* w : wl) {
                if (w->objectName().isEmpty()) {
                    QString name;
                    QTextStream strm(&name);
                    QString name_prefix = w->parentWidget() ? (w->parentWidget()->objectName() + '.') : root_prefix;
                    unsigned ctr = name_prefix.startsWith(root_prefix) ? level_ctr[level]++ : preset_name_ctr++;
                    //std::cout << "level " << level << " parent " << w->parentWidget() << std::endl;
                    strm << name_prefix << ctr;
                    w->setObjectName(name);
                    w->setToolTip(name);
                    std::cout << "Set object name to " << name.toStdString() << " for w " << w << std::endl;
                }
                else if (w->toolTip().isEmpty()) {
                    w->setToolTip(w->objectName());
                }
                else if (w->objectName().startsWith(root_prefix)) {
                    break;
                }

                for (QObject* ch_obj : w->children()) {
                    if (QWidget* ch_w = dynamic_cast<QWidget*>(ch_obj)) {
                        ch_wl.append(ch_w);
                    }
                }
            }
            if (ch_wl.isEmpty()) {
                break;
            }
            wl = std::move(ch_wl);
            level++;
        }
    }
}