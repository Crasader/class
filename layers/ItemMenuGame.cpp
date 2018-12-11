#include "ItemMenuGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../layergames/TaiXiuMessageDefs.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "../layergames/LayerPlayGameMiniPoker.h"
#include "../layergames/LayerPlayGameDoDenMini.h"
#include "../layergames/LayerCaoThapPopupGame.h"
#include "../layergames/LayerPlayVQMM.h"
#include "../layergames/LayerPlaySlotMachine.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "LayerPopupChonDongTien.h"
#include "../mUtils.h"
#include "../SceneManager.h"
#include "Requests/SubscribeRoomGroupRequest.h"
#include "Requests/UnsubscribeRoomGroupRequest.h"
#include "LayerPopupThongBao.h"
#include "LayerGuideInGame.h"
using namespace cocos2d::ui;


ItemMenuGame::ItemMenuGame()
{

}

ItemMenuGame::~ItemMenuGame()
{
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(_touchListener);
//    this->stopAllActions();
//    if (this->sprite){
//        this->sprite->stopAllActions();
//    }
//    if (this->sprTom){
//        this->sprTom->stopAllActions();
//    }
//    if (this->sprCua){
//        this->sprCua->stopAllActions();
//    }
//    if (this->sprCa){
//        this->sprCa->stopAllActions();
//    }
//    if (this->sprBubble){
//        this->sprBubble->stopAllActions();
//    }
//    this->unschedule(schedule_selector(ItemMenuGame::updateHuSlot));
//    this->removeChildByTag(99);
//    GameServer::getSingleton().removeListeners(this);
//    for (int i = 0 ; i < this->lstCard.size();i++){
//        this->lstCard[i]->stopAllActions();
//    }
//    for (int i = 0 ; i < this->lstCardCatte.size() ; i++){
//        this->lstCardCatte[i]->stopAllActions();
//    }
//    for (int i = 0 ; i < this->lstCardBaDoi.size() ; i++){
//        this->lstCardBaDoi[i]->stopAllActions();
//    }
//    for (int i = 0 ; i < this->lstCardMB.size() ; i++){
//        this->lstCardMB[i]->stopAllActions();
//    }
//    this->lstCard.clear();
//    this->lstCardCatte.clear();
//    this->lstCardBaDoi.clear();
//    this->lstCardMB.clear();
//    this->animFrames.clear();
//
//    this->sprite = NULL;
//    this->sprTom = NULL;
//    this->sprCua = NULL;
//    this->sprCa = NULL;
//    this->sprBubble = NULL;
}

