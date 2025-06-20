# MiniGit Design Document

## Overview

MiniGit is a lightweight version control system that implements the core concepts of Git using fundamental data structures and algorithms. This document outlines the design decisions, architecture, and implementation details.

## System Architecture

### High-Level Design

MiniGit follows a layered architecture with clear separation of concerns:

```
┌─────────────────────────────────────┐
│           CLI Interface              │  (main.cpp)
├─────────────────────────────────────┤
│         MiniGit Core                │  (minigit.cpp)
├─────────────────────────────────────┤
│      Data Structures                │  (commit.cpp, blob.cpp, branch.cpp)
├─────────────────────────────────────┤
│        Utilities                    │  (utils.cpp)
├─────────────────────────────────────┤
│      File System                    │  (.minigit/ directory)
└─────────────────────────────────────┘
```

### Core Components

1. **MiniGit Class**: Main orchestrator that manages repository state
2. **Commit Class**: Represents commit nodes in the DAG
3. **Blob Class**: Stores file content with hash-based identification
4. **Branch Class**: Manages branch references
5. **Utils Namespace**: Common utility functions

## Data Structures

### 1. Blob (File Content Storage)

**Purpose**: Store file content with content-based addressing

**Structure**:

```cpp
class Blob {
    std::string hash;      // SHA-1 hash of content
    std::string content;   // Actual file content
    std::string filename;  // Original filename
};
```

**DSA Concepts**:

- **Hashing**: SHA-1 for content-based addressing
- **I/O Operations**: File reading and writing
- **Content Addressability**: Files identified by content hash

**Implementation Details**:

- Hash computed using OpenSSL SHA-1
- Serialized in human-readable format for debugging
- Supports deduplication (same content = same hash)

### 2. Commit (DAG Node)

**Purpose**: Snapshot of repository state with metadata

**Structure**:

```cpp
class Commit {
    std::string hash;                           // Commit identifier
    std::string message;                        // Commit message
    std::string author;                         // Author information
    std::time_t timestamp;                      // Creation time
    std::vector<std::string> parent_hashes;     // Parent commits
    std::map<std::string, std::string> file_blobs; // filename -> blob_hash
};
```

**DSA Concepts**:

- **Directed Acyclic Graph (DAG)**: Commit history forms a DAG
- **Linked List**: Linear commit chains
- **Tree Structure**: Branching and merging
- **HashMap**: Efficient file lookup

**Implementation Details**:

- Supports multiple parents for merge commits
- File mappings stored as filename to blob hash
- Timestamp for chronological ordering

### 3. Branch (Reference)

**Purpose**: Named pointer to a commit

**Structure**:

```cpp
class Branch {
    std::string name;           // Branch name
    std::string commit_hash;    // Pointed commit
};
```

**DSA Concepts**:

- **Pointer**: References to commits
- **HashMap**: Branch name to commit mapping
- **Symbolic Reference**: Human-readable names

### 4. Staging Area

**Purpose**: Track files to be committed

**Structure**:

```cpp
std::map<std::string, std::shared_ptr<Blob>> staging_area;
```

**DSA Concepts**:

- **Hash Table**: Efficient filename lookup
- **Set-like Behavior**: Unique file entries
- **Temporary Storage**: Between add and commit

## Storage Format

### Object Storage

All objects (blobs, commits) are stored in `.minigit/objects/` with their hash as the filename.

#### Blob Format

```
blob <hash>
filename <filename>
content <length>
<actual content>
```

#### Commit Format

```
commit <hash>
message <message>
author <author>
timestamp <timestamp>
parents <count>
parent <parent_hash1>
parent <parent_hash2>
...
files <count>
file <filename1> <blob_hash1>
file <filename2> <blob_hash2>
...
```

#### Branch Format

```
branch <name>
commit <commit_hash>
```

### Directory Structure

```
.minigit/
├── objects/          # Content-addressable storage
│   ├── a1b2c3d4...  # Blob objects
│   ├── e5f6g7h8...  # Commit objects
│   └── ...
├── refs/            # Branch references
│   ├── main         # Main branch
│   ├── feature      # Feature branch
│   └── ...
└── HEAD             # Current commit pointer
```

## Algorithms

### 1. Commit History Traversal

**Purpose**: Navigate commit history for log and merge operations

**Algorithm**:

```cpp
std::vector<std::string> get_commit_ancestors(const std::string& commit_hash) {
    std::vector<std::string> ancestors;
    std::string current = commit_hash;

    while (!current.empty()) {
        auto commit = load_commit(current);
        if (!commit) break;

        ancestors.push_back(current);

        if (commit->is_initial_commit()) break;
        current = commit->get_parents().empty() ? "" : commit->get_parents()[0];
    }

    return ancestors;
}
```

**Complexity**: O(n) where n is the number of commits

