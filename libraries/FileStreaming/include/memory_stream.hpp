// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef MEMORY_STREAM_HPP_
#define MEMORY_STREAM_HPP_

#include "stream.hpp"
#include "buffer.hpp"
#include <string>

namespace cxl {
    class MemoryStream : public Stream {
    public:
        // Constructors.
        MemoryStream(uint64_t capacity = 0);
        MemoryStream(uint8_t* data, uint64_t size);

        // Destructor.
        ~MemoryStream(){};
        
        bool load(const FileSystem* fs, const std::string& filename) override;
        bool save(const FileSystem* fs, const std::string& filename) override;
        bool save() override;
        
        bool writeString(const std::string& str);
        bool readString(std::string& str);
        
        void rewind();
        
    private:
        bool writeBuffer(const void* buffer, uint64_t size, uint64_t count) override;
        bool readBuffer(void* buffer, uint64_t size, uint64_t count) override;
        Buffer buffer_;
        uint64_t position_;
    };
}

#endif // MEMORY_STREAM_HPP_
