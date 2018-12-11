#include "LayerIAP.h"

#include "LayerPopupThongBao.h"
#include "../layergames/_Chat_.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/ChanUtils.h"
#include "../layergames/PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "layergames/_Chat_.h"
#include <vector>
#include "json/rapidjson.h"
#include "json/document.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
#define TIME_DISABLE 5

bool LayerIAP::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerIAP.csb");
    int gameID = SceneManager::getSingleton().getGameID();
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    if (gameID == kGameChan || gameID == kGameCoTuong || gameID == kGameCoUp){
//        root->setAnchorPoint(Point::ZERO);
//        root->setPosition(Point::ZERO);
//    }
//    else{
//        root->setAnchorPoint(Point(0.5, 0.5));
//        root->setPosition(Point(visibleSize / 2));
//    }
    ui::Helper::doLayout(root);
    addChild(root);
    Layout* panel_bronze = (Layout*)root->getChildByName("panel_bronze");
    int money_rate = dataManager.getAppConfig().iap_rate;
    if (panel_bronze)
    {
        auto labelBronze_0 = static_cast<Text*> (panel_bronze->getChildByName("labelBronze_0"));
        labelBronze_0->setString(dataManager.GetSysString(939));
        Button* btnBronze = static_cast<Button*>(panel_bronze->getChildByName("btnBronze"));
        if (btnBronze)
        {
           // btnBronze->setTitleText("0.99$");
            btnBronze->addTouchEventListener(CC_CALLBACK_2(LayerIAP::onButtonBronze, this));
        }
        Text *txtBronze = static_cast<Text*>(panel_bronze->getChildByName("labelBronze"));
        if (txtBronze)
            txtBronze->setString(mUtils::convertMoneyEx(money_rate * 1000));
    }
    
    Layout* panel_silver = (Layout*)root->getChildByName("panel_silver");
    if (panel_silver)
    {
        auto labelBronze_0 = static_cast<Text*> (panel_silver->getChildByName("labelBronze_0"));
        labelBronze_0->setString(dataManager.GetSysString(939));
        Button* btnSilver = static_cast<Button*>(panel_silver->getChildByName("btnSilver"));
        if (btnSilver)
        {
            btnSilver->setTitleText("9.99$");
            btnSilver->addTouchEventListener(CC_CALLBACK_2(LayerIAP::onButtonSilver, this));
        }
        Text *txtSilver = static_cast<Text*>(panel_silver->getChildByName("labelSilver"));
        if (txtSilver)
            txtSilver->setString(mUtils::convertMoneyEx(money_rate * 1000 * 10));
    }
    
    Layout* panel_gold = (Layout*)root->getChildByName("panel_gold");
    if (panel_gold)
    {
        auto labelBronze_0 = static_cast<Text*> (panel_gold->getChildByName("labelBronze_0"));
        if (labelBronze_0)
            labelBronze_0->setString(dataManager.GetSysString(939));
        Button* btnGold = static_cast<Button*>(panel_gold->getChildByName("btnGold"));
        if (btnGold)
        {
            btnGold->setTitleText("99.99$");
            btnGold->addTouchEventListener(CC_CALLBACK_2(LayerIAP::onButtonGold, this));
        }
        Text *txtGold = static_cast<Text*>(panel_gold->getChildByName("labelGold"));
        if (txtGold)
            txtGold->setString(mUtils::convertMoneyEx(money_rate * 1000 * 100));
    }
    // 	Button* btnClose = static_cast<Button*>(root->getChildByName("btnClose"));
    // 	if (btnClose)
    // 		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerIAP::onButtonClose, this));
    // 	Button* btnMuabac = static_cast<Button*>(root->getChildByName("btnMuabac"));
    // 	if (btnMuabac)
    // 		btnMuabac->setTitleText("Mua Bạc");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    iap = new mIAP();
    iap->loadIAP();
#endif
    return true;
}

LayerIAP::LayerIAP()
{
    GameServer::getSingleton().addListeners(this);

}

LayerIAP::~LayerIAP()
{
    GameServer::getSingleton().removeListeners(this);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    iap->removeListener();
    delete iap;
#endif
}
void LayerIAP::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    
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
            _scene->addChild(layerPopup,2000);
            SceneManager::getSingleton().hideLoading();
        }
        else{
            string valuePur = "";
            auto _scene = Director::getInstance()->getRunningScene();

            LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
            layerPopup->showAButton();
            layerPopup->setMessage(dataManager.GetSysString(141));
            layerPopup->setContentMess(dataManager.GetSysString(640));
            _scene->addChild(layerPopup,2000);
            SceneManager::getSingleton().hideLoading();
        }
    }
}
void LayerIAP::showLayer(){
    this->setVisible(true);

}
void LayerIAP::hideLayer(){
    this->setVisible(false);
}
void LayerIAP::onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
      //  iap->purchaseItem(0);
#endif
    }
}

void LayerIAP::onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS  || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       // iap->purchaseItem(1);
#endif
    }
}

void LayerIAP::onButtonGold(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       // iap->purchaseItem(2);
#endif
    }
}
