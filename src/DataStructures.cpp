
#include "Declarations.cpp"

// ==================== AVL TREE ====================

AVLTree::Node::Node(int value) {
    id = value;
    height = 1;
    left = nullptr;
    right = nullptr;
}

AVLTree::AVLTree() {
    root = nullptr;
}

int AVLTree::height(Node* node) {
    if (node == nullptr)
        return 0;

    return node->height;
}

int AVLTree::balanceFactor(Node* node) {
    if (node == nullptr)
        return 0;

    return height(node->left) - height(node->right);
}

AVLTree::Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLTree::Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLTree::Node* AVLTree::insertNode(Node* node, int id) {
    if (node == nullptr)
        return new Node(id);

    if (id < node->id)
        node->left = insertNode(node->left, id);
    else if (id > node->id)
        node->right = insertNode(node->right, id);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = balanceFactor(node);

    if (balance > 1 && id < node->left->id)
        return rotateRight(node);

    if (balance < -1 && id > node->right->id)
        return rotateLeft(node);

    if (balance > 1 && id > node->left->id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && id < node->right->id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLTree::insert(int id) {
    root = insertNode(root, id);
}

void AVLTree::inorder(Node* node) {
    if (node == nullptr)
        return;

    inorder(node->left);
    cout << node->id << " ";
    inorder(node->right);
}

void AVLTree::display() {
    cout << "\nUser IDs in AVL Tree: ";
    inorder(root);
    cout << endl;
}

// ==================== WEIGHTED GRAPH ====================

void Graph::setSize(int size) {
    adj.resize(size);
}

void Graph::addConnection(int u, int v, int weight) {
    if (u < 0 || v < 0 || u >= (int)adj.size() || v >= (int)adj.size() || u == v || weight < 0)
        return;

    removeConnection(u, v);

    adj[u].push_back({v, weight});
    adj[v].push_back({u, weight});
}

void Graph::removeConnection(int u, int v) {
    if (u < 0 || v < 0 || u >= (int)adj.size() ||
        v >= (int)adj.size())
        return;

    auto& first = adj[u];
    first.erase(
        remove_if(first.begin(), first.end(),
            [v](const pair<int, int>& edge) {
                return edge.first == v;
            }),
        first.end()
    );

    auto& second = adj[v];
    second.erase(
        remove_if(second.begin(), second.end(),
            [u](const pair<int, int>& edge) {
                return edge.first == u;
            }),
        second.end()
    );
}

bool Graph::isConnected(int u, int v) {
    if (u < 0 || v < 0 || u >= (int)adj.size() ||
        v >= (int)adj.size())
        return false;

    for (const auto& edge : adj[u]) {
        if (edge.first == v)
            return true;
    }

    return false;
}

// ==================== BFS ====================

vector<int> Graph::bfs(int start) {
    vector<int> result;

    if (start < 0 || start >= (int)adj.size())
        return result;

    vector<bool> visited(adj.size(), false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        result.push_back(current);

        for (const auto& edge : adj[current]) {
            int neighbour = edge.first;

            if (!visited[neighbour]) {
                visited[neighbour] = true;
                q.push(neighbour);
            }
        }
    }

    return result;
}

// ==================== SHORTEST DISTANCE USING BFS ====================

int Graph::shortestDistance(int start, int destination) {
    if (start < 0 || destination < 0 ||
        start >= (int)adj.size() ||
        destination >= (int)adj.size())
        return -1;

    vector<int> distance(adj.size(), -1);
    queue<int> q;

    distance[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == destination)
            return distance[current];

        for (const auto& edge : adj[current]) {
            int neighbour = edge.first;

            if (distance[neighbour] == -1) {
                distance[neighbour] = distance[current] + 1;
                q.push(neighbour);
            }
        }
    }

    return -1;
}

// ==================== DIJKSTRA'S ALGORITHM ====================

pair<int, vector<int>> Graph::dijkstra(int start, int destination) {
    const int INF = numeric_limits<int>::max();

    if (start < 0 || destination < 0 ||
        start >= (int)adj.size() ||
        destination >= (int)adj.size())
        return {-1, {}};

    vector<int> distance(adj.size(), INF);
    vector<int> parent(adj.size(), -1);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int current = pq.top().second;
        pq.pop();

        if (currentDistance != distance[current])
            continue;

        if (current == destination)
            break;

        for (const auto& edge : adj[current]) {
            int neighbour = edge.first;
            int weight = edge.second;

            if (distance[current] <= INF - weight &&
                distance[current] + weight < distance[neighbour]) {

                distance[neighbour] = distance[current] + weight;
                parent[neighbour] = current;

                pq.push({distance[neighbour], neighbour});
            }
        }
    }

    if (distance[destination] == INF)
        return {-1, {}};

    vector<int> path;

    for (int current = destination; current != -1;
         current = parent[current]) {
        path.push_back(current);
    }

    reverse(path.begin(), path.end());

    return {distance[destination], path};
}

