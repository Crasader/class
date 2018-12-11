//
//  LayerGameTaiXiuMini.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 6/22/16.
//
//

#include "LayerGameTaiXiuMini.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerPlayGameTXMini.h"
#include "../SceneManager.h"
#include "ChanUtils.h"
#include "TaiXiuMessageDefs.h"
#include "PhomMessDef.h"
#include "LayerPlayGameMiniPoker.h"
#include "LayerCaoThapPopupGame.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerGameTaiXiuMini::LayerGameTaiXiuMini()
{
    
}


LayerGameTaiXiuMini::~LayerGameTaiXiuMini()
{
    
}

bool LayerGameTaiXiuMini::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerGameTaiXiuMini.csb");
    
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    Size sizeAdd = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd.height));    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    this->menu = static_cast<ImageView*>(rootNode->getChildByName("imgIcon"));
    this->menu->addClickEventListener(CC_CALLBACK_1(LayerGameTaiXiuMini::onBtnOpen, this));
    //this->menu->setAnchorPoint(Vec2(0.5,0.5));
   // log("x : y: %f%f",menu->getPositionX(),menu->getPositionY());
    layerPlayerGame = NULL;
    this->imgBGTimer = static_cast<ImageView*>(rootNode->getChildByName("imgBGTimer"));
    this->txtTimer = static_cast<Text*>(this->imgBGTimer->getChildByName("txtTimer"));
    this->txtTimer->setString("");
    this->imgBGTimer->setVisible(false);
	this->setContentSize(menu->getContentSize());
    
    this->mQueueMsg = new ThreadQueueMsg();
    
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameTaiXiuMini::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiuMini::updateEvent),this,1/60, false);
    
    GameServer::getSingleton().addListeners(this);
    
    
    ///Cheat
    
  
    
    
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
    GameServer::getSingleton().Send(req);

    
    
    return true;
}

void LayerGameTaiXiuMini::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    pObjs->PutUtfString("_cmd_", *cmd);
    if(0 == cmd->compare(EXT_EVENT_START_TXMINI)) {
        this->event_EXT_EVENT_START();
    } else if(0 == cmd->compare(EXT_EVENT_END_TXMINI)) {
        this->event_EXT_EVENT_END();
    } else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        this->event_EXT_EVENT_GAME_STATE_NTF();
    }
    this->mQueueMsg->addQueue(pObjs);
}

void LayerGameTaiXiuMini::OnExtensionResponseProcess(){
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

void LayerGameTaiXiuMini::event_EXT_EVENT_START(){
    long playerTime = *this->param->GetInt("pt");
    this->txtTimer->setVisible("");
    this->PlayTurnTime = playerTime / 1000;
    this->UpdateTimeStartGame();
    
}
void LayerGameTaiXiuMini::UpdateTimeStartGame(){
    if(!this->layerPlayerGame)
        this->imgBGTimer->setVisible(true);
        this->imgBGTimer->stopAllActions();
        this->imgBGTimer->setOpacity(255);
        this->txtTimer->setVisible(true);
        this->StopMyTimer();
        this->TimerCountDown = this->PlayTurnTime;
        
       Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiuMini::updateTimer),this,1, false);
    
}
void LayerGameTaiXiuMini::updateTimer(float dt) {
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
        if(!this->layerPlayerGame)
            this->imgBGTimer->setVisible(true);
    }
    
    if (this->TimerCountDown == 0) {
        // this.imgBGTimer.setVisible(false);
        this->StopMyTimer();
    }
}
void LayerGameTaiXiuMini::StopMyTimer() {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiuMini::updateTimer),this);

    //this.txtTimer.setVisible(false);
    this->TimerCountDown = this->PlayTurnTime;
}
void LayerGameTaiXiuMini::event_EXT_EVENT_END(){
    this->StopMyTimer();
}
void LayerGameTaiXiuMini::event_EXT_EVENT_GAME_STATE_NTF(){
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
            if(!this->layerPlayerGame)
                this->imgBGTimer->setVisible(true);
            this->imgBGTimer->stopAllActions();
            this->imgBGTimer->setOpacity(255);
            this->txtTimer->setVisible(true);
           
            // this.schedule(this.UpdateHayDatcuoc, 0.5)
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiuMini::updateTimer),this,1, false);
        }
    }
}
void LayerGameTaiXiuMini::updateEvent(float dt){
    if(this->mQueueMsg->isQueueEmpty())
        return;
    
    this->mQueueMsg->run();
}
void LayerGameTaiXiuMini::onBtnOpen(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
//    layerPlayerGame = LayerPlayGameTXMini::create();
//    this->addChild(layerPlayerGame);
//    this->imgBGTimer->setVisible(false);
    //    this.btnOpen.setEnabled(false);
    if (SceneManager::getSingleton().getIsShowTX())
        return;
    LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
    _currScene->addChild(layerPlayerGame,1000,9009);
    SceneManager::getSingleton().setIsShowTX(true);
    
}
bool LayerGameTaiXiuMini::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
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
        return true;
    }
    
    return false;
}
void LayerGameTaiXiuMini::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
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
void LayerGameTaiXiuMini::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
    Size sizeAdd;
    
    sizeAdd = ChanUtils::getSizePos();
    
    auto target = pEvent->getCurrentTarget();
    
    if (this->menu->getPositionX()>visibleSize.width/2)
    {
        auto posY = this->menu->getPositionY();
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
}
void LayerGameTaiXiuMini::onEnter()
{
    Layer::onEnter();
 //   auto dispatcher = Director::getInstance()->getEventDispatcher();
//    listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerGameTaiXiuMini::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(LayerGameTaiXiuMini::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(LayerGameTaiXiuMini::onTouchEnded, this);
//    
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerGameTaiXiuMini::onExit()
{
    Layer::onExit();
    SceneManager::getSingleton().setIsShowTX(false);

//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(listener);
//    Director::getInstance()->getScheduler()->unscheduleAll();
//    boost::shared_ptr<ISFSObject> params (new SFSObject());
//    boost::shared_ptr<IRequest> req (new ExtensionRequest("txl", params));
//    GameServer::getSingleton().Send(req);
//    GameServer::getSingleton().removeListeners(this);

    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}


