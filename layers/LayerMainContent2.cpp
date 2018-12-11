#include "LayerMainContent2.h"
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
#include "ItemMenuGame.h"
USING_NS_CC;

bool LayerMainContentNew2::init()
{
	if (!Layer::init())
		return false;
//    UserDefault *def = UserDefault::getInstance();
//    SceneManager::getSingleton().currLang= def->getIntegerForKey("lang",2);
	Node* rootNode = CSLoader::getInstance()->createNode("LayerMainChonGame2.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());
	auto pnlBg = (Layout*)rootNode->getChildByName("pnlBg");
    this->pnlBG = pnlBg;
    this->pnlBG->setSwallowTouches(false);
	pvMain = (PageView*)pnlBg->getChildByName("pnlGame");
    pvMain->setSwallowTouches(false);
	btnArrow1 = (Button*)pnlBg->getChildByName("btnArrowLeft");
	btnArrow2 = (Button*)pnlBg->getChildByName("btnArrowRight");

	if (btnArrow1)
	{
		//btnArrow1->setVisible(false);
		btnArrow1->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonArrow1, this));
	}

	if (btnArrow2)
	{
		//btnArrow2->setVisible(false);
		btnArrow2->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonArrow1, this));
	}

   // dataManager.getAppConfig().list_game = "112_true;139_true;128_true;140_true;250_true;138_dis;137_dis;100_dis";//

//<<<<<<< HEAD
//    this->getDiscount(0);
//    this->schedule(schedule_selector(LayerMainContentNew2::getDiscount), 60);
    
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
    //this->schedule(schedule_selector(LayerMainContentNew2::loadHu),1);

    SceneManager::getSingleton().setIsChuyenTab(false);
    SceneManager::getSingleton().setCurrRoomType(1);
    
