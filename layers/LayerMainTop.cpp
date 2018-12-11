

#include "LayerMainTop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "../layers/LayerPopupSetting.h"
#include "../layers/LayerPromotion.h"
#include "../scenes/SceneMain.h"
#include "../layers/LayerFriendTop.h"
#include "../layers/LayerMailInbox.h"
#include "../SceneManager.h"
#include "../data_manager.h"
#include "../layers/LayerPopupMission.h"
#include "AllData.h"
#include "../layergames/PhomMessDef.h"
#include "../layergames/_Chat_.h"
#include "BagItem.h"
#include "LayerPopupDauTruong.h"
#include "LayerPopupSuKien.h"
#include "../scenes/ScenePickRoom.h"
#include "LayerPopupQuaTang.h"
#include "LayerPopupThongBao.h"
#include "LayerLogin.h"
#include "LayerPopupHotLine.h"
#include "LayerPopupEvent.h"
#include "LayerPopupProfile.h"
#include "LayerPopupCuaHang.h"
#include "LayerPopupDoiMoc.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOS.h"
#endif
USING_NS_CC;
using namespace cocos2d::ui;


LayerMainTop::LayerMainTop()
{
    //pThread1->stopEvent
}

LayerMainTop::~LayerMainTop()
{
	this->unschedule(schedule_selector(LayerMainTop::onScheduleThongBao));

    this->unschedule(schedule_selector(LayerMainTop::onScheduleNotify));

	GameServer::getSingleton().removeListeners(this);
}

bool LayerMainTop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    string path = "LayerMainTopNew.csb";
    if (SceneManager::getSingleton().getIsFlagStore()){
        path = "New_LayerMainTop.csb";
    }
	auto rootNode = CSLoader::getInstance()->createNode(path);
    this->node = rootNode;
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	this->setContentSize(rootNode->getContentSize());

	//add event listenner
    //moc3
    if (SceneManager::getSingleton().getIsFlagStore()){
        auto btnBack = dynamic_cast<Button*>(rootNode->getChildByName("btnBack"));
        if (btnBack != NULL){
            btnBack->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonBack, this));
            btnBack->setPressedActionEnabled(true);
        }
        
        auto btnGif = dynamic_cast<Button*>(rootNode->getChildByName("btnGif"));
        if (btnGif != NULL){
            btnGif->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonGif, this));
            btnGif->setPressedActionEnabled(true);
        }
        btnQuayThuong = dynamic_cast<Button*>(rootNode->getChildByName("btnQuayThuong"));
        if (btnQuayThuong != NULL){
            btnQuayThuong->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonFriend, this));
            btnQuayThuong->setPressedActionEnabled(true);
        }
        auto btnRank = dynamic_cast<Button*>(rootNode->getChildByName("btnRank"));
        if (btnRank != NULL){
            btnRank->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonRank, this));
            btnRank->setPressedActionEnabled(true);
        }
        auto btnNotifi = dynamic_cast<Button*>(rootNode->getChildByName("btnNotifi"));
        if (btnNotifi != NULL){
            btnNotifi->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonNotify, this));
            btnNotifi->setPressedActionEnabled(true);
        }
        auto btnMail = dynamic_cast<Button*>(rootNode->getChildByName("btnMail"));
        if (btnMail != NULL){
            btnMail->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonInbox, this));
            btnMail->setPressedActionEnabled(true);
        }
        auto btnGiaiDau = dynamic_cast<Button*>(rootNode->getChildByName("btnGiaiDau"));
        if (btnGiaiDau != NULL){
            btnGiaiDau->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnGiaiDau, this));
            btnGiaiDau->setPressedActionEnabled(true);
        }
        btnSetting = dynamic_cast<Button*>(rootNode->getChildByName("btnSetting"));
        
        CCLOG("position Button Setting %f X", btnSetting->getPosition().x);
        CCLOG("position Button Setting %f Y", btnSetting->getPosition().y);
        
        if (btnSetting != NULL){
            btnSetting->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonSetting, this));
            btnSetting->setPressedActionEnabled(true);
        }
        
        auto btnHotline = static_cast<Button*>(rootNode->getChildByName("btnHotLine"));
        if (btnHotline != NULL){
            btnHotline->setPressedActionEnabled(true);
            btnHotline->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonHotLineClicked, this));
            btnHotline->setPressedActionEnabled(true);
            btnHotline->setVisible(!dataManager.getAppConfig().isHideBilling);
        }
        
        auto lblHotLine = dynamic_cast<Text*>(rootNode->getChildByName("lblHotLine"));
        
        lblHotLine->setString("" + dataManager.getAppConfig().callcenter);
        lblHotLine->setVisible(false);
        //
        pnlThongBao = dynamic_cast<Layout*>(rootNode->getChildByName("pnlThongBao"));
       // createLabelThongBao("   ");
        
        btnThongBao = dynamic_cast<Button*>(rootNode->getChildByName("btnThongBao"));
        if (btnThongBao){
            btnThongBao->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnThongBaoClick, this));
        }
        btnGioVang = dynamic_cast<Button*>(rootNode->getChildByName("btnGioVang"));
        if (btnGioVang){
            btnGioVang->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnGioVang, this));
        }
        
        
