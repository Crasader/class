#include "LayerMainBottom2.h"
#include "cocostudio/CocoStudio.h"
#include "LayerPopupProfile.h"
#include "../scenes/SceneMain.h"
#include "../scenes/ScenePickRoom.h"
#include "../layers/LayerChuyenTien.h"
#include "../layers/LayerDoiThuong.h"
#include "LayerPopupUngTien.h"
#include "../layergames/PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "../mUtils.h"
#include "../data_manager.h"
#include "../layergames/ChanUtils.h"
#include "layergames/QuayThuongHN.h"
#include "LayerPopupMission.h"
#include "../layergames/LayerPlayGameMiniPoker.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "../layergames/LayerPlayGameDoDenMini.h"
#include "../layergames/LayerCaoThapPopupGame.h"
#include "../layergames/LayerPlayVQMM.h"
#include "LayerPopupQuaTang.h"
#include "LayerPopupCuaHang.h"
#include "LayerLogin.h"
#include "../SceneManager.h"
#include "LayerPopupEvent.h"
#include "LayerPopupDoiMoc.h"
#include "LayerPopupEventThoiVu.h"
#include "Requests/LeaveRoomRequest.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../layergames/NativeHelper.h"
#endif
USING_NS_CC;

LayerMainBottom2::LayerMainBottom2() : circleAvatar(NULL)
{
}


LayerMainBottom2::~LayerMainBottom2()
{
    this->unschedule(CC_SCHEDULE_SELECTOR(LayerMainBottom2::scheduleUpdateInfo));
    this->unschedule(schedule_selector(LayerMainBottom2::onScheduleThongBao));
    GameServer::getSingleton().removeListeners(this);
}

