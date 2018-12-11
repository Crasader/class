#include "LayerGameXocDia.h"
#include "AllData.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "ClientMsgDefs.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/SetRoomVariablesRequest.h"
#include "bautom/LayerGameBauTom.h"
#include "Requests/JoinRoomRequest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layers/LayerBangXepHang.h"
#include "../scenes/SceneMain.h"
#define ROBOTO_BOLD_FONT "__Roboto-Bold.ttf"
#define EXT_FIELD_GAME_BOSS_UID  "buid"
LayerGameXocDia::LayerGameXocDia()
{
	status = 0;
	longpresstime = 0.5;
	press = 0;
	BetweentwoPlayTime = 15;
	TimerCountDown = 45;
	TimerCountNextTime = 15;
	PlayTurnTime = 45;
	CurrentTag = -1;
	CountZoder = 1001;
}

LayerGameXocDia::~LayerGameXocDia()
{
   // if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;

	GameServer::getSingleton().removeListeners(this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateTimer),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateEvent),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updatepress),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateToNextTurn),this);

	vector<TaiXiuBet*>::iterator it;
	for (it = this->arrBet.begin(); it != this->arrBet.end();){
		//delete *it;
		it = this->arrBet.erase(it);
	}
	this->arrBet.clear();

	vector<TaiXiuCoin*>::iterator it1;
	for (it1 = this->arrCoin.begin(); it1 != this->arrCoin.end();){
		//delete *it1;
		it1 = this->arrCoin.erase(it1);
	}
	this->arrCoin.clear();

	this->listHistory.clear();
	this->listTopWinner.clear();

	if (this->mQueueMsg != NULL){
		delete this->mQueueMsg;
	}

	if (this->playerInfo != NULL){
		delete this->playerInfo;
	}

	if (this->mQueueTaiXiu != NULL){
		delete this->mQueueTaiXiu;
	}
	this->ArrListValueBet.clear();
	this->ArrListValueBetDefault.clear();
}

bool LayerGameXocDia::init()
{
	if (!Layer::init())
		return false;
	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return false;
	MyAI = mySelf->Name()->c_str();
	this->myName = GameUtils::getMyName();

	this->mQueueTaiXiu = new QueueTaiXiu();
	mQueueMsg = new ThreadQueueMsg();
	mQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameXocDia::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updateEvent),this,1/60, false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	sizeAdd = mUtils::getSizePos();
    
    

    
    

	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		playerInfo = new PlayerTaiXiuInfo();
		double amf = 0;
		boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable(EXT_VAL_AMOUNT_BALANCE);
		if (amfptr != NULL)
		{
			amf = *amfptr->GetDoubleValue();
			playerInfo->initMoney(amf);
		}
	}
	else{
		playerInfo = new PlayerTaiXiuInfo();
		double amf = 0;
		boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable("amfs");
		if (amfptr != NULL)
		{
			amf = *amfptr->GetDoubleValue();
			playerInfo->initMoney(amf);
		}
	}
	ArrListValueBetDefault.push_back(BET_VALUE_1K);
	ArrListValueBetDefault.push_back(BET_VALUE_5K);
	ArrListValueBetDefault.push_back(BET_VALUE_10K);
	ArrListValueBetDefault.push_back(BET_VALUE_50K);
	ArrListValueBetDefault.push_back(BET_VALUE_100K);
	ArrListValueBetDefault.push_back(BET_VALUE_500K);
	ArrListValueBetDefault.push_back(BET_VALUE_1M);
	//ArrListValueBetDefault.push_back(BET_VALUE_5M);
	SetRoomConfig();
	//add background
	log("Curenttype: %d", SceneManager::getSingleton().getCurrRoomType());
	Sprite* bg = Sprite::create("xocdia-bg.jpg");
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg->setScaleY(visibleSize.height / bg->getContentSize().height);
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	//bg->setScaleY(visibleSize.height / HEIGHT_DESIGN);
	this->addChild(bg);

    
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerBottomXocDia.csb");
    rootNode->setAnchorPoint(Point(0, 0));
    rootNode->setPosition(Point(0,0));
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);
    
    

    
	/*moc3- remove and add*/
	this->avatarBoss = CircleAvatar::create();
	this->avatarBoss->setAvatarImage("");
	this->avatarBoss->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->avatarBoss->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - this->avatarBoss->getContentSize().height / 2 + 55));

	this->btnCuopCai = ui::Button::create();
	this->btnCuopCai->loadTextures("ResXocDia/moc3-cuop-cai-normal.png", "ResXocDia/moc3-cuop-cai-active.png", "ResXocDia/moc3-cuop-cai-active.png");
	this->btnCuopCai->setAnchorPoint(Vec2(0, 1));
	this->btnCuopCai->setTitleFontSize(30);
	//this->btnCuopCai->setTitleFontName("__Roboto-Bold.ttf");
	this->btnCuopCai->setPosition(Vec2(this->avatarBoss->getPositionX() + this->avatarBoss->getContentSize().width / 2 - 23, visibleSize.height));
	this->btnCuopCai->setEnabled(false);
	this->btnCuopCai->setBright(false);
    this->btnCuopCai->setVisible(false);
	this->lblBoss = Label::createWithSystemFont(dataManager.GetSysString(42), "", 35);
	this->lblBoss->setHorizontalAlignment(TextHAlignment::CENTER);
	this->lblBoss->setVerticalAlignment(TextVAlignment::CENTER);
	this->lblBoss->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->btnCuopCai->addChild(this->lblBoss);
	this->lblBoss->setPosition(Point(this->btnCuopCai->getContentSize().width / 2,
		this->btnCuopCai->getContentSize().height / 2 + 10.0));

	this->btnCuopCai->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::processButtonCupCaiTouched, this));

	Sprite *nhaCaiBg = Sprite::create("moc3-nha-cai.png");
	nhaCaiBg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	nhaCaiBg->setPosition(Vec2(this->avatarBoss->getPositionX() - this->avatarBoss->getContentSize().width / 2 + 23, visibleSize.height));
    nhaCaiBg->setVisible(false);
	this->bossName = Label::createWithSystemFont(dataManager.GetSysString(43), "", 30);
	this->bossName->setHorizontalAlignment(TextHAlignment::CENTER);
	this->bossName->setVerticalAlignment(TextVAlignment::CENTER);
	this->bossName->setAnchorPoint(Vec2(0.5, 0.5));
	nhaCaiBg->addChild(this->bossName);

	this->lblMoneyBoss = Label::createWithTTF("", "__Roboto-Bold.ttf", 30);
	this->lblMoneyBoss->setHorizontalAlignment(TextHAlignment::CENTER);
	this->lblMoneyBoss->setVerticalAlignment(TextVAlignment::CENTER);
	this->lblMoneyBoss->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	nhaCaiBg->addChild(this->lblMoneyBoss);
	this->lblMoneyBoss->setPosition(Point(nhaCaiBg->getContentSize().width / 2, 30));

	//update 8/9
	//set khi he thong lam cai 
	// 	if (this->lstBoss.size() == 0){
	// 		this->bossName->setSystemFontSize(35);
	// 		this->bossName->setPosition(Point(nhaCaiBg->getContentSize().width / 2,
	// 			nhaCaiBg->getContentSize().height / 2 + 10.0));
	// 
	// 		this->lblMoneyBoss->setVisible(false);
	// 	}
	// 	else{
	this->bossName->setSystemFontSize(30);
	this->bossName->setPosition(Vec2(nhaCaiBg->getContentSize().width / 2,
		nhaCaiBg->getContentSize().height - 15.0));

	// 	}

