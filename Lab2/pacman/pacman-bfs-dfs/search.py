# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
#
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    "*** YOUR CODE HERE ***"

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))

    stack = util.Stack()
    stack.push((problem.getStartState(), []))
    print("Start stack:", stack.list)
    
    visited = list()

    while not stack.isEmpty():
        currentState, steps = stack.pop()
        if currentState in visited:
            continue
        if problem.isGoalState(currentState):
            return steps

        visited.append(currentState)
        for state, action, cost in problem.getSuccessors(currentState):
            stack.push((state, steps + [ action ]))

    return []

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    """
    Perform Breadth-First Search (BFS) on the given problem to find a path to the goal.

    Args:
        problem: A search problem instance that provides methods like:
            - getStartState(): Returns the start state.
            - isGoalState(state): Checks if a given state is the goal state.
            - getSuccessors(state): Returns successors (state, action, cost).

    Returns:
        A list of actions that form the path to the goal, or an empty list if no path exists.
    """

    # Initialize a queue (FIFO) to store states to explore.
    # Each entry in the queue is a tuple: (currentState, actionsSoFar)
    queue = util.Queue()
    queue.push((problem.getStartState(), []))  # Enqueue the start state with an empty path.
    print("Start queue:", queue.list)  # Debug: Show initial queue contents.

    # Maintain a list of visited states to prevent revisiting.
    visited = []

    # Continue exploring until there are no more states in the queue.
    while not queue.isEmpty():
        # Dequeue the first element (FIFO).
        currentState, steps = queue.pop()
        print("Dequeued:", currentState, "with path:", steps)  # Debug: Show current state and path.

        # Skip this state if it has already been visited.
        if currentState in visited:
            continue

        # If the current state is the goal, return the path to it.
        if problem.isGoalState(currentState):
            print("Goal found! Path:", steps)  # Debug: Goal state reached.
            return steps

        # Mark the current state as visited.
        visited.append(currentState)

        # Get all successors of the current state.
        for state, action, cost in problem.getSuccessors(currentState):
            # If the successor state has not been visited, enqueue it with the updated path.
            if state not in visited:
                queue.push((state, steps + [action]))
                print("Enqueued:", state, "with path:", steps + [action])  # Debug: Show enqueued state and path.

    # If the loop finishes without finding a goal, return an empty list (no solution).
    print("No path to goal found.")  # Debug: No solution.
    return []
    
def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    util.raiseNotDefined()
    
def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """
    Perform A* Search on the given problem using the provided heuristic.

    Args:
        problem: A search problem instance with methods:
            - getStartState(): Returns the start state.
            - isGoalState(state): Checks if a given state is the goal.
            - getSuccessors(state): Returns successors (state, action, cost).
        heuristic: A function estimating the cost from a state to the goal.
    
    Returns:
        A list of actions that form the path to the goal, or an empty list if no path exists.
    """

    # Priority queue to store states with their priority (combined cost + heuristic)
    priority_queue = util.PriorityQueue()
    
    # Start state and path are pushed with their initial priority
    # Priority = path cost (g) + heuristic estimate to goal (h)
    start_state = problem.getStartState()
    priority_queue.push((start_state, [], 0), heuristic(start_state, problem))

    # List to track visited states
    visited = {}

    while not priority_queue.isEmpty():
        # Pop the state with the lowest priority (g + h)
        currentState, steps, g_cost = priority_queue.pop()

        # If already visited with a lower cost, skip processing
        if currentState in visited and visited[currentState] <= g_cost:
            continue

        # Mark the state as visited with its g_cost
        visited[currentState] = g_cost

        # Check if the current state is the goal
        if problem.isGoalState(currentState):
            return steps

        # Get all successors of the current state
        for state, action, step_cost in problem.getSuccessors(currentState):
            # Calculate the cost to reach this successor
            new_g_cost = g_cost + step_cost
            # Compute the priority as g + h
            priority = new_g_cost + heuristic(state, problem)
            # Push the successor with the updated path and priority
            priority_queue.push((state, steps + [action], new_g_cost), priority)

    # If no solution is found, return an empty list
    return []
    
# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch