#pragma once
#include <iostream>
#include <string>

std::string slurp_file(const std::string& file_path, bool log);
std::string extract_all_occurrences(const std::string& content, const std::string& keyword, bool log);
void extract_segments(const std::string& content, std::string& errors, std::string& warnings, std::string& info, std::string& header, bool log);
