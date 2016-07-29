//
//  Helper.hpp
//  KmerCounting
//
//  Created by Rishil on 7/8/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#ifndef Helper_hpp
#define Helper_hpp

#include <stdio.h>
#include <vector>
#include "Occ.hpp"


void processNode();
std::vector<long long> getIntervalBasedOn(std::vector<long long> interval, char character, Occ & occ);
std::vector<long long> getReverseIntervalBasedOn(std::vector<long long> interval, std::vector<long long> reverseInterval, char character, string & reverseBWT, Occ & occ);
string prepend(char charToPrepend, string pattern);
std::vector<long long> deltaOcc(std::vector<long long> interval, Occ & occ);
    
#endif /* Helper_hpp */
