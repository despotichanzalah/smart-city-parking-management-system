#include "../include/traffic_system.h"
#include <iostream>
#include <limits>

void showMenu() {
    std::cout << "\n--- Smart City Traffic & Parking Management System ---\n";
    std::cout << "1. Add vehicle\n";
    std::cout << "2. Remove vehicle\n";
    std::cout << "3. Search vehicle\n";
    std::cout << "4. Display all vehicles\n";
    std::cout << "5. Show parking status\n";
    std::cout << "6. Emergency vehicle handling\n";
    std::cout << "7. Incoming traffic queue\n";
    std::cout << "8. Toll booth simulation\n";
    std::cout << "9. Route traversal\n";
    std::cout << "10. Vehicle history\n";
    std::cout << "11. Undo last removal\n";
    std::cout << "12. Save data and exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    TrafficSystem system;
    system.loadData();
    bool running = true;
    while (running) {
        showMenu();
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: {
                Vehicle v;
                std::cout << "Enter vehicle number: ";
                std::getline(std::cin, v.number);
                std::cout << "Enter owner name: ";
                std::getline(std::cin, v.owner);
                std::cout << "Enter vehicle type (Car/Bike/Truck): ";
                std::getline(std::cin, v.type);
                std::cout << "Is emergency vehicle? 1 = Yes, 0 = No: ";
                std::cin >> v.emergency;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                system.addVehicle(v);
                break;
            }
            case 2: {
                std::string number;
                std::cout << "Enter vehicle number to remove: ";
                std::getline(std::cin, number);
                system.removeVehicle(number);
                break;
            }
            case 3: {
                std::string number;
                std::cout << "Enter vehicle number to search: ";
                std::getline(std::cin, number);
                Vehicle found;
                if (system.searchVehicle(number, found)) {
                    std::cout << "Found: " << found.number << " | " << found.owner << " | " << found.type << " | Slot " << found.slot << "\n";
                } else {
                    std::cout << "Vehicle not found.\n";
                }
                break;
            }
            case 4:
                system.displayAllVehicles();
                break;
            case 5:
                system.showParkingStatus();
                break;
            case 6:
                system.processEmergency();
                break;
            case 7: {
                std::cout << "1. Add incoming vehicle\n";
                std::cout << "2. Serve incoming vehicle\n";
                int option;
                std::cin >> option;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (option == 1) {
                    Vehicle v;
                    std::cout << "Vehicle number: ";
                    std::getline(std::cin, v.number);
                    std::cout << "Vehicle type: ";
                    std::getline(std::cin, v.type);
                    system.enqueueIncoming(v);
                } else {
                    system.dequeueIncoming();
                }
                system.showIncomingQueue();
                break;
            }
            case 8: {
                std::cout << "1. Add toll vehicle\n";
                std::cout << "2. Process toll vehicle\n";
                int option;
                std::cin >> option;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (option == 1) {
                    Vehicle v;
                    std::cout << "Vehicle number: ";
                    std::getline(std::cin, v.number);
                    std::cout << "Vehicle type: ";
                    std::getline(std::cin, v.type);
                    system.enqueueToll(v);
                } else {
                    system.dequeueToll();
                }
                system.showTollQueue();
                break;
            }
            case 9: {
                std::cout << "Enter start node (0-7): ";
                int start;
                std::cin >> start;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                system.routeBFS(start);
                system.routeDFS(start);
                break;
            }
            case 10:
                system.showHistory();
                break;
            case 11:
                system.undoLastRemoval();
                break;
            case 12:
                system.saveData();
                running = false;
                break;
            default:
                std::cout << "Invalid selection. Please try again.\n";
                break;
        }
    }
    return 0;
}
//Avl trees arer in group

