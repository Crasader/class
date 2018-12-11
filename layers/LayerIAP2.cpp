//
//  LayerIAP2.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#include "LayerIAP2.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "../data_manager.h"
#include "Requests/ExtensionRequest.h"
#include "LayerPopupThongBao.h"
#include "LayerGuideInGame.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
LayerIAP2::LayerIAP2()
{
    GameServer::getSingleton().addListeners(this);
    
}


LayerIAP2::~LayerIAP2()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerIAP2::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    //Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerIAP2.csb");
    
    
    rootNode->setPosition(Vec2(0,0));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    this->setContentSize(rootNode->getContentSize());
    auto pnlContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    pnlContent->setVisible(false);
    pnlContent->setSwallowTouches(false);
    this->scrollTyGia = ui::ScrollView::create();
    this->scrollTyGia->setDirection(ui::ScrollView::Direction::VERTICAL);
    this->scrollTyGia->setClippingEnabled(true);
    this->scrollTyGia->setContentSize(pnlContent->getContentSize());
    this->scrollTyGia->setAnchorPoint(pnlContent->getAnchorPoint());
    this->scrollTyGia->setPosition(pnlContent->getPosition());
    this->scrollTyGia->setScrollBarEnabled(false);
    this->scrollTyGia->setBounceEnabled(false);
    this->scrollTyGia->setTag(129323);
    this->scrollTyGia->setInnerContainerSize(Size(734, pnlContent->getContentSize().height));
    this->addChild(this->scrollTyGia);

    
    return true;
}

void LayerIAP2::showLayer(){
    this->setVisible(true);
    GameServer::getSingleton().addListeners(this);


}
void LayerIAP2::hideLayer(){
    this->setVisible(false);
    GameServer::getSingleton().removeListeners(this);

}
void LayerIAP2::loadTyGia(int _type){
    this->scrollTyGia->setInnerContainerSize(Size(1430,680));
    this->scrollTyGia->removeAllChildren();
    this->type = _type;
    if (this->type == 0){
        this->lstIAP = {iap("0.99 $","50,000",1),iap("4.99 $", "250,000", 2),iap("9.99 $", "500,000", 2),iap("49.99 $", "2,500,000", 2)};
    }else{
        this->lstIAP = {iap("0.99 $","500,000",1),iap("4.99 $", "2,500,000", 2),iap("9.99 $", "5,000,000", 2),iap("49.99 $", "25,000,000", 2)};
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    mIAP* iap = new mIAP();
     iap->loadIAP();
    for (int i =0;i<this->lstIAP.size();i++){
        ItemIAP * item = ItemIAP::create();
        item->setDatas(this->lstIAP[i].price, this->lstIAP[i].gold, i, this->type,iap);
        this->scrollTyGia->addChild(item);
        item->setPosition(Vec2(35*i + 328*(i),100));
    }
#endif
}
void LayerIAP2::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    Scene *scene = Director::getInstance()->getRunningScene();
    if (!scene) return;
   // log("OnExtensionResponse %s", cmd->c_str());
    if (strcmp("rsiapp", cmd->c_str()) == 0){//OK
        int rc = *param->GetInt("rc");
        if (rc == 0){
            string valuePur = "";
            auto _scene = Director::getInstance()->getRunningScene();
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(639));
            _scene->addChild(layerPopup,4000);
            SceneManager::getSingleton().hideLoading();
        }
        else{
            string valuePur = "";
            auto _scene = Director::getInstance()->getRunningScene();
            
            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(640));
            _scene->addChild(layerPopup,4000);
            SceneManager::getSingleton().hideLoading();
        }
    }

}


void LayerIAP2::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerIAP2::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerIAP2::onExit()
{
    Layer::onExit();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
bool ItemIAP::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto root = CSLoader::getInstance()->createNode("LayerItemIAP.csb");
    ui::Helper::doLayout(root);
    this->addChild(root);
    
    this->setContentSize(root->getContentSize());
    this->txtPrice= static_cast<Text*>(root->getChildByName("txtPrice"));
    this->txtMoney= static_cast<Text*>(root->getChildByName("txtMoney"));
    this->txtType= static_cast<Text*>(root->getChildByName("txtType"));
    auto btnBuy = static_cast<Button*>(root->getChildByName("btnBuy"));
    if (btnBuy){
        btnBuy->addClickEventListener(CC_CALLBACK_1(ItemIAP::onBtnBuy, this));
    }

    return true;
}
void ItemIAP::setDatas(string price,string money,int tag,int type, mIAP* _iap){
    this->txtPrice->setString(price);
    this->txtMoney->setString(money);
    this->txtType->setString(SceneManager::getSingleton().getCoin(type));
    this->type = type;
    this->tag = tag;
    this->iap = _iap;

}
void ItemIAP::onBtnBuy(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    this->iap->purchaseItem(this->tag,this->type);
#endif
}