bool ItemMenuGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //this->setKeyboardEnabled(false);
    this->setTag(POPUP_THONGBAO);
    
    auto rootNode = CSLoader::getInstance()->createNode("ItemMenuGame.csb");
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    this->pnlDisable = static_cast<Layout*>(rootNode->getChildByName("pnlDisable"));
    this->pnlActive = static_cast<Layout*>(rootNode->getChildByName("pnlActive"));
    this->pnlDisable->setSwallowTouches(false);
    this->pnlActive->setSwallowTouches(false);
    
    this->pnlTLMN = static_cast<Layout*>(this->pnlActive->getChildByName("pnlTLMN"));
    this->pnlTLMN->setSwallowTouches(false);
    this->pnlTLMN->setVisible(false);
    
    this->pnlNewGame = static_cast<Layout*>(this->pnlActive->getChildByName("pnlNewGame"));
    this->pnlSlot = static_cast<Layout*>(this->pnlActive->getChildByName("pnlSlot"));
    this->pnlSlot->setSwallowTouches(false);
    
    this->pnlSlot->setVisible(false);
    
    this->pnlTXBig = static_cast<Layout*>(this->pnlActive->getChildByName("pnlTXBig"));
    this->pnlTXBig->setSwallowTouches(false);
    this->pnlTXBig->setVisible(false);
    
    
    
    this->pnlTX = static_cast<Layout*>(this->pnlActive->getChildByName("pnlTX"));
    this->pnlTX->setSwallowTouches(false);
    
    this->pnlTX->setVisible(false);
    
    this->pnlCatte = static_cast<Layout*>(this->pnlActive->getChildByName("pnlCatte"));
    this->pnlCatte->setSwallowTouches(false);
    
    this->pnlCatte->setVisible(false);
    
    this->pnlBaDoi = static_cast<Layout*>(this->pnlActive->getChildByName("pnlBaDoi"));
    this->pnlBaDoi->setSwallowTouches(false);
    
    this->pnlBaDoi->setVisible(false);
    
    this->pnlMB = static_cast<Layout*>(this->pnlActive->getChildByName("pnlMB"));
    this->pnlMB->setSwallowTouches(false);
    
    this->pnlMB->setVisible(false);
    
    this->pnlXD = static_cast<Layout*>(this->pnlActive->getChildByName("pnlXD"));
    this->pnlXD->setSwallowTouches(false);
    
    this->pnlXD->setVisible(false);
    
    
    this->pnlTCC = static_cast<Layout*>(this->pnlActive->getChildByName("pnlTCC"));
    this->pnlTCC->setSwallowTouches(false);
    
    this->pnlTCC->setVisible(false);
    
    this->imgGameName = static_cast<ImageView*>(this->pnlActive->getChildByName("imgGameName"));
    //    auto btnGame = static_cast<Button*>(this->pnlActive->getChildByName("btnGame"));
    ////    if (btnGame){
    ////        btnGame->addTouchEventListener(CC_CALLBACK_2(ItemMenuGame::onBtnGame, this));
    //    btnGame->setSwallowTouches(false);
    //    this->setSwallowTouches(false);
    //    }
    
    if (SceneManager::getSingleton().isNagaWin){
        this->pnlEvent = static_cast<Layout*>(rootNode->getChildByName("pnlEvent"));
        this->pnlEvent->setVisible(false);
        this->sprStick = static_cast<Sprite*>(this->pnlEvent->getChildByName("sprStick"));
        this->txtEvent = static_cast<Text*>(this->pnlEvent->getChildByName("txtEvent"));
        auto rotation = RotateBy::create(0.5, 360, 360);
        rotation->setTag(123);
        this->sprStick->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.15,1.02),CallFunc::create([this](){this->sprStick->setTexture("stiker-do.png");}),DelayTime::create(0.15),rotation,DelayTime::create(0.5),CallFunc::create([this](){this->sprStick->stopActionByTag(123);}),CallFunc::create([this](){this->sprStick->setRotation(0);}),ScaleTo::create(0.15,1),CallFunc::create([this](){this->sprStick->setTexture("stiker-xanh.png");}),DelayTime::create(0.5),NULL)));
        auto btnSticker = static_cast<Button*>(this->pnlEvent->getChildByName("btnSticker"));
        if (btnSticker){
            btnSticker->addClickEventListener(CC_CALLBACK_1(ItemMenuGame::onBtnSticker, this));
        }
    }
    
   
    this->sprite = NULL;
    this->sprTom = NULL;
    this->sprCua = NULL;
    this->sprCa = NULL;
    this->sprBubble = NULL;
    
    return true;
}
void ItemMenuGame::onBtnSticker(Ref* pSender){
    LayerGuideInGame* layer = LayerGuideInGame::create();
    layer->setLink(this->urlSticker);
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene)
        return;
    scene->addChild(layer, 2000);
    layer->setTag(1504);
}
void ItemMenuGame::hideButtonPlayTCC(){
    this->isNoOpen = true;
}
void ItemMenuGame::playAnimationTCC(){
    this->pnlTCC->setVisible(true);

    if (SceneManager::getSingleton().isNagaWin){
        
        auto imgDice = (Sprite*)this->pnlTCC->getChildByName("imgDice");
        auto orgX = imgDice->getPositionX();
        auto orgY = imgDice->getPositionY();
        auto step = 5;
        if (imgDice){
            imgDice->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.05,Point(orgX,orgY+step)),MoveTo::create(0.05,Point(orgX+step,orgY+step)),MoveTo::create(0.05,Point(orgX-step,orgY+step)),MoveTo::create(0.05,Point(orgX-step,orgY-step)),DelayTime::create(1), NULL)));
            //imgDice->runAction(RepeatForever::create(Shaky3D::create(0.1, Size(10,10), 5, false)));
            //imgDice->runAction(RepeatForever::create(Sequence::create(MoveBy::create(0.1, Vec2(10,0)),DelayTime::create(0.15),MoveBy::create(0.1, Vec2(0,-10)),DelayTime::create(0.15),MoveBy::create(0.1, Vec2(-10,10)),DelayTime::create(0.15),MoveBy::create(0.1, Vec2(-10,0)),DelayTime::create(0.15), NULL)));
        }
        return;
    }
    
    
    //return;
    this->gameID = kGameBauTom;
    this->imgGameName->setVisible(false);
    this->imgGameName = static_cast<ImageView*>(this->pnlTCC->getChildByName("Image_3"));
    this->imgGameName->setVisible(true);
    string _imgNormal = StringUtils::format("gameicons/%s%d.png",SceneManager::getSingleton().getFolderLang().c_str(),gameID);
    this->imgGameName->loadTexture(_imgNormal);
    auto cua = static_cast<Sprite*>(this->pnlTCC->getChildByName("sprCua"));
    cua->setVisible(false);
    this->sprCua = Sprite::create("AnimBauCua/cua1.png");
    this->pnlTCC->addChild(this->sprCua);
    this->sprCua->setPosition(cua->getPosition());
    this->sprCua->setAnchorPoint(cua->getAnchorPoint());
    this->sprCua->setLocalZOrder(100);
    this->animFrames.clear();
    
    string str = "";
    for (int i = 1; i <= 2; i++) {
        str = StringUtils::format("AnimBauCua/cua%d.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 209, 136));
        this->animFrames.pushBack(spriteFrame);
    }
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    Animate* animN = Animate::create(animation);
    this->sprCua->runAction(RepeatForever::create(Sequence::create(animN,NULL)));
    this->sprCua->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.5),MoveTo::create(1.3, Vec2(100,73)),DelayTime::create(1.5),MoveTo::create(1.3, Vec2(250,73)),DelayTime::create(1.5), NULL)));
    
    
    
    auto tom = static_cast<Sprite*>(this->pnlTCC->getChildByName("sprTom"));
    tom->setVisible(false);
    this->sprTom = Sprite::create("AnimBauCua/tom.png");
    this->pnlTCC->addChild(this->sprTom);

    this->sprTom->setPosition(tom->getPosition());
    this->sprTom->setAnchorPoint(tom->getAnchorPoint());
    this->sprTom->setLocalZOrder(100);
    
    this->sprTom->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),MoveTo::create(1, Vec2(110,217)),DelayTime::create(0.5),MoveTo::create(1, Vec2(110,196)),DelayTime::create(0.5), NULL)));
    
    
    auto bubble = static_cast<Sprite*>(this->pnlTCC->getChildByName("bubble1"));
    bubble->setVisible(false);
    this->sprBubble = Sprite::create("AnimBauCua/bubble2.png");
    this->pnlTCC->addChild(this->sprBubble);

    this->sprBubble->setPosition(bubble->getPosition());
    this->sprBubble->setAnchorPoint(bubble->getAnchorPoint());
    this->sprBubble->setLocalZOrder(100);
    
    this->sprBubble->runAction(RepeatForever::create(Sequence::create(MoveTo::create(1, Vec2(this->sprBubble->getPositionX(),478)),MoveTo::create(1, Vec2(this->sprBubble->getPositionX(),458)), NULL)));
    auto ca = static_cast<Sprite*>(this->pnlTCC->getChildByName("sprCa"));
    ca->setVisible(false);
    this->sprCa = Sprite::create("AnimBauCua/ca1.png");
    
    this->sprCa->setPosition(ca->getPosition());
    this->sprCa->setAnchorPoint(ca->getAnchorPoint());
    this->sprCa->setLocalZOrder(100);
    this->animFrames.clear();
    this->pnlTCC->addChild(this->sprCa);

    str= "";
    for (int i = 1; i <= 3; i++) {
        str = StringUtils::format("AnimBauCua/ca%d.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 243, 150));
        this->animFrames.pushBack(spriteFrame);
    }
    animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    animN = Animate::create(animation);
    this->sprCa->runAction(RepeatForever::create(Sequence::create(animN,NULL)));
    
    auto btnGame = static_cast<Button*>(this->pnlTCC->getChildByName("btnGame"));
    btnGame->addClickEventListener(CC_CALLBACK_1(ItemMenuGame::onBtnGameOpen, this));
    auto emitter = ParticleSystemQuad::create("particles/bubble.plist");    
    emitter->setPosition(Vec2(this->pnlTCC->getContentSize().width/2,20));
    emitter->setAutoRemoveOnFinish(true);
    emitter->setLife(3);
    TextureCache* tc = Director::getInstance()->getTextureCache();
    Texture2D* coin = tc->addImage("particles/bubble.png");
    emitter->setTexture(coin);
    this->pnlTCC->addChild(emitter, 99,99);
}
void ItemMenuGame::scheduleBubble(float dt){
    
}
void ItemMenuGame::onBtnGameOpen(Ref* pSender){
    if(dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(kGameBauTom)) != std::string::npos) {
        auto currscene = Director::getInstance()->getRunningScene();
        if (currscene == NULL)
            return;
        LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
        layerpoup->setMessage(dataManager.GetSysString(141));
        layerpoup->setContentMess(dataManager.GetSysString(612));
        layerpoup->showAButton();
        layerpoup->setTag(24);
        currscene->addChild(layerpoup,2000);
        return;
    }
    if ( this->isNoOpen) return;
    SceneManager::getSingleton().setCurrRoomType(0);
   // SceneManager::getSingleton().showLoading();
    SceneManager::getSingleton().setGameID(kGameBauTom);
    SceneManager::getSingleton().gotoPickGame(kGameBauTom);
}
void ItemMenuGame::showXD(){
    this->pnlXD->setVisible(true);
    
    if (SceneManager::getSingleton().isNagaWin){
        this->pnlTX->setVisible(true);
        
        this->sprite = (Sprite*)this->pnlTX->getChildByName("sprAnim");
        this->animFrames.clear();
        string str = "";
        for (int i = 0; i <= 20; i++) {
            str = StringUtils::format("icongame-xocdia-anim_%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 247, 281));
            this->animFrames.pushBack(spriteFrame);
        }
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        Animate* animN = Animate::create(animation);
        this->sprite->runAction(RepeatForever::create(Sequence::create(animN,DelayTime::create(0.5),NULL)));
        return;
    }
    
    
    auto imgKQ = static_cast<ImageView*>(this->pnlXD->getChildByName("imgKQ"));
    auto txtKQXD= static_cast<Text*>(imgKQ->getChildByName("txtKQ"));
    txtKQXD->setString( dataManager.GetSysString(536));
}

void ItemMenuGame::showTXBig(){
    this->pnlTXBig->setVisible(true);
    
}

void ItemMenuGame::setShouldTouch(bool isTouch){
    this->shouldTouch = isTouch;
}
void ItemMenuGame::onBtnGame(Ref* pSender, ui::Widget::TouchEventType pType){
    if (pType == ui::Widget::TouchEventType::ENDED) {
        if (!this->shouldTouch) return;
        int tag = this->gameID;
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
        UserDefault::getInstance()->flush();
        SceneManager::getSingleton().setGameID(tag);
        if (tag == kGameTaiXiu || tag == kGameQuayThuong  || tag == kGameBauTom) {
            /*SceneManager::getSingleton().showLoading();
             SceneManager::getSingleton().gotoPickGame(tag);*/
            LayerPopupChonDongTien *_layerPopupChonDongTien = LayerPopupChonDongTien::create(tag);
            _currScene->addChild(_layerPopupChonDongTien, 10000,9102);
        }
        else if (tag == kGameSlot|| tag == kGameXocDia){
            SceneManager::getSingleton().setCurrRoomType(0);
            SceneManager::getSingleton().showLoading();
            SceneManager::getSingleton().gotoPickGame(tag);
        }
        else if (tag == kGameVQMM || tag == kGameMNPK || tag== kGameXDMN|| tag == kGameCT|| tag == kGameTX){
            this->gotoGameMini(tag);
        }
        else
        {
            SceneManager::getSingleton().gotoPickGame(tag);
        }
    }
}
void ItemMenuGame::gotoGameMini(int tag){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    if (tag == kGameMNPK){
        if (SceneManager::getSingleton().getIsShowPK())
            return;
        LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
        _currScene->addChild(layerPlayerGame,1000,9103);
        SceneManager::getSingleton().setIsShowPK(true);
    }
    else if(tag == kGameXDMN){
        if (SceneManager::getSingleton().getIsShowXD())
            return;
        LayerPlayGameDoDenMini* layerPlayerGame = LayerPlayGameDoDenMini::create();
        _currScene->addChild(layerPlayerGame,1000,9104);
        SceneManager::getSingleton().setIsShowXD(true);
    }
    else if(tag == kGameCT){
        if (SceneManager::getSingleton().getIsShowCT())
            return;
        LayerCaoThapPopupGame* layerPlayerGame = LayerCaoThapPopupGame::create();
        _currScene->addChild(layerPlayerGame,1000,9105);
        SceneManager::getSingleton().setIsShowCT(true);
    }
    else if(tag == kGameVQMM){
        if (SceneManager::getSingleton().getIsShowVQ())
            return;
        LayerPlayVQMM* layerPlayerGame = LayerPlayVQMM::create();
        _currScene->addChild(layerPlayerGame,1000,9106);
        SceneManager::getSingleton().setIsShowVQ(true);
    }
    else if(tag == kGameTX){
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        _currScene->addChild(layerPlayerGame,1000,9107);
        layerPlayerGame->shouldShowHuongDan();
        SceneManager::getSingleton().setIsShowTX(true);
        
    }
    
    
}
void ItemMenuGame::reloadLang(){
    string _imgNormal = StringUtils::format("gameicons/%s%d.png",SceneManager::getSingleton().getFolderLang().c_str(),this->gameID);
    this->imgGameName->loadTexture(_imgNormal);
}
void ItemMenuGame::setGameID(int gameID, bool state){
    string _imgNormal = StringUtils::format("gameicons/%s%d.png",SceneManager::getSingleton().getFolderLang().c_str(),gameID);
    this->imgGameName->loadTexture(_imgNormal);
    this->gameID = gameID;
    switch (gameID) {
        case 112:
            this->animationTLMN();
            break;
        case 116:
            this->animationTLMN();
            break;
        case 139:
            this->animationSLot();
            break;
        case 250:
            this->animationBaDoi();
            break;
        case 140:
            this->animationCatte();
            break;
        case 128:
            this->animationTX();
            break;
        case 137:
            this->animationMB();
            break;
        case 113:
            this->showTXBig();
            break;
        case 114:
            this->showXD();
            break;
        case 133:
            this->playAnimationTCC();
            break;
        case 138:
//            this->playAnimationTCC();
            break;
        default:
            this->setVisible(false);
            break;
    }
    if (state){
        this->pnlDisable->setVisible(false);
        this->pnlActive->setVisible(true);
    }else{
        this->pnlDisable->setVisible(true);
        this->pnlActive->setVisible(false);
        auto imgGame = static_cast<ImageView*>(this->pnlDisable->getChildByName("imgGame"));
        if (imgGame){
            imgGame->loadTexture(StringUtils::format("gameicons/%s%d_dis.png",SceneManager::getSingleton().getFolderLang().c_str(),gameID));
        }
    }
}
void ItemMenuGame::animationMB(){
    this->pnlMB->setVisible(true);
    this->lstCardMB.clear();
    for (int i = 1; i <= 13;i++){
        auto img = static_cast<ImageView*>(this->pnlMB->getChildByName(StringUtils::format("img%d",i)));
        this->lstCardMB.push_back(img);
    }
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationMB1, this)),DelayTime::create(0.7),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationMB2, this)),DelayTime::create(0.9),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationMB3, this)),DelayTime::create(1), NULL)));
}
void ItemMenuGame::playAnimationMB1(){
    auto orgX1 = this->lstCardMB[0]->getPositionX();
    auto orgX2 = this->lstCardMB[1]->getPositionX();
    auto orgX3 = this->lstCardMB[2]->getPositionX();
    
    auto orgY1 = this->lstCardMB[0]->getPositionY();
    auto orgY2 = this->lstCardMB[1]->getPositionY();
    auto orgY3 = this->lstCardMB[2]->getPositionY();
    
    int step = 10;
    this->lstCardMB[0]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX1, orgY1+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX1, orgY1)), NULL));
    this->lstCardMB[1]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX2, orgY2+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX2, orgY2)), NULL));
    this->lstCardMB[2]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX3, orgY3+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX3, orgY3)), NULL));
}
void ItemMenuGame::playAnimationMB2(){
    auto orgX4 = this->lstCardMB[3]->getPositionX();
    auto orgX5 = this->lstCardMB[4]->getPositionX();
    auto orgX6 = this->lstCardMB[5]->getPositionX();
    auto orgX7 = this->lstCardMB[6]->getPositionX();
    auto orgX8 = this->lstCardMB[7]->getPositionX();
    
    auto orgY4 = this->lstCardMB[3]->getPositionY();
    auto orgY5 = this->lstCardMB[4]->getPositionY();
    auto orgY6 = this->lstCardMB[5]->getPositionY();
    auto orgY7 = this->lstCardMB[6]->getPositionY();
    auto orgY8 = this->lstCardMB[7]->getPositionY();
    
    int step = 10;
    this->lstCardMB[3]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX4, orgY4+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX4, orgY4)), NULL));
    this->lstCardMB[4]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX5, orgY5+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX5, orgY5)), NULL));
    this->lstCardMB[5]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX6, orgY6+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX6, orgY6)), NULL));
    this->lstCardMB[6]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX7, orgY7+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX7, orgY7)), NULL));
    this->lstCardMB[7]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX8, orgY8+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX8, orgY8)), NULL));
}
void ItemMenuGame::playAnimationMB3(){
    auto orgX9 = this->lstCardMB[8]->getPositionX();
    auto orgX10 = this->lstCardMB[9]->getPositionX();
    auto orgX11 = this->lstCardMB[10]->getPositionX();
    auto orgX12 = this->lstCardMB[11]->getPositionX();
    auto orgX13 = this->lstCardMB[12]->getPositionX();
    
    auto orgY9 = this->lstCardMB[8]->getPositionY();
    auto orgY10 = this->lstCardMB[9]->getPositionY();
    auto orgY11 = this->lstCardMB[10]->getPositionY();
    auto orgY12 = this->lstCardMB[11]->getPositionY();
    auto orgY13 = this->lstCardMB[12]->getPositionY();
    
    int step = 10;
    this->lstCardMB[8]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX9, orgY9+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX9, orgY9)), NULL));
    this->lstCardMB[9]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX10, orgY10+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX10, orgY10)), NULL));
    this->lstCardMB[10]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX11, orgY11+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX11, orgY11)), NULL));
    this->lstCardMB[11]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX12, orgY12+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX12, orgY12)), NULL));
    this->lstCardMB[12]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX13, orgY13+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX13, orgY13)), NULL));
}
void ItemMenuGame::animationBaDoi(){
    this->pnlBaDoi->setVisible(true);
    this->lstCardBaDoi.clear();
    for (int i = 1; i <= 6;i++){
        auto img = static_cast<ImageView*>(this->pnlBaDoi->getChildByName(StringUtils::format("img%d",i)));
        this->lstCardBaDoi.push_back(img);
    }
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationBaDoi1, this)),DelayTime::create(0.7),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationBaDoi2, this)),DelayTime::create(0.9),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationBaDoi3, this)),DelayTime::create(1), NULL)));
    
}
void ItemMenuGame::animationCatte(){
    this->pnlCatte->setVisible(true);
    this->lstCardCatte.clear();
    
    for (int i = 1; i <= 3;i++){
        auto img = static_cast<ImageView*>(this->pnlCatte->getChildByName(StringUtils::format("img%d",i)));
        this->lstCardCatte.push_back(img);
    }
    
    this->runAction(RepeatForever::create(Sequence::create(CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationCatte1, this)),DelayTime::create(0.4),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::playAnimationCatte2, this)),DelayTime::create(1.4), NULL)));
    
    
}
void ItemMenuGame::playAnimationCatte1(){
    auto orgX = this->lstCardCatte[1]->getPositionX();
    auto orgY = this->lstCardCatte[1]->getPositionY();
    int step = 30;
    if (SceneManager::getSingleton().isNagaWin){
        step = 15;
    }
    
    this->lstCardCatte[1]->runAction(Sequence::create(MoveTo::create(0.5,Vec2(orgX, orgY+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX, orgY)), NULL));
}
void ItemMenuGame::playAnimationCatte2(){
    auto orgX0 = this->lstCardCatte[0]->getPositionX();
    auto orgX2 = this->lstCardCatte[2]->getPositionX();
    
    auto orgY0 = this->lstCardCatte[0]->getPositionY();
    auto orgY2 = this->lstCardCatte[2]->getPositionY();
    int step = 30;
    if (SceneManager::getSingleton().isNagaWin){
        step = 15;
    }
    this->lstCardCatte[0]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX0, orgY0+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX0, orgY0)), NULL));
    this->lstCardCatte[2]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX2, orgY2+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX2, orgY2)), NULL));
    
}
void ItemMenuGame::playAnimationBaDoi1(){
    auto orgX1 = this->lstCardBaDoi[0]->getPositionX();
    auto orgX2 = this->lstCardBaDoi[1]->getPositionX();
    
    auto orgY1 = this->lstCardBaDoi[0]->getPositionY();
    auto orgY2 = this->lstCardBaDoi[1]->getPositionY();
    int step = 30;
    if (SceneManager::getSingleton().isNagaWin){
        step = 15;
    }
    this->lstCardBaDoi[0]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX1, orgY1+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX1, orgY1)), NULL));
    this->lstCardBaDoi[1]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX2, orgY2+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX2, orgY2)), NULL));
}
void ItemMenuGame::playAnimationBaDoi2(){
    auto orgX3 = this->lstCardBaDoi[2]->getPositionX();
    auto orgX4 = this->lstCardBaDoi[3]->getPositionX();
    
    auto orgY3 = this->lstCardBaDoi[2]->getPositionY();
    auto orgY4 = this->lstCardBaDoi[3]->getPositionY();
    int step = 30;
    this->lstCardBaDoi[2]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX3, orgY3+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX3, orgY3)), NULL));
    this->lstCardBaDoi[3]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX4, orgY4+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX4, orgY4)), NULL));
}
void ItemMenuGame::playAnimationBaDoi3(){
    auto orgX5 = this->lstCardBaDoi[4]->getPositionX();
    auto orgX6 = this->lstCardBaDoi[5]->getPositionX();
    
    auto orgY5 = this->lstCardBaDoi[4]->getPositionY();
    auto orgY6 = this->lstCardBaDoi[5]->getPositionY();
    int step = 30;
    this->lstCardBaDoi[4]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX5, orgY5+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX5, orgY5)), NULL));
    this->lstCardBaDoi[5]->runAction(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX6, orgY6+step)),DelayTime::create(0.3),MoveTo::create(0.5,Vec2(orgX6, orgY6)), NULL));
}
void ItemMenuGame::updateHuTX(float dt){
    this->huTX->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huTaiXiu));
}
void ItemMenuGame::animationTX(){
    
    if (SceneManager::getSingleton().isNagaWin){
        this->pnlTX->setVisible(true);
        
        this->sprite = (Sprite*)this->pnlTX->getChildByName("sprAnim");
        this->animFrames.clear();
        string str = "";
        for (int i = 0; i <= 20; i++) {
            str = StringUtils::format("icongame-taixiu-anim_%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 247, 281));
            this->animFrames.pushBack(spriteFrame);
        }
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        Animate* animN = Animate::create(animation);
        this->sprite->runAction(RepeatForever::create(Sequence::create(animN,DelayTime::create(0.5),NULL)));
        return;
    }
    this->pnlTX->setVisible(true);
    this->imgKQ = static_cast<ImageView*>(this->pnlTX->getChildByName("imgKQ"));
    this->txtKQ= static_cast<Text*>(this->imgKQ->getChildByName("txtKQ"));
    this->imgdice1= static_cast<ImageView*>(this->pnlTX->getChildByName("imgdice1"));
    this->imgdice2= static_cast<ImageView*>(this->pnlTX->getChildByName("imgdice2"));
    this->imgdice3= static_cast<ImageView*>(this->pnlTX->getChildByName("imgdice3"));
    this->imgBat= static_cast<ImageView*>(this->pnlTX->getChildByName("imgBat"));
    this->imgTimer= static_cast<ImageView*>(this->pnlTX->getChildByName("imgTimer"));
    this->txtTimer= static_cast<Text*>(this->imgTimer->getChildByName("txtTimer"));
    this->huTX = static_cast<Text*>(this->pnlTX->getChildByName("txtHu"));
    this->schedule(schedule_selector(ItemMenuGame::updateHuTX));
    if (SceneManager::getSingleton().isLogin){
        this->huTX->setString("---");
        this->mQueueMsg = new ThreadQueueMsg();
        this->mQueueMsg->setFunCallBack(this, callfunc_selector(ItemMenuGame::OnExtensionResponseProcess));
        this->schedule(schedule_selector(ItemMenuGame::updateEvent));
        //this->schedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton),1);
        GameServer::getSingleton().addListeners(this);
        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutInt("tm",0);
        boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
        GameServer::getSingleton().Send(req);
        this->imgTimer->setVisible(false);
    }else{
        this->huTX->setString("999,999,999");
        this->imgBat->setVisible(false);
        this->imgTimer->setVisible(false);
        this->txtTimer->setString("");
        this->imgKQ->setVisible(true);
        this->txtKQ->setString(StringUtils::format("%s : %d", dataManager.GetSysString(709).c_str(),3));
        RepeatForever* action = RepeatForever::create(Sequence::create(DelayTime::create(1),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::animTXBeforeLogin, this)),DelayTime::create(10), NULL));
        action->setTag(1523);
        this->runAction(action);
    }
    
}
void ItemMenuGame::isLogon(){
    if (SceneManager::getSingleton().isNagaWin){
        return;
    }
    this->_isLogon = true;
    if (this->gameID == 128){
        this->stopActionByTag(1523);
        this->pnlTX->removeChildByTag(TAG_ANIM_END_GAME);

        this->imgBat->setVisible(true);
        this->imgTimer->setVisible(true);
        this->txtTimer->setString("");
        this->imgKQ->setVisible(false);
        this->huTX->setString("---");
        this->mQueueMsg = new ThreadQueueMsg();
        this->mQueueMsg->setFunCallBack(this, callfunc_selector(ItemMenuGame::OnExtensionResponseProcess));
        this->schedule(schedule_selector(ItemMenuGame::updateEvent));
        //this->schedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton), 1);
        GameServer::getSingleton().addListeners(this);
        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutInt("tm",0);
        boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
        GameServer::getSingleton().Send(req);
        this->imgTimer->setVisible(false);
    }else if (this->gameID == kGameSlot){
        this->huSlot1->setString("---");
        this->huSlot2->setString("---");
        this->huSlot3->setString("---");
        this->schedule(schedule_selector(ItemMenuGame::updateHuSlot));
    }
}
void ItemMenuGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    pObjs->PutUtfString("_cmd_", *cmd);
    
    if (0 == cmd->compare("txlrs")){
        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutInt("tm",0);
        boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
        GameServer::getSingleton().Send(req);
        return;
    }
    
    if(0 != cmd->compare(EXT_EVENT_START_TXMINI) && 0 != cmd->compare(EXT_EVENT_END_TXMINI) && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        return;
    }
    this->mQueueMsg->addQueue(pObjs);
}