bool LayerMainBottom2::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    string filename = "LayerMainBottom2New.csb";
    //    if (SceneManager::getSingleton().getCurrScene() == 1){
    //        if (SceneManager::getSingleton().getGameID() == kGameChan)
    //            filename = "LayerMainBottom2Chan.csb";
    //    }
    
    auto rootNode = CSLoader::getInstance()->createNode(filename);
    rootNode->setContentSize(visibleSize);
    
    //rootNode->setPosition(Point(visibleSize.width * 4 / 5, visibleSize.height / 2));
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    this->setContentSize(rootNode->getContentSize());
    
    txtName = dynamic_cast<Text*>(rootNode->getChildByName("txtName"));
    txtLevel = dynamic_cast<Text*>(rootNode->getChildByName("txtLevel"));
    txtGold = dynamic_cast<Text*>(rootNode->getChildByName("txtGold"));
    this->txtSilver = dynamic_cast<Text*>(rootNode->getChildByName("txtSilver"));
    this->txtVip = dynamic_cast<Text*>(rootNode->getChildByName("txtVip"));
    this->txtName->setString("MegaWin");
    if (SceneManager::getSingleton().isNagaCard){
        this->txtName->setString("NagaCard");
    }
    this->txtLevel->setString("LV 100");
    this->txtGold->setString("888.888.888");
    this->txtSilver->setString("888.888.888");
    this->txtVip->setString("VIP 10");
    
    this->sprCoin = dynamic_cast<Sprite*>(rootNode->getChildByName("sprCoin"));
    if (this->sprCoin){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 12; i++) {
            str = StringUtils::format("moc3-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 201, 202));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        this->animN = Animate::create(animation);
    }
    auto sprLight = dynamic_cast<Sprite*>(rootNode->getChildByName("sprLight"));
    
    
    //    int numEvent = UserDefault::getInstance()->getIntegerForKey("numEvent");
    //
    //    if (numEvent > 0){
    if (sprLight){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 3; i++) {
            str = StringUtils::format("moc3-bg-as%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 323, 320));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        Animate* animN = Animate::create(animation);
        sprLight->runAction(RepeatForever::create(animN));
    }
    if (this->sprCoin){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 12; i++) {
            str = StringUtils::format("moc3-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 323, 320));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        this->animN = Animate::create(animation);
        this->sprCoin->runAction(RepeatForever::create(animN));
    }
    //  }
    
    //moc3
    //	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //		btnBaoDanh = dynamic_cast<Button*>(rootNode->getChildByName("btnBaoDanh"));
    //		btnBaoDanh->setTag(kGameQuayThuong);
    //		btnBaoDanh->setTouchEnabled(true);
    //		btnBaoDanh->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onGotoBaoDanh, this));
    //		btnBaoDanh->setPressedActionEnabled(true);
    //
    //		btnBaoDanhStatus = dynamic_cast<Button*>(rootNode->getChildByName("btnBaoDanhStatus"));
    //		if (btnBaoDanhStatus) //for pulish to store
    //		{
    //			if (dataManager.getAppConfig().isHideBilling) //for pulish to store
    //			{
    //				btnBaoDanh->setVisible(false);
    //				btnBaoDanhStatus->setVisible(false);
    //			}
    //			else{
    //				//isFirstLogin - cheat for server : if  = 0 - login first
    //				boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    //				if (mySelf != NULL)
    //				{
    //					boost::shared_ptr<UserVariable> aNptr = mySelf->GetVariable("aN");
    //					if (aNptr != NULL)
    //					{
    //						if (0 != aNptr->GetStringValue()->compare(""))
    //						{
    //							int lf = -1;
    //							if (dataManager.getAppConfig().isLoginFisrt) //dang nhap lan dau tien
    //								lf = 0;
    //							boost::shared_ptr<ISFSObject> params(new SFSObject());
    //							params->PutInt(EXT_FIELD_LOGIN_FIRST, lf);
    //							boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_STATUS_SPIN, params));
    //							GameServer::getSingleton().Send(request);
    //						}
    //					}
    //
    //				}
    //			}
    //
    //		}
    //	}
    //moc3-end
    Button* btnAvatar = dynamic_cast<Button*>(rootNode->getChildByName("btnAvatar"));
    
    if (btnAvatar){
        btnAvatar->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonUserInfo, this));
        btnAvatar->setPressedActionEnabled(true);
    }
    
    this->btnPoker = dynamic_cast<Button*>(rootNode->getChildByName("btnPoker"));
    
    if (this->btnPoker){
        this->btnPoker->setTag(130);
        this->btnPoker->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonMiniGame, this));
        this->btnPoker->setPressedActionEnabled(true);
    }
    this->btnTaiXiu = dynamic_cast<Button*>(rootNode->getChildByName("btnTaiXiu"));
    
    // if (this->btnTaiXiu){
    this->btnTaiXiu->setTag(128);
    this->btnTaiXiu->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonMiniGame, this));
    this->btnTaiXiu->setPressedActionEnabled(true);
    this->btnTaiXiu->setVisible(false);
    this->menu = LayerMenuMiniGame::create();
    this->addChild(this->menu,100000);
    this->menu->setAnchorPoint(this->btnTaiXiu->getAnchorPoint());
    this->menu->setPosition(this->btnTaiXiu->getPosition());
    // }
    
    //    if (!dataManager.getAppConfig().isLoginFisrtDay)
    //        this->onButtonMiniGame(this->btnTaiXiu);
    
    this->btnSapNgua = dynamic_cast<Button*>(rootNode->getChildByName("btnSapNgua"));
    
    
    if (this->btnSapNgua){
        this->btnSapNgua->setTag(133);
        this->btnSapNgua->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonMiniGame, this));
        this->btnSapNgua->setPressedActionEnabled(true);
    }
    
    this->btnCaoThap = dynamic_cast<Button*>(rootNode->getChildByName("btnCaoThap"));
    
    if (this->btnCaoThap){
        this->btnCaoThap->setTag(134);
        this->btnCaoThap->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonMiniGame, this));
        this->btnCaoThap->setPressedActionEnabled(true);
    }
    this->btnVongQuay = dynamic_cast<Button*>(rootNode->getChildByName("btnVongQuay"));
    
    if (this->btnVongQuay){
        this->btnVongQuay->setTag(135);
        this->btnVongQuay->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonMiniGame, this));
        this->btnVongQuay->setPressedActionEnabled(true);
    }
    
    this->txtTX= dynamic_cast<Text*>(rootNode->getChildByName("txtTX"));
    this->txtPK= dynamic_cast<Text*>(this->btnPoker->getChildByName("txtPK"));
    this->txtDD= dynamic_cast<Text*>(this->btnSapNgua->getChildByName("txtDD"));
    this->txtVQ= dynamic_cast<Text*>(this->btnVongQuay->getChildByName("txtVQ"));
    this->txtCT= dynamic_cast<Text*>(this->btnCaoThap->getChildByName("txtCT"));
    
    this->txtTX->setString(dataManager.GetSysString(969));
    this->txtPK->setString(dataManager.GetSysString(859));
    this->txtDD->setString(dataManager.GetSysString(970));
    this->txtVQ->setString(dataManager.GetSysString(971));
    this->txtCT->setString(dataManager.GetSysString(858));
    
    
    
    auto pokerPos = this->btnPoker->getPosition();
    auto xocdiaPos = this->btnSapNgua->getPosition();
    this->btnPoker->setPosition(xocdiaPos);
    this->btnSapNgua->setPosition(pokerPos);
    
    Button* btnSuKien = dynamic_cast<Button*>(rootNode->getChildByName("btnSuKien"));
    
    if (btnSuKien){
        btnSuKien->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonSuKien, this));
        btnSuKien->setPressedActionEnabled(true);
    }
    
    Button* btnNapGold = dynamic_cast<Button*>(rootNode->getChildByName("btnNapGold"));
    
    if (btnNapGold){
        btnNapGold->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onBtnNapGold, this));
        btnNapGold->setPressedActionEnabled(true);
    }
    
    Button* btnNapSilver = dynamic_cast<Button*>(rootNode->getChildByName("btnNapSilver"));
    
    if (btnNapSilver){
        btnNapSilver->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onBtnNapSilver, this));
        btnNapSilver->setPressedActionEnabled(true);
    }
    
    //moc3
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //        this->btnUngTien = dynamic_cast<Button*>(rootNode->getChildByName("btnUngTien"));
    //
    //        if (this->btnUngTien){
    //            this->btnUngTien->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonUngTien, this));
    //            this->btnUngTien->setPressedActionEnabled(true);
    //            this->btnUngTien->setVisible(!dataManager.getAppConfig().ungtien);
    //
    //            boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    //            int acctype = 0;
    //            double money = 0;
    //            if (mySelf != NULL)
    //            {
    //                boost::shared_ptr<UserVariable> acctype_ptr = mySelf->GetVariable("accVipType");
    //                if (acctype_ptr != NULL)
    //                    acctype = *acctype_ptr->GetIntValue();
    //                boost::shared_ptr<UserVariable> amf = mySelf->GetVariable("amf");
    //                if (amf != NULL)
    //                    money = *amf->GetDoubleValue();
    //            }
    //
    //            int minVip = 10 - dataManager.getAppConfig().minungtien.size()+1;
    //
    //            if (acctype >= minVip){
    //                if (money <= dataManager.getAppConfig().minungtien[acctype-minVip]*1000)
    //                    this->btnUngTien->setEnabled(true);
    //                else{
    //                    this->btnUngTien->setEnabled(false);
    //                }
    //            }
    //            else{
    //                this->btnUngTien->setEnabled(false);
    //            }
    //        }
    //    }
    //moc3-end
    ImageView* img = dynamic_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
    if (img != NULL)
    {
        this->circleAvatar = CircleAvatar::create();
        this->circleAvatar->hideIconDefault(false);
        
        float scX = img->getScaleX();
        float scY = img->getScaleY();
        
        Size _sizeImg = img->getContentSize();
        Size _sizeAva = this->circleAvatar->GetSizeAvatar();
        
        scX = _sizeImg.width / _sizeAva.width * scX;
        scY = _sizeImg.height / _sizeAva.height * scY;
        
        this->circleAvatar->setAnchorPoint(Vec2::ZERO);
        //this->circleAvatar->setScale(0.8);
        
        this->circleAvatar->setPosition(img->getPosition());
        this->circleAvatar->setClickCallBack(CC_CALLBACK_0(LayerMainBottom2::onButtonAvatar, this));
        rootNode->addChild(this->circleAvatar, 5);
    }
    
    //moc3
    //add event listenner
    //	auto btnCharge = dynamic_cast<Button*>(rootNode->getChildByName("btnCharge"));
    //	if (btnCharge != NULL){
    //		btnCharge->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonCharge, this));
    //		btnCharge->setPressedActionEnabled(true);
    //	}
    //
    //	auto btnChuyenTien = dynamic_cast<Button*>(rootNode->getChildByName("btnChuyenTien"));
    //	if (btnChuyenTien != NULL){
    //		btnChuyenTien->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonChuyenTien, this));
    //		btnChuyenTien->setPressedActionEnabled(true);
    //		btnChuyenTien->setVisible(!dataManager.getAppConfig().isHideBilling);
    //	}
    //	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //		if (!dataManager.getAppConfig().isHideBilling)
    //			btnChuyenTien->setVisible(!dataManager.getAppConfig().isHideTransfer);
    //	}
    //
    //	//btnDoiThuong = dynamic_cast<Button*>(rootNode->getChildByName("//btnDoiThuong"));
    //	if (//btnDoiThuong != NULL){
    //		//btnDoiThuong->addClickEventListener(CC_CALLBACK_1(LayerMainBottom2::onButtonDoiThuong, this));
    //		//btnDoiThuong->setPressedActionEnabled(true);
    //	}
    //	boost::shared_ptr<ISFSObject> params(new SFSObject());
    //	boost::shared_ptr<IRequest> request(new ExtensionRequest("pmchkrq", params));
    //	GameServer::getSingleton().Send(request);
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
    //        //btnDoiThuong->setEnabled(false);
    //    else
    //        //btnDoiThuong->setVisible(false);
    //moc3-end
    
    spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("sprAvatar"));
    //moc3
    //	this->avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
    //	this->avtVip->setLocalZOrder(7);
    //	this->avtVip->setVisible(false);
    
    //	prgLevelProgress = dynamic_cast<LoadingBar*>(rootNode->getChildByName("prgProgress"));
    //
    //moc3-end
    SceneManager::getSingleton().setIsShowCT(false);
    SceneManager::getSingleton().setIsShowPK(false);
    SceneManager::getSingleton().setIsShowTX(false);
    SceneManager::getSingleton().setIsShowVQ(false);
    SceneManager::getSingleton().setIsShowXD(false);
    
    this->txtTimeTX = dynamic_cast<Text*>(rootNode->getChildByName("txtTimeTX"));
    this->schedule(schedule_selector(LayerMainBottom2::onScheduleTX),1);
    this->imgDoiThuong = dynamic_cast<ImageView*>(rootNode->getChildByName("imgDoiThuong"));
    this->imgDoiThuong->loadTexture(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"imgdoiwing.png");
    this->imgDoiThuong->setVisible(false);
    this->scheduleUpdateInfo(0);
    //loadAllDatas();
    //moc3
    this->onScheduleThongBao(0);
    this->schedule(schedule_selector(LayerMainBottom2::onScheduleThongBao),1);
    //GameServer::getSingleton().addListeners(this);
    //
    //	if (dataManager.getAppConfig().isHideBilling){
    //		Label* versionLabel = Label::createWithTTF("", kNormalFont, 50);
    //		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
    //			versionLabel->setString("");
    //		}
    //		versionLabel->setColor(Color3B::WHITE);
    //		versionLabel->setAnchorPoint(Vec2(0, 0));
    //		versionLabel->setPosition(Vec2(1300, 30));
    //		this->addChild(versionLabel);
    //
    //
    //	}
    
    //moc3-end
    return true;
}
void LayerMainBottom2::logOut(){
    this->txtName->setString("MegaWin");
    if (SceneManager::getSingleton().isNagaCard){
        this->txtName->setString("NagaCard");
    }
    this->txtLevel->setString("LV 100");
    this->txtGold->setString("888.888.888");
    this->txtSilver->setString("888.888.888");
    this->txtVip->setString("VIP 10");
}
void LayerMainBottom2::hideTXMenu(){
//    if (SceneManager::getSingleton().isNagaWin){
//        return;
//    }
    this->btnTaiXiu->setVisible(true);
    this->menu->setVisible(false);
    this->menu->removeListenerServer();
}
void LayerMainBottom2::showTXMenu(){
//    if (SceneManager::getSingleton().isNagaWin){
//        this->hideTXMenu();
//        return;
//    }
    this->btnTaiXiu->setVisible(false);
    this->menu->setVisible(true);
    this->menu->addListenerServer();
}
void LayerMainBottom2::reloadLang(){
    this->imgDoiThuong->loadTexture(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"imgdoiwing.png");
    this->txtTX->setString(dataManager.GetSysString(969));
    this->txtPK->setString(dataManager.GetSysString(859));
    this->txtDD->setString(dataManager.GetSysString(970));
    this->txtVQ->setString(dataManager.GetSysString(971));
    this->txtCT->setString(dataManager.GetSysString(858));
}
void LayerMainBottom2::onScheduleTX(float dt){
    if (this->timerTX>0){
        this->timerTX-=1;
        this->txtTimeTX->setString(StringUtils::format("%d",this->timerTX));
    }
    if (this->timerTX == 0){
        this->timerTX = 60;
    }
}
void LayerMainBottom2::onEnter()
{
    Layer::onEnter();
    //    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //    auto listener = EventListenerTouchOneByOne::create();
    //    listener->setSwallowTouches(true);
    //    listener->onTouchBegan = CC_CALLBACK_2(LayerMainBottom2::onTouchBegan, this);
    //    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //    _touchListener = listener;
    
    GameServer::getSingleton().addListeners(this);
}
bool LayerMainBottom2::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (!SceneManager::getSingleton().isLogin){
        Scene *scene = Director::getInstance()->getRunningScene();
        if (!scene)
            return true;
        LayerLogin* login = LayerLogin::create();
        scene->addChild(login,10000);
        return true;
    }
    return true;
}
void LayerMainBottom2::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //GameServer::getSingleton().removeListeners(this);
    Layer::onExit();
}
void LayerMainBottom2::onBtnNapGold(Ref* pSender){
    
    //    auto _currScene = Director::getInstance()->getRunningScene();
    //    if (!_currScene) return ;
    //    LayerPopupEventThoiVu* top = LayerPopupEventThoiVu::create();
    //    top->setCurrentEvent(0);
    //    _currScene->addChild(top);
    
    SceneManager::getSingleton().showLoading();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (dataManager.getAppConfig().isHideBilling){
        popup->onBtnNapHao(NULL);
    }
    _currScene->addChild(popup,2000,9024);
}
void LayerMainBottom2::onBtnNapSilver(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
    popup->onBtnNapHao(NULL);
    _currScene->addChild(popup,2000,9025);
}
void LayerMainBottom2::onScheduleThongBao(float dt)
{
    auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
    for (int i = 0; i < lstmini.size(); i++){
        if (strcmp(lstmini[i].c_str(), "128") == 0){
            menu->setVisible(false);
            this->txtTX->setVisible(false);
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
        if (strcmp(lstmini[i].c_str(), "134") == 0){
            this->btnSapNgua->setVisible(false);
        }
    }
    bool isShowFB = false;
    auto lstVersion = mUtils::splitString(dataManager.getAppConfig().hidefb, ';');
    vector<string> vsIOS;
    vector<string> vsAndroid;
    string is = "ios";
    string as = "and";
    for (int i = 0; i < lstVersion.size(); i++) {
        string vs = lstVersion[i];
        if (vs.find(is) != std::string::npos) {
            vsIOS.push_back(vs);
        }
        if (vs.find(as) != std::string::npos) {
            vsAndroid.push_back(vs);
        }
    }
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (int i = 0; i < vsIOS.size(); i++) {
        if (strcmp(vsIOS[i].c_str(), dataManager.getAppConfig().version_ios.c_str()) == 0){
            isShowFB = true;
        }
    }
    
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    for (int i = 0; i < vsAndroid.size(); i++) {
        if (strcmp(vsAndroid[i].c_str(), dataManager.getAppConfig().version_android.c_str()) == 0){
            isShowFB = true;
        }
    }
#endif
    
    if (isShowFB){
        this->imgDoiThuong->setVisible(true);
    }
    
    //sprCoin->stopAllActions();
    //sprCoin->runAction(RepeatForever::create(this->animN));
    
}
void LayerMainBottom2::onButtonSuKien(Ref* pSender){
    //    auto _currScene = Director::getInstance()->getRunningScene();
    //    if (!_currScene) return;
    //
    //    LayerPopupEvent* popup = LayerPopupEvent::create();
    //    _currScene->addChild(popup,10000);
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
    if (dataManager.getAppConfig().is24h || dataManager.getAppConfig().isHideBilling)
        popup->onBtnNapHao(NULL);
    _currScene->addChild(popup,2000,9026);
    
}
void LayerMainBottom2::onButtonUngTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    LayerPopupUngTien* popup = LayerPopupUngTien::create();
    _currScene->addChild(popup,10000,9027);
}
void LayerMainBottom2::onButtonCharge(Ref* pSender)
{
    //    ScenePickRoom* _parent2 = (ScenePickRoom*)this->getParent();
    //    if (currScene == 2){
    //        _parent2->gotoChargeMoney();
    //        UserDefault::getInstance()->setBoolForKey("isShowEvent",false);
    //        return;
    //    }
    //	SceneMain* _parent = (SceneMain*)this->getParent();
    //	if (currScene == 1){
    //		_parent->gotoChargeMoney();
    //        UserDefault::getInstance()->setBoolForKey("isShowEvent",false);
    //        return;
    //	}
    SceneMain* _parent = (SceneMain*)this->getParent();
    if (_parent != NULL){
        _parent->gotoChargeMoney();
        UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
        UserDefault::getInstance()->flush();
    }
}
void LayerMainBottom2::onButtonMiniGame(Ref* pSender){
    SceneManager::getSingleton().hideLoading();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    auto node = (Node*)pSender;
    int tag = node->getTag();
    if (tag == 130){
        if (SceneManager::getSingleton().getIsShowPK())
            return;
        LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
        _currScene->addChild(layerPlayerGame,1000,9028);
        SceneManager::getSingleton().setIsShowPK(true);
    }
    else if(tag == 128){
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        _currScene->addChild(layerPlayerGame,1000,9029);
        layerPlayerGame->shouldShowHuongDan();
        SceneManager::getSingleton().setIsShowTX(true);
    } else if(tag == 133){
        if (SceneManager::getSingleton().isNagaWin){
            boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
            if (lastRoom)
            {
                if (0 == lastRoom->GroupId()->compare(boost::to_string(118)))
                {
                    boost::shared_ptr<IRequest> request(new LeaveRoomRequest(lastRoom));
                    GameServer::getSingleton().Send(request);
                }
            }
            SceneManager::getSingleton().setCurrRoomType(0);
            SceneManager::getSingleton().setGameID((kGameBauTom));
            // SceneManager::getSingleton().showLoading();
            SceneManager::getSingleton().gotoPickGame(kGameBauTom);
            return;
        }
        if (SceneManager::getSingleton().getIsShowXD())
            return;
        LayerPlayGameDoDenMini* layerPlayerGame = LayerPlayGameDoDenMini::create();
        _currScene->addChild(layerPlayerGame,1000,9030);
        SceneManager::getSingleton().setIsShowXD(true);
    }
    else if(tag == 134){
        if (SceneManager::getSingleton().getIsShowCT())
            return;
        LayerCaoThapPopupGame* layerPlayerGame = LayerCaoThapPopupGame::create();
        _currScene->addChild(layerPlayerGame,1000,9031);
        SceneManager::getSingleton().setIsShowCT(true);
    }
    else if(tag == 135){
        if (SceneManager::getSingleton().getIsShowVQ())
            return;
        LayerPlayVQMM* layerPlayerGame = LayerPlayVQMM::create();
        _currScene->addChild(layerPlayerGame,1000,9032);
        SceneManager::getSingleton().setIsShowVQ(true);        
    }
    
}
void LayerMainBottom2::onGotoBaoDanh(Ref* pSender){
    // 	int tag = btnBaoDanh->getTag();
    // 	SceneManager::getSingleton().showLoading();
    // 	SceneManager::getSingleton().gotoPickGame(tag);
    auto currScene = this->getParent();
    if (currScene != NULL)
    {
        LayerPopupMission* popup = LayerPopupMission::create();
        //		quaythuongHN->setLuotquay(luotquay);
        currScene->addChild(popup, 10000,9033);
        popup->onBtnTabCongDong(NULL);
    }
}

