// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include "Maze.h"

/* escapeMaze:
 *
 * Recursively backtracks through a maze file to determine
 * whether there exists an escape route, returning a boolean.
 * The logic, or "story," behind the code is a little tricky, so
 * I have LIBERALLY commented in an attempt to spell out exactly
 * what is happening.
 */
bool escapeMaze(Maze& maze, int row, int col) {

    // if we are in the maze, we need to check that
    // we aren't starting on a wall. by checking in bounds,
    // we avoid the potential for an exception
    if (maze.inBounds(row, col)) {

        // the choices are self-explanatory...
        if (maze.isWall(row, col)) {
            return false;
        }
        // ... but we put this here so that we are guaranteed a marked cell
        // (the only case in which we wouldnt mark a cell is when we are OOB
        // which is caught...)
        else {
            maze.mark(row, col);
        }
    }

    // (...in this if statement). now, we know we arent on a wall
    // if we can get out on the next turn, we're done
    // this includes being outside the maze
    bool oobCheck = !maze.inBounds(row-1, col) || !maze.inBounds(row+1, col) ||
            !maze.inBounds(row, col-1) || !maze.inBounds(row, col+1);
    if (oobCheck) {
        return true;
    }

    // each location defaults to false, because...
    bool upCheck, downCheck, leftCheck, rightCheck = false;

    // ...if we cannot check it (for either a wall or previous mark)
    // then looking in this direction is false. however, if the direction
    // IS open, repeat the whole process. if that process finds a way out,
    // bubble up true, immediately. otherwise try the remaining directions.
    // if they all return false, unmark the cell, back up and try the
    // remaining directions
    if (maze.isOpen(row-1, col)) {
        upCheck = escapeMaze(maze, row-1, col);
        if (upCheck) {
            return true;
        }
    }

    if (maze.isOpen(row+1, col)) {
        downCheck = escapeMaze(maze, row+1, col);
        if (downCheck) {
            return true;
        }
    }

    if (maze.isOpen(row, col-1)) {
        leftCheck = escapeMaze(maze, row, col-1);
        if (leftCheck) {
            return true;
        }
    }

    if (maze.isOpen(row, col+1)) {
        rightCheck = escapeMaze(maze, row, col+1);
        if (rightCheck) {
            return true;
        }
    }

    // at this point, we have continued to go in one of the four
    // directions and returned true - which means unmark() hasn't been
    // called at all on the current path - OR all four paths don't work
    // in which case we unmark the path and look to the next possible
    // direction
    if (!oobCheck) {
        maze.unmark(row, col);
    }
    return false;
}
