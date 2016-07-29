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

using namespace std;



//"Global" variable to store the kmer counts:
long long kmerCounts;


long long k;
string BWT;
string reverseBWT;
Occ occ;







void countKmers(Occ & occ) {
    
    stack<Node> nodeStack;
    
    
    
    //Initializing things:
    
    vector<long long>interval = {1, static_cast<long long>(BWT.length())}; //starting interval
    //note, this goes from 1 to bwt.length, and it will crash if it is set from 0 to bwt.length - 1
    
    auto reverseInterval = interval; //both the interval and the reverse interval start the same
    
    long long depth = 0;
    
    string pattern = ""; //just for debugging
    
    
    
    Node internalNode;
    
    internalNode.interval = interval;
    internalNode.reverseInterval = reverseInterval;
    internalNode.depth = depth;
    
    internalNode.pattern = pattern; //just for debugging
    
    
    
    nodeStack.push(internalNode);
    
    
    
    
    
    //Stack traversal:
    
    while(!nodeStack.empty()) {
        
        //Available variables: interval, reverseInterval, depth, pattern (for debugging)
        
        
        
        //Load the top stack item and remove that item from the stack
        internalNode = nodeStack.top();
        nodeStack.pop();
        
//        //DEBUG PRINTOUT:
//        
//        cout << "Internal node information:" << endl;
//        cout << "Interval: [" << internalNode.interval[0] << "," << internalNode.interval[1] << "]" << endl;
//        cout << "Reverse interval: [" << internalNode.reverseInterval[0] << "," << internalNode.reverseInterval[1] << "]" << endl;
//        cout << "Depth: " << internalNode.depth << endl;
//        cout << "Pattern: " << internalNode.pattern << endl << endl << endl;
//        
        
        //TODO: Process node if depth >= k
        if (internalNode.depth >= k) {
            kmerCounts += (1 - internalNode.numberOfChildren);
        
//            //DEBUG PRINTOUT:
//            
//            cout << "Node was processed. It had " << internalNode.numberOfChildren << " children." << endl << endl << endl;
        }
        
        
        
        
        //Use Delta OCC to determine which character(s) to prepend to check
        
        
        vector<long long> dOcc = deltaOcc(internalNode.interval, occ);
        
//        //DEBUG PRINTOUT:
//        cout << "Delta Occ returned the following values: " << endl;
//            cout << "A: " << dOcc[0] << endl;
//            cout << "C: " << dOcc[1] << endl;
//            cout << "G: " << dOcc[2] << endl;
//            cout << "T: " << dOcc[3] << endl << endl << endl;
        
        
        char prependedCharacter;
        Node possibleNode;
        if(dOcc[0] > 1) {
            prependedCharacter = 'A';
            possibleNode.pattern = prepend(prependedCharacter, internalNode.pattern); //just for debugging
            
            //Save previous interval for use in determining reverse interval
            vector<long long> previousInterval = internalNode.interval;
            
            //Determine interval
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            
//            //DEBUG PRINTOUT:
//            cout << "We tried appending A. The new pattern is: " << possibleNode.pattern << endl;
//            cout << "and the new interval is: [" << possibleNode.interval[0] << "," << possibleNode.interval[1] << "]" << endl;
            
            
            //Determine reverse interval (note: not sure if I can do this step later or not
            possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            
//            //MORE DEBUG PRINTOUT:
//            cout << "and the new reverse interval is: [" << possibleNode.reverseInterval[0] << "," << possibleNode.reverseInterval[1] << "]" << endl << endl;
            
            
            //Determine if right maximal, and if so, obtain reverse interval, incremement depth and then PUSH
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
//            //DEBUG PRINTOUT:
//            cout << "The number of children was: " << possibleNode.numberOfChildren << endl << endl;
//            
            
            if(possibleNode.isRightMaximal()) {
                
                
                possibleNode.depth = internalNode.depth + 1;
                nodeStack.push(possibleNode);
                
//                cout << "We pushed it because it was right maximal" << endl << endl << endl;
            } else {
//                cout << "We didn't push it because it wasn't right maximal" << endl << endl << endl;
            }
        }
        
        if (dOcc[1] > 1) {
            prependedCharacter = 'C';
            possibleNode.pattern = prepend(prependedCharacter, internalNode.pattern); //just for debugging
            
            //Save previous interval for use in determining reverse interval
            vector<long long> previousInterval = internalNode.interval;
            
            
            //Determine interval
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            
            //Determine reverse interval (note: not sure if I can do this step later or not
            possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            
//            //DEBUG PRINTOUT:
//            cout << "We tried appending C. The new pattern is: " << possibleNode.pattern << endl;
//            cout << "and the new interval is: [" << possibleNode.interval[0] << "," << possibleNode.interval[1] << "]" << endl << endl;
//            cout << "and the new reverse interval is: [" << possibleNode.reverseInterval[0] << "," << possibleNode.reverseInterval[1] << "]" << endl << endl;
            
            
            //Determine if right maximal, and if so, obtain reverse interval and PUSH
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
//            //DEBUG PRINTOUT:
//            cout << "The number of children was: " << possibleNode.numberOfChildren << endl << endl;
//            
            
            if(possibleNode.isRightMaximal()) {
                
//                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                possibleNode.depth = internalNode.depth + 1;

                nodeStack.push(possibleNode);
                
//                cout << "We pushed it because it was right maximal" << endl << endl << endl;

            } else {
//                cout << "We didn't push it because it wasn't right maximal" << endl << endl << endl;
            }
        }
        
        if (dOcc[2] > 1) {
            prependedCharacter = 'G';
            possibleNode.pattern = prepend(prependedCharacter, internalNode.pattern); //just for debugging
            
            
            //Save previous interval for use in determining reverse interval
            vector<long long> previousInterval = internalNode.interval;
            
            //Determine interval
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            
            //Determine reverse interval (note: not sure if I can do this step later or not
            possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            
//            //DEBUG PRINTOUT:
//            cout << "We tried appending G. The new pattern is: " << possibleNode.pattern << endl;
//            cout << "and the new interval is: [" << possibleNode.interval[0] << "," << possibleNode.interval[1] << "]" << endl << endl;
//            cout << "and the new reverse interval is: [" << possibleNode.reverseInterval[0] << "," << possibleNode.reverseInterval[1] << "]" << endl << endl;
            
            
            //Determine if right maximal, and if so, obtain reverse interval and PUSH
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
//            //DEBUG PRINTOUT:
//            cout << "The number of children was: " << possibleNode.numberOfChildren << endl << endl;
            
            
            if(possibleNode.isRightMaximal()) {
                
//                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                possibleNode.depth = internalNode.depth + 1;

                nodeStack.push(possibleNode);
                
//                cout << "We pushed it because it was right maximal" << endl << endl << endl;

            } else {
//                cout << "We didn't push it because it wasn't right maximal" << endl << endl << endl;
            }
        }
        
        if (dOcc[3] > 1) {
            prependedCharacter = 'T';
            possibleNode.pattern = prepend(prependedCharacter, internalNode.pattern); //just for debugging
            
            
            
            //Save previous interval for use in determining reverse interval
            vector<long long> previousInterval = internalNode.interval;
            
            
            //Determine interval
            possibleNode.interval = getIntervalBasedOn(internalNode.interval, prependedCharacter, occ);
            
            //Determine reverse interval (note: not sure if I can do this step later or not
            possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
            
//            //DEBUG PRINTOUT:
//            cout << "We tried appending T. The new pattern is: " << possibleNode.pattern << endl;
//            cout << "and the new interval is: [" << possibleNode.interval[0] << "," << possibleNode.interval[1] << "]" << endl << endl;
//            cout << "and the new reverse interval is: [" << possibleNode.reverseInterval[0] << "," << possibleNode.reverseInterval[1] << "]" << endl << endl;
            
            
            //Determine if right maximal, and if so, obtain reverse interval and PUSH
            possibleNode.getNumberOfChildren(reverseBWT, occ.$_pos);
            
//            //DEBUG PRINTOUT:
//            cout << "The number of children was: " << possibleNode.numberOfChildren << endl << endl;
//            
            
            if(possibleNode.isRightMaximal()) {
                
//                possibleNode.reverseInterval = getReverseIntervalBasedOn(previousInterval, internalNode.reverseInterval, prependedCharacter, reverseBWT, occ);
                possibleNode.depth = internalNode.depth + 1;

                nodeStack.push(possibleNode);
                
//                cout << "We pushed it because it was right maximal" << endl << endl << endl;

            } else {
//                cout << "We didn't push it because it wasn't right maximal" << endl << endl << endl;
            }
        }
        
        
    }
}



