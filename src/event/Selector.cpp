/**
  * @file Selector.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "event/Selector.h"

#include <sys/epoll.h>
#include "event/Event.h"

namespace iphael {
    Selector::Selector()
            : poller{ -1 },
              eventCount{ 0 } {
        poller = epoll_create1(EPOLL_CLOEXEC);
        if (poller < 0) {
            Fetal("Create Epoll Selector Failed");
        }
    }

    Selector::~Selector() noexcept {
        if (poller >= 0) {
            close(poller);
        }
    }

    static uint32_t EpollEvent(EventModeSet mode) {
        uint32_t ret = 0;
        if (mode[event_mode::READ]) { ret |= EPOLLIN; }
        if (mode[event_mode::WRITE]) { ret |= EPOLLOUT; }
        return ret;
    }

    static EventModeSet EventModes(uint32_t epev) {
        EventModeSet ret{};
        if (epev & EPOLLIN) { ret[event_mode::READ] = true; }
        if (epev & EPOLLOUT) { ret[event_mode::WRITE] = true; }
        return ret;
    }

    void Selector::UpdateEvent(Event *event) {
        int op = EPOLL_CTL_MOD;

        if (event->Index() < 0) {
            eventCount++;
            op = EPOLL_CTL_ADD;
            event->Index() = 1;
        }

        epoll_event epev{};
        epev.data.ptr = event;
        epev.events = EpollEvent(event->EnabledModes()) | EPOLLONESHOT;

        epoll_ctl(poller, op, event->Fildes(), &epev);
    }

    void Selector::RemoveEvent(Event *event) {
        epoll_ctl(poller, EPOLL_CTL_DEL, event->Fildes(), nullptr);
        event->Index() = -1;
        eventCount--;
    }

    std::pair<Event *, EventModeSet> Selector::Wait() {
        epoll_event epev{};
        int count = epoll_wait(poller, &epev, 1, -1);

        if (count != 1) { return {nullptr, EventModeSet{}}; }
        return {
            static_cast<Event *>(epev.data.ptr),
            EventModes(epev.events)};
    }
} // iphael