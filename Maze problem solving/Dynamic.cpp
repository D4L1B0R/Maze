// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

// *** Resolving all the functions that are in charge of navigating and moving throughout the game ***

#include <random>
#include <iostream>
#include <set>
#include <queue>
#include <conio.h> // For _getch on Windows. Replace or abstract for portability.
#include "Dynamic.hpp"
#include "Maze.hpp"

int randomNumberGenerator(int start, int end) {
    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()
    std::uniform_int_distribution<> distr(start, end);
    return distr(gen);
}

// Core logic and system for gameplay (movement, turn order, sector detection, etc.)
// This is a modular file so that gameplay loop can call these functions directly.

bool highlightPath = false;

bool sameSector(Coord a, Coord b, int sectorSize) {
    return (abs(a.first - b.first) < sectorSize && abs(a.second - b.second) < sectorSize);
}

void printGame(Maze& maze) {
    maze.printMaze();
}

bool moveRobot(char input, Maze& maze) {
    Coord curr = maze.getRobotPos();
    Coord next = curr;

    switch (input) {
    case 'w': next.first--; break;
    case 's': next.first++; break;
    case 'a': next.second--; break;
    case 'd': next.second++; break;
    default: return false;
    }

    if (maze.isWalkable(next)) {
        maze.setRobotPos(next);

        if (maze.getCharAt(next) == 'P') {
            char effect = maze.getEffectAt(next);
            maze.applyEffect(effect);
            maze.erasePowerUpAt(next);
        }

        return true;
    }

    return false;
}

void moveMinotaur(Maze& maze) {
    Coord minotaurPos = maze.getMinotaurPos();
    if (!maze.inBounds(minotaurPos) || maze.getCharAt(minotaurPos) == '#') {
        std::cout << "[WARN] Minotaur's position was invalid, resetting...\n";
        return;
    }

    if (!maze.getMinotaurAlive()) {
        return;
    }

    int timer = maze.getMinotaurStunTimer();
    if (timer > 0) {
        timer--;
        maze.setMinotaurStunTimer(timer);
        return;
    }

    Coord robotPos = maze.getRobotPos();

    // Minotaur's randomized movement if it's to far away from Robot
    if (!maze.inSameSector(robotPos, minotaurPos)) {
        std::vector<Coord> directions = { {-1,0}, {1,0}, {0,-1}, {0,1} };
        std::shuffle(directions.begin(), directions.end(), std::mt19937(std::random_device{}()));
        for (const auto& d : directions) {
            Coord next = { minotaurPos.first + d.first, minotaurPos.second + d.second };
            if (maze.isWalkable(next)) {
                maze.setMinotaurPos(next);
                return;
            }
        }
        return;
    }
    // Not enabling the Minotaur to move if Robot have shield, he gets stunned
    else if (maze.isAdjacentUpDownLeftRight(minotaurPos, robotPos) && maze.getShieldTimer() > 0) {
        return;
    }
    // If Minotaur and Robot are in the same sector, Minotaur is moving towards the Robot
    else {
        Coord bestMove = minotaurPos;
        int minDist = INT_MAX;
        for (const auto& d : std::vector<Coord>{ {-1,0}, {1,0}, {0,-1}, {0,1} }) {
            Coord next = { minotaurPos.first + d.first, minotaurPos.second + d.second };
            if (maze.isWalkable(next)) {
                int dist = abs(next.first - robotPos.first) + abs(next.second - robotPos.second);
                if (dist < minDist) {
                    minDist = dist;
                    bestMove = next;
                }
            }
        }
        maze.setMinotaurPos(bestMove);
    }
}

void printActivePowers(Maze& maze) {
    std::cout << "\nActive Powers: ";
    bool noneActive = true;
    if (maze.getFogActive()) {
        std::cout << "[Fog (" << maze.getFogTimer() << " moves left)] ";
        noneActive = false;
    }

    if (maze.getSword()) {
        std::cout << "[Sword (" << maze.getSwordTimer() << " moves left)] ";
        noneActive = false;
    }

    if (maze.getShield()) {
        std::cout << "[Shield (" << maze.getShieldTimer() << " moves left)] ";
        noneActive = false;
    }

    if (maze.getHammer()) {
        std::cout << "[Hammer (" << maze.getHammerTimer() << " moves left)] ";
        noneActive = false;
    }

    if (noneActive) {
        std::cout << "None";
    }

    std::cout << "\n";
}