// ==================== STACK ====================

UndoStack::Node::Node(int a, int b) {
    first = a;
    second = b;
    next = nullptr;
}

UndoStack::UndoStack() {
    topNode = nullptr;
}

void UndoStack::push(int a, int b) {
    Node* newNode = new Node(a, b);

    newNode->next = topNode;
    topNode = newNode;
}

bool UndoStack::pop(int& a, int& b) {
    if (topNode == nullptr)
        return false;

    Node* temp = topNode;

    a = temp->first;
    b = temp->second;

    topNode = topNode->next;

    delete temp;

    return true;
}

bool UndoStack::isEmpty() {
    return topNode == nullptr;
}

// ==================== QUEUE ====================

RequestQueue::Node::Node(int value) {
    request = value;
    next = nullptr;
}

RequestQueue::RequestQueue() {
    frontNode = nullptr;
    rearNode = nullptr;
}

void RequestQueue::enqueue(int request) {
    Node* newNode = new Node(request);

    if (rearNode == nullptr) {
        frontNode = rearNode = newNode;
        return;
    }

    rearNode->next = newNode;
    rearNode = newNode;
}

bool RequestQueue::dequeue(int& request) {
    if (frontNode == nullptr)
        return false;

    Node* temp = frontNode;

    request = temp->request;
    frontNode = frontNode->next;

    if (frontNode == nullptr)
        rearNode = nullptr;

    delete temp;

    return true;
}

bool RequestQueue::isEmpty() {
    return frontNode == nullptr;
}

// ==================== MAX HEAP ====================

void MaxHeap::insert(int score, int userIndex) {
    heap.push_back({score, userIndex});

    int index = (int)heap.size() - 1;

    while (index > 0) {
        int parent = (index - 1) / 2;

        if (heap[parent].first >= heap[index].first)
            break;

        swap(heap[parent], heap[index]);
        index = parent;
    }
}

pair<int, int> MaxHeap::extractMax() {
    if (heap.empty())
        return {-1, -1};

    pair<int, int> result = heap[0];

    heap[0] = heap.back();
    heap.pop_back();

    int index = 0;
    int size = (int)heap.size();

    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < size &&
            heap[left].first > heap[largest].first)
            largest = left;

        if (right < size &&
            heap[right].first > heap[largest].first)
            largest = right;

        if (largest == index)
            break;

        swap(heap[index], heap[largest]);
        index = largest;
    }

    return result;
}

bool MaxHeap::isEmpty() {
    return heap.empty();
}

// ==================== MERGE SORT ====================

void mergeSort(vector<User>& users, int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    mergeSort(users, left, mid);
    mergeSort(users, mid + 1, right);

    vector<User> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (users[i].name <= users[j].name)
            temp.push_back(users[i++]);
        else
            temp.push_back(users[j++]);
    }

    while (i <= mid)
        temp.push_back(users[i++]);

    while (j <= right)
        temp.push_back(users[j++]);

    for (int k = 0; k < (int)temp.size(); k++)
        users[left + k] = temp[k];
}

// ==================== BINARY SEARCH ====================

int binarySearch(vector<User>& users, int id) {
    int left = 0;
    int right = (int)users.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (users[mid].id == id)
            return mid;

        if (users[mid].id < id)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return -1;
}