//        bagItem = BagItem::create();
//        this->addChild(bagItem);
//        bagItem->setPosition(btnMail->getPositionX() + 80, btnMail->getPositionY() + 80);
//        bagItem->setVisible(false);
//        
//        
//        bagItem4Promotion = BagItem::create();
//        this->addChild(bagItem4Promotion);
//        this->bagItem4Promotion->setVisible(false);
//        this->bagItem4Promotion->setPosition(btnGif->getPositionX() + 80, btnGif->getPositionY() + 90);
        
        //    Layout* pnlRuong = static_cast<Layout*>(rootNode->getChildByName("pnlRuongVip"));
        
        //    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1) {
        //        menuRuong = LayerMenuRuongVip::create();
        //        menuRuong->setAnchorPoint(pnlRuong->getAnchorPoint());
        //        this->addChild(menuRuong);
        //        menuRuong->setPosition(pnlRuong->getPosition());
        //        menuRuong->setVisible(!dataManager.getAppConfig().ruongvip);
        //    }
        //update 29/7
        return true;
    }
    
	this->btnThoat = dynamic_cast<Button*>(rootNode->getChildByName("btnThoat"));
	if (this->btnThoat != NULL){
        
        //only for china
        //this->btnThoat->setVisible(false);
		this->btnThoat->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonThoat, this));
		this->btnThoat->setPressedActionEnabled(true);
	}

	auto btnQuaTang = dynamic_cast<Button*>(rootNode->getChildByName("btnQuaTang"));
	if (btnQuaTang != NULL){
		btnQuaTang->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonQuaTang, this));
		btnQuaTang->setPressedActionEnabled(true);
	}
	btnQuayThuong = dynamic_cast<Button*>(rootNode->getChildByName("btnQuayThuong"));
	if (btnQuayThuong != NULL){
		btnQuayThuong->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonFriend, this));
		btnQuayThuong->setPressedActionEnabled(true);
	}
	
	
	auto btnHoTro = static_cast<Button*>(rootNode->getChildByName("btnHoTro"));
	if (btnHoTro != NULL){
		btnHoTro->setPressedActionEnabled(true);
		btnHoTro->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonHotLineClicked, this));
		btnHoTro->setPressedActionEnabled(true);
		//btnHoTro->setVisible(!dataManager.getAppConfig().isHideBilling);
	}

    auto btnMail = dynamic_cast<Button*>(rootNode->getChildByName("btnMail"));
    if (btnMail != NULL){
        btnMail->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onButtonInbox, this));
        btnMail->setPressedActionEnabled(true);
    }
	
    this->imgPopMenu = dynamic_cast<Sprite*>(rootNode->getChildByName("imgPopMenu"));
    this->imgPopMenu->setVisible(false);
    auto btnExit = dynamic_cast<Button*>(this->imgPopMenu->getChildByName("btnExit"));
    if (btnExit != NULL){
        btnExit->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnExit, this));
        btnExit->setPressedActionEnabled(true);
    }
    auto btnSetting = dynamic_cast<Button*>(this->imgPopMenu->getChildByName("btnSetting"));
    if (btnSetting != NULL){
        btnSetting->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnSetting, this));
        btnSetting->setPressedActionEnabled(true);
    }
	//
	pnlThongBao = dynamic_cast<Layout*>(rootNode->getChildByName("pnlThongBao"));
    this->_txtThongBao = dynamic_cast<Text*>(pnlThongBao->getChildByName("lblThongBao"));
    createLabelThongBao("   ");

//moc3-end
	btnThongBao = dynamic_cast<Button*>(rootNode->getChildByName("btnThongBao"));
	if (btnThongBao){
		btnThongBao->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnThongBaoClick, this));
	}
//    if (btnGioVang){
//        btnGioVang->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnGioVang, this));
//    }


//	bagItem = BagItem::create();
//	this->addChild(bagItem);
//	bagItem->setPosition(btnMail->getPositionX() + 80, btnMail->getPositionY() + 80);
//	bagItem->setVisible(false);
//
//
//	bagItem4Promotion = BagItem::create();
//	this->addChild(bagItem4Promotion);
//	this->bagItem4Promotion->setVisible(false);
//	this->bagItem4Promotion->setPosition(btnGif->getPositionX() + 80, btnGif->getPositionY() + 90);

//	Layout* pnlRuong = static_cast<Layout*>(rootNode->getChildByName("pnlRuongVip"));

//	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1) {
//		menuRuong = LayerMenuRuongVip::create();
//		menuRuong->setAnchorPoint(pnlRuong->getAnchorPoint());
//		this->addChild(menuRuong);
//		menuRuong->setPosition(pnlRuong->getPosition());
//        menuRuong->setVisible(!dataManager.getAppConfig().ruongvip);
//	}
    
    this->imgThongBao= dynamic_cast<ImageView*>(rootNode->getChildByName("imgThongBao"));
    this->txtQuaTang= dynamic_cast<Text*>(imgThongBao->getChildByName("txtQuaTang"));
    this->imgThongBao->setVisible(false);
    
    this->imgThuMoi= dynamic_cast<ImageView*>(rootNode->getChildByName("imgThuMoi"));
    this->txtThuMoi= dynamic_cast<Text*>(imgThuMoi->getChildByName("txtThuMoi"));
    this->imgThuMoi->setVisible(false);
    
    this->imgSuKien= dynamic_cast<ImageView*>(rootNode->getChildByName("imgSuKien"));
    this->txtSuKien= dynamic_cast<Text*>(this->imgSuKien->getChildByName("txtSuKien"));
    this->imgSuKien->setVisible(false);
    
    auto btnSuKien = dynamic_cast<Button*>(rootNode->getChildByName("btnSuKien"));
    if (btnSuKien){
        btnSuKien->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnSuKien, this));
    }
    

    
    this->schedule(schedule_selector(LayerMainTop::onScheduleThongBao));
	this->schedule(schedule_selector(LayerMainTop::onScheduleUpdateBagItem),60);
    this->onScheduleUpdateBagItem(0);
    
