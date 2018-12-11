#include "LayerMainContent.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../AllData.h"
#include "LayerPopupChonDongTien.h"
#include "SceneManager.h"
#include "Requests/ExtensionRequest.h"
#include "../scenes/ScenePickRoom.h"
#include "layergames/PhomMessDef.h"
#include "../layergames/LayerPlayGameMiniPoker.h"
#include "../layergames/LayerPlayGameDoDenMini.h"
#include "../layergames/LayerCaoThapPopupGame.h"
#include "../layergames/LayerPlayVQMM.h"
#include "../layergames/LayerPlaySlotMachine.h"
#include "../layergames/LayerPlayGameTXMini.h"
#include "../mUtils.h"
#include "LayerLogin.h"
#include "LayerPopupCuaHang.h"
#include "../data_manager.h"
USING_NS_CC;

bool LayerMainContentNew::init()
{
	if (!Layer::init())
		return false;
    //UserDefault *def = UserDefault::getInstance();
    //SceneManager::getSingleton().currLang= def->getIntegerForKey("lang",2);
	Node* rootNode = CSLoader::getInstance()->createNode("LayerMainChonGame.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
	auto pnlBg = (Layout*)rootNode->getChildByName("pnlBg");
    this->pnlBG = pnlBg;
	pvMain = (PageView*)pnlBg->getChildByName("pnlGame");
	btnArrow1 = (Button*)pnlBg->getChildByName("btnArrowLeft");
	btnArrow2 = (Button*)pnlBg->getChildByName("btnArrowRight");
	if (btnArrow1)
	{
		//btnArrow1->setVisible(false);
		btnArrow1->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonArrow1, this));
	}

	if (btnArrow2)
	{
		//btnArrow2->setVisible(false);
		btnArrow2->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonArrow1, this));
	}



//<<<<<<< HEAD
//    this->getDiscount(0);
//    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::getDiscount), 60);
    
    this->pnlTX = (Layout*)pnlBg->getChildByName("pnlTX");
    this->pnlPK =(Layout*)pnlBg->getChildByName("pnlPK");
    this->pnlCT =(Layout*)pnlBg->getChildByName("pnlCT");

    this->txtHuTX = (Text*)this->pnlTX->getChildByName("txtHuTX");
    this->txtHuCT= (Text*)this->pnlCT->getChildByName("txtHuCT");
    this->txtHuPK= (Text*)this->pnlPK->getChildByName("txtHuPK");
    this->txtHuVQ= (Text*)pnlBg->getChildByName("txtHuVQ");
    this->txtHuTX->setString("---");
    this->txtHuCT->setString("---");
    this->txtHuPK->setString("---");
    this->txtHuVQ->setString("---");
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::loadHu),this,1, false);

    SceneManager::getSingleton().setIsChuyenTab(false);
    SceneManager::getSingleton().setCurrRoomType(1);
    
    auto imgHieuUng = dynamic_cast<Sprite*>(pnlBg->getChildByName("imgHieuUng"));
    imgHieuUng->setVisible(false);
    if (imgHieuUng){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 2; i++) {
            str = StringUtils::format("anims/moc3-head-jackpot-animation-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 363, 140));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        Animate* animN = Animate::create(animation);
        imgHieuUng->runAction(RepeatForever::create(animN));
    }
    
    auto imgHieuUng2 = dynamic_cast<Sprite*>(pnlBg->getChildByName("imgHieuUng2"));
    imgHieuUng2->setVisible(false);

    if (imgHieuUng2){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 2; i++) {
            str = StringUtils::format("anims/bxh-body-anim-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 366, 658));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
        Animate* animN = Animate::create(animation);
        imgHieuUng2->runAction(RepeatForever::create(animN));
    }
    this->btnNapTien = dynamic_cast<Button*>(pnlBg->getChildByName("btnNapTien"));
    if (this->btnNapTien){
        this->btnNapTien->addClickEventListener(CC_CALLBACK_1(LayerMainContentNew::onBtnNapTien, this));
        this->btnNapTien->setVisible(false);
        this->btnNapTien->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.2),ScaleTo::create(0.8, 1), NULL)));
    }
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::showNapTien),this,1, false);
    
    this->fakeHuTX = SceneManager::getSingleton().huTaiXiu;
    this->fakePoker = SceneManager::getSingleton().huPoker;
    
    this->hieuUngHuThuong();
    this->showNapTien(0);
    this->reloadLang();
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::onScheduleThongBao),this,1, false);


    return true;
}
void LayerMainContentNew::onScheduleThongBao(float dt)
{
    auto lstmini = mUtils::splitString(dataManager.getAppConfig().gamemini, '_');
    for (int i = 0; i < lstmini.size(); i++){
        if (strcmp(lstmini[i].c_str(), "128") == 0){
            this->pnlTX->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "130") == 0){
            this->pnlPK->setVisible(false);
        }
        if (strcmp(lstmini[i].c_str(), "129") == 0){
            this->pnlCT->setVisible(false);
        }
    }
    //sprCoin->stopAllActions();
    //sprCoin->runAction(RepeatForever::create(this->animN));
    
}
void LayerMainContentNew::tangHuTX(float dt){
//    static int count = 0;
//    static int count2 = 0;
//    static int step = 37;
//    this->fakeHuTX+=step;
//    count+=37;
//    if (count>32443){
//        count2+=34;
//        step= 0;
//        if (count2>6244){
//            step = 37;
//            count2 = 0 ;
//            count = 0;
//
//        }
//
//    }
    this->txtHuTX->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huTaiXiu));

