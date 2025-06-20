#include "minigit.h"
#include "utils.h"
#include <algorithm>
#include <set>

MiniGit::MiniGit(const std::string& path) 
    : repo_path(path), is_initialized(false) {
    minigit_path = repo_path + "/.minigit";
    objects_path = minigit_path + "/objects";
    refs_path = minigit_path + "/refs";
    head_path = minigit_path + "/HEAD";
    
    // Check if already initialized
    if (utils::directory_exists(minigit_path)) {
        is_initialized = true;
        current_branch = "main";
        
        // Load existing branches
        std::vector<std::string> branch_files = utils::list_files(refs_path);
        for (const auto& branch_file : branch_files) {
            std::string branch_name = branch_file;
            std::string branch_path = refs_path + "/" + branch_file;
            std::string branch_data = utils::read_file(branch_path);
            if (!branch_data.empty()) {
                auto branch = Branch::from_string(branch_data);
                if (branch) {
                    branches[branch_name] = branch;
                }
            }
        }
    }
}

bool MiniGit::init() {
    if (is_initialized) {
        utils::print_warning("MiniGit repository already initialized");
        return true;
    }
    
    create_directory_structure();
    
    // Create initial branch
    current_branch = "main";
    auto main_branch = std::make_shared<Branch>("main");
    branches["main"] = main_branch;
    save_branch(main_branch);
    
    is_initialized = true;
    utils::print_success("Initialized empty MiniGit repository");
    return true;
}

void MiniGit::create_directory_structure() {
    utils::create_directory(minigit_path);
    utils::create_directory(objects_path);
    utils::create_directory(refs_path);
}

std::string MiniGit::compute_hash(const std::string& content) {
    return utils::sha1_hash(content);
}

void MiniGit::save_blob(const std::shared_ptr<Blob>& blob) {
    std::string blob_path = objects_path + "/" + blob->get_hash();
    utils::write_file(blob_path, blob->to_string());
}

std::shared_ptr<Blob> MiniGit::load_blob(const std::string& hash) {
    std::string blob_path = objects_path + "/" + hash;
    std::string blob_data = utils::read_file(blob_path);
    if (blob_data.empty()) {
        return nullptr;
    }
    return Blob::from_string(blob_data);
}

void MiniGit::save_commit(const std::shared_ptr<Commit>& commit) {
    std::string commit_path = objects_path + "/" + commit->get_hash();
    utils::write_file(commit_path, commit->to_string());
}

std::shared_ptr<Commit> MiniGit::load_commit(const std::string& hash) {
    std::string commit_path = objects_path + "/" + hash;
    std::string commit_data = utils::read_file(commit_path);
    if (commit_data.empty()) {
        return nullptr;
    }
    return Commit::from_string(commit_data);
}

void MiniGit::save_head(const std::string& commit_hash) {
    utils::write_file(head_path, commit_hash);
}

std::string MiniGit::load_head() {
    return utils::read_file(head_path);
}

void MiniGit::save_branch(const std::shared_ptr<Branch>& branch) {
    std::string branch_path = refs_path + "/" + branch->get_name();
    utils::write_file(branch_path, branch->to_string());
}

std::shared_ptr<Branch> MiniGit::load_branch(const std::string& name) {
    std::string branch_path = refs_path + "/" + name;
    std::string branch_data = utils::read_file(branch_path);
    if (branch_data.empty()) {
        return nullptr;
    }
    return Branch::from_string(branch_data);
}

bool MiniGit::add(const std::string& filename) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    if (!utils::file_exists(filename)) {
        utils::print_error("File '" + filename + "' does not exist");
        return false;
    }
    
    std::string content = utils::read_file(filename);
    auto blob = std::make_shared<Blob>(content, filename);
    staging_area[filename] = blob;
    
    utils::print_success("Added '" + filename + "' to staging area");
    return true;
}

bool MiniGit::commit(const std::string& message) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    if (staging_area.empty()) {
        utils::print_error("No changes staged for commit");
        return false;
    }
    
    // Create commit
    auto commit = std::make_shared<Commit>(message);
    
    // Add parent commit if exists
    std::string head_commit = load_head();
    if (!head_commit.empty()) {
        commit->add_parent(head_commit);
    }
    
    // Add staged files
    for (const auto& [filename, blob] : staging_area) {
        save_blob(blob);
        commit->add_file(filename, blob->get_hash());
    }
    
    // Save commit
    save_commit(commit);
    
    // Update HEAD and current branch
    save_head(commit->get_hash());
    if (branches.find(current_branch) != branches.end()) {
        branches[current_branch]->set_commit_hash(commit->get_hash());
        save_branch(branches[current_branch]);
    }
    
    // Clear staging area
    staging_area.clear();
    
    utils::print_success("Committed " + std::to_string(commit->get_files().size()) + " files");
    utils::print_info("Commit: " + commit->get_hash().substr(0, 8));
    return true;
}

