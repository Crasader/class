#include "LayerGameTaiXiu.h"
#include "LayerTaiXiuHistory.h"
#include "LayerTaiXiuPopupWinners.h"
#include "TaiXiuMessageDefs.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#include "../Common.h"
#include "_Button_inGame_.h"
#include "ChanUtils.h"
#include "LayerAnimationTaiXiu.h"
#include "PhomMessDef.h"

#include "../layers/LayerPopupRecharge.h"
#include "../layers/LayerGuideInGame.h"

#define TAG_TAI_XIU_HISTORY 500
#define TAG_TAI_XIU_TOPWINS 501
#define DURATION_ANIMATE_COIN 0.4
#define DURATION_ANIMATE_COIN_CLICK 0.1
#define TAG_TAI_XIU_XO_BA0 502
#define TAG_ANIM_END_GAME 503
#define SCALE_COIN_TAI_XIU 0.5

LayerGameTaiXiu::LayerGameTaiXiu()
:Toast(NULL),
resultDice1(NULL),
resultDice2(NULL),
resultDice3(NULL),
txtNumOfPlayers(NULL),
lblBetMoney(NULL),
lblWinMoney(NULL),
lblMoney(NULL),
txtTimer(NULL),
lblCountNextTime(NULL),
CurrentGameState(false),
CurrentBetValue(""),
TimerCountDown(45),
TimerCountNextTime(15),

BetweentwoPlayTime(15),
PlayTurnTime(45),

HayDatCuoc(NULL),
CountSizeDatCuoc(0),

rootNode(NULL),
CountZoder(1001),
CurrentTag(-1),
longpresstime(0.5),
Animating(false),
mQueueMsg(NULL),
playerInfo(NULL),
mQueueTaiXiu(NULL),
posFrameAnim(Vec2(0, 0)),
signalNetwork(NULL)
{

	this->rg = "";
	this->rt10 = "";

	this->arrBet.clear();
	this->arrCoin.clear();
	this->listHistory.clear();
	this->listTopWinner.clear();
	this->arrTextHayDatCuoc.clear();
	this->press = 0;
	this->SizeAdd = ChanUtils::getSizePos();

	this->ArrListValueBetDefault.push_back(BET_VALUE_1K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_2K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_5K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_10K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_50K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_100K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_500K);
	this->ArrListValueBetDefault.push_back(BET_VALUE_1M);
	this->ArrListValueBetDefault.push_back(BET_VALUE_5M);
}

