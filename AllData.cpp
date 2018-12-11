//
//  AllData.cpp
//  iCasinov2
//
//  Created by Thanhhv on 5/23/14.
//
//

#include "AllData.h"
//#include <algorithm>

DoTogether::DoTogether(){};
DoTogether::~DoTogether(){};

void DoTogether::releaseVector(vector<int> &arr) {
    while (arr.size() > 0) {
        arr.pop_back();
    }
}

void DoTogether::releaseVector(vector<string> &arr) {
    while (arr.size() > 0) {
        arr.pop_back();
    }
}