//    Button* btnHistory = ui::Button::create();
//    btnHistory->loadTextures("ResXocDia/moc3-history.png", "", "");
//    btnHistory->setAnchorPoint(Vec2(0, 1));
//    btnHistory->setPosition(Vec2(btnCuopCai->getPositionX() + btnCuopCai->getContentSize().width + 20, visibleSize.height));
//    btnHistory->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::processButtonHistoryTouched, this));

    nhaCaiBg->setVisible(false);
    btnCuopCai->setVisible(false);
    this->avatarBoss->setVisible(false);
	this->addChild(nhaCaiBg);
	this->addChild(btnCuopCai);
	this->addChild(this->avatarBoss);
	//this->addChild(btnHistory);
	//Sprite* top =  Sprite::create("xocdia-top.png");
	//top->setAnchorPoint(Vec2(0,1));
	//top->setPosition(Vec2(0, visibleSize.height +sizeAdd.height));
	////bg->setScaleY(visibleSize.height / HEIGHT_DESIGN);
	//this->addChild(top);
	//
	//Sprite* tablebg = Sprite::create("xocdia-table.png");
	//tablebg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	//tablebg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 30));
	////bg->setScaleY(visibleSize.height / HEIGHT_DESIGN);
	//this->addChild(tablebg);
	/*moc3- end change*/

	mDiaNoc = ImageView::create("dia-1.png");
	mDiaNoc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	mDiaNoc->setPosition(Vec2(visibleSize.width / 2, this->avatarBoss->getPositionY() - mDiaNoc->getContentSize().height / 2 - 25));
	this->addChild(mDiaNoc);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			ImageView* img = ImageView::create("moc3-item-red-result.png");
			img->setAnchorPoint(Vec2::ZERO);
			img->setPosition(Vec2(500 / 2 - 55 + 55 * j, 500 / 2 + 12 - 55 * (i % 2)));
			img->setVisible(false);
			mDiaNoc->addChild(img);
			arrResult.push_back(img);
		}
	}
	for (int k = 0; k < 20; k++){
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				int ranx = rand() % 10;
				int posx = ranx - 5;
				int rany = rand() % 20;
				int posy = rany;

				posVi.push_back(Vec2(214 / 2 - 33 + (33 + posx) * j, 194 / 2 + 12 - (33 + posy) * (i % 2)));
			}
		}
	}

	this->lblBetMoney = static_cast<Text*>(rootNode->getChildByName("txtTongDat"));
    this->lblBetMoney->setString(dataManager.GetSysString(44)+" 0");

	this->lblWinMoney = static_cast<Text*>(rootNode->getChildByName("txtTongThu"));
    this->lblWinMoney->setString(dataManager.GetSysString(45)+" 0");

	/*Sprite* bgNum = Sprite::create("xocdia-online.png");
	bgNum->setPosition(Vec2(WIDTH_DESIGN / 6 * 4 + 25, HEIGHT_DESIGN / 6 * 5 + 40 + sizeAdd.height));
	this->addChild(bgNum);

	txtNumOfPlayers = Label::createWithTTF(dataManager.GetSysString(46)+" 0", ROBOTO_BOLD_FONT, 33);
	txtNumOfPlayers->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	txtNumOfPlayers->setColor(Color3B(78, 137, 174));
	txtNumOfPlayers->setPosition(Vec2(WIDTH_DESIGN / 6 * 4 + 25, HEIGHT_DESIGN / 6 * 5 + 40 + sizeAdd.height));
	this->addChild(txtNumOfPlayers);
	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
	txtNumOfPlayers->setPosition(Vec2(WIDTH_DESIGN / 6 * 4 + 25, HEIGHT_DESIGN / 6 * 5 + 37 + sizeAdd.height));
	}*/

	bgTimer = Sprite::create("moc3-xocdia-timer.png");
	bgTimer->setPosition(Vec2(WIDTH_DESIGN / 6 * 4 + 25, visibleSize.height / 2 + visibleSize.height / 4));
	this->addChild(bgTimer);

	txtTimer = Label::createWithTTF("", ROBOTO_BOLD_FONT, 50);
	txtTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	txtTimer->setColor(Color3B(255, 255, 255));
	txtTimer->setPosition(Vec2(this->bgTimer->getContentSize().width / 2, this->bgTimer->getContentSize().height / 2));
	this->bgTimer->addChild(txtTimer);
    if (SceneManager::getSingleton().isNagaWin){
        txtTimer->setColor(Color3B::BLACK);
        txtTimer->setPosition(Vec2(this->bgTimer->getContentSize().width / 2-2, this->bgTimer->getContentSize().height / 2+4));

    }

	txtMoney = Label::createWithBMFont("robotofont.fnt", "");
	txtMoney->setAnchorPoint(Vec2(1, 0));
	txtMoney->setPosition(Vec2(WIDTH_DESIGN - 20, HEIGHT_DESIGN - 500 + sizeAdd.height));
	txtMoney->setScale(1.1);
	txtMoney->setColor(_colorTextNormal);
	txtMoney->setVisible(false);
	this->addChild(txtMoney);
	this->DisplayInfoMoney(playerInfo->SoTienThuc, playerInfo->SoTienDatCuocClient);

	vector<string> arr;
	arr.push_back("tx-chip-01.png");
	arr.push_back("tx-chip-02.png");
	arr.push_back("tx-chip-03.png");
	arr.push_back("tx-chip-04.png");
	arr.push_back("tx-chip-05.png");
	arr.push_back("tx-chip-06.png");
	arr.push_back("tx-chip-07.png");
	//arr.push_back("tx-chip-08.png");
	vector <string> arr1;
	arr1.push_back("tx-chip-01.png");
	arr1.push_back("tx-chip-02.png");
	arr1.push_back("tx-chip-03.png");
	arr1.push_back("tx-chip-04.png");
	arr1.push_back("tx-chip-05.png");
	arr1.push_back("tx-chip-06.png");
	arr1.push_back("tx-chip-07.png");
	//arr1.push_back("tx-chip-08-disable.png");
	vector<string> arr2;
	arr2.push_back(BET_VALUE_1K);
	arr2.push_back(BET_VALUE_5K);
	arr2.push_back(BET_VALUE_10K);
	arr2.push_back(BET_VALUE_50K);
	arr2.push_back(BET_VALUE_100K);
	arr2.push_back(BET_VALUE_500K);
	arr2.push_back(BET_VALUE_1M);
	//arr2.push_back(BET_VALUE_5M);

	vector<int> arrTagC;
	arrTagC.push_back(tag_BET_VALUE_1K);
	arrTagC.push_back(tag_BET_VALUE_5K);
	arrTagC.push_back(tag_BET_VALUE_10K);
	arrTagC.push_back(tag_BET_VALUE_50K);
	arrTagC.push_back(tag_BET_VALUE_100K);
	arrTagC.push_back(tag_BET_VALUE_500K);
	arrTagC.push_back(tag_BET_VALUE_1M);
	//arrTagC.push_back(tag_BET_VALUE_5M);


	for (int i = 0; i < ArrListValueBetDefault.size(); i++)
	{
		CoinXocDia* btn = CoinXocDia::create(arr2.at(i), arr.at(i), arr1.at(i));
		btn->setPressedActionEnabled(false);
		btn->setTouchEnabled(true);
        btn->setScale(0.85);
		btn->setAnchorPoint(Vec2(0.5, 0));
		float posX = 642;
		btn->setPosition(Vec2(posX + 165 * i*0.85, 115));
		int tag = 152 + i;
		btn->setTag(tag);
		this->addChild(btn);

		ArrayListCoinOrg.push_back(BtnCoin(btn, tag));
	}

	initListCoin();
	Size size = Size(219, 219);
	UrlCollection url1 = UrlCollection("xocdia-1x2.png", "xocdia-1x2-active.png", "xocdia-1x2-press.png", "xocdia-1x2-an.png");
	BetXocDiaChanLe* btnChan = BetXocDiaChanLe::create(BET_TYPE_CHAN);
	btnChan->loadTextures("xocdia-1x2.png", "xocdia-1x2-press.png", "");
	/*moc3- change*/
	btnChan->setAnchorPoint(Vec2(0.5, 0.5));
	btnChan->setPosition(Vec2(btnChan->getContentSize().width + size.width / 2 - 20, visibleSize.height / 2 + 100 + sizeAdd.height));
	/*moc3- end change*/
	btnChan->setPressedActionEnabled(true);
	btnChan->setTag(tag_XD_BET_TYPE_CHAN);
	this->addChild(btnChan);
	CreateContainerButton(btnChan, BET_TYPE_CHAN, VALUE_BOBA, tag_XD_BET_TYPE_CHAN, BET_TYPE_CHAN, url1);

	BetXocDiaChanLe* btnLe = BetXocDiaChanLe::create(BET_TYPE_LE);
	btnLe->loadTextures("xocdia-1x2.png", "xocdia-1x2-press.png", "");
	/*moc3 - change*/
	btnLe->setAnchorPoint(Vec2(0.5, 0.5));
	btnLe->setPosition(Vec2(visibleSize.width - (btnLe->getContentSize().width + size.width / 2) + 20, visibleSize.height / 2 + 100 + sizeAdd.height));
	/*moc3- end change*/
	btnLe->setPressedActionEnabled(true);
	btnLe->setTag(tag_XD_BET_TYPE_LE);
	this->addChild(btnLe);
	CreateContainerButton(btnLe, BET_TYPE_LE, VALUE_BOBA, tag_XD_BET_TYPE_LE, BET_TYPE_LE, url1);

	/*moc3-change*/
	float startLeft = btnChan->getPositionX() + btnChan->getContentSize().width / 2 - 25;
	/*moc3-end change*/
	vector<string> arrT;
	arrT.push_back(BET_TYPE_CAP_4_0);
	arrT.push_back(BET_TYPE_CAP_0_4);
	arrT.push_back(BET_TYPE_CAP_3_1);
	arrT.push_back(BET_TYPE_CAP_1_3);
	vector<int> arrT2;
	arrT2.push_back(tag_XD_BET_TYPE_CAP_4_0);
	arrT2.push_back(tag_XD_BET_TYPE_CAP_0_4);
	arrT2.push_back(tag_XD_BET_TYPE_CAP_3_1);
	arrT2.push_back(tag_XD_BET_TYPE_CAP_1_3);
	UrlCollection url2 = UrlCollection("xocdia-1x6-1x3.png", "xocdia-1x6-1x3-active.png", "xocdia-1x6-1x3-press.png", "xocdia-1x6-1x3-an.png");
	for (int i = 0; i < 4; i++)
	{
		BetXocDiaDiem* img = BetXocDiaDiem::create(arrT.at(i));
		img->loadTextures("xocdia-1x6-1x3.png", "xocdia-1x6-1x3-press.png", "");
		img->setAnchorPoint(Vec2::ZERO);
		float posY = visibleSize.height / 2 - 70 + sizeAdd.height;
		if (i == 0 || i == 3){
			posY = visibleSize.height / 2 - 120 + sizeAdd.height;
		}
		else{
			posY = visibleSize.height / 2 - 150 + sizeAdd.height;
		}
		img->setPosition(Vec2(startLeft + i * size.width + 5, posY));
		img->setPressedActionEnabled(true);
		img->setTag(arrT2.at(i));
		this->addChild(img);
		this->CreateContainerButton(img, arrT.at(i).c_str(), VALUE_BOBA, arrT2.at(i), arrT.at(i).c_str(), url2);
	}

	//layerHistory
	layerHistory = LayerHistoryXocDia::create();
	layerHistory->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	log("width: %f, height: %f", layerHistory->getContentSize().width, layerHistory->getContentSize().height);
	layerHistory->setPosition(Vec2(visibleSize.width / 2 - layerHistory->getContentSize().width / 2, visibleSize.height - layerHistory->getContentSize().height));
	this->addChild(layerHistory);
	this->layerHistory->setVisible(false);
	Button *btnExitHistory = this->layerHistory->getButtonClose();
	if (btnExitHistory != NULL){
		btnExitHistory->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::processButtonExitTouched, this));
	}

	//chon muc coin cua nguoi choi
	SelectCoin(playerInfo->SoTienThuc);
	//Layer toast
	Toast = LayerChanToast::create();
	Toast->setLocalZOrder(1000);
	this->addChild(Toast);

	//Button in game
	layerButton = LayerButtonInGame::create();
	layerButton->hideButtonInChanGame();
	layerButton->setLocalZOrder(400);
	this->addChild(layerButton);

	/*moc3-end change*/
	//this->mAvatar = AvatarXiTo::create();
	this->mAvatar = Avatar52La::create(false, 1);
	//this->mAvatar->loadImgIconRechardMe();
	this->mAvatar->setPosition(Vec2(472,160));
	this->addChild(mAvatar);
	this->mAvatar->hideLayerInvite();
	this->mAvatar->setAI(MyAI);
	this->mAvatar->displayButtonRechard(true);

	/*moc3- change aN=>adn*/
	boost::shared_ptr<UserVariable> aNptr = mySelf->GetVariable("adn");
	if (aNptr)
	{
		mAvatar->setName(aNptr->GetStringValue()->c_str());
	}
	else{
		mAvatar->setName(MyAI);
	}
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		boost::shared_ptr<UserVariable> amfptr2 = mySelf->GetVariable("amf");
		if (amfptr2)
		{
			mAvatar->setMoney(*amfptr2->GetDoubleValue());
		}
	}
	else{
		boost::shared_ptr<UserVariable> amfptr2 = mySelf->GetVariable("amfs");
		if (amfptr2)
		{
			mAvatar->setMoney(*amfptr2->GetDoubleValue());
		}
	}
	//test
	//this->updateListUser();

	auto btnListBoss = layerButton->getButtonListCai();
	btnListBoss->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::processButtonDanhSachCaiTouched, this));
    btnListBoss->setVisible(false);
	//update 8/9
	this->initAllListAvatar();
    this->btnDongTien = Button::create();
    if (SceneManager::getSingleton().getCurrRoomType() ==0 ){
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"dongmoc-min.png");
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"dongmoc-min.png");
    }else{
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"donghao-min.png");
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"donghao-min.png");
    }
    this->addChild(this->btnDongTien);
    this->btnDongTien->setPosition(Vec2(87,730+sizeAdd.height));
    this->btnDongTien->setContentSize(Size(100,100));
    this->btnDongTien->setScale(1.1);
    this->btnDongTien->setPressedActionEnabled(false);
    this->btnDongTien->addClickEventListener(CC_CALLBACK_1(LayerGameXocDia::onBtnChuyenDongTien, this));
    
    if (SceneManager::getSingleton().isNagaWin){
        this->btnDongTien->setVisible(false);
    }
    
    this->chat = LayerChatGame::create();
    this->addChild(this->chat,2000);
    this->chat->setPosition(Vec2(1100, 0));
    
    Button *btnDatX2 = static_cast<Button*>(rootNode->getChildByName("btnDatX2"));
    if (btnDatX2 != NULL){
        btnDatX2->addClickEventListener(CC_CALLBACK_1(LayerGameXocDia::processButtonDatX2, this));
        btnDatX2->loadTextures(StringUtils::format("design/ResXD/%sbetx2-act.png",SceneManager::getSingleton().getFolderLang().c_str()), StringUtils::format("design/ResXD/%sbetx2-dis.png",SceneManager::getSingleton().getFolderLang().c_str()),StringUtils::format("design/ResXD/%sbetx2-dis.png",SceneManager::getSingleton().getFolderLang().c_str()));
    }
    
    this->txtPhienGanNhat = static_cast<Text*>(rootNode->getChildByName("txtPhienGanNhat"));
    if (this->txtPhienGanNhat){
        this->txtPhienGanNhat->setString(dataManager.GetSysString(856));
    }
    
    for (int i = 1; i <= 5; i++){
        for (int j = 1; j <= 4; j++){
                auto img = static_cast<ImageView*>(rootNode->getChildByName(StringUtils::format("imgHis%d%d",i,j)));
            img->setVisible(false);
                this->lstImgHis.push_back(img);
        }
    }
    
    auto imgNhaCai = Sprite::create("ResXD/nha-cai-lamcai.png");
    imgNhaCai->setAnchorPoint(Vec2(0.5,1));
    imgNhaCai->setPosition(Vec2(WIDTH_DESIGN /2, visibleSize.height));
    imgNhaCai->setTexture(StringUtils::format("design/ResXD/%snha-cai-lamcai.png",SceneManager::getSingleton().getFolderLang().c_str()));
    this->addChild(imgNhaCai);
    imgNhaCai->setVisible(false);
    auto userIcon = Sprite::create("ResXD/use-icon.png");
    userIcon->setAnchorPoint(Vec2(1,1));
    userIcon->setPosition(Vec2(WIDTH_DESIGN -27, visibleSize.height-280));
    this->addChild(userIcon);
    
    this->lblUserCount = Label::createWithTTF("", ROBOTO_BOLD_FONT, 45);
    this->addChild(this->lblUserCount);
    this->lblUserCount->setAnchorPoint(Vec2(0.5,1));
    this->lblUserCount->setPosition(Vec2(userIcon->getPositionX() - userIcon->getContentSize().width/2, visibleSize.height-385));
    
    
    auto btnXepHang = Button::create();
    btnXepHang->loadTextureNormal("design/ResXD/moc3-popup-menu-rank.png");
    btnXepHang->loadTexturePressed("design/ResXD/moc3-popup-menu-rank.png");
   
    this->addChild(btnXepHang);
    btnXepHang->setAnchorPoint(Vec2(1,1));

    btnXepHang->setPosition(Vec2(WIDTH_DESIGN -27, visibleSize.height-150));
    btnXepHang->addClickEventListener(CC_CALLBACK_1(LayerGameXocDia::onBtnXepHang, this));
    GameServer::getSingleton().addListeners(this);
    SceneManager::getSingleton().showLoading();

	return true;
}

