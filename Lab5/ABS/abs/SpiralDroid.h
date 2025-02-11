#include "Droid.h"
#include "MoveTo.h"
#include <iostream>

class SpiralDroid : public Routine {

public:

    MoveTo* moveTo;       // Movement behavior
    int layer;            // Current layer of the spiral (how far inward/outward)
    int direction;        // Current direction (0 = Right, 1 = Down, 2 = Left, 3 = Up)
    bool movingInward;    // True if moving inward, False if moving outward
    int destX, destY;     // Current target position

    /**
     * Constructor: Initializes the droid to follow a spiral path.
     * Starts at the outermost edge and begins moving in a square pattern.
     */
    SpiralDroid(Grid& grid) : Routine()   
    {
        this->routineType = "Spiral";   // Assign routine type
        this->routineGrid = &grid;
        this->layer = 0;                // Start at the outermost boundary
        this->direction = 0;            // Start moving RIGHT
        this->movingInward = true;      // Initially moving toward the center
        destX = 1;                      // Starting X position
        destY = 1;                      // Starting Y position
        moveTo = new MoveTo(destX, destY, grid); // Create movement routine
    }

    /**
     * Starts the spiral movement routine.
     * Displays a debug message and sets the routine state to Running.
     */
    void start(std::string msg)
    {
        std::cout << ">>> Starting routine " << routineType << msg << std::endl;
        state = RoutineState::Running;
        moveTo->start(" from a Spiral node.");
    }

    /**
     * Resets the spiral movement.
     * Resets position, layer, direction, and movement state.
     */
    void reset(std::string msg)
    {
        std::cout << ">>> Resetting routine " << routineType << msg << std::endl;
        this->layer = 0;                // Reset to the outermost layer
        this->direction = 0;            // Start moving RIGHT again
        this->movingInward = true;      // Reset to inward movement
        destX = 1;                      // Reset position to (1,1)
        destY = 1;
        moveTo = new MoveTo(destX, destY, *routineGrid); // Reset movement routine
        moveTo->reset(" from a Spiral node");
        state = RoutineState::None;
    }

    /**
     * Executes the spiral movement logic.
     * Moves the droid to its next destination.
     */
    void act(Droid* droid, Grid& grid) 
    {
        if (!moveTo->isRunning()) 
        {
            return;
        }

        moveTo->act(droid, grid); // Execute movement

        // When movement is completed, update to the next spiral position
        if (moveTo->isSuccess()) {
            updateSpiralPosition(grid); // Compute next spiral position
            moveTo = new MoveTo(destX, destY, grid); // Move to next position
            moveTo->start(" from a Spiral node.");
        }
        else if (moveTo->isFailure()) {
            fail();
        }
    }

    /**
     * Calculates the next position in the spiral pattern.
     * Moves in a square pattern inward toward the center, then outward.
     */
    void updateSpiralPosition(Grid& grid) {
        int gridSize = grid.gridSize;
        int minLimit = 1 + layer;    // Inner boundary of the current layer
        int maxLimit = gridSize - layer; // Outer boundary of the current layer

        // Move in the current direction and update when reaching a boundary
        switch (direction) {
            case 0: // Move RIGHT
                destX++; 
                if (destX >= maxLimit) direction = 1; // Switch to DOWN
                break; 
            case 1: // Move DOWN
                destY++; 
                if (destY >= maxLimit) direction = 2; // Switch to LEFT
                break; 
            case 2: // Move LEFT
                destX--; 
                if (destX <= minLimit) direction = 3; // Switch to UP
                break; 
            case 3: // Move UP
                destY--; 
                if (destY <= minLimit) {  // Completed a full lap
                    if (movingInward) {
                        layer++; // Move inward to the next layer
                        if (layer >= gridSize / 2) {
                            movingInward = false; // Switch to outward movement
                        }
                    } else {
                        layer--; // Move outward to the next layer
                        if (layer <= 0) {
                            succeed("Finished spiral movement"); // End movement
                            return;
                        }
                    }
                    direction = 0; // Reset to moving RIGHT
                }
                break;
        }
    }
};