bool MiniGit::log() {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    std::string current_commit_hash = load_head();
    if (current_commit_hash.empty()) {
        utils::print_info("No commits yet");
        return true;
    }
    
    std::string commit_hash = current_commit_hash;
    int commit_count = 0;
    
    while (!commit_hash.empty()) {
        auto commit = load_commit(commit_hash);
        if (!commit) {
            break;
        }
        
        std::cout << "\ncommit " << commit->get_hash() << std::endl;
        std::cout << "Author: " << commit->get_author() << std::endl;
        std::cout << "Date:   " << utils::timestamp_to_string(commit->get_timestamp()) << std::endl;
        std::cout << std::endl;
        std::cout << "    " << commit->get_message() << std::endl;
        
        if (commit->is_initial_commit()) {
            break;
        }
        
        commit_hash = commit->get_parents().empty() ? "" : commit->get_parents()[0];
        commit_count++;
        
        if (commit_count > 100) { // Prevent infinite loops
            break;
        }
    }
    
    return true;
}

bool MiniGit::branch(const std::string& branch_name) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    if (branches.find(branch_name) != branches.end()) {
        utils::print_error("Branch '" + branch_name + "' already exists");
        return false;
    }
    
    std::string current_commit = load_head();
    auto new_branch = std::make_shared<Branch>(branch_name, current_commit);
    branches[branch_name] = new_branch;
    save_branch(new_branch);
    
    utils::print_success("Created branch '" + branch_name + "'");
    return true;
}

bool MiniGit::checkout(const std::string& target) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    // Check if target is a branch
    if (branches.find(target) != branches.end()) {
        current_branch = target;
        std::string commit_hash = branches[target]->get_commit_hash();
        if (!commit_hash.empty()) {
            save_head(commit_hash);
        }
        utils::print_success("Switched to branch '" + target + "'");
        return true;
    }
    
    // Check if target is a commit hash
    auto commit = load_commit(target);
    if (commit) {
        save_head(target);
        utils::print_success("Switched to commit " + target.substr(0, 8));
        return true;
    }
    
    utils::print_error("Target '" + target + "' not found");
    return false;
}

std::vector<std::string> MiniGit::get_commit_ancestors(const std::string& commit_hash) {
    std::vector<std::string> ancestors;
    std::string current = commit_hash;
    
    while (!current.empty()) {
        auto commit = load_commit(current);
        if (!commit) {
            break;
        }
        
        ancestors.push_back(current);
        
        if (commit->is_initial_commit()) {
            break;
        }
        
        current = commit->get_parents().empty() ? "" : commit->get_parents()[0];
    }
    
    return ancestors;
}

std::string MiniGit::find_lowest_common_ancestor(const std::string& commit1_hash, const std::string& commit2_hash) {
    auto ancestors1 = get_commit_ancestors(commit1_hash);
    auto ancestors2 = get_commit_ancestors(commit2_hash);
    
    std::set<std::string> set1(ancestors1.begin(), ancestors1.end());
    
    for (const auto& ancestor : ancestors2) {
        if (set1.find(ancestor) != set1.end()) {
            return ancestor;
        }
    }
    
    return "";
}

std::map<std::string, std::string> MiniGit::get_file_changes(const std::string& from_hash, const std::string& to_hash) {
    std::map<std::string, std::string> changes;
    
    auto from_commit = load_commit(from_hash);
    auto to_commit = load_commit(to_hash);
    
    if (!from_commit || !to_commit) {
        return changes;
    }
    
    auto from_files = from_commit->get_files();
    auto to_files = to_commit->get_files();
    
    // Find changed files
    for (const auto& [filename, blob_hash] : to_files) {
        auto it = from_files.find(filename);
        if (it == from_files.end() || it->second != blob_hash) {
            changes[filename] = blob_hash;
        }
    }
    
    return changes;
}

std::string MiniGit::merge_files(const std::string& base_content, const std::string& ours_content, const std::string& theirs_content) {
    if (ours_content == theirs_content) {
        return ours_content;
    }
    
    if (base_content == ours_content) {
        return theirs_content;
    }
    
    if (base_content == theirs_content) {
        return ours_content;
    }
    
    // Simple merge strategy: append conflict markers
    std::string merged = base_content;
    merged += "\n<<<<<<< HEAD\n";
    merged += ours_content;
    merged += "\n=======\n";
    merged += theirs_content;
    merged += "\n>>>>>>> MERGE\n";
    
    return merged;
}

