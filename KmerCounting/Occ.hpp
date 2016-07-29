//
//  Occ.hpp
//  KmerCounting
//
//  Created by Rishil on 6/6/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#ifndef Occ_hpp
#define Occ_hpp

#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

//Naive occ:

struct Occ {
    
public:
    void build(string & B, int granularity = 1);
    
    friend ostream& operator<<(ostream &os, const Occ& occ);
    
    //getter for $
    
    long long return_$() {
        return $_pos;
    }
    
    long long get(char base, long long index);
    
    //*C* array (PROBABLY SHOULD NOT BE PUBLIC)
    
    long long c_array[6];
    
    //these will store *occ* values for each letter
    
    vector<long long> A;
    vector<long long> C;
    vector<long long> G;
    vector<long long> T;
    vector<long long> N;

    long long $_pos; //refers to the position in B where the dollar sign appears
  
    
    //the BWT, just in case we need it
    
    string bwt;
    
private:
    
    //Update vector function
    
    void updateVectorWith(long long A_count, long long C_count, long long G_count, long long T_count, long long N_count);
    
    
    
};



#endif /* Occ_hpp */
