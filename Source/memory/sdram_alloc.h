#pragma once
#ifndef INFS_SDRAM_ALLOC_H
#define INFS_SDRAM_ALLOC_H

#include "sdram_pool.h"
#include <atomic>
#include <new>

namespace infrasonic
{
    class SDRAM {

        public:
            template<typename T, class... Args>
            static T* allocate(Args &&... args)
            {
                // TODO: handle nullptr if exceeded limits
                auto ptr = instance().allocate_raw(sizeof(T), alignof(T));
                return new (ptr) T(std::forward<Args>(args)...);
            }

        private:
            SDRAM() = default;
            ~SDRAM() = default;

            static SDRAM& instance() {
                static SDRAM sdram;
                return sdram;
            }

            void* allocate_raw(size_t size, size_t alignment);

            std::atomic<size_t> pool_pos_ = 0;
    };
    
} // namespace infrasonic

#endif