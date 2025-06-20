# MiniGit: A Custom Version Control System

## 📌 Project Overview

MiniGit is a lightweight, Git-inspired version control system built from scratch using C++. It simulates core Git functionalities like tracking file changes, commits, branching, merging, and viewing history — all through a command-line interface. This project helps students understand how real-world version control tools work by applying key Data Structures and Algorithms (DSA) principles such as hashing, trees, DAGs, and file diffing.

---

## 🎯 Learning Objectives

- Design and implement complex data structures (e.g., DAGs, trees).
- Understand the internal workings of Git.
- Improve file I/O handling and CLI application development.
- Write modular, scalable, and maintainable code.
- Apply software engineering practices: versioning, diffing, merging.

---

## 🚀 Key Features

| Feature                              | Description                                                          |
| ------------------------------------ | -------------------------------------------------------------------- |
| `init`                               | Initializes a `.minigit/` directory and sets up tracking structures. |
| `add <filename>`                     | Stages files for commit; hashes file content and stores blobs.       |
| `commit -m "<message>"`              | Saves a snapshot with metadata, parent link, and file references.    |
| `log`                                | Displays commit history from HEAD with hashes and messages.          |
| `branch <name>`                      | Creates a new branch pointing to the current commit.                 |
| `checkout <branch/commit>`           | Switches working directory to the specified state.                   |
| `merge <branch>`                     | Merges changes between branches, resolving conflicts.                |
| `diff <commit1> <commit2>` _(Bonus)_ | Displays line-by-line differences between two commits.               |

---

## 📂 Internal Data Structures

| Component            | Purpose                        | DSA Concepts     |
| -------------------- | ------------------------------ | ---------------- |
| **Blob**             | Stores content of a file       | Hashing, I/O     |
| **Commit Node**      | Stores metadata and blob links | DAG, Linked List |
| **Branch Reference** | Maps branch names to commits   | HashMap          |
| **Staging Area**     | Tracks added files             | Set, Hash Table  |
| **Log History**      | Tracks commit chain            | Linked List      |

---

## 🛠️ Technology Stack

- **Language**: C++
- **CLI**: Command-line interface (custom-built)
- **Hashing**: SHA-1 or custom hash implementation
- **File Storage**: Local filesystem for blobs and commit data

---

## 📄 Expected Deliverables

- ✅ Functional CLI-based MiniGit system
- ✅ Clean, modular, and well-commented code
- ✅ Documentation explaining:
  - Data structures used
  - Design choices
  - Limitations & improvements
- ✅ GitHub repository with team commit history
- ✅ Short video demo of MiniGit in action

---

## 🧪 Grading Criteria

| Criteria                                    | Weight |
| ------------------------------------------- | ------ |
| Core Features (init, add, commit, log)      | 30%    |
| Advanced Features (branch, checkout, merge) | 30%    |
| Code Quality & Modularity                   | 15%    |
| Documentation & Report                      | 15%    |
| CLI Usability                               | 10%    |

---

## 👥 Group Members

| Name              | ID          |
| ----------------- | ----------- |
| Anatoli Chala     | UGR/4369/16 |
| Asanti Oluma      | UGR/8165/16 |
| Ebbise Tekle      | UGR/9482/16 |
| Eleni Demlie      | UGR/7940/16 |
| Mufarihat Tadesse | UGR/9735/16 |

---

## 📽️ Demo

📌 _[Insert your video demo link here]_

---

## 📎 License

This project is developed as part of a university course and is intended for educational use only.

---

## 📫 Contact

For any inquiries, please contact any of the group members or your course instructor.

---