bool MiniGit::merge(const std::string& branch_name) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    if (branches.find(branch_name) == branches.end()) {
        utils::print_error("Branch '" + branch_name + "' does not exist");
        return false;
    }
    
    std::string current_commit = load_head();
    std::string target_commit = branches[branch_name]->get_commit_hash();
    
    if (current_commit == target_commit) {
        utils::print_info("Already up to date");
        return true;
    }
    
    // Find lowest common ancestor
    std::string lca = find_lowest_common_ancestor(current_commit, target_commit);
    if (lca.empty()) {
        utils::print_error("No common ancestor found");
        return false;
    }
    
    // Get changes from LCA to current and target
    auto current_changes = get_file_changes(lca, current_commit);
    auto target_changes = get_file_changes(lca, target_commit);
    
    bool has_conflicts = false;
    std::map<std::string, std::string> merged_files;
    
    // Merge files
    for (const auto& [filename, blob_hash] : target_changes) {
        auto current_it = current_changes.find(filename);
        
        if (current_it == current_changes.end()) {
            // File only changed in target branch
            merged_files[filename] = blob_hash;
        } else if (current_it->second == blob_hash) {
            // Same change in both branches
            merged_files[filename] = blob_hash;
        } else {
            // Conflict - both branches modified the file
            has_conflicts = true;
            utils::print_warning("CONFLICT: both modified " + filename);
            
            // Load the three versions
            auto base_blob = load_blob(load_commit(lca)->get_files().at(filename));
            auto ours_blob = load_blob(current_it->second);
            auto theirs_blob = load_blob(blob_hash);
            
            if (base_blob && ours_blob && theirs_blob) {
                std::string merged_content = merge_files(
                    base_blob->get_content(),
                    ours_blob->get_content(),
                    theirs_blob->get_content()
                );
                
                auto merged_blob = std::make_shared<Blob>(merged_content, filename);
                save_blob(merged_blob);
                merged_files[filename] = merged_blob->get_hash();
            }
        }
    }
    
    // Add files that only changed in current branch
    for (const auto& [filename, blob_hash] : current_changes) {
        if (target_changes.find(filename) == target_changes.end()) {
            merged_files[filename] = blob_hash;
        }
    }
    
    // Create merge commit
    std::string merge_message = "Merge branch '" + branch_name + "' into " + current_branch;
    auto merge_commit = std::make_shared<Commit>(merge_message);
    merge_commit->add_parent(current_commit);
    merge_commit->add_parent(target_commit);
    
    // Add all merged files
    for (const auto& [filename, blob_hash] : merged_files) {
        merge_commit->add_file(filename, blob_hash);
    }
    
    // Save merge commit
    save_commit(merge_commit);
    save_head(merge_commit->get_hash());
    
    // Update current branch
    if (branches.find(current_branch) != branches.end()) {
        branches[current_branch]->set_commit_hash(merge_commit->get_hash());
        save_branch(branches[current_branch]);
    }
    
    if (has_conflicts) {
        utils::print_warning("Merge completed with conflicts");
    } else {
        utils::print_success("Merge completed successfully");
    }
    
    return true;
}

bool MiniGit::diff(const std::string& commit1, const std::string& commit2) {
    if (!is_initialized) {
        utils::print_error("Not a MiniGit repository");
        return false;
    }
    
    auto commit1_obj = load_commit(commit1);
    auto commit2_obj = load_commit(commit2);
    
    if (!commit1_obj || !commit2_obj) {
        utils::print_error("Invalid commit hash");
        return false;
    }
    
    auto files1 = commit1_obj->get_files();
    auto files2 = commit2_obj->get_files();
    
    std::set<std::string> all_files;
    for (const auto& [filename, _] : files1) {
        all_files.insert(filename);
    }
    for (const auto& [filename, _] : files2) {
        all_files.insert(filename);
    }
    
    for (const auto& filename : all_files) {
        auto it1 = files1.find(filename);
        auto it2 = files2.find(filename);
        
        if (it1 == files1.end()) {
            // File added in commit2
            auto blob2 = load_blob(it2->second);
            if (blob2) {
                std::cout << "diff --git a/" << filename << " b/" << filename << std::endl;
                std::cout << "new file mode 100644" << std::endl;
                std::cout << "--- /dev/null" << std::endl;
                std::cout << "+++ b/" << filename << std::endl;
                
                auto lines = utils::split(blob2->get_content(), '\n');
                for (const auto& line : lines) {
                    std::cout << "+" << line << std::endl;
                }
            }
        } else if (it2 == files2.end()) {
            // File deleted in commit2
            auto blob1 = load_blob(it1->second);
            if (blob1) {
                std::cout << "diff --git a/" << filename << " b/" << filename << std::endl;
                std::cout << "deleted file mode 100644" << std::endl;
                std::cout << "--- a/" << filename << std::endl;
                std::cout << "+++ /dev/null" << std::endl;
                
                auto lines = utils::split(blob1->get_content(), '\n');
                for (const auto& line : lines) {
                    std::cout << "-" << line << std::endl;
                }
            }
        } else if (it1->second != it2->second) {
            // File modified
            auto blob1 = load_blob(it1->second);
            auto blob2 = load_blob(it2->second);
            
            if (blob1 && blob2) {
                std::cout << "diff --git a/" << filename << " b/" << filename << std::endl;
                std::cout << "--- a/" << filename << std::endl;
                std::cout << "+++ b/" << filename << std::endl;
                
                auto diff_lines = utils::compute_diff(blob1->get_content(), blob2->get_content());
                for (const auto& line : diff_lines) {
                    std::cout << line << std::endl;
                }
            }
        }
    }
    
    return true;
}

std::vector<std::string> MiniGit::get_branches() const {
    std::vector<std::string> branch_names;
    for (const auto& [name, _] : branches) {
        branch_names.push_back(name);
    }
    return branch_names;
}

std::string MiniGit::get_head_commit() const {
    return load_head();
} 