LayerGameTaiXiu::~LayerGameTaiXiu()
{
	GameServer::getSingleton().removeListeners(this);

	vector<TaiXiuBet*>::iterator it;
	for (it = this->arrBet.begin(); it != this->arrBet.end();){
		delete *it;
		it = this->arrBet.erase(it);
	}
	this->arrBet.clear();

	vector<TaiXiuCoin*>::iterator it1;
	for (it1 = this->arrCoin.begin(); it1 != this->arrCoin.end();){
		delete *it1;
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
	this->ArrayListCoinFromStudio.clear();

}

static inline void printAllChildrenTypes(Node* node, int level)
{
	std::string spaces(level, ' ');
	log("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());

	++level;
	int index = 0;

	while (index < node->getChildren().size()){
		Node* obj = node->getChildren().at(index);
		printAllChildrenTypes(static_cast<Node*>(obj), level);
		index++;
	}
}

bool LayerGameTaiXiu::init()
{
	if (!Layer::init())
		return false;

	boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
	if (mySelf == NULL)
		return false;

	Size SizeAdd = ChanUtils::getSizePos();
	Size winsize = Director::getInstance()->getVisibleSize();
	Sprite* p = Sprite::create("tx-background.jpg");
	p->setScaleX(winsize.width / p->getContentSize().width);
	p->setScaleY(winsize.height / p->getContentSize().height);
	p->setAnchorPoint(Vec2(0, 0));
	p->setPosition(Vec2(-SizeAdd.width, -SizeAdd.height));
	this->addChild(p);

	this->MyAI = *mySelf->Name();

	this->mQueueMsg = new ThreadQueueMsg();
	this->playerInfo = new PlayerTaiXiuInfo();
	this->mQueueTaiXiu = new QueueTaiXiu();

	this->SetRoomConfig();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->posFrameAnim = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	this->rootNode = CSLoader::getInstance()->createNode("LayerTaiXiuMain.csb");
	this->rootNode->setContentSize(visibleSize);

	//tool set
	this->tfResult = dynamic_cast<TextField*>(this->rootNode->getChildByName("tfResult"));
	auto btnSet = dynamic_cast<Button*>(this->rootNode->getChildByName("btnSet"));
	btnSet->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::processButtonSetTouched, this));
	this->tfResult->setVisibleEditBox(false);
	btnSet->setVisible(false);

	//moc3-begin change (dua button charge ra tx main)
	this->lblMoney = dynamic_cast<Text*>(this->rootNode->getChildByName("lblMoney"));
	float amf = 0;
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		boost::shared_ptr<UserVariable> userVars = mySelf->GetVariable("amf");
		if (userVars != NULL){
			amf = *userVars->GetDoubleValue();
			log("amf currType == 0");
			this->playerInfo->initMoney((long)amf);
		}
	}
	else if (SceneManager::getSingleton().getCurrRoomType() == 1){
		boost::shared_ptr<UserVariable> userVars = mySelf->GetVariable("amfs");
		if (userVars != NULL){
			amf = *userVars->GetDoubleValue();
			log("amf currType == 1");
			this->playerInfo->initMoney((long)amf);
		}
	}
    auto Text_50 = static_cast<Text*> (this->rootNode->getChildByName("Text_50"));
    Text_50->setString(dataManager.GetSysString(746));
    auto Text_51 = static_cast<Text*> (this->rootNode->getChildByName("Text_51"));
    Text_51->setString(dataManager.GetSysString(45));
	this->lblBetMoney = (Text*)this->rootNode->getChildByName("lblBetMoney");
	this->lblWinMoney = (Text*)this->rootNode->getChildByName("lblWinMoney");
	this->lblWinMoney->setString("0");
	this->iconMoney = static_cast<ImageView*>(rootNode->getChildByName("imgcoin"));
	int currentType = SceneManager::getSingleton().getCurrRoomType();
	if (currentType == 0){
		this->iconMoney->loadTexture("moc3-icon-moc.png");
	}
	else{
		this->iconMoney->loadTexture("moc3-icon-hao.png");
	}

	this->lblMoney->setString(mUtils::convertMoneyEx(amf));
	this->DisplayInfoMoney(this->playerInfo->SoTienThuc, this->playerInfo->SoTienDatCuocClient);

	Button* btnCharge = static_cast<Button*>(this->rootNode->getChildByName("btnRecharge"));
	if (btnCharge != NULL){
		btnCharge->setPressedActionEnabled(true);
		btnCharge->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::eventTouchBtnCoin, this));
	}
	//moc3-end change
	Sprite* resultBackground;
	Node* layerTaiXiuTop = static_cast<cocos2d::Node*>(rootNode->getChildByName("layerTaiXiuTop"));
	if (layerTaiXiuTop != NULL){

		resultBackground = static_cast<cocos2d::Sprite*>(layerTaiXiuTop->getChildByName("resultBackground"));

		if (resultBackground != NULL){

			this->posFrameAnim = resultBackground->getParent()->convertToWorldSpace(resultBackground->getPosition());

			this->resultDice1 = static_cast<cocos2d::Sprite*>(resultBackground->getChildByName("resultDice1"));
			this->resultDice2 = static_cast<cocos2d::Sprite*>(resultBackground->getChildByName("resultDice2"));
			this->resultDice3 = static_cast<cocos2d::Sprite*>(resultBackground->getChildByName("resultDice3"));

			if (this->resultDice1 && this->resultDice2 && this->resultDice3) {
				this->resultDice1->setVisible(false);
				this->resultDice2->setVisible(false);
				this->resultDice3->setVisible(false);
			}
		}

		Button* btnTopThang = static_cast<Button*>(layerTaiXiuTop->getChildByName("btnTopThang"));
		if (btnTopThang != NULL){
			btnTopThang->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::OnButtonTopWinClick, this));
		}

		Button* btnChargeMoney = static_cast<Button*>(layerTaiXiuTop->getChildByName("btnCharge"));
		if (btnChargeMoney != NULL){
			btnChargeMoney->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::eventTouchBtnCoin, this));
		}

		this->txtNumOfPlayers = static_cast<Text*>(layerTaiXiuTop->getChildByName("txtNumOfPlayers"));
		if (this->txtNumOfPlayers != NULL){
			boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lstRoom != NULL){
				std::string uCount = boost::to_string(lstRoom->UserCount());
				uCount = uCount + " "+dataManager.GetSysString(19);
				this->txtNumOfPlayers->setString(uCount);
			}
			else{
				this->txtNumOfPlayers->setString("0 "+dataManager.GetSysString(19));
			}
		}

		this->txtTimer = static_cast<Text*>(layerTaiXiuTop->getChildByName("txtTimer"));

		if (this->txtTimer != NULL){
			this->lblCountNextTime = Label::createWithBMFont(LucidaGrandeFnt, "");
			this->lblCountNextTime->setAnchorPoint(this->txtTimer->getAnchorPoint());
			this->lblCountNextTime->setPosition(this->txtTimer->getPosition());
			this->lblCountNextTime->setScale(1.8);
			this->lblCountNextTime->setColor(Color3B::YELLOW);
			this->lblCountNextTime->setVisible(false);
			layerTaiXiuTop->addChild(this->lblCountNextTime);
		}

		Button* btnLichSu = static_cast<Button*>(layerTaiXiuTop->getChildByName("btnLichSu"));
		if (btnLichSu != NULL){
			btnLichSu->setPressedActionEnabled(true);
			btnLichSu->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::OnButtonHistoryClick, this));
            btnLichSu->setTitleText(dataManager.GetSysString(856));

		}
	}

	Node* layerTaiXiuBottom = static_cast<cocos2d::Node*>(rootNode->getChildByName("layerTaiXiuBottom"));
	if (layerTaiXiuBottom != NULL) {

		Button* btn1k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn1k"));
		Button* btn5k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn5k"));
		Button* btn10k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn10k"));
		Button* btn50k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn50k"));
		Button* btn100k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn100k"));
		Button* btn500k = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn500k"));
		Button* btn1M = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn1M"));
		Button* btn5M = static_cast<Button*>(layerTaiXiuBottom->getChildByName("btn5M"));

		BtnCoin btnCoin1;
		btnCoin1.btn = btn1k;
		btnCoin1.tag = tag_BET_VALUE_2K;
		this->ArrayListCoinFromStudio.push_back(btnCoin1);
		BtnCoin btnCoin2;
		btnCoin2.btn = btn5k;
		btnCoin2.tag = tag_BET_VALUE_5K;
		this->ArrayListCoinFromStudio.push_back(btnCoin2);
		BtnCoin btnCoin3;
		btnCoin3.btn = btn10k;
		btnCoin3.tag = tag_BET_VALUE_10K;
		this->ArrayListCoinFromStudio.push_back(btnCoin3);
		BtnCoin btnCoin4;
		btnCoin4.btn = btn50k;
		btnCoin4.tag = tag_BET_VALUE_50K;
		this->ArrayListCoinFromStudio.push_back(btnCoin4);
		BtnCoin btnCoin5;
		btnCoin5.btn = btn100k;
		btnCoin5.tag = tag_BET_VALUE_100K;
		this->ArrayListCoinFromStudio.push_back(btnCoin5);
		BtnCoin btnCoin6;
		btnCoin6.btn = btn500k;
		btnCoin6.tag = tag_BET_VALUE_500K;
		this->ArrayListCoinFromStudio.push_back(btnCoin6);
		BtnCoin btnCoin7;
		btnCoin7.btn = btn1M;
		btnCoin7.tag = tag_BET_VALUE_1M;
		this->ArrayListCoinFromStudio.push_back(btnCoin7);
		BtnCoin btnCoin8;
		btnCoin8.btn = btn5M;
		btnCoin8.tag = tag_BET_VALUE_5M;
		this->ArrayListCoinFromStudio.push_back(btnCoin8);
		initListCoin();
		//initPlayerInfo();
	}

	//Line 1.
	Node* nodeCuocLine1 = static_cast<cocos2d::Node*>(rootNode->getChildByName("nodeCuocLine1"));
	if (nodeCuocLine1 != NULL){

		Button* btnXiu = static_cast<Button*>(nodeCuocLine1->getChildByName("btnXiu"));
        auto lblTitleXiu = static_cast<Text*> (btnXiu->getChildByName("taiLbl_0_0"));
        lblTitleXiu->setString(dataManager.GetSysString(891));
		Button* btnTai = static_cast<Button*>(nodeCuocLine1->getChildByName("btnTai"));
        auto lblTitleTai = static_cast<Text*> (btnTai->getChildByName("taiLbl_0"));
        lblTitleTai->setString(dataManager.GetSysString(892));
		this->CreateContainerButton(btnXiu, "btnXiu", VALUE_TAIXIU, tag_BET_TYPE_XIU, BET_TYPE_XIU, "table-1x1");
		this->CreateContainerButton(btnTai, "btnTai", VALUE_TAIXIU, tag_BET_TYPE_TAI, BET_TYPE_TAI, "table-1x1");

		Node* nodeCuocDoi1 = static_cast<cocos2d::Node*>(nodeCuocLine1->getChildByName("nodeCuocDoi1"));
		Button* btnCuocDoi1 = static_cast<Button*>(nodeCuocDoi1->getChildByName("btnCuocDoi1"));
		Button* btnCuocDoi2 = static_cast<Button*>(nodeCuocDoi1->getChildByName("btnCuocDoi2"));
		Button* btnCuocDoi3 = static_cast<Button*>(nodeCuocDoi1->getChildByName("btnCuocDoi3"));

		this->CreateContainerButton(btnCuocDoi1, "btnCuocDoi1", VALUE_BODOI, tag_BET_TYPE_DOI_1, BET_TYPE_DOI_1, "table-1x11-element");
		this->CreateContainerButton(btnCuocDoi2, "btnCuocDoi2", VALUE_BODOI, tag_BET_TYPE_DOI_2, BET_TYPE_DOI_2, "table-1x11-element");
		this->CreateContainerButton(btnCuocDoi3, "btnCuocDoi3", VALUE_BODOI, tag_BET_TYPE_DOI_3, BET_TYPE_DOI_3, "table-1x11-element");

		//Cuoc bo Ba
		Node* nodeCuocBa1 = static_cast<cocos2d::Node*>(nodeCuocLine1->getChildByName("nodeCuocBa1"));
		Button* btnCuocBa1 = static_cast<Button*>(nodeCuocBa1->getChildByName("btnCuocBa1"));
		Button* btnCuocBa2 = static_cast<Button*>(nodeCuocBa1->getChildByName("btnCuocBa2"));
		Button* btnCuocBa3 = static_cast<Button*>(nodeCuocBa1->getChildByName("btnCuocBa3"));
		this->CreateContainerButton(btnCuocBa1, "btnCuocBa1", VALUE_BOBA, tag_BET_TYPE_BA_1, BET_TYPE_BA_1, "table-1x180-element");
		this->CreateContainerButton(btnCuocBa2, "btnCuocBa2", VALUE_BOBA, tag_BET_TYPE_BA_2, BET_TYPE_BA_2, "table-1x180-element");
		this->CreateContainerButton(btnCuocBa3, "btnCuocBa3", VALUE_BOBA, tag_BET_TYPE_BA_3, BET_TYPE_BA_3, "table-1x180-element");

		Button* btnCuocBaAll = static_cast<Button*>(nodeCuocLine1->getChildByName("btnCuocBaAll"));
		this->CreateContainerButton(btnCuocBaAll, "btnCuocBaAll", VALUE_ANYBA, tag_BET_TYPE_ANY_BA, BET_TYPE_ANY_BA, "table-1x30");

		Node* nodeCuocBa2 = static_cast<cocos2d::Node*>(nodeCuocLine1->getChildByName("nodeCuocBa2"));
		Button* btnCuocBa4 = static_cast<Button*>(nodeCuocBa2->getChildByName("btnCuocBa4"));
		Button* btnCuocBa5 = static_cast<Button*>(nodeCuocBa2->getChildByName("btnCuocBa5"));
		Button* btnCuocBa6 = static_cast<Button*>(nodeCuocBa2->getChildByName("btnCuocBa6"));

		this->CreateContainerButton(btnCuocBa4, "btnCuocBa4", VALUE_BOBA, tag_BET_TYPE_BA_4, BET_TYPE_BA_4, "table-1x180-element");
		this->CreateContainerButton(btnCuocBa5, "btnCuocBa5", VALUE_BOBA, tag_BET_TYPE_BA_5, BET_TYPE_BA_5, "table-1x180-element");
		this->CreateContainerButton(btnCuocBa6, "btnCuocBa6", VALUE_BOBA, tag_BET_TYPE_BA_6, BET_TYPE_BA_6, "table-1x180-element");

		//Cuoc doi 2
		Node* nodeCuocDoi2 = static_cast<cocos2d::Node*>(nodeCuocLine1->getChildByName("nodeCuocDoi2"));
		Button* btnCuocDoi4 = static_cast<Button*>(nodeCuocDoi2->getChildByName("btnCuocDoi4"));
		Button* btnCuocDoi5 = static_cast<Button*>(nodeCuocDoi2->getChildByName("btnCuocDoi5"));
		Button* btnCuocDoi6 = static_cast<Button*>(nodeCuocDoi2->getChildByName("btnCuocDoi6"));

		this->CreateContainerButton(btnCuocDoi4, "btnCuocDoi4", VALUE_BODOI, tag_BET_TYPE_DOI_4, BET_TYPE_DOI_4, "table-1x11-element");
		this->CreateContainerButton(btnCuocDoi5, "btnCuocDoi5", VALUE_BODOI, tag_BET_TYPE_DOI_5, BET_TYPE_DOI_5, "table-1x11-element");
		this->CreateContainerButton(btnCuocDoi6, "btnCuocDoi6", VALUE_BODOI, tag_BET_TYPE_DOI_6, BET_TYPE_DOI_6, "table-1x11-element");
	}

	//Line tong diem
	Node* nodeCuocTong = static_cast<cocos2d::Node*>(rootNode->getChildByName("nodeCuocTong"));
	if (nodeCuocTong != NULL){
		Button* btnCuocTong4 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong4"));
		Button* btnCuocTong5 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong5"));
		Button* btnCuocTong6 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong6"));
		Button* btnCuocTong7 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong7"));
		Button* btnCuocTong8 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong8"));
		Button* btnCuocTong9 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong9"));
		Button* btnCuocTong10 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong10"));
		Button* btnCuocTong11 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong11"));
		Button* btnCuocTong12 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong12"));
		Button* btnCuocTong13 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong13"));
		Button* btnCuocTong14 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong14"));
		Button* btnCuocTong15 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong15"));
		Button* btnCuocTong16 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong16"));
		Button* btnCuocTong17 = static_cast<Button*>(nodeCuocTong->getChildByName("btnCuocTong17"));

		this->CreateContainerButton(btnCuocTong4, "btnCuocTong4 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_4, BET_TYPE_NUMBER_4, "table-small");
		this->CreateContainerButton(btnCuocTong5, "btnCuocTong5 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_5, BET_TYPE_NUMBER_5, "table-small");
		this->CreateContainerButton(btnCuocTong6, "btnCuocTong6 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_6, BET_TYPE_NUMBER_6, "table-small");
		this->CreateContainerButton(btnCuocTong7, "btnCuocTong7 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_7, BET_TYPE_NUMBER_7, "table-small");
		this->CreateContainerButton(btnCuocTong8, "btnCuocTong8 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_8, BET_TYPE_NUMBER_8, "table-small");
		this->CreateContainerButton(btnCuocTong9, "btnCuocTong9 ", VALUE_DIEM, tag_BET_TYPE_NUMBER_9, BET_TYPE_NUMBER_9, "table-small");
		this->CreateContainerButton(btnCuocTong10, "btnCuocTong10", VALUE_DIEM, tag_BET_TYPE_NUMBER_10, BET_TYPE_NUMBER_10, "table-small");
		this->CreateContainerButton(btnCuocTong11, "btnCuocTong11", VALUE_DIEM, tag_BET_TYPE_NUMBER_11, BET_TYPE_NUMBER_11, "table-small");
		this->CreateContainerButton(btnCuocTong12, "btnCuocTong12", VALUE_DIEM, tag_BET_TYPE_NUMBER_12, BET_TYPE_NUMBER_12, "table-small");
		this->CreateContainerButton(btnCuocTong13, "btnCuocTong13", VALUE_DIEM, tag_BET_TYPE_NUMBER_13, BET_TYPE_NUMBER_13, "table-small");
		this->CreateContainerButton(btnCuocTong14, "btnCuocTong14", VALUE_DIEM, tag_BET_TYPE_NUMBER_14, BET_TYPE_NUMBER_14, "table-small");
		this->CreateContainerButton(btnCuocTong15, "btnCuocTong15", VALUE_DIEM, tag_BET_TYPE_NUMBER_15, BET_TYPE_NUMBER_15, "table-small");
		this->CreateContainerButton(btnCuocTong16, "btnCuocTong16", VALUE_DIEM, tag_BET_TYPE_NUMBER_16, BET_TYPE_NUMBER_16, "table-small");
		this->CreateContainerButton(btnCuocTong17, "btnCuocTong17", VALUE_DIEM, tag_BET_TYPE_NUMBER_17, BET_TYPE_NUMBER_17, "table-small");
	}

	//Line cuoc cap
	Node* nodeCuocCap = static_cast<cocos2d::Node*>(rootNode->getChildByName("nodeCuocCap"));
	if (nodeCuocCap != NULL){
		Button* btnCuocCap12 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap12"));
		Button* btnCuocCap13 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap13"));
		Button* btnCuocCap14 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap14"));
		Button* btnCuocCap15 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap15"));
		Button* btnCuocCap16 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap16"));
		Button* btnCuocCap23 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap23"));
		Button* btnCuocCap24 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap24"));
		Button* btnCuocCap25 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap25"));
		Button* btnCuocCap26 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap26"));
		Button* btnCuocCap34 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap34"));
		Button* btnCuocCap35 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap35"));
		Button* btnCuocCap36 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap36"));
		Button* btnCuocCap45 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap45"));
		Button* btnCuocCap46 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap46"));
		Button* btnCuocCap56 = static_cast<Button*>(nodeCuocCap->getChildByName("btnCuocCap56"));

		this->CreateContainerButton(btnCuocCap12, "btnCuocCap12", VALUE_CAPDOI, tag_BET_TYPE_CAP_1_2, BET_TYPE_CAP_1_2, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap13, "btnCuocCap13", VALUE_CAPDOI, tag_BET_TYPE_CAP_1_3, BET_TYPE_CAP_1_3, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap14, "btnCuocCap14", VALUE_CAPDOI, tag_BET_TYPE_CAP_1_4, BET_TYPE_CAP_1_4, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap15, "btnCuocCap15", VALUE_CAPDOI, tag_BET_TYPE_CAP_1_5, BET_TYPE_CAP_1_5, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap16, "btnCuocCap16", VALUE_CAPDOI, tag_BET_TYPE_CAP_1_6, BET_TYPE_CAP_1_6, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap23, "btnCuocCap23", VALUE_CAPDOI, tag_BET_TYPE_CAP_2_3, BET_TYPE_CAP_2_3, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap24, "btnCuocCap24", VALUE_CAPDOI, tag_BET_TYPE_CAP_2_4, BET_TYPE_CAP_2_4, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap25, "btnCuocCap25", VALUE_CAPDOI, tag_BET_TYPE_CAP_2_5, BET_TYPE_CAP_2_5, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap26, "btnCuocCap26", VALUE_CAPDOI, tag_BET_TYPE_CAP_2_6, BET_TYPE_CAP_2_6, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap34, "btnCuocCap34", VALUE_CAPDOI, tag_BET_TYPE_CAP_3_4, BET_TYPE_CAP_3_4, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap35, "btnCuocCap35", VALUE_CAPDOI, tag_BET_TYPE_CAP_3_5, BET_TYPE_CAP_3_5, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap36, "btnCuocCap36", VALUE_CAPDOI, tag_BET_TYPE_CAP_3_6, BET_TYPE_CAP_3_6, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap45, "btnCuocCap45", VALUE_CAPDOI, tag_BET_TYPE_CAP_4_5, BET_TYPE_CAP_4_5, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap46, "btnCuocCap46", VALUE_CAPDOI, tag_BET_TYPE_CAP_4_6, BET_TYPE_CAP_4_6, "table-1x6-element");
		this->CreateContainerButton(btnCuocCap56, "btnCuocCap56", VALUE_CAPDOI, tag_BET_TYPE_CAP_5_6, BET_TYPE_CAP_5_6, "table-1x6-element");
	}

	//Line cuoc don
	Node* nodeCuocDon = static_cast<cocos2d::Node*>(rootNode->getChildByName("nodeCuocDon"));
	if (nodeCuocDon != NULL){
		Button* btnCuocDon1 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon1"));
		Button* btnCuocDon2 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon2"));
		Button* btnCuocDon3 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon3"));
		Button* btnCuocDon4 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon4"));
		Button* btnCuocDon5 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon5"));
		Button* btnCuocDon6 = static_cast<Button*>(nodeCuocDon->getChildByName("btnCuocDon6"));

        auto Text_41 = static_cast<Text*> (nodeCuocDon->getChildByName("Text_41"));
        Text_41->setString(dataManager.GetSysString(893));
        
        auto Text_41_1 = static_cast<Text*> (nodeCuocDon->getChildByName("Text_41_1"));
        Text_41_1->setString(dataManager.GetSysString(894));
        
        auto Text_41_0 = static_cast<Text*> (nodeCuocDon->getChildByName("Text_41_0"));
        Text_41_0->setString(dataManager.GetSysString(895));
        
		this->CreateContainerButton(btnCuocDon1, "btnCuocDon1", VALUE_DONLE, tag_BET_TYPE_DON_LE_1, BET_TYPE_DON_LE_1, "table-bottom-element");
		this->CreateContainerButton(btnCuocDon2, "btnCuocDon2", VALUE_DONLE, tag_BET_TYPE_DON_LE_2, BET_TYPE_DON_LE_2, "table-bottom-element");
		this->CreateContainerButton(btnCuocDon3, "btnCuocDon3", VALUE_DONLE, tag_BET_TYPE_DON_LE_3, BET_TYPE_DON_LE_3, "table-bottom-element");
		this->CreateContainerButton(btnCuocDon4, "btnCuocDon4", VALUE_DONLE, tag_BET_TYPE_DON_LE_4, BET_TYPE_DON_LE_4, "table-bottom-element");
		this->CreateContainerButton(btnCuocDon5, "btnCuocDon5", VALUE_DONLE, tag_BET_TYPE_DON_LE_5, BET_TYPE_DON_LE_5, "table-bottom-element");
		this->CreateContainerButton(btnCuocDon6, "btnCuocDon6", VALUE_DONLE, tag_BET_TYPE_DON_LE_6, BET_TYPE_DON_LE_6, "table-bottom-element");
	}

	ui::Helper::doLayout(this->rootNode);
	this->addChild(this->rootNode);

	this->SelectCoin(this->playerInfo->SoTienThuc);

	this->arrTextHayDatCuoc.clear();
	this->arrTextHayDatCuoc.push_back(dataManager.GetSysString(12)+" .");
	this->arrTextHayDatCuoc.push_back(dataManager.GetSysString(12)+" ..");
	this->arrTextHayDatCuoc.push_back(dataManager.GetSysString(12)+" ...");

	this->CountSizeDatCuoc = 0;
	this->HayDatCuoc = Label::createWithSystemFont(dataManager.GetSysString(12)+" .","",40);
	this->HayDatCuoc->setColor(Color3B::YELLOW);
	this->HayDatCuoc->setScale(1.35);
	this->HayDatCuoc->setAnchorPoint(Vec2(0, 0.5));
	this->HayDatCuoc->setPosition(Vec2(30, resultBackground->getContentSize().height / 2));
	this->HayDatCuoc->setVisible(false);
	resultBackground->addChild(this->HayDatCuoc);

	this->Toast = LayerChanToast::create();
	this->Toast->setLocalZOrder(1000);
	this->addChild(this->Toast);

	// Layer Buttons (Dung chung cho cac game)
	LayerButtonInGame* layerButton = LayerButtonInGame::create();
	layerButton->hideButtonInChanGame();
	layerButton->setLocalZOrder(1100);
	this->addChild(layerButton);

	float widButton2 = 82;
	float dis = 30;

	Button* btnCoin = (Button*)layerButton->getChildByTag(TAG_LB_BTN_COIN);
	if (btnCoin != NULL){
		btnCoin->setVisible(false);
	}

	Button* btnHuongDan = (Button*)layerTaiXiuTop->getChildByName("btnHelp");
	if (btnHuongDan != NULL) {
		btnHuongDan->setPressedActionEnabled(true);
		btnHuongDan->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::OnBtnHuongDanChan, this));
        btnHuongDan->setTitleText(dataManager.GetSysString(887));
	}

	Button* btnChat = (Button*)layerButton->getChildByTag(TAG_LB_BTN_CHAT);
	if (btnChat != NULL){
		btnChat->setAnchorPoint(Vec2(0, 0));
		btnChat->setVisible(true);
		btnChat->setPosition(Vec2(dis, dis));
		btnChat->setVisible(false);
	}

	Button* btnBack = (Button*)layerButton->getChildByTag(TAG_LB_BTN_BACK);
	btnBack->setAnchorPoint(Vec2(0, 1));
	btnBack->setPosition(Vec2(dis, HEIGHT_DESIGN - dis));

	//Sprite* signal = (Sprite*)layerButton->getChildByTag(120);
	//if (signal != NULL){
	//	signal->setVisible(false);
	//	this->signalNetwork = Sprite::create("signal-good.png");
	//	this->signalNetwork->setAnchorPoint(Vec2(0, 1));
	//	this->signalNetwork->setPosition(Vec2(visibleSize.width - signalNetwork->getContentSize().width - dis, HEIGHT_DESIGN - dis));
	//	this->addChild(this->signalNetwork);
	//}

	Button* menu = (Button*)layerButton->getChildByTag(101);
	if (menu != NULL) {
		menu->setAnchorPoint(Vec2(0, 1));
		//menu->setScale(0.8);
		menu->setPosition(Vec2(20, HEIGHT_DESIGN - 20));
	}

	this->mQueueMsg->setFunCallBack(this, callfunc_selector(LayerGameTaiXiu::OnExtensionResponseProcess));
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::updateEvent),this,1/60, false);

	GameServer::getSingleton().addListeners(this);

	return true;
}