void LayerGameXocDia::onBtnXepHang(Ref* pSender){
    LayerBangXepHangTo* xh = LayerBangXepHangTo::create();
    xh->loadTopByType();
    this->addChild(xh,700000);
    
}
void LayerGameXocDia::processButtonDatX2(Ref* pSender){
    
}
void LayerGameXocDia::onButtonSilver(Ref* pSender){

//    if (SceneManager::getSingleton().getMoneyByType(1) < SceneManager::getSingleton().getMinMoney(1114)){
//        SceneManager::getSingleton().hideLoading();
//        Chat* toast = Chat::create(StringUtils::format("%s %s %s %s %s !",dataManager.GetSysString(47).c_str(),mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(1114))->c_str(), SceneManager::getSingleton().getCoin(1).c_str(),dataManager.GetSysString(48).c_str(), SceneManager::getSingleton().getCoin(1).c_str()), -1);
//        this->addChild(toast, 399);
//
//        return;
//    }

	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (NULL == lastRoom)
	{
		Chat* toast = Chat::create(dataManager.GetSysString(49), -1);
		this->addChild(toast, 399);

		return;
	}
	else
	{
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
		GameServer::getSingleton().Send(request);
	}


	/* moc3-change
	this->btnGold->setEnabled(true);
	this->btnGold->setBright(true);
	this->btnSilver->setEnabled(false);
	this->btnSilver->setBright(false);

	moc3-end change*/
	this->ChuyenTab = true;

	SceneManager::getSingleton().setCurrRoomType(1);
	SceneManager::getSingleton().setIsChuyenTab(true);
	this->ResetGame(false);
	initPlayerInfo();
	SelectCoin(playerInfo->SoTienThuc);
}
void LayerGameXocDia::onBtnChuyenDongTien(Ref* sender){
    this->isJoin = false;
    for (int i = 0 ; i < this->lstImgHis.size() ; i++){
        this->lstImgHis[i]->setVisible(false);
    }
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (NULL == lastRoom)
        {
            Chat* toast = Chat::create(dataManager.GetSysString(49), -1);
            this->addChild(toast, 399);
            SceneManager::getSingleton().gotoMain();

            return;
        }
        else
        {
            SceneManager::getSingleton().showLoading();
            boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
            GameServer::getSingleton().Send(request);
        }
        
        ///*moc3-change

        

        
        //moc3-endchange*/
        this->ChuyenTab = true;
        
        SceneManager::getSingleton().setIsChuyenTab(true);
        this->ResetGame(false);
        initPlayerInfo();
        
        
    }else{
        boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (NULL == lastRoom)
        {
            Chat* toast = Chat::create(dataManager.GetSysString(49), -1);
            this->addChild(toast, 399);
            return;
        }
        else
        {
            SceneManager::getSingleton().showLoading();
            boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
            GameServer::getSingleton().Send(request);
        }
        
        ///*moc3-change
    
        //moc3-endchange*/
        this->ChuyenTab = true;
        
        SceneManager::getSingleton().setIsChuyenTab(true);
        this->ResetGame(false);
        initPlayerInfo();
        
    }
    SceneManager::getSingleton().showLoading();
 
}
void LayerGameXocDia::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
//    // Get the user parameter of the event
//    return;
    boost::shared_ptr<map<string, boost::shared_ptr<void>>> ptrEventParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
    boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
    // Get the room parameter of the event
    boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEventParams)["room"];
    boost::shared_ptr<Room> ptrNotifiedRoom = ((boost::static_pointer_cast<Room>))(ptrEventParamValueRoom);
    //
    if (ptrNotifiedUser == NULL || ptrNotifiedRoom == NULL) return;
    
    if (ptrNotifiedRoom->GroupId()->compare(SERVICE_CHAT_ROOM) == 0
        || ptrNotifiedRoom->Id() == 118 || ptrNotifiedRoom->GroupId()->compare("200") == 0
        || ptrNotifiedRoom->Id() == 200)
        return;
    //if it is me
    if (!ptrNotifiedUser->IsItMe()){
        return;
    }
    if (this->isJoin) return;
    if (SceneManager::getSingleton().getCurrRoomType() == -1 || !SceneManager::getSingleton().getIsChuyenTab()) return;
    if (SceneManager::getSingleton().getGameID() != kGameXocDia) return;
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameXocDia));
       // log("list room : %d", list_room->size());
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = *room->GetVariable("roomType")->GetIntValue();
    

                    if (roomType == 1){
                        SceneManager::getSingleton().setCurrRoomType(1);
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }else{
        boost::shared_ptr<vector<boost::shared_ptr<Room> > > list_room = GameServer::getSingleton().getSmartFox()->GetRoomListFromGroup(boost::to_string(kGameXocDia));
       // log("list room : %d", list_room->size());
        for (int i = 0; i < list_room->size(); i++)
        {
            boost::shared_ptr<Room> room = list_room->at(i);
            if (room != NULL)
            {
                if (room->GetVariable("roomType") != NULL){
                    int roomType = *room->GetVariable("roomType")->GetIntValue();
                    //log("roomType: %d - currRoom: %d", roomType, getCurrRoomType());
                    /*if (getCurrRoomType() == 0){
                     if (this->getMoneyByType(0) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(1);
                     }

                     if (getCurrRoomType() == 1){
                     if (this->getMoneyByType(1) < this->getMinMoney(kGameXocDia))
                     setCurrRoomType(0);
                     }*/

                    if (roomType == 0){
                        SceneManager::getSingleton().setCurrRoomType(0);
                        boost::shared_ptr<long> lastRoomID(new long(-1));
                        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
                        if (lastRoom)
                            lastRoomID = boost::shared_ptr<long>(new long(lastRoom->Id()));
                       // log("Room Tai Xiu != NULL");
                        boost::shared_ptr<IRequest> request(new JoinRoomRequest(room, "", lastRoomID, false));
                        GameServer::getSingleton().Send(request);
                    }
                }
            }
        }
    }
}
void LayerGameXocDia::OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    //return;
    if (this->chat != NULL)
        this->chat->loadAllDatas();
    if (!this->btnDongTien || !SceneManager::getSingleton().getIsChuyenTab()) return;
    if (SceneManager::getSingleton().getCurrRoomType() ==0 ){
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"dongmoc-min.png");
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"dongmoc-min.png");
    }else{
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"donghao-min.png");
        this->btnDongTien->loadTextureNormal(PATH_SLOT+"donghao-min.png");
    }
    this->initPlayerInfo();
    this->initListCoin();
    SelectCoin(playerInfo->SoTienThuc);
}
void LayerGameXocDia::onButtonGold(Ref* pSender){

//    if (SceneManager::getSingleton().getMoneyByType(0) < SceneManager::getSingleton().getMinMoney(114)){
//        SceneManager::getSingleton().hideLoading();
//        Chat* toast = Chat::create(StringUtils::format("%s %s %s %s %s !",dataManager.GetSysString(47).c_str(), mUtils::format_money((long)SceneManager::getSingleton().getMinMoney(114))->c_str(), SceneManager::getSingleton().getCoin(0).c_str(), dataManager.GetSysString(48).c_str(),SceneManager::getSingleton().getCoin(0).c_str()), -1);
//        this->addChild(toast, 399);
//
//        return;
//    }

	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (NULL == lastRoom)
	{
		Chat* toast = Chat::create(dataManager.GetSysString(49), -1);
		this->addChild(toast, 399);
		return;
	}
	else
	{
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<IRequest> request(new LeaveRoomRequest());
		GameServer::getSingleton().Send(request);
	}

	///*moc3-change
    this->btnGold->setEnabled(false);
    this->btnGold->setBright(false);
    this->btnSilver->setEnabled(true);
    this->btnSilver->setBright(true);

	//moc3-endchange*/
	this->ChuyenTab = true;

	SceneManager::getSingleton().setCurrRoomType(0);
	SceneManager::getSingleton().setIsChuyenTab(true);
	this->ResetGame(false);
	initPlayerInfo();
	SelectCoin(playerInfo->SoTienThuc);
}
void LayerGameXocDia::sendRequestInfoGame()
{

}

void LayerGameXocDia::initListCoin()
{
    arrCoin.clear();
	for (int i = 0; i < ArrListValueBet.size(); i++)
	{
		this->CreateContainerCoin(
			ArrayListCoinOrg.at(i).btn,
			"btn1k",
			ArrayListCoinOrg.at(i).tag,
			ArrListValueBet.at(i).c_str()
			);
	}
	if (ArrListValueBet.size() < ArrayListCoinOrg.size())
	{
		int len = ArrListValueBet.size();
		for (int i = len; i < ArrayListCoinOrg.size(); i++)
		{
			ArrayListCoinOrg.at(i).btn->setVisible(false);
		}
	}
}

void LayerGameXocDia::SelectCoin(long _money)
{
	string vals = SelectCoinValue(_money, 0);
	bool ret = false;
	for (int i = 0; i < arrCoin.size(); i++)
	{
		TaiXiuCoin *pCoin = arrCoin.at(i);
		if (0 == pCoin->GetType().compare(vals))
		{
			pCoin->SetSelected(true);
			CurrentBetValue = vals;
			ret = true;
		}
		else{
			pCoin->SetSelected(false);
		}
	}
	if (!ret){
		CurrentBetValue = "";
	}
}
string LayerGameXocDia::SelectCoinValue(long money, int ext)
{
	bool r1 = ArrListValueBet.at(0).compare(BET_VALUE_1K) == 0;
	if (r1)
	{
        if (SceneManager::getSingleton().getCurrRoomType() == 1){
            if (money < 2000)
                return "";
            double per;
            if (0 == ext) {
                per = money / 20;
            }
            else {
                per = money;
            }
            
            if (per >= 100 && per <= 2000) {
                return BET_VALUE_2K;
            }
            else if (per > 2000 && per < 10000) {
                return BET_VALUE_5K;
            }
            
            //10k
            else if (per >= 10000 && per < 50000) {
                return BET_VALUE_10K;
            }
            //50k
            else if (per >= 50000 && per < 100000) {
                return BET_VALUE_50K;
            }
            
            else if (per >= 100000 && per < 500000) {
                return BET_VALUE_100K;
            }
            
            else if (per >= 500000 && per < 1000000) {
                return BET_VALUE_500K;
            }
            return BET_VALUE_1M;
        }else{
            if (money < 100)
                return "";
            double per;
            if (0 == ext) {
                per = money / 20;
            }
            else {
                per = money;
            }
            
            if (per >= 5 && per < 500) {
                return BET_VALUE_100;
            }
            
            else  if (per >= 500 && per < 1000) {
                return BET_VALUE_500;
            }
            
            else if (per >= 1000 && per < 5000) {
                return BET_VALUE_1K;
            }
            else if (per >= 5000 && per < 10000) {
                return BET_VALUE_5K;
            }
            else if (per >= 10000 && per < 50000) {
                return BET_VALUE_10K;
            }
            
            else if (per >= 50000 &&  per < 500000) {
                return BET_VALUE_100K;
            }
            
            else if (per >= 500000 && per < 1000000) {
                return BET_VALUE_1M;
            }
            return BET_VALUE_1M;
        } 
	}
	else{
        if (SceneManager::getSingleton().getCurrRoomType() == 1){
            if (money < 2000)
                return "";
            double per;
            if (0 == ext) {
                per = money / 20;
            }
            else {
                per = money;
            }
            
            if (per >= 100 && per <= 2000) {
                return BET_VALUE_2K;
            }
            else if (per > 2000 && per < 10000) {
                return BET_VALUE_5K;
            }
            
            //10k
            else if (per >= 10000 && per < 50000) {
                return BET_VALUE_10K;
            }
            //50k
            else if (per >= 50000 && per < 100000) {
                return BET_VALUE_50K;
            }
            
            else if (per >= 100000 && per < 500000) {
                return BET_VALUE_100K;
            }
            
            else if (per >= 500000 && per < 1000000) {
                return BET_VALUE_500K;
            }
            return BET_VALUE_1M;
        }else{
            if (money < 100)
                return "";
            double per;
            if (0 == ext) {
                per = money / 20;
            }
            else {
                per = money;
            }
            
            if (per >= 5 && per < 500) {
                return BET_VALUE_100;
            }
            
            else  if (per >= 500 && per < 1000) {
                return BET_VALUE_500;
            }
            
            else if (per >= 1000 && per < 5000) {
                return BET_VALUE_1K;
            }
            else if (per >= 5000 && per < 10000) {
                return BET_VALUE_5K;
            }
            else if (per >= 10000 && per < 50000) {
                return BET_VALUE_10K;
            }
            
            else if (per >= 50000 &&  per < 500000) {
                return BET_VALUE_100K;
            }
            
            else if (per >= 500000 && per < 1000000) {
                return BET_VALUE_1M;
            }
            return BET_VALUE_1M;
        }
	}
}

void LayerGameXocDia::myTimer()
{

}