int main(int argc, const char * argv[]) {
    
    //Proper input style: <command>, <kmer size>, <BWT file>, <reverse BWT file>
    
    if (argc != 5) {
        //perhaps return an error.
        cout << "error: why did you not supply 4 arguments?" << endl << "The proper input style is <command>, <kmer size>, <BWT file>, <reverse BWT file> (without the commas)" << endl << "You supplied the following arguments:" << endl;
        for (size_t i = 1; i < static_cast<unsigned int>(argc); i++) {
            cout << argv[i] << endl;
        }
        return 0;
    }
    
    //the only command we want to support right now is count
    
    if (argv[1] == string("count")) {
        //call the count function
//        cout << "Count called on filename " << argv[0];
        
        ifstream ifs;
        
        
        
        //Retrieve kmer size:
        ifs.open(argv[2]);
        
        
        if (ifs.is_open() == false) {
            cout << "ERROR, the kmer size could not be understood" << endl;
            ifs.close();
            return 0;
        }
        while (!ifs.eof()) {
            ifs >> k;
        }
        ifs.close();
//        cout << " with kmer size of " << k << endl;
        
        
        //Retrieve forward BWT:
        ifs.open(argv[3]);
        
        if (ifs.is_open() == false) {
            cout << "ERROR, the input file could not be read" << endl;
            ifs.close();
            return 0;
        }
        
        while (!ifs.eof())
        {
            //do some stuff, specifically parse the bwt
            //I don't ever want to have to copy the entire string into a variable or function, so pass by reference makes sense I think
            
            //for now, store the BWT string into a variable and pass that by reference, I'll figure out how to avoid this step later
            
            ifs >> BWT;
            
        }
        ifs.close();
        
//        cout << "The BWT appears to have been read successfully" << endl;
        
        //Retrieve reverse BWT:
        ifs.open(argv[4]); //open the reverse BWT
        
        if (ifs.is_open() == false) {
            cout << "ERROR, the reverse BWT could not be read" << endl;
            ifs.close();
            return 0;
        }
        
        while (!ifs.eof()) {
            ifs >> reverseBWT;
        }
        ifs.close();
        
//        cout << "The reverse BWT appears to have been read successfully" << endl;
//        cout << "DEBUG reverse BWT: " << reverseBWT << endl;
        
        //Check to see if the size of the BWT and size of the reverse BWT are the same. If not, we have a problem.
        if(BWT.size() != reverseBWT.size()) {
            cout << "BWT's are different sizes, please check files for accuracy." << endl;
            return 0;
        }
        
        
        //Build the occ table using the BWT
        occ.build(BWT, 1);
        
        
        //Initialize the K-mer counts using the BWT
        
        
        const long long NUMBER_OF_RELEVANT_LEAF_NODES = BWT.size() - (k - 1);
        const long long OFFSET_DUE_TO_$ = 1;
        
        
        kmerCounts = NUMBER_OF_RELEVANT_LEAF_NODES - OFFSET_DUE_TO_$; //the final kmerCount will also include a value representing the sum over all the internal nodes of (1 - # of children at each node)
        
//        //Debugging occ
//        cout << "Occ table:" << endl << occ << endl;
//        cout << occ.return_$() << endl;
//        
        
        //K-mer counting:
        
        countKmers(occ);
        
        cout << "The result of the kmer counting program is that there are:" << endl << kmerCounts << endl << "unique kmers in the sequence string" << endl;

        
    } else {
        cout << "ERROR: you did not call the count command" << endl;
        return 0;
    }
    
    return 0;
    
    //eventually we will also allow people to input their own C array and occ table, but not now.
}