void LayerGameTaiXiu::SelectCoin(const double& amf){
	std::string vals = this->SelectCoinValue(amf);
	bool ret = false;
	for (int i = 0; i < this->arrCoin.size(); ++i) {
		try {
			TaiXiuCoin* pCoin = this->arrCoin.at(i);
			if (pCoin->GetType() == vals) {
				pCoin->SetSelected(true);
				this->CurrentBetValue = vals;
				ret = true;
			}
			else {
				pCoin->SetSelected(false);
			}
		}
		catch (...) {
			log("exception when enable coin");
		}
	}
	if (!ret) {
		this->CurrentBetValue = "";
	}
}
void LayerGameTaiXiu::initListCoin() {

	//this.ArrayListCoinFromStudio.push({btn: btn1k, tag: tag_BET_VALUE_1K});
	for (int i = 0; i < this->ArrListValueBet.size(); ++i) {
		try {
			this->CreateContainerCoin(
				this->ArrayListCoinFromStudio.at(i).btn,
				"btn1k",
				this->ArrayListCoinFromStudio.at(i).tag,
				this->ArrListValueBet.at(i).c_str());
		}
		catch (std::out_of_range &e) {
			log("Caught an out_of_range exception: %s", e.what());
		}
		catch (std::exception &e) {
			log("Caught an exception of an unexpected type %s", e.what());
		}
		catch (...) {
			log("Caught an unknown exception");
		}
	}

	if (this->ArrListValueBet.size() < this->ArrayListCoinFromStudio.size()) {
		int len = this->ArrListValueBet.size();
		try {
			for (int i = len; i < this->ArrayListCoinFromStudio.size(); ++i) {
				this->ArrayListCoinFromStudio.at(i).btn->setVisible(false);
			}
		}
		catch (std::out_of_range &e) {
			log("Caught an out_of_range exception: %s", e.what());
		}
		catch (std::exception &e) {
			log("Caught an exception of an unexpected type %s", e.what());
		}
		catch (...) {
			log("Caught an unknown exception");
		}

	}
}

