#include "../include/traffic_system.h"
#include <algorithm>

// ParkingSlotManager implementation
ParkingSlotManager::ParkingSlotManager() {
    for (int i = 0; i < MAX_PARKING_SLOTS; i++) {
        occupied[i] = false;
    }
}

int ParkingSlotManager::allocateSlot() {
    for (int i = 0; i < MAX_PARKING_SLOTS; i++) {
        if (!occupied[i]) {
            occupied[i] = true;
            return i + 1;
        }
    }
    return -1;
}

void ParkingSlotManager::releaseSlot(int slot) {
    if (slot >= 1 && slot <= MAX_PARKING_SLOTS) {
        occupied[slot - 1] = false;
    }
}

void ParkingSlotManager::showSlots() const {
    std::cout << "Parking Slot Status:\n";
    for (int i = 0; i < MAX_PARKING_SLOTS; i++) {
        std::cout << "Slot " << (i + 1) << ": " << (occupied[i] ? "Occupied" : "Available") << '\n';
    }
}

bool ParkingSlotManager::isSlotAvailable(int slot) const {
    return slot >= 1 && slot <= MAX_PARKING_SLOTS && !occupied[slot - 1];
}

int ParkingSlotManager::availableSlots() const {
    int count = 0;
    for (int i = 0; i < MAX_PARKING_SLOTS; i++) {
        if (!occupied[i]) count++;
    }
    return count;
}

// Linked List implementation
VehicleNode::VehicleNode(const Vehicle& vehicle) : data(vehicle), next(nullptr) {}

VehicleLinkedList::VehicleLinkedList() : head(nullptr) {}

VehicleLinkedList::~VehicleLinkedList() {
    clear();
}

void VehicleLinkedList::addVehicle(const Vehicle& vehicle) {
    VehicleNode* node = new VehicleNode(vehicle);
    node->next = head;
    head = node;
}

