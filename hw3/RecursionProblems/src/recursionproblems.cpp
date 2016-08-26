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
#include "vector.h"
#include "strlib.h"
#include "foreach.h"
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




/* Recursive meat and potatoes of the string to integer conversion
 */
int stoi_recurser(string stringToFlip) {

    if (stringToFlip.length() == 1) {
        return stringToFlip[0] - '0'; // how to safely convert string of length one to char
    }
    else {
        return 10 * stoi_recurser(stringToFlip.substr(0, stringToFlip.length() - 1)) +
                stoi_recurser(stringToFlip.substr(stringToFlip.length() - 1));
    }
}

/* Flips a valid string to an integer. A valid string contains
 * only numeral characters, and optionally a hyphen.
 * This function wraps the actual recursion.
 */
int convertStringToInteger(string stringToFlip) {

    // input sanity checks
    for (int i = 0; i < stringToFlip.length(); i++) {
        if ((stringToFlip[i] < '0') || (stringToFlip[i] > '9')) {
            if (stringToFlip[i] != '-') {
                throw "String is not in a valid format!";
                return 0;
            }
        }
    }

    int multiplier, flippedInt;
    if (stringToFlip[0] == '-') {
        multiplier = -1;
        flippedInt = stoi_recurser(stringToFlip.substr(1, stringToFlip.length() - 1));
    }
    else {
        multiplier = 1;
        flippedInt = stoi_recurser(stringToFlip);
    }

    return multiplier * flippedInt;
}



/* Iterates through the "bracket-like" strings and determines
 * whether any are present. In greedy fashion, it returns the
 * provided string with the first found bracket-like string removed.
 * If no such string is found, it returns the original string (hence
 * the lack of return by ref).
 */
string check_for_balance(string s) {

    // a collection of the substrings to check for
    Vector<string> stringsToFind = Vector<string>();
    stringsToFind += "{}", "()", "<>", "[]"; // may need to call string() for them

    foreach (string subStr in stringsToFind) {

        std::size_t found = s.find(subStr); // std::size_t IS NOT THE SAME as uint or int, its arch dependent
        if (found != string::npos) {
            return s.erase(found, 2);
        }
    }

    return s;
}

/* Recursively determines whether the provided string is balanced as
 * defined in the problem set.
 */
bool isBalanced(string s) {

    // base case, empty string
    if (s.empty()) {
        return true;
    }

    // otherwise, we filter the string for brackets
    else {
        string filteredS = check_for_balance(s);

        // and if the filter fails to find brackets, then the string
        // is unbalanced
        if (filteredS == s) {
            return false;
        }
        else {
            return isBalanced(filteredS);
        }
    }
}







/* Given a "triangle" of weights, returns the total load on the person
 * at (row, col) in the triangle.
 */
double weightOnKnees(int row, int col, Vector<Vector<double>>& weights) {

    // for the top person
    if ((row == 0) & (col == 0)) {
        return weights[0][0];
    }

    // for outside the human triangle to the right side
    if ((row > weights.size() - 1) || (col > weights[row].size() - 1)) {
        return 0;
    }
    // for outside to the left
    if (col < 0) {
        return 0;
    }

    else {
        double knees = weights[row][col];
        double leftShoulder = 0.5 * (weightOnKnees(row-1, col-1, weights));
        double rightShoulder = 0.5 * weightOnKnees(row-1, col, weights);
        return knees + leftShoulder + rightShoulder;
    }
}





/* Recursively illustrates the Sierpinksi Triangle, given a GWindow, starting point,
 * initial size and order to which to draw. I must say, the hints/restrictions in the
 * homeworks are pretty misleading. Draw three K-1-order triangles, but don't redraw
 * lines? Not sure that's possible without call some more specialized functions (which
 * they also don't want). However, we can draw ONE inverted K-1 order triangle?
 */
void drawSierpinskiTriangle(GWindow & gw, double x, double y, double size, int order) {

    // for manipulating coordinates
    double xMultiplier = 0.5;
    double yMultiplier = sqrt((double)3)/ 2.0;

    if ((x < 0) || (y < 0) || (size < 0) || (order < 0)) {
        throw "X, Y, Size, or Order are negative!";
        return;
    }

    // should not happen unless passed order 0 directly
    if (order == 0) {
        return;
    }

    if (order == 1) {
        gw.drawLine(x, y, x+size, y);
        gw.drawLine(x, y, x + xMultiplier*size, y + yMultiplier*size);
        gw.drawLine(x + xMultiplier*size, y + yMultiplier*size, x + size, y);
        return;
    }

    double newSize = size / 2;
    drawSierpinskiTriangle(gw, x, y, newSize, order - 1);
    drawSierpinskiTriangle(gw, x + newSize, y, newSize, order - 1);
    drawSierpinskiTriangle(gw, x + xMultiplier*newSize, y + yMultiplier*newSize, newSize, order - 1);
}




