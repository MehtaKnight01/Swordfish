//
//  MutedQueue.hpp
//  KmerCounting
//
//  Created by Rishil on 8/9/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#ifndef MutedQueue_hpp
#define MutedQueue_hpp

#include <stdio.h>
#include <queue>
#include "Node.hpp"
#include <mutex>

//Implements a thread-safe version of nodeQueue for this program

class MutedQueue {
    
public:
    void loadQueue(std::queue<Node> initialQueue);
    bool loadIntoInternalNode(Node &internalNode);


private:
    std::queue<Node> nodeQueue;
    std::mutex queueMutex;
    
};

#endif /* MutedQueue_hpp */
