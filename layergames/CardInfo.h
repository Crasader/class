/*!
 * \class
 *
 * \brief
 *
 * \author Administrator
 * \date November 2015
 */
#ifndef CardInfo_h__
#define CardInfo_h__
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class CardInfo{
private:
	// 	int id; //id quan bai
	// 	int value; //gia tri quan bai
	// 	int substance;// chat'
	// 	bool turnup;// quan bai dang mo hay up
	CC_SYNTHESIZE(int, id, IdCard);
	CC_SYNTHESIZE(int, value, ValueCard);
	CC_SYNTHESIZE(int, substance, SubstanceCard);
	CC_SYNTHESIZE(bool, turnup, TurnUpCard);

public:
	enum{ /// cac chat cua 1 quan bai
		CARD_SUBSTANCE_BICH = 1,
		CARD_SUBSTANCE_TEP,
		CARD_SUBSTANCE_DIAMOND,
		CARD_SUBSTANCE_HEART
	};

	CardInfo();
	CardInfo(int id, int value, int substance);
	CardInfo(int id, int value, int substance, bool turnup);

	int compareTo(CardInfo *card);
	string getTypeCardName(int type);
	string getCardName(int num);
};
struct less_than_key
{
	inline bool operator() (const CardInfo* str1, const CardInfo* str2)
	{
		return str1->getIdCard() < str2->getIdCard();
	}
};
struct less_than_key_value_card
{
	inline bool operator() (const CardInfo* str1, const CardInfo* str2)
	{
		return str1->getValueCard() < str2->getValueCard();
	}
};
#endif // CardInfo_h__
