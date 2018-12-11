//
//  LayerLauncherMiniGame.cpp
//  iCasino_v4
//
//  Created by Nguyen Tuan Anh on 9/1/16.
//
//

#include "LayerLauncherMiniGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerExpandMiniMenu.h"
#include "../SceneManager.h"
#include "ChanUtils.h"
#include "TaiXiuMessageDefs.h"
#include "PhomMessDef.h"
#include "../layergames/LayerPlayGameMiniPoker.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "../layergames/LayerPlayGameDoDenMini.h"
#include "../layergames/LayerCaoThapPopupGame.h"
#include "../layergames/LayerMenuMiniGame.h"
#include "../layergames/LayerPlayVQMM.h"
#include "ChanUtils.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerLauncherMiniGame::LayerLauncherMiniGame()
{
    
}


LayerLauncherMiniGame::~LayerLauncherMiniGame()
{
    GameServer::getSingleton().removeListeners(this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this);

    if (this->mQueueMsg != NULL){
        delete this->mQueueMsg;
    }
}

bool LayerLauncherMiniGame::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerLaucherMiniGame.csb");
    
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
    Size sizeAdd = Size(0,0);
//    //rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd.height));
	ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    this->pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    if (SceneManager::getSingleton().getGameID()==kGamePoker || SceneManager::getSingleton().getGameID()==kGameSlot|| SceneManager::getSingleton().getGameID()==kGameMauBinh)
        this->pnlBg->setPositionY(visibleSize.height-this->pnlBg->getContentSize().height-150);
    this->pnlBg->setSwallowTouches(false);
    this->menu = static_cast<ImageView*>(this->pnlBg->getChildByName("imgIcon"));
    auto btnOpen = static_cast<Button*>(this->pnlBg->getChildByName("btnOpen"));
    btnOpen->addClickEventListener(CC_CALLBACK_1(LayerLauncherMiniGame::onBtnOpen, this));
    //this->menu->setPositionX(sizeAdd.width);
   // this->menu->setPosition(Vec2(menu->getPositionX(),menu->getPositionY()-100));
    //this->menu->setAnchorPoint(Vec2(0.5,0.5));
    //log("x : y: %f%f",menu->getPositionX(),menu->getPositionY());
    this->imgketqua= static_cast<ImageView*>(this->pnlBg->getChildByName("imgketqua"));
    this->imgketqua->setVisible(false);

    this->imgBGTimer = static_cast<ImageView*>(this->menu->getChildByName("imgBGTimer"));
    this->txtTimer = static_cast<Text*>(this->imgBGTimer->getChildByName("txtTimer"));
    this->txtTimer->setString("");
    this->imgBGTimer->setVisible(false);
    this->setContentSize(menu->getContentSize());
    
    this->mQueueMsg = new ThreadQueueMsg();
    
    this->isShow = false;
    this->isHaveHiden = true;
    
    this->btnPoker = dynamic_cast<Button*>(this->pnlBg->getChildByName("btnPoker"));
    
    if (this->btnPoker){
        this->btnPoker->setTag(130);
        this->btnPoker->addClickEventListener(CC_CALLBACK_1(LayerLauncherMiniGame::onButtonMiniGame, this));
        this->btnPoker->setPressedActionEnabled(true);
    }
    
    this->btnSapNgua = dynamic_cast<Button*>(this->pnlBg->getChildByName("btnSapNgua"));
    
    if (this->btnSapNgua){
        this->btnSapNgua->setTag(133);
        this->btnSapNgua->addClickEventListener(CC_CALLBACK_1(LayerLauncherMiniGame::onButtonMiniGame, this));
        this->btnSapNgua->setPressedActionEnabled(true);
    }
    
    this->btnCaoThap = dynamic_cast<Button*>(this->pnlBg->getChildByName("btnCaoThap"));
    
    if (this->btnCaoThap){
        this->btnCaoThap->setTag(134);
        this->btnCaoThap->addClickEventListener(CC_CALLBACK_1(LayerLauncherMiniGame::onButtonMiniGame, this));
        this->btnCaoThap->setPressedActionEnabled(true);
    }
    this->btnVongQuay = dynamic_cast<Button*>(this->pnlBg->getChildByName("btnVongQuay"));
    
    if (this->btnVongQuay){
        this->btnVongQuay->setTag(135);
        this->btnVongQuay->addClickEventListener(CC_CALLBACK_1(LayerLauncherMiniGame::onButtonMiniGame, this));
        this->btnVongQuay->setPressedActionEnabled(true);
    }
    auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');

    for (int i = 0; i < lstmini.size(); i++){
        if (strcmp(lstmini[i].c_str(), "128") == 0){
            btnOpen->setVisible(false);
            this->menu->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "130") == 0){
            this->btnPoker->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "129") == 0){
            this->btnCaoThap->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "132") == 0){
            this->btnVongQuay->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "133") == 0){
            this->btnSapNgua->setVisible(false);
        }
    }
    
    
    auto pokerPos = this->btnPoker->getPosition();
    auto xocdiaPos = this->btnSapNgua->getPosition();
    this->btnPoker->setPosition(xocdiaPos);
    this->btnSapNgua->setPosition(pokerPos);
    this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerLauncherMiniGame::OnExtensionResponseProcess));
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLauncherMiniGame::updateEvent),this,1/60, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this,1, false);
    
    //GameServer::getSingleton().addListeners(this);

    
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    params->PutInt("tm",0);
    boost::shared_ptr<IRequest> req (new ExtensionRequest("txj", params));
    GameServer::getSingleton().Send(req);
    
    
    
    return true;
}
void LayerLauncherMiniGame::onButtonMiniGame(Ref* pSender){
    if (this->isShow)
        this->startWaittoHide();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    auto node = (Node*)pSender;
    int tag = node->getTag();
    if (tag == 130){
        if (SceneManager::getSingleton().getIsShowPK())
            return;
        LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
        _currScene->addChild(layerPlayerGame,2000,9013);
        SceneManager::getSingleton().setIsShowPK(true);
    }
    else if(tag == 128){
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        _currScene->addChild(layerPlayerGame,2000,9014);
        layerPlayerGame->shouldShowHuongDan();

        SceneManager::getSingleton().setIsShowTX(true);
    } else if(tag == 133){
        if (SceneManager::getSingleton().getIsShowXD())
            return;
        LayerPlayGameDoDenMini* layerPlayerGame = LayerPlayGameDoDenMini::create();
        _currScene->addChild(layerPlayerGame,2000,9015);
        SceneManager::getSingleton().setIsShowXD(true);
    }
    else if(tag == 134){
        if (SceneManager::getSingleton().getIsShowCT())
            return;
        LayerCaoThapPopupGame* layerPlayerGame = LayerCaoThapPopupGame::create();
        _currScene->addChild(layerPlayerGame,2000,9016);
        SceneManager::getSingleton().setIsShowCT(true);
    }
    else if(tag == 135){
        if (SceneManager::getSingleton().getIsShowVQ())
            return;
        LayerPlayVQMM* layerPlayerGame = LayerPlayVQMM::create();
        _currScene->addChild(layerPlayerGame,2000,9017);
        SceneManager::getSingleton().setIsShowVQ(true);
    }
    
    
}
void LayerLauncherMiniGame::onTimerToHideButton(float dt){
    this->timerToHide--;
    if(this->timerToHide == 0) {
        if (this->isShow){
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this);
            auto x = this->pnlBg->getPositionX()+sizeMove;
            auto y = this->pnlBg->getPositionY();
            this->isShow = false;
            this->isHaveHiden = true;
            this->pnlBg->runAction(MoveTo::create(0.5,Vec2(x,y)));
        }
    }
}
void LayerLauncherMiniGame::startWaittoHide(){
    if (this->isShow) {
        this->timerToHide = 3;
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this,1, false);
//        if (this->isHaveHiden) {
//            auto x = this->pnlBg->getPositionX()+sizeMove;
//            auto y = this->pnlBg->getPositionY();
//            this->pnlBg->runAction(MoveTo::create(0.5, Vec2(x, y)));
//            this->isShow = true;
//        }
    }
}
void LayerLauncherMiniGame::stopWaitWhenTouch(){
    if (!this->isShow) {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this);
        auto x = this->pnlBg->getPositionX()-sizeMove;
        auto y = this->pnlBg->getPositionY();
        this->pnlBg->runAction(MoveTo::create(0.5, Vec2(x, y)));
        this->isShow = true;
        this->isShow = true;
        this->isHaveHiden = false;
    }
}
void LayerLauncherMiniGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    pObjs->PutUtfString("_cmd_", *cmd);
    if(0 != cmd->compare(EXT_EVENT_START_TXMINI) && 0 != cmd->compare(EXT_EVENT_END_TXMINI) && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF_TXMINI)) {
        return;
    }
    this->mQueueMsg->addQueue(pObjs);
}

