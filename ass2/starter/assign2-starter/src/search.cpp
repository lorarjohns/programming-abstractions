// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "search.h"
#include <ctype.h> // ispunc
#include <algorithm> // std::any_of
#include <iterator>
using namespace std;

 /** Converts a string to lowercase.
  *
  * @param string s, string to convert.
  * @return string result, lowercased
  * version of the string.
  */
string lowerCase (string s) {
    string result = "";
    for (unsigned int i = 0; i < s.length(); i++) {
        result += tolower(s[i]);
        }
    return result;
}

/** Checks whether a string contains
 * at least one alphabetic character.
 *
 * @param string s, the string to check.
 * @return bool true if contains an alphabetic
 * character, false if not.
 */
bool hasLetter(string s) {
    for (char c: s) {
        if (isalpha(c)) {
            return true;
        }
    }
    return false;
}

/** Normalizes tokens by removing numbers and terminal
 * punctuation. If token contains no characters other
 * than numbers and punctuation, returns an empty string.
 *
 * @param string& s, reference to token to process.
 * @return string result, the original token, lowercased
 * and with leading and trailing punctuation removed. If
 * the original token contains no alphabetic characters,
 * returs the empty string.
 */
string cleanToken(string token) {
    /* TODO: Fill in the remainder of this function. */
    if (ispunct(token[0])) {
            // cleanToken(token.substr(1, token.length()-2));
            return cleanToken(token.substr(1));
    }
    else if (ispunct(token[token.length()-1])) {
            return cleanToken(token.substr(0, token.length()-1));
} else {
       if (hasLetter(token)) {
           string result = lowerCase(token);
           return result;
       } else {
           return "";
       }
    }
}

/** Read in a file with alternating web page URLs and
 * their corresponding content and create an index with the
 * URLs as keys and the tokenized and cleaned content as
 * values.
 *
 * @param string dbfile, location on disk of the text file
 * with the information to read into the map
 * @return Map<string, Set<string>> docs, a map with URLs as
 * keys and the page content tokenized and normalized as values.
 */
Map<string, Set<string>> readDocs(string dbfile) {
    Map<string, Set<string>> docs;
    /* Read the data from the file into a vector
     * of strings.
     */
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;

    readEntireFile(in, lines);

    string key;
    Vector<string> urlParts;

    /* Tokenize each URL */
    for (int i = 0; i < lines.size()-1; i=i+2) {
        string key = lines[i];
        Vector<string> urlParts = stringSplit(lines[i+1], " ");
            for (string token: urlParts) {
                string cleaned = cleanToken(token);
                if (cleaned != "") {
                    docs[key].add(cleaned);
                }
            }
    }
    return docs;
}

/** Helper function for buildIndex to add a key, value pair
 * of URL / terms from docs to the inverted index.
 *
 * @param string& key, reference to a URL
 * @param Set<string>& values, reference to a set of document terms
 * @param Map<string, Set<string>>& index, reference to the inverted index
 * into which the inverted values are to be inserted
 */
void reverseIndex(string& key, Set<string>& values, Map<string, Set<string>>& index) {
    for (string word: values) {
        index[word].add(key);
    }
}

/** Converts a forward index into an inverted index that maps from words
 * to the pages that contain them.
 *
 * @param Map<string, Set<string>>& docs, reference to an index of documents
 * to the words that are contained in those documents.
 * @return Map<string, Set<string>> index, an inverted index in which unique
 * words are the keys and the documents in which they appear are contained
 * in the values.
 */
Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs) {
    Map<string, Set<string>> index;
    /* TODO: Fill in the remainder of this function. */
    for (string key: docs.keys()) {
        reverseIndex(key, docs[key], index);
    }

    return index;
}

/** Searches index for search terms, subject to boolean set operations.
 *
 * @param Map<string, Set<string>>& index, reference to inverted index of
 * terms and pages.
 * @param string query, space-delimited list of one or more search terms
 * with optional operators:
 *     + indicates set intersection ("spam AND eggs")
 *     - indicates set difference ("eggs AND NOT bacon")
 *     no operator indicates set union ("ham OR spam")
 * Search is left-associative.
 * @return Set<string> result, set of matching URLs containing desired
 * queries.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    /* TODO: Fill in the remainder of this function. */
    Vector<string> searchTerms = stringSplit(query, " ");
    for (string term: searchTerms) {
        switch(term[0]) {
        case '+':
            result = result * index[cleanToken(term)]; // "intersection"
            break;
        case '-':
            result = result - index[cleanToken(term)]; // "difference"
            break;
        default:
            result = result + index[cleanToken(term)]; // "union"
        }
    }
    return result;
}

/** Console program that accepts user search query and returns
 * list of URLs that contain matching terms.
 *
 * @param string dbfile, location on disk containing URLs and
 * page contents.
 * @return void, prints interactively to console, allowing user
 * to input search queries and informing user how many web pages
 * were searched and indexed, how many unique words were indexed,
 * and which pages contained matches for the queries submitted.
 */
void searchEngine(string dbfile) {
    /* read the docs into memory */
    Map<string, Set<string>> docs = readDocs(dbfile);

    cout << "Stand by while building index..." << endl;

    /* create the index */
    Map<string, Set<string>> index = buildIndex(docs);

    cout << "Indexed " << docs.keys().size() << " pages containing " <<
            index.keys().size() << " unique terms." << endl;

    while (true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        if (query == "") {
            break;
        }
        Set<string> result = findQueryMatches(index, query);

        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on tokens in tiny") {
    EXPECT_EQUAL(cleanToken("EGGS!"), "eggs");
    EXPECT_EQUAL(cleanToken("milk"), "milk");
    EXPECT_EQUAL(cleanToken("fish"), "fish");
    EXPECT_EQUAL(cleanToken("@"), "");
    EXPECT_EQUAL(cleanToken("bread"), "bread");
    EXPECT_EQUAL(cleanToken("cheese"), "cheese");

}
STUDENT_TEST("lowerCase on EGGS!") {

    EXPECT_EQUAL(lowerCase("EGGS"), "eggs");

}
STUDENT_TEST("hasLetter on \"@\"") {
    EXPECT_EQUAL(hasLetter("@"), false);
}

STUDENT_TEST("searchEngine") {
    searchEngine("res/website.txt");
}
