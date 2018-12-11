/*!
 * \class
 *
 * \brief
 *
 * \author Administrator
 * \date November 2015
 */

#ifndef TienLenMBUtil_h__
#define TienLenMBUtil_h__
#include "cocos2d.h"
#include "CardInfo.h"
USING_NS_CC;
using namespace std;

class TienLenMBUtil{
public:
	static bool isSequence(vector<CardInfo*> listc){
		bool issequence = true;
		for (int i = 0; i < listc.size(); ++i){
			if (listc.at(i + 1)->getValueCard() - listc.at(i)->getValueCard() != 1
				|| listc.at(i + 1)->getSubstanceCard() != listc.at(i)->getSubstanceCard()
				|| listc.at(i + 1)->getValueCard() == 15
				)
			{
				issequence = false;
				break;
			}
		}
		return issequence;
	}

};



#endif // TienLenMBUtil_h__
