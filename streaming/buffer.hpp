// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef BUFFER_HPP_
#define BUFFER_HPP_

#include <cstdint>
#include <cstddef>

namespace cxl {
    class Buffer {
    public:
        
        Buffer(uint64_t size);
        Buffer(uint8_t* data, uint64_t size);
        ~Buffer();
        
        void resize(uint64_t size);
        uint64_t size() const { return size_; }
        uint8_t* data() const { return data_; }
        
    private:
        
        uint8_t* data_;
        uint64_t size_;
    };
}

#endif // BUFFER_HPP
