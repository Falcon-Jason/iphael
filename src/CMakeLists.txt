add_library(iphael
        core/Selector.cpp
        core/EventLoop.cpp
        core/Signal.cpp
        core/Event.cpp
        core/EventBuffer.cpp
        net/InetAddress.cpp
        net/TcpSocket.cpp
        coroutine/Coroutine.cpp
        coroutine/TcpConnection.cpp
        coroutine/TcpListener.cpp
        coroutine/TcpServer.cpp)

target_link_libraries(iphael fmt)