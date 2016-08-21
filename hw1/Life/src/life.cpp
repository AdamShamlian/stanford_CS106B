// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void print_startup_message();
void get_input_stream(std::ifstream&);
void populate_grid(Grid<char>&, std::ifstream&);
int get_generation_option();
void iterate_life(Grid<char>&);
void calculate_next_generation(Grid<char>&);
char get_cell_status(int, int, Grid<char>&, int[8][2]);

int main() {
    // TODO: Finish the program!

    print_startup_message();

    // load up stream
    std::ifstream filestream;
    get_input_stream(filestream);

    Grid<char> primaryGrid;
    populate_grid(primaryGrid, filestream);

    iterate_life(primaryGrid);

    cout << "Have a nice Life!" << endl;
    return 0;
}

void print_startup_message() {
   
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << " Cells (X) life and die by the following rules." << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;

    return;
}

void get_input_stream(std::ifstream& gridInputStream) {

    bool fileSuccessFlag = false;
    string prompt = "Grid input file name? ";
    string filename;

    while (!fileSuccessFlag)
    {
        getLine(prompt, filename);
        fileSuccessFlag = openFile(gridInputStream, filename);
        if (fileSuccessFlag) {
            break;
        }
        cout << endl << "Unable to open that file. Try again. " << endl;
    }

    return;
}

void populate_grid(Grid<char>& gridToPopulate, std::ifstream& gridInputStream) {

    // store rows and columns, resize the grid
    string rows, columns;
    getline(gridInputStream, rows);
    getline(gridInputStream, columns);
    int intRows = stringToInteger(rows);
    int intCols = stringToInteger(columns);
    gridToPopulate.resize(intRows, intCols);

    // initialize the individual grid elements
    // fortunately, the notation is the same as for arrays
    string rowI;
    for (int i = 0; i < intRows; i++) {

        getline(gridInputStream, rowI);

        for (int j = 0; j < intCols; j++) {

            gridToPopulate[i][j] = rowI[j];
        }
    }

    // close inputStream
    gridInputStream.close();
    return;
}

int get_generation_option() {

    string prompt = "a)nimate, t)ick, q)uit? ";
    string response;
    getLine(prompt, response);

    if (response == "a" || response == "A") {

        string integerPrompt = "How many frames? ";
        int iters = getInteger(integerPrompt); // we want the default reprompt

        if (iters <= 0) {
            cout << "Must have greater than 0 generations. Quitting..." << endl;
            iters = 0;
        }
        return iters;
    }
    else if (response == "t" || response == "T") {
        return 1;
    }
    else if (response == "q" || response == "Q") {
        return 0;
    }
    else {
        cout << "Please try again. a, t, or q will do." << endl;
        return get_generation_option();
    }
}

void iterate_life(Grid<char>& displayGrid) {

    // initial display
    cout << displayGrid.toString2D("","","","\n") << endl;

    // prompting loop
    while (true) {

        // decide if we want to quit or see some number of generations
        int iterationFlag = get_generation_option();
        if (iterationFlag == 0) {
            break;
        }

        // generations loop
        // nonzero's are true
        while (iterationFlag) {

            pause(50);
            clearConsole();

            calculate_next_generation(displayGrid);
            cout << displayGrid.toString2D("","","","\n") << endl;

            iterationFlag--;
        }
    }
    return;
}

void calculate_next_generation(Grid<char>& displayGrid) {

    Grid<char> tmpGrid = displayGrid;

    int neighbors[8][2] = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, 1},
        {1, 1},
        {1, 0},
        {1, -1},
        {0, -1},
    };

    for (int i = 0; i < displayGrid.numRows(); i++) {
        for (int j = 0; j < displayGrid.numCols(); j++) {

            char cellStatus = get_cell_status(i, j, displayGrid, neighbors);
            tmpGrid.set(i, j, cellStatus);
        }
    }

    displayGrid = tmpGrid; // tmpGrid should be wiped out when this fcn returns
    return;
}

char get_cell_status(int cellRow, int cellCol, Grid<char>& displayGrid, int neighbors[8][2]) {

    int neighborRow;
    int neighborCol;
    int count = 0;

    for (int i = 0; i < 8; i++) {

        // bounds checking
        neighborRow = cellRow + neighbors[i][0];
        neighborRow = (neighborRow >= 0) ? neighborRow : 0;
        neighborRow = (neighborRow < displayGrid.numRows()) ? neighborRow : displayGrid.numRows() - 1;

        neighborCol = cellCol + neighbors[i][1];
        neighborCol = (neighborCol >= 0) ? neighborCol : 0;
        neighborCol = (neighborCol < displayGrid.numCols()) ? neighborCol : displayGrid.numCols() - 1;

        if (displayGrid.get(neighborRow, neighborCol) == 'X') {
            count++;
        }
    }

    char cellStatus = displayGrid.get(cellRow, cellCol);
    switch (count) {
        case 2:
            break;
        case 3:
            cellStatus = 'X';
            break;
        default:
            cellStatus = '-';
            break;
    }
    return cellStatus;
}






























