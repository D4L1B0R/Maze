#ifndef COORD_HASH_HPP
#define COORD_HASH_HPP

#include <utility>
#include <functional>

using Coord = std::pair<int, int>;

namespace std {
    template<>
    struct hash<Coord> {
        std::size_t operator()(const Coord& p) const noexcept {
            return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
        }
    };
}

#endif