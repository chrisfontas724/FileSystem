// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "buffer.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace cxl {

Buffer::Buffer(uint64_t size)
    : size_(size) {
    data_ = new uint8_t[size];
    memset(data_, 0, size);
}

Buffer::Buffer(uint8_t* data, uint64_t size)
    : size_(size) {
    data_ = new uint8_t[size];
    memcpy(data_, data, size);
}

Buffer::~Buffer() {
    if(data_ != nullptr) {
        delete [] data_;
    }
}

void Buffer::resize(uint64_t size)  {
    if (size > 0) {
        size_ = size;
        data_ = static_cast<uint8_t* >(realloc(data_, size));
    }
}

} // cxl
