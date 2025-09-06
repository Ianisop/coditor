#include <iostream>
#include <filesystem>
#include <vector>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace FileExplorer {

struct FileInfo {
        std::string name;
        std::uintmax_t size;
        bool is_directory;
        std::time_t last_write_time;
        std::string contents; // New field for file contents
};

struct Directory {
        std::string name;
        std::vector<FileInfo> files;
};
    

std::string ReadFileContents(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return {};
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::vector<FileInfo> ListFiles(const std::string& path) {
    std::vector<FileInfo> files;
    for (const auto& entry : fs::directory_iterator(path)) {
        FileInfo info;
        info.name = entry.path().filename().string();
        info.is_directory = entry.is_directory();
        info.size = info.is_directory ? 0 : entry.file_size();
        auto ftime = entry.last_write_time();
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now()
            + std::chrono::system_clock::now()
        );
        info.last_write_time = std::chrono::system_clock::to_time_t(sctp);
        if (!info.is_directory) {
            info.contents = ReadFileContents(entry.path());
        }
        files.push_back(info);
    }
    return files;
    }

    Directory GetDirectoryInfo(const std::string& path) {
        Directory dir;
        dir.name = fs::path(path).filename().string();
        dir.files = ListFiles(path);
        return dir;
    }
}
