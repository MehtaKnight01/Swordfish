//
//  Node.cpp
//  KmerCounting
//
//  Created by Rishil on 6/21/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#include "Node.hpp"
#include <vector>
#include <string>

int Node::getNumberOfChildren(std::string & reverseBWT, long long $_pos) {
    //NOTE: I opted against using reverse Occ to do this computation. I think this may increase the computational time a little, but halves the space requirement, and lowers the complexity of the code.
    
    //This code iterates through the reverse BWT at the interval specified by the reverseInterval and determines how many of the 4 characters are present.
            //After a lot of thought, I think that the code will run at most 5 times per interval (the only children are A,C,T,G and $). It may be valuable to pre-determine the location of the '$' in the reverseBWT-- doing so may save me up to 20% time running this code, although I think my math is wrong and it will not save me that much time at all.
    
    //An alternate route would be to maintain a reverse Occ table, and call rprev for each of the four characters using the reverseInterval and reverseOcc
    
    std::vector<int> characters = {0,0,0,0,0};
    
    unsigned long long start = static_cast<unsigned long long>(reverseInterval[0] - 1);
    unsigned long long end = static_cast<unsigned long long>(reverseInterval[1] - 1);
    
    for (size_t i = start; i <= end; i++) {
        char rBWTChar = reverseBWT[i];
        switch(rBWTChar) {
            case 'A' : characters[0] = 1;
                break;
            case 'C' : characters[1] = 1;
                break;
            case 'G' : characters[2] = 1;
                break;
            case 'T' : characters[3] = 1;
                break;
            case 'N' : //TODO: add support for N
                break;
            case '$' : characters[4] = 1;
                break;
        }
        
    }
    
    this->numberOfChildren = characters[0] + characters[1] + characters[2] + characters[3] + characters[4];
    return numberOfChildren;
}


bool Node::isRightMaximal() {
    
    if(numberOfChildren >= 2) {
        return true;
    }
    return false;
}