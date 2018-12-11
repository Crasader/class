//
//  LayerMainGame2.hpp
//  EnglishGame-mobile
//
//  Created by Sherlock Tuan on 11/30/18.
//

#ifndef LayerMainGame2_hpp
#define LayerMainGame2_hpp
#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class LayerMainGame2 : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    // implement the "static create()" method manually
    void OnBtnConfirm(Ref* pSender);
    void OnBtnDelete(Ref* pSender);
    void onClickLetter(Ref* pSender);
    void OnBtnNext(Ref* pSender);
    void OnBtnReplay(Ref* pSender);
    void OnBtnMenu(Ref* pSender);
    void showFail();
    void showSuccess();
    void Complete();
    
    void showBgNewGame(float dt);
    
    void showQuest();

    CREATE_FUNC(LayerMainGame2);
    Layout* pnlPopup_;
    ImageView* imgQuest;
    vector<Button*> lstBtnLetter;
    vector<string> lstLetter;
    vector<int> lstQuest;

    Text* txtName;
    string name = "";
    int currentQuest = 0;
    
    Layout* pnlPopup;
    Button* btnNext;
    Text* txtNotice;
    Button* btnMenu;
    Button* btnExit;
    Button* btnReplay;

};


#endif /* LayerMainGame2_hpp */
