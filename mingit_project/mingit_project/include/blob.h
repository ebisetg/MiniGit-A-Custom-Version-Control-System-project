#pragma once

#include <string>
#include <memory>

class Blob {
private:
    std::string hash;
    std::string content;
    std::string filename;

public:
    Blob(const std::string& content, const std::string& filename = "");
    
    // Getters
    std::string get_hash() const { return hash; }
    std::string get_content() const { return content; }
    std::string get_filename() const { return filename; }
    
    // Setters
    void set_hash(const std::string& h) { hash = h; }
    void set_content(const std::string& c) { content = c; }
    void set_filename(const std::string& f) { filename = f; }
    
    // Utility methods
    std::string to_string() const;
    static std::shared_ptr<Blob> from_string(const std::string& data);
    bool is_empty() const { return content.empty(); }
    size_t size() const { return content.size(); }
}; 
