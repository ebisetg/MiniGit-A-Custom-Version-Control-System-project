
#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <openssl/sha.h>
#include <iomanip>

namespace utils {
    // File operations
    std::string read_file(const std::string& filename);
    bool write_file(const std::string& filename, const std::string& content);
    bool file_exists(const std::string& filename);
    bool directory_exists(const std::string& path);
    void create_directory(const std::string& path);
    std::vector<std::string> list_files(const std::string& directory);
    
    // Hashing
    std::string sha1_hash(const std::string& input);
    std::string hex_encode(const unsigned char* data, size_t length);
    
    // String operations
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
    
    // Time operations
    std::string timestamp_to_string(std::time_t timestamp);
    std::time_t string_to_timestamp(const std::string& str);
    
    // Diff operations
    std::vector<std::string> compute_diff(const std::string& old_content, const std::string& new_content);
    std::string apply_patch(const std::string& content, const std::vector<std::string>& patch);
    
    // Color output (for terminal)
    void print_success(const std::string& message);
    void print_error(const std::string& message);
    void print_warning(const std::string& message);
    void print_info(const std::string& message);
} 