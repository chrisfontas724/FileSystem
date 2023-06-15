// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "file_stream.hpp"
#include <istream>
#include <iostream>

namespace cxl {

FileStream::FileStream() : Stream() {}

FileStream::FileStream(const FileSystem* fs, const std::string& filename)
           :Stream(fs, filename) {
    load(fs, filename);
}

bool FileStream::load(const FileSystem* fs, const std::string& filename) {
    if (!fs || filename.length() <= 0) {
        return false;
    }
    
    if (!fs->fileExists(filename)) {
        return false;
    }
    
    fileSystem_ = fs;
    filename_ = filename;
    
    std::string result;
    if (fs->readFile(filename, result)) {
        stream_.str(result);
        return true;
    }
    
    return false;
}

bool FileStream::save(const FileSystem* fs, const std::string& filename) {
    if (!fs || filename.length() <= 0)
        return false;
    return fs->writeFile(filename, text());
}

bool FileStream::save() {
    return save(fileSystem_, filename_);
}

void FileStream::set_text(const std::string& text) {
    stream_.str(text);
}

std::string FileStream::text() {
    std::stringstream buffer;
    buffer << stream_.rdbuf();
    stream_.seekg(0, std::ios::beg);
    return buffer.str();
}

bool FileStream::getLine(std::string & line) {
    return static_cast<bool>(std::getline(stream_, line));
}

void FileStream::writeLine(const std::string& line) {
    stream_ << line;
}

// TODO: Fill this out.
bool FileStream::writeBuffer(const void* buffer, uint64_t size, uint64_t count) {
    stream_ << buffer; 
    return true;
}

// TODO: Fill this out.
bool FileStream::readBuffer(void* buffer, uint64_t size, uint64_t count) {
    return true;
}

} // cxl
