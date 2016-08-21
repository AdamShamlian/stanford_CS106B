#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "filelib.h"
#include "strlib.h"
#include "queue.h"
#include "stack.h"
#include "foreach.h"

using namespace std;

void print_startup_message();
Lexicon load_dictionary();
bool load_words(Lexicon &, string &, string &);
void construct_word_ladder(Lexicon &, string &, string &);
Lexicon get_lexical_neighbors(Lexicon &, string);
void print_ladder(Stack<string> &, string &, string &);


int main() {
    // TODO: Finish the program!

    print_startup_message();
    Lexicon dictionary = load_dictionary();

    // this is hacky af but w/e
    string wordOne, wordTwo;
    bool prematureExit = load_words(dictionary, wordOne, wordTwo);
    while (!prematureExit) {

        // if we didn't get "Enter" (empty string) during word input
        // continue on with the ladder creation
        construct_word_ladder(dictionary, wordOne, wordTwo);

        // order of these two statements is flipped because of
        // initial call to load_words. ordering this way
        // guarantees we dont call construct_word_ladder on bogus words
        prematureExit = load_words(dictionary, wordOne, wordTwo);
    }

    cout << "Have a nice day." << endl;
    return 0;
}


/* print_startup_message():
 *
 * This merely wraps any initial output to the console.
 */
void print_startup_message() {

    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by change one letter at a time." << endl;
    cout << endl;
    return;
}


/* load_lexicon():
 *
 * Encapsulates the loading of the dictionary of words
 * to be used. Accepts a viable .dat or .txt file. If
 * the given file does not exist, it will reprompt for
 * a new file.
 */
Lexicon load_dictionary() {

    string filename;
    cout << "Dictionary file name? ";
    getline(cin, filename);

    if (!fileExists(filename)) {
        cout << "Unable to open that file. Try again." << endl;
        return load_dictionary();
    }
    else {
        return Lexicon(filename);
    }
}


/* load_words():
 *
 * Encapsulates word loading process. Prompts user for
 * words with which to construct a ladder. If the words
 * are not the same length, fail to appear in the dictionary,
 * or are the same, the function will display this and reprompt.
 */
bool load_words(Lexicon& dictionary, string& wordOne, string& wordTwo) {

    // load word one and check to see whether we should quit
    cout << endl << "Word #1 (or Enter to quit): ";
    getline(cin, wordOne);

    if (wordOne.empty()) {
        return true;
    }

    // do the same for word two
    cout << "Word #2 (or Enter to quit): ";
    getline(cin, wordTwo);

    if (wordTwo.empty()) {
        return true;
    }

    toLowerCaseInPlace(wordOne);
    toLowerCaseInPlace(wordTwo);

    if (equalsIgnoreCase(wordOne, wordTwo)) {
        cout << "The two words must be different." << endl;
        return load_words(dictionary, wordOne, wordTwo);
    }
    else if (wordOne.length() != wordTwo.length()) {
        cout << "The two words must be the same length." << endl;
        return load_words(dictionary, wordOne, wordTwo);
    }
    else if (!dictionary.contains(wordOne) || !dictionary.contains(wordTwo)) {
        cout << "The two words must be found in the dictionary." << endl;
        return load_words(dictionary, wordOne, wordTwo);
    }
    else {
        return false;
    }
}


/* construct_word_ladder():
 *
 * Pretty self-explanatory here.
 */
void construct_word_ladder(Lexicon & dictionary, string & wordOne, string & wordTwo) {

    // set up initial conditions
    Queue<Stack<string>> fullLadder = Queue<Stack<string>>();
    Stack<string> firstPartialLadder = Stack<string>();
    firstPartialLadder.push(wordOne);
    fullLadder.enqueue(firstPartialLadder);

    // we ignore these words if they pop up to:
    // - guarantee shortest path
    // - avoid infinite ladders
    Lexicon wordsToIgnore = Lexicon();
    wordsToIgnore.add(wordOne);

    while (!fullLadder.isEmpty()) {

        Stack<string> partialLadder = fullLadder.dequeue();
        string partialLadderFirstWord = partialLadder.peek();

        Lexicon neighbors = get_lexical_neighbors(dictionary, partialLadderFirstWord);
        foreach (string neighbor in neighbors) {

            // recall we want to avoid repeating words...
            if (!wordsToIgnore.contains(neighbor)) {

                // winner winner chicken dinner
                if (neighbor == wordTwo) {
                    print_ladder(partialLadder, wordOne, wordTwo);
                    return;
                }
                else {
                    // stitch together the potential ladder
                    Stack<string> partialLadderCopy = partialLadder;
                    partialLadderCopy.push(neighbor);
                    fullLadder.enqueue(partialLadderCopy);

                    wordsToIgnore.add(neighbor); // be sure to add this word to ignore list
                }
            }
        }
    }

    // this comment is a little redundant, but more english-y
    // if we get here then we have looked at every partial ladder,
    // and in each such partial ladder, we have looked at all possible neighbors
    // AND we STILL haven't found a ladder, in which case...
    cout << "No word ladder found from " << wordTwo << " back to " << wordOne << "." << endl;
    return;
}


/* get_lexical_neighbors():
 *
 * Generates a Lexicon instance containing all words of the same
 * length as the given word, such that only one character differs
 * between the given word and the neighboring word.
 */
Lexicon get_lexical_neighbors(Lexicon & dictionary, string wordToCheck) {

    Lexicon neighbors = Lexicon();
    for (int i = 0; i < wordToCheck.length(); i++) {

        for (int j = 97; j < 123; j++) { // j loops through ascii code no's for lowercase alphabet

            string potentialNeighbor = wordToCheck; // copy to preserve wordToCheck
            // note to self: string::replace is safer than
            // string index replacement, because attempting [] on the empty
            // string results in UB. in this case we are guaranteed non empty string
            potentialNeighbor[i] = char(j);
            if (dictionary.contains(potentialNeighbor)) {
                neighbors.add(potentialNeighbor);
            }
        }
    }
    return neighbors;
}


/* print_ladder():
 *
 * Also pretty self-explanatory.
 * Prints the completed word ladder in reverse.
 */
void print_ladder(Stack<string>& completeLadder, string& wordOne, string& wordTwo) {

    cout << "A ladder from " << wordTwo << " back to " << wordOne << ":" << endl;
    cout << wordTwo;

    while (!completeLadder.isEmpty()) {
        string nextWordInLadder = completeLadder.pop();
        cout << " " << nextWordInLadder;
    }

    cout << endl;
    return;
}




























