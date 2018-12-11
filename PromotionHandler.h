
#ifndef iCasinov2_PromotionHandler_h
#define iCasinov2_PromotionHandler_h

#include <iostream>
#include "cocos2d.h"
#include <vector>

using namespace std;

struct PromotionOptions{
	int gameid;
	string kmopt;
	string numgameplayed;
	string numgamekm;
};

struct PromotionInfo{
	string kmdesc;
	string kmname;	
	int kmid;
    bool isUsed;
	vector<PromotionOptions> lstPromotionOptions;
};

class PromotionHandler{
public:
	PromotionHandler();
	~PromotionHandler();

	void handleDatas(string datas);
	CC_SYNTHESIZE(string, sUser_ID, UserID);

    vector<PromotionInfo> getListPromotionInfo();
    void updateListPromotionInfo(PromotionInfo info);
private:
	vector<PromotionInfo> lstPromotionInfo;
};
#endif