// ham nay chon muc tien phu hop voi so tien hien co cua player 
std::string LayerGameTaiXiu::SelectCoinValue(const double& amf, const int& ext)
{
	auto r1 = this->ArrListValueBet.at(0) == BET_VALUE_1K;

	if (r1) {
		if (amf < 1000)
			return "";
		double per;
		if (0 == ext) {
			per = amf / 20;
		}
		else {
			per = amf;
		}

		if (per >= 0 && per < 2000) {
			return BET_VALUE_1K;
		}
		else if (per >= 2000 && per < 5000) {
			return BET_VALUE_2K;
		}
		else if (per >= 5000 && per < 10000) {
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

		else if (per >= 1000000 && per < 5000000) {
			return BET_VALUE_1M;
		}

		return BET_VALUE_5M;
	}
	else{
		if (amf < 1)
			return "";
		double per;
		if (0 == ext) {
			per = amf / 20;
		}
		else {
			per = amf;
		}

		if (per >= 0 && per < 100) {
			return BET_VALUE_1;
		}
		else if (per >= 100 && per < 500) {
			return BET_VALUE_100;
		}
		else if (per >= 500 && per < 1000) {
			return BET_VALUE_500;
		}
		else if (per >= 1000 && per < 2000) {
			return BET_VALUE_1K;
		}
		else if (per >= 2000 && per < 5000) {
			return BET_VALUE_2K;
		}
		else if (per >= 5000 && per < 10000) {
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

		else if (per >= 1000000 && per < 5000000) {
			return BET_VALUE_500K;
		}

		return BET_VALUE_500K;
	}
}

void LayerGameTaiXiu::CreateContainerButton(Button* btn, const char* name, const int& group, const int& tag, const char* type, const char* url){

	btn->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::OnButtonBetClick, this));

	TaiXiuBet* pTaiXiu = TaiXiuBetFactory::CreateTXBet(group, btn, name, tag, type);
	pTaiXiu->SetTarget(this);
	std::string normal = string(url) + ".png";
	std::string select = string(url) + "-press.png";
	std::string active = string(url) + "-active.png";
	std::string eate = string(url) + "-an.png";
	pTaiXiu->SetImage(normal, select, active, eate);

	// Tao o ghi chu.
	SuggestTaiXiu* suggest = SuggestTaiXiu::create();
	suggest->setAnchorPoint(Vec2(0.5, 0.5));
	suggest->loadTexture("tx-popup-press.png");

	Vec2 pos = btn->getParent()->convertToWorldSpace(btn->getPosition());
	Size s = btn->getContentSize();

	Vec2 point;
	Vec2 _poscoin = Vec2(30, 30);
	Vec2 pp = Vec2(0, 0);

	switch (group) {
	case VALUE_TAIXIU:
	{
						 point.x = pos.x + s.width / 2 - 30;
						 point.y = pos.y + s.height / 2 + 30;
						 _poscoin.x = s.width / 2;
						 _poscoin.y = 80;

						 pp.x = pos.x;
						 pp.y = pos.y - 80;
	}
		break;
	case VALUE_BODOI:
	{
						point.x = pos.x + s.width / 2 - 30;
						point.y = pos.y + s.height / 2 + 70;

						_poscoin.x = s.width / 2;
						_poscoin.y = s.height / 2;

						pp.x = pos.x;
						pp.y = pos.y;
	}
		break;
	case VALUE_BOBA:
	{
					   point.x = pos.x + s.width / 2 - 60;
					   point.y = pos.y + s.height / 2 + 70;

					   _poscoin.x = s.width / 2 - 35;
					   _poscoin.y = s.height / 2;

					   pp.x = pos.x - 35;
					   pp.y = pos.y - s.height / 2 + 30;
	}
		break;

	case VALUE_ANYBA:
	{
						point.x = pos.x + s.width / 2 - 160;
						point.y = pos.y + s.height / 2 + 70;

						_poscoin.x = s.width / 2;
						_poscoin.y = s.height / 2 - 40;

						pp.x = pos.x;
						pp.y = pos.y - 40;

	}
		break;
	case VALUE_CAPDOI:
	{
						 point.x = pos.x + s.width / 2 + 30;
						 point.y = pos.y + s.height / 2 + 70;
						 _poscoin.x = 25;
						 _poscoin.y = s.height / 2;

						 pp.x = pos.x - 25;
						 pp.y = pos.y;

	}
		break;
	case VALUE_DIEM:
	{
					   point.x = pos.x + s.width / 2 + 30;
					   point.y = pos.y + s.height / 2 + 65;
					   _poscoin.y = s.height / 2;

					   pp.x = pos.x - s.width / 2 + 30;
					   pp.y = pos.y;
	}
		break;
	case VALUE_DONLE:
	{
						point.x = pos.x + s.width / 2 + 30;
						point.y = pos.y + s.height / 2 + 65;

						pp.x = pos.x - s.width / 2 + 30;
						pp.y = pos.y - s.height / 2 + 30;

	}
		break;
	}

	suggest->setPosition(point);
	suggest->setVisible(true);
	suggest->setVisible(false);
	this->addChild(suggest, this->CountZoder++);

	//Container quan ly note nay.
	pTaiXiu->SetSuggest(suggest);
	pTaiXiu->SetPositionCoin(_poscoin);
	pTaiXiu->SetWorldPos(pp);

	this->arrBet.push_back(pTaiXiu);
}

