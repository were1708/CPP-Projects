// Joshua Revilla
// jtrevill
// pa6

#include <cmath>
#include <iostream>
#include <string>
#include "BigInteger.h"

#define power 9
#define base  1000000000

// EXTREMELY IMPORTANT FUNCTION-------------------
void clearZeros(List &L) {
    L.moveFront();
    int64_t x;
    while (L.position() != L.length()) {
        x = L.moveNext();
        if (x == 0) {
            L.eraseBefore();
        } else {
            break; // we only wanna clear the front zeros!
        }
    }
    return;
}

// Class Constructors and Destructors--------------------------

BigInteger::BigInteger() {
    signum = 0;
    List digits;
}

BigInteger::BigInteger(std::string s) {
    BigInteger big;
    signum = 0;
    if (s[0] == '+') {
        signum = 1;
        s.erase(0, 1); // erase the first char
    }
    if (s[0] == '-') {
        signum = (-1);
        s.erase(0, 1); // erase the first char
    }
    if (s.length() == 0) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }

    int pos = s.length();
    if (pos <= power) { // we can just use one number here
        int element = stol(s);
        digits.insertAfter(element);
        pos = 0;
    }

    while (pos >= power) {
        std::string sub = s.substr(pos - power, power);
        for (unsigned long i = 0; i < sub.length(); i++) {
            if (isdigit(sub[i]) == 0) {
                throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
            }
        }
        int loopElement = stol(sub);
        digits.insertAfter(loopElement);
        pos -= power;
    }
    if (pos > 0) {
        std::string lastSub = s.substr(0, pos);
        int lastLoopElement = stol(lastSub);
        digits.insertAfter(lastLoopElement);
        pos = 0;
    }
    clearZeros(digits);
    if (digits.length() != 0 && signum == 0) {
        signum = 1;
    }
}

BigInteger::BigInteger(const BigInteger &N) {
    signum = N.signum;
    digits = N.digits;
}

// Access Functions-------------------

int BigInteger::sign() const {
    return signum;
}

int BigInteger::compare(const BigInteger &N) const {
    if (this->digits.length() > N.digits.length()) {
        return 1;
    } else if (this->digits.length() < N.digits.length()) {
        return -1;
    }
    if (signum > N.signum) {
        return 1;
    }
    else if (signum < N.signum) {
        return -1;
    }
    List checkList = digits;
    List checkListN = N.digits;
    checkList.moveFront();
    checkListN.moveFront();
    while (checkList.position() != checkList.length()) { // we know the lists are the same length
        int64_t x = checkList.moveNext();
        int64_t y = checkListN.moveNext();
        if (x > y) {
            return 1;
        } else if (y > x) {
            return -1;
        }
    }
    return 0;
}

// Manipulation Functions -------------------------

void BigInteger::makeZero() {
    digits.clear();
    signum = 0;
}

void BigInteger::negate() {
    if (signum == 1) {
        signum = (-1);
    } else if (signum == -1) {
        signum = 1;
    }
}

// BigInteger Arithmetic operations ----------------------------------------

// HELPER FUNCTIONS FOR ARITHEMETIC OPERATIONS-----------------------

void negateList(List &L) {
    L.moveFront();
    int64_t x;
    while (L.position() != L.length()) {
        x = L.moveNext();
        L.setBefore(-x);
    }
    return;
}

void sumList(List &S, List A, List B, int sgn) {
    A.moveBack();
    B.moveBack();
    S.clear();
    int64_t x, y;
    while (A.position() != 0 && B.position() != 0) {
        x = A.movePrev();
        y = B.movePrev();
        S.insertAfter(x + y * sgn);
    }
    while (A.position() != 0) {
        x = A.movePrev();
        S.insertAfter(x);
    }
    while (B.position() != 0) {
        y = B.movePrev();
        S.insertAfter(y * sgn);
    }
    return;
}

void scalarMultList(List &L, ListElement m) {
    L.moveFront();
    int64_t x;
    while (L.position() != L.length()) {
        x = L.moveNext();
        L.setBefore(x * m);
    }
    return;
}

void shiftList(List &L, int p) {
    L.moveBack();
    for (int i = 0; i < p; i++) {
        L.insertBefore(0);
    }
    return;
}

