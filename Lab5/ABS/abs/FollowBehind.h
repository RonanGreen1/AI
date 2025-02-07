#ifndef FOLLOW_BEHIND_H
#define FOLLOW_BEHIND_H

#include "Droid.h"
#include "Routine.h"
#include <iostream>
#include <cmath> // std::abs

//
// This behaviour follows behind Droid A by exactly one square.
//
class FollowBehind : public Routine {
public:
    int droidA;  // The droid to follow
    int destX;
    int destY;
    sf::Vector2f target;

    FollowBehind(int droidA, Grid& g) : Routine() {
        this->droidA = droidA - 1; // Convert to zero-based index
        this->routineType = "FollowBehind";
        this->routineGrid = &g;
    }

    void reset(std::string msg) override {
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        state = RoutineState::None;
    }

    void act(Droid* droid, Grid& grid) override {
        if (grid.m_gridDroids.empty() || droidA < 0 || droidA >= grid.m_gridDroids.size()) {
            std::cerr << "Error: Invalid droid index in FollowBehind!" << std::endl;
            fail();
            return;
        }

        Droid* leader = grid.m_gridDroids[droidA];

        // Determine movement direction of Droid A
        sf::Vector2f direction = leader->position - leader->target;

        if (std::abs(grid.length(direction)) > 0) {
            sf::Vector2f unitDirection = droid->normalize(direction);

            // Compute the position exactly one square behind Droid A
            sf::Vector2f behindPosition = leader->position + (unitDirection * -1.0f * grid.nodes[0][0].s_radius);
            
            // Convert position to grid coordinates
            destX = round(behindPosition.x);
            destY = round(behindPosition.y);

            // Ensure it remains inside the grid
            destX = std::clamp(destX, 1, grid.gridSize);
            destY = std::clamp(destY, 1, grid.gridSize);

            droid->target = grid.getGridLocation(destX, destY);

            // Move if not at the target position
            if (!isDroidAtDestination(droid, grid)) {
                moveDroid(droid, grid);
            } else {
                succeed("FollowBehind for " + droid->name);
            }
        } else {
            succeed("Leader is stationary, stopping.");
        }
    }

    void moveDroid(Droid* droid, Grid& grid) {
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
