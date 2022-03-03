set(SELECTOR
        core/eventloop/selector/Selector.cpp
        core/eventloop/selector/EpollSelector.cpp)

set(EVENTLOOP
        core/EventLoop.cpp
        core/eventloop/ReactorEventLoop.cpp)

add_library(iphael
        ${SELECTOR}
        ${EVENTLOOP}
        core/Signal.cpp
        core/Event.cpp
        net/InetAddress.cpp
        net/TcpSocket.cpp
        coroutine/Coroutine.cpp
        coroutine/TcpConnection.cpp
        coroutine/TcpListener.cpp
        coroutine/TcpServer.cpp)

target_link_libraries(iphael fmt)