//    if (this->fakeHuTX > this->huTaiXiu){
//        this->fakeHuTX = 623471;
//        this->txtHuTX->setString(mUtils::convertMoneyEx(this->fakeHuTX));
//    }
//    SceneManager::getSingleton().huTaiXiu = this->fakeHuTX;
}
void LayerMainContentNew::tangHuPK(float dt){
//    static int count = 0;
//    static int count2 = 0;
//    static int step = 37;
//    this->fakePoker+=step;
//    count+=37;
//    if (count>12443){
//        count2+=34;
//        step= 0;
//        if (count2>6244){
//            step = 37;
//            count2 = 0 ;
//            count = 0;
//
//        }
//
//    }
    this->txtHuPK->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huPoker));
//    if (this->fakePoker > this->huPoker){
//        this->fakePoker = 629812;
//        this->txtHuPK->setString(mUtils::convertMoneyEx(this->fakePoker));
//    }
//     = this->fakePoker;
}
void LayerMainContentNew::tangHuCT(float dt){
    this->txtHuCT->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huCaoThap));
}

void LayerMainContentNew::hieuUngHuThuong(){
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::tangHuTX),this,1, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::tangHuPK),this,1, false);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerMainContentNew::tangHuCT),this,1, false);

}
void LayerMainContentNew::showNapTien(float dt){
    if (!SceneManager::getSingleton().isLogin || dataManager.getAppConfig().isHideBilling)
        return;
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    if (myself->GetVariable("am") == NULL) return;
    auto amf = myself->GetVariable("am")->GetDoubleValue();
    if (amf){
        int am = *amf;
        if (am <=0){
            this->btnNapTien->setVisible(true);
        }else{
            this->btnNapTien->setVisible(false);
        }
    }
}
void LayerMainContentNew::onBtnNapTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (dataManager.getAppConfig().isHideBilling){
        popup->onBtnNapHao(NULL);
    }
    _currScene->addChild(popup,2000,9036);
}
void LayerMainContentNew::reloadLang(){
    this->createListData();
    //this->createScrollView();
    this->createPageView();
    auto lblTX = static_cast<Text*> (this->pnlTX->getChildByName("Text_1"));
    lblTX->setString(dataManager.GetSysString(857));
    auto lblCT = static_cast<Text*> (this->pnlCT->getChildByName("Text_1_0_0"));
    lblCT->setString(dataManager.GetSysString(858));
    auto lblPK = static_cast<Text*> (this->pnlPK->getChildByName("Text_1_0_0_0"));
    lblPK->setString(dataManager.GetSysString(859));
    this->btnNapTien->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png");
}
void LayerMainContentNew::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerMainContentNew::onTouchBegan, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    
    //GameServer::getSingleton().addListeners(this);
}
bool LayerMainContentNew::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
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
void LayerMainContentNew::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
   // GameServer::getSingleton().removeListeners(this);
}
void LayerMainContentNew::loadHu(float dt){
    //return;
    //128:654936:0;132:377839:0;1301:2565031:0;1302:475380:0;1303:1070291:0
//    auto lst =  mUtils::splitString(dataManager.getAppConfig().huthuong,';');
//
//    for (int i = 0; i <lst.size();i++){
//        if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 128){
//            this->huTaiXiu = atoi(mUtils::splitString(lst[i],':')[1].c_str());
//
//        }
////        if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 132){
////            this->txtHuCT->setString(mUtils::convertMoneyEx(atof(mUtils::splitString(lst[i],':')[1].c_str())));
////
////        }
//        if (atoi(mUtils::splitString(lst[i],':')[0].c_str()) == 1303){
//            this->huPoker = atoi(mUtils::splitString(lst[i],':')[1].c_str());
//
//        }
//    }
    
}
void LayerMainContentNew::getDiscount(float dt){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_DISCOUNT_INF_REQ, params));
	GameServer::getSingleton().Send(request);
}
void LayerMainContentNew::createListData()
{

	//string _listRoom = "100_true;113_true;111_true;112_true;102_true;109_true;116_false;103_false;114_false;115_false;117_false";
    this->lstIcons.clear();
    string _listRoom = "112_true;139_true;128_true;140_true;250_true;137_true;114_true;138_dis;;100_dis";//dataManager.getAppConfig().list_game;
	// 	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	// 		_listRoom = "112_true;113_true;111_true;102_true;100_true;116_true;114_true;117_true;109_true;106_true;103_true;115_false";
	vector<string> lstconfig = mUtils::splitString(_listRoom, ';');
	for (int i = 0; i < lstconfig.size(); i++)
	{
		vector<string> configs = mUtils::splitString(lstconfig.at(i), '_');
		if (configs.size() > 1)
		{
			string _gameid = configs.at(0);
			string _str1 = configs.at(1);
			int _id = atoi(_gameid.c_str());
			bool _state = 0 == _str1.compare("true") ? true : false;
			string _imgDis = StringUtils::format("gameicons/%s%s_dis.png",SceneManager::getSingleton().getFolderLang().c_str(),_gameid.c_str());
            string _imgNormal = StringUtils::format("gameicons/%s%s.png",SceneManager::getSingleton().getFolderLang().c_str(),_gameid.c_str());
			this->lstIcons.push_back(iconStruct(_id, _state, _imgNormal, _imgDis));
		}
	}
}

