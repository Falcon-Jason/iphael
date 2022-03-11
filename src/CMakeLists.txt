add_library(iphael
        event/Event.cpp
        event/Selector.cpp
        event/EventLoop.cpp
        event/ThreadPool.cpp
        event/Signal.cpp
        coro/Coroutine.cpp
        net/InetAddress.cpp
        net/TcpSocket.cpp
        net/TcpConnection.cpp
        net/TcpListener.cpp
        net/TcpConnectionSet.cpp)

target_link_libraries(iphael fmt)