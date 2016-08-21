#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "map.h"
#include "random.h"

using namespace std;

void print_startup_message();
int load_N();
int load_num_words_to_generate(int);
void load_file(std::ifstream &);
Vector<string> process_file_by_word(ifstream &);
Map<Vector<string>, Vector<string>> construct_n_gram_possibilities(Vector<string> &, int);
void generate_n_grams(Map<Vector<string>, Vector<string> > &, int, int);

int main() {
    // TODO: Finish the program!

    print_startup_message();

    std::ifstream sourceFile;
    load_file(sourceFile);

    int N = load_N();
    int M;

    Vector<string> corpus = process_file_by_word(sourceFile);
    Map<Vector<string>, Vector<string>> potentialNGrams =
            construct_n_gram_possibilities(corpus, N);

    while (true) {

        M = load_num_words_to_generate(N);
        if (M == 0) {
            break;
        }
        generate_n_grams(potentialNGrams, N, M);
    }

    cout << "Exiting." << endl;
    return 0;
}


void print_startup_message() {

    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
    cout << endl;
    return;
}


/* load_N():
 *
 * Prompts user for the chain size N, until N is a valid
 * integer greater than 1.
 *
 */
int load_N() {

    int N = 0;
    while (true) {

        N = getInteger(string("Value of N? "));
        if (N >= 2) {
            break;
        }
        cout << "N must be 2 or greater." << endl;
    }
    return N;
}


/* load_num_words_to_generate():
 *
 * Prompts user for the number of words to generate,
 * until M is a valid integer greater than N (from load_N)
 * or M is 0, in which case we shall quit the program.
 *
 */
int load_num_words_to_generate(int N) {

    int M = 0;
    while (true) {

        cout << endl; // just for formatting to Stanford's wishes
        M = getInteger(string("# of random words to generate (0 to quit)? "));
        if (M == 0 || M >= N) {
            break;
        }
        cout << "Must be at least " << N << " words." << endl;
    }
    return M;
}


void load_file(std::ifstream & file) {

    string test = promptUserForFile(file, string("Input file name? "));
    return;
}


/* process_file_by_word():
 *
 * Stores all the "words" of the provided file in a vector.
 * Words are defined to be a sequence of characters delimited
 * by whitespace.
 */
Vector<string> process_file_by_word(std::ifstream& file) {

    Vector<string> corpus = Vector<string>();
    string word;
    while (file >> word) {
        corpus.add(word);
    }
    file.close();
    return corpus;
}


/* construct_n_gram_possibilities():
 *
 * This guy is a little tricky. Firstly, it goes through all the
 * non wraparound subsequences of the corpus, and makes them keys of
 * the map. After that, it will use a decreasingly small amount of the
 * corpus as the window is within N of the end of the corpus. The
 * remaining length, N - length_of_window_to_end_of_corpus, is retrieved
 * at the beginning of the corpus.
 *
 * There's actually a way to embed this all in the while loop. The line
 * number xxx (Vector<string> window = corpus.subList(i, corpus.size()))
 * could be ... corpus.subList(i, min(i+N, corpus.size());
 */
Map<Vector<string>, Vector<string>> construct_n_gram_possibilities(Vector<string>& corpus, int N) {

    Map<Vector<string>, Vector<string>> potentialNGrams = Map<Vector<string>, Vector<string>>();

    // typically, i would go for the modulo operator here
    // but Vector.subList() doesn't play too nicely with that idea
    int i;
    Vector<string> window;
    for (i = 0; i < (corpus.size() - N); i++) {

        window = corpus.subList(i, N);

        if (!potentialNGrams.containsKey(window)) {
            potentialNGrams.add(window, Vector<string>());
        }
        potentialNGrams[window].add(corpus[i + N]);
    }

    // add the wrap-around... note that i is still useful
    while (i < corpus.size()) {

        window = corpus.subList(i, corpus.size() - i);

        int j;
        for (j = 0; j < (N + i - corpus.size()); j++) {
            window.add(corpus[j]);
        }

        if (!potentialNGrams.containsKey(window)) {
            potentialNGrams.add(window, Vector<string>());
        }
        potentialNGrams[window].add(corpus[j]);
        i++;
    }
    return potentialNGrams;
}


void generate_n_grams(Map<Vector<string>, Vector<string>>& potentialNGrams, int N, int M) {

    int intStart = randomInteger(0, potentialNGrams.size());
    Vector<string> keyToStart = potentialNGrams.keys()[intStart]; // is this a copy? i think so

    // get the N-GRAM of length M !
    for (int i = 0; i < M; i++) {

        Vector<string> potentialNextWords = potentialNGrams[keyToStart.subList(i, N)];
        string randomWord = potentialNextWords[randomInteger(0, potentialNextWords.size()-1)];
        keyToStart.add(randomWord);
    }

    // output
    cout << "... ";
    for (int i = 0; i < M; i++) {
        cout << keyToStart[i] << " ";
    }
    cout << "..." << endl;

    return;
}



































