//    this->schedule(schedule_selector(LayerMainTop::onScheduleUpdateGioVang), 60);
//    this->onScheduleUpdateGioVang(0);

	currIndexNotification = 0;

	isShowStatus = false;
    if (SceneManager::getSingleton().isNagaWin){
        this->schedule(schedule_selector(LayerMainTop::onScheduleNotify));
        auto txtName = (Text*)(rootNode->getChildByName("txtName"));
        auto txtMoney = (Text*)(rootNode->getChildByName("txtMoney"));
        auto imgVip = (ImageView*)(rootNode->getChildByName("imgVip"));
        imgVip->setVisible(false);
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return true;

        auto amf = myself->GetVariable("amf");
        auto aN = myself->GetVariable("adn");
        if (aN){
            string adn = *aN->GetStringValue();
            std::string _strNameSmall = "";
            (adn.length() > 10) ? _strNameSmall = adn.substr(0, 20) + "..." : _strNameSmall = adn;
            if (_strNameSmall.compare("") != 0)
                txtName->setString(_strNameSmall.c_str());
            
            
        }
        if (amf){
            txtMoney->setString(mUtils::convertMoneyEx(*amf->GetDoubleValue()));
        }
        
        auto btnProfile = (Button*)(rootNode->getChildByName("btnProfile"));
        if (btnProfile){
            btnProfile->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnProfile, this));
        }
        
        auto btnNapTien = (Button*)(rootNode->getChildByName("btnNapTien"));
        if (btnNapTien){
            btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerMainTop::btnNapTien, this));
        }
        auto btnDoiThuong = (Button*)(rootNode->getChildByName("btnDoiThuong"));
        if (btnDoiThuong){
            btnDoiThuong->addClickEventListener(CC_CALLBACK_1(LayerMainTop::btnDoiThuong, this));
            if (dataManager.getAppConfig().isHideBilling){
                btnDoiThuong->setVisible(false);
            }
        }
        
        this->imgLogo = (Sprite*)(rootNode->getChildByName("logoSpr"));
        
        if (this->imgLogo){
            Vector<SpriteFrame*> animFrames;
            
            string str = "";
            for (int i = 0; i <= 36; i++) {
                str = StringUtils::format("logo_%d.png",i);
                auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 1920, 219));
                animFrames.pushBack(spriteFrame);
            }
            
            Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
            auto animN = Animate::create(animation);
            this->imgLogo->runAction(RepeatForever::create(animN));
        }
        
        auto imgAvatar = (ImageView*)(rootNode->getChildByName("imgAvatar"));
        if (imgAvatar){
            auto aal = myself->GetVariable("aal");
            if (aal){
                string url = *aal->GetStringValue();
                
                if (0 == url.compare("") || 0 == url.compare("null")){
                    UserDefault *def = UserDefault::getInstance();
                    int avatar = def->getIntegerForKey("avatar");
                    string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
                    imgAvatar->loadTexture(src);
                }
                else{
                    string urlCut = mUtils::replaceAll(url, "https", "http");
                    string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
                    imgAvatar->loadTexture(src);
                }
                
            }

        }
        Text* txtVip = (Text*)(rootNode->getChildByName("txtVip"));
        if (txtVip){
            if (myself->GetVariable("accVipType") != NULL){
                int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
                //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
                //this->circleAvatar->setTypeVip(accVipType);
                txtVip->setString(StringUtils::format("VIP %d",accVipType));
            }
        }
        auto txtDoiThuong = (Text*)(rootNode->getChildByName("txtDoiThuong"));
        txtDoiThuong->setString(dataManager.GetSysString(596));
        
        auto txtCaiDat = (Text*)(rootNode->getChildByName("txtCaiDat"));
        txtCaiDat->setString(dataManager.GetSysString(749));
        
        auto txtCaiDat_0 = (Text*)(rootNode->getChildByName("txtCaiDat_0"));
        txtCaiDat_0->setString(dataManager.GetSysString(1017));
        
        auto txtCaiDat_0_0 = (Text*)(rootNode->getChildByName("txtCaiDat_0_0"));
        txtCaiDat_0_0->setString(dataManager.GetSysString(761));
        
