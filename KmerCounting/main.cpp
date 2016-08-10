//
//  main.cpp
//  Kmer
//
//  Created by Rishil on 6/2/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//



//NOTE: THE PROGRAM USES THE DNA BASES IN THE FOLLOWING ORDER: A C G T N $

#include <iostream>
#include <istream>
#include <string>
#include <fstream>
#include <ostream>
#include "Occ.hpp"
#include "Node.hpp"
#include <vector>
#include <stack>
#include "Helper.hpp"
#include "Usage.hpp"
#include <thread>
#include <queue>
#include <numeric>
#include "MutedQueue.hpp"


using namespace std;



//"Global" variable to store the kmer counts:
long long kmerCounts;


long long k;
string BWT;
string reverseBWT;
Occ occ;
unsigned int nThreads;

MutedQueue threadSafeQueue;

queue<Node> bfs(Occ & occ) {
    
    queue<Node> nodeQueue;
    
    //Implement BFS until queue just exceeds number of available threads (very similar to DFS so no comments will be provided
    
    vector<long long>interval = {1, static_cast<long long>(BWT.length())};
    auto reverseInterval = interval;
    long long depth = 0;
    
    Node internalNode;
    internalNode.interval = interval;
    internalNode.reverseInterval = reverseInterval;
    internalNode.depth = depth;
    
    nodeQueue.push(internalNode);
    
    while ((nodeQueue.size() < nThreads) && !nodeQueue.empty()) {
        internalNode = nodeQueue.front();
        nodeQueue.pop();
        
        if(internalNode.depth >= k) {
            kmerCounts += (1-internalNode.numberOfChildren);
        }
        
        vector<long long> dOcc = deltaOcc(internalNode.interval, occ);
        
        char prependedCharacter;
        Node possibleNode;
        if(dOcc[0] > 1) {
            prependedCharacter = 'A';
            
            possibleNode.reverseInterval = getReverseIntervalBasedOn(internalNode.interval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
            if(possibleNode.isRightMaximal()) {
                
                
                possibleNode.depth = internalNode.depth + 1;
                nodeQueue.push(possibleNode);
                
            }
        }
        
        if (dOcc[1] > 1) {
            prependedCharacter = 'C';
            
            possibleNode.reverseInterval = getReverseIntervalBasedOn(internalNode.interval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
            if(possibleNode.isRightMaximal()) {
                possibleNode.depth = internalNode.depth + 1;
                
                nodeQueue.push(possibleNode);
                
            }
        }
        
        if (dOcc[2] > 1) {
            prependedCharacter = 'G';
            
            possibleNode.reverseInterval = getReverseIntervalBasedOn(internalNode.interval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
            if(possibleNode.isRightMaximal()) {
                
                possibleNode.depth = internalNode.depth + 1;
                
                nodeQueue.push(possibleNode);
                
            }
        }
        
        if (dOcc[3] > 1) {
            prependedCharacter = 'T';
            
            possibleNode.reverseInterval = getReverseIntervalBasedOn(internalNode.interval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
            if(possibleNode.isRightMaximal()) {
                
                possibleNode.depth = internalNode.depth + 1;
                
                nodeQueue.push(possibleNode);
                
            }
        }
    
    }
    return nodeQueue;
}



void countKmers(Occ & occ, vector<long long> & results, int location) {
    
    //"counts" will temporarily store kmer counts
    long long counts = results[location];

    //TODO: Mutex around this:
    
    Node internalNode;
    
    while(threadSafeQueue.loadIntoInternalNode(internalNode)) {
        
        stack<Node> nodeStack;
        
        nodeStack.push(internalNode);
        
        //Stack traversal:
        
        while(!nodeStack.empty()) {
            
            //Available variables: interval, reverseInterval, depth, pattern (for debugging)
            
            //Load the top stack item and remove that item from the stack
            internalNode = nodeStack.top();
            nodeStack.pop();
            
            //Process node if depth >= k
            if (internalNode.depth >= k) {
                counts += (1 - internalNode.numberOfChildren);
            }
            
            //Use Delta OCC to determine which character(s) to prepend to check
            vector<long long> dOcc = deltaOcc(internalNode.interval, occ);
            
            
            char prependedCharacter;
            Node possibleNode;
            if(dOcc[0] > 1) {
                prependedCharacter = 'A';
                
                //Save previous interval for use in determining reverse interval
                vector<long long> previousInterval = internalNode.interval;
                
                //Determine interval
                possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
                
                //Determine reverse interval (note: not sure if I can do this step later or not
                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                
                //Determine if right maximal, and if so, obtain reverse interval, incremement depth and then PUSH
                possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
                
                if(possibleNode.isRightMaximal()) {
                    
                    
                    possibleNode.depth = internalNode.depth + 1;
                    nodeStack.push(possibleNode);
                    
                }
            }
            
            if (dOcc[1] > 1) {
                prependedCharacter = 'C';
                
                //Save previous interval for use in determining reverse interval
                vector<long long> previousInterval = internalNode.interval;
                
                
                //Determine interval
                possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
                
                //Determine reverse interval (note: not sure if I can do this step later or not
                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                
             
                //Determine if right maximal, and if so, obtain reverse interval and PUSH
                possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
                
                if(possibleNode.isRightMaximal()) {
                    possibleNode.depth = internalNode.depth + 1;

                    nodeStack.push(possibleNode);
                    

                }
            }
            
            if (dOcc[2] > 1) {
                prependedCharacter = 'G';
                
                //Save previous interval for use in determining reverse interval
                vector<long long> previousInterval = internalNode.interval;
                
                //Determine interval
                possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
                
                //Determine reverse interval (note: not sure if I can do this step later or not
                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                
                //Determine if right maximal, and if so, obtain reverse interval and PUSH
                possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
                
                if(possibleNode.isRightMaximal()) {
                    
                    possibleNode.depth = internalNode.depth + 1;

                    nodeStack.push(possibleNode);
                    
                }
            }
            
            if (dOcc[3] > 1) {
                prependedCharacter = 'T';
                
                //Save previous interval for use in determining reverse interval
                vector<long long> previousInterval = internalNode.interval;
                
                
                //Determine interval
                possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
                
                //Determine reverse interval (note: not sure if I can do this step later or not
                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                
                //Determine if right maximal, and if so, obtain reverse interval and PUSH
                possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
                
                if(possibleNode.isRightMaximal()) {
                    
                    possibleNode.depth = internalNode.depth + 1;

                    nodeStack.push(possibleNode);
                    
                }
            }
            
            
        }
    }
    
    //load kmer counts into results vector
    results[location] = counts;
}


void threadedCount(Occ & occ) {
    threadSafeQueue.loadQueue(bfs(occ));
    vector<std::thread> threads;
    vector<long long> results;
    for (size_t i = 0; i < nThreads - 1; i++) {//-1 so that main can also count as a thread
        results.push_back(0);
        threads.push_back(std::thread(countKmers, std::ref(occ), std::ref(results), i));
    }
    
    //Count k-mers using main thread as well
    results.push_back(0);
    countKmers(occ, results, nThreads - 1);
    
    for (size_t i = 0; i < nThreads - 1; i++) {
        if(threads[i].joinable()) {
            threads[i].join();
        } else {
            std::cerr << "Error, thread was not joinable" << endl;
        }
    }
    
    kmerCounts += std::accumulate(results.begin(), results.end(), 0);
    
    
}






int main(int argc, const char * argv[]) {
    
    
    //Proper input style: <command>, <kmer size>, <BWT file>, <reverse BWT file>
    
    string command = argv[0];
    
    
    //If 4 arguments were not supplied:
    
    if (argc != 5) {
        return usage(command);
    }
    

    
    //If the user called "count", run the program if possible
    
    if (argv[1] != string("count")) {
        return usage(command);
    } else {

        
        
        ifstream ifs;
        
        
        
        //Retrieve kmer size
        ifs.open(argv[2]);
        
        if (ifs.is_open() == false) {
            cerr << "ERROR, the kmer size could not be understood" << endl;
            ifs.close();
            return usage(command);
        } else {
        
            while (!ifs.eof()) {
                ifs >> k;
            }
            
        }
        ifs.close();
        
        
        
        
        
        //Retrieve forward BWT
        ifs.open(argv[3]);
        
        if (ifs.is_open() == false) {
            cerr << "ERROR, the input file could not be read" << endl;
            ifs.close();
            return usage(command);
        } else {
        
            while (!ifs.eof())
            {
                ifs >> BWT;
            }
            
        }
        ifs.close();

        
        
        
        
        
        //Retrieve reverse BWT
        ifs.open(argv[4]);
        
        if (ifs.is_open() == false) {
            cerr << "ERROR, the reverse BWT could not be read" << endl;
            ifs.close();
            return usage(command);
        } else {
            
            while (!ifs.eof()) {
                ifs >> reverseBWT;
            }
            ifs.close();

        }
        
        
        
        
        
        //Make sure BWT and reverseBWT are the same length
        if(BWT.size() != reverseBWT.size()) {
            cerr << "BWT's are different sizes." << endl;
            return usage(command);
        }
        
        
        
        
        
        //Build the occ table using the BWT
        occ.build(BWT, 1);
     
        
        
        
//        cout << "Count called with k-mer size " << k << endl;

        
        
        //Initialize the K-mer counts using the BWT
        const long long NUMBER_OF_RELEVANT_LEAF_NODES = BWT.size() - (k - 1);
        const long long OFFSET_DUE_TO_$ = 1;
        
        kmerCounts = NUMBER_OF_RELEVANT_LEAF_NODES - OFFSET_DUE_TO_$; //the final kmerCount will also include a value representing the sum over all the internal nodes of (1 - # of children at each node)
        
        //Determine number of available threads
        nThreads = std::thread::hardware_concurrency();
        
        
        
        //K-mer counting:
        threadedCount(occ);
            cout << "The result of the kmer counting program is that there are:" << endl;
            cout << kmerCounts << endl;
            cout << "unique kmers in the sequence string" << endl;
        
    }
    return 0;
    //eventually we will also allow people to input their own C array and occ table, but not now.
}












