# iphael: a network programming library for C++20 coroutines

## About iphael

In C++20, there is a new feature called ``coroutine``, which
provides a method to program asynchronized code in a 
synchronized way with the newly added keywords ``co_await``,
``co_return`` and ``co_yield``.

For example, a synchronized TCP echo server (without error
handling) are usually like:
```c
extern struct inet_addr address;

void handleConnection(int fd) {
    char buffer[BUFFER_SIZE];
    
    for(;;) {
        ssize_t length = read(fd, buffer, BUFFER_SIZE);
        if (length <= 0) { return; }
        write(fd, buffer, length);
    }
}

void handleListener(int fd) {
    for(;;) {
        int connFd = accept(fd, NULL, NULL);
        handleConnection(connFd);
    }
}

int main() {
    int listenerFd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenerFd, &address, sizeof(address));
    listen(listenerFd, LISTENQ_LENGTH);
    
    handleListener(listenerFd);
}

```

After C++20, with ``coroutine``, we can program 
asynchronized code in a synchronized way, like:
```c++
Coroutine handleConnection(EventLoop &loop, Socket sock) {
    TcpConnection conn{loop, std::move(sock)};
    char buffer[BUFFER_SIZE];
    
    for(;;) {
        ssize_t length = co_await conn.read(buffer, BUFFER_SIZE);
        if (length <= 0) { co_return; }
        co_await conn.write(buffer, length);
    }
}

Coroutine handleListener(EventLoop &loop, Socket sock) {
    TcpListener listener{loop, std::move(sock)};
    
    for(;;) {
        Socket conn = co_await listener.accept();
        handleConnection(loop, std::move(conn));
    }
}

int main() {
    EventLoop loop;
    
    Socket listener = Socket::Listen(address);
    handleListener(loop, std::move(listener));
}
```

However, C++ Standard only provides a framework of coroutine.
To realize coding as above, we need a network programming
library supports coroutines, and iphael is such a library.

## Building and Usage
The project of iphael is managed by CMake. Use
```bash
mkdir build && cd build
cmake ..
```
to build this library.

## Structure of the Project

All header files are stored in the `/include` directory, 
while all source files are stored in the `/src` directory.

There are 3 modules `/event`, `/coro` and `/net`:

* `/event` module contains classes related to event
and event dispatching.

* `/coro` module mainly contains the wrapper class of
coroutine.

* `/net` modules contains the classes for network 
programming.

In `/example`, there is an example of multi-thread
asynchronized echo server, while more examples are going to
be uploaded.

## About me
I am Jason Chang, a senior, majoring in Computer Science 
and Technology. I'm interested and good at Network Programming and System 
Programming in C/C++. I am interested in all kinds of features of modern C++, 
including newly included ones. 

I am looking for a job of C++ Developer. If available, 
please give me a chance.

Here is my email: jasoncheung_0@foxmail.com