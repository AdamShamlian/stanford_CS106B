// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include <string>
#include "boggle.h"
#include "grid.h"
#include "stack.h"
#include "lexicon.h"
#include "strlib.h"
using namespace std;

/* verify_recurser():
 *
 * I actually wrote this after the computer search recurser. Notice
 * how similar they are.
 */
bool verify_recurser(Grid<char>& board, string word, Set<Vector<int>>& usedLocations,
                     int curRow, int curCol) {

    // negative base csae when we dont find a letter anywhere nearby
    if (!board.get(curRow, curCol) == word[0]) {
        return false;
    }

    // positive basecase when a single char is left and nearby
    if ((word.length() == 1) && (board.get(curRow, curCol) == word[0])) {
        return true;
    }

    // loop through the possible directions we can go
    int newRow, newCol;
    Vector<int> loc;
    bool res;

    for (int tmpCol = -1; tmpCol < 2; tmpCol++) {
        for (int tmpRow = -1; tmpRow < 2; tmpRow++) {

            res = false; // ensures returning a proper default value

            // these were declared above, outside of the loop
            newRow = curRow + tmpRow;
            newCol = curCol + tmpCol;
            loc = Vector<int>();
            loc += newRow, newCol;

            // pathological checks to ignore: same place, oob, already used
            if ((newRow == curRow) & (newCol == curCol)) {
                continue;
            }
            else if (!board.inBounds(newRow, newCol)) {
                continue;
            }
            else if (usedLocations.contains(loc)) {
                continue;
            }
            else {
                // the secret sauce is here!
                usedLocations.add(loc);
                res = verify_recurser(board, word.substr(1, word.length()), usedLocations,
                                newRow, newCol);
                if (res) { return true; } // if we verify the word, stop the search
                usedLocations.remove(loc);
            }
        }
    }
    return res;
}

/* humanWordSearch():
 *
 * Ideally, we call the computer's turn first and check against
 * human input values to obtain the human's score and the computer's
 * score. However, we can't do this without modifying this fcn's
 * signature (which isn't allowed).
 */
bool humanWordSearch(Grid<char>& board, string word) {

    // we know the board uses upper case only
    toUpperCaseInPlace(word);

    Set<Vector<int>> usedLocations = Set<Vector<int>>();

    for (int curCol = 0; curCol < board.numCols(); curCol++) {
        for (int curRow = 0; curRow < board.numRows(); curRow++) {

            if (verify_recurser(board, word, usedLocations, curRow, curCol)) {
                return true;
            }
        }
    }
    return false;
}


/* boggle_recurser():
 *
 * This function performs the recursion of the assignment.
 */
bool boggle_recurser(Set<string>& foundWords, Grid<char>& board, Lexicon& dictionary,
                     string word, Set<Vector<int>>& usedLocations,
                     int curRow, int curCol, int wordLength) {

    // prune the recursive tree
    if (!dictionary.containsPrefix(toLowerCase(word))){
        return false;
    }

    // base case, const provided in boggle.h
    if (wordLength >= MIN_WORD_LENGTH) {

        // dict only has lowercase words, btw
        if (dictionary.contains(toLowerCase(word))) {
            foundWords.add(word);
            // gotta keep going for longer length words
            boggle_recurser(foundWords, board, dictionary,
                            word, usedLocations, curRow, curCol, wordLength);
            return true;
        }
        else {
            return false;
        }
    }

    // loop through the possible directions we can go
    int newRow, newCol;
    Vector<int> loc;
    bool res;

    for (int tmpCol = -1; tmpCol < 2; tmpCol++) {
        for (int tmpRow = -1; tmpRow < 2; tmpRow++) {

            res = false; // ensures returning a proper default value

            // these were declared above, outside of the loop
            newRow = curRow + tmpRow;
            newCol = curCol + tmpCol;
            loc = Vector<int>();
            loc += newRow, newCol;

            if ((newRow == curRow) & (newCol == curCol)) {
                continue;
            }
            else if (!board.inBounds(newRow, newCol)) {
                continue;
            }
            else if (usedLocations.contains(loc)) {
                continue;
            }
            else {
                // the secret sauce is here!
                usedLocations.add(loc);
                word += board.get(newRow, newCol);
                res = boggle_recurser(foundWords, board, dictionary, word, usedLocations,
                                newRow, newCol, wordLength - 1);
                word.pop_back();
                usedLocations.remove(loc);
            }
        }
    }
    return res;
}

/* computerWordSearch():
 *
 * Exhaustively looks through the board for any viable boggle words, returning all
 * that have been located at the end of the function.
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary) {

    Set<string> allWords;
    Set<Vector<int>> usedLocs = Set<Vector<int>>(); // tracks letters we cant re-use
    string word; // dummy var for use in recursion

    for (int curCol = 0; curCol < board.numCols(); curCol++) {
        for (int curRow = 0; curRow < board.numRows(); curRow++) {
            boggle_recurser(allWords, board, dictionary, word, usedLocs,
                            curRow, curCol, 4);
        }
    }
    return allWords;
}
