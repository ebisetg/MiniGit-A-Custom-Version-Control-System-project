# MiniGit: A Custom Version Control System

A lightweight, educational implementation of Git-like version control system written in C++. This project demonstrates the core concepts of version control systems including commits, branching, merging, and file diffing.

## 🎯 Learning Objectives

By studying and using this project, you will learn:

- **Data Structures**: Implementation of Directed Acyclic Graphs (DAGs), trees, and hash maps
- **Git Internals**: Understanding how Git works under the hood
- **File I/O**: Efficient file handling and persistence
- **CLI Development**: Command-line interface design and implementation
- **Software Engineering**: Versioning, diffing, and merge strategies

## 🚀 Features

### Core Functionality

- ✅ **Repository Initialization** (`init`) - Create new MiniGit repositories
- ✅ **File Staging** (`add`) - Stage files for commit with hash computation
- ✅ **Commit Management** (`commit`) - Create snapshots with metadata
- ✅ **History Viewing** (`log`) - Traverse commit history
- ✅ **Branching** (`branch`) - Create and manage branches
- ✅ **Checkout** (`checkout`) - Switch between branches and commits
- ✅ **Merging** (`merge`) - Three-way merge with conflict resolution
- ✅ **Diff Viewing** (`diff`) - Line-by-line difference comparison

### Advanced Features

- 🔐 **SHA-1 Hashing** - Secure content-based addressing
- 🌳 **DAG Structure** - Efficient commit history representation
- 🔄 **Conflict Resolution** - Automatic and manual merge conflict handling
- 📊 **Status Reporting** - Repository state information
- 🎨 **Colored Output** - User-friendly terminal interface

## 📁 Project Structure

```
minigit/
├── CMakeLists.txt          # Build configuration
├── README.md              # This file
├── include/               # Header files
│   ├── minigit.h         # Main MiniGit class
│   ├── commit.h          # Commit data structure
│   ├── blob.h            # File content storage
│   ├── branch.h          # Branch management
│   └── utils.h           # Utility functions
├── src/                  # Source files
│   ├── main.cpp          # CLI interface
│   ├── minigit.cpp       # Core MiniGit implementation
│   ├── commit.cpp        # Commit operations
│   ├── blob.cpp          # Blob operations
│   ├── branch.cpp        # Branch operations
│   └── utils.cpp         # Utility implementations
└── docs/                 # Documentation
    ├── DESIGN.md         # Design decisions and architecture
    └── API.md            # API documentation
```

## 🛠️ Installation

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher
- OpenSSL development libraries

### Building on macOS/Linux

```bash
# Install dependencies (macOS)
brew install openssl cmake

# Install dependencies (Ubuntu/Debian)
sudo apt-get install libssl-dev cmake build-essential

# Clone and build
git clone <repository-url>
cd minigit
mkdir build && cd build
cmake ..
make
```

### Building on Windows

```bash
# Install dependencies via vcpkg
vcpkg install openssl

# Build with CMake
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

## 📖 Usage

### Basic Workflow

```bash
# Initialize a new repository
./minigit init

# Create a file
echo "Hello, World!" > hello.txt

# Stage the file
./minigit add hello.txt

# Commit the changes
./minigit commit -m "Initial commit"

# View commit history
./minigit log

# Create a new branch
./minigit branch feature

# Switch to the new branch
./minigit checkout feature

# Make changes and commit
echo "New feature!" >> hello.txt
./minigit add hello.txt
./minigit commit -m "Add new feature"

# Switch back to main and merge
./minigit checkout main
./minigit merge feature
```

### Available Commands

| Command             | Description           | Example                       |
| ------------------- | --------------------- | ----------------------------- |
| `init`              | Initialize repository | `minigit init`                |
| `add <file>`        | Stage file            | `minigit add file.txt`        |
| `commit -m <msg>`   | Commit changes        | `minigit commit -m "message"` |
| `log`               | Show history          | `minigit log`                 |
| `branch <name>`     | Create branch         | `minigit branch feature`      |
| `checkout <target>` | Switch branch/commit  | `minigit checkout main`       |
| `merge <branch>`    | Merge branch          | `minigit merge feature`       |
| `diff <c1> <c2>`    | Show differences      | `minigit diff abc123 def456`  |
| `status`            | Show status           | `minigit status`              |
| `help`              | Show help             | `minigit help`                |

## 🏗️ Architecture

### Data Structures

#### Blob

- **Purpose**: Store file content with hash-based identification
- **DSA Concepts**: Hashing, I/O operations
- **Implementation**: SHA-1 hash of content for unique identification

#### Commit

- **Purpose**: Snapshot of repository state with metadata
- **DSA Concepts**: Linked List / DAG nodes
- **Implementation**: Contains parent references, file mappings, and metadata

#### Branch

- **Purpose**: Named reference to a commit
- **DSA Concepts**: HashMap, pointers
- **Implementation**: Maps branch names to commit hashes

#### Staging Area

- **Purpose**: Track files to be committed
- **DSA Concepts**: Set, Hash Table
- **Implementation**: Map of filename to blob objects

### Internal Storage

```
.minigit/
├── objects/          # Content-addressable storage
│   ├── <hash1>      # Blob objects
│   ├── <hash2>      # Commit objects
│   └── ...
├── refs/            # Branch references
│   ├── main         # Main branch pointer
│   ├── feature      # Feature branch pointer
│   └── ...
└── HEAD             # Current commit pointer
```

## 🔧 Design Decisions

### 1. Content-Addressable Storage

- Files are stored by their SHA-1 hash
- Enables deduplication and integrity verification
- Similar to Git's object database

### 2. DAG-Based Commit History

- Commits form a directed acyclic graph
- Supports branching and merging
- Efficient ancestor traversal

### 3. Three-Way Merge Strategy

- Uses lowest common ancestor (LCA) for merge base
- Handles conflicts with standard markers
- Preserves history integrity

### 4. Simple File Format

- Human-readable serialization
- Easy to debug and understand
- Extensible for future features

## 🧪 Testing

### Manual Testing

```bash
# Test basic workflow
./minigit init
echo "test" > test.txt
./minigit add test.txt
./minigit commit -m "test commit"
./minigit log

# Test branching
./minigit branch test-branch
./minigit checkout test-branch
echo "branch content" > branch.txt
./minigit add branch.txt
./minigit commit -m "branch commit"

# Test merging
./minigit checkout main
./minigit merge test-branch
```

### Automated Testing

```bash
# Run test suite (if implemented)
make test
```

## 📚 Educational Value

This project demonstrates several key computer science concepts:

1. **Graph Theory**: DAGs for commit history
2. **Cryptography**: SHA-1 hashing for content addressing
3. **Data Structures**: Trees, hash maps, linked lists
4. **Algorithms**: Merge algorithms, graph traversal
5. **Systems Programming**: File I/O, command-line interfaces
6. **Software Design**: Modular architecture, separation of concerns

## 🤝 Contributing

This is an educational project. Contributions that improve:

- Code clarity and documentation
- Educational value
- Performance and efficiency
- Feature completeness

are welcome!

## 📄 License

This project is released under the MIT License. See LICENSE file for details.

## 🙏 Acknowledgments

- Inspired by Git's design and implementation
- Built for educational purposes in data structures and algorithms courses
- Uses OpenSSL for cryptographic operations

---

**Note**: This is an educational implementation and should not be used for production version control. For real projects, use Git or other established version control systems.
