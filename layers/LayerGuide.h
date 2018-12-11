//
//  LayerGuide.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerGuide_hpp
#define LayerGuide_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ItemMenuGame.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerGuide: public cocos2d::Layer{
public:
    LayerGuide();
    ~LayerGuide();
    virtual bool init();
  
    
    void onBtnYes(Ref* pSender);
    void onBtnNo(Ref* pSender);
    void onBtnEnd(Ref* pSender);

    void showGuide1();
    void showGuide2();
    void showGuide3();
    void showGuide4();
    void showGuide5();
    void showGuide6();
    void showGuide7();
    void showGuide8();
    void showEnd();

    void onBtnNextGuide1(Ref* pSender);
    void onBtnNextGuide2(Ref* pSender);
    void onBtnNextGuide3(Ref* pSender);
    void onBtnNextGuide4(Ref* pSender);
    void onBtnNextGuide5(Ref* pSender);
    void onBtnNextGuide6(Ref* pSender);
    void onBtnNextGuide7(Ref* pSender);
    void onBtnNextGuide8(Ref* pSender);
    void onBtnCancelGuide(Ref* pSender);

    void tangHuTX(float dt);
    void tangHuPK(float dt);
    void tangHuCT(float dt);
    
    void createPageView();
    void createListData();
   // bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerGuide);
private:
    
    struct menuStruct{
        int gameID;
        bool state;
    };
    vector<menuStruct> lstIcons;
    vector<ItemMenuGame*> lstItemGame;
    struct iconStruct{
        int gameID;
        bool state;
        string imgNormalPath;
        string imgDisablePath;
        iconStruct(int _gameid, bool _state, string _imgNormal, string _imgDisable) :
        gameID(_gameid), state(_state), imgNormalPath(_imgNormal), imgDisablePath(_imgDisable){}
    };
    vector<iconStruct> lstIconsNew;

    Layout* pnlOption ;
    Layout* pnlGuide1;
    Layout* pnlGuide2;
    Layout* pnlGuide3;
    Layout* pnlGuide4;
    Layout* pnlGuide5;
    Layout* pnlGuide6;
    Layout* pnlGuide7;
    Layout* pnlGuide8;
    Layout* pnlEnd;

    PageView* pvMain;
    ImageView* imgTextGuide1;
    Button* btnNextGuide1;
    Button* btnCancelGuide1;
    ImageView* imgLightGuide1;
    
    
    ImageView* imgTextGuide2;
    Button* btnNextGuide2;
    Button* btnCancelGuide2;
    ImageView* imgLightGuide2;
    ImageView* imgLightBGGuide2;

    
    ImageView* imgTextGuide3;
    Button* btnNextGuide3;
    Button* btnCancelGuide3;
    ImageView* imgLightGuide3;

    ImageView* imgTextGuide4;
    Button* btnNextGuide4;
    Button* btnCancelGuide4;
    ImageView* imgLightGuide4;
    
    ImageView* imgTextGuide5;
    Button* btnNextGuide5;
    Button* btnCancelGuide5;
    ImageView* imgLightGuide5;
    
    ImageView* imgTextGuide6;
    Button* btnNextGuide6;
    Button* btnCancelGuide6;
    ImageView* imgLightGuide6;
    
    ImageView* imgTextGuide7;
    Button* btnNextGuide7;
    Button* btnCancelGuide7;
    ImageView* imgLightGuide7;
    
    ImageView* imgTextGuide8;
    Button* btnNextGuide8;
    Button* btnCancelGuide8;
    ImageView* imgLightGuide8;
    ImageView* imgLightBGGuide3;
    ImageView* imgLightBGGuide4;
    ImageView* imgLightBGGuide5;
    ImageView* imgLightBGGuide6;
    ImageView* imgLightBGGuide7;
    ImageView* imgLightBGGuide8;

    Text* txtHuTX;
    Text* txtHuCT;
    Text* txtHuPK;
    Layout* pnlTX;
    Layout* pnlPK;
    Layout* pnlCT;
};

#endif /* LayerGuide_hpp */