//        auto txtCaiDat_0_0_0 = (Text*)(rootNode->getChildByName("txtCaiDat_0_0_0"));
//        txtCaiDat_0_0_0->setString(dataManager.GetSysString(596));
    }
    
    if (SceneManager::getSingleton().isNagaNew){
        this->schedule(schedule_selector(LayerMainTop::onScheduleNotify));
        auto txtName = (Text*)(rootNode->getChildByName("txtName"));
        txtName->setString("YakClub");
        auto txtMoney = (Text*)(rootNode->getChildByName("txtGold"));
        auto txtSilver = (Text*)(rootNode->getChildByName("txtSilver"));

        Text* txtVip = (Text*)(rootNode->getChildByName("txtVip"));
        txtVip->setString("VIP 10");
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return true;
        
        auto amf = myself->GetVariable("amf");
        auto amfs = myself->GetVariable("amfs");

        auto aN = myself->GetVariable("adn");
        if (aN){
            string adn = *aN->GetStringValue();
            std::string _strNameSmall = "";
            (adn.length() > 10) ? _strNameSmall = adn.substr(0, 20) + "..." : _strNameSmall = adn;
            if (_strNameSmall.compare("") != 0)
                txtName->setString(_strNameSmall.c_str());
            
            
        }
        if (amf){
            txtMoney->setString(mUtils::convertMoneyEx(*amf->GetDoubleValue()));
        }
        if (amfs){
            txtSilver->setString(mUtils::convertMoneyEx(*amfs->GetDoubleValue()));
        }
        
        auto btnProfile = (Button*)(rootNode->getChildByName("btnProfile"));
        if (btnProfile){
            btnProfile->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnProfile, this));
        }
        
        auto btnNapTien = (Button*)(rootNode->getChildByName("btnNapTien"));
        if (btnNapTien){
            btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerMainTop::btnNapTien, this));
            btnNapTien->setTag(0);
        }
        auto btnDoiThuong = (Button*)(rootNode->getChildByName("btnDoiThuong"));
        if (btnDoiThuong){
            btnDoiThuong->addClickEventListener(CC_CALLBACK_1(LayerMainTop::btnDoiThuong, this));
        }
        
        //this->imgLogo = (ImageView*)(rootNode->getChildByName("imgLogo"));
        
        auto imgAvatar = (ImageView*)(rootNode->getChildByName("imgAvatar"));
        if (imgAvatar){
            auto aal = myself->GetVariable("aal");
            if (aal){
                string url = *aal->GetStringValue();
                
                if (0 == url.compare("") || 0 == url.compare("null")){
                    UserDefault *def = UserDefault::getInstance();
                    int avatar = def->getIntegerForKey("avatar");
                    string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
                    imgAvatar->loadTexture(src);
                }else{
                    string urlCut = mUtils::replaceAll(url, "https", "http");
                    string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
                    imgAvatar->loadTexture(src);
                }

                
               
            }else{
                    UserDefault *def = UserDefault::getInstance();
                    int avatar = def->getIntegerForKey("avatar");
                    string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
                imgAvatar->loadTexture(src);
                

            }
            
        }
     
        if (txtVip){
            if (myself->GetVariable("accVipType") != NULL){
                int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
                //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
                //this->circleAvatar->setTypeVip(accVipType);
                txtVip->setString(StringUtils::format("VIP %d",accVipType));
            }
        }
        
        auto btnNapGold = (Button*)(rootNode->getChildByName("btnNapGold"));
        if (btnNapGold){
            btnNapGold->setTag(1);
            btnNapGold->addClickEventListener(CC_CALLBACK_1(LayerMainTop::btnNapTien, this));
        }
       
    }
    this->btnBack = (Button*)(rootNode->getChildByName("btnBack"));
    if (this->btnBack){
        this->btnBack->addClickEventListener(CC_CALLBACK_1(LayerMainTop::onBtnBack, this));
        this->btnBack->setVisible(false);
    }
    auto sprDt = (Sprite*)(rootNode->getChildByName("sprDt"));
    if (sprDt){
        sprDt->runAction(RepeatForever::create(RotateBy::create(0.05, 40)));
        if (dataManager.getAppConfig().isHideBilling){
            sprDt->setVisible(false);
        }
    }

	return true;
}
void LayerMainTop::loadDatas(){
    if (SceneManager::getSingleton().isNagaNew){
        auto txtMoney = (Text*)(node->getChildByName("txtGold"));
        auto txtSilver = (Text*)(node->getChildByName("txtSilver"));
        auto txtName = (Text*)(node->getChildByName("txtName"));
        txtName->setString("YakClub");
        boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
        if (myself == NULL)
            return;
        auto aN = myself->GetVariable("adn");
        if (aN){
            string adn = *aN->GetStringValue();
            std::string _strNameSmall = "";
            (adn.length() > 10) ? _strNameSmall = adn.substr(0, 20) + "..." : _strNameSmall = adn;
            if (_strNameSmall.compare("") != 0)
                txtName->setString(_strNameSmall.c_str());
            
            
        }
        auto amf = myself->GetVariable("amf");
        auto amfs = myself->GetVariable("amfs");
        
        if (amf){
            txtMoney->setString(mUtils::convertMoneyEx(*amf->GetDoubleValue()));
        }
        if (amfs){
            txtSilver->setString(mUtils::convertMoneyEx(*amfs->GetDoubleValue()));
        }
        Text* txtVip = (Text*)(node->getChildByName("txtVip"));
        if (txtVip){
            if (myself->GetVariable("accVipType") != NULL){
                int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
                //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
                //this->circleAvatar->setTypeVip(accVipType);
                txtVip->setString(StringUtils::format("VIP %d",accVipType));
            }
        }
        auto imgAvatar = (ImageView*)(node->getChildByName("imgAvatar"));
        if (imgAvatar){
            auto aal = myself->GetVariable("aal");
            if (aal){
                string url = *aal->GetStringValue();
                string urlCut = mUtils::replaceAll(url, "https", "http");
                string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
                imgAvatar->loadTexture(src);
            }
            
        }
    }
}
void LayerMainTop::onButtonBack(Ref* pSender)
{
    SceneMain* _scene = (SceneMain*)this->getParent();
    _scene->onButtonBackPress();
}
void LayerMainTop::onButtonGif(Ref* pSender)
{

    SceneManager::getSingleton().showToast("Chức năng đang được hoàn thiện");
  
}
void LayerMainTop::onBtnBack(Ref* pSender){
    ScenePickRoom* _scene1 = (ScenePickRoom*)this->getParent();
    if (_scene1){
        _scene1->unSubscribeGame();
        SceneManager::getSingleton().setGameID(-1);
        SceneManager::getSingleton().gotoMain();
    }
}
void LayerMainTop::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
    if (ptrEventParams == NULL)
        return;
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    if (ptrNotifiedUser->IsItMe() == true)
    {
        if (SceneManager::getSingleton().isNagaWin ){
            auto txtMoney = (Text*)(node->getChildByName("txtMoney"));

            boost::shared_ptr<User> myself = ptrNotifiedUser;
            if (myself == NULL)
                return ;
            
            auto amf = myself->GetVariable("amf");

            if (amf){
                txtMoney->setString(mUtils::convertMoneyEx(*amf->GetDoubleValue()));
            }
    
            Text* txtVip = (Text*)(node->getChildByName("txtVip"));
            if (txtVip){
                if (myself->GetVariable("accVipType") != NULL){
                    int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
                    //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
                    //this->circleAvatar->setTypeVip(accVipType);
                    txtVip->setString(StringUtils::format("VIP %d",accVipType));
                }
            }
            auto imgAvatar = (ImageView*)(node->getChildByName("imgAvatar"));
            if (imgAvatar){
                auto aal = myself->GetVariable("aal");
                if (aal){
                    string url = *aal->GetStringValue();

                    if (0 == url.compare("") || 0 == url.compare("null")){
                        UserDefault *def = UserDefault::getInstance();
                        int avatar = def->getIntegerForKey("avatar");
                        string src= StringUtils::format("avatars/avatar/avatar_%d.png",avatar);
                        imgAvatar->loadTexture(src);
                    }
                    else{
                        string urlCut = mUtils::replaceAll(url, "https", "http");
                        string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
                        imgAvatar->loadTexture(src);
                    }
                    
                    
                }
                
            }
        }
        if (SceneManager::getSingleton().isNagaNew){
            auto txtMoney = (Text*)(node->getChildByName("txtGold"));
            auto txtSilver = (Text*)(node->getChildByName("txtSilver"));
            
            boost::shared_ptr<User> myself = ptrNotifiedUser;
            if (myself == NULL)
                return ;
            
            auto amf = myself->GetVariable("amf");
            auto amfs = myself->GetVariable("amfs");
            
            if (amf){
                txtMoney->setString(mUtils::convertMoneyEx(*amf->GetDoubleValue()));
            }
            if (amfs){
                txtSilver->setString(mUtils::convertMoneyEx(*amfs->GetDoubleValue()));
            }
            Text* txtVip = (Text*)(node->getChildByName("txtVip"));
            if (txtVip){
                if (myself->GetVariable("accVipType") != NULL){
                    int accVipType = *myself->GetVariable("accVipType")->GetIntValue();
                    //avtVip->loadTexture(mUtils::getSrcVip(accVipType));
                    //this->circleAvatar->setTypeVip(accVipType);
                    txtVip->setString(StringUtils::format("VIP %d",accVipType));
                }
            }
            auto imgAvatar = (ImageView*)(node->getChildByName("imgAvatar"));
            if (imgAvatar){
                auto aal = myself->GetVariable("aal");
                if (aal){
                    string url = *aal->GetStringValue();
                    string urlCut = mUtils::replaceAll(url, "https", "http");
                    string src= StringUtils::format("avatars/avatar/%s",mUtils::getFileName(url).c_str());
                    imgAvatar->loadTexture(src);
                }
                
            }
        }
    }
}
void LayerMainTop::btnDoiThuong(Ref* pSender){
   
    
    auto _currScene = Director::getInstance()->getRunningScene();

    if (!_currScene) return;
    
    LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
    if (dataManager.getAppConfig().is24h || dataManager.getAppConfig().isHideBilling)
        popup->onBtnNapHao(NULL);
    _currScene->addChild(popup,2000,9026);
}
void LayerMainTop::onBtnProfile(Ref* pSender){
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
void LayerMainTop::btnNapTien(Ref* pSender){
    if (dataManager.getAppConfig().isHideBilling){
        SceneManager::getSingleton().showLoading();
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
        popup->onBtnNapThe(NULL);
        if (dataManager.getAppConfig().isHideBilling){
            popup->onBtnNapHao(NULL);
        }
        _currScene->addChild(popup,2000,9024);
        return;
    }
    SceneManager::getSingleton().showLoading();
    auto btn = (Button*)pSender;
    int tag = btn->getTag();
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (tag == 1){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        LayerPopupDoiMoc* popup = LayerPopupDoiMoc::create();
        popup->onBtnNapHao(NULL);
        _currScene->addChild(popup,2000,9025);
        return;
    }
    
    _currScene->addChild(popup,2000,9024);
}
void LayerMainTop::onBtnSuKien(Ref* pSender){
    
    if (SceneManager::getSingleton().lstEvent.size()<=0){
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
        
        Chat* popup = Chat::create(dataManager.GetSysString(436),-1);
        _currScene->addChild(popup,10000,9041);
        return;
    }
    
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h || !SceneManager::getSingleton().isLogin)
    {
        return;
    }
        auto _currScene = Director::getInstance()->getRunningScene();
        if (!_currScene) return;
    
        LayerPopupEvent* popup = LayerPopupEvent::create();
        _currScene->addChild(popup,10000,9042);
}
void LayerMainTop::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(false);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerMainTop::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    
    GameServer::getSingleton().addListeners(this);
}
bool LayerMainTop::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
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
void LayerMainTop::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
   // GameServer::getSingleton().removeListeners(this);
}
void LayerMainTop::onScheduleUpdateGioVang(float dt){
    
    //offer
    boost::shared_ptr<ISFSObject> params(new SFSObject());
    boost::shared_ptr<IRequest> request(new ExtensionRequest("eeglsorq",params));
    GameServer::getSingleton().Send(request);
}
void LayerMainTop::onBtnGioVang(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    if (this->getChildByTag(89098)){
        this->removeChildByTag(89098);
        return;
    }
    ///this->btnGioVang->setEnabled(false);

    auto _layer = LayerPopupSuKien::create();
    //_layer->setPosition(Vec2(1393,HEIGHT_DESIGN-481));
    _layer->setTag(89098);
    _currScene->addChild(_layer, 10000,9043);
}
void LayerMainTop::onBtnGiaiDau(Ref* pSender){
    auto currScene = Director::getInstance()->getRunningScene();
    if (currScene != NULL)
    {
        LayerPopupDauTruong* layer = LayerPopupDauTruong::create();
        currScene->addChild(layer);
    }
}
void LayerMainTop::onBtnThongBaoClick(Ref* pSender)
{
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h || !SceneManager::getSingleton().isLogin)
        return;
	bool isExist;
	if (dataManager.getAppConfig().isHideBilling)return;
	auto _currScene = Director::getInstance()->getRunningScene();
	if (!_currScene) return;
	/*getUser*/
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL || myself->Name() == NULL)
		return;
	string strlink = "";
	if (SceneManager::getSingleton().gListNotification.size() > 0){
		strlink = SceneManager::getSingleton().gListNotification[0].linkShow;
		if (0 != strlink.compare(""))
		{
			isExist = isCheckURL(strlink);
			if (isExist){
				strlink += "&username=" + *myself->Name() + "&checksum=" + mUtils::getMD5KeyHis(*myself->Name());
			}
			else{
				strlink += "?username=" + *myself->Name() + "&checksum=" + mUtils::getMD5KeyHis(*myself->Name());
			}
            LayerPopupWebView* _layer = LayerPopupWebView::create();
            _layer->setWebView(strlink);
            _currScene->addChild(_layer, 2000,9044);
		}

		
	}
}
void LayerMainTop::onBtnExit(Ref* pSender){
    SceneMain* _scene = (SceneMain*)this->getParent();
    if (_scene)
        _scene->onButtonBackPress();
    this->imgPopMenu->setVisible(false);
}
void LayerMainTop::onBtnSetting(Ref* pSender){
    this->imgPopMenu->setVisible(false);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    LayerSettingMain* _layer = LayerSettingMain::create();
    UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    CCLOG("LayerPopupSetting size %f width:", _layer->getContentSize().width);
    CCLOG("LayerPopupSetting size %f height:", _layer->getContentSize().height);
    this->getParent()->getParent()->addChild(_layer, 10000);
}
void LayerMainTop::onButtonThoat(Ref* pSender)
{
    if (SceneManager::getSingleton().isNagaWin || SceneManager::getSingleton().isNagaNew){
        Scene* currScene = Director::getInstance()->getRunningScene();
        if (currScene == NULL) return;
        Size visibleSize = Director::getInstance()->getVisibleSize();
        LayerSettingMain* _layer = LayerSettingMain::create();
        UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
        CCLOG("LayerPopupSetting size %f width:", _layer->getContentSize().width);
        CCLOG("LayerPopupSetting size %f height:", _layer->getContentSize().height);
        currScene->addChild(_layer, 2000,2000);
        return;
    }
    
    if (this->currentParent == 0){
        if (this->imgPopMenu->isVisible()){
            this->imgPopMenu->setVisible(false);
        }else{
            this->imgPopMenu->setVisible(true);
        }
        
    }
    if (this->currentParent == 1){
        ScenePickRoom* _scene1 = (ScenePickRoom*)this->getParent();
        if (_scene1){
            _scene1->unSubscribeGame();
            SceneManager::getSingleton().setGameID(-1);
            SceneManager::getSingleton().gotoMain();
        }
    }
}
void LayerMainTop::onButtonQuaTang(Ref* pSender)
{

	//
    Scene* currScene = Director::getInstance()->getRunningScene();
    if (currScene == NULL) return;
	LayerPopupQuaTang* layer = LayerPopupQuaTang::create();
    layer->onBtnQuaTang(NULL);
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	Size s = Size(0, 0);
	if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameXiTo){
		s = mUtils::getSizePos();
		layer->setPosition(Vec2(s.width, s.height));
	}
	currScene->addChild(layer,2000,9045);
}
void LayerMainTop::gotoRuongVip()
{
	//
	SceneMain* _scene = (SceneMain*)this->getParent();
	_scene->gotoRuongVip();

}

