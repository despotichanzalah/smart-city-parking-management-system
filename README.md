# Smart City Traffic & Parking Management System

This project is a complete semester-level DSA project with a C++ backend and a separate HTML/CSS/JS frontend.

## Structure

- `backend/`
  - `include/traffic_system.h` - core data structure definitions and class declarations.
  - `src/traffic_system.cpp` - backend implementation with arrays, linked lists, stacks, queues, trees, hash table, heap, and graph.
  - `src/main.cpp` - console menu and user interaction for the system.
  - `data/parking_data.txt` - persistent storage for saved records.
- `frontend/`
  - `index.html` - login page.
  - `dashboard.html` - main dashboard layout.
  - `parking.html` - parking management UI.
  - `traffic.html` - traffic and toll simulation UI.
  - `search.html` - vehicle search UI.
  - `trees.html` - AVL/BST visualization page.
  - `graph.html` - graph visualization page.
  - `history.html` - activity history page.
  - `css/style.css` - shared dashboard styles.
  - `js/app.js` - front-end interactivity.
- `docs/` - documentation and project report files.

## Setup Instructions

### Backend
1. Open `backend/` in VS Code.
2. Use the C++ compiler of your choice.
3. Compile the project:
   - On Windows with MinGW:
     ```powershell
     g++ src/main.cpp src/traffic_system.cpp -o SmartCityTraffic.exe
     ```
   - On Linux/macOS:
     ```bash
     g++ src/main.cpp src/traffic_system.cpp -o SmartCityTraffic
     ```
4. Run the executable and use the console menu.

### Frontend
1. Open `frontend/` in your browser.
2. Start from `index.html`.
3. Use the sidebar to navigate pages.

## Features

- Add, remove, search, and display vehicles.
- Parking slot management with fixed-size arrays.
- Dynamic vehicle records using linked list.
- Undo removal and history tracking with stacks.
- Incoming vehicle queue and toll circular queue.
- BST and AVL trees for sorted vehicle storage.
- Hash table for fast lookup.
- Emergency priority queue.
- City road network graph with BFS and DFS traversal.
- File save and load for persistent data.

## Notes

This project is designed to be beginner-friendly while still using professional DSA concepts and clean modular C++ code.