void ItemMenuGame::OnExtensionResponseProcess(){
    this->param = this->mQueueMsg->frontQueue();
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
    
    if(0 == cmd->compare(EXT_EVENT_START_TXMINI)) {
        this->event_EXT_EVENT_START();
    } else if(0 == cmd->compare(EXT_EVENT_END_TXMINI)) {
        this->event_EXT_EVENT_END();
    } else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    this->mQueueMsg->deQueue();
}

void ItemMenuGame::event_EXT_EVENT_START(){
    int playerTime = *this->param->GetInt("pt");
    this->imgTimer->setVisible(true);
    this->imgTimer->stopAllActions();
    this->imgTimer->setOpacity(255);
    this->txtTimer->setVisible("");
    this->PlayTurnTime = playerTime / 1000;
    this->UpdateTimeStartGame();
    this->imgBat->setVisible(true);
    this->imgdice1->setVisible(false);
    this->imgdice2->setVisible(false);
    this->imgdice3->setVisible(false);
    this->imgKQ->setVisible(false);
    
    
}
void ItemMenuGame::UpdateTimeStartGame(){
    
    this->imgTimer->setVisible(true);
    this->StopMyTimer();
    this->TimerCountDown = this->PlayTurnTime;
    this->schedule(schedule_selector(ItemMenuGame::updateTimer),1);
    
}
void ItemMenuGame::updateTimer(float dt) {
    this->TimerCountDown-=1;
    if (this->TimerCountDown < 0) {
        this->StopMyTimer();
        return;
    }
    if(this->TimerCountDown <=5){
        this->imgTimer->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3),FadeIn::create(0.3),NULL)));
    }
    string str = StringUtils::format("%d",this->TimerCountDown);
    if (this->txtTimer != NULL) {
        this->txtTimer->setString(str);
        this->imgTimer->setVisible(true);
    }
    
    if (this->TimerCountDown == 0) {
        // this.imgBGTimer.setVisible(false);
        this->StopMyTimer();
    }
}
void ItemMenuGame::StopMyTimer() {
    this->unschedule(schedule_selector(ItemMenuGame::updateTimer));
    //this.txtTimer.setVisible(false);
    this->TimerCountDown = this->PlayTurnTime;
}
void ItemMenuGame::event_EXT_EVENT_END(){
    this->StopMyTimer();
    this->_isLogon = false;
    boost::shared_ptr<string> rg= this->param->GetUtfString(EXT_FIELD_RESULT_GAME);
    this->rg = *rg;
    this->imgBat->setVisible(false);
    this->imgKQ->setVisible(false);
    this->imgTimer->setVisible(false);
    this->imgdice1->setVisible(false);
    this->imgdice2->setVisible(false);
    this->imgdice3->setVisible(false);
    Size sizeAdd;
    //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    if (SceneManager::getSingleton().isNagaNew){
        Vec2 pos = Vec2(Vec2(this->imgBat->getPosition().x*0.7-940, this->imgBat->getPosition().y*0.7+15-sizeAdd.height*0.6-360));
        auto animatea = LayerAnimationTaiXiuMini::createInstance(pos);
        animatea->setScale(0.45);
        animatea->SetCallBackResult(this, callfuncO_selector(ItemMenuGame::CallBackResult));
        this->pnlTX->addChild(animatea, 999, TAG_ANIM_END_GAME);
        return;
    }
    Vec2 pos = Vec2(Vec2(this->imgBat->getPosition().x*0.7-270, this->imgBat->getPosition().y*0.7+15-sizeAdd.height*0.6));
    auto animatea = LayerAnimationTaiXiuMini::createInstance(pos);
    animatea->setScale(0.7);
    animatea->SetCallBackResult(this, callfuncO_selector(ItemMenuGame::CallBackResult));
    this->pnlTX->addChild(animatea, 999, TAG_ANIM_END_GAME);
}
void ItemMenuGame::isLogout(){
    if (SceneManager::getSingleton().isNagaWin) return;
    this->_isLogon = false;
    if (this->gameID == 128){
        this->unschedule(schedule_selector(ItemMenuGame::updateTimer));
        this->huTX->setString("999,999,999");
        this->imgBat->setVisible(false);
        this->imgTimer->setVisible(false);
        this->txtTimer->setString("");
        this->imgKQ->setVisible(true);
        this->stopAllActions();
        this->txtKQ->setString(StringUtils::format("%s : %d", dataManager.GetSysString(709).c_str(),3));
        RepeatForever* action = RepeatForever::create(Sequence::create(DelayTime::create(1),CallFunc::create(CC_CALLBACK_0(ItemMenuGame::animTXBeforeLogin, this)),DelayTime::create(10), NULL));
        action->setTag(1523);
        this->runAction(action);
    }else if (this->gameID == kGameSlot){
        this->huSlot1->setString("9,999,999");
        this->huSlot2->setString("9,999,999");
        this->huSlot3->setString("9,999,999");
        this->unschedule(schedule_selector(ItemMenuGame::updateHuSlot));
    }
}
void ItemMenuGame::animTXBeforeLogin(){
    if (this->_isLogon) return;
    this->rg = "1-1-1";
    this->imgBat->setVisible(false);
    this->imgKQ->setVisible(false);
    this->imgTimer->setVisible(false);
    this->imgdice1->setVisible(false);
    this->imgdice2->setVisible(false);
    this->imgdice3->setVisible(false);
    Size sizeAdd;
    //  if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
    sizeAdd = ChanUtils::getSizePos();
    if (SceneManager::getSingleton().isNagaNew){
        Vec2 pos = Vec2(Vec2(this->imgBat->getPosition().x*0.7-940, this->imgBat->getPosition().y*0.7+15-sizeAdd.height*0.6-360));
        auto animatea = LayerAnimationTaiXiuMini::createInstance(pos);
        animatea->setScale(0.5);
        animatea->SetCallBackResult(this, callfuncO_selector(ItemMenuGame::CallBackResult));
        this->pnlTX->addChild(animatea, 999, TAG_ANIM_END_GAME);
        return;
    }
    
    Vec2 pos = Vec2(Vec2(this->imgBat->getPosition().x*0.7-270, this->imgBat->getPosition().y*0.7+15));
    auto animatea = LayerAnimationTaiXiuMini::createInstance(pos);
    animatea->setScale(0.7);
    animatea->SetCallBackResult(this, callfuncO_selector(ItemMenuGame::CallBackResult));
    this->pnlTX->addChild(animatea, 999, TAG_ANIM_END_GAME);
}
void ItemMenuGame::CallBackResult(Ref *pSend){
    if (this->_isLogon) return;
    this->CallBackFromAnim();
}

