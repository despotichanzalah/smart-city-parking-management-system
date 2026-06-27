#ifndef TRAFFIC_SYSTEM_H
#define TRAFFIC_SYSTEM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <list>
#include <unordered_map>

const int MAX_PARKING_SLOTS = 50;
const int MAX_TOLL_QUEUE = 20;

struct Vehicle {
    std::string number;
    std::string owner;
    std::string type;
    bool emergency;
    int slot;
    double fee;
};

struct UndoAction {
    Vehicle vehicle;
    int slot;
};

class ParkingSlotManager {
public:
    ParkingSlotManager();
    int allocateSlot();
    void releaseSlot(int slot);
    void showSlots() const;
    bool isSlotAvailable(int slot) const;
    int availableSlots() const;
private:
    bool occupied[MAX_PARKING_SLOTS];
};

class VehicleNode {
public:
    Vehicle data;
    VehicleNode* next;
    VehicleNode(const Vehicle& vehicle);
};

class VehicleLinkedList {
public:
    VehicleLinkedList();
    ~VehicleLinkedList();
    void addVehicle(const Vehicle& vehicle);
    bool removeVehicle(const std::string& number, Vehicle& removed);
    Vehicle* findVehicle(const std::string& number);
    std::vector<Vehicle> getAllVehicles() const;
    void clear();
private:
    VehicleNode* head;
};

class UndoStack {
public:
    void push(const UndoAction& action);
    bool pop(UndoAction& action);
    bool empty() const;
private:
    std::stack<UndoAction> undoStack;
};

class HistoryStack {
public:
    void push(const Vehicle& vehicle);
    std::vector<Vehicle> getHistory() const;
private:
    std::stack<Vehicle> stackHistory;
};

class VehicleQueue {
public:
    void enqueue(const Vehicle& vehicle);
    bool dequeue(Vehicle& vehicle);
    bool empty() const;
    std::vector<Vehicle> listAll() const;
private:
    std::queue<Vehicle> queueVehicles;
};

class TollCircularQueue {
public:
    TollCircularQueue();
    bool enqueue(const Vehicle& vehicle);
    bool dequeue(Vehicle& vehicle);
    bool isFull() const;
    bool isEmpty() const;
    void showQueue() const;
private:
    Vehicle data[MAX_TOLL_QUEUE];
    int frontIndex;
    int rearIndex;
    int count;
};

struct BSTNode {
    Vehicle data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Vehicle& vehicle);
};

class BST {
public:
    BST();
    ~BST();
    void insert(const Vehicle& vehicle);
    bool search(const std::string& number, Vehicle& found) const;
    bool remove(const std::string& number);
    std::vector<Vehicle> inorderTraversal() const;
    void clear();
private:
    BSTNode* root;
    BSTNode* insertNode(BSTNode* node, const Vehicle& vehicle);
    BSTNode* removeNode(BSTNode* node, const std::string& number);
    BSTNode* findMin(BSTNode* node) const;
    bool searchNode(BSTNode* node, const std::string& number, Vehicle& found) const;
    void inorder(BSTNode* node, std::vector<Vehicle>& list) const;
    void clearTree(BSTNode* node);
};

struct AVLNode {
    Vehicle data;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const Vehicle& vehicle);
};

class AVLTree {
public:
    AVLTree();
    ~AVLTree();
    void insert(const Vehicle& vehicle);
    bool search(const std::string& number, Vehicle& found) const;
    bool remove(const std::string& number);
    std::vector<Vehicle> inorderTraversal() const;
    void clear();
private:
    AVLNode* root;
    int height(AVLNode* node) const;
    int getBalance(AVLNode* node) const;
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, const Vehicle& vehicle);
    AVLNode* deleteNode(AVLNode* node, const std::string& number);
    AVLNode* getMinValueNode(AVLNode* node) const;
    bool searchNode(AVLNode* node, const std::string& number, Vehicle& found) const;
    void inorder(AVLNode* node, std::vector<Vehicle>& list) const;
    void clearTree(AVLNode* node);
};

class HashTable {
public:
    HashTable(int size = 101);
    void insert(const Vehicle& vehicle);
    bool search(const std::string& number, Vehicle& found) const;
    bool remove(const std::string& number);
private:
    std::vector<std::list<Vehicle>> table;
    int hashFunction(const std::string& key) const;
};

class EmergencyQueue {
public:
    void add(const Vehicle& vehicle);
    bool pop(Vehicle& vehicle);
    bool empty() const;
    std::vector<Vehicle> listAll() const;
private:
    struct EmergencyCompare {
        bool operator()(const Vehicle& a, const Vehicle& b) const {
            return a.emergency < b.emergency || (a.emergency == b.emergency && a.number > b.number);
        }
    };
    std::priority_queue<Vehicle, std::vector<Vehicle>, EmergencyCompare> queue;
};

class Graph {
public:
    Graph(int nodes = 8);
    void addEdge(int u, int v);
    std::vector<int> bfs(int start) const;
    std::vector<int> dfs(int start) const;
    void showEdges() const;
private:
    int nodes;
    std::vector<std::vector<int>> adjacency;
    void dfsHelper(int node, std::vector<bool>& visited, std::vector<int>& order) const;
};

class TrafficSystem {
public:
    TrafficSystem();
    void loadData();
    void saveData() const;
    bool addVehicle(const Vehicle& vehicle);
    bool removeVehicle(const std::string& number);
    bool searchVehicle(const std::string& number, Vehicle& found) const;
    void displayAllVehicles() const;
    void showParkingStatus() const;
    void enqueueIncoming(const Vehicle& vehicle);
    void dequeueIncoming();
    void showIncomingQueue() const;
    void enqueueToll(const Vehicle& vehicle);
    void dequeueToll();
    void showTollQueue() const;
    void processEmergency();
    void showRoadNetwork() const;
    void routeBFS(int start) const;
    void routeDFS(int start) const;
    void undoLastRemoval();
    void showHistory() const;
    double calculateFee(int hours) const;
    void sortVehicles();
    void displaySortedVehicles() const;
private:
    ParkingSlotManager parking;
    VehicleLinkedList list;
    UndoStack undoStack;
    HistoryStack history;
    VehicleQueue incomingQueue;
    TollCircularQueue tollQueue;
    BST bst;
    AVLTree avl;
    HashTable hashTable;
    EmergencyQueue emergency;
    Graph cityGraph;
    std::vector<Vehicle> sortedVehicles;
};

#endif // TRAFFIC_SYSTEM_H
