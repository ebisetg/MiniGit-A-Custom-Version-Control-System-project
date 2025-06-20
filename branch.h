#pragma once

#include <string>
#include <memory>

class Branch {
private:
    std::string name;
    std::string commit_hash;

public:
    Branch(const std::string& name, const std::string& commit_hash = "");
    
    // Getters
    std::string get_name() const { return name; }
    std::string get_commit_hash() const { return commit_hash; }
    
    // Setters
    void set_name(const std::string& n) { name = n; }
    void set_commit_hash(const std::string& hash) { commit_hash = hash; }
    
    // Utility methods
    std::string to_string() const;
    static std::shared_ptr<Branch> from_string(const std::string& data);
    bool is_empty() const { return commit_hash.empty(); }
};