bool gameLoop(Maze& maze) {
    while (true) {
        maze.printMaze();

        if (maze.getGameOver()) {
            return false;
        }

        printActivePowers(maze);

        std::cout << "\nCommand (w/a/s/d to move, h to toggle highlight, q to quit): ";
        char c = _getch();
        std::cout << std::endl;
        if (c == 'q') return false;
        if (c == 'h') {
            maze.toggleHighlightPath();
            continue;
        }

        // Controling area, based on the input we get movement of the character R (Robot Robert)
        if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
            Coord oldRobotPos = maze.getRobotPos();

            Coord nextRobotPos = oldRobotPos;
            switch (c) {
            case 'w': nextRobotPos.first--; break;
            case 's': nextRobotPos.first++; break;
            case 'a': nextRobotPos.second--; break;
            case 'd': nextRobotPos.second++; break;
            }

            if (maze.isWalkable(nextRobotPos)) {
                maze.setRobotPos(nextRobotPos);

                if (maze.getCharAt(nextRobotPos) == 'P') {
                    char effect = maze.getEffectAt(nextRobotPos);

                    // Figuring out which magic object is Robot Robert taking
                    switch(effect) {
                    case 'F': {
                            maze.setFogActive(true);
                            int forTimer = maze.getFogTimer();
                            forTimer += 4;
                            maze.setFogTimer(forTimer);
                            std::cout << "\nEffect Activated: Fog - Vision reduced for" << forTimer - 1 << " moves.\n";
                        }
                    case 'S': {
                        maze.setSword(true);
                        int swordTimer = maze.getSwordTimer();
                        swordTimer += 4;
                        maze.setSwordTimer(swordTimer);
                        std::cout << "\nEffect Activated: Sword - You can kill the Minotaur for " << swordTimer - 1 << " moves.\n";
                    }
                    case 'H': {
                        maze.setShield(true);
                        int shieldTimer = maze.getShieldTimer();
                        shieldTimer += 4;
                        maze.setShieldTimer(shieldTimer);
                        std::cout << "\nEffect Activated: Shield - You are safe from Minotaur for " << shieldTimer - 1 << " moves.\n";
                    }
                    case 'B': {
                        maze.setHammer(true);
                        int hammerTimer = maze.getHammerTimer();
                        hammerTimer += 4;
                        maze.setHammerTimer(hammerTimer);
                        std::cout << "\nEffect Activated: Hammer - You can break one wall for " << hammerTimer - 1 << " moves.\n";
                    }
                    }
                    maze.erasePowerUpAt(nextRobotPos);
                }
            }
            else {
                // Seeing if Robot Robert can break the wall in front him
                Coord hammerTarget = nextRobotPos;
                if (maze.getHammer() && maze.getCharAt(hammerTarget) == '#' && maze.inBounds(nextRobotPos)) {
                    maze.breakWallAt(hammerTarget);
                    std::cout << "\nYou smashed a wall with the hammer! Hammer is now used up.\n";
                    maze.setHammer(false);
                    maze.setHammerTimer(0);
                }
                else if (nextRobotPos != maze.getMinotaurPos()) {
                    std::cout << "\nYou hit the wall! Cannot move there.\n";
                    continue;
                }
                else {
                    std::cout << "\nDon't touch the beast when it's sleeping. It's better for you to RUN!\n";
                    continue;
                }
            }

            if (maze.getRobotPos() == maze.getExitAbove()) {
                maze.setGameWon(true);
                maze.setGameOver(true);
                maze.printMaze();
                return true;
            }
            
            if (maze.getMinotaurAlive()) {
                moveMinotaur(maze);

                Coord robotPos = maze.getRobotPos();
                Coord minotaurPos = maze.getMinotaurPos();

                if (maze.getMinotaurStunTimer() == 0) {
                    if (minotaurPos != Coord{ -1, -1 } && maze.isAdjacentUpDownLeftRight(robotPos, minotaurPos)) {
                        if (maze.getSword()) {
                            std::cout << "\nYou killed the Minotaur with your sword!\n";
                            maze.setSwordTimer(0);
                            maze.setMinotaurAlive(false);
                            maze.setMinotaurPos({ -1, -1 });
                            maze.setSword(false);
                        }
                        else if (maze.getShield()) {
                            std::cout << "\nShield blocked the Minotaur's attack! Minotaur is stunned for 3 turns.\n";
                            maze.setShieldTimer(0);
                            maze.setMinotaurStunTimer(3);
                            maze.setShield(false);
                        }
                        else {
                            maze.setGameOver(true);
                            maze.setGameWon(false);
                            maze.setMinotaurPos(robotPos);
                            maze.setRobotPos({ -1, -1 });
                            maze.setFogActive(false);
                            maze.printMaze();
                            return false;
                        }
                    }
                }
            }

            // Checking the status of the magic objects and lovering down the timer
            if (maze.getFogActive()) {
                int fogTimer = maze.getFogTimer();
                fogTimer--;
                maze.setFogTimer(fogTimer);
                if (fogTimer <= 0) {
                    maze.setFogActive(false);
                    std::cout << "\nFog effect has worn off.\n";
                }
            }
            if (maze.getSword()) {
                int swordTimer = maze.getSwordTimer();
                swordTimer--;
                maze.setSwordTimer(swordTimer);
                if (swordTimer <= 0) {
                    maze.setSword(false);
                    std::cout << "\nSword effect has worn off.\n";
                }
            }
            if (maze.getShield()) {
                int shieldTimer = maze.getShieldTimer();
                shieldTimer--;
                maze.setShieldTimer(shieldTimer);
                if (shieldTimer <= 0) {
                    maze.setShield(false);
                    std::cout << "\nShield effect has worn off.\n";
                }
            }
            if (maze.getHammer()) {
                int hammerTimer = maze.getHammerTimer();
                hammerTimer--;
                maze.setHammerTimer(hammerTimer);
                if (hammerTimer <= 0) {
                    maze.setHammer(false);
                    std::cout << "\nHammer effect has worn off.\n";
                }
            }
        }
    }
}

char getPowerTypeAt(Coord pos, const std::unordered_map<Coord, char>& powerUpTypes) {
    auto it = powerUpTypes.find(pos);
    return (it != powerUpTypes.end()) ? it->second : ' ';
}