void LayerMainContentNew::onButtonGame(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED) {
		Button* _btn = (Button*)pSender;
		if (_btn != NULL) {
			int tag = _btn->getTag();
			UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
			UserDefault::getInstance()->flush();
			SceneManager::getSingleton().setGameID(tag);
			if (tag == kGameTaiXiu || tag == kGameQuayThuong || tag == kGameXocDia || tag == kGameBauTom) {
				/*SceneManager::getSingleton().showLoading();
				SceneManager::getSingleton().gotoPickGame(tag);*/
				LayerPopupChonDongTien *_layerPopupChonDongTien = LayerPopupChonDongTien::create(tag);
				this->getParent()->addChild(_layerPopupChonDongTien, 10000);
			}
            else if (tag == kGameSlot){
                SceneManager::getSingleton().setCurrRoomType(0);
                SceneManager::getSingleton().showLoading();
                SceneManager::getSingleton().gotoPickGame(tag);
            }
            else if (tag == kGameVQMM || tag == kGameMNPK || tag== kGameXDMN|| tag == kGameCT|| tag == kGameTX){
                this->gotoGameMini(tag);
            }
			else
			{
				SceneManager::getSingleton().gotoPickGame(tag);
			}
		}
	}
}

void LayerMainContentNew::gotoGameMini(int tag){
    auto _currScene = this->getParent();
    if (!_currScene) return;

    if (tag == kGameMNPK){
        if (SceneManager::getSingleton().getIsShowPK())
            return;
        LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
        _currScene->addChild(layerPlayerGame,1000,9037);
        SceneManager::getSingleton().setIsShowPK(true);
    }
    else if(tag == kGameXDMN){
        if (SceneManager::getSingleton().getIsShowXD())
            return;
        LayerPlayGameDoDenMini* layerPlayerGame = LayerPlayGameDoDenMini::create();
        _currScene->addChild(layerPlayerGame,1000,9038);
        SceneManager::getSingleton().setIsShowXD(true);
    }
    else if(tag == kGameCT){
        if (SceneManager::getSingleton().getIsShowCT())
            return;
        LayerCaoThapPopupGame* layerPlayerGame = LayerCaoThapPopupGame::create();
        _currScene->addChild(layerPlayerGame,1000,9039);
        SceneManager::getSingleton().setIsShowCT(true);
    }
    else if(tag == kGameVQMM){
        if (SceneManager::getSingleton().getIsShowVQ())
            return;
        LayerPlayVQMM* layerPlayerGame = LayerPlayVQMM::create();
        _currScene->addChild(layerPlayerGame,1000,9152);
        SceneManager::getSingleton().setIsShowVQ(true);
    }
    
    else if(tag == kGameTX){
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        this->addChild(layerPlayerGame, 1000,9040);
        layerPlayerGame->shouldShowHuongDan();

        SceneManager::getSingleton().setIsShowTX(true);
    }
}

