#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace FileExplorer {
    struct FileInfo {
        std::string name;
        std::uintmax_t size;
        bool is_directory;
        time_t last_write_time;
        std::string contents; // New field for file contents
    };

    struct Directory {
        std::string name;
        std::vector<FileInfo> files;
    };

    std::string ReadFileContents(const std::filesystem::path& path);
    std::vector<FileInfo> ListFiles(const std::string& path);
    Directory GetDirectoryInfo(const std::string& path);
}
