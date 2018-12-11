 //
//  LayerBaoDanhNew.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerBaoDanhNew.h"
#include "Requests/ExtensionRequest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "LayerPopupThongBao.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerBaoDanhNew::LayerBaoDanhNew()
{
    GameServer::getSingleton().addListeners(this);

}


LayerBaoDanhNew::~LayerBaoDanhNew()
{
    GameServer::getSingleton().removeListeners(this);

}

bool LayerBaoDanhNew::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupDiemDanh.csb");
    auto Panel_2 = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    Panel_2->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
 //   if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
     this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    this->bgBaoDanh = pnlBg;
    
    auto btnClose  = (Button*)pnlBg->getChildByName("btnClose");
    if (btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerBaoDanhNew::onBtnClose, this));
    }
    
    for(int i = 1 ; i <= 5 ; i++){
        auto textngay  = (Text*)pnlBg->getChildByName(StringUtils::format("txtDay%d",i));
        textngay->setString(StringUtils::format("%s%d",dataManager.GetSysString(1048).c_str(),i));
        this->lstTextNgay.push_back(textngay);
        
        auto textMoney  = (Text*)pnlBg->getChildByName(StringUtils::format("txtMoney%d",i));
        textMoney->setColor(Color3B(254,199,54));
        this->lstTextMoney.push_back(textMoney);
        
        auto textcoin  = (Text*)pnlBg->getChildByName(StringUtils::format("txtCoin%d",i));
        textcoin->setColor(Color3B(254,199,54));
        this->lstTextCoin.push_back(textcoin);
        
        auto sprite  = (Sprite*)pnlBg->getChildByName(StringUtils::format("sprDone%d",i));
        sprite->setVisible(false);
        this->lstSpriteDone.push_back(sprite);
        
        auto button  = (Button*)pnlBg->getChildByName(StringUtils::format("btnNhan%d",i));
        if (button){
            button->addClickEventListener(CC_CALLBACK_1(LayerBaoDanhNew::onBtnNhanThuong, this));
            button->setVisible(false);
            button->setTitleText(dataManager.GetSysString(638));
        }
        this->lstButton.push_back(button);
    }
    int osid = 1;
    string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osid = 1;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
    else
        devID = NativeHelper::getDeviceId();
    /*isConnectting = NativeHelper::hasConnection();*/
    //devID = NativeHelper::getDeviceID();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osid = 2;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
    else
        devID = IOS::getDeviceID();
#endif
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);
    
    int day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900;
    string dateTime = StringUtils::format("%d/%d", month, year).c_str();
    params->PutUtfString("month", dateTime);
    params->PutInt("osid", osid);
    params->PutUtfString("devid", devID);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_LIST_ATTEND_REQ, params)); //REQUEST_GET_CONFIG = "reqgcf";
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().showLoading();
    this->bgBaoDanh->setVisible(false);
    
    return true;
}
void LayerBaoDanhNew::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerBaoDanhNew::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp(EXT_EVENT_LIST_ATTEND_RES, cmd->c_str()) == 0){//OK
        SceneManager::getSingleton().hideLoading();
        auto numattend = param->GetInt(EXT_FILED_NUM_ATTEND);
        auto listattend = param->GetUtfString(EXT_FIELD_LIST_ATTEND);
        if (numattend){
            for (int i = 0 ; i < *numattend; i++){
                this->lstSpriteDone[i]->setVisible(true);
                this->lstTextMoney[i]->setColor(Color3B(82,13,8));
                this->lstTextCoin[i]->setColor(Color3B(82,13,8));
                this->lstButton[i]->setVisible(false);
            }
            if (*numattend<this->lstButton.size())
                this->lstButton[*numattend]->setVisible(true);
        }
        if (listattend){
            auto lst = mUtils::splitString(*listattend,',');
            if (lst.size()<=0 || *listattend == " "|| *listattend == ""){
                this->removeFromParentAndCleanup(true);
                return;
            }
            for (int i = 0;i < lst.size();i++){
                auto item = mUtils::splitString(lst[i],':');
                if (item.size()<2) continue;
                this->lstTextMoney[i]->setString(mUtils::convertMoneyEx(atoi(item[1].c_str())));
            }
            this->bgBaoDanh->setVisible(true);

            if (*numattend == lst.size()){
                this->removeFromParentAndCleanup(true);
            }
        }

    }else if (strcmp(EXT_EVENT_ATTEND_RES, cmd->c_str()) == 0){
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        time_t theTime = time(NULL);
        struct tm *aTime = localtime(&theTime);
        
        int day = aTime->tm_mday;
        int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
        int year = aTime->tm_year + 1900;
        string dateTime = StringUtils::format("%d/%d", month, year).c_str();
        params->PutUtfString("month", dateTime);
        params->PutUtfString("month", dateTime);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rla", params));
        GameServer::getSingleton().Send(request);
        SceneManager::getSingleton().showLoading();
        log("ra");
        
        if (param->GetUtfString("atb") == NULL || param->GetInt("rc") == NULL) return;
        SceneManager::getSingleton().hideLoading();
        int rc = *param->GetInt("rc");
        string atb = *param->GetUtfString("atb");
        auto lst = mUtils::splitString(atb,'|');
        if (lst.size() < 2) return;
        log("RA :%d", rc);
        if (rc == 0){
            auto currscene = Director::getInstance()->getRunningScene();
            if (currscene == NULL)
                return;
            LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
            layerpoup->setMessage(dataManager.GetSysString(141));
            layerpoup->setContentMess(StringUtils::format("%s %s Zen",dataManager.GetSysString(530).c_str(),mUtils::convertMoneyEx(atoi(lst[0].c_str())).c_str()));
            layerpoup->showAButton();
            layerpoup->setTag(24);
            currscene->addChild(layerpoup,50000);
        }
        else{
            auto currscene = Director::getInstance()->getRunningScene();
            if (currscene == NULL)
                return;
            LayerPopupThongBao* layerpoup = LayerPopupThongBao::create();
            layerpoup->setMessage(dataManager.GetSysString(141));
            layerpoup->setContentMess(dataManager.GetSysString(49));
            layerpoup->showAButton();
            layerpoup->setTag(24);
            currscene->addChild(layerpoup,50000);
        }
        this->removeFromParentAndCleanup(true);
    }
}

bool LayerBaoDanhNew::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerBaoDanhNew::onBtnNhanThuong(Ref* pSender){
    int osid = 1;
    string devID = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    osid = 1;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + NativeHelper::getDeviceId();
    else
        devID = NativeHelper::getDeviceId();
    /*isConnectting = NativeHelper::hasConnection();*/
    //devID = NativeHelper::getDeviceID();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    osid = 2;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
        devID = dataManager.getAppConfig().tientomocp2 + IOS::getDeviceID();
    else
        devID = IOS::getDeviceID();
#endif
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutInt("osid", osid);
    params->PutUtfString("devid", devID);
    params->PutInt("srcref", 1);
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_ATTEND_REQ, params));
    GameServer::getSingleton().Send(request);
}

void LayerBaoDanhNew::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerBaoDanhNew::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
    SceneManager::getSingleton().numPopup+=1;
}

void LayerBaoDanhNew::onExit()
{
    
    Layer::onExit();
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("aI", SceneManager::getSingleton().getMyName());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
    GameServer::getSingleton().Send(request);
    SceneManager::getSingleton().numPopup-=1;
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}

