//
//  Occ.cpp
//  KmerCounting
//
//  Created by Rishil on 6/6/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#include "Occ.hpp"
#include <iostream>
using namespace std;




void Occ::build(string & B, int granularity) {
    //assuming I don't need the reverse BWT. If I do, I'll have to re-package everything into functions.
    Occ::bwt = B;
    
    long long A_count = 0;
    long long C_count = 0;
    long long G_count = 0;
    long long T_count = 0;
//    long long N_count = 0;
    
    char letter;
    
    //The first number for all the vectors will be zero, and then we will move from there.
    updateVectorWith(0, 0, 0, 0);
    
    
    for (size_t i = 0; i < B.length(); i++) {
        //cout << B[i] << endl;
        //cout << "Got here" << endl;
        
        letter = B[i];
        
        //update vectors based on whether the letter was found or not
        
        if (letter == 'A' || letter == 'a') {
            
            A_count++;
            
        } else if (letter == 'C' || letter == 'c') {
            
            C_count++;
            
        } else if (letter == 'G' || letter == 'g') {
            
            G_count++;
    
        } else if (letter == 'T' || letter == 't') {
            
            T_count++;
            
        } else if (letter == 'N' || letter == 'n') {
            cerr << "N detected" << endl;
//            N_count++;
            
        } else if (letter == '$') {
            $_pos = i + 1; //treat $ separately -- mark its position and then don't increment other vectors
            //the plus one is a result of the fact that we offset the entire occ table by 1
        }
        else { //catchall error if the letter is not ACGT$
            cout << "Error, unidentified character found" << endl;
        }
            
        
        
        if((i % granularity == 0)) {
            updateVectorWith(A_count, C_count, G_count, T_count);
        }
        
       
    }
    
    
    //Now that occ is built, quickly build C.
    
    c_array[0] = 0; //                              $
    c_array[1] = 1; //                              A
    c_array[2] = 1 + A_count; //                   C
    c_array[3] = c_array[2] + C_count; //          G
    c_array[4] = c_array[3] + G_count; //          T
    c_array[5] = c_array[4] + T_count; //          N
    
    
}


void Occ::updateVectorWith(long long A_count, long long C_count, long long G_count, long long T_count) {
    A.push_back(A_count);
    C.push_back(C_count);
    G.push_back(G_count);
    T.push_back(T_count);
//    N.push_back(N_count);
}


//DEBUG printout:: not for use on large data sets
ostream& operator<<(ostream& os, const Occ& occ) {
    
    //doesn't work
//    for (long long i = 0; i < occ.bwt.length(); i++) {
//        os << i+1 << " ";
//    }
//    
//    os << endl;
    
    os << occ.bwt << endl << endl;
    
    //print out a messy version of the occ table
    
    os << "A: ";
    
    for (size_t i = 0; i < occ.A.size(); i++) {
        os << occ.A[i] << " ";
    }
    
    os << endl;
    os << "C: ";

    for (size_t i = 0; i < occ.C.size(); i++) {
        os << occ.C[i] << " ";
    }
    
    os << endl;
    os << "G: ";
    
    for (size_t i = 0; i < occ.G.size(); i++) {
        os << occ.G[i] << " ";
    }
    
    os << endl;
    os << "T: ";
    
    for (size_t i = 0; i < occ.T.size(); i++) {
        os << occ.T[i] << " ";
    }
    
    os << endl;
//    os << "N: ";
//    
//    for (size_t i = 0; i < occ.N.size(); i++) {
//        os << occ.N[i] << " ";
//    }
    
    os << endl << endl;
    
    //print out the C array
    
    os << "C array: " << endl << "$: " << occ.c_array[0] << endl << "A: " << occ.c_array[1] << endl << "C: " << occ.c_array[2] << endl << "G: " << occ.c_array[3] << endl << "T: " << occ.c_array[4] << endl << "N: " << occ.c_array[5] << endl;
    
    return os;
}

long long Occ::get(char base, long long index) {
    
    long long numberOfOccurences;
    
    
    switch(base) {
        case 'A' : numberOfOccurences = A[index];
            break;
        case 'C' : numberOfOccurences = C[index];
            break;
        case 'G' : numberOfOccurences = G[index];
            break;
        case 'T' : numberOfOccurences = T[index];
            break;
        case 'N' : //TODO: add N as a viable character
            break;
        case '$' : numberOfOccurences = ($_pos >= index);
            break;
        default: numberOfOccurences = -1;
            cout << "ERROR: CHARACTER NOT UNDERSTOOD" << endl;
    }
    
    return numberOfOccurences;
}