void LayerMainTop::onButtonFriend(Ref* pSender)
{
	if (dataManager.getAppConfig().isHideBilling){
		Chat* toast = Chat::create(dataManager.GetSysString(436), -1);
		SceneMain* _scene = (SceneMain*)this->getParent();
		_scene->addChild(toast,2000);
		return;
	}
	SceneMain* _scene = (SceneMain*)this->getParent();
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);

	_scene->gotoEvent();
}
void LayerMainTop::onButtonRank(Ref* pSender)
{
    if (dataManager.getAppConfig().isHideBilling){
        Chat* toast = Chat::create(dataManager.GetSysString(437), -1);
        SceneMain* _scene = (SceneMain*)this->getParent();
        _scene->addChild(toast,2000);
        return;
    }

	SceneMain* _scene = (SceneMain*)this->getParent();
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	_scene->gotoRank();
}
void LayerMainTop::onButtonNotify(Ref* pSender)
{
	if (dataManager.getAppConfig().isHideBilling){
		Chat* toast = Chat::create(dataManager.GetSysString(438), -1);
		SceneMain* _scene = (SceneMain*)this->getParent();
		_scene->addChild(toast,2000);
		return;
	}
	SceneMain* _scene = (SceneMain*)this->getParent();
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	_scene->gotoNotifcation();
}
void LayerMainTop::onButtonInbox(Ref* pSender)
{
	
	SceneMain* _scene = (SceneMain*)this->getParent();
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	_scene->gotoMailInbox();
}
void LayerMainTop::onButtonSetting(Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	LayerSettingMain* _layer = LayerSettingMain::create();
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	CCLOG("LayerPopupSetting size %f width:", _layer->getContentSize().width);
	CCLOG("LayerPopupSetting size %f height:", _layer->getContentSize().height);
	this->getParent()->getParent()->addChild(_layer, 10);
}
void LayerMainTop::onScheduleUpdateBagItem(float dt)
{

    
    boost::shared_ptr<IRequest> request(new ExtensionRequest("cmi", NULL));//count msg inbox
    GameServer::getSingleton().Send(request);

}
void LayerMainTop::onScheduleThongBao(float dt)
{
    if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h || !SceneManager::getSingleton().isLogin)
    {
        return;
    }
    numEvent = UserDefault::getInstance()->getIntegerForKey("numEvent");

	if (numEvent > 0 && !this->isSet){
        this->imgSuKien->setVisible(true);
        this->txtSuKien->setString(StringUtils::format("%d", numEvent));
        isShowStatus = true;
        //numEvent = 0;
        this->isSet = true;
	}
	if (numEvent<0 && !this->isSet){
        this->imgSuKien->setVisible(false);
	}

	 	int x = lblThongBao->getPositionX();
        if (x < -1 * lblThongBao->getContentSize().width){
	 		x = pnlThongBao->getContentSize().width;
        }
	 	else
	 		x = lblThongBao->getPositionX() - 1;
	 	lblThongBao->setPositionX(x);
	if (x < -1 * lblThongBao->getContentSize().width || lblThongBao->getString().compare("   ") == 0) {
        
        
		x = pnlThongBao->getContentSize().width;
		//
		currIndexNotification++;
		if (currIndexNotification >= SceneManager::getSingleton().gListNotification.size())
			currIndexNotification = 0;
		string _strMsg = "   ";
		if (SceneManager::getSingleton().gListNotification.size() > 0)
			_strMsg = SceneManager::getSingleton().gListNotification[currIndexNotification].textShow;
		//
		//_strMsg = "Chào mừng bạn tới baifvip. Chúc bạn chơi game vui vẻ!";
		txtThongBao = _strMsg;
        lblThongBao->setString(_strMsg);
	}
	else
		x = lblThongBao->getPositionX() - 1;
	lblThongBao->setPositionX(x);
}
void LayerMainTop::onScheduleNotify(float dt)
{
    if (this->richText == NULL){
        if (this->stringNotify.size()>0){
            this->lblThongBao->setVisible(false);
            this->createNotify(this->stringNotify[this->stringNotify.size()-1]);
            this->stringNotify.pop_back();
            if (this->richText == NULL) return;
            int x = this->richText->getPositionX();
            if (x < -1 * this->richText->getContentSize().width){
                x = this->richText->getContentSize().width;
                if (this->stringNotify.size()>0){
                    richText->removeFromParent();
                    richText = NULL;
                }
            }
            else{
                x = this->richText->getPositionX() - 2;
            }
            this->richText->setPositionX(x);
        }
        else{
            this->lblThongBao->setVisible(true);
        }
    }else{
        this->lblThongBao->setVisible(false);

        int x = this->richText->getPositionX();

        if (x < -1 * this->richText->getContentSize().width){
            x = this->richText->getContentSize().width;
                richText->removeFromParent();
                richText = NULL;
            return;
        }
        else{
            x = this->richText->getPositionX() - 2;
        }
        this->richText->setPositionX(x);
    }
    
    
}
void LayerMainTop::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return;
    
    else if (0 == cmd->compare(EVENT_NOTIFY_MESSAGE_HANDLE))
    {
        
        boost::shared_ptr<string> ntfmsg = param->GetUtfString(EXT_FIELD_MESSAGE_STRING);
        if (ntfmsg)
        {
            auto lst = mUtils::splitString(*ntfmsg,'/');
            if (lst.size()<2) return;
            string content = "";
            if (SceneManager::getSingleton().currLang== 0){
                content = lst[0];
            }else if (SceneManager::getSingleton().currLang== 2){
                content = lst[1];
            }
            if (lst.size()>=3 && SceneManager::getSingleton().currLang== 1){
                content = lst[2];
            }
            if (lst.size()>=4 && SceneManager::getSingleton().currLang== 4){
                content = lst[3];
            }
            this->stringNotify.push_back(content);
        }
        
    }
    
    if (strcmp("cmi", cmd->c_str()) == 0){
        if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h)
            return;
        if (!param->GetInt("slmin")) return;
        int count = *param->GetInt("slmin");
        if (count == 0){
            this->imgThuMoi->setVisible(false);
            return;
        }
        this->imgThuMoi->setVisible(true);
        this->txtThuMoi->setString(boost::to_string(count));
    }
    else if (strcmp("eeglsors", cmd->c_str()) == 0){
        if (dataManager.getAppConfig().isHideBilling || dataManager.getAppConfig().is24h)
            return;
       // SceneManager::getSingleton().hideLoading();
        //4|Tặng 500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;11|Tặng 1.500 Vàng & khuyến mãi 5% Giá trị thẻ nạp duy nhất cho Thẻ nạp đầu tiên nếu nạp thẻ thành công bất kỳ mệnh giá|2|1|0;1|Tặng 100 Vàng + 100 EXP|1|2|0;
        if (param->GetUtfString("lso") == NULL) return;
        string lso = *param->GetUtfString("lso");
        auto lst = mUtils::splitString(lso, '/');
        if (lst.size()<2 ) return;
        int num = atoi(lst[1].c_str());
        if (num == 0){
            this->imgThongBao->setVisible(false);
            return;
        }
        this->imgThongBao->setVisible(true);
        this->txtQuaTang->setString(StringUtils::format("%d",num));
    
        //        if (this->lstOffer.size()>0){
        //            this->txtThongBao->setString("Chúc mừng, Bạn nhận được quà tặng bất ngờ từ Mộc Quán!");
        //        }else{
        //            this->txtThongBao->setString("Hiện tại bạn không nhận được phần thưởng nào từ Mộc quán!");
        //
        //        }
    }
    //moc3
