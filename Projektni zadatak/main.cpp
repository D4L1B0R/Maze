#include <iostream>
#include <sstream>
#include <string>
#include "Display.hpp"
#include "Dynamic.hpp"
#include "Maze.hpp"

int main() {
    // PREPARING THE COMMAND LINE
    std::string welcome = "Welcome to the Game of Survival!";
    int width = 80;
    int padding = (width - static_cast<int>(welcome.size())) / 2;
    std::cout << std::string(padding, ' ') << welcome << std::endl;

    std::cout << "\nYour mission is clear but perilous:\n"
        "Conquer the fearsome Minotaur and escape the labyrinthine maze.\n"
        "Every step matters, explore-cautiously, unravel ancient puzzles,\n"
        "and beware the deadly traps lurking in the shadows.\n"
        "Will you, Robot Robert, emerge victorious or become another lost soul?\n";

    char choice;
    while (true) {
        std::cout << "\n=== Press 'n' to view the Game Legend, 'p' to reveal Power-Ups, 'f' to move forward or 'q' to quit ===\n";
        std::cout << "\nEnter command: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Convert to lowercase to make input case-insensitive
        choice = std::tolower(choice);

        if (choice == 'n') {
            printGameLegend();
        }
        else if (choice == 'p') {
            revealAllPowerUps();
        }
        else if (choice == 'f') {

            // ENTERING DATA
            std::cout << "\nBefore your journey begins, brave explorer,\n"
                "we require some vital information to shape your destiny.\n"
                "Enter these details carefully, your survival depends on them.\n\n";

            std::string line;
            int row, col;

            while (true) {
                std::cout << "Dimension of the maze (row,col) with min(15,15): ";
                std::getline(std::cin, line);

                std::istringstream iss(line);
                if (!(iss >> row >> col)) {
                    std::cout << "Enter two valid numbers (int)!" << std::endl;
                    continue;
                }

                std::string extra;
                if (iss >> extra) {
                    std::cout << "Too many arguments entered. Please enter exactly two numbers." << std::endl;
                    continue;
                }

                if ((row >= 15 && col >= 15) && (row < 1000 && col < 1000)) {
                    break;
                }
                else {
                    std::cout << "Dimension must be at least 15x15 and less than 1000x1000!" << std::endl;
                }
            }

            // We limit the number of magic objects depending on the size of the maze 
            int area = row * col;
            double treshold = 3.6 + 0.15 * std::sqrt(area);

            int obj;
            while (true) {
                std::cout << "\nNumber of magic objects (min 4): ";
                std::getline(std::cin, line);
                std::istringstream iss(line);

                if (!(iss >> obj)) {
                    std::cout << "Enter a valid number (int)!" << std::endl;
                    continue;
                }

                std::string extra;
                if (iss >> extra) {
                    std::cout << "Too many arguments entered. Please enter exactly one number." << std::endl;
                    continue;
                }

                if (double(obj) > treshold) {
                    std::cout << "Too many magic objects makes this game easier. We won't allow it! :(" << std::endl;
                }
                else if (obj >= 4) {
                    break;
                }
                else {
                    std::cout << "Minimal number of magic objects is 4!" << std::endl;
                };
            }
            Maze maze(row, col, obj);
            gameLoop(maze);
        }
        else if (choice == 'q') {
            std::cout << "Quitting the game. Farewell, brave explorer!\n";
            break;
        }
        else {
            std::cout << "Invalid command! Press 'n' for legend, 'p' for power-ups, or 'q' to quit.\n";
        }
    }

    return 0;
}