void LayerGameXocDia::CreateContainerButton(Button* btn, const char* name, const int& group, const int& tag, const char* type, UrlCollection url)
{
	btn->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::OnButtonBetClick, this));
	TaiXiuBet* pTaiXiu = TaiXiuBetFactory::CreateTXBet(group, btn, name, tag, type);
	pTaiXiu->SetTarget(this);
	pTaiXiu->SetImage(url.normal, url.press, url.active, url.an);
	//tao o ghi chu
	SuggestTaiXiu* suggest = SuggestTaiXiu::create();
	suggest->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	suggest->loadTexture("tx-popup-press.png");

	Vec2 ppos = btn->getPosition();
	Vec2 point = btn->getPosition();
	Size size = Size(0, 0);

	if (0 == strcmp(type, BET_TYPE_CHAN))
	{
		/*moc3-change*/
		/*size.setSize(320, 155);
		point.x = ppos.x - 70;*/
		size.setSize(305, 304);
		point.y = ppos.y - 70;
		/*moc3-end change*/
	}
	else if (0 == strcmp(type, BET_TYPE_LE))
	{
		/*moc3-change*/
		/*size.setSize(320, 155);
		point.x = ppos.x + 70;*/
		size.setSize(305, 304);
		point.y = ppos.y - 70;
		/*moc3-end change*/
	}
	else if (0 == strcmp(type, BET_TYPE_CAP_0_4)
		|| 0 == strcmp(type, BET_TYPE_CAP_1_3)
		|| 0 == strcmp(type, BET_TYPE_CAP_3_1)
		|| 0 == strcmp(type, BET_TYPE_CAP_4_0))
	{
		/*moc3-change*/
		/*size.setSize(288, 108);
		point.x = ppos.x + 190;
		point.y = ppos.y + size.height / 2;*/
		size.setSize(219, 219);
		point.x = ppos.x + 110;
		point.y = ppos.y + size.height / 2;
		/*moc3-end change*/
	}

	suggest->setAnchorPoint(btn->getAnchorPoint());
	suggest->setPosition(Vec2(btn->getPositionX(), btn->getPositionY() + size.height - 15));
	suggest->setVisible(false);
	this->addChild(suggest, CountZoder++);

	pTaiXiu->SetSuggest(suggest);
	pTaiXiu->SetWorldPos(point);
	arrBet.push_back(pTaiXiu);
}

void LayerGameXocDia::CreateContainerCoin(Button* btn, const char* name, const int& tag, const char* type)
{
	btn->setTag(tag);
	btn->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::OnButtonChipClick, this));
	TaiXiuCoin * pCoin = CreateTaiXiuCoin(btn, name, type, tag);
	arrCoin.push_back(pCoin);
}

TaiXiuCoin* LayerGameXocDia::CreateTaiXiuCoin(Button* item, const std::string& name, const std::string& type, const int& tag)
{
	if (type == BET_VALUE_1) {
		return new TaiXiuCoin1(item, name, type, tag);
	}
	else if (type == BET_VALUE_100){
		return new  TaiXiuCoin100(item, name, type, tag);
	}
	else if (type == BET_VALUE_500){
		return new TaiXiuCoin500(item, name, type, tag);
	}
	else if (type == BET_VALUE_1K) {
		return new TaiXiuCoin1K(item, name, type, tag);
	}
	else if (type == BET_VALUE_2K) {
		return new TaiXiuCoin2K(item, name, type, tag);
	}
	else if (type == BET_VALUE_5K) {
		return new TaiXiuCoin5K(item, name, type, tag);
	}
	else if (type == BET_VALUE_10K) {
		return new TaiXiuCoin10K(item, name, type, tag);
	}
	else if (type == BET_VALUE_50K) {
		return new TaiXiuCoin50K(item, name, type, tag);
	}
	else if (type == BET_VALUE_100K) {
		return new TaiXiuCoin100K(item, name, type, tag);
	}
	else if (type == BET_VALUE_500K) {
		return new TaiXiuCoin500K(item, name, type, tag);
	}
	else if (type == BET_VALUE_1M) {
		return new TaiXiuCoin1M(item, name, type, tag);
	}
	else if (type == BET_VALUE_5M) {
		return new TaiXiuCoin5M(item, name, type, tag);
	}
	return NULL;
}

TaiXiuBet* LayerGameXocDia::GetContainerBetByButtonTag(const int& tag)
{
	for (int i = 0; i < arrBet.size(); i++)
	{
		TaiXiuBet* bet = arrBet.at(i);
		if (bet->GetTag() == tag)
			return bet;
	}

	return NULL;
}

TaiXiuBet* LayerGameXocDia::GetContainerBetByBetType(const string& bettype)
{
	for (int i = 0; i < arrBet.size(); i++)
	{
		TaiXiuBet* bet = arrBet.at(i);
		if (0 == bet->GetType().compare(bettype))
			return bet;
	}

	return NULL;
}

TaiXiuCoin* LayerGameXocDia::GetTaiXiuCoin(const string& type)
{
	for (int i = 0; i < arrCoin.size(); i++)
	{
		TaiXiuCoin* bet = arrCoin.at(i);
		if (0 == bet->GetType().compare(type))
			return bet;
	}

	return NULL;
}

int LayerGameXocDia::GetMoneyBet(const std::string& gbv)
{
	if (0 == gbv.compare(BET_VALUE_1))
		return 1;
	else if (0 == gbv.compare(BET_VALUE_100))
		return 100;
	else  if (0 == gbv.compare(BET_VALUE_500))
		return 500;
	else if (0 == gbv.compare(BET_VALUE_1K))
		return 1000;
	else if (0 == gbv.compare(BET_VALUE_2K))
		return 2000;
	else if (0 == gbv.compare(BET_VALUE_5K))
		return 5000;
	else if (0 == gbv.compare(BET_VALUE_10K))
		return 10000;
	else if (0 == gbv.compare(BET_VALUE_50K))
		return 50000;
	else if (0 == gbv.compare(BET_VALUE_100K))
		return 100000;
	else if (0 == gbv.compare(BET_VALUE_500K))
		return 500000;
	else if (0 == gbv.compare(BET_VALUE_1M))
		return 1000000;
	else if (0 == gbv.compare(BET_VALUE_5M))
		return 5000000;
	else
		return 0;
}

void LayerGameXocDia::OnButtonChipClick(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		if (this->status == 0){
			TaiXiuCoin *_current = NULL;
			for (int i = 0; i < arrCoin.size(); i++)
			{
				TaiXiuCoin* bet = arrCoin.at(i);
				if (0 == bet->GetType().compare(CurrentBetValue))
				{
					_current = arrCoin.at(i);
					break;
				}
			}
			//TaiXiuCoin* item = (TaiXiuCoin*)sender;
			Button* item = (Button*)sender;
			for (int i = 0; i < arrCoin.size(); i++)
			{
				TaiXiuCoin* pCoin = arrCoin.at(i);
				if (pCoin->GetTag() == item->getTag())
				{
					if (SceneManager::getSingleton().getCurrRoomType() == 0){
						boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
						if (mySelf)
						{
							boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable(EXT_VAL_AMOUNT_BALANCE);
							if (amfptr)
							{
								int money = pCoin->GetValue().value;
								double amf = *amfptr->GetDoubleValue();
								if (amf < money)
								{
									Toast->ShowToast(dataManager.GetSysString(33));
								}
								else{
									_current = pCoin;
								}
							}
						}
					}
					else{
						boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
						if (mySelf)
						{
							boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable("amfs");
							if (amfptr)
							{
								int money = pCoin->GetValue().value;
								double amf = *amfptr->GetDoubleValue();
								if (amf < money)
								{
									Toast->ShowToast(dataManager.GetSysString(33));
								}
								else{
									_current = pCoin;
								}
							}
						}
					}
				}
				else{
					pCoin->SetSelected(false);
				}
			}
			if (_current != NULL)
			{
				_current->SetSelected(true);
				CurrentBetValue = _current->GetType();
			}
		}
		else{
			SceneManager::getSingleton().showToast(dataManager.GetSysString(37));
		}
	}
}

void LayerGameXocDia::OnButtonBetClick(Ref *sender, ui::Widget::TouchEventType pType)
{
    if (SceneMain::getSingleton().chatting == true) return;

	if (this->status == 0){
		if (pType == ui::Widget::TouchEventType::BEGAN){

			Button* btn = (Button*)sender;
			this->CurrentTag = btn->getTag();
			this->press = 0;
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updatepress),this,0.1, false);

		}
		if (pType == ui::Widget::TouchEventType::ENDED) {

			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updatepress),this);
			this->HideAllSuggestion();

			log("long press = %f", this->press);
			if (this->press >= this->longpresstime)
			{
				return;
			}

			Button* btn = (Button*)sender;
			int tag = btn->getTag();
			this->BetGameByClient(tag);
		}

		if (pType == ui::Widget::TouchEventType::CANCELED){
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updatepress),this);
			this->HideAllSuggestion();
		}
	}
	else{
		SceneManager::getSingleton().showToast(dataManager.GetSysString(37));
	}
}

void LayerGameXocDia::OnButtonHistoryClick(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		removeChildByTag(TAG_TAI_XIU_HISTORY);
		for (int i = 0; i < arrBet.size(); i++)
		{
			arrBet.at(i)->HideCurrentBet();
		}
	}
}

void LayerGameXocDia::OnButtonTopWinClick(Ref *sender, ui::Widget::TouchEventType pType)
{

}

void LayerGameXocDia::initPlayerInfo()
{
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf)
		{
			boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable("amf");
			if (amfptr)
			{
				double amf = *amfptr->GetDoubleValue();
				playerInfo->initMoney(amf);
                //SelectCoin(playerInfo->SoTienThuc);
			}
		}
	}
	else{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf)
		{
			boost::shared_ptr<UserVariable> amfptr = mySelf->GetVariable("amfs");
			if (amfptr)
			{
				double amf = *amfptr->GetDoubleValue();
				playerInfo->initMoney(amf);
                //SelectCoin(playerInfo->SoTienThuc);
			}
		}
	}
	//this->mAvatar->reloadCoin();

}

void LayerGameXocDia::CallBackFromAnim()
{
	log("CallBackFromAnim");
	Animating = false;
	//hien thi ket qua van choi
	if (0 != rg.compare(""))
	{
		listHistory.push_back(rg);
		if (listHistory.size() > 20)
		{
			listHistory.erase(listHistory.begin());
		}
	}
	if (0 != rt10.compare(""))
	{

	}

	//update 8/9
	//update next boss
	if (this->bossNext.compare("") != 0)
		this->updateInfoCai(this->bossNext);
}

void LayerGameXocDia::updateEvent(float dt)
{
	if (mQueueMsg == NULL) return;
	if (mQueueMsg->isQueueEmpty())
		return;
	mQueueMsg->run();
}

void LayerGameXocDia::OnExtensionResponseProcess()
{
	this->param = mQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
	if (0 == cmd->compare("hbc"))
	{
		return;
	}
	else if (0 == cmd->compare(EXT_EVENT_START))
	{
		event_EXT_EVENT_START();
	}
	else if (0 == cmd->compare(EXT_EVENT_END))
	{
		event_EXT_EVENT_END();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT))
	{
		event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY))
	{
		event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_INFO_NTF))
	{
		event_EXT_EVENT_GAME_INFO_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_HIS_NTF))
	{
		event_EXT_EVENT_GAME_HIS_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF))
	{
		event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF))
	{
		event_EXT_EVENT_GAME_STATE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF))
	{
		event_EXT_EVENT_GAME_BET_NTF();
	}
	else if (0 == cmd->compare("updateinfome"))
	{
		if (SceneManager::getSingleton().getCurrRoomType() == 0){
			boost::shared_ptr<double> amfptr = param->GetDouble("amf");
			boost::shared_ptr<string> uidptr = param->GetUtfString("uid");
			if (uidptr && amfptr)
			{
				string uid = uidptr->c_str();
				if (0 == uid.compare(MyAI))
				{
					playerInfo->SoTienAo = *amfptr;
				}
				DisplayInfoMoney(*amfptr, playerInfo->SoTienDatCuocClient);
			}
		}
		else{
			boost::shared_ptr<double> amfptr = param->GetDouble("amfs");
			boost::shared_ptr<string> uidptr = param->GetUtfString("uid");
			if (uidptr && amfptr)
			{
				string uid = uidptr->c_str();
				if (0 == uid.compare(MyAI))
				{
					playerInfo->SoTienAo = *amfptr;
				}
				DisplayInfoMoney(*amfptr, playerInfo->SoTienDatCuocClient);
			}
		}
	}
	else if (0 == cmd->compare(EXT_EVENT_REGISTER_BOSS_RES)){
		log("regbrs");
		this->event_EXT_EVENT_REGISTER_BOSS_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_UNREGISTER_BOSS_RES)){
		log("unregbrs");
		this->event_EXT_EVENT_UNREGISTER_BOSS_RES();
	}
	else if (0 == cmd->compare(EXT_EVENT_NEXT_BOSS_NTF)){
		log("nbntf");
		this->event_EXT_EVENT_NEXT_BOSS_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_DEL_BOSS_NTF)){
		log("dbntf");
		this->event_EXT_EVENT_DEL_BOSS_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_OFFER_BOSS_NTF)){
		log("bofntf");
		this->event_EXT_EVENT_OFFER_BOSS_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_WIN_NTF)){
		log("lwntf");
		this->event_EXT_EVENT_LIST_WIN_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GET_LIST_BOSS_RES)){
		this->event_EXT_EVENT_GET_LIST_BOSS_RES(param);
	}
}

void LayerGameXocDia::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 != cmd->compare(EXT_EVENT_START)
        && 0 != cmd->compare(EXT_EVENT_END)
        && 0 != cmd->compare(EXT_EVENT_GAME_RESULT)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY)
        && 0 != cmd->compare(EXT_EVENT_GAME_INFO_NTF)
        && 0 != cmd->compare(EXT_EVENT_GAME_HIS_NTF)
        && 0 != cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF)
        && 0 != cmd->compare(EXT_EVENT_GAME_STATE_NTF)
        && 0 != cmd->compare(EXT_EVENT_GAME_BET_NTF)
        && 0 != cmd->compare("updateinfome")
        && 0 != cmd->compare(EXT_EVENT_REGISTER_BOSS_RES)
        && 0 != cmd->compare(EXT_EVENT_UNREGISTER_BOSS_RES)
        && 0 != cmd->compare(EXT_EVENT_NEXT_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_DEL_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_OFFER_BOSS_NTF)
        && 0 != cmd->compare(EXT_EVENT_LIST_WIN_NTF)
        && 0 != cmd->compare(EXT_EVENT_GET_LIST_BOSS_RES)){
            return;
        }
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mQueueMsg->addQueue(pObjs);
}

