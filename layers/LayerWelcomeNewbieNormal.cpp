//
//  LayerWelcomeNewbieNormal.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/7/16.
//
//

#include "LayerWelcomeNewbieNormal.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "LayerPopupMission.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "LayerPopupThongBao.h"
#include "LayerPopupMission.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginFacebook/PluginFacebook.h"
#include "mFaceBook.h"
#endif
USING_NS_CC;
using namespace cocos2d::ui;

LayerWelcomeNewbieNormal::LayerWelcomeNewbieNormal()
{
    
}


LayerWelcomeNewbieNormal::~LayerWelcomeNewbieNormal()
{
    
}

bool LayerWelcomeNewbieNormal::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerWelcomeNewbieNormal.csb");
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        ImageView* imgCircle = static_cast<ImageView*>(pnlBg->getChildByName("Sprite_2"));
        auto ActionRotate = RepeatForever::create(RotateBy::create(0.2,10));
        imgCircle->runAction(ActionRotate);
    }
    
//     img1 = static_cast<ImageView*>(pnlBg->getChildByName("img1"));
//     this->imgMoney.push_back(img1);
//     img2 = static_cast<ImageView*>(pnlBg->getChildByName("img2"));
//     this->imgMoney.push_back(img2);
//     img3 = static_cast<ImageView*>(pnlBg->getChildByName("img3"));
//     this->imgMoney.push_back(img3);
//     img4 = static_cast<ImageView*>(pnlBg->getChildByName("img4"));
//     this->imgMoney.push_back(img4);
//     img5 = static_cast<ImageView*>(pnlBg->getChildByName("img5"));
//     this->imgMoney.push_back(img5);
	this->lbmMoney = (TextBMFont*)pnlBg->getChildByName("lbmMoney");


    this->CoinImg = static_cast<Sprite*>(pnlBg->getChildByName("iconBac"));
    
    btnLKFB = dynamic_cast<Button*>(pnlBg->getChildByName("btnLK"));
    if(btnLKFB){
        btnLKFB->addClickEventListener(CC_CALLBACK_1(LayerWelcomeNewbieNormal::onBtnLKFBClick, this));
        auto txtBonus = dynamic_cast<Text*>(btnLKFB->getChildByName("txtMoney"));
        if (txtBonus) {
            txtBonus->setString(StringUtils::format("+%d",SceneManager::getSingleton().getFacebookBonus()));
        }
                                            
    }
    
    btnMisisonFB = dynamic_cast<Button*>(pnlBg->getChildByName("btnFB"));
    if(btnMisisonFB){
        btnMisisonFB->addClickEventListener(CC_CALLBACK_1(LayerWelcomeNewbieNormal::onBtnMissionFBClick, this));
        auto txtBonus = dynamic_cast<Text*>(btnMisisonFB->getChildByName("txtMoney"));
        if (txtBonus) {
            txtBonus->setString(StringUtils::format("+%d",SceneManager::getSingleton().getFacebookBonus()));
        }
        btnMisisonFB->setVisible(false);
    }
    
    btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnBoQua"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerWelcomeNewbieNormal::onBtnClose, this));
    }
    
    imageLK = static_cast<ImageView*>(pnlBg->getChildByName("Image_11"));
    if (std::strcmp("", StringUtils::format("%d",SceneManager::getSingleton().getLoginBonus()).c_str()) != 0){
        if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0)
            this->setMoney(StringUtils::format("%d",SceneManager::getSingleton().getLoginBonus()));
        else
            this->setMoney(StringUtils::format("%d",SceneManager::getSingleton().getLoginBonusNotFB()));
    }
	Text* Text_1 = (Text*)pnlBg->getChildByName("Text_1");
	if (Text_1)
	{
		Text_1->setString(StringUtils::format("Liên kết tài khoản facebook ngay để nhận %d %s", SceneManager::getSingleton().getFacebookBonus(), SceneManager::getSingleton().getCoin(0).c_str()));
	}
//     if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
//         txtGuide = dynamic_cast<Text*>(pnlBg->getChildByName("txtGuide"));
//         txtGuide->setString(StringUtils::format("Liên kết tài khoản facebook ngay để nhận %d %s",SceneManager::getSingleton().getFacebookBonus(),SceneManager::getSingleton().getCoinUnit().c_str()));
//     }
    