void LayerGameTaiXiu::HideAllSuggestion(){
	for (int i = 0; i < this->arrBet.size(); ++i){
		this->arrBet.at(i)->HideCurrentBet();
	}
	this->CurrentTag = -1;
	this->OldPressTag = -1;
}

void LayerGameTaiXiu::CreateContainerCoin(Button* btn, const char* name, const int& tag, const char* type){
	btn->addTouchEventListener(CC_CALLBACK_2(LayerGameTaiXiu::OnButtonChipClick, this));
	btn->setTag(tag);

	std::string n = boost::to_string(name);
	std::string t = boost::to_string(type);



	//    item, name, type, tag

	TaiXiuCoin* pCoin = TaiXiuCoinFactory::CreateTaiXiuCoin(btn, n, t, tag);
	btn->loadTextureNormal(pCoin->GetValue().imgnormal);
	btn->loadTexturePressed(pCoin->GetValue().imgselect);
	this->arrCoin.push_back(pCoin);
}

TaiXiuBet* LayerGameTaiXiu::GetContainerBetByButtonTag(const int& tag){
	for (int i = 0; i < this->arrBet.size(); ++i){
		TaiXiuBet* bet = this->arrBet.at(i);
		if (bet->GetTag() == tag){
			return bet;
		}
	}
	return NULL;
}

TaiXiuBet* LayerGameTaiXiu::GetContainerBetByBetType(const string& bettype){
	for (int i = 0; i < this->arrBet.size(); ++i){
		TaiXiuBet* bet = this->arrBet.at(i);
		if (0 == bettype.compare(bet->GetType())){
			return bet;
		}
	}
	return NULL;
}

TaiXiuCoin *LayerGameTaiXiu::GetTaiXiuCoin(const string &type)
{
	for (int i = 0; i < this->arrCoin.size(); ++i){
		TaiXiuCoin* pCoin = this->arrCoin.at(i);
		if (0 == pCoin->GetType().compare(type)){
			return pCoin;
		}
	}
	return NULL;
}

int LayerGameTaiXiu::GetMoneyBet(const string &gbv)
{
	if (0 == gbv.compare(BET_VALUE_1)) {
		return 1;
	}
	else if (0 == gbv.compare(BET_VALUE_100))
		return 100;
	else if (0 == gbv.compare(BET_VALUE_500))
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

void LayerGameTaiXiu::updatepress(float delta)
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

void LayerGameTaiXiu::OnButtonChipClick(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {

		TaiXiuCoin* __current = NULL;

		for (int i = 0; i < this->arrCoin.size(); ++i) {
			if (this->arrCoin.at(i)->GetType() == this->CurrentBetValue) {
				__current = this->arrCoin.at(i);
				break;
			}
		}

		Button* item = (Button*)sender;
		for (int i = 0; i < this->arrCoin.size(); ++i)
		{
			TaiXiuCoin* pCoin = this->arrCoin.at(i);
			if (pCoin->GetTag() == item->getTag())
			{
				if (boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->MySelf())
				{
					if (SceneManager::getSingleton().getCurrRoomType() == 0){
						if (boost::shared_ptr<UserVariable> amf = user->GetVariable("amf")){
							int money = pCoin->GetValue().value;
							if (*amf->GetDoubleValue() < money){
								this->Toast->ShowToast(dataManager.GetSysString(33));
							}
							else
							{
								__current = pCoin;
							}
						}
					}
					else{
						if (boost::shared_ptr<UserVariable> amf = user->GetVariable("amfs")){
							int money = pCoin->GetValue().value;
							if (*amf->GetDoubleValue() < money){
								this->Toast->ShowToast(dataManager.GetSysString(33));
							}
							else
							{
								__current = pCoin;
							}
						}
					}
				}
			}
			else
			{
				pCoin->SetSelected(false);
			}
		}

		if (__current != NULL) {
			__current->SetSelected(true);
			this->CurrentBetValue = __current->GetType();
		}
	}
}

void LayerGameTaiXiu::OnButtonBetClick(Ref *sender, Widget::TouchEventType pType)
{

	if (pType == ui::Widget::TouchEventType::BEGAN){
		Button* btn = (Button*)sender;
		this->CurrentTag = btn->getTag();
		this->press = 0;
		Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::updatepress),this,0.1, false);
	}

	if (pType == ui::Widget::TouchEventType::ENDED) {
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updatepress),this);
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
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updatepress),this);
		this->HideAllSuggestion();
	}
}


