/**
  * @file Selector.cpp
  * @author jason
  * @date 2022/2/19
  */

#include "core/Event.h"
#include "core/Selector.h"
#include <sys/epoll.h>

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

    static uint32_t EpollEvent(EventMode mode) {
        switch (mode) {
            case EventMode::READ:
                return EPOLLIN;
            case EventMode::WRITE:
                return EPOLLOUT;
            default:
                return 0;
        }
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
        epev.events = EpollEvent(event->Mode()) | EPOLLONESHOT;

        epoll_ctl(poller, op, event->Fildes(), &epev);
    }

    void Selector::RemoveEvent(Event *event) {
        epoll_ctl(poller, EPOLL_CTL_DEL, event->Fildes(), nullptr);
        event->Index() = -1;
        eventCount--;
    }

    Event *Selector::Wait() {
        epoll_event epev{};
        int count = epoll_wait(poller, &epev, 1, -1);

        if (count != 1) { return nullptr; }
        return static_cast<Event *>(epev.data.ptr);
    }
}