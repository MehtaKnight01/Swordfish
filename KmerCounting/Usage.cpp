//
//  Usage.cpp
//  KmerCounting
//
//  Created by Rishil on 7/30/16.
//  Copyright © 2016 Rishil Mehta. All rights reserved.
//

#include "Usage.hpp"
#include <iostream>

int usage(std::string name) {
    
    std::cerr << "usage: " << name << " k-mer_size fwdBWT_file revBWT_file" << std::endl;
    std::cerr << "\t k-mer_size: between 1 and 32" << std::endl;
    
    return 0;
    
}