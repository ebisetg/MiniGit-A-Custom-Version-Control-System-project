#include "branch.h"
#include "utils.h"

Branch::Branch(const std::string& name, const std::string& commit_hash) 
    : name(name), commit_hash(commit_hash) {
}

std::string Branch::to_string() const {
    std::stringstream ss;
    ss << "branch " << name << "\n";
    ss << "commit " << commit_hash << "\n";
    return ss.str();
}

std::shared_ptr<Branch> Branch::from_string(const std::string& data) {
    std::vector<std::string> lines = utils::split(data, '\n');
    if (lines.size() < 2) {
        return nullptr;
    }
    
    std::string branch_line = lines[0];
    std::string commit_line = lines[1];
    
    if (!branch_line.starts_with("branch ")) {
        return nullptr;
    }
    
    std::string name = branch_line.substr(7);
    
    if (!commit_line.starts_with("commit ")) {
        return nullptr;
    }
    
    std::string commit_hash = commit_line.substr(7);
    
    return std::make_shared<Branch>(name, commit_hash);
}