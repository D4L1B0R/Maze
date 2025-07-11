﻿// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

// *** Resolving all the functions that are in charge of generating the maze, connecting sectors and monitoring the process ***

/*  The idea of finding the way out of the maze is to follow safe path to it's exit point
    but in first place we need to generate that maze, to do that, we will seperate it into sectors with the same dimensions
    inside each sector the path is drown, that leads from one point to another on the border of it
    beginning with sector bellow the entrance point 'U' where it's exit will also be the entrance of the next sector
    following the idea, we get, using 3 lvl. fallback system:
                                                            -> ** 1st lvl. ** sectors begin to colide, changing the direction, rising the bias
                                                            -> ** 2nd lvl. ** sectors more and more colide, removing last few sectors, rising bias even more
                                                                (NOTE: Through the time risises the danger and number sectors to remove)
                                                            -> ** 3rd lvl. ** final resolution, deleting the whole path and starting from beginning
    
    then connect them until we reach the sector till the ending, then priority is to connect current position to the exit point*/


#include <queue>
#include <algorithm>
#include <random>
#include <chrono> // --> using it for getting maze generation time
#include "Maze.hpp"
#include "Fog.hpp"
#include "Sword.hpp"
#include "Shield.hpp"
#include "Hammer.hpp"

Maze::Maze(int row, int col, int obj) : row(row), col(col), obj(obj) {
    entrance = { 0, randomNumberGenerator(1, col - 2) };
    exit = { row - 1, randomNumberGenerator(1, col - 2) };

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (i == 0 || j == 0 || i == row - 1 || j == col - 1)
                maze[{i, j}] = std::make_unique<Type>('#');
            else
                maze[{i, j}] = std::make_unique<Type>('.');
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    // Initializing starting and ending point
    maze[entrance] = std::make_unique<Type>('U');
    maze[exit] = std::make_unique<Type>('I');

    // Calling the functions that operate with genrating the maze
    generateMaze();
    extractShortestPath();
    placeObjects();
    placeInternalWalls();

    robotPos = getEntranceBelow();
    minotaurPos = getRandomMinotaurStart();

    // For the start we put label 'safe path' on characters starting points
    maze[robotPos] = std::make_unique<Type>('.');
    maze[minotaurPos] = std::make_unique<Type>('.');

    safePath.insert(robotPos);
    safePath.insert(minotaurPos);

    // Time needed for generating the maze
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    std::cout << "Maze generated in " << duration.count() << " seconds.\n";
}

