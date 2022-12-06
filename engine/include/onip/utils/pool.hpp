#ifndef __ONIP_UTILS_POOL_HPP__
#define __ONIP_UTILS_POOL_HPP__

#include "onip/utils/utils.hpp"

#include <list>
#include <vector>

namespace onip {
    class Pool {
    public:
        struct Chunk {
            Chunk* next;
            Chunk* prev;
        };

        class Iterator {
        public:
            Iterator(Chunk* ptr)
                : m_ptr(ptr) {}

            ONIP_INLINE Iterator& operator++() { m_ptr = m_ptr->next; return *this; }
            ONIP_INLINE Iterator& operator++(int) { Iterator it = *this; ++(*this); return it; }
            ONIP_INLINE Iterator& operator--() { m_ptr = m_ptr->prev; return *this; }
            ONIP_INLINE Iterator& operator--(int) { Iterator it = *this; --(*this); return it; }

            ONIP_INLINE bool operator==(const Iterator& other) { return m_ptr == other.m_ptr; }
            ONIP_INLINE bool operator!=(const Iterator& other) { return m_ptr != other.m_ptr; }

            ONIP_INLINE void* operator*() {
                Byte* byte_data = reinterpret_cast<Byte*>(m_ptr);
                byte_data = byte_data + sizeof(Chunk);
                return static_cast<void*>(byte_data);
            }

            ONIP_INLINE void* operator->() { return static_cast<void*>(**this); }

        private:
            Chunk* m_ptr = nullptr;
        };

        Pool() = default;
        Pool(size_t chunkSize, size_t blockSize);
        ~Pool();

        ONIP_INLINE static constexpr Byte null() { return static_cast<Byte>(-1); }
        ONIP_INLINE static bool is_null(void* ptr) { return *static_cast<Byte*>(ptr) == null(); }

        ONIP_INLINE size_t chunk_size() const { return m_chunk_size; }
        ONIP_INLINE size_t block_size() const { return m_block_size; }
        ONIP_INLINE size_t size() const { return m_blocks.size() * m_block_size; }
        ONIP_INLINE size_t size_in_bytes() const { return (sizeof(Chunk) + m_chunk_size) * m_blocks.size() * m_block_size; }
        ONIP_INLINE size_t block_allocations() const { return m_blocks.size(); }
        ONIP_INLINE size_t allocations() const { return m_allocations; }
        
        Iterator begin();
        Iterator end();

        void* allocate();
        void release(void* ptr);

        template <typename _Object>
        _Object* allocate() {
            _Object* object = static_cast<_Object*>(allocate());
            new (object) _Object{};
            return object;
        }

        template <typename _Object>
        void release(_Object* ptr) {
            ptr->~_Object();
            release(static_cast<void*>(ptr));
        }
    private:
        Chunk* allocate_block();
        void set_chunk_default(Chunk* chunk, const size_t& size);

        size_t m_chunk_size = 0;
        size_t m_block_size = 0;
        size_t m_allocations = 0;
        Chunk* m_next = nullptr;
        Chunk* m_tail = nullptr;
        std::list<void*> m_blocks;
        std::vector<void*> m_released;
    };
}

#endif // __ONIP_UTILS_POOL_HPP__