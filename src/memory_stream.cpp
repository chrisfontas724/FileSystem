// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "streaming/memory_stream.hpp"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace cxl {

MemoryStream::MemoryStream(uint64_t capacity)
    : Stream()
    , buffer_(capacity)
    , position_(0){}

MemoryStream::MemoryStream(uint8_t* data, uint64_t size)
    : Stream()
    , buffer_(data, size)
    , position_(0)
{}

bool MemoryStream::load(const FileSystem *fs, const std::string& filename) {
    if (!fs || filename.length() <= 0)
        return false;
    
    filename_ = filename;
    if( !fs->readBinary(filename, buffer_)) {
        return false;
    }
    return true;
}

bool MemoryStream::save(const FileSystem *fs, const std::string& filename) {
    if( !fs->writeBinary(filename, buffer_)) {
        return false;
    }
    return true;
}

bool MemoryStream::save() {
    return true;
}

void MemoryStream::rewind() {
    position_ = 0;
}

bool MemoryStream::writeString(const std::string& str) {
    uint32_t length = (uint32_t)str.length();
    write(length);
    write(str.c_str(), length + 1);
    return true;
}

bool MemoryStream::readString(std::string& str) {
    uint32_t length;
    read(length);
    char result[length + 1];
    read(result, (uint32_t)length + 1);
    str = std::string(result);
    return true;
}

bool MemoryStream::readBuffer(void* buffer, uint64_t size, uint64_t count) {
    uint32_t requested = size * count;
    uint32_t available = (uint32_t)buffer_.size() - position_;
    uint32_t toRead = std::min(requested, available);
    
    if (toRead > 0) {
        memcpy(buffer, buffer_.data() + position_, toRead);
        position_ += toRead;
        return true;
    }
    
    return false;
}

bool MemoryStream::writeBuffer(const void* buffer, uint64_t size, uint64_t count) {
    uint32_t toWrite = size * count;
    if (toWrite == 0 || buffer == nullptr) {
        return false;
    }

    uint32_t total = position_ + toWrite;
    if (total > buffer_.size()) {
        buffer_.resize(total);
    }
    
    memcpy(buffer_.data() + position_, buffer, toWrite);
    position_ += toWrite;
    return true;
}

} // cxl