//
//  LayerPopupEventThoiVu.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerPopupEventThoiVu.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#include "Requests/ExtensionRequest.h"
#endif
#include "LayerPopupThongBao.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerPopupEventThoiVu::LayerPopupEventThoiVu()
{
    GameServer::getSingleton().addListeners(this);

}


LayerPopupEventThoiVu::~LayerPopupEventThoiVu()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerPopupEventThoiVu::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupEventThoiVu.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);

    Size sizeAdd;
    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
    }else{
        sizeAdd = Size(0,0);
    }
    Size sizeAdd2;
    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
        sizeAdd2 = ChanUtils::getSizePos();
        
    }else{
        sizeAdd2 = Size(0,0);
    }
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlBg"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupEventThoiVu::onBtnClose, this));
    }
    
    this->txtName = dynamic_cast<Text*>(pnlBg->getChildByName("txtName"));
    this->txtContent = dynamic_cast<Text*>(pnlBg->getChildByName("txtContent"));
    this->btnAction  = dynamic_cast<Button*>(pnlBg->getChildByName("btnAction"));
    if(btnAction){
        btnAction->addClickEventListener(CC_CALLBACK_1(LayerPopupEventThoiVu::onBtnAction, this));
    }
    //this->txtChuChay->setScale(5);
    return true;
}
void LayerPopupEventThoiVu::setCurrentEvent(int eventNum){
    this->eventNum = eventNum;
    this->txtName->setString(mUtils::getEventName(eventNum));
    this->txtContent->setString(mUtils::getEventDescription(eventNum));
    if (mUtils::getEventType(eventNum)==2){
        this->btnAction->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"nhan_thuong.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"nhan_thuong.png");
    }
}
void LayerPopupEventThoiVu::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
  
    
}
void LayerPopupEventThoiVu::onBtnAction(Ref* pSender){
    if (mUtils::getEventType(this->eventNum)==2){
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt(TYPE_EVENT_REWARD, mUtils::getEventType(this->eventNum));
        boost::shared_ptr<IRequest> request(new ExtensionRequest(EXTENSION_REQUEST_REWARD_EVENT_REQUEST, params));
        GameServer::getSingleton().Send(request);
        SceneManager::getSingleton().showLoading();
        this->removeFromParent();
        return;
    }
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    mFaceBook *face = new mFaceBook();
    face->setFunCallBack(this, callfuncND_selector(LayerPopupEventThoiVu::shareSucess));
    face->onRequestPublishPermission(mUtils::getEventImage(eventNum));
#endif
}
void LayerPopupEventThoiVu::shareSucess(Node* sender, void* data){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt(TYPE_EVENT_REWARD, mUtils::getEventType(this->eventNum));
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXTENSION_REQUEST_REWARD_EVENT_REQUEST, params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    //this->removeFromParent();
}
void LayerPopupEventThoiVu::onBtnClose(Ref* pSender){
    
    this->removeFromParentAndCleanup(true);
}
bool LayerPopupEventThoiVu::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupEventThoiVu::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupEventThoiVu::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupEventThoiVu::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