void LayerMainBottom2::onButtonDoiThuong(Ref* pSender)
{
    // 	SceneMain* _parent = (SceneMain*)this->getParent();
    // 	if (_parent != NULL){
    // 		_parent->gotoChatRoom();
    // 	}
    LayerDoiThuong* layerDoiThuong = LayerDoiThuong::create();
    this->addChild(layerDoiThuong);
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->flush();
}
void LayerMainBottom2::onButtonChuyenTien(Ref* pSender)
{
    
    LayerChuyenTien* layerChuyenTien = LayerChuyenTien::create();
    this->addChild(layerChuyenTien);
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->flush();
}
void LayerMainBottom2::onButtonUserInfo(Ref* pSender)
{
    
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return ;
    //    LayerPopupEventThoiVu* top = LayerPopupEventThoiVu::create();
    //    _currScene->addChild(top, ZORDER_LIST::ZORDER_TAIXIU_MINI);
    //
    LayerPopupProfile* profile = LayerPopupProfile::create();
    if (profile == NULL)
        return;
    profile->setLocalZOrder(2000);
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->flush();
    _currScene->addChild(profile,2000,9034);
    
}

void LayerMainBottom2::loadAllDatas()
{
    
    if (SceneManager::getSingleton().isNagaNew){
        return;
    }
    
    if (!SceneManager::getSingleton().isLogin && !SceneManager::getSingleton().getIsFlagStore()){
        this->txtName->setString("MegaWin");
        if (SceneManager::getSingleton().isNagaCard){
            this->txtName->setString("NagaCard");
        }
        this->txtLevel->setString("LV 100");
        this->txtGold->setString("888.888.888");
        this->txtSilver->setString("888.888.888");
        this->txtVip->setString("VIP 10");
        return;
    }
    
    //	if (dataManager.getAppConfig().isOpenChest) return;
    boost::shared_ptr<double> am, amf,amfs;
    
    boost::shared_ptr<string> aal;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself == NULL ||
        myself->GetVariable("am") == NULL ||
        myself->GetVariable("amf") == NULL
        || myself->GetVariable("aal") == NULL
        || myself->GetVariable("lvl") == NULL
        || myself->GetVariable("exp") == NULL
        || myself->GetVariable("accVipType") == NULL || myself->GetVariable("amfs") == NULL)
    {
        log("myself %s = NULL | myself->GetVariable(am) %s = NULL | myself->GetVariable(amf)%s =NULL",
            myself == NULL ? "=" : "!",
            myself->GetVariable("am") == NULL ? "=" : "!",
            myself->GetVariable("amf") == NULL ? "=" : "!");
        return;
    }
    
    //am = myself->GetVariable("am")->GetDoubleValue();
    amf = myself->GetVariable("amf")->GetDoubleValue();
    amfs = myself->GetVariable("amfs")->GetDoubleValue();
    aal = myself->GetVariable("aal")->GetStringValue();
    int silver = 0;
    int money = 0;
    //Set Name
    string _strName = ChanUtils::GetNamePlayer(SceneManager::getSingleton().getMyName());
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
            NativeHelper::trackLogin(_strName.c_str());
        }
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
            IOS::trackLogin(_strName);
        }