bool VehicleLinkedList::removeVehicle(const std::string& number, Vehicle& removed) {
    VehicleNode* prev = nullptr;
    VehicleNode* current = head;
    while (current) {
        if (current->data.number == number) {
            removed = current->data;
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

Vehicle* VehicleLinkedList::findVehicle(const std::string& number) {
    VehicleNode* current = head;
    while (current) {
        if (current->data.number == number) {
            return &current->data;
        }
        current = current->next;
    }
    return nullptr;
}

std::vector<Vehicle> VehicleLinkedList::getAllVehicles() const {
    std::vector<Vehicle> result;
    VehicleNode* current = head;
    while (current) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

void VehicleLinkedList::clear() {
    VehicleNode* current = head;
    while (current) {
        VehicleNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// UndoStack implementation
void UndoStack::push(const UndoAction& action) {
    undoStack.push(action);
}

bool UndoStack::pop(UndoAction& action) {
    if (undoStack.empty()) {
        return false;
    }
    action = undoStack.top();
    undoStack.pop();
    return true;
}

bool UndoStack::empty() const {
    return undoStack.empty();
}

// HistoryStack implementation
void HistoryStack::push(const Vehicle& vehicle) {
    stackHistory.push(vehicle);
}

std::vector<Vehicle> HistoryStack::getHistory() const {
    std::vector<Vehicle> result;
    std::stack<Vehicle> copy = stackHistory;
    while (!copy.empty()) {
        result.push_back(copy.top());
        copy.pop();
    }
    return result;
}

// VehicleQueue implementation
void VehicleQueue::enqueue(const Vehicle& vehicle) {
    queueVehicles.push(vehicle);
}

bool VehicleQueue::dequeue(Vehicle& vehicle) {
    if (queueVehicles.empty()) return false;
    vehicle = queueVehicles.front();
    queueVehicles.pop();
    return true;
}

bool VehicleQueue::empty() const {
    return queueVehicles.empty();
}

std::vector<Vehicle> VehicleQueue::listAll() const {
    std::vector<Vehicle> result;
    std::queue<Vehicle> copy = queueVehicles;
    while (!copy.empty()) {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}

// TollCircularQueue implementation
TollCircularQueue::TollCircularQueue() : frontIndex(0), rearIndex(-1), count(0) {}

bool TollCircularQueue::enqueue(const Vehicle& vehicle) {
    if (isFull()) return false;
    rearIndex = (rearIndex + 1) % MAX_TOLL_QUEUE;
    data[rearIndex] = vehicle;
    count++;
    return true;
}

bool TollCircularQueue::dequeue(Vehicle& vehicle) {
    if (isEmpty()) return false;
    vehicle = data[frontIndex];
    frontIndex = (frontIndex + 1) % MAX_TOLL_QUEUE;
    count--;
    return true;
}

bool TollCircularQueue::isFull() const {
    return count == MAX_TOLL_QUEUE;
}

bool TollCircularQueue::isEmpty() const {
    return count == 0;
}

void TollCircularQueue::showQueue() const {
    std::cout << "Toll Booth Queue:\n";
    if (isEmpty()) {
        std::cout << "No vehicles at toll booth.\n";
        return;
    }
    int idx = frontIndex;
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << data[idx].number << " (" << data[idx].type << ")\n";
        idx = (idx + 1) % MAX_TOLL_QUEUE;
    }
}

namespace {
    double calculateTollFee(const Vehicle& vehicle) {
        if (vehicle.type == "Car") return 50.0;
        if (vehicle.type == "Bike") return 20.0;
        if (vehicle.type == "Truck") return 100.0;
        return 0.0;
    }
}

// BST implementation
BSTNode::BSTNode(const Vehicle& vehicle) : data(vehicle), left(nullptr), right(nullptr) {}

BST::BST() : root(nullptr) {}

BST::~BST() {
    clear();
}

void BST::insert(const Vehicle& vehicle) {
    root = insertNode(root, vehicle);
}

BSTNode* BST::insertNode(BSTNode* node, const Vehicle& vehicle) {
    if (!node) return new BSTNode(vehicle);
    if (vehicle.number < node->data.number) {
        node->left = insertNode(node->left, vehicle);
    } else if (vehicle.number > node->data.number) {
        node->right = insertNode(node->right, vehicle);
    }
    return node;
}

bool BST::search(const std::string& number, Vehicle& found) const {
    return searchNode(root, number, found);
}

bool BST::searchNode(BSTNode* node, const std::string& number, Vehicle& found) const {
    if (!node) return false;
    if (node->data.number == number) {
        found = node->data;
        return true;
    }
    if (number < node->data.number) {
        return searchNode(node->left, number, found);
    }
    return searchNode(node->right, number, found);
}

bool BST::remove(const std::string& number) {
    root = removeNode(root, number);
    return true;
}

BSTNode* BST::removeNode(BSTNode* node, const std::string& number) {
    if (!node) return nullptr;
    if (number < node->data.number) {
        node->left = removeNode(node->left, number);
    } else if (number > node->data.number) {
        node->right = removeNode(node->right, number);
    } else {
        if (!node->left) {
            BSTNode* rightChild = node->right;
            delete node;
            return rightChild;
        } else if (!node->right) {
            BSTNode* leftChild = node->left;
            delete node;
            return leftChild;
        }
        BSTNode* minNode = findMin(node->right);
        node->data = minNode->data;
        node->right = removeNode(node->right, minNode->data.number);
    }
    return node;
}

BSTNode* BST::findMin(BSTNode* node) const {
    while (node && node->left) node = node->left;
    return node;
}

std::vector<Vehicle> BST::inorderTraversal() const {
    std::vector<Vehicle> list;
    inorder(root, list);
    return list;
}

void BST::inorder(BSTNode* node, std::vector<Vehicle>& list) const {
    if (!node) return;
    inorder(node->left, list);
    list.push_back(node->data);
    inorder(node->right, list);
}

void BST::clear() {
    clearTree(root);
    root = nullptr;
}

void BST::clearTree(BSTNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// AVL Tree implementation
AVLNode::AVLNode(const Vehicle& vehicle) : data(vehicle), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    clear();
}

int AVLTree::height(AVLNode* node) const {
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode* node) const {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* t2 = x->right;
    x->right = y;
    y->left = t2;
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* t2 = y->left;
    y->left = x;
    x->right = t2;
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));
    return y;
}

void AVLTree::insert(const Vehicle& vehicle) {
    root = insertNode(root, vehicle);
}

AVLNode* AVLTree::insertNode(AVLNode* node, const Vehicle& vehicle) {
    if (!node) return new AVLNode(vehicle);
    if (vehicle.number < node->data.number) {
        node->left = insertNode(node->left, vehicle);
    } else if (vehicle.number > node->data.number) {
        node->right = insertNode(node->right, vehicle);
    } else {
        return node;
    }
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && vehicle.number < node->left->data.number) return rotateRight(node);
    if (balance < -1 && vehicle.number > node->right->data.number) return rotateLeft(node);
    if (balance > 1 && vehicle.number > node->left->data.number) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && vehicle.number < node->right->data.number) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

bool AVLTree::search(const std::string& number, Vehicle& found) const {
    return searchNode(root, number, found);
}

bool AVLTree::searchNode(AVLNode* node, const std::string& number, Vehicle& found) const {
    if (!node) return false;
    if (node->data.number == number) {
        found = node->data;
        return true;
    }
    if (number < node->data.number) return searchNode(node->left, number, found);
    return searchNode(node->right, number, found);
}

AVLNode* AVLTree::getMinValueNode(AVLNode* node) const {
    AVLNode* current = node;
    while (current->left) current = current->left;
    return current;
}

bool AVLTree::remove(const std::string& number) {
    root = deleteNode(root, number);
    return true;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, const std::string& number) {
    if (!node) return nullptr;
    if (number < node->data.number) {
        node->left = deleteNode(node->left, number);
    } else if (number > node->data.number) {
        node->right = deleteNode(node->right, number);
    } else {
        if (!node->left || !node->right) {
            AVLNode* temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = getMinValueNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.number);
        }
    }
    if (!node) return node;
    node->height = 1 + std::max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0) return rotateRight(node);
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

std::vector<Vehicle> AVLTree::inorderTraversal() const {
    std::vector<Vehicle> list;
    inorder(root, list);
    return list;
}

void AVLTree::inorder(AVLNode* node, std::vector<Vehicle>& list) const {
    if (!node) return;
    inorder(node->left, list);
    list.push_back(node->data);
    inorder(node->right, list);
}

void AVLTree::clear() {
    clearTree(root);
    root = nullptr;
}

void AVLTree::clearTree(AVLNode* node) {
    if (!node) return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// HashTable implementation
HashTable::HashTable(int size) : table(size) {}

int HashTable::hashFunction(const std::string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % table.size();
}

void HashTable::insert(const Vehicle& vehicle) {
    int index = hashFunction(vehicle.number);
    for (auto& item : table[index]) {
        if (item.number == vehicle.number) {
            item = vehicle;
            return;
        }
    }
    table[index].push_back(vehicle);
}

bool HashTable::search(const std::string& number, Vehicle& found) const {
    int index = hashFunction(number);
    for (const auto& item : table[index]) {
        if (item.number == number) {
            found = item;
            return true;
        }
    }
    return false;
}

bool HashTable::remove(const std::string& number) {
    int index = hashFunction(number);
    auto& items = table[index];
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->number == number) {
            items.erase(it);
            return true;
        }
    }
    return false;
}

// EmergencyQueue implementation
void EmergencyQueue::add(const Vehicle& vehicle) {
    queue.push(vehicle);
}

bool EmergencyQueue::pop(Vehicle& vehicle) {
    if (queue.empty()) return false;
    vehicle = queue.top();
    queue.pop();
    return true;
}

bool EmergencyQueue::empty() const {
    return queue.empty();
}

std::vector<Vehicle> EmergencyQueue::listAll() const {
    std::vector<Vehicle> result;
    std::priority_queue<Vehicle, std::vector<Vehicle>, EmergencyCompare> copy = queue;
    while (!copy.empty()) {
        result.push_back(copy.top());
        copy.pop();
    }
    return result;
}

// Graph implementation
Graph::Graph(int nodes) : nodes(nodes), adjacency(nodes) {}

void Graph::addEdge(int u, int v) {
    if (u < 0 || v < 0 || u >= nodes || v >= nodes) return;
    adjacency[u].push_back(v);
    adjacency[v].push_back(u);
}

std::vector<int> Graph::bfs(int start) const {
    std::vector<bool> visited(nodes, false);
    std::vector<int> order;
    std::queue<int> q;
    if (start < 0 || start >= nodes) return order;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        order.push_back(node);
        for (int neighbor : adjacency[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    return order;
}

std::vector<int> Graph::dfs(int start) const {
    std::vector<bool> visited(nodes, false);
    std::vector<int> order;
    if (start < 0 || start >= nodes) return order;
    dfsHelper(start, visited, order);
    return order;
}

void Graph::dfsHelper(int node, std::vector<bool>& visited, std::vector<int>& order) const {
    visited[node] = true;
    order.push_back(node);
    for (int neighbor : adjacency[node]) {
        if (!visited[neighbor]) {
            dfsHelper(neighbor, visited, order);
        }
    }
}

void Graph::showEdges() const {
    std::cout << "City Road Network:\n";
    for (int i = 0; i < nodes; i++) {
        std::cout << "Node " << i << ": ";
        for (int neighbor : adjacency[i]) {
            std::cout << neighbor << " ";
        }
        std::cout << '\n';
    }
}

// TrafficSystem implementation
TrafficSystem::TrafficSystem() : cityGraph(8) {
    cityGraph.addEdge(0, 1);
    cityGraph.addEdge(0, 2);
    cityGraph.addEdge(1, 3);
    cityGraph.addEdge(2, 4);
    cityGraph.addEdge(3, 5);
    cityGraph.addEdge(4, 6);
    cityGraph.addEdge(5, 7);
    cityGraph.addEdge(6, 7);
}

void TrafficSystem::loadData() {
    std::ifstream input("backend/data/parking_data.txt");
    if (!input.is_open()) return;
    Vehicle vehicle;
    while (input >> vehicle.number >> vehicle.owner >> vehicle.type >> vehicle.emergency >> vehicle.slot >> vehicle.fee) {
        list.addVehicle(vehicle);
        bst.insert(vehicle);
        avl.insert(vehicle);
        hashTable.insert(vehicle);
        sortedVehicles.push_back(vehicle);
        parking.allocateSlot();
    }
}

void TrafficSystem::saveData() const {
    std::ofstream output("backend/data/parking_data.txt");
    auto vehicles = list.getAllVehicles();
    for (const auto& vehicle : vehicles) {
        output << vehicle.number << " " << vehicle.owner << " " << vehicle.type << " " << vehicle.emergency << " " << vehicle.slot << " " << vehicle.fee << "\n";
    }
}

bool TrafficSystem::addVehicle(const Vehicle& vehicle) {
    Vehicle copy = vehicle;
    if (copy.emergency) {
        std::cout << "Emergency vehicle bypassing toll: " << copy.number << "\n";
        int slot = parking.allocateSlot();
        if (slot < 0) {
            std::cout << "Parking full. Cannot add emergency vehicle.\n";
            return false;
        }
        copy.slot = slot;
        copy.fee = calculateFee(1);
        list.addVehicle(copy);
        bst.insert(copy);
        avl.insert(copy);
        hashTable.insert(copy);
        history.push(copy);
        emergency.add(copy);
        sortedVehicles.push_back(copy);
        std::cout << "Emergency vehicle " << copy.number << " parked in slot " << slot << "\n";
        return true;
    }

    enqueueIncoming(copy);
    std::cout << "Vehicle " << copy.number << " added to incoming queue.\n";
    if (!tollQueue.enqueue(copy)) {
        std::cout << "Toll booth queue is full. Vehicle waiting in incoming queue.\n";
        return false;
    }
    std::cout << "Vehicle " << copy.number << " added to toll queue.\n";

    Vehicle tolled = copy;
    if (tollQueue.dequeue(tolled)) {
        double tollFee = calculateTollFee(tolled);
        std::cout << "Toll paid for " << tolled.number << ": " << tollFee << "\n";
        int slot = parking.allocateSlot();
        if (slot < 0) {
            std::cout << "Parking full after toll. Cannot park vehicle.\n";
            return false;
        }
        tolled.slot = slot;
        tolled.fee = calculateFee(1);
        list.addVehicle(tolled);
        bst.insert(tolled);
        avl.insert(tolled);
        hashTable.insert(tolled);
        history.push(tolled);
        sortedVehicles.push_back(tolled);
        std::cout << "Vehicle " << tolled.number << " parked in slot " << slot << "\n";
        return true;
    }

    std::cout << "Unable to process toll for " << copy.number << ".\n";
    return false;
}

bool TrafficSystem::removeVehicle(const std::string& number) {
    Vehicle removed;
    if (!list.removeVehicle(number, removed)) {
        std::cout << "Vehicle not found.\n";
        return false;
    }
    parking.releaseSlot(removed.slot);
    bst.remove(number);
    avl.remove(number);
    hashTable.remove(number);
    undoStack.push({removed, removed.slot});
    double exitToll = calculateTollFee(removed);
    std::cout << "Vehicle " << number << " removed from slot " << removed.slot << ". Exit toll: " << exitToll << "\n";
    return true;
}

bool TrafficSystem::searchVehicle(const std::string& number, Vehicle& found) const {
    if (hashTable.search(number, found)) {
        return true;
    }
    return bst.search(number, found);
}

void TrafficSystem::displayAllVehicles() const {
    auto vehicles = list.getAllVehicles();
    std::cout << "All parked vehicles:\n";
    for (const auto& vehicle : vehicles) {
        std::cout << vehicle.number << " | " << vehicle.owner << " | " << vehicle.type << " | Slot " << vehicle.slot << " | Fee " << vehicle.fee << "\n";
    }
}

void TrafficSystem::showParkingStatus() const {
    parking.showSlots();
    std::cout << "Available slots: " << parking.availableSlots() << "\n";
}

void TrafficSystem::enqueueIncoming(const Vehicle& vehicle) {
    incomingQueue.enqueue(vehicle);
}

void TrafficSystem::dequeueIncoming() {
    Vehicle vehicle;
    if (incomingQueue.dequeue(vehicle)) {
        std::cout << "Incoming vehicle served: " << vehicle.number << "\n";
        enqueueToll(vehicle);
        std::cout << "Vehicle " << vehicle.number << " forwarded to toll queue.\n";
    } else {
        std::cout << "No incoming vehicles waiting.\n";
    }
}

void TrafficSystem::showIncomingQueue() const {
    auto items = incomingQueue.listAll();
    std::cout << "Incoming Vehicle Queue:\n";
    for (const auto& vehicle : items) {
        std::cout << vehicle.number << " | " << vehicle.type << "\n";
    }
}

void TrafficSystem::enqueueToll(const Vehicle& vehicle) {
    if (!tollQueue.enqueue(vehicle)) {
        std::cout << "Toll booth queue is full.\n";
    }
}

void TrafficSystem::dequeueToll() {
    Vehicle vehicle;
    if (tollQueue.dequeue(vehicle)) {
        std::cout << "Vehicle passed toll: " << vehicle.number << "\n";
    } else {
        std::cout << "No vehicle at toll booth.\n";
    }
}

void TrafficSystem::showTollQueue() const {
    tollQueue.showQueue();
}

void TrafficSystem::processEmergency() {
    Vehicle vehicle;
    if (emergency.pop(vehicle)) {
        std::cout << "Emergency vehicle prioritized: " << vehicle.number << "\n";
    } else {
        std::cout << "No emergency vehicles waiting.\n";
    }
}

void TrafficSystem::showRoadNetwork() const {
    cityGraph.showEdges();
}

void TrafficSystem::routeBFS(int start) const {
    auto order = cityGraph.bfs(start);
    std::cout << "BFS route order: ";
    for (int node : order) std::cout << node << " ";
    std::cout << '\n';
}

void TrafficSystem::routeDFS(int start) const {
    auto order = cityGraph.dfs(start);
    std::cout << "DFS route order: ";
    for (int node : order) std::cout << node << " ";
    std::cout << '\n';
}

void TrafficSystem::undoLastRemoval() {
    UndoAction action;
    if (!undoStack.pop(action)) {
        std::cout << "Nothing to undo.\n";
        return;
    }
    parking.allocateSlot();
    list.addVehicle(action.vehicle);
    bst.insert(action.vehicle);
    avl.insert(action.vehicle);
    hashTable.insert(action.vehicle);
    std::cout << "Undo completed: " << action.vehicle.number << " restored to slot " << action.slot << "\n";
}

void TrafficSystem::showHistory() const {
    auto historyList = history.getHistory();
    std::cout << "Vehicle history stack:\n";
    for (const auto& vehicle : historyList) {
        std::cout << vehicle.number << " | " << vehicle.owner << "\n";
    }
}

double TrafficSystem::calculateFee(int hours) const {
    double baseRate = 35.0;
    double hourly = 15.0;
    return baseRate + hours * hourly;
}

void TrafficSystem::sortVehicles() {
    sortedVehicles = list.getAllVehicles();
    // Quick Sort by vehicle number
    std::sort(sortedVehicles.begin(), sortedVehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.number < b.number;
    });
}

void TrafficSystem::displaySortedVehicles() const {
    std::cout << "Sorted vehicles by number:\n";
    for (const auto& vehicle : sortedVehicles) {
        std::cout << vehicle.number << " | " << vehicle.owner << "\n";
    }
}
