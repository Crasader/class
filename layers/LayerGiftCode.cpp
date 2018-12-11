//
//  LayerGiftCode.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerGiftCode.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
LayerGiftCode::LayerGiftCode()
{
    GameServer::getSingleton().addListeners(this);
}


LayerGiftCode::~LayerGiftCode()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerGiftCode::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerGiftCode.csb");
//    rootNode->setAnchorPoint(Point(0.5, 0.5));
//    rootNode->setPosition(Point(visibleSize / 2));
//    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
//        sizeAdd = ChanUtils::getSizePos();
//        
//    }else{
//        sizeAdd = Size(0,0);
//    }
//    Size sizeAdd2;
//    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
//        sizeAdd2 = ChanUtils::getSizePos();
//        
//    }else{
//        sizeAdd2 = Size(0,0);
//    }
//    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    //pnlBg->setPosition(Point(visibleSize.width/2,visibleSize.height/2- sizeAdd2.height));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerGiftCode::onBtnClose, this));
    }
    
    this->tfGifCode= static_cast<TextField*>(pnlBg->getChildByName("tfGifCode"));
    this->tfGifCode->setPlaceHolder(dataManager.GetSysString(789));
    Button* btnXacNhan = dynamic_cast<Button*>(pnlBg->getChildByName("btnXacNhan"));
    if(btnXacNhan){
        btnXacNhan->addClickEventListener(CC_CALLBACK_1(LayerGiftCode::onBtnXacNhan, this));
        btnXacNhan->setTitleText(dataManager.GetSysString(790));
    }
           return true;
}
void LayerGiftCode::hideLayer(){
    this->tfGifCode->setEnableEditBox(false);
    this->setVisible(false);
}
void LayerGiftCode::showLayer(){
    this->tfGifCode->setEnableEditBox(true);
    this->setVisible(true);

}
void LayerGiftCode::onBtnXacNhan(Ref* pSender){


    if (this->tfGifCode->getString() == "") {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(146));
        return;
    }
    if (this->tfGifCode->getString().size() >=30) {
        SceneManager::getSingleton().showToast(dataManager.GetSysString(139));
        return;
    }
    if (mUtils::findSpecialCharSpace(this->tfGifCode->getString())){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(147));
        return;
    }
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString(GIFT_CODE, this->tfGifCode->getString());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(RECEIVED_GIFT_CODE_AWARD_REQ,params));
    GameServer::getSingleton().Send(request);
    
    SceneManager::getSingleton().showLoading();
}

void LayerGiftCode::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerGiftCode::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(RECEIVED_GIFT_CODE_AWARD_RESP, cmd->c_str()) == 0){
        SceneManager::getSingleton().hideLoading();
        if (param->GetInt(AWARD_SILVER) == NULL || param->GetInt("rc") == NULL || param->GetInt(AWARD_GOLD) == NULL || param->GetUtfString("rd") == NULL || param->GetInt(AWARD_EXP) == NULL) return;
        int rc = *param->GetInt("rc");
        if (rc == 0){
            int silver = *param->GetInt(AWARD_SILVER);
            int gold = *param->GetInt(AWARD_GOLD);
            int exp = *param->GetInt(AWARD_EXP);
            
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->setMessage(dataManager.GetSysString(141));
            string content = dataManager.GetSysString(530)+ " ";
            if (silver> 0){
                content+=StringUtils::format("%s %s ",mUtils::convertMoneyEx(silver).c_str(),SceneManager::getSingleton().getCoin(1).c_str());
            }
            if (gold> 0){
                content+=StringUtils::format("%s %s ",mUtils::convertMoneyEx(gold).c_str(),SceneManager::getSingleton().getCoin(0).c_str());
            }
            if (exp> 0){
                content+=StringUtils::format("%d EXP ",exp);
            }
            content += "!";
            layerPopup->setContentMess(content);
            layerPopup->setTag(24);
            layerPopup->showAButton();
            _currScene->addChild(layerPopup,1000000,9094);
            
            //SceneManager::getSingleton().showLoading();
            
        }
        else{
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(*param->GetUtfString("rd"));
            _currScene->addChild(layerPopup,1000000,9095);
            
        }

    }
    
}


bool LayerGiftCode::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerGiftCode::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerGiftCode::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerGiftCode::onExit()
{
    Layer::onExit();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
//void LayerGiftCode::onPopupCallBack(Ref *pSender)
//{
//    auto _currScene = Director::getInstance()->getRunningScene();
//    if (_currScene)
//        _currScene->removeChildByTag(24);
//    
//    boost::shared_ptr<ISFSObject> params(new SFSObject());
//    params->PutInt(EXCHANGE_GOLD_VALUE,atoi(this->tfGold->getString().c_str()));
//    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_EXCHANGE_GOLD2SILVER_REQ,params));
//    GameServer::getSingleton().Send(request);
//    
//    SceneManager::getSingleton().showLoading();
//}
