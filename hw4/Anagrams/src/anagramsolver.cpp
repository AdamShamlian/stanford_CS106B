// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

// Please feel free to add any other #includes you need!
#include <iostream>
#include <string>
#include "letterinventory.h"
#include "set.h"
#include "strlib.h"
#include "foreach.h"
#include "queue.h"
#include "stack.h"
using namespace std;



bool anagram_recurser(LetterInventory &phraseDecomposition,
                      Vector<Vector<string> > &possibleAnagrams,
                      Vector<string> &wordsToCheck,
                      Vector<string> &thisAnagram, int max);

int findAnagrams(string phrase, int max, Set<string>& dictionary) {

    // check for pathological inputs
    if (max < 0) {
        throw max;
    }

    if (phrase == "") {
        return 0;
    }

    // this is to match provided output, though im not sure this is
    // intended behavior? there are some "none" provided maxes that print out
    // a number of anagrams
    if (max == 0) {
        max = phrase.length();
    }

    // change phrase to list of letters
    LetterInventory phraseDecomp = LetterInventory(phrase);

    // build a list of words to check, which will make the recursion tractable
    // linear w.r.t. size of dictionary
    Vector<string> wordsToCheck = Vector<string>();
    foreach (string vocab in dictionary) {
        if (phraseDecomp.contains(vocab)) {
            wordsToCheck.add(vocab);
        }
    }

    // initialize variables to be returned to us by reference
    Vector<Vector<string>> possibleAnagrams = Vector<Vector<string>>();
    Vector<string> storedWords = Vector<string>();
    Vector<string> thisAnagram = Vector<string>();
    anagram_recurser(phraseDecomp, possibleAnagrams, wordsToCheck, thisAnagram, max);

    foreach (Vector<string> anagram in possibleAnagrams) {
        cout << anagram.toString() << endl;
    }

    return possibleAnagrams.size();
 }


bool anagram_recurser(LetterInventory &phraseDecomposition,
                      Vector<Vector<string> > &possibleAnagrams,
                      Vector<string> &wordsToCheck,
                      Vector<string> &thisAnagram, int max) {

    // this series of if clauses is designed to check, first, if there
    // is a complete anagram, and second, what value max is at. this is
    // done in a way such that we always check for anagrams first UNTIL
    // max is reached.
    if (max < 0) {
        return false;
    }
    else if (phraseDecomposition.isEmpty()) {
        possibleAnagrams.add(thisAnagram);
        return true;
    }
    else if (max == 0) {
        return false;
    }
    else {
        // pass
    }

    bool deeperAnagram; // declare in scope
    foreach (string word in wordsToCheck) {

        deeperAnagram = false; // ensures we return the proper default, regardless of ifs

        if (phraseDecomposition.contains(word)) {

            // build up anagram and work from there, then tear down so that we
            // can continue to check from the original state we had in this
            // particular recursive call
            thisAnagram.add(word);
            phraseDecomposition.subtract(word);
            deeperAnagram = anagram_recurser(phraseDecomposition, possibleAnagrams,
                                           wordsToCheck, thisAnagram, max-1);
            thisAnagram.remove(thisAnagram.size()-1);
            phraseDecomposition.add(word);
        }
        // this is a bit redundant, i suppose
        else {
            continue;
        }
    }
    return deeperAnagram;
}
