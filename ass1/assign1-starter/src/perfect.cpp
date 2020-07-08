// Program to compute perfect numbers

#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "timer.h"
using namespace std;

/* This function takes in a number n and calculates the sum
 * of all proper divisors of n, excluding itself.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function is provided a number n and returns a boolean
 * (true/false) value representing whether or not the number is
 * perfect. A perfect number is a non-zero positive number whose
 * sum of its proper divisors is equal to itself.
 */
bool isPerfect (long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function performs an exhaustive search for perfect numbers.
 * It takes as input a number called `stop` and searches for perfect
 * numbers between 1 and `stop`. Any perfect numbers that are found will
 * be printed out to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush;
    }
    cout << "Done searching up to " << stop << endl;
}

/* Finds the sum of an integer's divisors by checking
 * only those divisors up to its square root.
 */
long smarterSum(long n){
    long total = 0;
    for (long i = sqrt(n); i > 0; i--) {
        if (n == 1) break;
        else if (i == 1) {
            total += i;
        } else if (i*i == n) {
            total += i;
        } else if (n % i == 0) {
            total += i + n/i;
        }
    }
    return total;
}

/* Uses the smarterSum function to more efficiently
 * search for numbers that equal the sum of their divisors.
 * input: integer n; output: boolean
 */
bool isPerfectSmarter(long n){
    return (n != 0) && (n == smarterSum(n));
}

/* Uses the isPerfectSmarter function to more efficiently
 * search for numbers that equal the sum of their divisors.
 * input: integer stop, output: printout of perfect numbers
 * found
 */
void findPerfectsSmarter(long stop){

    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number:" << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush;
        }
    }
    cout << "Done searching up to " << stop << endl;
}

/* Helper function that returns true if the input is a prime
 * number and false otherwise.
 * input: long int
 * output: bool
 */

bool isPrime(long num) {

    bool isPrime = true;
    for (int i=2; i <= num/2; i++) {
        if (num % i == 0) {
            isPrime = false;
            break;
        }
    }
    return (num > 0) && (num != 1) && isPrime;
}

/* Function that uses the Euclid-Euler theorem
 * to find the nth perfect number.
 * input: long int n representing the ordinal position
 * of the desired perfect number
 * output: long int = the nth perfect number
 */

long findNthPerfectEuclid(long n){
    long k = 1;
    long m;
    long perfect;
    long i = 1;

    while (k <= n) {
        m =  pow(2, i) - 1;
        if (isPrime(m)) {
            perfect = pow(2, i-1) * (pow(2, i) - 1);
            k++;
        }
        i++;
    }
    return perfect;
}

void findPerfectsEuclid(long stop){

    for (long num = 1; num <= stop; num++) {
        long perfect = findNthPerfectEuclid(num);

        cout << "The #" << num << " perfect number is:" << perfect << endl;
    }
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Time multiple trials of findPerfects function on doubling input sizes."){
    cout << endl;
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
    TIME_OPERATION(320000, findPerfects(320000));
    TIME_OPERATION(640000, findPerfects(640000));
}

PROVIDED_TEST("Check divisor sums of small numbers") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Check 6 and 28, should be perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Imperfect numbers") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

PROVIDED_TEST("33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

STUDENT_TEST("Check that negative input returns false") {
    EXPECT(!isPerfect(-6));
    EXPECT(!isPerfect(-28));
}

// STUDENT_TEST("Bogus test") {
//     EXPECT(isPerfect(7));
// }

STUDENT_TEST("Check that smarterSum output matches divisorSum output") {
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
    EXPECT_EQUAL(divisorSum(28), smarterSum(28));
    EXPECT_EQUAL(divisorSum(0), smarterSum(0));
    EXPECT_EQUAL(divisorSum(1), smarterSum(1));
    EXPECT_EQUAL(divisorSum(-1), smarterSum(-1));
    EXPECT_EQUAL(divisorSum(98765), smarterSum(98765));
}


STUDENT_TEST("Time multiple trials of findPerfectsSmarter function on doubling input sizes."){
    cout << endl;
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(160000, findPerfectsSmarter(160000));
    TIME_OPERATION(320000, findPerfectsSmarter(320000));
    TIME_OPERATION(640000, findPerfectsSmarter(640000));
    TIME_OPERATION(33550336, findPerfectsSmarter(33550336));
}

STUDENT_TEST("Test isPrime helper function") {
    EXPECT(isPrime(3));
    EXPECT(isPrime(13));
    EXPECT(isPrime(31));
    EXPECT(!isPrime(0));
    EXPECT(!isPrime(18));
    EXPECT(isPrime(2));
    EXPECT(!isPrime(-5));
    EXPECT(!isPrime(1));
}

STUDENT_TEST("Test findNthPerfectEuclid: Outputs are perfect numbers") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(1)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(3)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(4)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(5)));
}


STUDENT_TEST("Test findNthPerfectEuclid: Outputs are correct perfect numbers") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}

STUDENT_TEST("Time Euclid-Euler algorithm") {
    TIME_OPERATION(5, findPerfectsEuclid(5));
}
