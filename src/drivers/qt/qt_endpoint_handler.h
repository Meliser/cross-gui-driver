#pragma once
#include "endpoint_handler_i.h"
#include <QObject>
#include <memory>
#include <QEvent>
#include <QSemaphore>
#include <thread>
#include <future>
#include <variant>
#include <qobjectdefs.h>

#include <boost/dll/alias.hpp>

class GuiSyncher : public QObject
{
Q_OBJECT
public:
    GuiSyncher(int event_type) : m_event_type(event_type)
    {}
    bool event(QEvent *event) override;
    int get_event_type() const { return m_event_type; }
private:
    int m_event_type;
};


class TaskEvent : public QEvent
{
public:
    using TaskType = std::packaged_task<void()>;
    TaskEvent(int t, std::shared_ptr<TaskType> c) :
                                                 QEvent(QEvent::Type(t)),
                                                 m_c(std::move(c))
                                                 {}
    void operator()()
    {
        (*m_c)();
    }
private:
    std::shared_ptr<TaskType> m_c;
};

class QtEndpointHandler : public EndpointHandlerI
{
public:
    bool init() override;
    std::string getText(const std::string& name) override;
private:
    void post_and_wait(std::function<void(void)> f);
private:
    std::unique_ptr<GuiSyncher> m_gui_syncher;
};

std::unique_ptr<EndpointHandlerI> create();

BOOST_DLL_ALIAS(
    create,
    create_handler
)