// Project: Maze Problem solving in c++
// Name of an author: Nikolić Dalibor SV13-2023
// Date and time of the last changes: 23.06.2025. 18:47

#ifndef COORD_HASH_HPP
#define COORD_HASH_HPP

#include <utility>
#include <functional>

using Coord = std::pair<int, int>;

// Global initialization of the struct Coord that represents coordinates in the maze
namespace std {
    template<>
    struct hash<Coord> {
        std::size_t operator()(const Coord& p) const noexcept {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}

#endif