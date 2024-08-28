#include <iostream>
#include "strings.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
#include "readlog.h"

std::string slurp_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "file does not exist or cannot be opened: " << file_path << std::endl;
        std::perror("std::fstream");
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
}

std::string extract_all_occurrences(const std::string& content, const std::string& keyword) {
    std::string result;
    size_t pos = content.find(keyword);
    while (pos != std::string::npos) {
        size_t end_pos = content.find("\n", pos);
        if (end_pos == std::string::npos) {
            end_pos = content.length();
        }
        result += content.substr(pos, end_pos - pos + 1);
        pos = content.find(keyword, end_pos);
    }
    return result;
}

void extract_segments(const std::string& content, std::string& errors,
    std::string& warnings, std::string& info, std::string& header) {
    size_t head_end = content.find("Loading  TRK TDB");
    if (head_end != std::string::npos) {
        header = content.substr(0, head_end + std::string("Loading  TRK TDB").length());
    }
    errors = extract_all_occurrences(content, "Error:");
    warnings = extract_all_occurrences(content, "Warning:");
    info = extract_all_occurrences(content, "Information:");
}