LayerMainContentNew::LayerMainContentNew()
{
	GameServer::getSingleton().addListeners(this);

}

LayerMainContentNew::~LayerMainContentNew()
{
    GameServer::getSingleton().removeListeners(this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMainContentNew::loadHu),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerMainContentNew::onScheduleThongBao),this);


}
void LayerMainContentNew::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	//    if (strcmp(EXT_EVENT_GET_DISCOUNT_INF_RESP, cmd->c_str()) == 0){
	//        auto strdiscount = param->GetUtfString(DISCOUNT_INFO_RESULT);
	//        if (strdiscount != NULL)
	//        {
	//            string strdis = *strdiscount;
	//           // log("discount : %s",strdis.c_str());
	//            //109:3;112:4.5
	//            for (int i = 0 ; i < lstIcons.size();i++){
	//                scrollView->getChildByTag(lstIcons.at(i).gameID)->removeChildByTag(10);
	//            }
	//            auto lstdis = mUtils::splitString(strdis, ';');
	//            for (int i = 0;i < lstdis.size() ; i++){
	//                auto it = mUtils::splitString(lstdis[i], ':');
	//                int gameID = atoi(it[0].c_str());
	//                auto bgPhe = ImageView::create("giam_phe.png");
	//                auto labelDis = Label::createWithTTF("1", "UTM_HELVEBOLD_0.TTF", 30);
	//                labelDis->setAnchorPoint(Vec2(0.5,0.5));
	//                labelDis->setString(it[1] + " %");
	//                labelDis->setPosition(Vec2(bgPhe->getContentSize().width/2,bgPhe->getContentSize().height/2+5));
	//                labelDis->setColor(Color3B(184,16,0));
	//                bgPhe->addChild(labelDis);
	//                bgPhe->setTag(10);
	//                bgPhe->setPosition(Size(200,200));
	//                scrollView->getChildByTag(gameID)->addChild(bgPhe);
	//            }
	//        }
	//    }
}
void LayerMainContentNew::createPageView()
{
	if (!pvMain)
		return;
	pvMain->setClippingEnabled(false);
	pvMain->setUsingCustomScrollThreshold(true);
	pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
	pvMain->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_2(LayerMainContentNew::pageViewEvent, this));
	pvMain->setClippingEnabled(true);
    pvMain->removeAllPages();
	int jumpunit = 4;
    
	int pageCount = 2;
    
    if (lstIcons.size() <= 8){
        pageCount = 1;
        btnArrow1->setVisible(false);
        btnArrow2->setVisible(false);
    }
    if (lstIcons.size() > 8)
        pageCount = 2;
	int distanceX = 85;
	int distanceY = 5;
	Size sizeIcon = Size(333, 314);
	try{

		for (int i = 0; i < pageCount; i++)
		{
			Layout* layout = Layout::create();
			layout->setContentSize(pvMain->getContentSize());
			if (lstIcons.size() >= 8)
			{
				if (i == 0)
				{
					for (int j = 0; j < jumpunit * 2; j++)
					{
						float posX;
						float posY;
						if (j < jumpunit)
						{
							posX = j * sizeIcon.width + distanceX;
							posY = sizeIcon.height + distanceY * 4;
						}
						else{
							posX = (j - jumpunit) * sizeIcon.width + distanceX;
							posY = distanceY * 3;
						}
						Button* buttonGame = Button::create();
						buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
						buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
						buttonGame->setAnchorPoint(Vec2::ZERO);
						buttonGame->setPosition(Vec2(posX, posY));
						buttonGame->setContentSize(sizeIcon);
						if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
							buttonGame->setEnabled(false);
							buttonGame->setBright(false);
						}
						else{
							buttonGame->setEnabled(true);
							buttonGame->setBright(true);
						}
						buttonGame->setTag(lstIcons.at(j).gameID);
						buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
						layout->addChild(buttonGame);
					}
				}
				else{
					for (int j = jumpunit * 2; j < lstIcons.size(); j++)
					{
						float posX;
						float posY;
						if (j == jumpunit + 8)
						{
							posX = (j - (jumpunit * 2 + 4)) * sizeIcon.width + distanceX;
							posY = distanceY * 3;
						}
						else if (j > jumpunit + 8)
						{
							posX = (j - (jumpunit * 2 + 4)) * sizeIcon.width + distanceX;
							posY = distanceY * 3;
						}
						else {
							posX = (j - jumpunit * 2) * sizeIcon.width + distanceX;
							posY = sizeIcon.height + distanceY * 4;
						}
						Button* buttonGame = Button::create();
						buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
						buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
						buttonGame->setAnchorPoint(Vec2::ZERO);
						buttonGame->setPosition(Vec2(posX, posY));
						buttonGame->setContentSize(sizeIcon);
						if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
							buttonGame->setEnabled(false);
							buttonGame->setBright(false);
						}
						else{
							buttonGame->setEnabled(true);
							buttonGame->setBright(true);
						}
						// 						buttonGame->setEnabled(lstIcons.at(j).state);
						// 						buttonGame->setBright(lstIcons.at(j).state);
						buttonGame->setTag(lstIcons.at(j).gameID);
						buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
						layout->addChild(buttonGame);
					}
				}
            }else{
                if (i == 0)
                {
                    for (int j = 0; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX;
                            posY = sizeIcon.height + distanceY * 4;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = distanceY * 3;
                        }
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
                        buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
                            buttonGame->setEnabled(false);
                            buttonGame->setBright(false);
                        }
                        else{
                            buttonGame->setEnabled(true);
                            buttonGame->setBright(true);
                        }
                        buttonGame->setTag(lstIcons.at(j).gameID);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
                        layout->addChild(buttonGame);
                    }
                }
                else{
                    for (int j = jumpunit * 2; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit * 2 + 4)) * sizeIcon.width + distanceX;
                            posY = distanceY * 3;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit * 2 + 4)) * sizeIcon.width + distanceX;
                            posY = distanceY * 3;
                        }
                        else {
                            posX = (j - jumpunit * 2) * sizeIcon.width + distanceX;
                            posY = sizeIcon.height + distanceY * 4;
                        }
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
                        buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
                            buttonGame->setEnabled(false);
                            buttonGame->setBright(false);
                        }
                        else{
                            buttonGame->setEnabled(true);
                            buttonGame->setBright(true);
                        }
                        //                         buttonGame->setEnabled(lstIcons.at(j).state);
                        //                         buttonGame->setBright(lstIcons.at(j).state);
                        buttonGame->setTag(lstIcons.at(j).gameID);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
                        layout->addChild(buttonGame);
                    }
                }
            }
			pvMain->insertPage(layout, i);
		}
		pvMain->scrollToPage(0);
	}
	catch (...)
	{
		log("Unknow exceptions");
	}
}