void LayerGameTaiXiu::BetGameByClient(const int& tag){
	if (!this->CurrentGameState) {
		this->Toast->ShowToast(dataManager.GetSysString(38));
		return;
	}

	if (this->CurrentBetValue == "") {
		this->Toast->ShowToast(dataManager.GetSysString(6));
		return;
	}

	int pMoney = this->GetMoneyBet(this->CurrentBetValue);
	auto r1 = this->ArrListValueBet.at(0) == BET_VALUE_1K;
	auto minVal = r1 ? 1000 : 1;
	if (this->playerInfo->SoTienAo < r1 || pMoney > this->playerInfo->SoTienAo) {
		this->Toast->ShowToast(dataManager.GetSysString(40));
		return;
	}

	TaiXiuBet* bet = this->GetContainerBetByButtonTag(tag);

	if (bet != NULL) {
		std::string betType = bet->GetType();

		std::string src = "";
		Vec2 __pos = Vec2(WIDTH_DESIGN / 2, 200);

		for (int i = 0; i < this->arrCoin.size(); ++i) {
			TaiXiuCoin* pCoin = this->arrCoin[i];
			if (pCoin->GetType() == this->CurrentBetValue) {
				src = pCoin->GetValue().imgnormal;
				__pos = pCoin->GetPosWorld();
				break;
			}
		}

		if (src != "") {
			Vec2 p = bet->GetWorldPos();

			//Tao image de animations
			ImageView* imgCoin = ImageView::create();
			imgCoin->loadTexture("moc3-icon-moc.png");
			imgCoin->setScale(SCALE_COIN_TAI_XIU);
			__pos.y = __pos.y - 95;
			imgCoin->setPosition(__pos);

			MoveTo* moveTo = MoveTo::create(DURATION_ANIMATE_COIN_CLICK, p);
			CallFuncN * callfun = CallFuncN::create(CC_CALLBACK_1(LayerGameTaiXiu::AddCoinCallFunc2, this, bet));

			//Add image và animation.
			this->addChild(imgCoin);

			imgCoin->runAction(Sequence::create(moveTo, callfun, NULL));

			//Hiệu ứng trước
			this->playerInfo->PushBet(betType);
			this->playerInfo->TruTienThuc(pMoney);

			//Hiển thị số tiền đã cược.
			this->DisplayInfoMoney(this->playerInfo->SoTienAo, this->playerInfo->SoTienDatCuocClient);

			// Hiển thị thông tin ô bet.
			bet->MySelect();
			bet->ClientBet(pMoney);
		}

		boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (NULL == lstRoom)
			return;
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString(EXT_FIELD_GAME_BET_VALUE, this->CurrentBetValue);
		params->PutUtfString(EXT_FIELD_GAME_BET_TYPE, betType);
		boost::shared_ptr<IRequest> req2(new ExtensionRequest(EXT_EVENT_GAME_BET_REQ, params, lstRoom));
		GameServer::getSingleton().Send(req2);

		if (this->playerInfo->SoTienAo <= pMoney) {
			this->SelectCoin(this->playerInfo->SoTienAo);
		}
	}
}

void LayerGameTaiXiu::AddCoinCallFunc(Ref *sender, Struct4Call1 data){
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

void LayerGameTaiXiu::AddCoinCallFunc2(Ref *sender, TaiXiuBet* data){
	ImageView* img = (ImageView*)sender;
	if (img != NULL){
		data->AddCoin(img);
	}
}

void LayerGameTaiXiu::OnButtonHistoryClick(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {

		if (this->getChildByTag(TAG_TAI_XIU_HISTORY) != NULL)
			this->removeChildByTag(TAG_TAI_XIU_HISTORY);

		LayerTaiXiuHistory* History = LayerTaiXiuHistory::create(this->listHistory);
		History->setPosition(Vec2(-this->SizeAdd.width, -this->SizeAdd.height));
		this->addChild(History, 100, TAG_TAI_XIU_HISTORY);
	}
}

void LayerGameTaiXiu::OnButtonTopWinClick(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {

		if (this->getChildByTag(TAG_TAI_XIU_TOPWINS) != NULL) {
			this->removeChildByTag(TAG_TAI_XIU_TOPWINS);
		}

		LayerTaiXiuPopupWinners* topwin = LayerTaiXiuPopupWinners::createInstace(this->listTopWinner);
		topwin->setPosition(Vec2(0, 0));
		//-this->SizeAdd.width, -this->SizeAdd.height
		this->addChild(topwin, 101, TAG_TAI_XIU_TOPWINS);
	}
}

void LayerGameTaiXiu::OnBtnHuongDanChan(Ref *sender, ui::Widget::TouchEventType pType){
	if (pType == ui::Widget::TouchEventType::ENDED) {
		LayerGuideInGame* layer = LayerGuideInGame::create();
		layer->setPosition(Vec2(-ChanUtils::getSizePos().width, -ChanUtils::getSizePos().height));
		this->addChild(layer, 2000);
	}
}

//Listen Server Event
void LayerGameTaiXiu::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	this->mQueueMsg->addQueue(pObjs);
}

void LayerGameTaiXiu::OnExtensionResponseProcess(){
	this->param = this->mQueueMsg->frontQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");

	if (0 == cmd->compare(EXT_EVENT_START)) {
		this->event_EXT_EVENT_START();
	}
	else if (0 == cmd->compare(EXT_EVENT_END)) {
		this->event_EXT_EVENT_END();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT)) {
		log("grs");
		this->event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_ERROR_NOTIFY)) {
		this->event_EXT_EVENT_GAME_BET_ERROR_NOTIFY();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_INFO_NTF)) {
		this->event_EXT_EVENT_GAME_INFO_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_HIS_NTF)) {
		this->event_EXT_EVENT_GAME_HIS_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY_NTF)) {
		this->event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_STATE_NTF)) {
		this->event_EXT_EVENT_GAME_STATE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_BET_NTF)) {
		this->event_EXT_EVENT_GAME_BET_NTF();
	}
	else if (0 == cmd->compare("updateinfome")){
		if (SceneManager::getSingleton().getCurrRoomType() == 0){
			boost::shared_ptr<double> money = this->param->GetDouble("amf");
			boost::shared_ptr<string> uid = this->param->GetUtfString("uid");
			if (money != NULL){
				if (0 == uid->compare(this->MyAI)){
					this->playerInfo->SoTienAo = (long)(*money);
				}
				this->DisplayInfoMoney(*money, this->playerInfo->SoTienDatCuocClient);
			}
		}
		else{
			boost::shared_ptr<double> money = this->param->GetDouble("amfs");
			boost::shared_ptr<string> uid = this->param->GetUtfString("uid");
			if (money != NULL){
				if (0 == uid->compare(this->MyAI)){
					this->playerInfo->SoTienAo = (long)(*money);
				}
				this->DisplayInfoMoney(*money, this->playerInfo->SoTienDatCuocClient);
			}
		}
	}

	this->mQueueMsg->deQueue();
}

void LayerGameTaiXiu::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	//log("on user variable update");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);

	if (ptrNotifiedUser == NULL)
		return;

	double money = 0;
	string uid = *ptrNotifiedUser->Name();
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		if (0 == uid.compare(this->MyAI)) {
			if (ptrNotifiedUser->GetVariable(EXT_VAL_AMOUNT_BALANCE) != NULL) {
				money = *ptrNotifiedUser->GetVariable(EXT_VAL_AMOUNT_BALANCE)->GetDoubleValue();
			}
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutDouble("amf", money);
			params->PutUtfString("uid", uid);
			params->PutUtfString("_cmd_", "updateinfome");
			this->mQueueMsg->addQueue(params);
		}
	}
	else{
		if (0 == uid.compare(this->MyAI)) {
			if (ptrNotifiedUser->GetVariable(EXT_VAL_AMOUNT_BALANCE) != NULL) {
				money = *ptrNotifiedUser->GetVariable("amfs")->GetDoubleValue();
			}
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			params->PutDouble("amfs", money);
			params->PutUtfString("uid", uid);
			params->PutUtfString("_cmd_", "updateinfome");
			this->mQueueMsg->addQueue(params);
		}
	}

}

void LayerGameTaiXiu::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
	this->SetRoomConfig();
}

void LayerGameTaiXiu::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

void LayerGameTaiXiu::OnSmartFoxConnectionLost(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

void LayerGameTaiXiu::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

}

void LayerGameTaiXiu::OnSmartFoxUserCountChange(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> uCount = (*ptrEventParams)["uCount"];
	boost::shared_ptr<long> _uCount = ((boost::static_pointer_cast<long>))(uCount);
	std::string strCount = boost::to_string(*_uCount);
	this->txtNumOfPlayers->setString(dataManager.GetSysString(19)+" " + strCount);
}

void LayerGameTaiXiu::OnSmartFoxPingPong(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	/*boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["lagValue"];
	boost::shared_ptr<int> ptrNotifiedCmd = ((boost::static_pointer_cast<int>)(ptrEventParamValueCmd));
	if (NULL == ptrNotifiedCmd) return;
	if (*ptrNotifiedCmd <= 400)
	this->signalNetwork->setTexture("signal-good.png");
	else if (400 < *ptrNotifiedCmd && *ptrNotifiedCmd <= 1500)
	this->signalNetwork->setTexture("signal-normal.png");
	else
	this->signalNetwork->setTexture("signal-bad.png");*/
}

//Execute Server Event
void LayerGameTaiXiu::event_EXT_EVENT_START()
{
	boost::shared_ptr<long> ttp = this->param->GetInt(EXT_FIELD_GAME_TOTAL_PLAYER);
	if (ttp != NULL){
		string t = boost::to_string((int)*ttp);
		this->txtNumOfPlayers->setString(dataManager.GetSysString(19)+" " + t);
	}

	this->resultDice1->setVisible(false);
	this->resultDice2->setVisible(false);
	this->resultDice3->setVisible(false);

	this->ResetGame();

	this->CurrentGameState = true;

	this->UpdateTimeStartGame();
}