int normalizeList(List &L) {
    if (L.length() == 0) { // check for empty list
        return 0;
    }
    int64_t carry = 0;
    L.moveBack();
    int64_t x;
    while (L.position() != 1) {
        x = L.movePrev();
        int64_t newValue = x + carry;
        carry = 0;
        if (newValue < 0) {
            carry = newValue / base;
            carry -= 1;
            newValue += base * -carry;
        }
        if (newValue >= base) {
            carry = newValue / base;
            newValue -= base * carry;
        }
        L.setAfter(newValue);
    }
    int64_t lastValue = L.peekPrev() + carry;
    if (lastValue < 0) {
        L.setBefore(lastValue); // change the first value, we're about to negate it!
        negateList(L); // after this normalize again!
        carry = 0;
        normalizeList(L);
        return -1;
    } else if (lastValue >= base) {
        carry = lastValue / base;
        lastValue -= base * carry;
        L.setBefore(lastValue);
        L.moveFront();
        L.insertAfter(carry);
        return 1;
    }
    L.setBefore(lastValue);
    L.movePrev(); // we're now at the front;
    return 1;
}

// END OF HELPER FUNCTIONS------------------

BigInteger BigInteger::add(const BigInteger &N) const {
    BigInteger returnValue;
    List list1 = this->digits;
    List list2 = N.digits;
    if (signum == -1) {
        negateList(list1);
    } 
        sumList(returnValue.digits, list1, list2, N.signum);
        clearZeros(returnValue.digits);
        returnValue.signum = normalizeList(returnValue.digits);
        return returnValue;
    
}

BigInteger BigInteger::sub(const BigInteger &N) const {
    BigInteger returnValue;
    List list1 = this->digits;
    List list2 = N.digits;
    if (signum == -1) {
        negateList(list1);
    }
        sumList(returnValue.digits, list1, list2, -(N.signum));
        clearZeros(returnValue.digits);
        returnValue.signum = normalizeList(returnValue.digits);
        return returnValue;
}

BigInteger BigInteger::mult(const BigInteger &N) const {
    BigInteger returnValue;
    List list2 = N.digits;
    list2.moveBack();
    List copyList, copyFinal;
    int shift = 0;
    int64_t x;
    while (list2.position() != 0) {
        x = list2.movePrev();
        copyList = digits;
        copyFinal = returnValue.digits;
        scalarMultList(copyList, x);
        shiftList(copyList, shift);
        sumList(returnValue.digits, copyList, copyFinal, 1);
        normalizeList(returnValue.digits);
        shift++;
    }
    returnValue.signum = signum * N.signum; // set the sign
    clearZeros(returnValue.digits);
    return returnValue;
}

// Other Functions -------------------------------------------

std::string BigInteger::to_string() {
    digits.moveFront();
    if (digits.length() == 0) {
        return "0";
    }
    std::string returnString = "";
    if (signum == -1) {
        returnString += "-";
    }
    std::string addString;
    addString = std::to_string(digits.moveNext()); // we don't need to do anything to first entry!
    returnString += addString;
    int64_t element;
    while (digits.position() != digits.length()) { // while we're not at the end of the list
        element = digits.peekNext();
        addString = std::to_string(element);
        addString.insert(0, power - (addString.length()), '0');
        returnString += addString;
        digits.moveNext();
    }
    return returnString;
}

// Overriden Operators -----------------------------------------------------

std::ostream &operator<<(std::ostream &stream, BigInteger N) {
    return stream << N.BigInteger::to_string();
}

bool operator==(const BigInteger &A, const BigInteger &B) {
    if (A.compare(B) == 0) {
        return true;
    }
    return false;
}

bool operator<(const BigInteger &A, const BigInteger &B) {
    if (A.compare(B) < 0) {
        return true;
    }
    return false;
}

bool operator<=(const BigInteger &A, const BigInteger &B) {
    if (A.compare(B) <= 0) {
        return true;
    }
    return false;
}

bool operator>(const BigInteger &A, const BigInteger &B) {
    if (A.compare(B) > 0) {
        return true;
    }
    return false;
}

bool operator>=(const BigInteger &A, const BigInteger &B) {
    if (A.compare(B) >= 0) {
        return true;
    }
    return false;
}

BigInteger operator+(const BigInteger &A, const BigInteger &B) {
    return A.add(B);
}

BigInteger operator+=(BigInteger &A, const BigInteger &B) {
    BigInteger temp = A.add(B);
    std::swap(A.signum, temp.signum);
    A.digits = temp.digits;
    return A;
}

BigInteger operator-(const BigInteger &A, const BigInteger &B) {
    return A.sub(B);
}

BigInteger operator-=(BigInteger &A, const BigInteger &B) {
    BigInteger temp = A.sub(B);
    std::swap(A.signum, temp.signum);
    A.digits = temp.digits;
    return A;
}

BigInteger operator*(const BigInteger &A, const BigInteger &B) {
    return A.mult(B);
}

BigInteger operator*=(BigInteger &A, const BigInteger &B) {
    BigInteger temp = A.mult(B);
    std::swap(A.signum, temp.signum);
    A.digits = temp.digits;
    return A;
}
