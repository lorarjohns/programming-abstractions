// Program to compute Soundex codes and provide
// a user interface for finding similar names
// based on them.
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include <algorithm>
#include <set>
#include <map>
using namespace std;

/** @brief This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 * @param s, string to process
 * @return s, string with non-letter characters removed
 *
 * The original function doesn't account for the fact that it is
 * shortening s with each character removed, so i will become
 * greater than s.length() before the function has finished
 * removing all non-alpha characters.
 *
 * Decrementing i when a non-alpha character is found will
 * solve the problem.
 */
string removeNonLetters(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {

        if (!isalpha(s[i]) || s[i] == '#' || s[i] == '%' || s[i] == '&') {
            // remove the character at index i
            s = s.substr(0,i) + s.substr(i+1);
            i--;
        }
    }
    return s;
}


/**
 * @brief lowercaseString: Converts all characters in string to lower-
 * case.
 * @param s, string to convert
 * @return s, string in lowercase
 */
string lowercaseString(string s) {
   // see https://en.cppreference.com/w/cpp/algorithm/transform
   transform(s.begin(), s.end(), s.begin(),
             [](unsigned char c) -> unsigned char {return tolower(c); });
   return s;
};

/**
 * @brief encodeCharacters: Uppercases the first letter of
 * the input name. Replaces remaining alphabetic characters
 * with corresponding Soundex code number.
 * @param s
 * @return result
 */
string encodeCharacters (string s) {

    string name = lowercaseString(s);

    string result;
    // result += toupper(s[0]);

    for (char c: name) {
        switch(c) {
        case 'a' :
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'h':
        case 'w':
        case 'y':
            result += '0';
            break;
        case 'b':
        case 'f':
        case 'p':
        case 'v':
            result += '1';
            break;
        case 'c':
        case 'g':
        case 'j':
        case 'k':
        case 'q':
        case 's':
        case 'x':
        case 'z':
            result += '2';
            break;
        case 'd':
        case 't':
            result += '3';
            break;
        case 'l':
            result += '4';
            break;
        case 'm':
        case 'n':
            result += '5';
            break;
        case 'r':
            result += '6';
            break;
        }
    }
    return result;
}

/**
 * @brief removeZeros: Removes all instances of '0' from string.
 * @param s, string to process
 * @return result, copy of s without '0's
 */
string removeZeros(string s) {
    string result = "";
    for (char c: s) {
        if (c != '0') {
            result += c;
        }
    }
    return result;
}

/**
 * @brief normalizeLength: Pads code to 4 characters if it is
 * shorter than 4, or truncates code to 4 characters if it is
 * longer.
 * @param s, the raw Soundex code with zeros removed
 * @return s, the Soundex code with exactly 4 characters
 */
string normalizeLength(string s) {

    if (s.length() > 4) {
        s = s.substr(0, 4);
    } else if (s.length() < 4) {
        while (s.length() < 4) {
            s += '0';
        }
    }
    return s;
}

/**
 * @brief coalesceDigits: Collapse repeated adjacent digits
 * @param s
 * @return s, string with no adjacent repeated digits
 */
string coalesceDigits(string s) {
    // see http://www.cplusplus.com/reference/string/string/erase/
    // for erase documentation
    for (unsigned int i = 0; i < s.length()-1; i++) {
        if (s[i] == s[i+1]) {
            s.erase(i, 1);
            i--;
        }
    }

    return s;
}
/**
 * @brief soundex: A function to convert a name into
 * its corresponding Soundex code.
 * @param s, the name to convert
 * @return s, the soundex code for that name
 */
