#include <random>
#include "Dynamic.hpp"
#include "Maze.hpp"
#include <iostream>
#include <set>
#include <queue>
#include <conio.h> // For _getch on Windows. Replace or abstract for portability.

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

Coord bfsTowards(Coord start, Coord goal, Maze& maze) {
    std::queue<std::pair<Coord, Coord>> q;
    std::set<Coord> visited;
    q.push({ start, start });
    visited.insert(start);

    std::vector<Coord> directions = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!q.empty()) {
        auto front = q.front(); q.pop();
        Coord from = front.first;
        Coord curr = front.second;
        if (curr == goal) return from;

        for (const auto& d : directions) {
            Coord next = { curr.first + d.first, curr.second + d.second };
            if (maze.isWalkable(next) && !visited.count(next)) {
                q.push({ (from == start) ? next : from, next });
                visited.insert(next);
            }
        }
    }
    return start;
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

    if (minotaurPos == Coord{ -1, -1 }) {
        // Minotaur == DEAD ;(
        return;
    }

    int timer = maze.getMinotaurStunTimer();
    if (timer > 0) {
        timer--;
        maze.setMinotaurStunTimer(timer);
        return;
    }

    Coord robotPos = maze.getRobotPos();

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

bool isAdjacentUpDownLeftRight(Coord a, Coord b) {
    return ((a.first == b.first && abs(a.second - b.second) == 1) ||
        (a.second == b.second && abs(a.first - b.first) == 1));
}

void printActivePowers(Maze& maze) {
    std::cout << "\nActive Powers: ";
    bool noneActive = true;
    if (maze.getFogActive()) {
        std::cout << "[Fog (" << maze.getFogTimer() << " moves left)] ";
        noneActive = false;
    }
    if (maze.getSword() && !maze.getSwordUsed()) {
        std::cout << "[Sword] ";
        noneActive = false;
    }
    if (maze.getShield() && !maze.getShieldUsed()) {
        std::cout << "[" << maze.getShieldUsesLeft() << "x Shield] ";
        noneActive = false;
    }
    if (maze.getHammer() && !maze.getHammerUsed()) {
        std::cout << "[" << maze.getHammerUsesLeft() << "x Hammer] ";
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
                    if (effect == 'F') {
                        maze.setFogActive(true);
                        int timer = maze.getFogTimer();
                        timer += 5;
                        maze.setFogTimer(timer);
                        std::cout << "\nEffect Activated: Fog - Vision reduced for" << timer << " moves.\n";
                    }
                    else if (effect == 'S' && !maze.getSword()) {
                        maze.setSword(true);
                        std::cout << "\nEffect Activated: Sword - You can kill the Minotaur.\n";
                    }
                    else if (effect == 'H') {
                        maze.setShield(true);
                        int usesShield = maze.getShieldUsesLeft();
                        usesShield++;
                        maze.setShieldUsesLeft(usesShield);
                        std::cout << "\nEffect Activated: Shield - You are protected (" << usesShield << " uses).\n";
                    }
                    else if (effect == 'B') {
                        maze.setHammer(true);
                        int uses = maze.getHammerUsesLeft();
                        uses++;
                        maze.setHammerUsesLeft(uses);
                        std::cout << "\nEffect Activated: Hammer - You can break " << uses << " wall.\n";
                    }
                    maze.erasePowerUpAt(nextRobotPos);
                }
            }
            else {
                Coord hammerTarget = nextRobotPos;
                if (maze.getHammer() && maze.getHammerUsesLeft() > 0 && maze.getCharAt(hammerTarget) == '#') {
                    maze.breakWallAt(hammerTarget);
                    maze.decrementHammerUses();
                    std::cout << "\nYou smashed a wall with the hammer! Remaining uses: " << maze.getHammerUsesLeft() << "\n";

                    if (maze.getHammerUsesLeft() == 0) {
                        maze.setHammer(false);
                        std::cout << "Hammer is now used up.\n";
                    }
                }
                else {
                    std::cout << "\nYou hit a wall, cannot move there.\n";
                    continue;
                }
            }

            if (maze.getRobotPos() == maze.getExitAbove()) {
                std::cout << "\nYou reached the exit and escaped! Congratulations!\n";
                maze.setGameWon(true);
                maze.setGameOver(true);
                maze.printMaze();
                return true;
            }

            moveMinotaur(maze);

            Coord robotPos = maze.getRobotPos();
            Coord minotaurPos = maze.getMinotaurPos();

            if (minotaurPos != Coord{ -1, -1 } && isAdjacentUpDownLeftRight(robotPos, minotaurPos)) {
                if (maze.getSword() && !maze.getSwordUsed()) {
                    std::cout << "\nYou killed the Minotaur with your sword!\n";
                    maze.setMinotaurPos({ -1, -1 });
                    maze.setSwordUsed(true);
                }
                else if (maze.getShieldUsesLeft() > 0) {
                    std::cout << "\nShield blocked the Minotaur's attack! Minotaur is stunned for 3 turns.\n";
                    int shieldUses = maze.getShieldUsesLeft();
                    shieldUses--;
                    maze.setShieldUsesLeft(shieldUses);
                    maze.setMinotaurStunTimer(3);

                    if (shieldUses == 0) {
                        maze.setShield(false);
                        std::cout << "Your shield is now depleted.\n";
                    }
                }
                else {
                    std::cout << "\nThe Minotaur caught you! Game Over.\n";
                    maze.setGameOver(true);
                    maze.setGameWon(false);
                    maze.setRobotPos({ -1, -1 });
                    maze.setMinotaurPos(robotPos);
                    maze.printMaze();
                    return false;
                }
            }

            if (maze.getFogActive()) {
                int fogTimer = maze.getFogTimer();
                fogTimer--;
                maze.setFogTimer(fogTimer);
                if (fogTimer <= 0) {
                    maze.setFogActive(false);
                    std::cout << "\nFog effect has worn off.\n";
                }
            }
        }
    }
}

char getPowerTypeAt(Coord pos, const std::unordered_map<Coord, char>& powerUpTypes) {
    auto it = powerUpTypes.find(pos);
    return (it != powerUpTypes.end()) ? it->second : ' ';
}