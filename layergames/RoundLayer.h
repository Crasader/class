//
//  RoundLayer.hpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 8/31/16.
//
//

#ifndef RoundLayer_hpp
#define RoundLayer_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class RoundLayer : public Layer{
public:
    virtual bool init()override;
    CREATE_FUNC(RoundLayer);
    void updateRound(float dt);
    void startRound();
    void stopRound(int tag);
    void stopUpdate();
    void initCollectibleView(Layer* layer);
    void setStopCallBackFunc(Ref* target, SEL_CallFuncO callfun);
    int getTagCardFromStr(string strCard);

    
    virtual void onEnter() override;
    virtual void onExit() override;
    

    
private:
    
    struct card{
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
    vector<card> arrCard;
    int stopTag = 0;
    SEL_CallFuncO m_callbackListener;
    Ref *m_callback;
};
#endif /* RoundLayer_hpp */
