/**
  * @file EventArgument.h
  * @author Jason
  * @date 2022/3/4
  */

#pragma once
#include "core/Event.h"
#include <memory>

namespace iphael {
    class Event::Argument {
    private:
        std::variant<
                std::nullptr_t,
                std::unique_ptr<SingleBufferArgument>,
                std::unique_ptr<MultiBufferArgument>
        > content{nullptr};

    public:
        Argument() = default;

        Argument(nullptr_t) : Argument() {}

        Argument(void *buffer, size_t length) {
            Set(buffer, length);
        }

        bool operator==(nullptr_t) const {
            return content.index() == 0;
        }

        /**
         * @tparam T the type of content to be get
         * @return The pointer to content if matches, or nullptr if not matched.
         */
        template<class T>
        T *Get() { return std::get_if<std::unique_ptr<T>>(&content)->get(); }

        /**
         * Set this as null
         */
        void Set(std::nullptr_t);

        /**
         * Set this as a single buffer argument,
         * for read, write operations
         * @param buffer the data to be read/written
         * @param length the length of buffer
         */
        void Set(void *buffer, size_t length);

        /**
         * @return the returned length of operation bind to this argument.
         */
        ssize_t GetReturnedLength();
    };

    /**
     * @struct SingleBufferArgument
     * argument for single buffer i/o (read/write)
     */
    struct Event::SingleBufferArgument {
        char *buffer;
        size_t length;
        ssize_t lengthR;

        SingleBufferArgument(void *buffer, size_t length);

        ~SingleBufferArgument();
    };

    // TODO: NOT IMPLEMENTED
    struct Event::MultiBufferArgument {
        MultiBufferArgument();

        ~MultiBufferArgument();
    };
} // iphael