//    auto imgHieuUng = dynamic_cast<Sprite*>(pnlBg->getChildByName("imgHieuUng"));
//    imgHieuUng->setVisible(false);
//    if (imgHieuUng){
//        Vector<SpriteFrame*> animFrames;
//
//        string str = "";
//        for (int i = 1; i <= 2; i++) {
//            str = StringUtils::format("anims/moc3-head-jackpot-animation-%d.png",i);
//            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 363, 140));
//            animFrames.pushBack(spriteFrame);
//        }
//
//        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
//        Animate* animN = Animate::create(animation);
//        imgHieuUng->runAction(RepeatForever::create(animN));
//    }
//
//    auto imgHieuUng2 = dynamic_cast<Sprite*>(pnlBg->getChildByName("imgHieuUng2"));
//    imgHieuUng2->setVisible(false);
//
//    if (imgHieuUng2){
//        Vector<SpriteFrame*> animFrames;
//
//        string str = "";
//        for (int i = 1; i <= 2; i++) {
//            str = StringUtils::format("anims/bxh-body-anim-%d.png",i);
//            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 366, 658));
//            animFrames.pushBack(spriteFrame);
//        }
//
//        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
//        Animate* animN = Animate::create(animation);
//        imgHieuUng2->runAction(RepeatForever::create(animN));
//    }
    
    
    auto sprKhung = (Sprite*)(pnlBg->getChildByName("sprKhung"));
    
    if (sprKhung){
        Vector<SpriteFrame*> animFrames;
        
        string str = "";
        for (int i = 1; i <= 24; i++) {
            str = StringUtils::format("khung-%d.png",i);
            auto spriteFrame = SpriteFrame::create(str, Rect(0, 0, 1389, 766));
            animFrames.pushBack(spriteFrame);
        }
        
        Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        auto animN = Animate::create(animation);
        sprKhung->runAction(RepeatForever::create(Sequence::create(animN,DelayTime::create(2.5),NULL)));
    }
    
    
    //this->schedule(schedule_selector(LayerMainContentNew2::showNapTien),1);
    
    this->fakeHuTX = SceneManager::getSingleton().huTaiXiu;
    this->fakePoker = SceneManager::getSingleton().huPoker;
    
    this->hieuUngHuThuong();
    this->showNapTien(0);
    this->createListData();
    //this->createScrollView();
    if (SceneManager::getSingleton().isNagaWin){
        //this->createPageViewNew();
        this->createPageViewNew();

    }else if (SceneManager::getSingleton().isNagaNew){
        this->createPageView2();

    }else{
        this->createPageView();

    }
    this->setStickGame();

    auto lblTX = static_cast<Text*> (this->pnlTX->getChildByName("Text_1"));
    lblTX->setString(dataManager.GetSysString(857));
    auto lblCT = static_cast<Text*> (this->pnlCT->getChildByName("Text_1_0_0"));
    lblCT->setString(dataManager.GetSysString(858));
    auto lblPK = static_cast<Text*> (this->pnlPK->getChildByName("Text_1_0_0_0"));
    lblPK->setString(dataManager.GetSysString(859));
    //this->btnNapTien->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png");
    //this->reloadLang();
    //this->schedule(schedule_selector(LayerMainContentNew2::onScheduleThongBao),1);
    
    auto pnlTCCMenu = dynamic_cast<Layout*>(pnlBg->getChildByName("pnlTCCMenu"));
    pnlTCCMenu->setVisible(false);
    
    this->tcc = ItemMenuGame::create();
    tcc->playAnimationTCC();
    pnlBg->addChild(tcc);
    tcc->setPosition(pnlTCCMenu->getPosition());


    return true;
}
void LayerMainContentNew2::reloadListGame(){
    this->createListData();
    //this->createScrollView();
    if (SceneManager::getSingleton().isNagaWin){
        //this->createPageViewNew();
        this->createPageViewNew();
        this->setStickGame();
        
    }else if (SceneManager::getSingleton().isNagaNew){
        this->createPageView2();
        
    }else{
        this->createPageView();
        
    }
}
void LayerMainContentNew2::isLogon(){
    for (int i = 0; i < this->lstItemGame.size();i++){
        this->lstItemGame[i]->isLogon();
    }
}
void LayerMainContentNew2::isLogout(){
    for (int i = 0; i < this->lstItemGame.size();i++){
        this->lstItemGame[i]->isLogout();
    }
}
void LayerMainContentNew2::onScheduleThongBao(float dt)
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
void LayerMainContentNew2::tangHuTX(float dt){
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
void LayerMainContentNew2::tangHuPK(float dt){
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
void LayerMainContentNew2::tangHuCT(float dt){
    this->txtHuCT->setString(mUtils::convertMoneyEx(SceneManager::getSingleton().huCaoThap));
}

void LayerMainContentNew2::hieuUngHuThuong(){
//    this->schedule(schedule_selector(LayerMainContentNew2::tangHuTX));
//    this->schedule(schedule_selector(LayerMainContentNew2::tangHuPK));
//    this->schedule(schedule_selector(LayerMainContentNew2::tangHuCT));

}
void LayerMainContentNew2::showNapTien(float dt){
    return;
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
void LayerMainContentNew2::onBtnNapTien(Ref* pSender){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    LayerPopupCuaHang* popup = LayerPopupCuaHang::create();
    popup->onBtnNapThe(NULL);
    if (dataManager.getAppConfig().isHideBilling){
        popup->onBtnNapHao(NULL);
    }
    _currScene->addChild(popup,2000,9108);
}
void LayerMainContentNew2::reloadLang(){
    //this->createListData();
    //this->createScrollView();
    //this->createPageView();
    this->tcc->reloadLang();
    for (int i = 0; i < this->lstItemGame.size();i++){
        this->lstItemGame[i]->reloadLang();
    }
    auto lblTX = static_cast<Text*> (this->pnlTX->getChildByName("Text_1"));
    lblTX->setString(dataManager.GetSysString(857));
    auto lblCT = static_cast<Text*> (this->pnlCT->getChildByName("Text_1_0_0"));
    lblCT->setString(dataManager.GetSysString(858));
    auto lblPK = static_cast<Text*> (this->pnlPK->getChildByName("Text_1_0_0_0"));
    lblPK->setString(dataManager.GetSysString(859));
    //this->btnNapTien->loadTextures(IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png", IMAGE_PATH+SceneManager::getSingleton().getFolderLang()+"naptien.png");
}
void LayerMainContentNew2::onTouchEnded(Touch *touch, Event *unused_event){
    
    if (this->currPage != this->pvMain->getCurPageIndex()){
    for (int i = 0; i <  this->lstItemGame.size();i++){
        this->lstItemGame[i]->setShouldTouch(false);
    }
    }
}
void LayerMainContentNew2::onEnter()
{
    Layer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    listener->onTouchBegan = CC_CALLBACK_2(LayerMainContentNew2::onTouchBegan, this);
//    listener->onTouchEnded = CC_CALLBACK_2(LayerMainContentNew2::onTouchEnded, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//    _touchListener = listener;
    
}
bool LayerMainContentNew2::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    for (int i = 0; i <  this->lstItemGame.size();i++){
        this->lstItemGame[i]->setShouldTouch(false);
    }
    return true;
}
void LayerMainContentNew2::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
   // GameServer::getSingleton().removeListeners(this);
}
void LayerMainContentNew2::loadHu(float dt){
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
void LayerMainContentNew2::getDiscount(float dt){
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GET_DISCOUNT_INF_REQ, params));
	GameServer::getSingleton().Send(request);
}
void LayerMainContentNew2::createListData()
{

	//string _listRoom = "100_true;113_true;111_true;112_true;102_true;109_true;116_false;103_false;114_false;115_false;117_false";
    this->lstIcons.clear();
    //anhnt only for test
    string _listRoom = dataManager.getAppConfig().list_game;
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
            menuStruct it;
			it.gameID = atoi(_gameid.c_str());
			it.state = 0 == _str1.compare("true") ? true : false;
			this->lstIcons.push_back(it);
		}
	}
    
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
            this->lstIconsNew.push_back(iconStruct(_id, _state, _imgNormal, _imgDis));
        }
    }
}

