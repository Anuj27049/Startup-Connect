
#include "Declarations.cpp"

// ==================== INPUT HELPERS ====================

int getNumber(string message) {
    int value;

    while (true) {
        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getAmount(string message) {
    double value;

    while (true) {
        cout << message;

        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid amount. Enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string getText(string message) {
    string value;

    while (true) {
        cout << message;
        getline(cin, value);

        if (!value.empty() && value.find('|') == string::npos)
            return value;

        cout << "Input cannot be empty or contain |.\n";
    }
}

// ==================== CONNECTION COST ====================

int calculateConnectionCost(const User& first, const User& second) {
    bool sameLocation = (first.location == second.location);
    bool sameField = (first.field == second.field);

    if (sameLocation && sameField)
        return 1;

    if (sameField)
        return 2;

    if (sameLocation)
        return 3;

    return 5;
}

// ==================== USER CLASS ====================

User::User(int id, string name, string email,
           string password, string location,
           string role, string field,
           double investmentAmount) {

    this->id = id;
    this->name = name;
    this->email = email;
    this->password = password;
    this->location = location;
    this->role = role;
    this->field = field;
    this->investmentAmount = investmentAmount;
}

void User::display() {
    cout << "\nUser ID: " << id;
    cout << "\nName: " << name;
    cout << "\nEmail: " << email;
    cout << "\nLocation: " << location;
    cout << "\nRole: " << role;
    cout << "\nIndustry: " << field;

    if (role == "Investor")
        cout << "\nInvestment Budget: Rs. " << investmentAmount;

    cout << "\n-----------------------------\n";
}

void User::save(ofstream& file) {
    file << id << "|" << name << "|" << email << "|"
         << password << "|" << location << "|"
         << role << "|" << field << "|"
         << investmentAmount << "\n";
}

// ==================== PLATFORM CONSTRUCTOR ====================

Platform::Platform() {
    currentUserIndex = -1;

    loadUsers();
    loadConnections();
}

// ==================== USER SEARCH ====================

int Platform::findUserIndex(int id) {
    vector<User> sortedUsers = users;

    sort(sortedUsers.begin(), sortedUsers.end(),
         [](const User& a, const User& b) {
             return a.id < b.id;
         });

    int position = binarySearch(sortedUsers, id);

    if (position == -1)
        return -1;

    int actualID = sortedUsers[position].id;

    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].id == actualID)
            return i;
    }

    return -1;
}

// ==================== LOAD AND SAVE USERS ====================

void Platform::loadUsers() {
    ifstream file("data/users.txt");

    if (!file.is_open()) {
        cout << "Could not open data/users.txt\n";
        return;
    }

    string line;

    while (getline(file, line)) {
        if (line.empty())
            continue;

        stringstream ss(line);

        string idText, name, email, password;
        string location, role, field, amountText;

        if (!getline(ss, idText, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, email, '|') ||
            !getline(ss, password, '|') ||
            !getline(ss, location, '|') ||
            !getline(ss, role, '|') ||
            !getline(ss, field, '|') ||
            !getline(ss, amountText)) {

            cout << "Skipping an invalid user record.\n";
            continue;
        }

        try {
            int id = stoi(idText);
            double amount = stod(amountText);

            if (id <= 0 || amount < 0 || findUserIndex(id) != -1)
                continue;

            User user(id, name, email, password,
                      location, role, field, amount);

            users.push_back(user);
            userTree.insert(id);
        }
        catch (...) {
            cout << "Skipping an invalid user record.\n";
        }
    }

    file.close();

    network.setSize((int)users.size());
}

void Platform::saveUsers() {
    ofstream file("data/users.txt");

    if (!file.is_open()) {
        cout << "Unable to save users.\n";
        return;
    }

    for (User& user : users)
        user.save(file);

    file.close();
}

// ==================== LOAD AND SAVE CONNECTIONS ====================

void Platform::loadConnections() {
    ifstream file("data/connections.txt");

    if (!file.is_open())
        return;

    int firstID, secondID;

    while (file >> firstID >> secondID) {
        int first = findUserIndex(firstID);
        int second = findUserIndex(secondID);

        if (first == -1 || second == -1 || first == second)
            continue;

        if (network.isConnected(first, second))
            continue;

        int weight = calculateConnectionCost(
            users[first], users[second]
        );

        network.addConnection(first, second, weight);
        connections.push_back({first, second});

        undoHistory.push(first, second);
    }

    file.close();
}

void Platform::saveConnections() {
    ofstream file("data/connections.txt");

    if (!file.is_open()) {
        cout << "Unable to save connections.\n";
        return;
    }

    for (const auto& connection : connections) {
        file << users[connection.first].id << " "
             << users[connection.second].id << "\n";
    }

    file.close();
}

// ==================== REGISTRATION ====================

void Platform::registerUser() {
    cout << "\n========== JOIN STARTUP CONNECT ==========\n";

    int id = getNumber("Enter a unique User ID: ");

    if (id <= 0 || findUserIndex(id) != -1) {
        cout << "Invalid or already existing User ID.\n";
        return;
    }

    string name = getText("Enter Name: ");
    string email = getText("Enter Email: ");
    string password = getText("Create Password: ");
    string location = getText("Enter Location: ");

    cout << "\nSelect Role:\n";
    cout << "1. Startup\n";
    cout << "2. Investor\n";
    cout << "3. Mentor\n";
    cout << "4. Incubator\n";

    int choice = getNumber("Enter choice: ");

    string role;

    switch (choice) {
        case 1: role = "Startup"; break;
        case 2: role = "Investor"; break;
        case 3: role = "Mentor"; break;
        case 4: role = "Incubator"; break;

        default:
            cout << "Invalid role.\n";
            return;
    }

    string field = getText("Enter Industry/Field: ");

    double amount = 0;

    if (role == "Investor")
        amount = getAmount("Enter Investment Budget: ");

    User user(id, name, email, password,
              location, role, field, amount);

    users.push_back(user);
    userTree.insert(id);
    network.setSize((int)users.size());

    saveUsers();

    cout << "\nRegistration successful!\n";
}

// ==================== LOGIN ====================

void Platform::login() {
    cout << "\n========== LOGIN ==========\n";

    int id = getNumber("Enter User ID: ");
    int index = findUserIndex(id);

    if (index == -1) {
        cout << "User not found.\n";
        return;
    }

    string password = getText("Enter Password: ");

    if (users[index].password != password) {
        cout << "Incorrect password.\n";
        return;
    }

    currentUserIndex = index;

    cout << "\nWelcome, " << users[index].name << "!\n";
}

// ==================== EXPLORE COMMUNITY ====================

void Platform::exploreCommunity() {
    if (users.empty()) {
        cout << "\nNo users registered yet.\n";
        return;
    }

    vector<User> sortedUsers = users;

    mergeSort(sortedUsers, 0, (int)sortedUsers.size() - 1);

    cout << "\n========== COMMUNITY ==========\n";

    for (User& user : sortedUsers)
        user.display();

    cout << "\n========== SEARCH MEMBER ==========\n";

    int id = getNumber("Enter User ID to search (0 to skip): ");

    if (id != 0) {
        vector<User> idSortedUsers = users;

        sort(idSortedUsers.begin(), idSortedUsers.end(),
             [](const User& a, const User& b) {
                 return a.id < b.id;
             });

        int position = binarySearch(idSortedUsers, id);

        if (position == -1)
            cout << "User not found.\n";
        else
            idSortedUsers[position].display();
    }

    userTree.display();
}

// ==================== CONNECT WITH MEMBER ====================

void Platform::connectWithMember() {
    if (currentUserIndex == -1) {
        cout << "Please login first.\n";
        return;
    }

    int id = getNumber("Enter User ID to connect with: ");
    int index = findUserIndex(id);

    if (index == -1) {
        cout << "User not found.\n";
        return;
    }

    if (index == currentUserIndex) {
        cout << "You cannot connect with yourself.\n";
        return;
    }

    if (network.isConnected(currentUserIndex, index)) {
        cout << "You are already connected with this user.\n";
        return;
    }

    int weight = calculateConnectionCost(
        users[currentUserIndex], users[index]
    );

    network.addConnection(currentUserIndex, index, weight);

    connections.push_back({currentUserIndex, index});
    undoHistory.push(currentUserIndex, index);

    saveConnections();

    cout << "\nConnection created successfully!\n";
    cout << "Networking effort cost: " << weight << "\n";
}

// ==================== MY CONNECTIONS ====================

void Platform::myConnections() {
    if (currentUserIndex == -1) {
        cout << "Please login first.\n";
        return;
    }

    cout << "\n========== MY CONNECTIONS ==========\n";

    int count = 0;

    for (const auto& connection : connections) {
        int other = -1;

        if (connection.first == currentUserIndex)
            other = connection.second;
        else if (connection.second == currentUserIndex)
            other = connection.first;

        if (other != -1) {
            cout << "\nID: " << users[other].id;
            cout << "\nName: " << users[other].name;
            cout << "\nRole: " << users[other].role << "\n";

            count++;
        }
    }

    if (count == 0)
        cout << "You have no connections yet.\n";

    cout << "\n1. Undo Latest Connection\n";
    cout << "2. Back\n";

    int choice = getNumber("Enter choice: ");

    if (choice == 1) {
        int first, second;

        if (!undoHistory.pop(first, second)) {
            cout << "No connection available to undo.\n";
            return;
        }

        if (first != currentUserIndex) {
            undoHistory.push(first, second);

            cout << "Only the creator of the latest connection "
                 << "can undo it.\n";
            return;
        }

        network.removeConnection(first, second);

        connections.erase(
            remove(connections.begin(), connections.end(),
                   make_pair(first, second)),
            connections.end()
        );

        saveConnections();

        cout << "Latest connection undone successfully.\n";
    }
}

// ==================== INVESTOR RECOMMENDATIONS ====================

void Platform::discoverInvestors() {
    if (currentUserIndex == -1) {
        cout << "Please login first.\n";
        return;
    }

    if (users[currentUserIndex].role != "Startup") {
        cout << "Only startups can discover investors.\n";
        return;
    }

    recommendations = MaxHeap();

    User startup = users[currentUserIndex];

    for (int i = 0; i < (int)users.size(); i++) {
        if (users[i].role != "Investor")
            continue;

        int score = 0;

        if (startup.location == users[i].location)
            score += 40;

        if (startup.field == users[i].field)
            score += 60;

        if (users[i].investmentAmount > 0)
            score += 10;

        recommendations.insert(score, i);
    }

    cout << "\n========== RECOMMENDED INVESTORS ==========\n";

    if (recommendations.isEmpty()) {
        cout << "No investors available.\n";
        return;
    }

    while (!recommendations.isEmpty()) {
        pair<int, int> result = recommendations.extractMax();

        cout << "\nCompatibility Score: " << result.first;
        cout << "\n";

        users[result.second].display();
    }
}

// ==================== FUNDING OPPORTUNITIES ====================

void Platform::fundingOpportunities() {
    cout << "\n========== FUNDING OPPORTUNITIES ==========\n";
    cout << "1. Submit Funding Request\n";
    cout << "2. Process Next Request\n";
    cout << "3. Back\n";

    int choice = getNumber("Enter choice: ");

    if (choice == 1) {
        if (currentUserIndex == -1) {
            cout << "Please login first.\n";
            return;
        }

        if (users[currentUserIndex].role != "Startup") {
            cout << "Only startups can submit funding requests.\n";
            return;
        }

        requests.enqueue(currentUserIndex);

        cout << "Funding request added to the queue.\n";
    }
    else if (choice == 2) {
        int index;

        if (!requests.dequeue(index)) {
            cout << "No pending funding requests.\n";
            return;
        }

        cout << "\nProcessing Funding Request:\n";
        users[index].display();
    }
}

// ==================== EXPLORE NETWORK ====================

void Platform::exploreNetwork() {
    if (currentUserIndex == -1) {
        cout << "Please login first.\n";
        return;
    }

    cout << "\n========== NETWORK EXPLORATION ==========\n";

    vector<int> visited = network.bfs(currentUserIndex);

    cout << "\nMembers reachable through BFS:\n";

    for (int index : visited) {
        cout << users[index].name
             << " (ID: " << users[index].id << ")\n";
    }

    int id = getNumber("\nEnter destination User ID (0 to skip): ");

    if (id == 0)
        return;

    int destination = findUserIndex(id);

    if (destination == -1) {
        cout << "User not found.\n";
        return;
    }

    int hops = network.shortestDistance(
        currentUserIndex, destination
    );

    cout << "\nMinimum connection hops: " << hops << "\n";

    pair<int, vector<int>> result =
        network.dijkstra(currentUserIndex, destination);

    if (result.first == -1) {
        cout << "No networking route exists to this user.\n";
        return;
    }

    cout << "\n========== DIJKSTRA'S SHORTEST ROUTE ==========\n";

    cout << "Minimum Networking Cost: " << result.first << "\n";

    cout << "Recommended Route:\n";

    for (int i = 0; i < (int)result.second.size(); i++) {
        int index = result.second[i];

        cout << users[index].name
             << " (" << users[index].id << ")";

        if (i != (int)result.second.size() - 1)
            cout << " -> ";
    }

    cout << "\n";
}

// ==================== MY PROFILE ====================

void Platform::myProfile() {
    if (currentUserIndex == -1) {
        cout << "Please login first.\n";
        return;
    }

    cout << "\n========== MY PROFILE ==========\n";

    users[currentUserIndex].display();
}

// ==================== MAIN MENU ====================

void Platform::displayMenu() {
    cout << "\n====================================\n";
    cout << "          STARTUP CONNECT\n";
    cout << "====================================\n";

    if (currentUserIndex != -1)
        cout << "Logged in as: " << users[currentUserIndex].name << "\n";

    cout << "\n1. Join Platform";
    cout << "\n2. Login";
    cout << "\n3. Explore Community";
    cout << "\n4. Connect with a Member";
    cout << "\n5. My Connections";
    cout << "\n6. Discover Investors";
    cout << "\n7. Funding Opportunities";
    cout << "\n8. Explore My Network";
    cout << "\n9. My Profile";
    cout << "\n10. Logout";
    cout << "\n11. Exit\n";
}

// ==================== RUN APPLICATION ====================

void Platform::run() {
    int choice;

    do {
        displayMenu();

        choice = getNumber("\nEnter your choice: ");

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                login();
                break;

            case 3:
                exploreCommunity();
                break;

            case 4:
                connectWithMember();
                break;

            case 5:
                myConnections();
                break;

            case 6:
                discoverInvestors();
                break;

            case 7:
                fundingOpportunities();
                break;

            case 8:
                exploreNetwork();
                break;

            case 9:
                myProfile();
                break;

            case 10:
                currentUserIndex = -1;
                cout << "Logged out successfully.\n";
                break;

            case 11:
                cout << "\nThank you for using Startup Connect!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 11);
}