#endif
    }
    std::string _strNameSmall = "";
    (_strName.length() > 10) ? _strNameSmall = _strName.substr(0, 10) + "..." : _strNameSmall = _strName;
    if (_strNameSmall.compare("") != 0)
        txtName->setString(_strNameSmall.c_str());
    this->txtSilver->setString(mUtils::convertMoneyEx(*amfs).c_str());
    txtGold->setString(mUtils::convertMoneyEx(*amf).c_str());
    money = *amf;
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        txtLevel->setString(StringUtils::format("%d", *myself->GetVariable("lvl")->GetIntValue()));
        txtGold->setFontSize(40);
    }
    else{
        txtLevel->setString(StringUtils::format("LV %d", *myself->GetVariable("lvl")->GetIntValue()));
    }
    
    if (this->circleAvatar != NULL)
        this->circleAvatar->setAvatarImage(*aal);
    int accVipType = 0;
    if (myself->GetVariable("accVipType") != NULL){
        accVipType = *myself->GetVariable("accVipType")->GetIntValue();
        //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
        //this->circleAvatar->setTypeVip(accVipType);
        this->txtVip->setString(StringUtils::format("VIP %d",accVipType));
    }
    
    log("DEBUG: jump to LayerMain::loadAllMyDatas()");
    //int _p = dataManager.getLevelProgress(*myself->GetVariable("lvl")->GetIntValue(), *myself->GetVariable("exp")->GetIntValue());
    //prgLevelProgress->setPercent(dataManager.getLevelProgress(*myself->GetVariable("lvl")->GetIntValue(), *myself->GetVariable("exp")->GetIntValue()));
    
    //    int minVip = 10 - dataManager.getAppConfig().minungtien.size()+1;
    //
    //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
    //    if (accVipType >= minVip){
    //        if (money <= dataManager.getAppConfig().minungtien[accVipType-minVip]*1000)
    //            this->btnUngTien->setEnabled(true);
    //        else{
    //            this->btnUngTien->setEnabled(false);
    //        }
    //    }
    //    else{
    //        this->btnUngTien->setEnabled(false);
    //    }
    //    }
    
}

