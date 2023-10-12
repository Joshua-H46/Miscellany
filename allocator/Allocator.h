#pragma once
#include <vector>
#include <cstddef>

namespace allocator {

class FixedSizeAllocator
{
    constexpr static unsigned char N_BLOCK = 128;
    struct Chunk
    {
        Chunk(unsigned char nBlock_, std::size_t blockSize_);
	~Chunk();
        void init(unsigned char nBlock_, std::size_t blockSize_);
        unsigned char avail() const { return _avail; }
        void* allocate(std::size_t sz_);
        void deallocate(void* p_, std::size_t sz_);

        unsigned char* _data{nullptr};
        unsigned char _freeList;
        unsigned char _avail;
    };

public:
    FixedSizeAllocator(std::size_t size_);
    void init();
    void* allocate();
    void deallocate(void* p_);

private:
    Chunk* findAvailChunk();
    Chunk* findChunkToDealloc(void* p_);

private:
    std::size_t _size;
    std::vector<Chunk*> _chunks;
};

}