void LayerMainContentNew::pageViewEvent(Ref *pSender, ui::PageView::EventType type)
{
	if (type == ui::PageView::EventType::TURNING)
	{
		// 		PageView* pv = (PageView*)pSender;
		// 		int currIdx = pv->getCurPageIndex();
		// 		if (currIdx == 0)
		// 		{
		// 			btnArrow1->setVisible(false);
		// 			btnArrow2->setVisible(true);
		// 		}
		// 		else{
		// 			btnArrow1->setVisible(true);
		// 			btnArrow2->setVisible(false);
		// 		}
	}
}

void LayerMainContentNew::onButtonArrow1(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED)
	{
		int currIdx = pvMain->getCurPageIndex();
		if (currIdx != 0)
			pvMain->scrollToPage(currIdx - 1);
        int maxIdx = pvMain->getPages().size();
        if (currIdx == 0 && currIdx < maxIdx)
            pvMain->scrollToPage(currIdx + 1);
	}
}

void LayerMainContentNew::onButtonArrow2(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED)
	{
		int currIdx = pvMain->getCurPageIndex();

	}
}

void LayerMainContentNew::createScrollView()
{
	if (!pvMain)
		return;
	int jumpunit = 5;
	int pageCount = 2;
	int distanceX = 30;
	int distanceY = 30;
	Size sizeIcon = Size(300, 300);

	pvMain->setVisible(false);
	scrollView = ui::ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	scrollView->setClippingEnabled(true);
	scrollView->setContentSize(Size(WIDTH_DESIGN, pvMain->getContentSize().height));
	scrollView->setAnchorPoint(pvMain->getAnchorPoint());
	scrollView->setPosition(pvMain->getPosition());
	scrollView->setScrollBarEnabled(false);
	scrollView->setBounceEnabled(true);

	float delta = float(lstIcons.size()) / 2;
	float innerWidth = (sizeIcon.width + distanceX)* delta;
	float innerHeight = pvMain->getContentSize().height;
	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
	this->addChild(scrollView);
	try{

		for (int i = 0; i < pageCount; i++)
		{
			Layout* layout = Layout::create();
			layout->setContentSize(pvMain->getContentSize());
			if (lstIcons.size() >= 10)
			{
				if (i == 0)
				{
					for (int j = 0; j < jumpunit * 2; j++)
					{
						float posX;
						float posY;
						if (j < jumpunit)
						{
							posX = j * sizeIcon.width + distanceX;
							posY = sizeIcon.height + distanceY * 4;
						}
						else{
							posX = (j - jumpunit) * sizeIcon.width + distanceX;
							posY = distanceY * 3;
						}
						Button* buttonGame = Button::create();
						buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
						buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
						buttonGame->setAnchorPoint(Vec2::ZERO);
						buttonGame->setPosition(Vec2(posX, posY));
						buttonGame->setContentSize(sizeIcon);
						if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
							buttonGame->setEnabled(false);
							buttonGame->setBright(false);
						}
						else{
							buttonGame->setEnabled(true);
							buttonGame->setBright(true);
						}
						buttonGame->setTag(lstIcons.at(j).gameID);
						buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
						scrollView->addChild(buttonGame);
					}
				}
				else{
					for (int j = jumpunit * 2; j < lstIcons.size(); j++)
					{
						float posX;
						float posY;
						float delta = j / 2 - jumpunit;
						if (j % 2 == 0)
						{
							posX = (j - jumpunit - delta) * sizeIcon.width + distanceX;
							posY = sizeIcon.height + distanceY * 4;
						}
						else {
							posX = (j - jumpunit - 1) * sizeIcon.width + distanceX;
							posY = distanceY * 3;
						}

						Button* buttonGame = Button::create();
						buttonGame->loadTextureNormal(lstIcons.at(j).imgNormalPath);
						buttonGame->loadTextureDisabled(lstIcons.at(j).imgDisablePath);
						buttonGame->setAnchorPoint(Vec2::ZERO);
						buttonGame->setPosition(Vec2(posX, posY));
						buttonGame->setContentSize(sizeIcon);
						if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
							buttonGame->setEnabled(false);
							buttonGame->setBright(false);
						}
						else{
							buttonGame->setEnabled(true);
							buttonGame->setBright(true);
						}
						// 						buttonGame->setEnabled(lstIcons.at(j).state);
						// 						buttonGame->setBright(lstIcons.at(j).state);
						buttonGame->setTag(lstIcons.at(j).gameID);
						buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew::onButtonGame, this));
						scrollView->addChild(buttonGame);
					}
				}
			}
			//scrollView->addChild(layout);
			//pvMain->insertPage(layout, i);
		}
		//pvMain->scrollToPage(0);
	}
	catch (...)
	{
		log("Unknow exceptions");
	}
}

