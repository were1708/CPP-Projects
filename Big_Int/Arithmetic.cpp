// Joshua Revilla
// jtrevill
// pa6
#include <iostream>
#include <fstream>
#include <string>
#include "BigInteger.h"
using namespace std;

int main(int argc, char *argv[]) {
    ifstream in;
    ofstream out;

    // check command line for correct number of arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return (EXIT_FAILURE);
    }

    // open files for reading and writing
    in.open(argv[1]);
    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return (EXIT_FAILURE);
    }

    out.open(argv[2]);
    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return (EXIT_FAILURE);
    }

    string int1, int2, empty;
    getline(in, int1);
    getline(in, empty);
    getline(in, int2); 
    
    BigInteger A = BigInteger(int1);
    BigInteger B = BigInteger(int2);
    BigInteger three = BigInteger("3");
    BigInteger two = BigInteger("2");
    BigInteger nine = BigInteger("9");
    BigInteger teen = BigInteger("16");
    
    out << A << endl;
    out << endl;
    out << B << endl;
    out << endl;
    BigInteger C = A + B;
    out << C << endl;
    out << endl;
    BigInteger D = A - B;
    out << D << endl;
    out << endl;
    BigInteger E = A - A;
    out << A - A << endl;
    out << endl;
    BigInteger temp1 = A * three;
    BigInteger temp2 = B * two;
    BigInteger F = temp1 - temp2;
    out << F << endl;
    out << endl;
    BigInteger G = A * B;
    out << G << endl;
    out << endl;
    BigInteger H = A * A;
    out << H << endl;
    out << endl;
    BigInteger I = B * B;
    out << I << endl;
    out << endl;
    BigInteger temp3 = A;
    BigInteger temp4 = B;
    for (int i = 0; i < 3; i++) {
        temp3 *= A;
    }
    for (int i = 0; i < 4; i++) {
        temp4 *= B;
    }
    BigInteger temp5 = nine * temp3;
    BigInteger temp6 = teen * temp4;
    BigInteger J = temp5 + temp6;
    out << J << endl;
    out << endl;
    // close files
    in.close();
    out.close();
    return 0;
}
