# Startup Ecosystem – Submission Version

Console-based C++17 Data Structures Lab project.

## Compile (run from project root)
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o StartupEcosystem.exe
```

## Run (Windows PowerShell)
```powershell
.\StartupEcosystem.exe
```

Compile `main.cpp` only. The main file includes the two implementation modules.

## Structure
- `main.cpp`: program entry point and module integration
- `src/Declarations.cpp`: shared class declarations
- `src/DataStructures.cpp`: data structures and sorting/searching algorithms
- `src/Platform.cpp`: application features and menu
- `data/`: user and connection storage

## Features
Registration, login, profile search, Graph connections, BFS, shortest connection distance,
Hash Table with Chaining, AVL Tree, Binary Heap investor recommendations, Doubly Linked List
activity log, Queue funding requests, Stack undo, Merge Sort, Quick Sort and Binary Search.

## Notes
User profiles and connections persist in text files. Activity history, pending requests and undo
history are session-based. Passwords are stored as plain text for this academic demonstration;
use dummy passwords. Investor recommendations use a simple same-location score.