void LayerGameXocDia::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	//log("on user variable update");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	if (ptrEventParams == NULL)
		return;
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	if (ptrEventParamValueUser == NULL)
		return;
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (ptrNotifiedUser == NULL)
		return;

	double money = 0;

	if (SceneManager::getSingleton().getCurrRoomType() == 0) {
		if (ptrNotifiedUser->GetVariable(EXT_VAL_AMOUNT_BALANCE) != NULL) {
			money = *ptrNotifiedUser->GetVariable(EXT_VAL_AMOUNT_BALANCE)->GetDoubleValue();
		}
	}
	else{
		if (ptrNotifiedUser->GetVariable("amfs") != NULL) {
			money = *ptrNotifiedUser->GetVariable("amfs")->GetDoubleValue();
		}
	}

	string uid = *ptrNotifiedUser->Name();
	if (SceneManager::getSingleton().getCurrRoomType() == 0){

		if (0 == uid.compare(this->MyAI)) {

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutDouble("amf", money);
			params->PutUtfString("uid", uid);
			params->PutUtfString("_cmd_", "updateinfome");
			this->mQueueMsg->addQueue(params);
		}
	}
	else{

		if (0 == uid.compare(this->MyAI)) {

			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutDouble("amfs", money);
			params->PutUtfString("uid", uid);
			params->PutUtfString("_cmd_", "updateinfome");
			this->mQueueMsg->addQueue(params);
		}
	}

	if (this->lstBoss.size() > 0){
		double amfBoss = GameUtils::getAmfGoldUser(this->lstBoss[0]);
		if (SceneManager::getSingleton().getCurrRoomType() == 1)
			amfBoss = GameUtils::getAmfSilverUser(this->lstBoss[0]);
		if (amfBoss == 0)
		{
			this->lblMoneyBoss->setString("");
		}
		else{
			this->lblMoneyBoss->setString(mUtils::convertMoneyEx(amfBoss));
		}
	}

	//update for boss
	if (ptrNotifiedUser->Name()->compare(this->bossUserName) == 0)
	{
		this->updateInfoCai(this->bossUserName);
	}

}

void LayerGameXocDia::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	if (room != NULL)
	{
		boost::shared_ptr<RoomVariable> listUserPtr = room->GetVariable("lu");
		if (listUserPtr)
		{
			this->listUserPlay = listUserPtr->GetStringValue()->c_str();
			log("list player: %s", this->listUserPlay.c_str());
			this->updateListUser();
		}
		this->SetRoomConfig();

		//update 9/8
		boost::shared_ptr<RoomVariable> bossVar = room->GetVariable(EXT_FIELD_GAME_BOSS_UID);
		if (bossVar)
		{
			boost::shared_ptr<string> bossPtr = bossVar->GetStringValue();
			// 			if (bossPtr->compare("") != 0)
			// 			{
			if (this->CurrentGameState == false)
				this->updateInfoCai(bossPtr->c_str());
			//}

		}
	}
}

void LayerGameXocDia::updateListUser()
{
	vector<string> listUser = mUtils::splitString(this->listUserPlay, ';');
	if (listUser.size() < 1)
		return;
    this->lblUserCount->setString(StringUtils::format("%lu", listUser.size()));

	for (int i = 0; i < listUser.size(); i++)
	{
		if (listUser[i].compare(this->myName) == 0)
		{
			listUser.erase(listUser.begin() + i);
			break;
		}
	}

	for (int i = 0; i < this->listAvatar.size(); i++)
	{
		this->listAvatar[i]->setVisible(false);
	}
	int maxSize = listUser.size() > 4 ? 4 : listUser.size();
    if (maxSize<=0) return;
    vector<int> avatarChosse;
    while (avatarChosse.size()<maxSize) {
        if (avatarChosse.size() >=maxSize)
            break;
        int randomxy = rand() % maxSize;
        bool isShouldShow = true;
        for (int i = 0; i< avatarChosse.size();i++){
            if (avatarChosse[i] == randomxy){
                isShouldShow = false;
            }
        }
        if (isShouldShow){
            this->listAvatar[avatarChosse.size()]->setVisible(true);
            this->listAvatar[avatarChosse.size()]->setName(listUser[randomxy]);
            string urlAvatar = GameUtils::getAvatarUser(listUser[randomxy]);
            this->listAvatar[avatarChosse.size()]->setAvatarImage(urlAvatar);
            avatarChosse.push_back(randomxy);
        }

	}
}

void LayerGameXocDia::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void LayerGameXocDia::OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	/*boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> uCount = (*ptrEventParams)["uCount"];
	boost::shared_ptr<long> _uCount = ((boost::static_pointer_cast<long>))(uCount);
	std::string strCount = boost::to_string(*_uCount);
	this->txtNumOfPlayers->setString(dataManager.GetSysString(46)+" " + strCount);*/
}

void LayerGameXocDia::SetRoomConfig()
{
	boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lstRoom != NULL){
		boost::shared_ptr<RoomVariable> playtime = lstRoom->GetVariable("playtime");
		if (playtime != NULL){
			this->PlayTurnTime = *playtime->GetIntValue() / 1000;
		}
		boost::shared_ptr<RoomVariable> end = lstRoom->GetVariable("end");
		if (end != NULL){
			this->BetweentwoPlayTime = *end->GetIntValue() / 1000;
		}

		boost::shared_ptr<RoomVariable> activebets = lstRoom->GetVariable("activebets");
		this->ArrListValueBet.clear();
		if (activebets != NULL) {
			std::string betStr = *activebets->GetStringValue();
			//std::string betStr = "1K-5K-10K-50K-100K-500K-1M";
			auto arr = mUtils::splitString(betStr, ',');
			log("list value bet: %s", betStr.c_str());
			for (auto i = 0; i < arr.size(); ++i) {
				this->ArrListValueBet.push_back(arr.at(i));
			}
		}
		else {
			for (auto i = 0; i < this->ArrListValueBetDefault.size(); ++i) {
				this->ArrListValueBet.push_back(this->ArrListValueBetDefault.at(i));
			}
		}

        if (SceneManager::getSingleton().getIsChuyenTab()){
            this->initPlayerInfo();
            this->initListCoin();
            SelectCoin(playerInfo->SoTienThuc);

        }
	}
}

void LayerGameXocDia::DisplayInfoMoney(const long& tienconlai, const long& dadatcuoc)
{
	string m = "0";
	if (tienconlai > 9000000000)
	{
		m = formatMoneySymbol(tienconlai);
	}
	else {
		m = mUtils::convertMoneyEx(tienconlai);
	}
	txtMoney->setString(m);
	string m1 = mUtils::convertMoneyEx(dadatcuoc);
	lblBetMoney->setString(StringUtils::format("%s %s", dataManager.GetSysString(44).c_str(),m1.c_str()));
}
Vec2 LayerGameXocDia::getPointCoin(string type,TaiXiuBet *bet){
    Size s;
    string betType = type;
    if (betType == BET_TYPE_CHAN || betType == BET_TYPE_LE){
        s = Size(130,100);
    }
    if (betType == BET_TYPE_CAP_0_4 || betType == BET_TYPE_CAP_1_3 || betType == BET_TYPE_CAP_3_1 || betType == BET_TYPE_CAP_4_0){
        s = Size(110,110);
    }
    
    Vec2 p = Vec2(bet->GetWorldPos().x-s.width/2,bet->GetWorldPos().y-s.height/2);
    int width = s.width;
    int height = s.height;
    if (betType == BET_TYPE_CHAN || betType == BET_TYPE_LE){
        height = s.height+100;
    }
    int randomX = rand() % width + p.x;
    int randomY = rand() % height + p.y;

    return Vec2(randomX,randomY);
}
void LayerGameXocDia::BetGameByClient(int tag)
{
	if (!CurrentGameState)
	{
		Toast->ShowToast(dataManager.GetSysString(38));
		return;
	}
	if (0 == CurrentBetValue.compare("")){
		Toast->ShowToast(dataManager.GetSysString(39));
		return;
	}

	int pMoney = GetMoneyBet(CurrentBetValue);
	bool r1 = ArrListValueBet.at(0) == BET_VALUE_1K;
	int minVal = r1 ? 1000 : 1;
	if (playerInfo->SoTienAo < minVal || pMoney > playerInfo->SoTienAo)
	{
		Toast->ShowToast(dataManager.GetSysString(40));
		return;
	}
	TaiXiuBet *bet = GetContainerBetByButtonTag(tag);
	if (bet)
	{
		string betType = bet->GetType();
		string src;
		Vec2 _pos = Vec2(WIDTH_DESIGN / 2, 200);
		for (int i = 0; i < arrCoin.size(); i++)
		{
			TaiXiuCoin *pCoin = arrCoin.at(i);
			if (pCoin->GetType() == CurrentBetValue){
				src = pCoin->GetValue().imgnormal;
				_pos = pCoin->GetPosWorld();
				break;
			}
		}
		if (0 != src.compare(""))
		{
//#define  BET_TYPE_CAP_0_4  "BTC04"
//#define  BET_TYPE_CAP_1_3  "BTC13"
//#define  BET_TYPE_CAP_3_1  "BTC31"
//#define  BET_TYPE_CAP_4_0  "BTC40"
            
			//Tao image de animations
			ImageView *imgCoin = ImageView::create(src);
			log("src= %s", src.c_str());
			//imgCoin->setScale(SCALE_COIN_TAI_XIU);
			imgCoin->setScale(0.5);
			imgCoin->setPosition(_pos);

			MoveTo* moveTo = MoveTo::create(DURATION_ANIMATE_COIN_CLICK, this->getPointCoin(betType,bet));
			CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(LayerGameXocDia::AddCoinCallFunc2, this, bet));
			this->addChild(imgCoin,1000);

			imgCoin->runAction(Sequence::create(moveTo, callfun, nullptr));
			playerInfo->PushBet(betType);
			playerInfo->TruTienThuc(pMoney);
			DisplayInfoMoney(playerInfo->SoTienAo, playerInfo->SoTienDatCuocClient);
			bet->MySelect();
			bet->ClientBet(pMoney);
		}
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		//send smartfox sever msg
		boost::shared_ptr<SFSObject> params(new SFSObject());
		params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, CurrentBetValue);
		params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, betType);
		boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lastRoom));
		GameServer::getSingleton().Send(req2);
		if (playerInfo->SoTienAo <= pMoney)
		{
			SelectCoin(playerInfo->SoTienAo);
		}
	}
}

void LayerGameXocDia::AddCoinCallFunc2(Ref *sender, TaiXiuBet* data)
{
	ImageView* img = (ImageView*)sender;
	if (img != NULL){
		data->AddCoin(img);
	}
}

void LayerGameXocDia::event_EXT_EVENT_START()
{
    this->updateListUser();
    this->bgTimer->setVisible(false);
	ResetGame(false);
	this->removeChildByTag(TAG_ANIM_START_GAME);
	this->removeChildByTag(TAG_TOAST);
	Chat* toast = Chat::create(dataManager.GetSysString(50), -1);
	this->addChild(toast, 399);
	CurrentGameState = true;
	UpdateTimeStartGame();
}

void LayerGameXocDia::event_EXT_EVENT_END()
{
	Animating = true;
	CurrentGameState = false;
	for (int i = 0; i < arrBet.size(); i++)
	{
		arrBet.at(i)->HideCurrentBet();
	}
	UpdateTimeEndGame();
	// Xóa layer lịch sử nếu đang hiển thị.
	this->removeChildByTag(TAG_TAI_XIU_HISTORY);
	this->removeChildByTag(TAG_TAI_XIU_TOPWINS);
	boost::shared_ptr<string> rgptr = param->GetUtfString(EXT_FIELD_RESULT_GAME);
	boost::shared_ptr<string> rt10ptr = param->GetUtfString(EXT_FIELD_RESULT_TOP_10);
	if (rgptr == NULL || rt10ptr == NULL)
		return;
	rg = rgptr->c_str();
	rt10 = rt10ptr->c_str();

	//Kiểm tra xem số tiền cược.
	vector<string> arrPlayerBet = playerInfo->GetAllBet();
	if (arrPlayerBet.size() > 0)
	{
		for (int i = 0; i < arrPlayerBet.size(); i++){
			string b = arrPlayerBet.at(i);
			int val = GetMoneyBet(b);
		}
	}

	// Hien thi so tien da cuoc, cho dung voi server
	initPlayerInfo();
	DisplayInfoMoney(playerInfo->SoTienThuc, playerInfo->SoTienDatCuocServer);

	if (0 != rg.compare(""))
	{
		int randomxy = rand() % 160;
		vector<string> arr = mUtils::splitString(rg, '_');
		mDiaNoc->loadTexture("dia_xocdia.png");
		for (int i = 0; i < arr.size(); i++)
		{
			string str;
			if (atoi(arr.at(i).c_str()) == XOC_DIA_CHAN)
			{
				str = "moc3-item-red-result.png";
			}
			else
			{
				str = "moc3-item-yellow-result.png";
			}
			arrResult.at(i)->loadTexture(str);
			arrResult.at(i)->setVisible(true);
			if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
				arrResult.at(i)->setPosition(Vec2(posVi.at(randomxy + i)));
			}
		}
	}

	LayerAnimationXocDia *animate = LayerAnimationXocDia::create(Vec2(mDiaNoc->getPositionX(), mDiaNoc->getPositionY()), 1);
	//animate->setTarget(this);
	this->addChild(animate, 399, TAG_ANIM_END_GAME);

}

