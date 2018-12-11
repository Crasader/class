//
//  mUltils.hpp
//  GameDotJungle-mobile
//
//  Created by Hoàng Thảo on 4/18/18.
//

#ifndef mUtils_hpp
#define mUtils_hpp

#include <stdio.h>
using namespace std;
class mUtils{
    
public:
    static std::string convertMoneyEx(long long money){
        char sMoneyTmp[30], sMoney[30], s[30];
        
        sprintf(sMoneyTmp, "%lld", money);
        int len = strlen(sMoneyTmp);
        
        int i = len - 1, counterSeg = 0, counter = 0;
        
        while (i >= 0){
            sMoney[counter++] = sMoneyTmp[i];
            counterSeg++;
            if (counterSeg == 3){
                counterSeg = 0;
                if (i != 0)
                    sMoney[counter++] = ',';
            }
            i--;
        }
        
        for (i = counter - 1; i >= 0; i--){
            s[counter - i - 1] = sMoney[i];
        }
        s[counter] = 0;
        
        return string(s);
    };
};
#endif /* mUltils_hpp */
