#pragma once
#include <cstdlib>
#include <string.h>
#include <cassert>
#include <algorithm>

namespace cache_reader
{
    template<unsigned long BlockSize>
    class BlockReader
    {
    public:
        BlockReader(const char* str_, size_t size_) : _data(str_), _size(size_)
        {
            assert(str_ != nullptr);
        }

        int read(char* dst_)
        {
            size_t sz = std::min(_size, BlockSize);
            memcpy(dst_, _data, sz);
            _data += sz;
            _size -= sz;
            return sz;
        }

    private:
        const char* _data;
        size_t _size;
    };

    class CacheReader
    {
    public:
        CacheReader(const char* data_, size_t size_) : _br(data_, size_)
        {
        }

        int read(char* dst_, size_t sz_)
        {
            int readSz = 0;
            if (_cacheSize > 0)
            {
                memcpy(dst_, _cache + _pos, std::min(sz_, _cacheSize));
                if (sz_ > _cacheSize)
                {
                    readSz += _cacheSize;
                    sz_ -= _cacheSize;
                    _cacheSize = 0;
                }
                else
                {
                    readSz += sz_;
                    _pos += sz_;
                    _cacheSize -= sz_;
                    sz_ = 0;
                    return readSz;
                }
            }
            while (sz_ >= BlockSize)
            {
                auto sz = _br.read(dst_+readSz);
                readSz += sz;
                sz_ -= sz;
                if (sz < BlockSize)
                {
                    return readSz;
                }
            }
            if (sz_ > 0)
            {
                auto sz = _br.read(_cache);
                _pos = 0;
                _cacheSize = sz;
                auto realSz = std::min(sz_, _cacheSize);
                memcpy(dst_+readSz, _cache, realSz);
                _pos += realSz;
                _cacheSize -= realSz;
                sz_ -= realSz;
                readSz += realSz;
            }
            return readSz;
        }

    private:
        static constexpr size_t BlockSize = 8;
        BlockReader<BlockSize> _br;
        size_t _pos{0};
        size_t _cacheSize{0};
        char _cache[BlockSize];
    };
} // namespace cache_reader