void ItemMenuGame::CallBackFromAnim(){
    
    
    this->showResult();
    this->imgTimer->setVisible(false);
    
    this->runAction(Sequence::create(DelayTime::create(3), CallFunc::create(CC_CALLBACK_0(ItemMenuGame::showResultAfter, this)), NULL));
    
}
void ItemMenuGame::showResultAfter(){
    
}
void ItemMenuGame::showResultGame(string money, string result){
    
    
}
string ItemMenuGame::GetUrlXiNgau(int number) {
    string num = "";
    switch (number){
        case 1:
            num = "dice-01-black.png";
            break;
        case 2:
            num = "dice-02-black.png";
            break;
        case 3:
            num = "dice-03-black.png";
            break;
        case 4:
            num = "dice-04-black.png";
            break;
        case 5:
            num = "dice-05-black.png";
            break;
        case 6:
            num = "dice-06-black.png";
            break;
    }
    return num;
}
void ItemMenuGame::showResult(){
    vector<string> arrRg = mUtils::splitString(this->rg, '-');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    this->imgBat->setVisible(false);
    string url = this->GetUrlXiNgau(atoi(arrRg.at(0).c_str()));
    this->imgdice1->loadTexture(url);
    this->imgdice1->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(1).c_str()));
    this->imgdice2->loadTexture(url);
    this->imgdice2->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(2).c_str()));
    this->imgdice3->loadTexture(url);
    this->imgdice3->setVisible(true);
    string kq = "";
    if (total>=11)
        kq = dataManager.GetSysString(671);
    else
        kq = dataManager.GetSysString(709);
    this->imgKQ->setVisible(true);
    this->txtKQ->setString(StringUtils::format("%s : %d", kq.c_str(),total));
}
void ItemMenuGame::showPrevMatchHis(string str){
    vector<string> arrRg = mUtils::splitString(str, '_');
    
    if (arrRg.size() < 3)
        return;
    int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
    
    this->imgBat->setVisible(false);
    string url = this->GetUrlXiNgau(atoi(arrRg.at(0).c_str()));
    this->imgdice1->loadTexture(url);
    this->imgdice1->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(1).c_str()));
    this->imgdice2->loadTexture(url);
    this->imgdice2->setVisible(true);
    
    url = this->GetUrlXiNgau(atoi(arrRg.at(2).c_str()));
    this->imgdice3->loadTexture(url);
    this->imgdice3->setVisible(true);
    
    
    string kq = "";
    if (total>=11)
        kq = dataManager.GetSysString(671);
    else
        kq = dataManager.GetSysString(709);
    this->imgKQ->setVisible(true);
    this->txtKQ->setString(StringUtils::format("%s : %d", kq.c_str(),total));
    
}
void ItemMenuGame::event_EXT_EVENT_GAME_STATE_NTF(){
    boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
    boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
    boost::shared_ptr<string> ghs = this->param->GetUtfString("ghs");
    
    if (gstate != NULL) {
        if (0 == *gstate) {
            this->imgTimer->setVisible(false);
            if (ghs != NULL){
                if (0 != strcmp(ghs->c_str(), "")){
                    
                    auto arr = mUtils::splitString(*ghs, '|');
                    
                    if (arr.size() == 14){
                        this->showPrevMatchHis(arr[3]);
                    }
                }
            }
            
        } else {
            // this->CurrentGameState = true;
            
            this->StopMyTimer();
            //Update timer
            this->TimerCountDown = *gtime;
            this->imgTimer->setVisible(true);
            this->imgTimer->stopAllActions();
            this->imgTimer->setOpacity(255);
            this->txtTimer->setVisible(true);
            this->imgBat->setVisible(true);
            this->imgKQ->setVisible(false);
            
            // this.schedule(this.UpdateHayDatcuoc, 0.5)
            this->schedule(schedule_selector(ItemMenuGame::updateTimer),1);
        }
    }
}
void ItemMenuGame::updateEvent(float dt){
    if(this->mQueueMsg->isQueueEmpty())
        return;
    
    this->mQueueMsg->run();
}
void ItemMenuGame::animationSLot(){
    this->pnlSlot->setVisible(true);

    if (SceneManager::getSingleton().isNagaWin){
        auto star1 = static_cast<Sprite*>(this->pnlSlot->getChildByName("star1"));
        auto star3 = static_cast<Sprite*>(this->pnlSlot->getChildByName("star3"));

        auto icon7 = static_cast<Sprite*>(this->pnlSlot->getChildByName("icon7"));
        star1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1),DelayTime::create(3.3), NULL)));
        icon7->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1),ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1),DelayTime::create(2.8), NULL)));
        star3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.5),ScaleTo::create(0.2, 1.2),ScaleTo::create(0.2, 1),DelayTime::create(2.3), NULL)));

        return;
    }
    this->imgAnim = static_cast<ImageView*>(this->pnlSlot->getChildByName("imgAnim"));
    this->imgAnim->setVisible(false);
    this->sprite = Sprite::create("anim-slot-1.png");
    this->sprite->setPosition(this->imgAnim->getPosition());
    this->sprite->setAnchorPoint(this->imgAnim->getAnchorPoint());
    this->huSlot1 = static_cast<Text*>(this->pnlSlot->getChildByName("txtHu1"));
    this->huSlot2 = static_cast<Text*>(this->pnlSlot->getChildByName("txtHu2"));
    this->huSlot3 = static_cast<Text*>(this->pnlSlot->getChildByName("txtHu3"));
    if (SceneManager::getSingleton().isLogin){
        this->huSlot1->setString("---");
        this->huSlot2->setString("---");
        this->huSlot3->setString("---");
        this->schedule(schedule_selector(ItemMenuGame::updateHuSlot));

    }else{
        this->huSlot1->setString("9,999,999");
        this->huSlot2->setString("9,999,999");
        this->huSlot3->setString("9,999,999");
    }

    
    
    this->pnlSlot->addChild(sprite);
    this->animFrames.clear();
    string str = "";
    for (int i = 1; i <= 4; i++) {
        str = StringUtils::format("anims/anim-slot-%d-new.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 301, 215));
        this->animFrames.pushBack(spriteFrame);
    }
    
    for (int i = 2; i <= 7; i++) {
        str = StringUtils::format("anims/anim-slot-%d-new.png",i);
        auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 301, 215));
        this->animFrames.pushBack(spriteFrame);
    }
    
    
    
    Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
    Animate* animN = Animate::create(animation);
    this->sprite->runAction(RepeatForever::create(Sequence::create(animN,DelayTime::create(1.5),NULL)));
    
    
}
void ItemMenuGame::updateHuSlot(float dt){
    this->huSlot1->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().currHuSlot1));
    this->huSlot2->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().currHuSlot2));
    this->huSlot3->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().currHuSlot3));
    
}
void ItemMenuGame::OnSmartFoxSubscribeRoomGroup(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    int gameID = kGameSlot;
    //update 2/8
    if (gameID == kGameSlot){
       // log("OnSmartFoxSubscribeRoomGroup Game ID = %d", gameID);
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
       // log("list room : %d", list_room->size());
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = *room->GetVariable("roomType")->GetIntValue();
                    if (roomType == 0){
                        string jpvl = "";
                        if (room->GetVariable("jpvl")){
                        if (room->GetVariable("jpvl")->GetStringValue())
                            jpvl = *room->GetVariable("jpvl")->GetStringValue();
                        }
                        string betlist = "";
                        if (room->GetVariable("betlist")->GetStringValue())
                            betlist = *room->GetVariable("betlist")->GetStringValue();
                        
                        auto lstbet = mUtils::splitString(betlist, ',');
                        
                        //50000-195000|500-1050|5000-12550
                        auto lst = mUtils::splitString(jpvl, '|');
                        for (int i = 0 ; i < lst.size() ; i++){
                            auto it = mUtils::splitString(lst[i], '-');
                            if (atoi(it[0].c_str())==atoi(lstbet[0].c_str())){
                                SceneManager::getSingleton().currHuSlot1 = (atoi(it[1].c_str()));
                            }
                            if (atoi(it[0].c_str())==atoi(lstbet[1].c_str())){
                                SceneManager::getSingleton().currHuSlot2 = (atoi(it[1].c_str()));
                                
                            }
                            if (atoi(it[0].c_str())==atoi(lstbet[2].c_str())){
                                SceneManager::getSingleton().currHuSlot3 = (atoi(it[1].c_str()));
                                
                            }
                        }
                    }
                }
            }
        }
    }
}