void Maze::generateMaze() {
    Coord current = { entrance.first + 1, entrance.second };
    safePath.clear();
    safePath.insert(entrance);
    maze[entrance] = std::make_unique<Type>('U');

    int fallbackLevel = 0;  // --> level of fallback system
    int fallbackAttempts = 0;   // --> number of failed attempts
    std::vector<Coord> sectorCheckpoints = { current };

    // Process of connecting sectors inside the maze
    while (!closeEnough(current, exit)) {
        Direction dir = getBiasedDirection(current, exit, fallbackLevel);
        Sector sector(current, dir, row, col);
        sector.generatePath(dir, row, col);

        Coord next = sector.getExit();
        bool validSector = (next != current) && !sector.path.empty();

        // Sector is valid, there are no issues, no colisions
        if (validSector) {
            for (const auto& coord : sector.path) {
                if (inBounds(coord)) {
                    maze[coord] = std::make_unique<Type>('.');
                    safePath.insert(coord);
                }
            }

            Coord bridge = current;
            while (bridge != next) {
                if (bridge.first < next.first) bridge.first++;
                else if (bridge.first > next.first) bridge.first--;
                else if (bridge.second < next.second) bridge.second++;
                else if (bridge.second > next.second) bridge.second--;

                if (inBounds(bridge)) {
                    maze[bridge] = std::make_unique<Type>('.');
                    safePath.insert(bridge);
                }
            }

            current = next;
            sectorCheckpoints.push_back(current);
            fallbackAttempts = 0;
            fallbackLevel = std::max(0, fallbackLevel - 1);
        }
        // Falling into colision, raising the fallbackAttempts number
        else {
            fallbackAttempts++;
            if (fallbackAttempts >= 3 && fallbackLevel == 0) fallbackLevel = 1;
            else if (fallbackAttempts >= 6 && fallbackLevel == 1) {
                fallbackLevel = 2;
                int eraseCount = std::min(2, (int)sectorCheckpoints.size() - 1);
                for (int i = 0; i < eraseCount; ++i) {
                    Coord remove = sectorCheckpoints.back();
                    sectorCheckpoints.pop_back();
                    safePath.erase(remove);
                    maze[remove] = std::make_unique<Type>('#');
                }
                current = sectorCheckpoints.back();
                fallbackAttempts = 0;
            }
            else if (fallbackAttempts >= 10) {
                for (auto& coord : safePath) {
                    if (maze[coord]->getChar() == '.')
                        maze[coord] = std::make_unique<Type>('#');
                }
                safePath.clear();
                current = { entrance.first + 1, entrance.second };
                safePath.insert(entrance);
                fallbackLevel = 0;
                fallbackAttempts = 0;
                sectorCheckpoints = { current };
            }
        }
    }

    // Point of connecting neighbouring sectors
    Coord iConnect = { exit.first - 1, exit.second };
    Coord finalize = current;

    while (finalize != iConnect) {
        if (finalize.first < iConnect.first) finalize.first++;
        else if (finalize.first > iConnect.first) finalize.first--;
        else if (finalize.second < iConnect.second) finalize.second++;
        else if (finalize.second > iConnect.second) finalize.second--;

        if (inBounds(finalize)) {
            maze[finalize] = std::make_unique<Type>('.');
            safePath.insert(finalize);
        }
    }

    if (inBounds(iConnect)) {
        maze[iConnect] = std::make_unique<Type>('.');
        safePath.insert(iConnect);
    }
    maze[exit] = std::make_unique<Type>('I');
    safePath.insert(exit);
}

// Finding the shortest path through the maze, using the bfs
void Maze::extractShortestPath() {
    finalPath.clear();
    std::unordered_map<Coord, Coord> parent;
    std::unordered_set<Coord> visited;
    std::queue<Coord> q;

    Coord start = getEntranceBelow();
    q.push(start);
    visited.insert(start);

    bool found = false;
    while (!q.empty()) {
        Coord current = q.front(); q.pop();
        if (current == exit) {
            found = true;
            break;
        }

        for (auto d : std::vector<Coord>{ {0,1}, {1,0}, {0,-1}, {-1,0} }) {
            Coord next = { current.first + d.first, current.second + d.second };
            if (safePath.count(next) && !visited.count(next)) {
                visited.insert(next);
                parent[next] = current;
                q.push(next);
            }
        }
    }

    if (found) {
        for (Coord at = exit; at != start; at = parent[at])
            finalPath.push_back(at);
        finalPath.push_back(start);
        std::reverse(finalPath.begin(), finalPath.end());
    }
}

// Putting the magic objects inside the maze
void Maze::placeObjects() {
    std::vector<Coord> pathTiles(safePath.begin(), safePath.end());
    std::shuffle(pathTiles.begin(), pathTiles.end(), std::mt19937(std::random_device{}()));

    std::vector<char> allTypes;
    allTypes.push_back('S');

    std::vector<char> otherTypes = { 'F', 'H', 'B' };
    int remaining = obj - 1;

    for (int i = 0; i < remaining; ++i) {
        allTypes.push_back(otherTypes[i % 3]);
    }

    std::shuffle(allTypes.begin(), allTypes.end(), std::mt19937(std::random_device{}()));

    int idx = 0;

    for (const Coord& p : pathTiles) {
        if (idx >= obj) break;
        if (maze[p]->getChar() != '.') continue;

        char chosen = allTypes[idx++];
        std::unique_ptr<Type> power;

        if (chosen == 'F') power = std::make_unique<Fog>();
        else if (chosen == 'S') power = std::make_unique<Sword>();
        else if (chosen == 'H') power = std::make_unique<Shield>();
        else if (chosen == 'B') power = std::make_unique<Hammer>();

        maze[p] = std::move(power);
        powerUpTypes[p] = chosen;
    }
}

