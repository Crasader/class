//
//  RoundLayerSlot.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#ifndef RoundLayerSlot_hpp
#define RoundLayerSlot_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class RoundLayerSlot : public Layer{
public:
    virtual bool init();
    CREATE_FUNC(RoundLayerSlot);
    void updateRound(float dt);
    void startRound();
    void stopRound(string tag);
    void stopUpdate();
    void resetRound();

    void initCollectibleView(Layer* layer);
    void setStopCallBackFunc(Ref* target, SEL_CallFuncO callfun);
    int getPosition(int tag);
    int getTagCardFromStr(string strCard);
    void swapItem(int tagFrom, int tagEnd);
    void initCollectibleViewTop(Layer* layer);
    void pushItemEat(int type,int row);
    
    void resetRoundToshow();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    

    
private:
    
    struct slot{
        int tag;
        string str;
    };
    
    bool isStart = true;
    Layer* m_pRound;
    Layer* m_pRoundTop;
    float layerHeight = 0;
    bool bRet = true;
    int collectibleNum;
    vector<int> arrayHeader;
    vector<slot> arrCard;
    vector<Sprite*> lstItem;
    vector<Sprite*> lstItemTop;

    string stopTag = "";
    SEL_CallFuncO m_callbackListener;
    Ref *m_callback;
    int tagItem =200;
    int tagItem2 =200;

    int tagItem3 =200;

    
    vector<Sprite*> lstResult;
    vector<int> tagEat;
    
    int width = 272;
    int height = 214;


};
#endif /* RoundLayerSlot_hpp */