void ItemMenuGame::OnSmartFoxSubscribeRoomGroupError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    int gameID = SceneManager::getSingleton().getGameID();
    if (gameID == kGameSlot)
    {
        boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
        boost::shared_ptr<void> ptrEventParamValueErrorCode = (*ptrEventParams)["errorCode"];
        boost::shared_ptr<short> ptrErrorCode = ((boost::static_pointer_cast<short>))(ptrEventParamValueErrorCode);
        //
       // log("OnSmartFoxSubscribeRoomGroupError: %s", mUtils::getErrorSmartfoxExplain(*ptrErrorCode).c_str());
        if (*ptrErrorCode == 24){
            if (gameID == kGameSlot){
               // log("OnSmartFoxSubscribeRoomGroup Game ID = %d", gameID);
                boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(gameID));
               // log("list room : %d", list_room->size());
                for (int i = 0; i < list_room->size(); i++)
                {
                    boost::shared_ptr<Room> room = list_room->at(i);
                    if (room != NULL)
                    {
                        if (room->GetVariable("roomType") != NULL){
                            int roomType = *room->GetVariable("roomType")->GetIntValue();
                            if (roomType == 0){
                                string jpvl = "";
                                if (room->GetVariable("jpvl")){
                                    if (room->GetVariable("jpvl")->GetStringValue())
                                    jpvl = *room->GetVariable("jpvl")->GetStringValue();
                            }
                                string betlist = "";
                                if (room->GetVariable("betlist")->GetStringValue())
                                    betlist = *room->GetVariable("betlist")->GetStringValue();
                                
                                auto lstbet = mUtils::splitString(betlist, ',');
                                
                                //50000-195000|500-1050|5000-12550
                                auto lst = mUtils::splitString(jpvl, '|');
                                for (int i = 0 ; i < lst.size() ; i++){
                                    auto it = mUtils::splitString(lst[i], '-');
                                    if (atoi(it[0].c_str())==atoi(lstbet[0].c_str())){
                                        if (this->huSlot1)
                                            SceneManager::getSingleton().currHuSlot1 = (atoi(it[1].c_str()));
                                    }
                                    if (atoi(it[0].c_str())==atoi(lstbet[1].c_str())){
                                        SceneManager::getSingleton().currHuSlot2 = (atoi(it[1].c_str()));
                                        
                                    }
                                    if (atoi(it[0].c_str())==atoi(lstbet[2].c_str())){
                                        SceneManager::getSingleton().currHuSlot3 = (atoi(it[1].c_str()));
                                        
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
void ItemMenuGame::animationTLMN(){
    this->pnlTLMN->setVisible(true);

    if (SceneManager::getSingleton().isNagaWin){
        auto img1 = (Sprite*)this->pnlTLMN->getChildByName("img1");
        auto orgX1 = img1->getPositionX();
        auto orgY1 = img1->getPositionY();

        auto img2 = (Sprite*)this->pnlTLMN->getChildByName("img2");
        
        auto orgX2 = img2->getPositionX();
        auto orgY2 = img2->getPositionY();
        
        auto img3 = (Sprite*)this->pnlTLMN->getChildByName("img3");
        auto orgX3 = img3->getPositionX();
        auto orgY3 = img3->getPositionY();
        auto img4 = (Sprite*)this->pnlTLMN->getChildByName("img4");
        auto orgX4 = img4->getPositionX();
        auto orgY4 = img4->getPositionY();
        int step =25;
        img1->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX1 , orgY1+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX1, orgY1)),DelayTime::create(10.5), NULL)));
        
        img2->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX2 , orgY2+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX2, orgY2)),DelayTime::create(10.5), NULL)));

        img3->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX3 , orgY3+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX3, orgY3)),DelayTime::create(10.5), NULL)));

        img4->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.5),MoveTo::create(0.5,Vec2(orgX4 , orgY4+step)),DelayTime::create(0.1),MoveTo::create(0.5,Vec2(orgX4, orgY4)),DelayTime::create(10.5), NULL)));

        return;
    }
    
    
    this->lstCard.clear();
    
    for (int i = 1; i <= 5;i++){
        auto img = static_cast<ImageView*>(this->pnlTLMN->getChildByName(StringUtils::format("img%d",i)));
        this->lstCard.push_back(img);
    }
    for (int i = 0 ; i <this->lstCard.size();i++){
        float newCornor = -((this->lstCard[i]->getPositionX()) / 2 * 12) + 12 * i-100;
        float rad = newCornor * 3.14 / 180+i*15-5;
        
        RotateTo *rotateTo1 = RotateTo::create(1.5, rad);
        RotateTo *rotateTo2 = RotateTo::create(1.5, newCornor * 3.14 / 180+i+15);
        
        int delaytime = 0;
        if (i ==0 || i == 4)
            delaytime = 0;
        else if (i ==1 || i == 3)
            delaytime = 0.75;
        else if (i==2)
            delaytime =0.75;
        
        int delaytime2 = 0;
        if (i ==0 || i == 4)
            delaytime2 = 0.1;
        else if (i ==1 || i == 3)
            delaytime2 = 0.1;
        else if (i==2)
            delaytime2 =0.1;
        
        this->lstCard[i]->runAction(RepeatForever::create(Sequence::create(DelayTime::create(delaytime),rotateTo1,DelayTime::create(delaytime2),rotateTo2,DelayTime::create(0.1), NULL)));
    }
    
}
void ItemMenuGame::onEnter()
{
    Layer::onEnter();
}