string soundex(string s) {

    char firstLetter = toupper(s[0]);

    // remove all non-letters
    s = removeNonLetters(s);
    // encode each letter as a digit
    s = encodeCharacters(s);
    // coalesce adjacent digits
    s = coalesceDigits(s);

    // replace 1st letter original name
    s[0] = firstLetter;

    // remove all zeros
    s = removeZeros(s);
    // make the code exactly length 4
    s = normalizeLength(s);

    return s;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name 
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames
    
    map<string, Vector<string>> codesToNames;
    for (string name: databaseNames) {
        string code = soundex(name);
        codesToNames[code].add(name);
    }

    /* TODO: Fill in the remainder of this function. */
    while (true) {
        string query = getLine("Enter a surname (RETURN to quit): ");
        if (query == "") {
            break;
        }
        string code = soundex(query);

        //iterate over the database, compute Soundex of each name, and gather a vector of those surnames with a matching code.
        Vector<string> vec = codesToNames[code];
        vec.sort();
        cout << "Soundex code is " << code << endl;
        cout << "Matches from database: " <<endl;
        cout << vec << endl;

    }
    cout << "All done!" << endl;
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Master"), "M236");
    EXPECT_EQUAL(soundex("Jue"), "J000");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("Test removeNonLetters: special characters, empty strings, and edge cases") {
    EXPECT_EQUAL(removeNonLetters("@#!%^&*"), "");
    EXPECT_EQUAL(removeNonLetters(""), "");
    EXPECT_EQUAL(removeNonLetters("@!%^*"), "");
    EXPECT_EQUAL(removeNonLetters("348383"), "");
}

STUDENT_TEST("Test lowercaseString: ordinary cases, empty string") {
    EXPECT_EQUAL(lowercaseString("Ashcroft"), "ashcroft");
    EXPECT_EQUAL(lowercaseString("ashcroft"), "ashcroft");
    EXPECT_EQUAL(lowercaseString(""), "");
}

STUDENT_TEST("Test character encoding: ordinary cases, spaces, repeats") {
    EXPECT_EQUAL(encodeCharacters("Johns"), "20052");
    EXPECT_EQUAL(encodeCharacters("Wharton"), "0006305");
    EXPECT_EQUAL(encodeCharacters("Charlemagne"), "20064050250");
    EXPECT_EQUAL(encodeCharacters("von Trapp"), "10536011");
    EXPECT_EQUAL(encodeCharacters("Bowman"), "100505");
    EXPECT_EQUAL(encodeCharacters("Schwartz"), "22000632");
}

STUDENT_TEST("Test removeZeros: normal, all zeros, some spaces") {
    EXPECT_EQUAL(removeZeros(encodeCharacters("Bowman")), "155");
    EXPECT_EQUAL(removeZeros("00000"), "");
    EXPECT_EQUAL(removeZeros(encodeCharacters("von Trapp")), "153611");
}

STUDENT_TEST("Test coalesceDigits: normal, repeated but not adjacent, long, single character") {
    EXPECT_EQUAL(coalesceDigits("J0052"), "J052");
    EXPECT_EQUAL(coalesceDigits("1010101"), "1010101");
    EXPECT_EQUAL(coalesceDigits("12232433558859"), "1232435859");
    EXPECT_EQUAL(coalesceDigits("1"), "1");
}

STUDENT_TEST("Test normalizeLength: Test padding, truncating, and exactly 4") {
    EXPECT_EQUAL(normalizeLength(removeZeros(encodeCharacters(coalesceDigits("Bowman")))), "1550");
    EXPECT_EQUAL(normalizeLength(removeZeros(encodeCharacters(coalesceDigits("Johns")))), "2520");
    EXPECT_EQUAL(normalizeLength(removeZeros(encodeCharacters(coalesceDigits("Charlemagne")))), "2645");
    EXPECT_EQUAL(normalizeLength("B524"), "B524");
}

STUDENT_TEST("soundex: test oddball names, lots of vowels, long -- illustrating Q13") {
    EXPECT_EQUAL(soundex("Howe"), "H000");
    EXPECT_EQUAL(soundex("Huey"), "H000");
    EXPECT_EQUAL(soundex("Alexander"), soundex("Alexander-Wilson"));
}
