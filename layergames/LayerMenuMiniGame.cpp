//
//  LayerMenuMiniGame.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#include "LayerMenuMiniGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerExpandMiniMenu.h"
#include "../SceneManager.h"
#include "ChanUtils.h"
#include "TaiXiuMessageDefs.h"
#include "PhomMessDef.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerMenuMiniGame::LayerMenuMiniGame()
{
    
}


LayerMenuMiniGame::~LayerMenuMiniGame()
{
    
}

bool LayerMenuMiniGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerMenuMiniGame.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
//    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
//        sizeAdd = ChanUtils::getSizePos();
//        
//    }else{
//        sizeAdd = Size(0,0);
//    }
//    //rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd.height));
	ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->menu = static_cast<ImageView*>(rootNode->getChildByName("imgIcon"));
    auto btnOpen = static_cast<Button*>(rootNode->getChildByName("btnOpen"));
    btnOpen->addClickEventListener(CC_CALLBACK_1(LayerMenuMiniGame::onBtnOpen, this));
    //this->menu->setPositionX(sizeAdd.width);
   // this->menu->setPosition(Vec2(menu->getPositionX(),menu->getPositionY()-100));
    //this->menu->setAnchorPoint(Vec2(0.5,0.5));
    //log("x : y: %f%f",menu->getPositionX(),menu->getPositionY());
    this->imgketqua= static_cast<ImageView*>(rootNode->getChildByName("imgketqua"));
    this->imgketqua->setVisible(false);

    this->imgBGTimer = static_cast<ImageView*>(this->menu->getChildByName("imgBGTimer"));
    this->txtTimer = static_cast<Text*>(this->imgBGTimer->getChildByName("txtTimer"));
    this->txtTimer->setString("");
    this->imgBGTimer->setVisible(false);
    this->setContentSize(menu->getContentSize());
    
    return true;
}
void LayerMenuMiniGame::removeListenerServer(){
    GameServer::getSingleton().removeListeners(this);
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    boost::shared_ptr<IRequest> req (new ExtensionRequest("txl", params));
    GameServer::getSingleton().Send(req);
    this->imgBGTimer->setVisible(false);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::updateTimer),this);
    this->setVisible(false);
}
void LayerMenuMiniGame::addListenerServer(){
    this->setVisible(true);
    GameServer::getSingleton().addListeners(this);
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    params->PutInt("tm",0);
    boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
    GameServer::getSingleton().Send(req);
}
void LayerMenuMiniGame::onTimerToHideButton(float dt){
    this->timerToHide--;
    if(this->timerToHide == 0) {
        if (this->isShow){
            this->menu->setOpacity(100);
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton),this);
            auto x = this->menu->getPositionX();
            auto y = this->menu->getPositionY();
            if (x < WIDTH_DESIGN / 2)
                x -= 90;
            else
                x += 90;
            this->isShow = false;
            this->isHaveHiden = true;
            this->menu->runAction(MoveTo::create(0.5,Vec2(x,y)));
        }
    }
}
void LayerMenuMiniGame::startWaittoHide(){
    if (this->isShow) {
        this->timerToHide = 1;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton),this,1, false);
        if (this->isHaveHiden) {
            auto x = this->menu->getPositionX();
            auto y = this->menu->getPositionY();
            if (x < WIDTH_DESIGN / 2)
                x += 90;
            else
                x -= 90;
            this->menu->runAction(MoveTo::create(0.5, Vec2(x, y)));
            this->isShow = true;
        }
    }
}
void LayerMenuMiniGame::stopWaitWhenTouch(){
    if (!this->isShow) {
        this->menu->setOpacity(255);
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton),this);
//        auto x = this->menu->getPositionX();
//        auto y = this->menu->getPositionY();
//        if (x < WIDTH_DESIGN / 2)
//            x += 90;
//        else
//            x -= 90;
//        this->menu->runAction(MoveTo::create(0.5, Vec2(x, y)));
        this->isShow = true;
        this->isHaveHiden = false;
    }
}

void LayerMenuMiniGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    this->param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if(0 == cmd->compare(EXT_EVENT_START_TXMINI)) {
        this->event_EXT_EVENT_START();
    } else if(0 == cmd->compare(EXT_EVENT_END_TXMINI)) {
        this->event_EXT_EVENT_END();
    } else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
}

void LayerMenuMiniGame::OnExtensionResponseProcess(){
    boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
    
    if(0 == cmd->compare(EXT_EVENT_START_TXMINI)) {
        this->event_EXT_EVENT_START();
    } else if(0 == cmd->compare(EXT_EVENT_END_TXMINI)) {
        this->event_EXT_EVENT_END();
    } else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
}

void LayerMenuMiniGame::event_EXT_EVENT_START(){
    long playerTime = *this->param->GetInt("pt");
    this->txtTimer->setVisible("");
    this->PlayTurnTime = playerTime / 1000;
    this->UpdateTimeStartGame();
    this->imgketqua->setVisible(false);

    
}
void LayerMenuMiniGame::UpdateTimeStartGame(){
    this->imgBGTimer->setVisible(true);
    this->imgBGTimer->stopAllActions();
    this->imgBGTimer->setOpacity(255);
    this->txtTimer->setVisible(true);
    this->StopMyTimer();
    this->TimerCountDown = this->PlayTurnTime;
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMenuMiniGame::updateTimer),this,1, false);
    
}
void LayerMenuMiniGame::updateTimer(float dt) {
    this->TimerCountDown-=1;
    if (this->TimerCountDown < 0) {
        this->StopMyTimer();
        return;
    }
    if(this->TimerCountDown <=5){
        this->imgBGTimer->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.3),FadeIn::create(0.3),NULL)));
    }
    string str = StringUtils::format("%d",this->TimerCountDown);
    if (this->txtTimer != NULL) {
        this->txtTimer->setString(str);
        this->imgBGTimer->setVisible(true);
    }
    
    if (this->TimerCountDown == 0) {
        // this.imgBGTimer.setVisible(false);
        this->StopMyTimer();
    }
}
void LayerMenuMiniGame::StopMyTimer() {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::updateTimer),this);
    
    //this.txtTimer.setVisible(false);
    this->TimerCountDown = this->PlayTurnTime;
}
void LayerMenuMiniGame::event_EXT_EVENT_END(){
    this->StopMyTimer();
    boost::shared_ptr<string> rg= this->param->GetUtfString(EXT_FIELD_RESULT_GAME);
        vector<string> arrRg = mUtils::splitString(*rg, '-');
        
        if (arrRg.size() < 3)
            return;
        int total = atoi(arrRg.at(0).c_str()) + atoi(arrRg.at(1).c_str()) + atoi(arrRg.at(2).c_str());
        if (!SceneManager::getSingleton().getIsShowTX()){
            this->imgketqua->setVisible(true);
            if (total>=11)
                this->imgketqua->loadTexture(StringUtils::format("CommonRes/%smoc3-showTai.png",SceneManager::getSingleton().getFolderLang().c_str()));
            else
                this->imgketqua->loadTexture(StringUtils::format("CommonRes/%smoc3-showXiu.png",SceneManager::getSingleton().getFolderLang().c_str()));
            
        }
}
void LayerMenuMiniGame::event_EXT_EVENT_GAME_STATE_NTF(){
    boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
    boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
    if (gstate != NULL) {
        if (0 == *gstate) {
            this->imgBGTimer->setVisible(false);
            
        } else {
            // this->CurrentGameState = true;
            
            this->StopMyTimer();
            //Update timer
            this->TimerCountDown = *gtime;
            this->imgBGTimer->setVisible(true);
            this->imgBGTimer->stopAllActions();
            this->imgBGTimer->setOpacity(255);
            this->txtTimer->setVisible(true);
            
            // this.schedule(this.UpdateHayDatcuoc, 0.5)
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMenuMiniGame::updateTimer),this,1, false);
        }
    }
}
void LayerMenuMiniGame::updateEvent(float dt){
   
}
void LayerMenuMiniGame::onBtnOpen(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    //    layerPlayerGame = LayerPlayGameTXMini::create();
    //    this->addChild(layerPlayerGame);
    //    this->imgBGTimer->setVisible(false);
    //    this.btnOpen.setEnabled(false);
    if (SceneManager::getSingleton().getIsShowTX())
        return;
    LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
    _currScene->addChild(layerPlayerGame,1000,9011);
    SceneManager::getSingleton().setIsShowTX(true);
    
}
bool LayerMenuMiniGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto locationInNode = pTouch->getLocation();
    //var s = target.getContentSize();
    auto rectNode = this->menu->getBoundingBox();//Rect(this->menu->getPosition().x, this->menu->getPosition().y, 143,149);
    Size sizeAdd;
    
    sizeAdd = ChanUtils::getSizePos();
    
    auto rect = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y + sizeAdd.height, rectNode.size.width, rectNode.size.height);
    //listener->setSwallowTouches(false);
    if (rect.containsPoint(locationInNode)) {
        this->isTouchMove = false;
        if (!this->isShow)
            this->stopWaitWhenTouch();
        return true;
    }
    return false;
}
void LayerMenuMiniGame::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    auto target = this->menu;
    auto delta = pTouch->getDelta();
    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
    //     return;
    auto x = target->getPositionX() + delta.x;
    auto y = target->getPositionY() + delta.y;
    
    this->menu->setPosition(Vec2(x,y));
    if (abs(delta.x) > 10 || abs(delta.y) > 10)
        this->isTouchMove =true;
    //listener->setSwallowTouches(true);
}
void LayerMenuMiniGame::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
    Size sizeAdd;
    
    sizeAdd = ChanUtils::getSizePos();
    
    if (this->menu->getPositionX()>visibleSize.width/2)
    {
        auto posY = this->menu->getPositionY();;
        if(this->menu->getPositionY()>visibleSize.height-this->menu->getContentSize().height-sizeAdd.height){
            posY = visibleSize.height-this->menu->getContentSize().height-sizeAdd.height;
        }else if(this->menu->getPositionY()<(visibleSize.height-this->menu->getContentSize().height+50) && this->menu->getPositionY()> this->menu->getContentSize().height){
            posY = this->menu->getPositionY();
        }
        else{
            posY = -sizeAdd.height;
        }
        this->menu->runAction(MoveTo::create(0.2,Vec2(visibleSize.width - this->menu->getContentSize().width,posY)));
        
    }
    if (this->menu->getPositionX()<visibleSize.width/2)
    {
        auto posY = this->menu->getPositionY();;
        if(this->menu->getPositionY()>visibleSize.height-this->menu->getContentSize().height){
            posY = visibleSize.height-this->menu->getContentSize().height-sizeAdd.height;
        }else if(this->menu->getPositionY()<(visibleSize.height-this->menu->getContentSize().height+50) && this->menu->getPositionY()> this->menu->getContentSize().height){
            posY = this->menu->getPositionY();
        }
        else{
            posY = -sizeAdd.height;
        }
        this->menu->runAction(MoveTo::create(0.2,Vec2(0,posY)));
    }
    //listener->setSwallowTouches(false);
    //  self.menu.btnOpen.setEnabled(true);
    if(!this->isTouchMove){
        this->onBtnOpen(NULL);
        
    }
    if (this->isShow)
        this->startWaittoHide();
}
void LayerMenuMiniGame::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerMenuMiniGame::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(LayerMenuMiniGame::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(LayerMenuMiniGame::onTouchEnded, this);
//    
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //GameServer::getSingleton().addListeners(this);
}

void LayerMenuMiniGame::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    GameServer::getSingleton().removeListeners(this);

    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::onTimerToHideButton),this);
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMenuMiniGame::updateTimer),this);


}