// Putting the interiour walls in the maze
void Maze::placeInternalWalls() {
    double wallRatio = 0.45;
    int numWalls = static_cast<int>(row * col * wallRatio);
    int count = 0;
    while (count < numWalls) {
        int i = randomNumberGenerator(1, row - 2);
        int j = randomNumberGenerator(1, col - 2);
        Coord p = { i, j };
        if (maze[p]->getChar() == '.' && safePath.count(p) == 0) {
            maze[p] = std::make_unique<Type>('#');
            count++;
        }
    }
}

// Bias function - starts from equal chances for each direction (25%)
Direction Maze::getBiasedDirection(Coord current, Coord target, int fallbackLevel) {
    int dx = target.second - current.second;
    int dy = target.first - current.first;
    int bias = std::min(20 * (fallbackLevel + 1), 100);

    std::vector<std::pair<Direction, int>> directions = {
        {DOWN, 25}, {UP, 25}, {RIGHT, 25}, {LEFT, 25}
    };

    if (abs(dx) > abs(dy)) {
        if (dx > 0) directions[2].second += bias;
        else directions[3].second += bias;
    }
    else {
        if (dy > 0) directions[0].second += bias;
        else directions[1].second += bias;
    }

    int total = 0;
    for (const auto& d : directions) total += d.second;
    int choice = randomNumberGenerator(1, total);
    int acc = 0;
    for (const auto& d : directions) {
        acc += d.second;
        if (choice <= acc) return d.first;
    }
    return DOWN;
}

// Figuring out if two coordinates arre close to each other
bool Maze::closeEnough(Coord a, Coord b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second) <= 6;
}

void Maze::printMaze() {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            Coord c = { i, j };
            if (!gameOver) {
                // Fog is active - field of view is reduced, so only characters near Robot are showing on the screen
                bool isVisible = !getFogActive() || std::abs(robotPos.first - i) <= 3 && std::abs(robotPos.second - j) <= 3;

                if (!isVisible) {
                    std::cout << " ";
                    continue;
                }
            }

            if (c == robotPos) {
                std::cout << "\033[34mR\033[0m";    // --> Robot Robert (blue 'R')
            }
            else if (c == minotaurPos) {
                if (minotaurStunTimer > 0)
                    std::cout << "\033[90mM\033[0m";    // --> Minotaur in cooldown state (gray 'M')
                else
                    std::cout << "\033[31mM\033[0m";    // --> Minotaur (red 'M')
            }
            else if (maze[c]->getChar() == 'P') {
                char code = getPowerTypeAt(c, powerUpTypes);
                if (code == 'F') std::cout << "\033[90mP\033[0m";   // --> Fog (gray 'P')
                else if (code == 'S') std::cout << "\033[31mP\033[0m";  // --> Sword (red 'P')
                else if (code == 'H') std::cout << "\033[34mP\033[0m";  // --> Shield (blue 'P')
                else if (code == 'B') std::cout << "\033[33mP\033[0m";  // --> Hammer (yellow 'P')
                else std::cout << "P";
            }
            else if (showPath && isInFinalPath(c)) {
                if (c == robotPos) std::cout << "\033[36mR\033[0m";
                else if (c == minotaurPos) std::cout << "\033[35mM\033[0m";
                else if (maze[c]->getChar() == 'U' || maze[c]->getChar() == 'I') std::cout << maze[c]->getChar();
                else std::cout << "\033[33m*\033[0m";   // --> Safe path (yellow '*')
            }
            else if (getFogActive() && std::abs(robotPos.first - i) + std::abs(robotPos.second - j) > 3) {
                std::cout << " ";
            }
            else {
                std::cout << maze[c]->getChar();
            }
        }
        std::cout << '\n';
    }

    // Ending writing based on the outcome
    if (gameOver && !gameWon) std::cout << "\n\033[31mYou were caught by the Minotaur!\033[0m\n";
    else if (gameWon) std::cout << "\n\033[32mYou escaped the maze!\033[0m\n";
}

// Determine if the two positions are in the same sector
bool Maze::inSameSector(Coord a, Coord b) {
    return std::abs(a.first - b.first) <= 5 && std::abs(a.second - b.second) <= 5;
}

//  Checking if currecnt position is in the final path
bool Maze::isInFinalPath(Coord c) const {
    return std::find(finalPath.begin(), finalPath.end(), c) != finalPath.end();
}