void LayerGameXocDia::event_EXT_EVENT_GAME_RESULT()
{
	boost::shared_ptr<long> gbv = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
	boost::shared_ptr<string> gbce = this->param->GetUtfString(EXT_FIELD_GAME_BET_CAN_EAT);

	if (gbv != NULL && gtbv != NULL) {
		int m = (*gbv);
		std::string money = "0";
		if (m > 0){
			if (m > 9000000000)
			{
				money = formatMoneySymbol(m);
			}
			else{
				money = mUtils::convertMoneyEx(m);
			}
			lblWinMoney->setString(StringUtils::format("%s %s",dataManager.GetSysString(45).c_str(), money.c_str()));
		}
		else{
			lblWinMoney->setString(StringUtils::format("%s %s",dataManager.GetSysString(45).c_str(), money.c_str()));
		}

		// Hiển thị số tiền đã đặt cược.
		this->initPlayerInfo();
		this->playerInfo->SoTienDatCuocServer = *gtbv;
		this->playerInfo->SoTienDatCuocClient = *gtbv;
		this->DisplayInfoMoney(this->playerInfo->SoTienThuc, *gtbv);
		int t = this->BetweentwoPlayTime - 3;
		//this->scheduleOnce(schedule_selector(LayerGameXocDia::showAnimation), t);
        Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::showAnimation), this, t, 1, t, false);

	}

	if (gbce != NULL) {
		std::string _gbce = (*gbce);
		log("gbce=%s", _gbce.c_str());
		vector<string> arrays = mUtils::splitString(_gbce, ':');
		for (int i = 0; i < arrays.size(); ++i) {
			for (int j = 0; j < this->arrBet.size(); ++j){
				try {
					TaiXiuBet* pTaiXiu = this->arrBet.at(j);
					pTaiXiu->setIsAnimation(false);
					if (0 == arrays.at(i).compare(pTaiXiu->GetType())){
						pTaiXiu->CanEat();
						//animation tien thang.
						if (pTaiXiu->getIsAnimation()){
							this->processAnimationWin(this->mAvatar->getPosition(), pTaiXiu->GetWorldPos());
						}
					}
				}
				catch (std::out_of_range& e) {
					log("out_of_range %s", e.what());
				}
				catch (std::exception& e) {
					log("exception %s", e.what());
				}
				catch (...) {
				}
			}
		}
	}
}

void LayerGameXocDia::event_EXT_FIELD_GAME_BET_EXCHANGE()
{

}

void LayerGameXocDia::event_EXT_EVENT_GAME_INFO_NTF()
{
	boost::shared_ptr<string> ghtp = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_TOP_PLAYER);
	boost::shared_ptr<string> ghs = this->param->GetUtfString(EXT_FIELD_GAME_HISTORY_RESULT);

	if (ghtp != NULL) {
		this->listTopWinner.clear();
		vector<string> arr = mUtils::splitString(*ghtp, '|');
		for (int i = arr.size() - 1; i >= 0; --i) {
			if (arr.at(i) != "")
				this->listTopWinner.push_back(arr.at(i));
		}
	}

	if (ghs != NULL) {
        //0_0_0_0_;0_1_0_1_;1_1_0_1_;0_0_1_0_;0_1_1_0_;1_1_0_0_;1_1_1_1_;0_1_1_0_;0_1_1_1_;1_0_0_0_;
        auto lst = mUtils::splitString(*ghs, ';');
        int count = 0;
        int item = 0;
        if (lst.size()>5){
            item = lst.size() - 5;
        }
        else{
            item = 0;
        }
        for (int i = lst.size()-1;i >= item;i--){
            auto it = mUtils::splitString(lst[i], '_');
            for (int j = 0; j<it.size(); j++) {
                string str;
                if (atoi(it[j].c_str()) == XOC_DIA_CHAN)
                {
                    str = "moc3-item-red-result.png";
                }
                else
                {
                    str = "moc3-item-yellow-result.png";
                }
                this->lstImgHis[(this->lstImgHis.size()-1)-(j+4*count)]->setVisible(true);
                this->lstImgHis[(this->lstImgHis.size()-1)-(j+4*count)]->loadTexture(str);
            }
            count++;
        }
	}
}

void LayerGameXocDia::event_EXT_EVENT_GAME_HIS_NTF()
{

}

void LayerGameXocDia::event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF()
{

}

void LayerGameXocDia::event_EXT_EVENT_GAME_STATE_NTF()
{
    this->isJoin = true;
	log("event_EXT_EVENT_GAME_STATE_NTF");
	boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
	boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
	boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<string> gbex = this->param->GetUtfString(EXT_FIELD_GAME_BET_EXCHANGE);
    boost::shared_ptr<string> rgptr = param->GetUtfString(EXT_FIELD_RESULT_GAME);

	//update 8/9
	boost::shared_ptr<string> bossPtr = this->param->GetUtfString(EXT_FIELD_GAME_BOSS_UID);
	SceneManager::getSingleton().hideLoading();

    SelectCoin(playerInfo->SoTienThuc);

	if (bossPtr != NULL)
	{
		if (bossPtr->compare("") != 0)
		{
			this->updateInfoCai(bossPtr->c_str());
		}
	}
	// Thoi gian con lai cua van choi
	if (gtime != NULL) {
		this->TimerCountDown = *gtime;
		this->txtTimer->setString(StringUtils::format("%d", this->TimerCountDown));
	}

	if (gbt != NULL) {
		log("gbt = %s ", gbt->c_str());
		this->ReloadGameWhenReturn(*gbt);
	}

	if (gbex != NULL) {
		log("gbex = %s ", gbex->c_str());
		//this->LoadGameExchange(*gbex);
	}

	if (gstate != NULL) {
		log("gstate = %ld ", *gstate);
		if (0 == *gstate) {
			this->CurrentGameState = false;
            Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updateTimer),this,1, false);
            if(rgptr == NULL) return;
            rg = rgptr->c_str();
            if (0 != rg.compare(""))
            {
                int randomxy = rand() % 160;
                vector<string> arr = mUtils::splitString(rg, '_');
                mDiaNoc->loadTexture("dia_xocdia.png");
                for (int i = 0; i < arr.size(); i++)
                {
                    string str;
                    if (atoi(arr.at(i).c_str()) == XOC_DIA_CHAN)
                    {
                        str = "moc3-item-red-result.png";
                    }
                    else
                    {
                        str = "moc3-item-yellow-result.png";
                    }
                    arrResult.at(i)->loadTexture(str);
                    arrResult.at(i)->setVisible(true);
                    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2) {
                        arrResult.at(i)->setPosition(Vec2(posVi.at(randomxy + i)));
                    }
                }
            }
		}
		else {
			this->CurrentGameState = true;
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updateTimer),this,1, false);
		}
	}
    SceneManager::getSingleton().hideLoading();
}

void LayerGameXocDia::event_EXT_EVENT_GAME_BET_NTF()
{
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> gbv = this->param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
	boost::shared_ptr<long> gtbvc = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL);
	boost::shared_ptr<long> ttp = this->param->GetInt(EXT_FIELD_GAME_TOTAL_PLAYER);
	if (uid != NULL && gbv != NULL && gbt != NULL && gtbv != NULL && gtbvc != NULL) {
		// gbv: game bet value, gia tri vua bet.
		// gtbv : tong gia tri ma nguoi choi (IsMe) bet
		// gtbvc: tong gia tri o bet
		// Luu het vao queue, 5s moi cap nhat mot lan
		//log("uid=%s gbv=%s", (*uid).c_str(), (*gbv).c_str());
		std::string _gbt = (*gbt);
		long playermebet = 0 == uid->compare(this->MyAI) ? this->GetMoneyBet(*gbv) : 0;
		//this->mQueueTaiXiu->Push(_gbt, *gtbvc, playermebet);

        
        TaiXiuBet* bet = this->GetContainerBetByBetType(_gbt);

        if (bet != NULL){
            
            if (0 == uid->compare(this->MyAI)) {
                this->playerInfo->SoTienDatCuocServer = (float)(*gtbv);
                // Kiểm tra bet.
                bool chk = this->playerInfo->CheckBet(*gbt);
                if (!chk) {
                    
                }
                else {
                    
                }
            }else{
                long totalBet = bet->GetTotalBet();
                long delta = *gtbvc - totalBet;
                
                std::string coin = this->SelectCoinValue(delta, 1);
                
                //Lay src cua coin
                std::string src = "";
                for (int j = 0; j < this->arrCoin.size(); ++j) {
                    TaiXiuCoin* pCoin = this->arrCoin.at(j);
                    if (pCoin->GetType() == coin) {
                        src = pCoin->GetValue().imgnormal;
                        break;
                    }
                }
                if (0 != src.compare(""))
                {
                    //#define  BET_TYPE_CAP_0_4  "BTC04"
                    //#define  BET_TYPE_CAP_1_3  "BTC13"
                    //#define  BET_TYPE_CAP_3_1  "BTC31"
                    //#define  BET_TYPE_CAP_4_0  "BTC40"
                    
                    //Tao image de animations
                    
                    Vec2 beginPos;
                    vector<string> listUser = mUtils::splitString(this->listUserPlay, ';');
                    int max = listUser.size()-1;
                    int random = 0;
                    if (listUser.size()>=5){
                        random = rand()%4;
                    }else if (listUser.size() > 1 && listUser.size() < 4){
                        random = rand()% max;
                    }
                    if (max ==0){
                        random = 0;
                    }
                    auto visibleSize = Director::getInstance()->getVisibleSize();
                    if (random == 4){
                        beginPos = Vec2(WIDTH_DESIGN -27, visibleSize.height-280);
                    }else{
                        if (random < this->listAvatar.size())
                            beginPos = this->listAvatar[random]->getPosition();
                    }
                    
                    ImageView *imgCoin = ImageView::create(src);
                   // log("src= %s", src.c_str());
                    //imgCoin->setScale(SCALE_COIN_TAI_XIU);
                    imgCoin->setScale(0.5);
                    
                    imgCoin->setPosition(beginPos);
                    
                    MoveTo* moveTo = MoveTo::create(DURATION_ANIMATE_COIN_CLICK, this->getPointCoin(_gbt,bet));
                    CallFuncN* callfun = CallFuncN::create(CC_CALLBACK_1(LayerGameXocDia::AddCoinCallFunc2, this, bet));
                    this->addChild(imgCoin,1000);
                    
                    imgCoin->runAction(Sequence::create(moveTo, callfun, nullptr));
                }
                bet->SetTotalBet(*gtbvc);
            }
            
           
        }

        
		//Xác nhận lại việc đã đặt cược
		
	}
}

void LayerGameXocDia::event_EXT_EVENT_GAME_BET_ERROR_NOTIFY()
{
	boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
	if (errc != NULL) {
		log("bet errc = %ld", *errc);
		if (*errc == RESCODE_NOT_ENOUGH_MONEY) {
			this->Toast->ShowToast(dataManager.GetSysString(40));
		}
		else if (*errc == RESCODE_GAME_IS_ENDED)
		{
			Toast->ShowToast(dataManager.GetSysString(4));
		}
	}
}

void LayerGameXocDia::ResetGame(bool state)
{
	for (int i = 0; i < this->arrBet.size(); ++i){
		TaiXiuBet* pTaiXiu = this->arrBet.at(i);
		pTaiXiu->Reset();
	}
    //this->stop
	if (this->getChildByTag(TAG_TAI_XIU_XO_BA0) != NULL) {
		this->removeChildByTag(TAG_TAI_XIU_XO_BA0);
	}
	if (this->getChildByTag(TAG_ANIM_END_GAME) != NULL) {
		this->removeChildByTag(TAG_ANIM_END_GAME);
	}
	for (int i = 0; i < arrResult.size(); i++)
	{
		arrResult.at(i)->setVisible(false);
	}
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::showAnimation),this);

	mDiaNoc->loadTexture("dia-1.png");

	this->Animating = state;
	this->playerInfo->reset();
	this->mQueueTaiXiu->RemoveAll();
    //this->mQueueMsg->addQueue(<#boost::shared_ptr<ISFSObject> ptrEvent#>)

	this->initPlayerInfo();
	this->lblBetMoney->setString("0");
	this->lblWinMoney->setString("0");
	this->initPlayerInfo();
	this->lblBetMoney->setString(dataManager.GetSysString(44) + " 0");
	this->lblWinMoney->setString(dataManager.GetSysString(45) + " 0");

	this->DisplayInfoMoney(this->playerInfo->SoTienThuc, this->playerInfo->SoTienDatCuocServer);

