#include "onip/utils/pool.hpp"

#include <stdio.h>
#include <assert.h>

namespace onip {
    Pool::Pool(size_t getChunkSize, size_t getBlockSize)
        : m_chunk_size(getChunkSize), m_block_size(getBlockSize) {
        assert(m_chunk_size > 0 && "Failed to create Memory/Object Pool as _chunk_size is set to 0 which doesn't make any sense");
        assert(m_block_size > 0 && "Failed to create Memory/Object Pool as _block_size is set to 0 which doesn't make any sense");
    }

    Pool::~Pool() {
        if (m_blocks.size() > 0) {
            for (void* block : m_blocks) {
                free(block);
            }

            m_chunk_size = 0;
            m_block_size = 0;
            m_next = nullptr;
            m_tail = nullptr;
        }
    }

    Pool::Iterator Pool::begin() {
        return Iterator(static_cast<Chunk*>(*m_blocks.begin()));
    }

    Pool::Iterator Pool::end() {
        return Iterator(nullptr);
    }

    void* Pool::allocateData() {
        void* allocation = nullptr;
        if (m_released.size() > 0) {
            allocation = m_released[0];
            m_released.erase(m_released.begin());
        }
        else {
            if (m_next == nullptr) {
                m_next = allocateBlock();
            }

            Byte* byte_data = reinterpret_cast<Byte*>(m_next);
            allocation = static_cast<void*>(byte_data + sizeof(Chunk));
            m_next = m_next->next;
        }
        m_allocations++;
        return allocation;
    }

    void Pool::releaseData(void* ptr) {
        Byte* byte = static_cast<Byte*>(ptr);
        *byte = null();
        m_released.push_back(ptr);
        m_allocations--;
    }

    Pool::Chunk* Pool::allocateBlock() {
        size_t getBlockSize = (sizeof(Chunk) + m_chunk_size) * m_block_size;
        Chunk* block = static_cast<Chunk*>(malloc(getBlockSize));
        assert(block && "Failed to allocate memory block for Memory/Object Pool as OS ran out of memory");

        Chunk* chunk = block;
        size_t size = sizeof(Chunk) + m_chunk_size;
        for (size_t i = 1; i < m_block_size; i++) {
            setChunkDefault(chunk, size);
            chunk = chunk->next;
        }

        setChunkDefault(chunk, size);
        chunk->next = nullptr;

        block->prev = m_tail;
        if (m_tail != nullptr) {
            m_tail->next = block;
        }

        m_tail = chunk;
        m_blocks.push_back(block);
        return block;
    }

    void Pool::setChunkDefault(Chunk* chunk, const size_t& size) {
        Byte* byte_data = reinterpret_cast<Byte*>(chunk);
        byte_data[sizeof(Chunk)] = null();

        chunk->next = reinterpret_cast<Chunk*>(byte_data + size);
        chunk->prev = reinterpret_cast<Chunk*>(byte_data - size);
    }
}