/* Another embedded recursion function. We embed the recursion in here
 * so that we can test against the original color and know when to stop
 */
int flood_fill_recurser(GBufferedImage & image, int x, int y, int color, int initialColor) {

    // if you go oob, do nothing
    if (!image.inBounds(x, y)) {
        throw "You tried to go outside the pictures bounds.";
        return 0; // ? is this nothing
    }

    // click on the same color pixel, do nothing as well
    if (image.getRGB(x, y) == color) {
        return 0;
    }

    // change to the provided color
    image.setRGB(x, y, color);

    // check if we are at a color boundary:
    int leftFlood, rightFlood, upFlood, downFlood;
    if (image.getRGB(x - 1, y) != initialColor) {
        leftFlood = 0;
    }
    else {
        leftFlood = floodFill(image, x-1, y, color);
    }

    if (image.getRGB(x + 1, y) != initialColor) {
        rightFlood = 0;
    }
    else {
        rightFlood = floodFill(image, x+1, y, color);
    }

    if (image.getRGB(x, y + 1) != initialColor) {
        upFlood = 0;
    }
    else {
        upFlood = floodFill(image, x, y+1, color);
    }

    if (image.getRGB(x, y - 1) != initialColor) {
        downFlood = 0;
    }
    else {
        downFlood = floodFill(image, x, y-1, color);
    }
    return 1 + leftFlood + rightFlood + upFlood + downFlood;
}

/* Returns the number of color-altered pixels in the provided image. This
 * function also manipulates the image color as it goes.
 */
int floodFill(GBufferedImage & image, int x, int y, int color) {

    int initialColor = image.getRGB(x, y);
    return flood_fill_recurser(image, x, y, color, initialColor);
}





/* Function used to load and format the grammar from the
 * provided input stream object
 */
Map<string, string> load_grammar(istream& input) {

    Map<string, string> grammar = Map<string, string>();

    while (!input.eof()) {

        // acquire the grammar non-terminal line
        string line;
        getLine(input, line);

        // split into a key and rules, add to grammar
        Vector<string> splitLine = stringSplit(line, "::=");
        if (grammar.containsKey(splitLine[0])) { // stanford wants this
            throw "This non-terminal already has definitions!";
            return grammar;
        }
        grammar.add(splitLine[0], splitLine[1]);
    }

    return grammar;
}


/* Returns a vector of strings that is formattable for output in the grammar
 * emission problem. I'm not sure this is in the spirit of the hw as there is
 * technically a loop in here. :(
 */
Vector<string> recurse_grammar(Map<string, string> & grammar, string symbol) {

    Vector<string> message = Vector<string>();

    // base case, provided with a terminal
    if (!grammar.containsKey(symbol)) {
        message.add(symbol);
    }
    else {

        Vector<string> potentialRules = stringSplit(grammar[symbol], "|");
        string randomRule = potentialRules[randomInteger(0, potentialRules.size() - 1)];

        // at this point we have either a terminal or a string of spaced non terminals
        // we trim the string for safety/formatting
        // then we split by whitespace, which will neatly give us the terminal
        // or the first nonterminal
        Vector<string> selectedRule = stringSplit(trim(randomRule), " ");

        // recurse on the symbols in the selected rule
        foreach (string newSymbol in selectedRule) {
            message += recurse_grammar(grammar, newSymbol);
        }
    }

    return message;
}

/* Consolidates the grammar generation process: loads the grammar, ensures
 * input is viable and then proceeds to emit "sentences" as many times as is
 * specified.
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    // format the grammar we need
    Map<string, string> grammar = load_grammar(input);

    // check to make sure that the first provided symbol is not the empty string
    if (symbol == "") {
        throw "The empty string does not exist in the grammar.";
        return Vector<string>();
    }

    // generate times number of strings
    Vector<string> grammarEmission = Vector<string>();
    for (int i = 0; i < times; i++) {

        // get the ith emission vector, and then splice it together with spaces. add to return val
        Vector<string> thisEmission = recurse_grammar(grammar, symbol);
        string tmpString = stringJoin(thisEmission.toStlVector(), " ");
        grammarEmission.add(tmpString);
    }

    return grammarEmission;
}