void LayerGameTaiXiu::event_EXT_EVENT_END(){

	this->Animating = true;

	boost::shared_ptr<string> rg = this->param->GetUtfString(EXT_FIELD_RESULT_GAME);
	boost::shared_ptr<string> rt10 = this->param->GetUtfString(EXT_FIELD_RESULT_TOP_10);

	// Stop timer dem nguoc. bat dau timer den van moi.
	this->UpdateTimeEndGame();

	if (rg != NULL && rt10 != NULL) {

		// Xóa layer lịch sử nếu đang hiển thị.
		if (this->getChildByTag(TAG_TAI_XIU_HISTORY) != NULL){
			this->removeChildByTag(TAG_TAI_XIU_HISTORY);
		}

		//Xóa layer top winners nếu đang hiển thị.
		if (this->getChildByTag(TAG_TAI_XIU_TOPWINS) != NULL){
			this->removeChildByTag(TAG_TAI_XIU_TOPWINS);
		}

		this->listHistory.push_back(*rg);
		if (this->listHistory.size() > 20) {
			this->listHistory.erase(this->listHistory.begin());
		}

		this->rg = *rg;
		this->rt10 = *rt10;

		//Kiểm tra xem số tiền cược.
		vector<string> arrayPlayerBet = this->playerInfo->GetAllBet();
		if (arrayPlayerBet.size() > 0) {
			for (int i = 0; i < arrayPlayerBet.size(); ++i) {
				std::string b = arrayPlayerBet[i];
				int val = this->GetMoneyBet(b);
			}
		}

		// Hien thi so tien da cuoc, cho dung voi server
		this->initPlayerInfo();
		this->DisplayInfoMoney(this->playerInfo->SoTienThuc, this->playerInfo->SoTienDatCuocServer);

		//Hiển thị animation
		LayerAnimationTaiXiu* animate = LayerAnimationTaiXiu::createInstance(this->posFrameAnim);
		animate->SetCallBackResult(this, callfuncO_selector(LayerGameTaiXiu::CallBackResult));
		this->addChild(animate, 999, TAG_ANIM_END_GAME);
	}
}

void LayerGameTaiXiu::event_EXT_EVENT_GAME_RESULT(){
	boost::shared_ptr<long> gbv = this->param->GetInt(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
	boost::shared_ptr<string> gbce = this->param->GetUtfString(EXT_FIELD_GAME_BET_CAN_EAT);
	std::string betCanEat = *gbce;
	log("gbce: %s", betCanEat.c_str());
	if (gbv != NULL && gtbv != NULL) {
		long m = (*gbv);
		std::string money = mUtils::convertMoneyEx(m);
		if (m > 0){
			this->lblWinMoney->setString(money);
		}
		else{
			this->lblWinMoney->setString("0");
		}

		// Hiển thị số tiền đã đặt cược.
		this->initPlayerInfo();
		this->playerInfo->SoTienDatCuocServer = *gtbv;
		this->playerInfo->SoTienDatCuocClient = *gtbv;
		this->DisplayInfoMoney(this->playerInfo->SoTienThuc, *gtbv);
	}

	if (gbce != NULL) {
		vector<string> arrays = mUtils::splitString(*gbce, ':');
		for (int i = 0; i < arrays.size(); ++i) {
			for (int j = 0; j < this->arrBet.size(); ++j){
				try {
					TaiXiuBet* pTaiXiu = this->arrBet.at(j);
					pTaiXiu->setIsAnimation(false);
					if (0 == arrays.at(i).compare(pTaiXiu->GetType())){
						pTaiXiu->CanEat();
						//animation tien thang.
						if (pTaiXiu->getIsAnimation()){
							this->processAnimationWin(this->iconMoney->getPosition(), pTaiXiu->GetWorldPos());
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

void LayerGameTaiXiu::event_EXT_FIELD_GAME_BET_EXCHANGE(){

}

void LayerGameTaiXiu::event_EXT_EVENT_GAME_INFO_NTF(){

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
		vector<string> arr = mUtils::splitString(*ghs, ';');
		this->listHistory.clear();
		for (int i = 0; i < arr.size(); ++i) {
			this->listHistory.push_back(arr.at(i));
		}
	}
}

void LayerGameTaiXiu::event_EXT_EVENT_GAME_HIS_NTF(){

}

void LayerGameTaiXiu::event_EXT_EVENT_NOT_ENOUGH_MONEY_NTF(){

}

void LayerGameTaiXiu::event_EXT_EVENT_GAME_STATE_NTF(){
	log("event_EXT_EVENT_GAME_STATE_NTF");
	boost::shared_ptr<long> gtime = this->param->GetInt(EXT_FIELD_GAME_TIME);
	boost::shared_ptr<long> gstate = this->param->GetInt(EXT_FIELD_GAME_PLAY_STATE);
	boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<string> gbex = this->param->GetUtfString(EXT_FIELD_GAME_BET_EXCHANGE);

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
		}
		else {
			this->CurrentGameState = true;
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::updateTimer),this,1, false);
			Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::UpdateHayDatcuoc),this,0.5, false);
		}
	}
}

void LayerGameTaiXiu::event_EXT_EVENT_GAME_BET_NTF(){
	boost::shared_ptr<string> uid = this->param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> gbv = this->param->GetUtfString(EXT_FIELD_GAME_BET_VALUE);
	boost::shared_ptr<string> gbt = this->param->GetUtfString(EXT_FIELD_GAME_BET_TYPE);
	boost::shared_ptr<long> gtbv = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE);
	boost::shared_ptr<long> gtbvc = this->param->GetInt(EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL);
	boost::shared_ptr<long> ttp = this->param->GetInt(EXT_FIELD_GAME_TOTAL_PLAYER);

	if (ttp != NULL) {
		string t = boost::to_string((int)*ttp);
		this->txtNumOfPlayers->setString(dataManager.GetSysString(19)+" " + t);
	}

	if (uid != NULL && gbv != NULL && gbt != NULL && gtbv != NULL && gtbvc != NULL) {
		// gbv: game bet value, gia tri vua bet.
		// gtbv : tong gia tri ma nguoi choi (IsMe) bet
		// gtbvc: tong gia tri o bet

		// Luu het vao queue, 5s moi cap nhat mot lan
		std::string _gbt = (*gbt);
		long playermebet = 0 == uid->compare(this->MyAI) ? this->GetMoneyBet(*gbv) : 0;
		this->mQueueTaiXiu->Push(_gbt, *gtbvc, playermebet);

		//Xác nhận lại việc đã đặt cược
		if (0 == uid->compare(this->MyAI)) {
			this->playerInfo->SoTienDatCuocServer = (float)(*gtbv);
			// Kiểm tra bet.
			bool chk = this->playerInfo->CheckBet(*gbt);
			if (!chk) {

			}
			else {

			}
		}
	}
}

void LayerGameTaiXiu::Update5sStep(float dt){
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
				imgCoin->setScale(SCALE_COIN_TAI_XIU);
				imgCoin->setPosition(Vec2(WIDTH_DESIGN / 2, 100));

				MoveTo* moveTo = MoveTo::create(DURATION_ANIMATE_COIN, p);

				Struct4Call1 __d;
				__d.bet = bet;
				__d.total = info->GetTotalBet();

				CallFuncN * callfun = CallFuncN::create(CC_CALLBACK_1(LayerGameTaiXiu::AddCoinCallFunc, this, __d));

				this->addChild(imgCoin);
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

void LayerGameTaiXiu::event_EXT_EVENT_GAME_BET_ERROR_NOTIFY(){
	boost::shared_ptr<long> errc = this->param->GetInt(EXT_FIELD_ERROR_CODE);
	if (errc != NULL) {
		log("bet errc = %ld", *errc);
		if (*errc == RESCODE_NOT_ENOUGH_MONEY) {
			this->Toast->ShowToast(dataManager.GetSysString(33));
		}
	}
}

void LayerGameTaiXiu::eventTouchBtnCoin(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == Widget::TouchEventType::ENDED)
	{
		/*LayerPopupRecharge* _child = LayerPopupRecharge::create();
		Size ssi = ChanUtils::getSizePos();
		_child->setPosition(Vec2(-ssi.width, -ssi.height));
		this->addChild(_child, 1500);*/
		int roomType = SceneManager::getSingleton().getCurrRoomType();
		if (roomType == 0){
			LayerPopupCuaHang *popupCuaHang = LayerPopupCuaHang::create();
			popupCuaHang->onBtnNapThe(NULL);
			this->addChild(popupCuaHang, 1500);
		}
		else{
			LayerPopupDoiMoc *popup = LayerPopupDoiMoc::create();
			popup->onBtnNapHao(NULL);
			this->addChild(popup);
		}

	}
}

