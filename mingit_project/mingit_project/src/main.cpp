#include "minigit.h"
#include "utils.h"
#include <iostream>
#include <string>

void print_usage() {
    std::cout << "MiniGit - A Custom Version Control System\n";
    std::cout << "Usage: minigit <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  init                    Initialize a new MiniGit repository\n";
    std::cout << "  add <file>              Add file to staging area\n";
    std::cout << "  commit -m <message>     Commit staged changes\n";
    std::cout << "  log                     Show commit history\n";
    std::cout << "  branch <name>           Create a new branch\n";
    std::cout << "  checkout <target>       Switch to branch or commit\n";
    std::cout << "  merge <branch>          Merge branch into current branch\n";
    std::cout << "  diff <commit1> <commit2> Show differences between commits\n";
    std::cout << "  status                  Show repository status\n";
    std::cout << "  help                    Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  minigit init\n";
    std::cout << "  minigit add file.txt\n";
    std::cout << "  minigit commit -m \"Initial commit\"\n";
    std::cout << "  minigit branch feature\n";
    std::cout << "  minigit checkout feature\n";
    std::cout << "  minigit merge main\n";
}

void print_status(const MiniGit& git) {
    if (!git.is_repo_initialized()) {
        utils::print_error("Not a MiniGit repository");
        return;
    }
    
    std::cout << "On branch " << git.get_current_branch() << std::endl;
    
    std::string head_commit = git.get_head_commit();
    if (!head_commit.empty()) {
        std::cout << "HEAD: " << head_commit.substr(0, 8) << std::endl;
    } else {
        std::cout << "HEAD: (no commits yet)" << std::endl;
    }
    
    std::vector<std::string> branches = git.get_branches();
    if (!branches.empty()) {
        std::cout << "Branches: ";
        for (size_t i = 0; i < branches.size(); ++i) {
            if (i > 0) std::cout << ", ";
            if (branches[i] == git.get_current_branch()) {
                std::cout << "*" << branches[i];
            } else {
                std::cout << branches[i];
            }
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "help" || command == "--help" || command == "-h") {
        print_usage();
        return 0;
    }
    
    MiniGit git;
    
    if (command == "init") {
        if (!git.init()) {
            return 1;
        }
    } else if (command == "add") {
        if (argc < 3) {
            utils::print_error("Usage: minigit add <file>");
            return 1;
        }
        if (!git.add(argv[2])) {
            return 1;
        }
    } else if (command == "commit") {
        if (argc < 4 || std::string(argv[2]) != "-m") {
            utils::print_error("Usage: minigit commit -m <message>");
            return 1;
        }
        if (!git.commit(argv[3])) {
            return 1;
        }
    } else if (command == "log") {
        if (!git.log()) {
            return 1;
        }
    } else if (command == "branch") {
        if (argc < 3) {
            utils::print_error("Usage: minigit branch <name>");
            return 1;
        }
        if (!git.branch(argv[2])) {
            return 1;
        }
    } else if (command == "checkout") {
        if (argc < 3) {
            utils::print_error("Usage: minigit checkout <target>");
            return 1;
        }
        if (!git.checkout(argv[2])) {
            return 1;
        }
    } else if (command == "merge") {
        if (argc < 3) {
            utils::print_error("Usage: minigit merge <branch>");
            return 1;
        }
        if (!git.merge(argv[2])) {
            return 1;
        }
    } else if (command == "diff") {
        if (argc < 4) {
            utils::print_error("Usage: minigit diff <commit1> <commit2>");
            return 1;
        }
        if (!git.diff(argv[2], argv[3])) {
            return 1;
        }
    } else if (command == "status") {
        print_status(git);
    } else {
        utils::print_error("Unknown command: " + command);
        std::cout << "Use 'minigit help' for usage information." << std::endl;
        return 1;
    }
    
    return 0;
} 