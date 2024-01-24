#pragma once

#include "precomp.hpp"
#include "object.hpp"
#include "quadtree.hpp"

class Solver {
    public:
        uint64_t iterationSum = 0;
        uint32_t iterationCount = 0;

        void solve(Quadtree &quadtree, float deltaTime);
};