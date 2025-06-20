 #include "commit.h"
#include "utils.h"

Commit::Commit(const std::string& msg, const std::string& auth) 
    : message(msg), author(auth) {
    timestamp = std::time(nullptr);
}

void Commit::add_parent(const std::string& parent_hash) {
    if (!has_parent(parent_hash)) {
        parent_hashes.push_back(parent_hash);
    }
}

void Commit::add_file(const std::string& filename, const std::string& blob_hash) {
    file_blobs[filename] = blob_hash;
}

void Commit::remove_file(const std::string& filename) {
    file_blobs.erase(filename);
}

std::string Commit::to_string() const {
    std::stringstream ss;
    ss << "commit " << hash << "\n";
    ss << "message " << message << "\n";
    ss << "author " << author << "\n";
    ss << "timestamp " << timestamp << "\n";
    ss << "parents " << parent_hashes.size() << "\n";
    
    for (const auto& parent : parent_hashes) {
        ss << "parent " << parent << "\n";
    }
    
    ss << "files " << file_blobs.size() << "\n";
    for (const auto& [filename, blob_hash] : file_blobs) {
        ss << "file " << filename << " " << blob_hash << "\n";
    }
    
    return ss.str();
}

std::shared_ptr<Commit> Commit::from_string(const std::string& data) {
    std::vector<std::string> lines = utils::split(data, '\n');
    if (lines.size() < 5) {
        return nullptr;
    }
    
    // Parse header
    std::string commit_line = lines[0];
    std::string message_line = lines[1];
    std::string author_line = lines[2];
    std::string timestamp_line = lines[3];
    std::string parents_count_line = lines[4];
    
    if (!commit_line.starts_with("commit ")) {
        return nullptr;
    }
    
    std::string hash = commit_line.substr(7);
    
    if (!message_line.starts_with("message ")) {
        return nullptr;
    }
    
    std::string message = message_line.substr(8);
    
    if (!author_line.starts_with("author ")) {
        return nullptr;
    }
    
    std::string author = author_line.substr(7);
    
    if (!timestamp_line.starts_with("timestamp ")) {
        return nullptr;
    }
    
    std::time_t timestamp = utils::string_to_timestamp(timestamp_line.substr(10));
    
    if (!parents_count_line.starts_with("parents ")) {
        return nullptr;
    }
    
    size_t parents_count = std::stoul(parents_count_line.substr(8));
    
    auto commit = std::make_shared<Commit>(message, author);
    commit->set_hash(hash);
    commit->timestamp = timestamp;
    
    // Parse parents
    size_t line_index = 5;
    for (size_t i = 0; i < parents_count && line_index < lines.size(); ++i) {
        if (lines[line_index].starts_with("parent ")) {
            std::string parent_hash = lines[line_index].substr(7);
            commit->add_parent(parent_hash);
        }
        ++line_index;
    }
    
    // Parse files
    if (line_index < lines.size() && lines[line_index].starts_with("files ")) {
        size_t files_count = std::stoul(lines[line_index].substr(6));
        ++line_index;
        
        for (size_t i = 0; i < files_count && line_index < lines.size(); ++i) {
            if (lines[line_index].starts_with("file ")) {
                std::string file_info = lines[line_index].substr(5);
                size_t space_pos = file_info.find(' ');
                if (space_pos != std::string::npos) {
                    std::string filename = file_info.substr(0, space_pos);
                    std::string blob_hash = file_info.substr(space_pos + 1);
                    commit->add_file(filename, blob_hash);
                }
            }
            ++line_index;
        }
    }
    
    return commit;
}

bool Commit::has_parent(const std::string& parent_hash) const {
    return std::find(parent_hashes.begin(), parent_hashes.end(), parent_hash) != parent_hashes.end();
}
