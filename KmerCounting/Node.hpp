//
//  Node.hpp
//  KmerCounting
//
//  Created by Rishil on 6/21/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>

struct Node {
    std::vector<long long> interval;
    std::vector<long long>reverseInterval;
    long long depth;
//    std::string pattern;
    int numberOfChildren;
    int getNumberOfChildren(std::string & reverseBWT, long long $_pos);
    bool isRightMaximal();
    void processNode(std::vector<long long> & dOcc, std::stack<Node> & nodeStack);
};

#endif /* Node_hpp */
