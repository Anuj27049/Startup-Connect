#ifndef DECLARATIONS_CPP
#define DECLARATIONS_CPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <limits>
#include <utility>

using namespace std;

// USER CLASS

class User {
public:
    int id;
    string name;
    string email;
    string password;
    string location;
    string role;
    string field;
    double investmentAmount;

    User(int id = 0, string name = "", string email = "",
         string password = "", string location = "",
         string role = "", string field = "",
         double investmentAmount = 0);

    void display();
    void save(ofstream& file);
};

// AVL TREE

class AVLTree {
private:
    class Node {
    public:
        int id;
        int height;
        Node* left;
        Node* right;

        Node(int value);
    };

    Node* root;

    int height(Node* node);
    int balanceFactor(Node* node);

    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* insertNode(Node* node, int id);

    void inorder(Node* node);

public:
    AVLTree();

    void insert(int id);
    void display();
};

// WEIGHTED GRAPH

class Graph {
private:
    vector<vector<pair<int, int>>> adj;

public:
    void setSize(int size);

    void addConnection(int u, int v, int weight);
    void removeConnection(int u, int v);

    bool isConnected(int u, int v);

    vector<int> bfs(int start);

    int shortestDistance(int start, int destination);

    pair<int, vector<int>> dijkstra(int start, int destination);
};

// STACK FOR UNDO CONNECTION

class UndoStack {
private:
    class Node {
    public:
        int first;
        int second;
        Node* next;

        Node(int a, int b);
    };

    Node* topNode;

public:
    UndoStack();

    void push(int a, int b);
    bool pop(int& a, int& b);
    bool isEmpty();
};

// QUEUE FOR FUNDING REQUESTS

class RequestQueue {
private:
    class Node {
    public:
        int request;
        Node* next;

        Node(int value);
    };

    Node* frontNode;
    Node* rearNode;

public:
    RequestQueue();

    void enqueue(int request);
    bool dequeue(int& request);
    bool isEmpty();
};

// MAX HEAP FOR INVESTOR RECOMMENDATIONS

class MaxHeap {
private:
    vector<pair<int, int>> heap;

public:
    void insert(int score, int userIndex);
    pair<int, int> extractMax();
    bool isEmpty();
};

// SORTING AND SEARCHING ALGORITHMS

void mergeSort(vector<User>& users, int left, int right);

int binarySearch(vector<User>& users, int id);

// PLATFORM CLASS

class Platform {
private:
    vector<User> users;

    AVLTree userTree;
    Graph network;

    UndoStack undoHistory;
    RequestQueue requests;
    MaxHeap recommendations;

    vector<pair<int, int>> connections;

    int currentUserIndex;

    void loadUsers();
    void saveUsers();

    void loadConnections();
    void saveConnections();

    int findUserIndex(int id);

    void registerUser();
    void login();

    void exploreCommunity();
    void connectWithMember();
    void myConnections();

    void discoverInvestors();
    void fundingOpportunities();

    void exploreNetwork();
    void myProfile();

    void displayMenu();

public:
    Platform();

    void run();
};
#endif