//    if (this->CurrentBetValue == "" && this->playerInfo->SoTienThuc >= 1000) {
//        this->SelectCoin(this->playerInfo->SoTienThuc);
//    }

	this->rg = "";
	this->rt10 = "";
	//update 12/9
	this->mAvatar->showAnimationWin(false);
	for (int i = 0; i < this->listAvatar.size(); i++) {
		this->listAvatar[i]->showAnimationWin(false);
	}

}

void LayerGameXocDia::UpdateTimeStartGame()
{
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateToNextTurn),this);
	// Dem timer thoi gian van choi
	this->txtTimer->setVisible(true);
	this->TimerCountDown = this->PlayTurnTime;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updateTimer),this,1, false);
	this->bgTimer->setVisible(true);
	// Tat dem nguoc den van tiep theo
	// 	this->lblCountNextTime->setVisible(false);
	// 	string str = "Ván tiếp: " + boost::to_string(this->BetweentwoPlayTime);
	// 	this->lblCountNextTime->setString(str);
	// 	this->TimerCountNextTime = this->BetweentwoPlayTime;
}

void LayerGameXocDia::UpdateTimeEndGame()
{
	this->CurrentGameState = false;
	//Dung dem timer
	this->txtTimer->setVisible(false);
	bgTimer->setVisible(false);
	string str = boost::to_string(this->PlayTurnTime);
	this->txtTimer->setString(str);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateTimer),this);

	// Dem nguoc thoi gian den van tiep theo.
	//this->lblCountNextTime->setVisible(false);
	this->TimerCountNextTime = this->BetweentwoPlayTime;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::updateToNextTurn),this,1, false);
}

void LayerGameXocDia::ReloadGameWhenReturn(string& gbt)
{
	log("gbt = %s", gbt.c_str());

	vector<string> arrGbt = mUtils::splitString(gbt, ';');
	vector<string> info;

	long yourBet = 0;
	for (int i = 0; i < arrGbt.size(); ++i) {
		if (0 == arrGbt.at(i).compare("")) continue;
		try {
			info = mUtils::splitString(arrGbt.at(i), ':');

			long m = atol(info.at(2).c_str());

			TaiXiuBet* pTaiXiu = this->GetContainerBetByBetType(info.at(0));
			if (pTaiXiu == NULL) continue;

			if (m > 0){
				yourBet += m;
				pTaiXiu->MySelect();
			}

			pTaiXiu->SetTotalBet(atol(info.at(1).c_str()));
			pTaiXiu->SetPlayerBet(atol(info.at(2).c_str()));
			pTaiXiu->ExecuteRefresh();

		}
		catch (std::out_of_range& e) {
			log("ReloadGameWhenReturn out_of_range %s", e.what());
		}
		catch (std::exception& e) {
			log("ReloadGameWhenReturn exception %s", e.what());
		}
		catch (...) {

		}
	}

	if (yourBet > 0) {
		this->playerInfo->SoTienDatCuocServer = yourBet;
		this->playerInfo->SoTienDatCuocClient = yourBet;
		this->DisplayInfoMoney(this->playerInfo->SoTienThuc, yourBet);
	}

	arrGbt.clear();
}

void LayerGameXocDia::updateTimer(float dt)
{
	if (this->TimerCountDown > 0) {
		this->TimerCountDown--;

		string str = boost::to_string(this->TimerCountDown);
		this->txtTimer->setString(str);
		txtTimer->setVisible(true);
		bgTimer->setVisible(true);

		if (this->TimerCountDown > 5) {
			if (0 == this->TimerCountDown % 5) {
				this->Update5sStep(0);
			}
		}
		else if (this->TimerCountDown < 5 && this->TimerCountDown > 1) {
			if (0 == this->TimerCountDown % 2) {
				this->Update5sStep(0);
			}
		}
		else {
			this->Update5sStep(0);
		}
	}
	else {
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateTimer),this);
		this->TimerCountDown = this->PlayTurnTime;
	}
}

void LayerGameXocDia::updateToNextTurn(float dt)
{
	if (this->TimerCountNextTime > 0){
		this->TimerCountNextTime--;
	}
	else
	{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateToNextTurn),this);
		//this->lblCountNextTime->setVisible(false);
		this->TimerCountNextTime = this->BetweentwoPlayTime;
	}
	//string str = "Ván tiếp: " + boost::to_string(this->TimerCountNextTime);
	//this->lblCountNextTime->setString(str);
	//this->lblCountNextTime->setVisible(false);
}
void LayerGameXocDia::Update5sStep(float dt){
    return;
	this->Animating = true;
	try {
		vector<BetInfo*> mArrayInfo = this->mQueueTaiXiu->GetArrayBetInfo();
		for (int i = 0; i < mArrayInfo.size(); ++i){
			BetInfo* info = mArrayInfo.at(i);
			if (info == NULL)
				continue;

			log("type = %s, total = %ld, my bet = %ld", info->GetType().c_str(), info->GetTotalBet(), info->GetPlayerBet());

			std::string bettype = info->GetType();
			TaiXiuBet* bet = this->GetContainerBetByBetType(bettype);
			if (bet != NULL){
				long totalBet = bet->GetTotalBet();
				long delta = info->GetTotalBet() - totalBet;

				std::string coin = this->SelectCoinValue(delta, 1);

				//Lay src cua coin
				std::string src = "";
				for (int j = 0; j < this->arrCoin.size(); ++j) {
					TaiXiuCoin* pCoin = this->arrCoin.at(j);
					if (pCoin->GetType() == coin) {
						src = pCoin->GetValue().imgnormal;
						break;
					}
				}

				Vec2 p = bet->GetWorldPos();
				ImageView* imgCoin = ImageView::create();
				imgCoin->loadTexture(src);
				//imgCoin->setScale(SCALE_COIN_TAI_XIU);
				imgCoin->setScale(0.5);
				imgCoin->setPosition(Vec2(WIDTH_DESIGN / 2, 100));

                MoveTo* moveTo = MoveTo::create(DURATION_ANIMATE_COIN, this->getPointCoin(bettype,bet));

				Struct4Call1 __d;
				__d.bet = bet;
				__d.total = info->GetTotalBet();

				CallFuncN * callfun = CallFuncN::create(CC_CALLBACK_1(LayerGameXocDia::AddCoinCallFunc, this, __d));

				this->addChild(imgCoin,1000);
				imgCoin->runAction(Sequence::create(moveTo, callfun, NULL));
			}
		}
		this->mQueueTaiXiu->RemoveAll();

	}
	catch (...) {
		this->Animating = false;
	}
	this->Animating = false;
}

void LayerGameXocDia::AddCoinCallFunc(Ref *sender, Struct4Call1 data)
{
	TaiXiuBet* bet = data.bet;
	long money = data.total;

	if (bet != NULL) {
		long playerMoney = bet->GetPlayerBet();
		if (playerMoney > money) {
			money = playerMoney;
		}
		bet->SetTotalBet(money);
		ImageView* img = (ImageView*)sender;
		if (img != NULL){
			bet->AddCoin(img);
		}
	}
}

void LayerGameXocDia::updatepress(float delta)
{
	this->press += delta;
	if (this->press >= this->longpresstime && this->CurrentTag != -1){

		if (this->CurrentTag != this->OldPressTag){
			this->OldPressTag = this->CurrentTag;
			TaiXiuBet* bet = this->GetContainerBetByButtonTag(this->OldPressTag);
			if (bet != NULL){
				bet->DisplayCurrentBet();
			}
		}
	}
}

void LayerGameXocDia::HideAllSuggestion()
{
	for (int i = 0; i < this->arrBet.size(); ++i){
		this->arrBet.at(i)->HideCurrentBet();
	}
	this->CurrentTag = -1;
	this->OldPressTag = -1;
}
void LayerGameXocDia::showAnimation(float dt){
	ResetGame(false);
	LayerAnimationXocDia *animate = LayerAnimationXocDia::create(Vec2(mDiaNoc->getPositionX(), mDiaNoc->getPositionY()), 2);
	//animate->setTarget(this);
	Chat* toast = Chat::create(dataManager.GetSysString(51), -1);
	this->addChild(toast, 399, TAG_TOAST);
	this->addChild(animate, 399, TAG_ANIM_START_GAME);
}

void LayerGameXocDia::processButtonExitTouched(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		this->layerHistory->setVisible(false);
	}
}

void LayerGameXocDia::processButtonHistoryTouched(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		this->layerHistory->setVisible(true);
	}
}

void LayerGameXocDia::processButtonCupCaiTouched(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<SFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_REGISTER_BOSS_REG, params, lastRoom));
		GameServer::getSingleton().Send(req2);
		this->removeChildByTag(92);
	}

}

void LayerGameXocDia::processButtonHuyCaiTouched(Ref *sender, ui::Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL)
			return;
		boost::shared_ptr<SFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_UNREGISTER_BOSS_REG, params, lastRoom));
		GameServer::getSingleton().Send(req2);
	}
}

void LayerGameXocDia::processButtonDanhSachCaiTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		// 		if (this->lstBoss.size() > 0){
		// 			this->dsBoss = "";
		// 			for (int i = 0; i < this->lstBoss.size() - 1; ++i){
		// 				this->dsBoss += GameUtils::getNamePlayer(this->lstBoss[i]) + ",";
		// 			}
		// 			this->dsBoss += GameUtils::getNamePlayer(this->lstBoss[this->lstBoss.size() - 1]);
		// 		}
		// 		LayerPopupDanhSachCai *popupDanhSach = LayerPopupDanhSachCai::create(this->dsBoss);
		// 		this->addChild(popupDanhSach, 10000);
		LayerPopupListCai* popupCai = LayerPopupListCai::create();
		if (this->canBeBoss == false)
			popupCai->disableButtonRegister();
		popupCai->setLocalZOrder(400);
		this->addChild(popupCai);
	}
}

void LayerGameXocDia::processAnimationWin(Vec2 _dest, Vec2 _pos)
{
	vector<Sprite*> coinsArr;
	string pathImg = "moc3-icon-hao.png";
	if (SceneManager::getSingleton().getCurrRoomType() == 0)//gold
	{
		pathImg = "moc3-icon-moc.png";
	}
	for (int i = 0; i < 12; i++)
	{
		Sprite* coins = Sprite::create(pathImg);
		coins->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		coins->setPosition(_pos);
		coins->setLocalZOrder(244);
		this->addChild(coins);
		coinsArr.push_back(coins);
	}
	CallFuncN* callBack = CallFuncN::create([&](Node* pSender){
		pSender->removeFromParentAndCleanup(true);
	});
	for (unsigned int i = 0; i < coinsArr.size(); i++)
	{
		float _delay = 0.01f + 0.03f*i;
		MoveTo * moveTo = MoveTo::create(0.2 + _delay, _dest);
		ScaleTo* scaleTo = ScaleTo::create(0.2 + _delay, 1);
		DelayTime* delayAction = DelayTime::create(_delay);
		Spawn* spawnAction = Spawn::create(moveTo, scaleTo, NULL);
		coinsArr.at(i)->runAction(Sequence::create(delayAction, spawnAction, callBack, NULL));
	}
}

void LayerGameXocDia::event_EXT_EVENT_REGISTER_BOSS_RES()
{
	boost::shared_ptr<long> rc = this->param->GetInt("errc");
	boost::shared_ptr<string> rd = this->param->GetUtfString("eexp");
	std::string eexp;
	int errc;
	if (rc != NULL){
		errc = *rc;
	}
	if (rd != NULL)
		eexp = *(rd);
	log("rc=%d, rd=%s", errc, eexp.c_str());
	SceneManager::getSingleton().showToast(eexp);

}

void LayerGameXocDia::event_EXT_EVENT_UNREGISTER_BOSS_RES()
{
	boost::shared_ptr<long> rc = this->param->GetInt("errc");
	boost::shared_ptr<string> rd = this->param->GetUtfString("eexp");
	std::string eexp;
	int errc;
	if (rc != NULL){
		errc = *(rc);
	}
	if (rd != NULL)
		eexp = *(rd);
	log("rc=%d, rd=%s", errc, eexp.c_str());
	SceneManager::getSingleton().showToast(eexp);
}

