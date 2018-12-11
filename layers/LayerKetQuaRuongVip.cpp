//
//  LayerKetQuaRuongVip.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/10/16.
//
//

#include "LayerKetQuaRuongVip.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "LayerPopupRuongVip.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerKetQuaRuongVip::LayerKetQuaRuongVip()
{
    
}


LayerKetQuaRuongVip::~LayerKetQuaRuongVip()
{
    
}

bool LayerKetQuaRuongVip::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerResultRuongVip.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    ImageView* imgCircle = static_cast<ImageView*>(pnlBg->getChildByName("Sprite_2"));
    auto ActionRotate = RepeatForever::create(RotateBy::create(0.2,10));
    imgCircle->runAction(ActionRotate);
    
    img1 = static_cast<ImageView*>(pnlBg->getChildByName("img1"));
    this->imgMoney.push_back(img1);
    img2 = static_cast<ImageView*>(pnlBg->getChildByName("img2"));
    this->imgMoney.push_back(img2);
    img3 = static_cast<ImageView*>(pnlBg->getChildByName("img3"));
    this->imgMoney.push_back(img3);
    img4 = static_cast<ImageView*>(pnlBg->getChildByName("img4"));
    this->imgMoney.push_back(img4);
    img5 = static_cast<ImageView*>(pnlBg->getChildByName("img5"));
    this->imgMoney.push_back(img5);
    img6 = static_cast<ImageView*>(pnlBg->getChildByName("img6"));
    this->imgMoney.push_back(img6);
    img7 = static_cast<ImageView*>(pnlBg->getChildByName("img7"));
    this->imgMoney.push_back(img7);
    img8 = static_cast<ImageView*>(pnlBg->getChildByName("img8"));
    this->imgMoney.push_back(img8);
    img9 = static_cast<ImageView*>(pnlBg->getChildByName("img9"));
    this->imgMoney.push_back(img9);
    img10 = static_cast<ImageView*>(pnlBg->getChildByName("img10"));
    this->imgMoney.push_back(img10);

    btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnGet"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerKetQuaRuongVip::onBtnClose, this));
    }
    
    imgTrungLon = static_cast<Sprite*>(pnlBg->getChildByName("imgTrungLon"));
    iconBac = static_cast<Sprite*>(pnlBg->getChildByName("iconBac"));
    sprThongBao = static_cast<Sprite*>(pnlBg->getChildByName("Sprite_3"));
    return true;
}
void LayerKetQuaRuongVip::onBtnClose(Ref* pSender){
    
    LayerPopupRuongVip* parent = (LayerPopupRuongVip*)(this->getParent());
    if(parent){
        parent->showWebview();
        parent->isOpenning = false;
    }
    this->removeFromParentAndCleanup(true);
}
bool LayerKetQuaRuongVip::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerKetQuaRuongVip::setMoney(string money){
    int lengh = money.size();
    for (int i = 0; i< imgMoney.size(); i++) {
        this->imgMoney.at(i)->setVisible(false);
    }
    
    if (std::strcmp(money.c_str(), "0")==0){
        iconBac->setVisible(false);
        imgTrungLon->setVisible(false);
        sprThongBao->setTexture("chuc-may-man-lan-sau.png");
        btnClose->loadTextureNormal("ok-button.png");
        btnClose->loadTexturePressed("ok-button.png");
        btnClose->loadTextureDisabled("ok-button.png");
        return;
    }
    
    int a = imgMoney.size()-lengh;
    
    for (int i = 0; i < lengh; i++) {
        this->imgMoney.at(a+i)->loadTexture(StringUtils::format("number-%c.png",money.at(i)));
        this->imgMoney.at(a+i)->setVisible(true);
    }
    if (atoi(money.c_str()) < 50000){
        imgTrungLon->setVisible(false);
    }
}
void LayerKetQuaRuongVip::onEnter()
{
    Layer::onEnter();
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(LayerKetQuaRuongVip::onTouchBegan, this);
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerKetQuaRuongVip::onExit()
{
    Layer::onExit();
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}