//	if (strcmp("cmi", cmd->c_str()) == 0){
//		if (dataManager.getAppConfig().isHideBilling)
//			return;
//		if (!param->GetInt("slmin")) return;
//		int count = *param->GetInt("slmin");
//		if (count == 0){
//			bagItem->setVisible(false);
//			return;
//		}
//		bagItem->setVisible(true);
//		bagItem->stopAllActions();
//		bagItem->setNumber(boost::to_string(count));
//		bagItem->setVisible(true);
//		bagItem->runAction(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 1), NULL));// cc.sequence(cc.scaleTo(0.5, 1.2), cc.scaleTo(0.5, 1)));
//	}
//    else if(strcmp(EXT_EVENT_GET_EVENT_MISSION_RESP, cmd->c_str()) == 0){
//        
//        if (!param->GetUtfString(EVENT_MISSION_RESULT_LIST)) return;
//        string strGioVang = *param->GetUtfString(EVENT_MISSION_RESULT_LIST);
//        
//       // log("giờ vang: %s",strGioVang.c_str());
//        auto lstauto = mUtils::splitString(strGioVang,';');
//        int numsukien = 0;
//        for (int i = 0; i< lstauto.size(); i++) {
//            auto lstitem = mUtils::splitString(lstauto[i],'|');
//            if (lstitem.size()<7) {
//                continue;
//            }
//            if (atoi(lstitem[6].c_str()) == 1 ) {
//                numsukien++;
//            }
//            
//            
//        }
//        if (numsukien > 0){
//            this->btnGioVang->setVisible(true);
//            this->btnGioVang->setEnabled(true);
//        }else{
//            this->removeChildByTag(89098);
//            this->btnGioVang->setVisible(false);
//
//        }
//    }
    //moc3-end
}
void LayerMainTop::onButtonHotLineClicked(Ref* pSender){
    

    
    
    Scene* currScene = Director::getInstance()->getRunningScene();
    if (currScene == NULL) return;
    
    if (dataManager.getAppConfig().isHideBilling){
        LayerPopupThongBao* layerPopup = LayerPopupThongBao::create();
        layerPopup->setMessage(dataManager.GetSysString(615));
        layerPopup->setContentMess("Email : winplaycshkh68@gmail.com");
        layerPopup->showAButton();
        currScene->addChild(layerPopup,10000,9046);
        return;
    }
    
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
    LayerPopupHotLine* layerPopup = LayerPopupHotLine::create();
    currScene->addChild(layerPopup,2000,9047);

}

