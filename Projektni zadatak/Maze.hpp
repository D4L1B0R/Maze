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

class Maze {
private:
    int row, col, obj;
    Coord entrance;
    Coord exit;
    std::map<Coord, std::unique_ptr<Type>> maze;
    std::unordered_set<Coord> safePath;
    std::vector<Coord> finalPath;
    Coord robotPos;
    Coord minotaurPos;
    bool showPath = false;
    bool gameOver = false;
    bool gameWon = false;
    std::unordered_map<Coord, char> powerUpTypes; // location → type ('F', 'S', 'H', 'B')
    int effectTimer = 0;
    char activeEffect = ' '; // 'F' - fog, 'S' - sword, 'H' - shield, 'B' - hammer
    int shieldTimer = 0;
    bool hasSword = false;
    bool hasShield = false;
    bool hasHammer = false;
    bool fogActive = false;
    int fogTimer = 0;
    bool hammerUsed = false;
    bool shieldUsed = false;
    bool swordUsed = false;
    int hammerUsesLeft = 0;
    int shieldUsesLeft = 0;
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
    bool getGameOver() { return gameOver; };
    void setGameOver(bool over) { gameOver = over; }
    bool getGameWon() { return gameWon; };
    void setGameWon(bool won) { gameWon = won; }
    char getCharAt(Coord pos) const;
    bool isWalkable(Coord pos) const;
    Coord getEntrance() const { return entrance; }
    Coord getExit() const { return exit; }
    Coord getEntranceBelow() const { return { entrance.first + 1, entrance.second }; }
    Coord getExitAbove() const { return { exit.first - 1, exit.second }; }
    char getEffectAt(Coord pos) const {
        auto it = powerUpTypes.find(pos);
        return (it != powerUpTypes.end()) ? it->second : ' ';
    }
    std::unordered_map<Coord, char>& getPowerUpTypes() { return powerUpTypes; }
    void erasePowerUpAt(Coord pos);
    Coord getRobotPos() const { return robotPos; }
    void setRobotPos(Coord pos) { robotPos = pos; }
    Coord getMinotaurPos() const { return minotaurPos; }
    void setMinotaurPos(Coord newPos);
    char getActiveEffect() const { return activeEffect; }
    void setActiveEffect(char c) { activeEffect = c; }
    int getShieldTimer() const { return shieldTimer; }
    void setShieldTimer(int newShieldTimer) { shieldTimer = newShieldTimer; }
    int getEffectTimer() const { return effectTimer; }
    void setEffectTimer(int newEffectTimer) { effectTimer = newEffectTimer; }
    void breakWallAt(Coord pos);
    bool isInFinalPath(Coord c) const;
    void toggleHighlightPath();
    char getHammerUsed() const { return hammerUsed; }
    void setHammerUsed(bool used) { hammerUsed = used; }
    char getSwordUsed() const { return swordUsed; }
    void setSwordUsed(bool used) { swordUsed = used; }
    char getShieldUsed() const { return shieldUsed; }
    void setShieldUsed(bool used) { shieldUsed = used; }
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
    bool isAdjacentUpDownLeftRight(Coord a, Coord b);
    Coord getRandomMinotaurStart();
    int getHammerUsesLeft() const { return hammerUsesLeft; }
    void setHammerUsesLeft(int hammerUses) { hammerUsesLeft = hammerUses; }
    int getShieldUsesLeft() const { return shieldUsesLeft; }
    void setShieldUsesLeft(int shieldUses) { shieldUsesLeft = shieldUses; }
    void decrementHammerUses();
    char getMinotaurStunTimer() const { return minotaurStunTimer; }
    void setMinotaurStunTimer(int stunTime) { minotaurStunTimer = stunTime; }
};

#endif