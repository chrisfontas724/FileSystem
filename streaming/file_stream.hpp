// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef FILE_STREAM_HPP_
#define FILE_STREAM_HPP_

#include "streaming/stream.hpp"
#include <sstream>

namespace cxl {
    class FileStream : public Stream {
    public:
        FileStream();
        FileStream(const FileSystem* fs, const std::string& filename);
        bool load(const FileSystem* fs, const std::string& filename) override;
        bool save(const FileSystem* fs, const std::string& filename) override;
        bool save() override;
        
        void set_text(const std::string& text);
        std::string text();
        
        void writeLine(const std::string& line);
        bool getLine(std::string& line);
        
    private:
        std::stringstream stream_;
        bool writeBuffer(const void* buffer, uint64_t size, uint64_t count) override;
        bool readBuffer(void* buffer, uint64_t size, uint64_t count) override;   
};
}

#endif // FILE_STREAM_HPP_
