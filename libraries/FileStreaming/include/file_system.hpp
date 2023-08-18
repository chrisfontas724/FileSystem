// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#ifndef STREAMING_FILE_SYSTEM_HPP_
#define STREAMING_FILE_SYSTEM_HPP_

#include "buffer.hpp"
#include <string>
#include <vector>
#include <map>

namespace cxl {
class FileSystem {
public:
        
    FileSystem(const std::string& directory);
        
    FileSystem* getSubSystem(const std::string& directory) const;
        
    bool fileExists(const std::string& filename) const;
    bool subdirectoryExists(const std::string& subdirectory) const;
        
    bool makeFile(const std::string& filename) const;
    bool makeDirectory(const std::string& directory) const;
        
    bool deleteFile(const std::string& filename) const;
    
    bool readFile(const std::string& filename, std::string& text) const;
    bool writeFile(const std::string& filename, const std::string& text) const;
        
    uint64_t timestamp(const std::string& filename) const;
    bool fileModified(const std::string& filename, uint64_t previous) const;
        
    bool readBinary(const std::string& filename, Buffer& buffer) const;
    bool writeBinary(const std::string& filename, const Buffer& buffer) const;
        
    int32_t clearAllFiles(bool recursive = false);

    std::vector<std::string> getSubDirectories();
        
    const std::string& directory() const;
        
    // Static initializers
    static FileSystem* getHome();
    static FileSystem* getDesktop();
    static FileSystem* getCurrent();
        
    static std::string removeExtension(const std::string& filename);
    static bool getExtension(const std::string& filename, std::string& extension);
        
    // Quick functions to get common paths.
    static const std::string desktopPath();
    static const std::string homePath();
    static const std::string currentPath();
    static const std::string currentExecutablePath();
    static bool directoryExists(const std::string& dir);
        
private:
    static std::map<uint64_t, FileSystem*> sub_dir_map_;

    std::string directory_;
	int32_t __clearAllFiles(const std::string& dirpath, bool recursive);
};
} // cxl

#endif // STREAMING_FILE_SYSTEM_HPP_
