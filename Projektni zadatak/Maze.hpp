// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef MAZE_HEADER
#define MAZE_HEADER

#include <iostream>
#include <map>
#include <memory>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <chrono>
#include "Type.hpp"
#include "Sector.hpp"
#include "CoordHash.hpp"

// Class Maze-> representing the whole maze with all the elements that includes it
class Maze {
private:
    int row, col;
    int obj;    // --> number of objects in the maze at the start
    Coord entrance;
    Coord exit;
    std::map<Coord, std::unique_ptr<Type>> maze;
    std::unordered_set<Coord> safePath; // --> path made by generating the sectors
    std::vector<Coord> finalPath;   // --> one of the paths from U to I exclude from safePath
    Coord robotPos;
    Coord minotaurPos;
    bool showPath = false;
    bool gameOver = false;
    bool gameWon = false;
    std::unordered_map<Coord, char> powerUpTypes;   // --> location → type ('F', 'S', 'H', 'B')
    bool hasSword = false;
    bool hasShield = false;
    bool hasHammer = false;
    bool fogActive = false;

    // The idea of magic objects is to extend time period every (+3s) for the objects of the same class
    int fogTimer = 0;
    int hammerTimer = 0;
    int shieldTimer = 0;
    int swordTimer = 0;

    // Status of the Minotaur
    bool minotaurAlive = true;
    int minotaurStunTimer = 0;

public:
    Maze(int row, int col, int obj);
    void generateMaze();
    bool inBounds(Coord pos) const;
    void placeObjects();
    void placeInternalWalls();
    Direction getBiasedDirection(Coord current, Coord target, int fallbackLevel);
    bool closeEnough(Coord a, Coord b);
    bool inSameSector(Coord a, Coord b);
    void printMaze();
    void extractShortestPath();
    void applyEffect(char effect);
    char getCharAt(Coord pos) const;
    bool isWalkable(Coord pos) const;
    Coord getEntrance() const { return entrance; }
    Coord getExit() const { return exit; }
    Coord getEntranceBelow() const { return { entrance.first + 1, entrance.second }; }
    Coord getExitAbove() const { return { exit.first - 1, exit.second }; }
    char getEffectAt(Coord pos) const;
    std::unordered_map<Coord, char>& getPowerUpTypes() { return powerUpTypes; }
    void erasePowerUpAt(Coord pos);
    bool isAdjacentUpDownLeftRight(Coord a, Coord b);
    Coord getRandomMinotaurStart();
    void breakWallAt(Coord pos);
    bool isInFinalPath(Coord c) const;
    void toggleHighlightPath();

    // *** Getters and Setters ***
    bool getGameOver() { return gameOver; };
    void setGameOver(bool over) { gameOver = over; }

    bool getGameWon() { return gameWon; };
    void setGameWon(bool won) { gameWon = won; }

    Coord getRobotPos() const { return robotPos; }
    void setRobotPos(Coord pos) { robotPos = pos; }

    Coord getMinotaurPos() const { return minotaurPos; }
    void setMinotaurPos(Coord newPos);

    bool getMinotaurAlive() { return minotaurAlive; }
    void setMinotaurAlive(bool status) { minotaurAlive = status; }

    char getHammer() const { return hasHammer; }
    void setHammer(bool used) { hasHammer = used; }

    char getShield() const { return hasShield; }
    void setShield(bool used) { hasShield = used; }

    char getSword() const { return hasSword; }
    void setSword(bool used) { hasSword = used; }

    char getFogActive() const { return fogActive; }
    void setFogActive(bool used) { fogActive = used; }

    int getFogTimer() const { return fogTimer; }
    void setFogTimer(int timer) { fogTimer = timer; }

    int getSwordTimer() const { return swordTimer; }
    void setSwordTimer(int timer) { swordTimer = timer; }

    int getShieldTimer() const { return shieldTimer; }
    void setShieldTimer(int timer) { shieldTimer = timer; }

    int getHammerTimer() const { return hammerTimer; }
    void setHammerTimer(int timer) { hammerTimer = timer; }

    char getMinotaurStunTimer() const { return minotaurStunTimer; }
    void setMinotaurStunTimer(int stunTime) { minotaurStunTimer = stunTime; }
};

#endif