string LayerGameTaiXiu::GetUrlXiNgau(const int& number) {
	string num = boost::to_string(number);
	num = "tx-" + num + "point.png";
	return num;
}

void LayerGameTaiXiu::UpdateHayDatcuoc(float dt)
{
	this->HayDatCuoc->setString(this->arrTextHayDatCuoc[this->CountSizeDatCuoc]);
	this->HayDatCuoc->setVisible(true);

	if (NULL != this->getChildByTag(TAG_ANIM_END_GAME)) {
		this->removeChildByTag(TAG_ANIM_END_GAME);
	}

	if (this->CountSizeDatCuoc < this->arrTextHayDatCuoc.size() - 1) {
		this->CountSizeDatCuoc++;
	}
	else {
		this->CountSizeDatCuoc = 0;
	}
}

void LayerGameTaiXiu::ReloadGameWhenReturn(string& gbt) {

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

void LayerGameTaiXiu::DisplayInfoMoney(const long& tienconlai, const long& dadatcuoc){
	//So tien con lai cua minh
	std::string m = mUtils::convertMoneyEx(tienconlai);
	if (tienconlai > 9000000000) {
		m = formatMoneySymbol((double)tienconlai);
	}
	else {
		m = mUtils::convertMoneyEx(tienconlai);
	}

	this->lblMoney->setString(m);

	//So tien da dat cuoc
	try {
		std::string m1 = mUtils::convertMoneyEx(dadatcuoc);
		this->lblBetMoney->setString(m1);
	}
	catch (...) {
		this->lblBetMoney->setString("0");
	}
}

void LayerGameTaiXiu::SetRoomConfig()
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
		std::string _bet = *activebets->GetStringValue();
		log("_bet %s", _bet.c_str());
		if (activebets != NULL) {
			auto arr = mUtils::splitString(*activebets->GetStringValue(), ',');
			for (auto i = 0; i < arr.size(); ++i) {
				this->ArrListValueBet.push_back(arr.at(i));
			}
		}
		else {
			for (auto i = 0; i < this->ArrListValueBetDefault.size(); ++i) {
				this->ArrListValueBet.push_back(this->ArrListValueBetDefault.at(i));
			}
		}

	}
}

void LayerGameTaiXiu::updateTimer(float dt)
{
	if (this->TimerCountDown > 0) {
		this->TimerCountDown--;

		string str = boost::to_string(this->TimerCountDown);
		this->txtTimer->setString(str);

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
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updateTimer),this);
		this->TimerCountDown = this->PlayTurnTime;
	}
}

void LayerGameTaiXiu::updateToNextTurn(float dt){
	if (this->TimerCountNextTime > 0){
		this->TimerCountNextTime--;
	}
	else
	{
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updateToNextTurn),this);
		this->lblCountNextTime->setVisible(false);
		this->TimerCountNextTime = this->BetweentwoPlayTime;
	}
	//string str = "Ván tiếp: " + boost::to_string(this->TimerCountNextTime);
	//this->lblCountNextTime->setString(str);
	this->lblCountNextTime->setVisible(false);
}

void LayerGameTaiXiu::ResetGame() {
	for (int i = 0; i < this->arrBet.size(); ++i){
		TaiXiuBet* pTaiXiu = this->arrBet.at(i);
		pTaiXiu->Reset();
	}

	if (this->getChildByTag(TAG_TAI_XIU_XO_BA0) != NULL) {
		this->removeChildByTag(TAG_TAI_XIU_XO_BA0);
	}

	this->Animating = false;
	this->playerInfo->reset();
	this->mQueueTaiXiu->RemoveAll();

	this->lblBetMoney->setString("0");
	this->lblWinMoney->setString("0");

	this->playerInfo->reset();
	this->initPlayerInfo();
	this->DisplayInfoMoney(this->playerInfo->SoTienThuc, this->playerInfo->SoTienDatCuocServer);

	if (this->CurrentBetValue == "" && this->playerInfo->SoTienThuc >= 1000) {
		this->SelectCoin(this->playerInfo->SoTienThuc);
	}

	this->rg = "";
	this->rt10 = "";
}

void LayerGameTaiXiu::initPlayerInfo() {
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf != NULL){
			boost::shared_ptr<UserVariable> amf = mySelf->GetVariable("amf");
			if (amf != NULL){
				this->playerInfo->initMoney((long)(*amf->GetDoubleValue()));
			}
		}
	}
	else{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf != NULL){
			boost::shared_ptr<UserVariable> amf = mySelf->GetVariable("amfs");
			if (amf != NULL){
				this->playerInfo->initMoney((long)(*amf->GetDoubleValue()));
			}
		}
	}
}

void LayerGameTaiXiu::UpdateTimeStartGame(){

	// Tat dem nguoc den van tiep theo
	this->lblCountNextTime->setVisible(false);
	string str = dataManager.GetSysString(700)+" " + boost::to_string(this->BetweentwoPlayTime);
	this->lblCountNextTime->setString(str);
	this->TimerCountNextTime = this->BetweentwoPlayTime;
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updateToNextTurn),this);

	// Dem timer thoi gian van choi
	this->txtTimer->setVisible(true);
	this->TimerCountDown = this->PlayTurnTime;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::updateTimer),this,1, false);
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::UpdateHayDatcuoc),this,0.5, false);
}

void LayerGameTaiXiu::UpdateTimeEndGame(){

	this->CurrentGameState = false;

	//Tat Hay Dat Cuoc
	this->HayDatCuoc->setVisible(false);
	this->CountSizeDatCuoc = 0;
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::UpdateHayDatcuoc),this);

	//Dung dem timer
	this->txtTimer->setVisible(false);
	string str = boost::to_string(this->PlayTurnTime);
	this->txtTimer->setString(str);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameTaiXiu::updateTimer),this);

	// Dem nguoc thoi gian den van tiep theo.
	this->lblCountNextTime->setVisible(false);
	this->TimerCountNextTime = this->BetweentwoPlayTime;
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameTaiXiu::updateToNextTurn),this,1, false);
}

void LayerGameTaiXiu::updateEvent(float dt){
	if (this->mQueueMsg->isQueueEmpty())
		return;

	if (this->Animating)
		return;

	this->mQueueMsg->run();
}

void LayerGameTaiXiu::CallBackResult(Ref *pSend){
	this->CallBackFromAnim();
}

void LayerGameTaiXiu::CallBackFromAnim(){
	this->Animating = false;
	if (0 != this->rg.compare("")) {

		this->listHistory.push_back(this->rg);
		if (this->listHistory.size() > 20) {
			//this->listHistory.splice(0, 1);
		}

		vector<string> arrRg = mUtils::splitString(this->rg, '_');
		try{
			string url = this->GetUrlXiNgau(atoi(arrRg.at(0).c_str()));
			this->resultDice1->initWithFile(url);
			this->resultDice1->setVisible(true);

			url = this->GetUrlXiNgau(atoi(arrRg.at(1).c_str()));
			this->resultDice2->initWithFile(url);
			this->resultDice2->setVisible(true);

			url = this->GetUrlXiNgau(atoi(arrRg.at(2).c_str()));
			this->resultDice3->initWithFile(url);
			this->resultDice3->setVisible(true);

			if (atoi(arrRg.at(0).c_str()) == atoi(arrRg.at(1).c_str()) &&
				atoi(arrRg.at(0).c_str()) == atoi(arrRg.at(2).c_str())) {
				Size visibleSize = Director::getInstance()->getVisibleSize();
				Sprite* xobao = Sprite::create("xobao.png");
				xobao->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				xobao->setTag(TAG_TAI_XIU_XO_BA0);
				this->addChild(xobao, 5001);
			}
		}
		catch (std::out_of_range& e){
			log("%s", e.what());
		}
		catch (std::exception& e){
			log("%s", e.what());
		}
	}

	if (this->rt10 != "") {

	}
}

void LayerGameTaiXiu::processAnimationWin(Vec2 _dest, Vec2 _pos)
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
		ScaleTo* scaleTo = ScaleTo::create(0.2 + _delay, 0.5);
		DelayTime* delayAction = DelayTime::create(_delay);
		Spawn* spawnAction = Spawn::create(moveTo, scaleTo, NULL);
		coinsArr.at(i)->runAction(Sequence::create(delayAction, spawnAction, callBack, NULL));
	}
}

void LayerGameTaiXiu::processButtonSetTouched(Ref *pObject, Widget::TouchEventType pType)
{
	if (pType == ui::Widget::TouchEventType::ENDED){
		boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (NULL == lstRoom)
			return;
		log("%s", this->tfResult->getString().c_str());
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		params->PutUtfString("rg", this->tfResult->getString());
		boost::shared_ptr<IRequest> req2(new ExtensionRequest("stv", params, lstRoom));
		GameServer::getSingleton().Send(req2);
	}
}