void LayerGameXocDia::event_EXT_EVENT_NEXT_BOSS_NTF()
{
	/*
		thong bao luot boss tiep theo
		- cap nhat trang thai neu luot tiep theo la chinh minh
		- cap nhat giao dien boss
		*/
	// 	std::string infoNextCai = "";
	// 	boost::shared_ptr<string> uid = this->param->GetUtfString("uid");
	// 	if (uid != NULL){
	// 		infoNextCai = *uid;
	// 	}
	// 	log("nextCai= %s", infoNextCai.c_str());
	// 	std::string _myName = GameUtils::getMyName();
	// 	if (_myName.compare(infoNextCai) == 0){
	// 		this->status = 1;
	// 	}
	// 	else{
	// 		this->status = 0;
	// 	}
	// 	SceneManager::getSingleton().showToast("Lượt cái tiếp theo là " + GameUtils::getNamePlayer(infoNextCai));
	// 	this->bossName->setString(GameUtils::getNamePlayer(infoNextCai));
	// 	this->bossName->setSystemFontSize(30);
	// 	this->bossName->setPositionY(this->btnCuopCai->getContentSize().height - 15.0);
	// 
	// 	/*double amfBoss = GameUtils::getAmfGoldUser(infoNextCai);
	// 	if (SceneManager::getSingleton().getCurrRoomType() == 1)
	// 	amfBoss = GameUtils::getAmfSilverUser(infoNextCai);
	// 	if (amfBoss == 0)
	// 	{
	// 	this->lblMoneyBoss->setString("");
	// 	}
	// 	else{
	// 	this->lblMoneyBoss->setString(mUtils::convertMoneyEx(amfBoss));
	// 	}*/
	// 	this->lblMoneyBoss->setVisible(true);
	// 
	// 	string url = GameUtils::getAvatarUser(infoNextCai);
	// 	this->avatarBoss->setAvatarImage(url);
	// 
	// 	this->btnCuopCai->setEnabled(false);
	// 	this->btnCuopCai->setEnabled(false);

	//update 8/9
	boost::shared_ptr<string> infoNextCaiPtr = this->param->GetUtfString("uid");
	if (infoNextCaiPtr != NULL)
	{

		this->bossNext = infoNextCaiPtr->c_str();
		if (this->CurrentGameState == false)
		{
			double amfBoss = GameUtils::getAmfGoldUser(infoNextCaiPtr->c_str());
			if (SceneManager::getSingleton().getCurrRoomType() == 1)
				amfBoss = GameUtils::getAmfSilverUser(infoNextCaiPtr->c_str());
			this->lblMoneyBoss->setString(mUtils::convertMoneyEx(amfBoss));
			string urlAvatarCai = GameUtils::getAvatarUser(infoNextCaiPtr->c_str());
			this->updateInfoCai(infoNextCaiPtr->c_str());
		}
		SceneManager::getSingleton().showToast(StringUtils::format("%s %s",dataManager.GetSysString(41).c_str(), GameUtils::getNamePlayer(infoNextCaiPtr->c_str()).c_str()));
	}
}

void LayerGameXocDia::event_EXT_EVENT_DEL_BOSS_NTF()
{
	/*xoa boss cap nhat lai
		Hien thi popup thong bao,
		giao dien hien thi boss,
		trang thai neu la chinh minh bi huy boss: status
		danh sach boss: lstBoss
		cap nhat danh sach nguoi choi (truong hop nguoi choi thoat game)
		*/
	std::string uid = "";
	boost::shared_ptr<string> _uid = this->param->GetUtfString("uid");
	if (_uid != NULL){
		uid = *_uid;
	}
	log("uid= %s", uid.c_str());
	LayerPopupThongBao *popupDellBoss = LayerPopupThongBao::create();
	popupDellBoss->showAButton();
	popupDellBoss->setContentMess(dataManager.GetSysString(52)+ " " + GameUtils::getNamePlayer(uid) + dataManager.GetSysString(53));
	if (!this->getChildByTag(911)){
		this->addChild(popupDellBoss);
		popupDellBoss->setTag(911);
	}

	std::string _myName = GameUtils::getMyName();
	// 	this->bossName->setString(dataManager.GetSysString(15) );
	// 	this->bossName->setSystemFontSize(35);
	// 	this->bossName->setPosition(Point(this->btnCuopCai->getContentSize().width / 2,
	// 		this->btnCuopCai->getContentSize().height / 2 + 10.0));

	//this->lblMoneyBoss->setVisible(false);
	//this->avatarBoss->setAvatarImage("");

	//cap nhat lai trang thai cua chinh minh neu minh bi huy boss
	if (_myName.compare(uid) == 0){
		this->status = 0;
	}
	else{
		this->status = 1;
	}
	//cap nhat lai listBoss
	vector<std::string> _lstTg;
	_lstTg.clear();
	for (int i = 0; i < this->lstBoss.size(); ++i){
		std::string _bossName = this->lstBoss[i];
		if (_bossName.compare(uid) != 0){
			_lstTg.push_back(_bossName);
		}
	}
	this->lstBoss.clear();
	for (int j = 0; j < _lstTg.size(); ++j){
		this->lstBoss.push_back(_lstTg[j]);
	}

	//update 8/9
	boost::shared_ptr<string> uidPtr = this->param->GetUtfString(EXT_FIELD_USERID);
	if (uidPtr != NULL)
	{
		if (0 == this->bossUserName.compare(uidPtr->c_str()))
		{
			this->updateInfoCai(dataManager.GetSysString(15));
		}
		if (0 == this->myName.compare(uidPtr->c_str()))
			SceneManager::getSingleton().showToast(dataManager.GetSysString(3));
		if (this->bossNext.compare(uidPtr->c_str()) == 0)
			this->bossNext = "";
	}
}

void LayerGameXocDia::event_EXT_EVENT_OFFER_BOSS_NTF()
{
	if (!this->getChildByTag(9)){
		LayerPopupActiveCai* popupActiveCai = LayerPopupActiveCai::create();
		this->addChild(popupActiveCai, 10000);
		popupActiveCai->setTag(9);
		//auto btnRegisterBoss = this->popupActiveCai->getButtonOk();
		//btnRegisterBoss->addTouchEventListener(CC_CALLBACK_2(LayerGameXocDia::processButtonCupCaiTouched, this));
		this->btnCuopCai->setEnabled(true);
		this->btnCuopCai->setBright(true);
		this->canBeBoss = true;
	}
}

void LayerGameXocDia::event_EXT_EVENT_LIST_WIN_NTF()
{
	boost::shared_ptr<string> listWinPtr = this->param->GetUtfString("wl");
	if (listWinPtr != NULL) {
		vector<string> listWin = mUtils::splitString(*listWinPtr, ',');
		for (int i = 0; i < listWin.size(); i++) {
			for (int j = 0; j < this->listAvatar.size(); j++) {
				if (this->listAvatar[j]->getName().compare(listWin[i]) == 0 && listWin[i].compare("") != 0) {
					this->listAvatar[j]->showAnimationWin(true);
				}
			}

		}

		for (int i = 0; i < listWin.size(); i++) {
			if (listWin[i].compare(this->myName) == 0) {
				this->mAvatar->showAnimationWin(true);
				break;
			}
		}

	}
}

void LayerGameXocDia::processAvatarClicked(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		Node* _node = (Node*)pObject;
		int _tag = _node->getTag();
		std::string _userName = "";
		if (_tag < this->lstPlayer.size())
			_userName = this->lstPlayer[_tag];
		log("avatar touched: %d - name %s", _tag, _userName.c_str());
		string userAvatar = GameUtils::getAvatarUser(_userName);
		string userIp = GameUtils::getUserIP(_userName);
		auto layerInfo = LayerPopupInfo::create();
		layerInfo->setFriendID(_userName);
		layerInfo->setUserIP(userIp);
		layerInfo->setAvatarUrl(userAvatar);
		layerInfo->setIsBossRoom(false);//bo nut chu phong
		layerInfo->reloadAllDatas();
		layerInfo->setIsPlaying(GameUtils::isStartedGame());
		this->addChild(layerInfo, 10000);
	}

}

void LayerGameXocDia::event_EXT_EVENT_GET_LIST_BOSS_RES(boost::shared_ptr<ISFSObject> param)
{
	// 	log("get list boss");
	// 	this->dsBoss = "";
	// 	std::string contentMess = "";
	// 	boost::shared_ptr<string> _dsBoss = param->GetUtfString("bl");
	// 	if (_dsBoss != NULL){
	// 		this->dsBoss = *_dsBoss;
	// 	}
	// 	//lstDkCai = "tester01,tester02, tester03,tester04,tester05";
	// 	LayerPopupThongBao *popupThongbao = LayerPopupThongBao::create();
	// 	popupThongbao->showAButton();
	// 	if (dsBoss.compare(" ") != 0){
	// 		this->lstBoss = mUtils::splitString(dsBoss, ',');
	// 		log("lst= %s, size: %d", this->dsBoss.c_str(), this->lstBoss.size());
	// 		contentMess = "Người chơi " + lstBoss[0] + " đang cầm cái.";
	// 	}
	// 	else{
	// 		contentMess = "Hệ thống đang cầm cái.";
	// 	}
	// 	popupThongbao->setContentMess(contentMess);
	// 	if (!this->getChildByTag(11)){
	// 		this->addChild(popupThongbao, 10000);
	// 		popupThongbao->setTag(11);
	// 	}
	// 	//set giao dien cam cai 
	// 	if (this->lstBoss.size() > 0){
	// 		this->bossName->setString(GameUtils::getNamePlayer(this->lstBoss[0]));
	// 		this->bossName->setSystemFontSize(30);
	// 		this->bossName->setPositionY(this->btnCuopCai->getContentSize().height - 15.0);
	// 
	// 		this->lblMoneyBoss->setVisible(true);
	// 		/*double amfBoss = GameUtils::getAmfGoldUser(this->lstBoss[0]);
	// 		if (SceneManager::getSingleton().getCurrRoomType() == 1)
	// 		amfBoss = GameUtils::getAmfSilverUser(this->lstBoss[0]);
	// 		if (amfBoss == 0)
	// 		{
	// 		this->lblMoneyBoss->setString("");
	// 		}
	// 		else{
	// 		this->lblMoneyBoss->setString(mUtils::convertMoneyEx(amfBoss));
	// 		}*/
	// 
	// 		string url = GameUtils::getAvatarUser(this->lstBoss[0]);
	// 		this->avatarBoss->setAvatarImage(url);
	// 	}

}

void LayerGameXocDia::onEnter()
{
	Layer::onEnter();

	//request list lay thong tin user dang cam cai
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL){
		boost::shared_ptr<SFSObject> params(new SFSObject());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest("glbrq", params, lastRoom));
		GameServer::getSingleton().Send(req2);
	}
	//this->scheduleOnce(schedule_selector(LayerGameXocDia::scheduleLoadAvatar), 3);
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameXocDia::scheduleLoadAvatar), this, 3, 1, 3, false);

}

void LayerGameXocDia::onExit()
{
	Layer::onExit();
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateTimer),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateToNextTurn),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::scheduleLoadAvatar),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updatepress),this);
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameXocDia::updateEvent),this);

}

void LayerGameXocDia::updateInfoCai(string _bossName)
{
	this->lblMoneyBoss->setVisible(true);
	this->bossName->setVisible(true);
	this->bossName->setString(dataManager.GetSysString(43));
	this->bossUserName = _bossName;
	string myName = GameUtils::getMyName();
	double amfBoss = GameUtils::getAmfGoldUser(this->bossUserName);
	if (SceneManager::getSingleton().getCurrRoomType() == 1)
		amfBoss = GameUtils::getAmfSilverUser(this->bossUserName);

	if (amfBoss == 0)
	{
		this->lblMoneyBoss->setString(dataManager.GetSysString(15) );
	}
	else{
		this->lblMoneyBoss->setString(mUtils::convertMoneyEx(amfBoss));
	}
	string urlAvatarCai = GameUtils::getAvatarUser(this->bossUserName);
	this->avatarBoss->setAvatarImage(urlAvatarCai);

	if (myName.compare(this->bossUserName) == 0)
	{
		this->status = 1;
		this->canBeBoss = true;
	}
	else
	{
		this->status = 0;
		this->canBeBoss = false;
	}
}

void LayerGameXocDia::initAllListAvatar()
{
	vector<Vec2> listPos = { Vec2(100,this->sizeAdd.height+ 530), Vec2(280,this->sizeAdd.height +400),Vec2(WIDTH_DESIGN-280,this->sizeAdd.height+400),Vec2(WIDTH_DESIGN-100,this->sizeAdd.height+ 530) };
	for (int i = 0; i < 4; i++)
	{
		CircleAvatar* avatar = CircleAvatar::create();
		avatar->setName("");
		avatar->setPosition(listPos[i]);
		avatar->setVisible(true);
		this->addChild(avatar,10000);
		this->listAvatar.push_back(avatar);
		avatar->setClickCallBackWithParam(CC_CALLBACK_2(LayerGameXocDia::avatarUserCallBack, this));
	}
}

void LayerGameXocDia::avatarUserCallBack(CircleAvatar* sender, string callback)
{
	string userAvatar = GameUtils::getAvatarUser(callback);
	string userIp = GameUtils::getUserIP(callback);
	auto layerInfo = LayerPopupInfo::create();
	layerInfo->setFriendID(callback);
	layerInfo->setUserIP(userIp);
	layerInfo->setAvatarUrl(userAvatar);
	layerInfo->setIsBossRoom(false);//bo nut chu phong
	layerInfo->reloadAllDatas();
	layerInfo->setIsPlaying(GameUtils::isStartedGame());
	this->addChild(layerInfo,10000);
}

void LayerGameXocDia::scheduleLoadAvatar(float dt)
{
	//reset avatar me
	string urlAvatar = GameUtils::getAvatarUser();
	this->mAvatar->setIcon(urlAvatar);
}
