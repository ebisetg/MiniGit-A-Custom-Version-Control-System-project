#include "blob.h"
#include "utils.h"

Blob::Blob(const std::string& content, const std::string& filename) 
    : content(content), filename(filename) {
    hash = utils::sha1_hash(content);
}

std::string Blob::to_string() const {
    std::stringstream ss;
    ss << "blob " << hash << "\n";
    ss << "filename " << filename << "\n";
    ss << "content " << content.length() << "\n";
    ss << content;
    return ss.str();
}

std::shared_ptr<Blob> Blob::from_string(const std::string& data) {
    std::vector<std::string> lines = utils::split(data, '\n');
    if (lines.size() < 4) {
        return nullptr;
    }
    
    // Parse header
    std::string blob_line = lines[0];
    std::string filename_line = lines[1];
    std::string content_length_line = lines[2];
    
    if (!blob_line.starts_with("blob ")) {
        return nullptr;
    }
    
    std::string hash = blob_line.substr(5);
    
    if (!filename_line.starts_with("filename ")) {
        return nullptr;
    }
    
    std::string filename = filename_line.substr(9);
    
    if (!content_length_line.starts_with("content ")) {
        return nullptr;
    }
    
    size_t content_length = std::stoul(content_length_line.substr(8));
    
    // Extract content
    std::string content;
    size_t content_start = data.find('\n', data.find('\n', data.find('\n') + 1) + 1) + 1;
    if (content_start < data.length()) {
        content = data.substr(content_start);
    }
    
    auto blob = std::make_shared<Blob>(content, filename);
    blob->set_hash(hash);
    return blob;
} 