### 2. Lowest Common Ancestor (LCA)

**Purpose**: Find merge base for three-way merge

**Algorithm**:

```cpp
std::string find_lowest_common_ancestor(const std::string& commit1, const std::string& commit2) {
    auto ancestors1 = get_commit_ancestors(commit1);
    auto ancestors2 = get_commit_ancestors(commit2);

    std::set<std::string> set1(ancestors1.begin(), ancestors1.end());

    for (const auto& ancestor : ancestors2) {
        if (set1.find(ancestor) != set1.end()) {
            return ancestor;
        }
    }

    return "";
}
```

**Complexity**: O(n²) where n is the number of commits

### 3. Three-Way Merge

**Purpose**: Merge changes from two branches

**Algorithm**:

1. Find LCA of the two branches
2. Compute changes from LCA to each branch
3. Apply changes, handling conflicts
4. Create merge commit

**Conflict Resolution**:

- If both branches modified the same file differently, create conflict markers
- If one branch modified and other didn't, use the modified version
- If both branches made the same change, use either version

### 4. File Diffing

**Purpose**: Show differences between file versions

**Algorithm**:

```cpp
std::vector<std::string> compute_diff(const std::string& old_content, const std::string& new_content) {
    std::vector<std::string> old_lines = split(old_content, '\n');
    std::vector<std::string> new_lines = split(new_content, '\n');

    std::vector<std::string> diff;
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
```

**Complexity**: O(n) where n is the number of lines

## Design Decisions

### 1. Content-Addressable Storage

**Decision**: Store objects by their content hash
**Rationale**:

- Enables deduplication
- Provides integrity verification
- Similar to Git's approach
- Simplifies object lookup

### 2. Human-Readable Serialization

**Decision**: Use text-based object format
**Rationale**:

- Easy to debug and understand
- No binary format complexity
- Educational value
- Extensible for future features

### 3. Simple Branch Model

**Decision**: Branches are just named pointers
**Rationale**:

- Simple to implement and understand
- Sufficient for educational purposes
- Easy to extend with more features

### 4. In-Memory Staging Area

**Decision**: Keep staging area in memory
**Rationale**:

- Faster operations
- Simpler implementation
- Sufficient for small repositories

## Performance Considerations

### Time Complexity

| Operation | Complexity | Notes                            |
| --------- | ---------- | -------------------------------- |
| Add file  | O(1)       | Hash computation + map insertion |
| Commit    | O(n)       | n = number of staged files       |
| Log       | O(n)       | n = number of commits            |
| Branch    | O(1)       | Simple pointer creation          |
| Checkout  | O(1)       | Pointer update                   |
| Merge     | O(n²)      | LCA computation dominates        |
| Diff      | O(n)       | n = number of lines              |

### Space Complexity

| Component      | Space | Notes                   |
| -------------- | ----- | ----------------------- |
| Blob storage   | O(f)  | f = total file content  |
| Commit storage | O(c)  | c = number of commits   |
| Branch storage | O(b)  | b = number of branches  |
| Staging area   | O(s)  | s = staged file content |

## Limitations and Future Improvements

### Current Limitations

1. **No Index File**: Staging area is in-memory only
2. **Simple Diff**: Line-by-line comparison only
3. **No Compression**: Objects stored uncompressed
4. **No Remote Operations**: Local-only repository
5. **Limited Conflict Resolution**: Basic merge conflict handling

### Potential Improvements

1. **Persistent Staging Area**: Store index file on disk
2. **Advanced Diff**: Myers diff algorithm implementation
3. **Object Compression**: Zlib compression for objects
4. **Remote Support**: HTTP/SSH protocol implementation
5. **Advanced Merge**: Recursive merge strategies
6. **Performance Optimization**: Object caching and lazy loading

## Security Considerations

### Hash Collisions

**Risk**: SHA-1 collision attacks
**Mitigation**:

- Educational project, not production use
- Could upgrade to SHA-256 in future
- Content verification on load

### File System Security

**Risk**: Path traversal attacks
**Mitigation**:

- Validate file paths
- Restrict to repository directory
- Sanitize user inputs

## Testing Strategy

### Unit Testing

- Test individual data structures
- Test utility functions
- Test serialization/deserialization

### Integration Testing

- Test complete workflows
- Test error conditions
- Test edge cases

### Manual Testing

- Real-world usage scenarios
- Performance testing
- User experience validation

## Conclusion

MiniGit successfully demonstrates the core concepts of version control systems using fundamental data structures and algorithms. The design prioritizes educational value and clarity over performance, making it an excellent learning tool for understanding how Git works under the hood.

The modular architecture allows for easy extension and modification, while the simple storage format makes debugging and understanding the system straightforward. The implementation covers all the essential features of a version control system while remaining accessible to students learning data structures and algorithms.
