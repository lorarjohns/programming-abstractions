// Script to generate solutions to maze files.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "stack.h"
#include "queue.h"
#include "vector.h"
#include "set.h"
#include "maze.h"
#include "mazegraphics.h"
#include "testing/SimpleTest.h"
using namespace std;


/** Given a perfect maze represented as a Grid of bool and a
 * current cell represented by the GridLocation cur, return
 * all valid moves from cur as a Set.
 *
 * @param Grid<bool> maze representing a perfect maze where true
 * is an open space and false is a wall
 * @param GridLocation cur with a row and column value indicating
 * current position in the maze
 * @return Set neighbors of GridLocations representing valid moves
 * in the North, East, South, or West directions
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    /* Check -/+1 row, same col */
    /* Check -/+1 col, same row */
    for (int c = max(cur.col - 1, 0); c <= cur.col + 1; c++) {
        for (int r = max(cur.row -1, 0); r <= cur.row +1; r++) {
            if ((c == cur.col) && (r != cur.row)) {
                if (maze.inBounds(r, c)) {
                    if (maze[r][c] == true) {
                        GridLocation newNeighbor = {r, c};
                        neighbors.add(newNeighbor);
                    }
                }
            } else if ((r == cur.row) && (c != cur.col)) {
                if (maze.inBounds(r, c)) {
                    if (maze[r][c] == true) {
                        GridLocation newNeighbor = {r, c};
                        neighbors.add(newNeighbor);
                    }
                }
            }
    }
    }
    return neighbors;
}


/** Compares a path of points to a given maze grid and returns
 * an error if the path does not constitute a valid solution
 * to the maze.
 *
 * @param Grid<bool> maze, the maze to check the solution against
 * @param Stack<GridLocation> path, a stack of (x, y) coordinates
 * representing a potential solution to the maze
 * @return void, if the path is a valid solution. Raises an error
 * describing the reason why the solution is invalid otherwise.
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {

    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    GridLocation mazeEntry = {0, 0};

    GridLocation cur = path.pop();
    while (path.size() > 0) {
        GridLocation next = path.pop();
        Set<GridLocation> neighbors = generateValidMoves(maze, cur);
        if (!maze.inBounds(next)) {
            error("Path contains an out-of-bounds step");
        }
        if (maze[next] == false) {
            error("Path contains a step through a wall");
        }
        if (next == cur) {
            error("Path contains a duplicate step");
        }
        if (!neighbors.contains(next)) {
            error("Path contains a step onto an illegal cell");
        } else {
            cur = next;
            if (path.size() == 0) {
                if (next != mazeEntry) {
                    error("Path does not begin at maze entry");
                }
            }
        }
    }

    /* If you find a problem with the solution, call error() to report it.
     * If the path is valid, then this function should run to completion
     * without throwing any errors.
     */
}

/** Generates solutions to a given maze using a breadth-
 * first search algorithm.
 *
 * @param Grid<bool>& maze, reference to a maze object
 * @return Stack<GridLocation>, stack of (x, y) points
 * in order describing a valid path from the upper left
 * to the lower right of the maze
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    GridLocation mazeEnd = {maze.numRows()-1, maze.numCols()-1};
    /* declare path variable */
    Stack<GridLocation> path;
    /* declare variable to track already visited nodes */
    Set<GridLocation> visitedNeighbors;

    /* create a queue of paths */
    Queue<Stack<GridLocation>> pathQueue;
    /* create a length-one path containing the entry location */
    Stack<GridLocation> startPath;
    startPath.push((GridLocation) {0,0});

    /* enqueue the new path */
    pathQueue.enqueue(startPath);
    /* add start to visited */
    visitedNeighbors.add(startPath.peek());

    /* While there are still more paths to explore */
    while (!pathQueue.isEmpty()) {
        /* dequeue path from queue */
        path = pathQueue.dequeue();

        /* if this path ends at exit, return it as solution */
        if (path.peek() == mazeEnd) {
            MazeGraphics::highlightPath(path, "green");
            return path;
        }
        /* if path does not end at exit
         * for each viable neighbor of path's end
         * check if we already visited it
         * if not, make a copy of path
         * extend by adding neighbor
         * and enqueue it
         * then add the neighbor to visited */
        MazeGraphics::highlightPath(path, "blue");
        Set<GridLocation> neighbors = generateValidMoves(maze, path.peek());
        for (GridLocation neighbor: neighbors) {
            if (!visitedNeighbors.contains(neighbor)) {
                Stack<GridLocation> newPath = path;
                newPath.push(neighbor);
                MazeGraphics::highlightPath(newPath, "blue");
                pathQueue.enqueue(newPath);
                visitedNeighbors.add(neighbor);
            }
        }

    }

    return path;
}