// Informing about the usage of the special effects
void Maze::applyEffect(char effect) {
    switch (effect) {
    case 'F':
        fogActive = true;
        fogTimer = 5;
        std::cout << "\nEffect Activated: Fog - Vision reduced.\n";
        break;
    case 'S':
        if (!hasSword) std::cout << "\nSword collected.\n";
        hasSword = true;
        break;
    case 'H':
        if (!hasShield) std::cout << "\nShield collected.\n";
        hasShield = true;
        break;
    case 'B':
        if (!hasHammer) std::cout << "\nHammer collected.\n";
        hasHammer = true;
        break;
    }
}

bool Maze::inBounds(Coord pos) const {
    return pos.first > 0 && pos.first < row - 1 &&
        pos.second > 0 && pos.second < col - 1;
}

char Maze::getCharAt(Coord pos) const {
    auto it = maze.find(pos);
    if (it != maze.end()) return it->second->getChar();
    return '#';
}

// Checking if the next position is valid (inside the maze, not the wall or stunned Minotaur)
bool Maze::isWalkable(Coord pos) const {
    if (!inBounds(pos)) return false;
    if (pos == minotaurPos) return false;
    return maze.at(pos)->getChar() != '#';
}

void Maze::erasePowerUpAt(Coord pos) {
    powerUpTypes.erase(pos);
    maze[pos] = std::make_unique<Type>('.');
}

void Maze::breakWallAt(Coord pos) {
    if (inBounds(pos) && maze[pos]->getChar() == '#') {
        maze[pos] = std::make_unique<Type>('.');
        safePath.insert(pos);
    }
}

// Based on the state of Minotaur, we get him positioned
void Maze::setMinotaurPos(Coord newPos) {
    if (minotaurAlive) {
        if (inBounds(minotaurPos)) {
            maze[minotaurPos] = std::make_unique<Type>('.');
        }
        if (inBounds(newPos) && maze[newPos]->getChar() != '#') {
            minotaurPos = newPos;

            if (!gameOver && newPos != Coord {-1, -1}) {
                maze[minotaurPos] = std::make_unique<Type>('M');
            }
        }
    }

    else {
        maze[minotaurPos] = std::make_unique<Type>('.');
        minotaurPos = Coord { -1,-1 };
    }
}

// Randomized Minotaur's starting point
Coord Maze::getRandomMinotaurStart() {
    std::vector<Coord> candidates;

    for (const auto& pos : safePath) {
        int distRobot = abs(pos.first - robotPos.first) + abs(pos.second - robotPos.second);
        if (distRobot >= 5) {
            if (!isAdjacentUpDownLeftRight(pos, entrance) && !isAdjacentUpDownLeftRight(pos, exit)) {
                if (maze[pos]->getChar() == '.') {
                    candidates.push_back(pos);
                }
            }
        }
    }

    if (candidates.empty()) {
        for (int i = 1; i < row - 1; ++i) {
            for (int j = 1; j < col - 1; ++j) {
                Coord c = { i, j };
                if (maze[c]->getChar() == '.' && safePath.count(c)) {
                    candidates.push_back(c);
                }
            }
        }
    }

    if (candidates.empty()) {
        std::cout << "No suitable candidates for Minotaur, placing near exit\n";
        return { exit.first - 1, exit.second };
    }

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, candidates.size() - 1);
    Coord chosen = candidates[dist(gen)];
    return chosen;
}

// Checking if two positions are one block away from each other
bool Maze::isAdjacentUpDownLeftRight(Coord a, Coord b) {
    return ((a.first == b.first && abs(a.second - b.second) == 1) ||
        (a.second == b.second && abs(a.first - b.first) == 1)) || (a == b);
}

// ON and OFF highlighted path
void Maze::toggleHighlightPath() {
    showPath = !showPath;
    std::cout << "\nHighlight path is now " << (showPath ? "ON" : "OFF") << "\n";
    std::cout << "finalPath size: " << finalPath.size() << "\n";
}

// Finding out the type of the magic object
char Maze::getEffectAt(Coord pos) const {
    auto it = powerUpTypes.find(pos);
    return (it != powerUpTypes.end()) ? it->second : ' ';
}