void ItemMenuGame::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    try {
        this->stopAllActions();
        if (this->sprite){
            this->sprite->stopAllActions();
        }
        if (this->sprTom){
            this->sprTom->stopAllActions();
        }
        if (this->sprCua){
            this->sprCua->stopAllActions();
        }
        if (this->sprCa){
            this->sprCa->stopAllActions();
        }
        if (this->sprBubble){
            this->sprBubble->stopAllActions();
        }
        this->unschedule(schedule_selector(ItemMenuGame::updateHuSlot));
        if (this->getChildByTag(99))
            this->removeChildByTag(99);
        GameServer::getSingleton().removeListeners(this);
        for (int i = 0 ; i < this->lstCard.size();i++){
            this->lstCard[i]->stopAllActions();
        }
        for (int i = 0 ; i < this->lstCardCatte.size() ; i++){
            this->lstCardCatte[i]->stopAllActions();
        }
        for (int i = 0 ; i < this->lstCardBaDoi.size() ; i++){
            this->lstCardBaDoi[i]->stopAllActions();
        }
        for (int i = 0 ; i < this->lstCardMB.size() ; i++){
            this->lstCardMB[i]->stopAllActions();
        }
        this->lstCard.clear();
        this->lstCardCatte.clear();
        this->lstCardBaDoi.clear();
        this->lstCardMB.clear();
        this->animFrames.clear();
    }catch(...){
       // log("eo biet nguyen nhan!");
    }
  
}

void ItemMenuGame::showNewGame(int gameID, vector<int> listNewGame){
    ImageView* imgNewGame = static_cast<ImageView*>(this->pnlNewGame->getChildByName("imgNewGame"));
    
    if (listNewGame.size() == 0){
        return;
    }else{
        for(int i = 0; i< listNewGame.size(); i++){
            if (gameID == listNewGame.at(i)) {
                imgNewGame->setVisible(true);
                return;
            }
        }
    }
}