void LayerMainContentNew2::onButtonGame(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED) {
		Button* _btn = (Button*)pSender;
		if (_btn != NULL) {
            int tag = _btn->getTag();
            //anhnt onlye for test
            //tag = kGameCoTuong;
            auto _currScene = Director::getInstance()->getRunningScene();
            if (!_currScene) return;
            UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
            UserDefault::getInstance()->flush();
            SceneManager::getSingleton().setGameID(tag);
            if (tag == kGameTaiXiu || tag == kGameQuayThuong) {
                /*SceneManager::getSingleton().showLoading();
                 SceneManager::getSingleton().gotoPickGame(tag);*/
                LayerPopupChonDongTien *_layerPopupChonDongTien = LayerPopupChonDongTien::create(tag);
                _currScene->addChild(_layerPopupChonDongTien, 10000,9109);
            }
            else if (tag == kGameSlot || tag == kGameXocDia || tag == kGameBauTom){
                SceneManager::getSingleton().setCurrRoomType(0);
                //SceneManager::getSingleton().showLoading();
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

void LayerMainContentNew2::gotoGameMini(int tag){
    auto _currScene = Director::getInstance()->getRunningScene();
    if (!_currScene) return;
    
    if (tag == kGameMNPK){
        if (SceneManager::getSingleton().getIsShowPK())
            return;
        LayerPlayGameMiniPoker* layerPlayerGame = LayerPlayGameMiniPoker::create();
        _currScene->addChild(layerPlayerGame,1000,9110);
        SceneManager::getSingleton().setIsShowPK(true);
    }
    else if(tag == kGameXDMN){
        if (SceneManager::getSingleton().getIsShowXD())
            return;
        LayerPlayGameDoDenMini* layerPlayerGame = LayerPlayGameDoDenMini::create();
        _currScene->addChild(layerPlayerGame,1000,9111);
        SceneManager::getSingleton().setIsShowXD(true);
    }
    else if(tag == kGameCT){
        if (SceneManager::getSingleton().getIsShowCT())
            return;
        LayerCaoThapPopupGame* layerPlayerGame = LayerCaoThapPopupGame::create();
        _currScene->addChild(layerPlayerGame,1000,9112);
        SceneManager::getSingleton().setIsShowCT(true);
    }
    else if(tag == kGameVQMM){
        if (SceneManager::getSingleton().getIsShowVQ())
            return;
        LayerPlayVQMM* layerPlayerGame = LayerPlayVQMM::create();
        _currScene->addChild(layerPlayerGame,1000,9113);
        SceneManager::getSingleton().setIsShowVQ(true);
    }
    else if(tag == kGameTX){
        if (SceneManager::getSingleton().getIsShowTX())
            return;
        LayerPlayGameTXMini* layerPlayerGame = LayerPlayGameTXMini::create();
        _currScene->addChild(layerPlayerGame,1000,9114);
        layerPlayerGame->shouldShowHuongDan();
        
        SceneManager::getSingleton().setIsShowTX(true);
        
    }
}

LayerMainContentNew2::LayerMainContentNew2()
{
	GameServer::getSingleton().addListeners(this);

}

LayerMainContentNew2::~LayerMainContentNew2()
{
    GameServer::getSingleton().removeListeners(this);
    this->unschedule(schedule_selector(LayerMainContentNew2::loadHu));
    this->unschedule(schedule_selector(LayerMainContentNew2::onScheduleThongBao));


}
void LayerMainContentNew2::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
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
void LayerMainContentNew2::createPageView()
{
    //this->createScrollView(); return;
    lstItemGame.clear();

    if (!pvMain)
        return;
    pvMain->setClippingEnabled(false);
    pvMain->setUsingCustomScrollThreshold(true);
    pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
    pvMain->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_2(LayerMainContentNew2::pageViewEvent, this));
    pvMain->setClippingEnabled(true);
    //pvMain->removeAllItems();
    pvMain->removeAllPages();
    pvMain->setSwallowTouches(false);
    int jumpunit = 4;
//
    int pageCount = 1;
//
    if (lstIcons.size()>4 && lstIcons.size()<=8)
        pageCount = 2;
    if (lstIcons.size()>8 && lstIcons.size()<=12)
        pageCount = 3;
    if (lstIcons.size()>12 && lstIcons.size()<=16)
        pageCount = 4;

    int distanceX = 45;
    int distanceY = 5;
    Size sizeIcon = Size(333, 535);
    Size sizeIcon2 = Size(315, 535);

    try{

        for (int i = 0; i < pageCount; i++)
        {
            Layout* layout = Layout::create();
            layout->setSwallowTouches(false);
            layout->setContentSize(pvMain->getContentSize());
            if (lstIcons.size() >= 4)
            {
                if (i == 0)
                {
                    for (int j = 0; j < jumpunit; j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        item->setTag(this->lstIcons[j].gameID);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        layout->addChild(buttonGame);
                        

                    }
                }
                else{
                    for (int j = jumpunit*i; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit  + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else {
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        
                        //item->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);

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
                            posY = 0;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);

                        layout->addChild(buttonGame);

                    }
                }
                else{
                    for (int j = jumpunit ; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        else {
                            posX = (j - jumpunit ) * sizeIcon.width + distanceX;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);

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
       // log("Unknow exceptions");
    }
}
void LayerMainContentNew2::createPageView2()
{
    //this->createScrollView(); return;
    lstItemGame.clear();
    if (!pvMain)
        return;
    pvMain->setClippingEnabled(false);
    pvMain->setUsingCustomScrollThreshold(true);
    pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
    pvMain->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_2(LayerMainContentNew2::pageViewEvent, this));
    pvMain->setClippingEnabled(true);
    //pvMain->removeAllItems();
    pvMain->removeAllPages();
    pvMain->setSwallowTouches(false);
    int jumpunit = 4;
    int pageCount = 2;
    if (lstIcons.size()<=8)
        pageCount=1;
    int distanceX = 55;
    int distanceY = 20;
    Size sizeIcon = Size(265.00, 315);
    try{
        
        for (int i = 0; i < pageCount; i++)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(pvMain->getContentSize());
            if (lstIcons.size() >= 6)
            {
                if (i == 0)
                {
                    for (int j = 0; j < jumpunit * 2; j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX * (j - 1) + 20;
                            posY = sizeIcon.height + distanceY-20 ;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 20;;
                            posY = -20;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        item->setTag(this->lstIcons[j].gameID);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        layout->addChild(buttonGame);
                        
                        
                    }
                }
                else{
                    for (int j = jumpunit * pageCount; j < this->lstIcons.size(); j++) {
                        int posX;
                        int posY;
                        int j_tg = j - jumpunit * 2;
                        if (j_tg < jumpunit) {
                            posX = j_tg * sizeIcon.width + distanceX * (j_tg - 1) + 20;
                            posY = sizeIcon.height + distanceX -20;
                        } else {
                            posX = (j_tg - jumpunit) * sizeIcon.width + (j_tg - jumpunit - 1) * distanceX + 20;
                            posY = -20;
                        }
                        
                        ItemMenuGame* item = ItemMenuGame::create();
                        
                        //item->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
                        layout->addChild(buttonGame);
                        
                    }
                }
            }else{
                if (i == 0)
                {
                    for (int j = 0; j < this->lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX * (j - 1) + 20;
                            posY = sizeIcon.height + distanceY -20;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 20;;
                            posY = -20;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
                        layout->addChild(buttonGame);
                        
                    }
                }
                else{
                    for (int j = jumpunit ; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+20;
                            posY = -20;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+20;
                            posY = -20;
                        }
                        else {
                            posX = (j - jumpunit ) * sizeIcon.width + distanceX+20;
                            posY = -20;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
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
        // log("Unknow exceptions");
    }
}

void LayerMainContentNew2::pageViewEvent(Ref *pSender, ui::PageView::EventType type)
{
    for (int i = 0; i <  this->lstItemGame.size();i++){
        this->lstItemGame[i]->setShouldTouch(false);
    }
	if (type == ui::PageView::EventType::TURNING)
	{
        for (int i = 0; i <  this->lstItemGame.size();i++){
            this->lstItemGame[i]->setShouldTouch(true);
        }
    }
}

void LayerMainContentNew2::onButtonArrow1(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED)
	{
        int currIdx = pvMain->getCurPageIndex();
        if (currIdx != 0)
            pvMain->scrollToPage(currIdx - 1);
        int maxIdx = pvMain->getPages().size();
        if (currIdx == 0 && currIdx < maxIdx)
            pvMain->scrollToPage(currIdx + 1);
        //if (scrollView->getInnerContainer().)
	}
}

void LayerMainContentNew2::onButtonArrow2(Ref* pSender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED)
	{
		int currIdx = pvMain->getCurPageIndex();

	}
}

void LayerMainContentNew2::createScrollView()
{
    lstItemGame.clear();

    if (!pvMain)
        return;
    int distanceX = 45;
    int distanceY = 5;
    Size sizeIcon = Size(333, 314);

    pvMain->setVisible(false);
    scrollView = ui::ScrollView::create();
    scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    scrollView->setClippingEnabled(true);
    scrollView->setContentSize(Size(pvMain->getContentSize().width, pvMain->getContentSize().height));
    scrollView->setAnchorPoint(pvMain->getAnchorPoint());
    scrollView->setPosition(pvMain->getPosition());
    scrollView->setScrollBarEnabled(false);
    scrollView->setBounceEnabled(true);
    scrollView->addEventListener((ui::ScrollView::ccScrollViewCallback)CC_CALLBACK_2(LayerMainContentNew2::scrollViewEvent, this));
    scrollView->setSwallowTouches(true);
    scrollView->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::touchEvent, this));
    


    float delta = lstIcons.size();
    float innerWidth = (sizeIcon.width + distanceX)* delta;
    float innerHeight = pvMain->getContentSize().height;
    scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
    this->addChild(scrollView);

                    for (int j = 0; j < this->lstIcons.size(); j++)
                    {
                        float posX = j * sizeIcon.width + distanceX;
                        float posY = 0;
                    
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        scrollView->addChild(item);
                        this->lstItemGame.push_back(item);
                    }

               
}
void LayerMainContentNew2::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:{
            for (int i = 0; i <  this->lstItemGame.size();i++){
                this->lstItemGame[i]->setShouldTouch(false);
            }
            break;
        }
            
        case Widget::TouchEventType::MOVED:{
            for (int i = 0; i <  this->lstItemGame.size();i++){
                this->lstItemGame[i]->setShouldTouch(false);
            }
            break;
        }
            
            
//        case Widget::TouchEventType::ENDED:
//        {
//            for (int i = 0; i <  this->lstItemGame.size();i++){
//                this->lstItemGame[i]->setShouldTouch(true);
//            }
//        }
//            break;
//            
        case Widget::TouchEventType::CANCELED:{
            for (int i = 0; i <  this->lstItemGame.size();i++){
                this->lstItemGame[i]->setShouldTouch(true);
            }
            break;
        }
            
            
        default:
            break;
    }
}
void LayerMainContentNew2::scrollViewEvent(Ref *pSender, ui::ScrollView::EventType type){
    if (type == ui::ScrollView::EventType::AUTOSCROLL_ENDED || type == ui::ScrollView::EventType::SCROLL_TO_TOP || type == ui::ScrollView::EventType::SCROLL_TO_LEFT|| type == ui::ScrollView::EventType::SCROLL_TO_BOTTOM|| type == ui::ScrollView::EventType::SCROLL_TO_RIGHT){
        for (int i = 0; i <  this->lstItemGame.size();i++){
            this->lstItemGame[i]->setShouldTouch(true);
        }
    }
}
void LayerMainContentNew2::createPageViewNew()
{
    lstItemGame.clear();

    if (!pvMain)
        return;
    pvMain->setClippingEnabled(false);
    pvMain->setUsingCustomScrollThreshold(true);
    pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
    pvMain->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_2(LayerMainContentNew2::pageViewEvent, this));
    pvMain->setClippingEnabled(true);
    //pvMain->removeAllItems();
    pvMain->removeAllPages();
    pvMain->setSwallowTouches(false);
    int jumpunit = 4;
    int pageCount = 2;
    if (lstIcons.size()<=8)
        pageCount=1;
    int distanceX = 65;
    int distanceY = 43;
    Size sizeIcon = Size(247, 281);
    Size sizeIcon2 = Size(247, 240);

    try{
        
        for (int i = 0; i < pageCount; i++)
        {
            Layout* layout = Layout::create();
            layout->setContentSize(pvMain->getContentSize());
            if (lstIcons.size() >= 6)
            {
                if (i == 0)
                {
                    for (int j = 0; j < jumpunit * 2; j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX * (j - 1) + 100;
                            posY = sizeIcon.height + distanceY ;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 100;;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        item->setTag(this->lstIcons[j].gameID);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon2);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        layout->addChild(buttonGame);
                        
                        
                    }
                }
                else{
                    for (int j = jumpunit * pageCount; j < this->lstIcons.size(); j++) {
                        int posX;
                        int posY;
                        int j_tg = j - jumpunit * 2;
                        if (j_tg < jumpunit) {
                            posX = j_tg * sizeIcon.width + distanceX * (j_tg - 1) + 100;
                            posY = sizeIcon.height + distanceX +0;
                        } else {
                            posX = (j_tg - jumpunit) * sizeIcon.width + (j_tg - jumpunit - 1) * distanceX + 100;
                            posY = 0;
                        }
                        
                        ItemMenuGame* item = ItemMenuGame::create();
                        
                        //item->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon2);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
                        layout->addChild(buttonGame);
                        
                    }
                }
            }else{
                if (i == 0)
                {
                    for (int j = 0; j < this->lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j < jumpunit)
                        {
                            posX = j * sizeIcon.width + distanceX * (j - 1) + 100;
                            posY = sizeIcon.height + distanceY +0;
                        }
                        else{
                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 100;;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon2);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
                        layout->addChild(buttonGame);
                        
                    }
                }
                else{
                    for (int j = jumpunit ; j < lstIcons.size(); j++)
                    {
                        float posX;
                        float posY;
                        if (j == jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+100;
                            posY = 0;
                        }
                        else if (j > jumpunit + 8)
                        {
                            posX = (j - (jumpunit + 4)) * sizeIcon.width + distanceX+100;
                            posY = 0;
                        }
                        else {
                            posX = (j - jumpunit ) * sizeIcon.width + distanceX+100;
                            posY = 0;
                        }
                        ItemMenuGame* item = ItemMenuGame::create();
                        item->setGameID(this->lstIcons[j].gameID,this->lstIcons[j].state);
                        item->setPosition(Vec2(posX,posY));
                        layout->addChild(item);
                        this->lstItemGame.push_back(item);
                        
                        Button* buttonGame = Button::create();
                        buttonGame->loadTextureNormal("anim-bg-menu-game-tran.png");
                        buttonGame->loadTextureDisabled("anim-bg-menu-game-tran.png");
                        buttonGame->setAnchorPoint(Vec2::ZERO);
                        buttonGame->setPosition(Vec2(posX, posY));
                        buttonGame->setContentSize(sizeIcon2);
                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
                        buttonGame->setTag(this->lstIcons[j].gameID);
                        buttonGame->setEnabled(this->lstIcons[j].state);
                        
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
        // log("Unknow exceptions");
    }
    
    
    
    return;
    
    
    //icon tinh
//    if (!pvMain)
//        return;
//    pvMain->setClippingEnabled(true);
//    pvMain->setUsingCustomScrollThreshold(true);
//    pvMain->setCustomScrollThreshold(pvMain->getContentSize().width / 4);
//    pvMain->addEventListenerPageView(this, pagevieweventselector(LayerMainContentNew2::pageViewEvent));
//    int jumpunit = 4;
//    int pageCount = 2;
//    if (lstIcons.size()<=8)
//        pageCount=1;
//    int distanceX = 50;
//    int distanceY = 50;
//    Size sizeIcon = Size(222, 253);
//    try{
//
//        for (int i = 0; i < pageCount; i++)
//        {
//            Layout* layout = Layout::create();
//            layout->setContentSize(pvMain->getContentSize());
//            if (lstIcons.size() >= 6)
//            {
//                if (i == 0)
//                {
//                    for (int j = 0; j < jumpunit * 2; j++)
//                    {
//                        float posX;
//                        float posY;
//                        if (j < jumpunit)
//                        {
//                            posX = j * sizeIcon.width + distanceX * (j - 1) + 70;
//                            posY = sizeIcon.height + distanceY + 20;
//                        }
//                        else{
//                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 70;;
//                            posY = 20;
//                        }
//                        Button* buttonGame = Button::create();
//                        buttonGame->loadTextureNormal(lstIconsNew.at(j).imgNormalPath);
//                        buttonGame->loadTextureDisabled(lstIconsNew.at(j).imgDisablePath);
//                        buttonGame->setAnchorPoint(Vec2::ZERO);
//                        buttonGame->setPosition(Vec2(posX, posY));
//                        buttonGame->setContentSize(sizeIcon);
//                        if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
//                            buttonGame->setEnabled(false);
//                            buttonGame->setBright(false);
//                        }
//                        else{
//                            buttonGame->setEnabled(true);
//                            buttonGame->setBright(true);
//                        }
//                        buttonGame->setTag(lstIcons.at(j).gameID);
//                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
//                        layout->addChild(buttonGame);
//                    }
//                }
//                else{
//                    for (int j = jumpunit * pageCount; j < this->lstIcons.size(); j++) {
//                        int posX;
//                        int posY;
//                        int j_tg = j - jumpunit * 2;
//                        if (j_tg < jumpunit) {
//                            posX = j_tg * sizeIcon.width + distanceX * (j_tg - 1) + 70;
//                            posY = sizeIcon.height + distanceX + 20;
//                        } else {
//                            posX = (j_tg - jumpunit) * sizeIcon.width + (j_tg - jumpunit - 1) * distanceX + 70;
//                            posY = 20;
//                        }
//                        //if (j == jumpunit + 10) {
//                        //    posX = (j - jumpunit * 2) * sizeIcon.width + distance;
//                        //    posY = distance * 3;
//                        //}
//                        //else if (j > jumpunit + 10) {
//                        //    posX = (j - jumpunit * 2) * sizeIcon.width + distance;
//                        //    posY = distance * 3;
//                        //} else {
//                        //    posX = (j - jumpunit * 2) * sizeIcon.width + distance;
//                        //    posY = sizeIcon.height + distance * 4;
//                        //}
//
//
//                        Button* buttonGame = Button::create();
//                        buttonGame->loadTextureNormal(lstIconsNew.at(j).imgNormalPath);
//                        buttonGame->loadTextureDisabled(lstIconsNew.at(j).imgDisablePath);
//                        buttonGame->setAnchorPoint(Vec2::ZERO);
//                        buttonGame->setPosition(Vec2(posX, posY));
//                        buttonGame->setContentSize(sizeIcon);
//                        if (dataManager.getAppConfig().sListHiddenGame.find(boost::to_string(lstIcons.at(j).gameID)) != std::string::npos) {
//                            buttonGame->setEnabled(false);
//                            buttonGame->setBright(false);
//                        }
//                        else{
//                            buttonGame->setEnabled(true);
//                            buttonGame->setBright(true);
//                        }
//                        //                         buttonGame->setEnabled(lstIcons.at(j).state);
//                        //                         buttonGame->setBright(lstIcons.at(j).state);
//                        buttonGame->setTag(lstIcons.at(j).gameID);
//                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
//                        layout->addChild(buttonGame);
//                    }
//                }
//            }
//            else{
//                if (i == 0)
//                {
//                    for (int j = 0; j < this->lstIcons.size(); j++)
//                    {
//                        float posX;
//                        float posY;
//                        if (j < jumpunit)
//                        {
//                            posX = j * sizeIcon.width + distanceX * (j - 1) + 60;
//                            posY = sizeIcon.height + distanceY + 20;
//                        }
//                        else{
//                            posX = (j - jumpunit) * sizeIcon.width + (j - jumpunit - 1) * distanceX + 70;;
//                            posY = 20;
//                        }
//                        Button* buttonGame = Button::create();
//                        buttonGame->loadTextureNormal(lstIconsNew.at(j).imgNormalPath);
//                        buttonGame->loadTextureDisabled(lstIconsNew.at(j).imgDisablePath);
//                        buttonGame->setAnchorPoint(Vec2::ZERO);
//                        buttonGame->setPosition(Vec2(posX, posY));
//                        buttonGame->setContentSize(sizeIcon);
//                        if (dataManager.getAppConfig().sListHiddenGame.find(lstIcons.at(j).gameID) != std::string::npos) {
//                            buttonGame->setEnabled(false);
//                            buttonGame->setBright(false);
//                        }
//                        else{
//                            buttonGame->setEnabled(true);
//                            buttonGame->setBright(true);
//                        }
//                        buttonGame->setTag(lstIcons.at(j).gameID);
//                        buttonGame->addTouchEventListener(CC_CALLBACK_2(LayerMainContentNew2::onButtonGame, this));
//                        layout->addChild(buttonGame);
//                    }
//                }
//            }
//            pvMain->insertPage(layout, i);
//        }
//        pvMain->scrollToPage(0);
//    }
//    catch (...)
//    {
//        log("Unknow exceptions");
//    }
}
void LayerMainContentNew2::setStickGame(){
    //137,EVENT,http://nagaclub.net;140,X2,http://xxx
    
    auto lst = mUtils::splitString(dataManager.getAppConfig().STICKER_GAME_STR,';');
    for (int i = 0;i < lst.size();i++){
        auto item = mUtils::splitString(lst[i],',');
        if (item.size() < 3) continue;
        for (int j = 0 ; j < this->lstItemGame.size();j++){
            if (this->lstItemGame[j]->gameID == atoi(item[0].c_str())){
                this->lstItemGame[j]->pnlEvent->setVisible(true);
                this->lstItemGame[j]->urlSticker = item[2];
                this->lstItemGame[j]->txtEvent->setString(item[1]);
            }
        }
    }

}