void LayerMainTop::createLabelThongBao(string _str)
{
    //moc3
    //if (this->pnlThongBao->getChildByTag(9587) != NULL) return;
	lblThongBao = Label::createWithSystemFont(_str, "", 40);
	lblThongBao->setAnchorPoint(this->_txtThongBao->getAnchorPoint());
	lblThongBao->setPosition(this->_txtThongBao->getPosition());
	lblThongBao->setVisible(!dataManager.getAppConfig().isHideBilling);
	this->pnlThongBao->addChild(lblThongBao);
    lblThongBao->setTag(9578);
    //moc3-end
}
void LayerMainTop::createNotify(string _str){
    string str = _str;
    auto lst = mUtils::splitString(str,'|');
    
    string content_msg = "";
    
    if (lst.size() == 7){
        content_msg = lst[0] + " " + lst[1] + " "+ lst[2] + " "+ lst[3] + " " +lst[4] + " " +lst[5];
        richText = RichText::create();
        richText->setAnchorPoint(this->_txtThongBao->getAnchorPoint());
        richText->setPosition(this->_txtThongBao->getPosition());
        richText->setVisible(!dataManager.getAppConfig().isHideBilling);
        richText->ignoreContentAdaptWithSize(false);
        richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
        
        richText->setContentSize(Size(1500, 50));

        RichElementText *re1 = RichElementText::create(1, Color3B(0,177,82), 255, lst[0] + " ", "", 35);
        richText->pushBackElement(re1);
        
        RichElementText *re2 = RichElementText::create(1, Color3B::WHITE, 255, lst[1] + " ", "", 35);
        richText->pushBackElement(re2);
        
        RichElementText *re3 = RichElementText::create(1, Color3B(0,177,82), 255, lst[2] + " ", "", 35);
        richText->pushBackElement(re3);
        
        RichElementText *re4 = RichElementText::create(1, Color3B::YELLOW, 255, lst[3] + " ", "", 35);
        richText->pushBackElement(re4);
        
        RichElementText *re5 = RichElementText::create(1, Color3B(0,177,82), 255, lst[4] + " ", "", 35);
        richText->pushBackElement(re5);
        
        RichElementText *re6 = RichElementText::create(1, Color3B(225,239,216), 255, lst[5] + " ", "", 35);
        richText->pushBackElement(re6);
        
        RichElementText *re7 = RichElementText::create(1, Color3B(225,239,216), 255, lst[6], "", 35);
        richText->pushBackElement(re7);
        
        richText->formatText();
        richText->setTag(123);
        this->pnlThongBao->addChild(richText);

    }
    if (lst.size() == 8){
        content_msg = lst[0] + " " + lst[1] + " "+ lst[2] + " "+ lst[3] + " " +lst[4] + " " +lst[5]+ " " +lst[6];
        richText = RichText::create();
        richText->setAnchorPoint(this->_txtThongBao->getAnchorPoint());
        richText->setPosition(this->_txtThongBao->getPosition());
        richText->setVisible(!dataManager.getAppConfig().isHideBilling);
        richText->ignoreContentAdaptWithSize(false);
        richText->setWrapMode(ui::RichText::WRAP_PER_CHAR);
        
        richText->setContentSize(Size(1400, 50));
        
        RichElementText *re1 = RichElementText::create(1, Color3B(0,177,82), 255, lst[0] + " ", "", 35);
        richText->pushBackElement(re1);
        
        RichElementText *re2 = RichElementText::create(1, Color3B::WHITE, 255, lst[1] + " ", "", 35);
        richText->pushBackElement(re2);
        
        RichElementText *re3 = RichElementText::create(1, Color3B(0,177,82), 255, lst[2] + " ", "", 35);
        richText->pushBackElement(re3);
        
        RichElementText *re4 = RichElementText::create(1, Color3B::YELLOW, 255, lst[3] + " ", "", 35);
        richText->pushBackElement(re4);
        
        RichElementText *re5 = RichElementText::create(1, Color3B(0,177,82), 255, lst[4] + " ", "", 35);
        richText->pushBackElement(re5);
        
        RichElementText *re6 = RichElementText::create(1, Color3B(225,239,216), 255, lst[5] + " ", "", 35);
        richText->pushBackElement(re6);
        RichElementText *re7 = RichElementText::create(1, Color3B(225,239,216), 255, lst[6]+ " ", "", 35);
        richText->pushBackElement(re7);
        RichElementText *re8 = RichElementText::create(1, Color3B::RED,255, lst[7], "", 35);
        richText->pushBackElement(re8);
        
        richText->formatText();
        richText->setTag(123);
        
        this->pnlThongBao->addChild(richText);

    }
}
void LayerMainTop::setValueMenuRuongVip(string value, string num){
	//menuRuong->setValueChest(value, num);
}

bool LayerMainTop::isCheckURL(string url){
	bool isExits = true;
	int pos = url.find('?');
	if (pos == -1){
		isExits = false;
	}

	return isExits;
}
