
# Startup Connect 🚀

A C++17 based startup networking and investment platform that connects startups, investors, mentors, and incubators.

## Features
- User registration and login
- Explore and search community members
- Create and undo connections
- Investor recommendations based on compatibility
- Funding request management
- Network exploration and shortest-route finding

## Data Structures
- AVL Tree – Maintains sorted user IDs
- Weighted Graph – Represents professional connections
- Stack – Undoes recent connections
- Queue – Processes funding requests
- Max Heap – Ranks investor recommendations

## Algorithms
- BFS – Finds minimum connection hops
- Dijkstra's Algorithm – Finds minimum-cost networking routes
- Merge Sort – Sorts community members alphabetically
- Binary Search – Searches users by ID

## Technologies
- C++
- C++17
- STL

## Project Structure
```text
Startup-Connect/
├── main.cpp
├── run.bat
├── README.md
├── .gitignore
├── src/
│   ├── Declarations.cpp
│   ├── DataStructures.cpp
│   └── Platform.cpp
└── data/
    ├── users.txt
    └── connections.txt
```

## How to Run

Requirements: GCC / MinGW with C++17 support.

Run using:
```bash
.\run.bat
```

Or compile manually:
```bash
g++ -std=c++17 main.cpp -o StartupConnect.exe
.\StartupConnect.exe
```

## Sample Login

| User | ID | Password |
|---|---|---|
| GreenKart | 1001 | green123 |
| Aditi Sharma | 1002 | aditi123 |
| Rahul Mehta | 1003 | rahul123 |

## Note
This is an academic DSA project. Sample credentials are for demonstration only. Passwords are stored in plain text, and funding requests are not persisted after the application exits.
