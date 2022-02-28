/**
  * @file EventArgument.h
  * @author jason
  * @date 2022/2/13
  */

#pragma once

#include <variant>

namespace iphael {
    /**
     * @struct SingleBufferArgument
     * argument for single buffer i/o (read/write)
     */
    struct SingleBufferArgument {
        char *buffer;
        size_t length;
        ssize_t lengthR;

        SingleBufferArgument(void *buffer, size_t length)
                : buffer{static_cast<char *>(buffer)},
                  length{length}, lengthR{0} {
        }
    };

    /**
     * @struct MultiBufferArgument
     * argument for multiple buffers i/o (readv/writev)
     */
    struct MultiBufferArgument {
        // TODO: NOT IMPLEMENTED
    };

    /**
     * @struct AsyncAcceptArgument
     * argument for async accept operation
     */
    struct AsyncAcceptArgument {
        int fildes{-1};
    };

    using EventArgument = std::variant<
            nullptr_t,
            SingleBufferArgument,
            MultiBufferArgument,
            AsyncAcceptArgument>;

} // iphael
