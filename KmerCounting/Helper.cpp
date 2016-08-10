//
//  Helper.cpp
//  KmerCounting
//
//  Created by Rishil on 7/8/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#include "Helper.hpp"
#include <string>
#include <iostream>



std::vector<long long> getIntervalBasedOn(std::vector<long long> interval, char character, Occ & occ) {
    
    long long cArrayCount;
    
    switch(character) {
        case 'A' : cArrayCount = occ.c_array[1];
            break;
        case 'C' : cArrayCount = occ.c_array[2];
            break;
        case 'G' : cArrayCount = occ.c_array[3];
            break;
        case 'T' : cArrayCount = occ.c_array[4];
            break;
        case 'N' : //TODO: add N as a viable character
            break;
        case '$' : cArrayCount = occ.c_array[0]; //I don't know why you'd ever ask for $ here but jic
            break;
        default:
                cArrayCount = -1;
                cout << "ERROR: CHARACTER NOT FOUND IN C ARRAY" << endl;
                return {0,0};
            break;
    }

    
    //rprev:
    long long i = cArrayCount + occ.get(character, interval[0] - 1) + 1; //'E' will break unless the start of its interval is set to 1
    long long j = cArrayCount + occ.get(character, interval[1]);

    
    
    return {i,j};
}


std::vector<long long> getReverseIntervalBasedOn(std::vector<long long> interval, std::vector<long long> reverseInterval, char character, string & reverseBWT, Occ & occ) {
    
    long long x = interval[0] - 1;
    long long y = interval[1];
    
    
    long long xPrime = reverseInterval[0];
    
    std::vector<char> characters = {'A', 'C', 'G', 'T'};
    
    long long lessThanChar;
    long long leqToChar;
    
    if (character == 'A') {
        lessThanChar = 0;
        leqToChar = occ.get('A',y) - occ.get('A',x);
    } else if (character == 'C') {
        lessThanChar = occ.get('A',y) - occ.get('A',x);
        leqToChar = lessThanChar + (occ.get('C', y) - occ.get('C',x));
    } else if (character == 'G') {
        lessThanChar = (occ.get('A',y) - occ.get('A',x)) + (occ.get('C', y) - occ.get('C',x));
        leqToChar = lessThanChar + (occ.get('G', y) - occ.get('G',x));
    } else if (character == 'T') {
        lessThanChar = (occ.get('A',y) - occ.get('A',x)) + (occ.get('C', y) - occ.get('C',x)) + (occ.get('G', y) - occ.get('G',x));
        leqToChar = lessThanChar + (occ.get('T', y) - occ.get('T', x));
    } else {
        lessThanChar = -1;
        leqToChar = -1;
        cout << "ERROR, character not found!" << endl;
        return {0,0};   
    }
    
    //adjust in case we missed $ in our delta occ computations
    if((occ.$_pos > x) && (occ.$_pos <= y)) {
        lessThanChar++;
        leqToChar++;
    }
    
    long long w = xPrime + lessThanChar;
    long long z = xPrime + leqToChar - 1; //not sure why this minus one needs to exist but I think it helps
    
    return {w,z};
    
}



string prepend(char charToPrepend, std::string pattern) {
    return pattern.insert(0, 1, charToPrepend);
}


std::vector<long long> deltaOcc(std::vector<long long> interval, Occ & occ)  {
    
    long long posOne = interval[0] - 1;
    long long posTwo = interval[1];
    
    long long A = occ.A[posTwo] - occ.A[posOne];
    long long C = occ.C[posTwo] - occ.C[posOne];
    long long G = occ.G[posTwo] - occ.G[posOne];
    long long T = occ.T[posTwo] - occ.T[posOne];
    //TODO: add N as a viable character
    
    
    return {A,C,G,T};
}