/**
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 * It has been updated to correctly handle mazes with uneven line
 * lengths and illegal characters.
 *
 * @param string filename, the filename of the maze pattern to read.
 * @param Grid<bool>& maze, a reference to an existing maze object
 * to populate according to the specification in the file.
 * @return void - modifies the maze object in-place, or raises the
 * appropriate error if the template file is invalid.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following lines read in the data from the file into a Vector
     * of strings representing the lines of the file. We haven't talked
     * in class about what ifstreams are, so don't worry if you don't fully
     * understand what is going on here.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;

    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    for (int i = 0; i < lines.size() - 1; i++) {
       if (lines[i].length() != lines[i+1].length()) {
           error("Maze file contains lines of inconsistent length.");
       }
    }

    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze file contains an invalid character.");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    Vector<string> lines;
    readEntireFile(in, lines);

    if (lines.size() != 1){
        error("File contained too many or too few lines.");
    }

    istringstream istr(lines[0]); // Stack read does its own error-checking
    if (!(istr >> soln)) {// if not successfully read
        error("Solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateNeighbors on location in the center of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateNeighbors on location on the side of 3x3 grid with no walls"){
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateNeighbors on corner of 2x2 grid with walls"){
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze"){
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}


PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    cout << maze << endl;
    cout << soln << endl;
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("Test readMazeFile on valid file 2x2.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/2x2.maze", maze));
}

PROVIDED_TEST("Test readMazeFile on valid file 5x7.maze") {
    Grid<bool> maze;

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(readMazeFile("res/5x7.maze", maze));
}

PROVIDED_TEST("readMazeFile on nonexistent file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/nonexistent_file", g));
}

PROVIDED_TEST("readMazeFile on malformed file should raise an error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed.maze", g));
}

/* * * * * * * Student Test Cases * * * * * * */

STUDENT_TEST("MY TESTS checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false, true},
                       {true, true, false},
                       {false, true, false},
                       {true, true, true},
                       {true, false, true},
                       {true, true, true}};
    Stack<GridLocation> not_end_at_exit = {{0, 0}, {1, 0}, {1, 1}, {2,1}, {3,1}, {3,0}, {4,0}, {5,0}, {5,1}};
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1}, {2,1}, {3,1}, {3,0}, {4,0}, {5,0}, {5,1}, {5,2}};
    Stack<GridLocation> go_through_wall = { {0 ,0}, {1, 0}, {1, 1}, {2,1}, {3,1}, {3,0}, {4,0}, {4,1}, {5,1}, {5,2}};
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1}, {3,1}, {3,0}, {4,0}, {5,0}, {5,1}, {5,2} };
    Stack<GridLocation> duplicate_step = { {1, 0}, {1, 0}, {1, 1}, {2,1}, {3,1}, {3,0}, {4,0}, {5,0}, {5,1}, {5,2}};
    Stack<GridLocation> out_of_bounds = { {0,0}, {1,0}, {1,1}, {2,1}, {3,1}, {3,2}, {3,3}, {4,2}, {5,2}};
    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
    EXPECT_ERROR(checkSolution(maze, duplicate_step));
    EXPECT_ERROR(checkSolution(maze, out_of_bounds));
}

STUDENT_TEST("generateNeighbors of {0,0}") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation start = {0, 0};
    Set<GridLocation> neighbors = {{0,1}, {1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, start));
}

STUDENT_TEST("solveMaze on file 17x41") {
    Grid<bool> maze;
    readMazeFile("res/17x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 2x2") {
    Grid<bool> maze;
    readMazeFile("res/2x2.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("solveMaze on file 19x11") {
    Grid<bool> maze;
    readMazeFile("res/19x11.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    // We expect that this line of code will execute without raising
    // an exception
    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("readMazeFile on maze of varying line lengths should raise error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/uneven.maze", g));
    EXPECT_ERROR(readMazeFile("res/uneven2.maze", g));
}

STUDENT_TEST("readMazeFile on maze with invalid characters should raise error") {
    Grid<bool> g;

    EXPECT_ERROR(readMazeFile("res/malformed2.maze", g));
    EXPECT_ERROR(readMazeFile("res/malformed3.maze", g));
}
