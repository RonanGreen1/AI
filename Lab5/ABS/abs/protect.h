#ifndef PROTECT_H
#define PROTECT_H

#include "Droid.h"
#include "Routine.h"
#include <iostream>
#include <cmath> // std::abs

//
// This behaviour picks 2 other droids: A (protected) and B (threatening).
// It then tries to position itself so that it is between A and B.
// In effect, it protects A from B.
//
class Protect : public Routine {
public:
    int destX;
    int destY;
    sf::Vector2f target;
    int droidA; // ID of the protected droid
    int droidB; // ID of the threatening droid

    Protect(int droidA, int droidB, Grid& g) : Routine() {
        this->destX = 1;
        this->destY = 1;
        this->target = g.getGridLocation(destX, destY);
        this->routineType = "Protect";
        this->routineGrid = &g;

        // If we get -1 passed in, we will self-select the droids in 'act' below
        this->droidA = droidA;
        this->droidB = droidB;
        if (droidA != -1) this->droidA = droidA - 1;
        if (droidB != -1) this->droidB = droidB - 1;
    }

    void reset(std::string msg) override {
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        state = RoutineState::None;
    }

    void act(Droid* droid, Grid& grid) override {
        if (grid.m_gridDroids.empty()) {
            std::cerr << "Error: No droids available in the grid!" << std::endl;
            fail();
            return;
        }
    
        if (droidA < 0 || droidA >= grid.m_gridDroids.size() ||
            droidB < 0 || droidB >= grid.m_gridDroids.size()) {
            std::cerr << "Error: Invalid droid indices! droidA=" << droidA << " droidB=" << droidB << std::endl;
            fail();
            return;
        }
    
        int droidAx = grid.m_gridDroids[droidA]->x;
        int droidAy = grid.m_gridDroids[droidA]->y;
        int droidBx = grid.m_gridDroids[droidB]->x;
        int droidBy = grid.m_gridDroids[droidB]->y;
    
        // Compute the midpoint between A and B
        sf::Vector2f AB = sf::Vector2f(droidBx - droidAx, droidBy - droidAy);
        sf::Vector2f midpoint = sf::Vector2f(droidAx, droidAy) + (AB * 0.5f);
    
        destX = std::round(midpoint.x);
        destY = std::round(midpoint.y);
    
        // Constrain within grid boundaries
        destX = std::clamp(destX, 1, grid.gridSize);
        destY = std::clamp(destY, 1, grid.gridSize);
    
        droid->target = grid.getGridLocation(destX, destY);
    
        if (!isDroidAtDestination(droid, grid)) {
            moveDroid(droid, grid);
        } else {
            succeed("Protect for " + droid->name);
        }
    }

    void moveDroid(Droid* droid, Grid& grid) {
        std::cout << ">>> Droid " << droid->name << " moving to " << destX << ", " << destY << std::endl;

        if (destX < 1 || destX > grid.gridSize || destY < 1 || destY > grid.gridSize)
            return;

        sf::Vector2f direction = droid->target - droid->position;
        if (std::abs(grid.length(direction)) > 0) {
            if (droid->target.y != droid->position.y) {
                droid->position.y += (droid->target.y > droid->position.y) ? 1 : -1;
            }
            if (droid->target.x != droid->position.x) {
                droid->position.x += (droid->target.x > droid->position.x) ? 1 : -1;
            }
        }
        if (isDroidAtDestination(droid, grid)) {
            succeed("MoveTo for " + droid->name);
        }
    }

    bool isDroidAtDestination(Droid* droid, Grid& grid) {
        sf::Vector2f direction = droid->target - droid->position;
        return ((int)grid.length(direction) == 0);
    }
};

#endif
