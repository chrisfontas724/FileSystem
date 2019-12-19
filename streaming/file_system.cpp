// Copyright 2019 Sic Studios. All rights reserved.
// Use of this source code is governed by our license that can be
// found in the LICENSE file.

#include "streaming/file_system.hpp"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <climits>
#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <pwd.h>
#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#define stat _stat
#endif

#define SUCCESS_STAT 0

namespace cxl {

std::map<uint64_t, FileSystem*>  FileSystem::sub_dir_map_;

FileSystem::FileSystem(const std::string& directory)
    : directory_(directory) {
    // Make sure directory ends in a forward slash
    if (directory_.back() != '/') {
        directory_ += "/";
    }
}

FileSystem *FileSystem::getSubSystem(const std::string& directory) const {
    if (subdirectoryExists(directory)) {
        std::string fullPath = directory_ + directory;
        
        std::hash<std::string> hasher;
        int64_t hash = hasher(fullPath);
        
        if (!sub_dir_map_.count(hash)) {
            FileSystem* fs = new FileSystem(fullPath);
            assert(fs);
            sub_dir_map_[hash] = fs;
            return fs;
        } else {
            return sub_dir_map_.at(hash);
        }
    }
    return nullptr;
}


FileSystem *FileSystem::getDesktop() {
    static FileSystem desktop(desktopPath());
    return &desktop;
}


FileSystem *FileSystem::getHome() {
    static FileSystem home(homePath());
    return &home;
}


FileSystem *FileSystem::getCurrent() {
    static FileSystem current(currentPath());
    return &current;
}


const std::string FileSystem::desktopPath() {
#ifdef __APPLE__
    passwd* pw = getpwuid(getuid());
    std::string workDir(pw->pw_dir);
    workDir += "/Desktop/";
    return workDir;
#elif defined(WIN32)
    return "";
#elif defined(__linux__)
#warning Need to not hardcode
    return "/home/chris/";
#endif
}


const std::string FileSystem::homePath() {
#if defined(__APPLE__) || defined(__linux__)
    passwd* pw = getpwuid(getuid());
    std::string workDir(pw->pw_dir);
    return workDir + "/";
#else
    return "";
#endif
}


const std::string FileSystem::currentPath() {
    char buff[1000];
    getcwd( buff, 1000 );
    std::string current_working_dir(buff);
    return current_working_dir + "/";
}


const std::string& FileSystem::directory() const {
    return directory_;
}


bool FileSystem::fileExists(const std::string& filename) const {
    struct stat buf;
    return stat((directory_ + filename).c_str(), &buf) != -1;
}


bool FileSystem::subdirectoryExists(const std::string& subdirectory) const {
    return directoryExists(directory_ + subdirectory);
}


bool FileSystem::makeFile(const std::string& filename) const {
    std::fstream file;
    file.open(directory_ + filename, std::fstream::out);
    file.close();
    return fileExists(filename);
}


bool FileSystem::makeDirectory(const std::string& directory) const {
    mkdir((directory_ + directory).c_str(), 0777);
    return true;
}


bool FileSystem::deleteFile(const std::string& filename) const {
    if (remove((directory_ + filename).c_str()) != 0) {
        return false;
    }
    return true;
}


std::string FileSystem::removeExtension(const std::string& filename) {
    std::string::size_type idx = filename.rfind('.');
    if(idx != std::string::npos) {
        return filename.substr(0, idx);
    }
    return filename;
}


bool FileSystem::getExtension(const std::string& filename, std::string& extension) {
    std::string::size_type idx = filename.rfind('.');
    if (idx != std::string::npos) {
        extension = filename.substr(idx + 1, filename.length() - idx);
        return true;
    }
    return false;
}


bool FileSystem::readFile(const std::string& filename, std::string& text) const {
    if (!fileExists(filename)) {
        makeFile(filename);
    }
    
    std::ifstream stream(directory_ + filename);
    if (!stream.good()) {
        std::cerr << "Failed to load file: " << filename << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    text = buffer.str();
    return true;
}


bool FileSystem::writeFile(const std::string& filename, const std::string& text) const {
    std::ofstream file;
    file.open (directory_ + filename);
    if (!file.is_open()) {
        return false;
    }
    file << text;
    file.close();
    return true;
}


uint64_t FileSystem::timestamp(const std::string& filename) const {
    struct stat result;
    auto mod_time = ULONG_MAX;
    
    const std::string fullPath = directory_ + filename;
    if(stat(fullPath.c_str(), &result)==0) {
        mod_time = result.st_mtime;
    }
    return mod_time;
}


bool FileSystem::fileModified(const std::string& filename, uint64_t previous) const {
    return timestamp(filename) >= previous;
}


bool FileSystem::readBinary(const std::string& filename, Buffer& buffer) const {
    std::FILE *fp = fopen((directory_ + filename).c_str(), "rb");
    if(!fp) {
        std::cerr << "Could not find binary file " << filename << std::endl;
        return false;
    }
    
    std::size_t binarySize;
    fseek(fp, 0, SEEK_END);
    binarySize = ftell(fp);
    rewind(fp);
    
    buffer.resize(binarySize);
    std::size_t readsize = fread(buffer.data(), 1, binarySize, fp);
    return readsize == binarySize;
}


bool FileSystem::writeBinary(const std::string& filename, const Buffer& buffer) const
{
    FILE *fp = fopen((directory_ + filename).c_str(), "wb");
    if (!fp) {
        return false;
    }
    
    fwrite(buffer.data(), 1, buffer.size(), fp);
    fclose(fp);

    return true;
}


std::vector<std::string> FileSystem::getSubDirectories() {
    std::vector<std::string> result;
    DIR *dir = opendir(directory_.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            if(strlen(entry->d_name) > 2) {
                result.push_back(entry->d_name);
            }
        }
        entry = readdir(dir);
    }
    return result;
}


bool FileSystem::directoryExists(const std::string& dir) {
    try {
        struct stat info;
        if (stat(dir.c_str(), &info ) != 0)
            return 0;
        else if(info.st_mode & S_IFDIR)
            return 1;
        else
            return 0;
    } catch(...) {
        std::cerr << "Stat failed " << dir << std::endl;
        return false;
    }
}


int32_t FileSystem::clearAllFiles(bool recursive) {
    return __clearAllFiles(directory_, recursive);
}


int32_t FileSystem::__clearAllFiles(const std::string& dirpath, bool recursive) {
    if (dirpath.empty()) {
        return SUCCESS_STAT;
    }

    DIR *theFolder = opendir(dirpath.c_str());
    struct dirent *next_file;
    char filepath[1024];
    int ret_val;

    if (theFolder == NULL) {
        return errno;
    }

    while ( (next_file = readdir(theFolder)) != NULL ) {
        // Build the path for each file in the folder.
        sprintf(filepath, "%s/%s", dirpath.c_str(), next_file->d_name);

        //we don't want to process the pointer to "this" or "parent" directory.
        if ((strcmp(next_file->d_name,"..") == 0) ||
            (strcmp(next_file->d_name,"." ) == 0) ) {
            continue;
        }

        // dirExists will check if the "filepath" is a directory.
        if (directoryExists(filepath)) {
            if (!recursive) {
                // If we aren't recursively deleting in subfolders, skip this dir.
                 continue;
            }

            ret_val = __clearAllFiles(filepath, recursive);

            if (ret_val != SUCCESS_STAT) {
                closedir(theFolder);
                return ret_val;
            }
        }

        ret_val = remove(filepath);
        //ENOENT occurs when i folder is empty, or is a dangling link, in
        //which case we will say it was a success because the file is gone
        if (ret_val != SUCCESS_STAT && ret_val != ENOENT) {
            closedir(theFolder);
            return ret_val;
        }

    }

    closedir(theFolder);

    return SUCCESS_STAT;
}
} // cxl
