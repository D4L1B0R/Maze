# Maze Generator & Solver (C++ OOP Project)

**Course:** Object-Oriented Programming 2  
**Language:** C++  
**Project Type:** Console-based maze generator and solver  

---

### 📌 Overview  
This project implements a maze generator and solver using object-oriented programming principles in C++.  
The maze is divided into sectors, with a procedural generation algorithm that ensures a single continuous safe path from entrance to exit. The project includes:

- Sector-based maze representation  
- Progressive maze generation with fallback strategies  
- BFS validation for ensuring path connectivity  
- Maze visualization with marked safe path  
- AI movement logic (e.g., Minotaur chasing the player)  
- Interactive console interface for gameplay and testing

---

### 🧩 Features

- Procedural maze generation with randomized sector expansion  
- Multi-level fallback system to avoid dead-ends and disconnected paths  
- Visualization of the maze with clear markings for entrances, exits, and safe paths  
- AI entities with simple pathfinding logic  
- Player controls and interaction in the maze environment  
- Use of C++ smart pointers, STL containers, and custom hash functions

---

### 🛠 Technologies Used

- **C++14**
- Standard Template Library (STL)  
- Smart pointers (`std::unique_ptr`)  
- Custom hashing for coordinate pairs  
- Console output with ANSI color codes (optional)  
