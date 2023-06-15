// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef STREAM_HPP_
#define STREAM_HPP_

#include "file_system.hpp"

namespace cxl {
    class Stream {
    public:
        
        Stream();
        Stream(const FileSystem* fs, const std::string& filename);
        
        virtual ~Stream() {}
        
        virtual bool load(const FileSystem* fs, const std::string& filename) = 0;
        virtual bool save(const FileSystem* fs, const std::string& filename) = 0;
        virtual bool save() = 0;

        template<typename T>
        bool read(T& t);
        
        template <typename T>
        bool read(T* t, uint64_t count);
        
        template<typename T>
        bool read(std::vector<T>& t);
        
        template<typename T>
        bool read(std::vector<T>& t, uint64_t count);
        
        template<typename T>
        bool readInsert(std::vector<T>& t, uint64_t count);
        
        template<typename T>
        bool write(const T &t);
        
        template<typename T>
        bool write(const T* t, uint64_t count);
        
        template<typename T>
        bool write(const std::vector<T>& t);
        
    protected:
        virtual bool writeBuffer(const void* buffer, uint64_t size, uint64_t count) = 0;
        virtual bool readBuffer(void* buffer, uint64_t size, uint64_t count) = 0;
        
        const FileSystem* fileSystem_;
        std::string filename_;
    };
    
    template<typename T>
    inline bool Stream::read(T& t) {
        return readBuffer(static_cast<void*>(&t), sizeof(T), 1);
    }
    
    template<typename T>
    inline bool Stream::read(T* t , uint64_t count) {
        return readBuffer(t, sizeof(T), count);
    }
    
    template<typename T>
    inline bool Stream::read(std::vector<T>& t) {
        return readBuffer(t.data(), sizeof(T), (uint64_t)t.size());
    }
    
    template<typename T>
    inline bool Stream::read(std::vector<T>& t, uint64_t count) {
        if (t.size() < count) {
            t.resize(count);
        }
        return readBuffer(t.data(), sizeof(T), count);
    }
    
    template<typename T>
    inline bool Stream::readInsert(std::vector<T>& t, uint64_t count) {
        uint64_t size = t.size();
        t.resize(size + count);
        T* data = &t[size-1];
        return readBuffer(t.data(), sizeof(T), count);
    }
    
    template<typename T>
    inline bool Stream::write(const T& t) {
        return writeBuffer(&t, sizeof(T), 1);
    }
    
    template<typename T>
    inline bool Stream::write(const T* t, uint64_t count) {
        return writeBuffer(t, sizeof(T), count);
    }
    
    template<typename T>
    inline bool Stream::write(const std::vector<T> & t) {
        return writeBuffer(t.data(), sizeof(T), t.size());
    }
}


#endif // STREAM_HPP_
