// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "stream.hpp"

namespace cxl {

Stream::Stream()
    : fileSystem_(nullptr)
    , filename_("")
{}

Stream::Stream(const FileSystem *fs, const std::string &filename)
    : fileSystem_(fs)
    , filename_(filename)
{}

} // cxl