void LayerMainBottom2::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    if (ptrEventParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    if (ptrNotifiedUser->IsItMe() == true)
        loadAllDatas();
}

void LayerMainBottom2::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp("pmchkrs", cmd->c_str()) == 0){
        /*SceneManager::getSingleton().hideLoading();
         boost::shared_ptr<string> pmstt_ptr = param->GetUtfString("pmstt");
         if (pmstt_ptr != NULL)
         {
         <<<<<<< HEAD
         string cf = *pmstt_ptr;
         auto cfs = mUtils::splitString(cf, '|');
         if (cfs.size() > 1){
         bool isShowLocalPayment = false;
         if (strcmp("0", cfs[0].c_str()) == 0)
         isShowLocalPayment = false;
         else
         isShowLocalPayment = true;
         if (!dataManager.getAppConfig().isHideBilling){
         if (dataManager.getAppConfig().isHideExchange){
         //if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
         //btnDoiThuong->setEnabled(false);
         //else
         //btnDoiThuong->setVisible(false);
         }
         else{
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
         if (isShowLocalPayment){
         //btnDoiThuong->setEnabled(isShowLocalPayment);
         }
         else{
         //btnDoiThuong->setVisible(true);
         //btnDoiThuong->setEnabled(false);
         //btnDoiThuong->setBright(false);
         }
         }
         else{
         //btnDoiThuong->setVisible(isShowLocalPayment);
         }
         }
         }
         else{
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
         //btnDoiThuong->setVisible(true);
         //btnDoiThuong->setEnabled(false);
         //btnDoiThuong->setBright(false);
         }
         }
         }
         =======
         string cf = *pmstt_ptr;
         auto cfs = mUtils::splitString(cf, '|');
         if (cfs.size() > 1){
         bool isShowLocalPayment = false;
         if (strcmp("0", cfs[0].c_str()) == 0)
         isShowLocalPayment = false;
         else
         isShowLocalPayment = true;
         if (!dataManager.getAppConfig().isHideBilling){
         if (dataManager.getAppConfig().isHideExchange){
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
         btnDoiThuong->setEnabled(false);
         else
         btnDoiThuong->setVisible(false);
         }
         else{
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
         if (isShowLocalPayment){
         btnDoiThuong->setEnabled(isShowLocalPayment);
         }
         else{
         btnDoiThuong->setVisible(true);
         btnDoiThuong->setEnabled(false);
         btnDoiThuong->setBright(false);
         }
         }
         else{
         btnDoiThuong->setVisible(isShowLocalPayment);
         }
         }
         }
         else{
         if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
         btnDoiThuong->setVisible(true);
         btnDoiThuong->setEnabled(false);
         btnDoiThuong->setBright(false);
         }
         }
         >>>>>>> 1b723e7ba20bd283fb21328961d8eab9b38c9ff4
         }
         }*/
        
    }
    else if (0 == cmd->compare(EXT_EVENT_RES_NUM_SPIN))
    {
        boost::shared_ptr<long> ptr_numspin = param->GetInt(EXT_FIELD_NUM_SPIN);
        if (ptr_numspin)
        {
            luotquay = *ptr_numspin;
            if (luotquay > 0)
            {
                //btnBaoDanhStatus->setVisible(true);
                // 				btnBaoDanhStatus->stopAllActions();
                // 				btnBaoDanhStatus->runAction(RepeatForever::create(ScaleBy::create(1, 1.2, 1.2)));
                btnBaoDanhStatus->loadTextureNormal("notify-chuabaodanh.png");
            }
            else{
                //btnBaoDanhStatus->setVisible(false);
                btnBaoDanhStatus->loadTextureNormal("notify-dabaodanh.png");
            }
        }
    }
}

void LayerMainBottom2::scheduleUpdateInfo(float dt)
{
    loadAllDatas();
}

void LayerMainBottom2::onButtonAvatar()
{
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return ;
    LayerPopupProfile* profile = LayerPopupProfile::create();
    if (profile == NULL)
        return;
    profile->setLocalZOrder(2000);
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    UserDefault::getInstance()->flush();
    _currScene->addChild(profile,2000,9035);
}

