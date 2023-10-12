#include "Allocator.h"
#include <math.h>
#include <cassert>
#include <iostream>

namespace allocator {

FixedSizeAllocator::Chunk::Chunk(unsigned char nBlock_, std::size_t blockSize_)
{
	std::cout << "New chunk allocated, block number: " << nBlock_ << ", block size: " << blockSize_ << std::endl;
    assert(blockSize_ > 0 && ((blockSize_ & (blockSize_ - 1)) == 0));     // blockSize_ should be 2's power
    init(nBlock_, blockSize_);
}

FixedSizeAllocator::Chunk::~Chunk()
{
	std::cout << "Chunk dtor\n";
	delete []_data;
}

void FixedSizeAllocator::Chunk::init(unsigned char nBlock_, std::size_t blockSize_)
{
    _data = new unsigned char[nBlock_ * blockSize_];
    for (int i=0; i<nBlock_; i++)
    {
        *(_data + i*blockSize_) = i+1;
    }
    _freeList = 0;
    _avail = nBlock_;
}

void* FixedSizeAllocator::Chunk::allocate(std::size_t sz_)
{
    if (_avail == 0)
    {
        return nullptr;
	} 
    _avail--;
    auto* result = _data + _freeList * sz_;
    _freeList = *result;
    return result;
}

void FixedSizeAllocator::Chunk::deallocate(void* p_, std::size_t sz_)
{
    auto* block = static_cast<unsigned char*>(p_);
    *block = _freeList;
    _freeList = (block - _data) / sz_;
    _avail++;
}

FixedSizeAllocator::FixedSizeAllocator(std::size_t size_)
		: _size(size_)
{
	std::cout << "FixedSizeAllocator ctor, block size: " << _size << std::endl;
	init();
}

void FixedSizeAllocator::init()
{
	_chunks.push_back(new Chunk(N_BLOCK, _size));
}

void* FixedSizeAllocator::allocate()
{
	// find which chunk to allcoate
	auto chunk = findAvailChunk();
	// allocate
	return chunk->allocate(_size);
}

void FixedSizeAllocator::deallocate(void* p_)
{
	auto chunk = findChunkToDealloc(p_);
	chunk->deallocate(p_, _size);
}

FixedSizeAllocator::Chunk* FixedSizeAllocator::findAvailChunk()
{
	// First we use a very naive solution
	// iterate over all chunks to find an available chunk
	for (auto& chunk : _chunks)
	{
		if (chunk->avail() > 0)
		{
			return chunk;
		}
	}
	_chunks.push_back(new Chunk(N_BLOCK, _size));
	return _chunks.back();
}

FixedSizeAllocator::Chunk* FixedSizeAllocator::findChunkToDealloc(void* p_)
{
	// First we use a very naive solution
	// iterate over all chunks to find the corresponding chunk
	for (auto& chunk : _chunks)
	{
		auto *addr = static_cast<unsigned char*>(p_);
		//std::cout << "addr: " << p_ << ", start: " << (void*)chunk->_data << ", end: " << (void*)chunk->_data + N_BLOCK * _size << std::endl;
		if (p_ >= chunk->_data && p_ < chunk->_data + N_BLOCK * _size)
		{
			return chunk;
		}
	}
	assert(false && "Failed to find valid chunk");
	return nullptr;
}

}
