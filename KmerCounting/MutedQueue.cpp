//
//  MutedQueue.cpp
//  KmerCounting
//
//  Created by Rishil on 8/9/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#include "MutedQueue.hpp"
#include <mutex>

void MutedQueue::loadQueue(std::queue<Node> initialQueue) {
    MutedQueue::nodeQueue = initialQueue;
}

bool MutedQueue::loadIntoInternalNode(Node &internalNode) {
    
    std::lock_guard<std::mutex> guard(MutedQueue::queueMutex);
    if(!MutedQueue::nodeQueue.empty()) {
        internalNode = MutedQueue::nodeQueue.front();
        MutedQueue::nodeQueue.pop();
        return true;
    }
    
    return false;
}