#include "utils.h"
#include <algorithm>
#include <cstring>

namespace utils {

std::string read_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool write_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    return true;
}

bool file_exists(const std::string& filename) {
    return std::filesystem::exists(filename) && std::filesystem::is_regular_file(filename);
}

bool directory_exists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

void create_directory(const std::string& path) {
    if (!directory_exists(path)) {
        std::filesystem::create_directories(path);
    }
}

std::vector<std::string> list_files(const std::string& directory) {
    std::vector<std::string> files;
    if (!directory_exists(directory)) {
        return files;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

std::string sha1_hash(const std::string& input) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, input.c_str(), input.length());
    SHA1_Final(hash, &sha1);
    
    return hex_encode(hash, SHA_DIGEST_LENGTH);
}

std::string hex_encode(const unsigned char* data, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < length; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
    if (vec.empty()) {
        return "";
    }
    
    std::string result = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        result += delimiter + vec[i];
    }
    return result;
}

std::string timestamp_to_string(std::time_t timestamp) {
    std::stringstream ss;
    ss << timestamp;
    return ss.str();
}

std::time_t string_to_timestamp(const std::string& str) {
    return static_cast<std::time_t>(std::stoll(str));
}

std::vector<std::string> compute_diff(const std::string& old_content, const std::string& new_content) {
    std::vector<std::string> diff;
    std::vector<std::string> old_lines = split(old_content, '\n');
    std::vector<std::string> new_lines = split(new_content, '\n');
    
    // Simple line-by-line diff
    size_t max_lines = std::max(old_lines.size(), new_lines.size());
    
    for (size_t i = 0; i < max_lines; ++i) {
        if (i >= old_lines.size()) {
            diff.push_back("+ " + new_lines[i]);
        } else if (i >= new_lines.size()) {
            diff.push_back("- " + old_lines[i]);
        } else if (old_lines[i] != new_lines[i]) {
            diff.push_back("- " + old_lines[i]);
            diff.push_back("+ " + new_lines[i]);
        } else {
            diff.push_back("  " + old_lines[i]);
        }
    }
    
    return diff;
}

std::string apply_patch(const std::string& content, const std::vector<std::string>& patch) {
    std::vector<std::string> lines = split(content, '\n');
    std::vector<std::string> result;
    
    for (const auto& patch_line : patch) {
        if (patch_line.length() < 2) {
            continue;
        }
        
        char op = patch_line[0];
        std::string line_content = patch_line.substr(2);
        
        switch (op) {
            case '+':
                result.push_back(line_content);
                break;
            case '-':
                // Skip this line (remove it)
                break;
            case ' ':
                result.push_back(line_content);
                break;
        }
    }
    
    return join(result, "\n");
}

void print_success(const std::string& message) {
    std::cout << "\033[32m✓ " << message << "\033[0m" << std::endl;
}

void print_error(const std::string& message) {
    std::cout << "\033[31m✗ " << message << "\033[0m" << std::endl;
}

void print_warning(const std::string& message) {
    std::cout << "\033[33m⚠ " << message << "\033[0m" << std::endl;
}

void print_info(const std::string& message) {
    std::cout << "\033[34mℹ " << message << "\033[0m" << std::endl;
}

} // namespace utils 