// Several minor problems have been posed as part of
// Problem set 3. What follows are the solutions to
// those problems.

// In some cases, we have to jump through hoops to fit the
// accepted reponses. In most cases, however, the required
// formatting is provided for us in recursionmain.cpp!

// Please feel free to add any other #includes you need!
#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
using namespace std;


/* Karel lives on a grid as specified by the problem. We
 * use the provided index to see our number of paths. Notice
 * that this is Pascal's triangle.
 */
int countKarelPaths(int street, int avenue) {

    // input sanity checks
    if ((street < 1) || (avenue < 1)) {
        throw string("Avenue or street is less than 1");
        return 0;
    }

    if ((street == 1) || (avenue == 1)) {
        return 1;
    }
    else {
        return countKarelPaths(street-1, avenue) +
            countKarelPaths(street, avenue-1);
    }
}

int convertStringToInteger(string exp) {
    // TODO: write this function
    return 0;
}

bool isBalanced(string exp) {
    // TODO: write this function
    return false;
}

double weightOnKnees(int row, int col, Vector<Vector<double> >& weights) {
    // TODO: write this function
    return 0.0;
}

void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    // TODO: write this function

}

int floodFill(GBufferedImage& image, int x, int y, int color) {
    // TODO: write this function
    return 0;
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    // TODO: write this function
    Vector<string> v;
    return v;
}












