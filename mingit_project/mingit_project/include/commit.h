#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>

class Commit {
private:
    std::string hash;
    std::string message;
    std::string author;
    std::time_t timestamp;
    std::vector<std::string> parent_hashes;
    std::map<std::string, std::string> file_blobs; // filename -> blob_hash

public:
    Commit(const std::string& msg, const std::string& auth = "user");
    
    // Getters
    std::string get_hash() const { return hash; }
    std::string get_message() const { return message; }
    std::string get_author() const { return author; }
    std::time_t get_timestamp() const { return timestamp; }
    std::vector<std::string> get_parents() const { return parent_hashes; }
    std::map<std::string, std::string> get_files() const { return file_blobs; }
    
    // Setters
    void set_hash(const std::string& h) { hash = h; }
    void add_parent(const std::string& parent_hash);
    void add_file(const std::string& filename, const std::string& blob_hash);
    void remove_file(const std::string& filename);
    
    // Utility methods
    std::string to_string() const;
    static std::shared_ptr<Commit> from_string(const std::string& data);
    bool has_parent(const std::string& parent_hash) const;
    bool is_merge_commit() const { return parent_hashes.size() > 1; }
    bool is_initial_commit() const { return parent_hashes.empty(); }
}; 
