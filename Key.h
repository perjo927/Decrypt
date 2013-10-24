#ifndef KEY_H
#define KEY_H

#include <iostream>

#define C 8 	    // number of characters in password
#define B 5         // number of bits per character
#define R (1 << B)  // size of alphabet (32)
#define N (B * C)   // number of bits per password

#define ALPHABET "abcdefghijklmnopqrstuvwxyz012345"

using namespace std;

/****************************************************************
 * An extended precision base R integer consisting of C digits.
 * An array packaged in a struct for easy memory management and
 * pass-by-value.
 ****************************************************************/
typedef struct {  
unsigned char digit[C];
} Key;

/****************************************************************
 * Initialize k from a character string.
 * Example: s = "abcdwxyz"  =>  k = 0 1 2 3 22 23 24 25
 ****************************************************************/
Key  KEYinit(unsigned char s[]);

/****************************************************************
 * Add and return the subset of the integers T[i] that are
 * indexed by the bits of k. Do sum mod 2^N.
 ****************************************************************/
Key  KEYsubsetsum(const Key& k, const Key T[N]);

int  KEYbit(const Key& k, int i);        // return the ith bit of k

bool operator==(const Key&, const Key&);
bool operator!=(const Key&, const Key&);
bool operator<(const Key&, const Key&);
bool operator>(const Key&, const Key&);
bool operator<=(const Key&, const Key&);
bool operator>=(const Key&, const Key&);
Key  operator+(const Key&, const Key&); // return a + b mod 2^N
Key  operator++(Key&, int);            // postfix increment
Key&  operator++(Key&);            // prefix increment
Key  operator-(const Key&, const Key&); // return a - b mod 2^N
ostream& operator<<(ostream&, const Key&);

#endif