void LayerLauncherMiniGame::OnExtensionResponseProcess(){
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

void LayerLauncherMiniGame::event_EXT_EVENT_START(){
    long playerTime = *this->param->GetInt("pt");
    this->txtTimer->setVisible("");
    this->PlayTurnTime = playerTime / 1000;
    this->UpdateTimeStartGame();
    this->imgketqua->setVisible(false);

    
}
void LayerLauncherMiniGame::UpdateTimeStartGame(){
    this->imgBGTimer->setVisible(true);
    this->imgBGTimer->stopAllActions();
    this->imgBGTimer->setOpacity(255);
    this->txtTimer->setVisible(true);
    this->StopMyTimer();
    this->TimerCountDown = this->PlayTurnTime;
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLauncherMiniGame::updateTimer),this,1, false);
    
}
void LayerLauncherMiniGame::updateTimer(float dt) {
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
void LayerLauncherMiniGame::StopMyTimer() {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::updateTimer),this);
    
    //this.txtTimer.setVisible(false);
    this->TimerCountDown = this->PlayTurnTime;
}
void LayerLauncherMiniGame::event_EXT_EVENT_END(){
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
            auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
            
            for (int i = 0; i < lstmini.size(); i++){
                if (strcmp(lstmini[i].c_str(), "128") == 0){
                    this->imgketqua->setVisible(false);

                }
            }
        }
}
void LayerLauncherMiniGame::event_EXT_EVENT_GAME_STATE_NTF(){
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
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerLauncherMiniGame::updateTimer),this,1, false);
        }
    }
}
void LayerLauncherMiniGame::updateEvent(float dt){
    if(this->mQueueMsg->isQueueEmpty())
        return;
    
    this->mQueueMsg->run();
}
void LayerLauncherMiniGame::onBtnOpen(Ref* pSender){
    if (this->isShow) {
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        //    layerPlayerGame = LayerPlayGameTXMini::create();
        //    this->addChild(layerPlayerGame);
        //    this->imgBGTimer->setVisible(false);
        //    this.btnOpen.setEnabled(false);
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        _currScene->addChild(layerPlayerGame,2000,9018);
        layerPlayerGame->shouldShowHuongDan();

        SceneManager::getSingleton().setIsShowTX(true);
    }
    else{
        auto x = this->pnlBg->getPositionX()-sizeMove;
        auto y = this->pnlBg->getPositionY();
        this->pnlBg->runAction(MoveTo::create(0.5, Vec2(x, y)));
        this->isShow = true;
        this->startWaittoHide();
    }
    
}
bool LayerLauncherMiniGame::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    auto locationInNode = pTouch->getLocation();
    //var s = target.getContentSize();
    auto rectNode = this->pnlBg->getBoundingBox();//Rect(this->menu->getPosition().x, this->menu->getPosition().y, 143,149);
    Size sizeAdd;
    
    sizeAdd = ChanUtils::getSizePos();
    
    auto rect = Rect(rectNode.origin.x + sizeAdd.width, rectNode.origin.y + sizeAdd.height, rectNode.size.width, rectNode.size.height);
    //listener->setSwallowTouches(false);
    if (rect.containsPoint(locationInNode)) {
//        this->isTouchMove = false;
        if (!this->isShow)
            this->stopWaitWhenTouch();
        return true;
    }
    return false;
}
void LayerLauncherMiniGame::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
//    auto target = this->menu;
//    auto delta = pTouch->getDelta();
//    //  if (target.x ==0 || target.x == WIDTH_DESIGN-143 || target.y ==0 || target.y == HEIGHT_DESIGN-149)
//    //     return;
//    auto x = target->getPositionX() + delta.x;
//    auto y = target->getPositionY() + delta.y;
//    
//    this->menu->setPosition(Vec2(x,y));
//    if (abs(delta.x) > 10 || abs(delta.y) > 10)
//        this->isTouchMove =true;
    //listener->setSwallowTouches(true);
}
void LayerLauncherMiniGame::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    
//    Size sizeAdd;
//    
//    sizeAdd = ChanUtils::getSizePos();
//    
//    auto target = pEvent->getCurrentTarget();
//    
//    if (this->menu->getPositionX()>visibleSize.width/2)
//    {
//        auto posY = this->menu->getPositionY();;
//        if(this->menu->getPositionY()>visibleSize.height-this->menu->getContentSize().height-sizeAdd.height){
//            posY = visibleSize.height-this->menu->getContentSize().height-sizeAdd.height;
//        }else if(this->menu->getPositionY()<(visibleSize.height-this->menu->getContentSize().height+50) && this->menu->getPositionY()> this->menu->getContentSize().height){
//            posY = this->menu->getPositionY();
//        }
//        else{
//            posY = -sizeAdd.height;
//        }
//        this->menu->runAction(MoveTo::create(0.2,Vec2(visibleSize.width - this->menu->getContentSize().width,posY)));
//        
//    }
//    if (this->menu->getPositionX()<visibleSize.width/2)
//    {
//        auto posY = this->menu->getPositionY();;
//        if(this->menu->getPositionY()>visibleSize.height-this->menu->getContentSize().height){
//            posY = visibleSize.height-this->menu->getContentSize().height-sizeAdd.height;
//        }else if(this->menu->getPositionY()<(visibleSize.height-this->menu->getContentSize().height+50) && this->menu->getPositionY()> this->menu->getContentSize().height){
//            posY = this->menu->getPositionY();
//        }
//        else{
//            posY = -sizeAdd.height;
//        }
//        this->menu->runAction(MoveTo::create(0.2,Vec2(0,posY)));
//    }
//    //listener->setSwallowTouches(false);
//    //  self.menu.btnOpen.setEnabled(true);
//    if(!this->isTouchMove){
//        this->onBtnOpen(NULL);
//        
//    }
    if (this->isShow)
        this->startWaittoHide();
}
void LayerLauncherMiniGame::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerLauncherMiniGame::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LayerLauncherMiniGame::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LayerLauncherMiniGame::onTouchEnded, this);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    GameServer::getSingleton().addListeners(this);
}

void LayerLauncherMiniGame::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    boost::shared_ptr<IRequest> req (new ExtensionRequest("txl", params));
    GameServer::getSingleton().Send(req);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::onTimerToHideButton),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerLauncherMiniGame::updateTimer),this);

}