//    if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0 || strcmp("google", dataManager.getAppConfig().loginType.c_str()) == 0){
//        UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", false);
//    }
    
    
    return true;
}
void LayerWelcomeNewbieNormal::onBtnLKFBClick(Ref* pSender){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    mFaceBook* fb = new mFaceBook();
    fb->setFunCallBack(this, callfuncND_selector(LayerWelcomeNewbieNormal::doLienKetFB));
    fb->onGetUID();
#endif
}
void LayerWelcomeNewbieNormal::onBtnMissionFBClick(Ref* pSender){
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_NEW_MISSION_REQ, params));
    params->PutInt(EVENT_MISSION_ID,1);
    GameServer::getSingleton().Send(request);

    SceneManager::getSingleton().showLoading();
    this->removeFromParentAndCleanup(true);
}
void LayerWelcomeNewbieNormal::onBtnClose(Ref* pSender){
   // this->gotoDiemDanh();
    //this->gotoDiemDanh();
    if (strcmp("facebook", dataManager.getAppConfig().loginType.c_str()) == 0 || strcmp("google", dataManager.getAppConfig().loginType.c_str()) == 0){
        UserDefault::getInstance()->setBoolForKey("isShouldShowWelcome", false);
    }
    this->removeFromParentAndCleanup(true);
}
bool LayerWelcomeNewbieNormal::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}
void LayerWelcomeNewbieNormal::setMoney(string money){
    
    int lengh = money.size();
//     for (int i = 0; i< imgMoney.size(); i++) {
//         this->imgMoney.at(i)->setVisible(false);
//     }
    
    if (dataManager.getAppConfig().NUM_REGISTER_DEVICE > 1) {
        this->CoinImg->setVisible(true);
        return;
    }
	this->lbmMoney->setString(money);
    
//     int a = imgMoney.size()-lengh;
// 
//     for (int i = 0; i < lengh; i++) {
//         this->imgMoney.at(a+i)->loadTexture(StringUtils::format("%.png",money.at(i)));
//         this->imgMoney.at(a+i)->setVisible(true);
//     }
}
void LayerWelcomeNewbieNormal::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
}
void LayerWelcomeNewbieNormal::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerWelcomeNewbieNormal::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
//
//    GameServer::getSingleton().addListeners(this);
}

void LayerWelcomeNewbieNormal::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
//    GameServer::getSingleton().removeListeners(this);
//    Layer::onExit();
}
void LayerWelcomeNewbieNormal::showFacebookUser(){
    if (std::strcmp("", StringUtils::format("%d",SceneManager::getSingleton().getLoginBonus()).c_str()) != 0)
        setMoney(StringUtils::format("%d",SceneManager::getSingleton().getLoginBonus()));
    btnLKFB->setVisible(false);
    btnMisisonFB->setVisible(true);
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        imageLK->loadTexture("lien-ket-nv.png");
    }
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        txtGuide->setString(StringUtils::format("Vui lòng vào nhiệm vụ bên dưới để nhận nốt %d %s",SceneManager::getSingleton().getLoginBonus(),SceneManager::getSingleton().getCoin(1).c_str()));
    }
}
void LayerWelcomeNewbieNormal::gotoDiemDanh()
{
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupMission* pop = LayerPopupMission::create();
    _currScene->addChild(pop,ZORDER_LIST::ZORDER_POPUP_NEWBIE,9052);
    pop->doDiemDanh();
}
void LayerWelcomeNewbieNormal::doLienKetFB(Node* sender, void* data){
    string *value = (string*) data;
    string uid = *value;
    if (strcmp(uid.c_str(), "cancel") == 0){
        if (dataManager.getAppConfig().isLoginFisrtDay)
            gotoDiemDanh();
        
        this->removeFromParentAndCleanup(true);
        return;
    }
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    params->PutUtfString("fbID", *value);
    boost::shared_ptr<IRequest> request(new ExtensionRequest("rilf", params));
    GameServer::getSingleton().Send(request);
    this->removeFromParentAndCleanup(true);
}



