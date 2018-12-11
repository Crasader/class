#include "LayerMainGame.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "gameUtils.h"
#include "../SceneManager.h"
#include "LayerPopupLose.h"
#include "LayerPopupWin.h"
#include "Requests/LeaveRoomRequest.h"
#include "SimpleAudioEngine.h"
#include "layergames/PhomMessDef.h"
#include "LayerCreateRoomChess.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "MyAnimation.h"
USING_NS_CC;

#define TIME_ALERT_CHESS 15
#define TOTAL_TIMER_ALERT_CHESS 45
// on "init" you need to initialize your instance

LayerMainGame::LayerMainGame()
{
	GameServer::getSingleton().addListeners(this);
	this->initVariables();
}

LayerMainGame::~LayerMainGame()
{
	if (mThreadQueueMsg != NULL) delete mThreadQueueMsg;
	GameServer::getSingleton().removeListeners(this);
}

bool LayerMainGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);

	sizeScreen = Director::getInstance()->getVisibleSize();
	float scaleX = sizeScreen.width / designResolutionSize.width;
	float scaleY = sizeScreen.height / designResolutionSize.height;
	scaleApp = (scaleX < scaleY) ? scaleX : scaleY;

	this->startLeft = (sizeScreen.width - designResolutionSize.width) / 2;
	this->startTop = (sizeScreen.height - designResolutionSize.height) / 2;


	auto rootNode = CSLoader::getInstance()->createNode("LayerMainGame.csb");
	rootNode->setContentSize(sizeScreen);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{

		this->initTableChess();
		this->initPanelUser();
		this->initUI();
		this->initPointInTable();
		this->resetGame();
	}

	return true;
}

void LayerMainGame::initUI()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//add background
	Sprite* bgBackground = Sprite::create("CoTuong/bg.jpg");
	bgBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bgBackground->setScale(visibleSize.width / bgBackground->getContentSize().width,
		visibleSize.height / bgBackground->getContentSize().height);
	bgBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	bgBackground->setScaleX(visibleSize.width / bgBackground->getContentSize().width);
	this->addChild(bgBackground, -1);

	//add layerBottom
	this->initBottomInGame();

	pnlChat = (Layout*)pnlMain->getChildByName("pnlChat");
	btnChat = (Button*)pnlMain->getChildByName("btnChat");
    btnChat->setVisible(false);
    pnlChat->setVisible(false);

	//init chat layer
	//btnChat->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonChat, this));
	//init chat layer
//    layerChat = LayerChat::create();
//    layerChat->setVisible(false);
//    this->addChild(layerChat);

	layerTimer = LayerPopupReady::create();
	layerTimer->setAnchorPoint(Vec2::ZERO);
	layerTimer->setPosition(Vec2::ZERO);
	pnlMain->addChild(layerTimer);
	layerTimer->hideTimer();

	//table chat
	// 	pnlChat->setTouchEnabled(false);
	// 	pnlChat->setSwallowTouches(true);

	tblChat = TableView::create(this, pnlChat->getContentSize());
	tblChat->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tblChat->setAnchorPoint(pnlChat->getAnchorPoint());
	tblChat->setPosition(pnlChat->getPosition());
	tblChat->setDelegate(this);
	tblChat->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	pnlMain->addChild(tblChat);
	tblChat->reloadData();

	popupKingCheck = LayerPopupKingCheck::create();
	this->addChild(popupKingCheck);
	popupKingCheck->reset();
}

void LayerMainGame::initVariables()
{
	this->setFirstConfig(true);
	this->currGameID = SceneManager::getSingleton().getGameID();
	this->myName = GameUtils::getMyName();
	this->nameCurrentTurn = "";
	//for logic game
	logicChess = new ChessLogic();
	logicChess->loadNewGame();
	/*
	int foldPos_[90] = {
	RED, RED, RED, RED, KING, RED, RED, RED, RED,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	EMPTY, RED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, RED, EMPTY,
	RED, EMPTY, RED, EMPTY, RED, EMPTY, RED, EMPTY, RED,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK,
	EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	BLACK, BLACK, BLACK, BLACK, KING, BLACK, BLACK, BLACK, BLACK
	};
	*/
	int foldPos_[90] = {
		BLACK, BLACK, BLACK, BLACK, KING, BLACK, BLACK, BLACK, BLACK,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY,
		BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		RED, EMPTY, RED, EMPTY, RED, EMPTY, RED, EMPTY, RED,
		EMPTY, RED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, RED, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		RED, RED, RED, RED, KING, RED, RED, RED, RED
	};

	if (this->currGameID == kGameCoTuong){
		for (int i = 0; i < 90; i++)
			foldPos_[i] = EMPTY;
	}
	else{
		logicChess->setChessIsFold(true);
	}
	memcpy(foldPos, foldPos_, 90 * sizeof(int));

	mThreadQueueMsg = new ThreadQueueMsg();
	mThreadQueueMsg->setFunCallBack(this, callfunc_selector(LayerMainGame::OnExtensionResponseProcess));
	this->schedule(schedule_selector(LayerMainGame::updateEvent));

	boost::shared_ptr<Room> lastJoinRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastJoinRoom != NULL)
	{
		boost::shared_ptr<RoomVariable> rv = lastJoinRoom->GetVariable("params");
		if (rv != NULL){
			string s = *rv->GetStringValue();
			log("Room %s update RoomVariables: %s", lastJoinRoom->Name()->c_str(), s.c_str());

		}
	}
}

void LayerMainGame::initPanelUser()

{
	//init 2 layer user info
	Node* nodeUser1 = pnlMain->getChildByName("nodeUser1");
	Node* nodeUser2 = pnlMain->getChildByName("nodeUser2");

	LayerUserPlayInfo* layerUser2 = LayerUserPlayInfo::create();
	layerUser2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	layerUser2->setPosition(nodeUser1->getPosition());
	pnlMain->addChild(layerUser2);
	//layerUser2->reversePos();

	LayerUserPlayInfo* layerUser1 = LayerUserPlayInfo::create();
	layerUser1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	layerUser1->setPosition(nodeUser2->getPosition());
	pnlMain->addChild(layerUser1);

	listUserInfoLayer.push_back(layerUser1);
	listUserInfoLayer.push_back(layerUser2);
	//other 
	lblRoomName = (Text*)spTable->getChildByName("lblRoomName");
	lblBetValue = (Text*)pnlMain->getChildByName("lblBetValue");
	lblTime1 = (Text*)pnlMain->getChildByName("lblTime1");
	lblTime2 = (Text*)pnlMain->getChildByName("lblTime2");

	lblRoomName->setString(GameUtils::getRoomNameForChess());
    lblRoomName->setVisible(false);
	lblBetValue->setString(formatMoneySymbol(GameUtils::getRoomBet()));

	popupVisitor = LayerPopupVisitor::create();
	popupVisitor->setVisible(false);
	this->addChild(popupVisitor);

	this->timeSprite1 = (Sprite*)pnlMain->getChildByName("timeSprite1");
	this->timeSprite2 = (Sprite*)pnlMain->getChildByName("timeSprite2");
	this->timeSpriteTurn1 = (Sprite*)pnlMain->getChildByName("timeSpriteTurn1");
	this->timeSpriteTurn2 = (Sprite*)pnlMain->getChildByName("timeSpriteTurn2");
	this->Sprite_13 = (Sprite*)pnlMain->getChildByName("Sprite_13");
	int roomType = SceneManager::getSingleton().getCurrRoomType();
	string pathImg = "bt_tien3.png";
	if (roomType == 0)//gold
	{
		pathImg = "bt_tien4.png";
	}
	this->Sprite_13->setTexture(pathImg);
}

void LayerMainGame::initTableChess()
{
	spTable = (Sprite*)pnlMain->getChildByName("spTable");
	if (spTable != NULL)
	{
		prevPoint = Sprite::create("dau-luu-nuoc-di.png");
		prevPoint->setPosition(Vec2(spTable->getContentSize().width / 2, spTable->getContentSize().height / 2));
		prevPoint->setVisible(false);
		spTable->addChild(prevPoint);

		// 		justPoint = Sprite::create("vong-luu-nuoc-di.png");
		// 		justPoint->setPosition(Vec2(spTable->getContentSize().width / 2, spTable->getContentSize().height / 2));
		// 		justPoint->setScale(1.18);
		// 		justPoint->setVisible(false);
		// 		spTable->addChild(justPoint);

		//den duoi - do tren
		CHESS_TYPE typeChess1 = CHESS_TYPE::SIMPLE;
		CHESS_TYPE typeChess2 = CHESS_TYPE::SIMPLE;
		// quan den
		Chess *xe_black_1 = Chess::create(BLACK, ROOK, 0, typeChess1);
		Chess *ma_black_1 = Chess::create(BLACK, KNIGHT, 1, typeChess1);
		Chess *tuongj_black_1 = Chess::create(BLACK, ELEPHANT, 2, typeChess1);
		Chess *si_black_1 = Chess::create(BLACK, BISHOP, 3, typeChess1);
		Chess *tuong_black = Chess::create(BLACK, KING, 4, typeChess1);
		Chess *si_black_2 = Chess::create(BLACK, BISHOP, 5, typeChess1);
		Chess *tuongj_black_2 = Chess::create(BLACK, ELEPHANT, 6, typeChess1);
		Chess *ma_black_2 = Chess::create(BLACK, KNIGHT, 7, typeChess1);
		Chess *xe_black_2 = Chess::create(BLACK, ROOK, 8, typeChess1);
		Chess *phao_black_1 = Chess::create(BLACK, CANNON, 19, typeChess1);
		Chess *phao_black_2 = Chess::create(BLACK, CANNON, 25, typeChess1);
		Chess *tot_black_1 = Chess::create(BLACK, PAWN, 27, typeChess1);
		Chess *tot_black_2 = Chess::create(BLACK, PAWN, 29, typeChess1);
		Chess *tot_black_3 = Chess::create(BLACK, PAWN, 31, typeChess1);
		Chess *tot_black_4 = Chess::create(BLACK, PAWN, 33, typeChess1);
		Chess *tot_black_5 = Chess::create(BLACK, PAWN, 35, typeChess1);

		//quan do

		Chess *xe_red_1 = Chess::create(RED, ROOK, 81, typeChess2);
		Chess *ma_red_1 = Chess::create(RED, KNIGHT, 82, typeChess2);
		Chess *tuongj_red_1 = Chess::create(RED, ELEPHANT, 83, typeChess2);
		Chess *si_red_1 = Chess::create(RED, BISHOP, 84, typeChess2);
		Chess *tuong_red = Chess::create(RED, KING, 85, typeChess2);
		Chess *si_red_2 = Chess::create(RED, BISHOP, 86, typeChess2);
		Chess *tuongj_red_2 = Chess::create(RED, ELEPHANT, 87, typeChess2);
		Chess *ma_red_2 = Chess::create(RED, KNIGHT, 88, typeChess2);
		Chess *xe_red_2 = Chess::create(RED, ROOK, 89, typeChess2);
		Chess *phao_red_1 = Chess::create(RED, CANNON, 64, typeChess2);
		Chess *phao_red_2 = Chess::create(RED, CANNON, 70, typeChess2);
		Chess *tot_red_1 = Chess::create(RED, PAWN, 54, typeChess2);
		Chess *tot_red_2 = Chess::create(RED, PAWN, 56, typeChess2);
		Chess *tot_red_3 = Chess::create(RED, PAWN, 58, typeChess2);
		Chess *tot_red_4 = Chess::create(RED, PAWN, 60, typeChess2);
		Chess *tot_red_5 = Chess::create(RED, PAWN, 62, typeChess2);

		//
		int jumUnit = 100;

		xe_red_1->setPosConfig(1 + jumUnit);
		xe_red_2->setPosConfig(9 + jumUnit);

		ma_red_1->setPosConfig(2 + jumUnit);
		ma_red_2->setPosConfig(8 + jumUnit);

		tuongj_red_1->setPosConfig(3 + jumUnit);
		tuongj_red_2->setPosConfig(7 + jumUnit);

		si_red_1->setPosConfig(4 + jumUnit);
		si_red_2->setPosConfig(6 + jumUnit);
		tuong_red->setPosConfig(5 + jumUnit);

		phao_red_1->setPosConfig(2 + jumUnit);
		phao_red_2->setPosConfig(8 + jumUnit);

		tot_red_1->setPosConfig(1 + jumUnit);
		tot_red_2->setPosConfig(3 + jumUnit);
		tot_red_3->setPosConfig(5 + jumUnit);
		tot_red_4->setPosConfig(7 + jumUnit);
		tot_red_5->setPosConfig(9 + jumUnit);

		//black nguoc lai
		xe_black_1->setPosConfig(9);
		xe_black_2->setPosConfig(1);

		ma_black_1->setPosConfig(8);
		ma_black_2->setPosConfig(2);

		tuongj_black_1->setPosConfig(7);
		tuongj_black_2->setPosConfig(3);

		si_black_1->setPosConfig(6);
		si_black_2->setPosConfig(4);
		tuong_black->setPosConfig(5);
		phao_black_1->setPosConfig(8);
		phao_black_2->setPosConfig(2);

		tot_black_1->setPosConfig(9);
		tot_black_2->setPosConfig(7);
		tot_black_3->setPosConfig(5);
		tot_black_4->setPosConfig(3);
		tot_black_5->setPosConfig(1);

		//
		arrChess.push_back(xe_black_1);
		arrChess.push_back(ma_black_1);
		arrChess.push_back(tuongj_black_1);
		arrChess.push_back(si_black_1);
		arrChess.push_back(tuong_black);
		arrChess.push_back(si_black_2);
		arrChess.push_back(tuongj_black_2);
		arrChess.push_back(ma_black_2);
		arrChess.push_back(xe_black_2);
		arrChess.push_back(phao_black_1);
		arrChess.push_back(phao_black_2);
		arrChess.push_back(tot_black_1);
		arrChess.push_back(tot_black_2);
		arrChess.push_back(tot_black_3);
		arrChess.push_back(tot_black_4);
		arrChess.push_back(tot_black_5);
		//
		arrChess.push_back(xe_red_1);
		arrChess.push_back(ma_red_1);
		arrChess.push_back(tuongj_red_1);
		arrChess.push_back(si_red_1);
		arrChess.push_back(tuong_red);
		arrChess.push_back(si_red_2);
		arrChess.push_back(tuongj_red_2);
		arrChess.push_back(ma_red_2);
		arrChess.push_back(xe_red_2);
		arrChess.push_back(phao_red_1);
		arrChess.push_back(phao_red_2);
		arrChess.push_back(tot_red_1);
		arrChess.push_back(tot_red_2);
		arrChess.push_back(tot_red_3);
		arrChess.push_back(tot_red_4);
		arrChess.push_back(tot_red_5);


		int rawDatas[] = { ROOK, KNIGHT, ELEPHANT, BISHOP, KING, BISHOP, ELEPHANT, KNIGHT, ROOK, CANNON, CANNON, PAWN, PAWN, PAWN, PAWN, PAWN
			, ROOK, KNIGHT, ELEPHANT, BISHOP, KING, BISHOP, ELEPHANT, KNIGHT, ROOK, CANNON, CANNON, PAWN, PAWN, PAWN, PAWN, PAWN };

		for (size_t i = 0; i < arrChess.size(); i++) {
			//Insert raw name of chess
			arrChess.at(i)->setRawName(FOLD);
			spTable->addChild(arrChess.at(i));
		}
		this->refreshChess();
	}
}

void LayerMainGame::initPointInTable()
{
	//start index tag from 220
	for (int i = 0; i < 90; i++)
	{
		Node* nodeChild = spTable->getChildByTag(2373 + i);
		if (nodeChild != NULL)
		{
			nodeChild->setLocalZOrder(11);
			Sprite* iconPoint = Sprite::create("goi-y-nuoc-di.png");
			iconPoint->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			iconPoint->setPosition(Vec2(nodeChild->getContentSize().width / 2 + 3,
				nodeChild->getContentSize().height / 2));
			iconPoint->setTag(i);
			nodeChild->addChild(iconPoint);
		}
	}
}


void LayerMainGame::refreshChess()
{
	// Đặt lại vị trí của quân cờ theo đúng id_pos
	for (int i = 0; i < arrChess.size(); i++) {
		Chess *chess = arrChess.at(i);
		chess->setPosition(getPositionFromIndex(chess->getIDPosInit()));
		chess->setIDPos(chess->getIDPosInit());
		//chess->setLocalZOrder(0);
		chess->setVisible(true);
		chess->setClick(false);
		chess->setFakeType(-1);
		chess->setOpacity(255);
		chess->showKingCheck(false, Vec2::ZERO);
		chess->showIsChecked(false);
		chess->hideShadow(false);
		chess->hideJustPoint(true);
		arrChess.at(i)->addMoveSprite();
	}
	//update 21/7
	this->logicChess->loadNewGame();
}

cocos2d::Vec2 LayerMainGame::getPositionFromIndex(int id)
{
	//start index tag from 220
	Node* nodeChild = spTable->getChildByTag(2373 + id);
	if (!nodeChild){
		return Vec2(0, 0);
	}
	return Vec2(nodeChild->getPosition().x, nodeChild->getPosition().y);
}

void LayerMainGame::onEnter()
{
	Layer::onEnter();
	auto _listener = EventListenerTouchOneByOne::create();
	_listener->setSwallowTouches(false);
	_listener->onTouchBegan = CC_CALLBACK_2(LayerMainGame::onTouchBegan, this);
	_listener->onTouchMoved = CC_CALLBACK_2(LayerMainGame::onTouchMoved, this);
	_listener->onTouchEnded = CC_CALLBACK_2(LayerMainGame::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, spTable);
}

void LayerMainGame::onExit()
{
	Layer::onExit();
	delete logicChess;
	_eventDispatcher->removeEventListenersForTarget(spTable, true);
}

bool LayerMainGame::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());

	Vec2 locationInNode = target->convertToNodeSpace(pTouch->getLocation());
	Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
	{
		//		log("onTouchBegan");
		return true;
	}
	return false;
}
void LayerMainGame::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if ((0 != nameCurrentTurn.compare(myName) &&
		isStartedGame && !isSpectator)
		|| !isYourTurn
		|| isSpectator
		|| !isStartedGame)
	{
		return;
	}

	auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
	Vec2 locationInNode = target->convertToNodeSpace(pTouch->getLocation());
	int index = getIndexFromPosition(locationInNode);

	//this->hideCurrentPoint();
	vector<int> ar;
	drawCanMove(ar);

	if (index > -1) {
		// kiểm tra xem có phải đã click vào 1 trong các quân cờ của mình hay ko
		// nếu có thì gán cho indexCurrent
		bool isClick = false;

		for (int i = 0; i < 16; i++) {
			Chess *chess = arrChess.at(i);
			if (chess->getIDPos() == index && chess->isVisible()) {
				indexCurrent = index;
				chess->setClick(true);
				//this->showCurrentPoint(indexCurrent);
				// goi y nuoc di
				vector<int> arrID = logicChess->getAllMovesFromPos(convertIDByRow(indexCurrent),
					foldPos[convertIDByRow(indexCurrent)] != EMPTY);
				vector<int> arrIDConvert;
				for (int j = 0; j < arrID.size(); j++) {
					int id = arrID.at(j);
					if (id < 0)
						break;
					arrIDConvert.push_back(convertIDByRow(id));
				}
				drawCanMove(arrIDConvert);

				isClick = true;
			}
			else
				chess->setClick(false);
		} // end for (int i = 0; i < 16; i++)

		log("INDEX = %d ", index);
		//check chap tien
		if (countChapTien > 0 && index > 45)
		{
			SceneManager::getSingleton().showToast("Nước đi không hợp lệ do phạm luật chấp tiên");
			return;
		}

		if (!isClick && this->indexCurrent > -1) {
			// gửi lên sv
			this->indexTarget = index;
			int fct = -1;
			Chess *chess = getChessByID(this->indexCurrent);
			if (chess != NULL)
			if (chess->getName() == FOLD)
				fct = chess->getFakeType();
			log("FCT CLIENT = %d", fct);
			this->indexTarget = convertIDByRow(this->indexTarget);
			// Check lai nuoc di
			vector<int> arrID = logicChess->getAllMovesFromPos(convertIDByRow(this->indexCurrent), foldPos[convertIDByRow(this->indexCurrent)] != EMPTY);
			vector<int> arrIDConvert;
			bool isCheck = false;
			for (size_t j = 0; j < arrID.size(); j++) {
				if (arrID[j] == this->indexTarget){
					isCheck = true; break;
				}
			}
			this->indexCurrent = this->convertIDByRow(this->indexCurrent);

			if (isCheck && this->indexCurrent > -1 && this->indexTarget > -1){
				//thuc hien animation truoc gui len sever sau:chuyen turn danh luon tranh truong hop danh 2, 3 nuoc duoc
				isYourTurn = false;
				indexCurrentHis = this->indexCurrent;
				indexTargetHis = this->indexTarget;
				this->moveChess(this->convertIDByRow(this->indexCurrent), this->convertIDByRow(this->indexTarget), true, fct);

				if (countChapTien > 0)
				{
					countChapTien--;
				}
				boost::shared_ptr<ISFSObject> params(new SFSObject());
				params->PutInt(EXT_FIELD_MOVEFROM, indexCurrent);
				params->PutInt(EXT_FIELD_MOVETO, indexTarget);
				boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
				if (lastRoom != NULL){
					boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_MOVE, params, lastRoom));
					GameServer::getSingleton().Send(request);
					vector<int> ar;
					drawCanMove(ar);
				}
			}
			// reset
			indexCurrent = -1;
			indexTarget = -1;
		}

	}
}

int LayerMainGame::getIndexFromPosition(Vec2 pos)
{
	int w_chess = 101;
	for (int i = 0; i < 90; i++) {
		Point point = getPositionFromIndex(i);
		if (getDistant2Point(pos, point) < w_chess / 2){
			return i;
		}
	}

	return -1;
}

void LayerMainGame::onButtonChat(Ref* pSender)
{
	// 	this->createLayerNotification(dataManager.GetSysString(270), dataManager.GetSysString(271), TAG_CALLBACK::TAG_LEAVE_GAME);
	// 	return;

//    layerChat->setVisible(true);
//    layerChat->showChat();
	//SceneManager::getSingleton().showPopupMoneyCharge();
	// 	CHESS_TYPE typeChess2 = CHESS_TYPE::GOLD;
	// 	for (unsigned int i = 16; i < arrChess.size(); i++)
	// 	{
	// 		Chess* _chess = arrChess.at(i);
	// 		if (_chess)
	// 		{
	// 			_chess->setImageChess(typeChess2);
	// 		}
	// 	}
	//popupKingCheck->showKingCheck(nameCurrentTurn);
}

void LayerMainGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> pObjs = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	pObjs->PutUtfString("_cmd_", *cmd);
	//only add to queues
	mThreadQueueMsg->addQueue(pObjs);
}

void LayerMainGame::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
	//
	if (ptrNotifiedUser == NULL || ptrNotifiedMessage == NULL) return;
	//log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	//them loc chat

	if (ptrNotifiedUser->IsSpectator() && SceneManager::getSingleton().getFilterChat() == true)//chan khong xem chat cua thang khach
		return;

	string name = ptrNotifiedUser->Name()->c_str();
	if (listStore.size() > MAX_CHAT_LENGTH)
		listStore.pop_back();
	listStore.push_back(ChatInfoStruct(name, ptrNotifiedMessage->c_str()));
	tblChat->reloadData();
	tblChat->setContentOffset(Vec2::ZERO);
	//layerChat->setListChat(name, ptrNotifiedMessage->c_str());
}

void LayerMainGame::OnExtensionResponseProcess()
{
	this->param = mThreadQueueMsg->deQueue();
	boost::shared_ptr<string> cmd = this->param->GetUtfString("_cmd_");
	log("Command MainGame = %s", cmd->c_str());
	if (0 == cmd->compare(EXT_HEATBEAT)){
		return;
	}
	else if (0 == cmd->compare(EXT_EVENT_LIST_USER_UPDATE))
	{
		this->event_EXT_EVENT_LIST_USER_UPDATE();
	}
	else if (0 == cmd->compare(EXT_EVENT_START))
	{
		this->event_EXT_EVENT_START();
	}
	else if (0 == cmd->compare(EXT_EVENT_NEXT_TURN))
	{
		this->event_EXT_EVENT_NEXT_TURN();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY))
	{
		this->event_EXT_EVENT_NEXT_TURN();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHESS_TABLE_NTF))
	{
		this->event_EXT_EVENT_CHESS_TABLE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_RESULT))
	{
		this->event_EXT_EVENT_GAME_RESULT();
	}
	else if (0 == cmd->compare(EXT_EVENT_READY_NTF))
	{
		this->event_EXT_EVENT_READY_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_MOVE_NTF))
	{
		this->event_EXT_EVENT_MOVE_NTF();
	}
	else if (0 == cmd->compare(EXT_EVENT_MOVE_ERROR))
	{
		this->event_EXT_EVENT_MOVE_ERROR();
	}
	else if (0 == cmd->compare(EXT_EVENT_START_READY_TIMER_NOTIF))
	{
		this->event_EXT_EVENT_START_READY_TIMER_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_AUTO_KICK))
	{
		this->event_EXT_EVENT_AUTOKICK();
	}
	else if (0 == cmd->compare(EXT_EVENT_USER_INFO))
	{
		this->event_EXT_EVENT_USER_INFO();
	}
	else if (0 == cmd->compare(EXT_EVENT_END))
	{
		this->event_EXT_EVENT_END();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_SETTING_RESPONSE))
	{
		this->event_EXT_EVENT_CHANGE_SETTING();
	}
	else if (0 == cmd->compare(EXT_EVENT_CHANGE_SETTING_NOTIF))
	{
		this->event_EXT_EVENT_CHANGE_SETTING_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_PRE_CHANGE_SETTING_NOTIF))
	{
		this->event_EXT_EVENT_PRE_CHANGE_SETTING_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_WARNING_NOTIF))
	{
		this->event_EXT_EVENT_WARNING_NOTIF();
	}
	else if (0 == cmd->compare(EXT_EVENT_NOT_ENOUGH_MONEY))
	{
		this->event_EXT_EVENT_NOT_ENOUGH_MONEY();
	}
	else if (0 == cmd->compare(EXT_EVENT_GAME_LEAVE_RES))
	{
		this->event_EXT_EVENT_GAME_LEAVE_RES(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_FOR_PEACE_NTF))
	{
		this->event_EXT_EVENT_FOR_PEACE_NTF(param);
	}
	else if (0 == cmd->compare(EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE))
	{
		this->event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(param);
	}
}

void LayerMainGame::updateEvent(float dt)
{
	if (mThreadQueueMsg->isQueueEmpty())
		return;
	mThreadQueueMsg->run();
}

void LayerMainGame::event_EXT_EVENT_LIST_USER_UPDATE()
{
	//px = 0x07dce7b8 "tester16|r|0.00;"
	boost::shared_ptr<string>  listPtr = this->param->GetUtfString(EXT_FIELD_LIST_USER);
	if (listPtr == NULL) return;
	//reset listUser first
	//update 17/7
	this->enemyName = "";

	for (ssize_t i = 0; i < listUserInfoLayer.size(); i++)
	{
		listUserInfoLayer.at(i)->resetInfo();
	}

	log("LIST User = %s", listPtr->c_str());
	this->listUserPlay = listPtr->c_str();
	isSpectator = this->checkSpectator();
	isStartedGame = GameUtils::isStartedGame();
	//check spectator 
	if (isSpectator)
	{
		this->btnSpec->setTitleText("Ngồi chơi");
	}
	else{
		this->btnSpec->setTitleText("Ngồi xem");
	}

	bool isBossRoom = GameUtils::isBossRoom(listPtr->c_str());//check xem co phai chu phong hay khong, neu khong phai thi phai reverse lai
	// 2 layer
	// 	if (isBossRoom)
	// 		isShowSetting = true;
	// 	else
	// 		isShowSetting = false;
	this->showSpectatorMode(isSpectator);
	//update 25/7
	this->checkShowSetting();

	if (isBossRoom || isSpectator){
		listUserInfoLayer.at(0)->setBossRoom(true);
		listUserInfoLayer.at(1)->setBossRoom(false);
	}
	else{
		listUserInfoLayer.at(0)->setBossRoom(false);
		listUserInfoLayer.at(1)->setBossRoom(true);
	}

	vector<string> listUser = mUtils::splitString(*listPtr, ';');
	if (isBossRoom || isSpectator)
	{
		for (unsigned int i = 0; i < listUser.size(); i++)
		{
			vector<string> listInfo = mUtils::splitString(listUser.at(i), '|');
			if (listInfo.size() >= 3)
			{
				string _name = listInfo.at(0);
				LayerUserPlayInfo* layerInfo = listUserInfoLayer.at(i);
				layerInfo->setInfoUser(_name);
				if (i == 0)
					player1 = _name;
				else
					player2 = _name;
				if (this->myName.compare(_name) != 0)
					this->enemyName = _name;
			}

		}
		//update 21/7
		this->bossName = player1;
	}
	else{
		std::reverse(listUser.begin(), listUser.end());
		for (unsigned int i = 0; i < listUser.size(); i++)
		{
			vector<string> listInfo = mUtils::splitString(listUser.at(i), '|');
			if (listInfo.size() >= 3)
			{
				string _name = listInfo.at(0);
				LayerUserPlayInfo* layerInfo = listUserInfoLayer.at(i);
				layerInfo->setInfoUser(_name);
				if (i == 0)
					player1 = _name;
				else
					player2 = _name;
			}

		}

		//update 25/7
		this->bossName = player2;
	}


	numUser = listUser.size();
	if (listUser.size() < 2)
	{
		player2 = "";
		this->isFirst = false;
		this->resetGame();
	}
	else{
		//update 25/7
		if (this->countGame == 0)
			this->setFirstConfig(true);
		// 		if (this->countGame == 0)
		// 			this->isFirst = true;
		// 		if (this->isFirst)
		// 		{
		// 			if (isBossRoom && !this->isStartedGame && this->countGame == 0)//phai la chu phong va chua bat dau game
		// 			{
		// 				boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 				boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 				if (lastRoom != NULL){
		// 					log("send request config");
		// 					boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PRE_CHANGE_SETTING_REQUEST, params, lastRoom));
		// 					GameServer::getSingleton().Send(request);
		// 				}
		// 			}
		// 		}
	}

	if (isSpectator) {
		// đảo lại player1 và player2 để đáp ứng NTF_TABLE
		//update 17/7
		string tg = player2;
		player2 = player1;
		player1 = tg;
		if (listUser.size() < 2)
		{
			//update 10/7
			double amfUser = GameUtils::getAmfGoldUser();
			if (SceneManager::getSingleton().getCurrRoomType() == 1)
				amfUser = GameUtils::getAmfSilverUser(this->myName);
			if (this->betValue > amfUser)
			{
				SceneManager::getSingleton().showToast("Không đủ tiền vào bàn");

			}
			else{
				popupVisitor->setVisible(true);
			}


		}
		else{
			popupVisitor->setVisible(false);
		}
	}

	// nếu tên mình đứng trước thì đảo lại, vì mình luôn nằm ở dưới bàn cờ
	if (strcmp(player1.c_str(), myName.c_str()) == 0) {
		player1 = player2;
		player2 = myName;
	}
	this->loadChessTableByType();
	//update 25/7
	this->initPanelPos();
}

void LayerMainGame::event_EXT_EVENT_NOT_ENOUGH_MONEY()
{
	// 	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);
	// 	if (uid != NULL)
	// 	{
	// 		if (strcmp(uid->c_str(), myName.c_str()) == 0){
	//khong du tien hien popup nap tien
	SceneManager::getSingleton().showToast("Bạn không đủ tiền để tiếp tục ván đấu");
	SceneManager::getSingleton().showPopupMoneyCharge();
	//}
	//	}
}

void LayerMainGame::event_EXT_EVENT_START()
{
	layerTimer->hideTimer();
	this->resetGame();
	this->schedule(schedule_selector(LayerMainGame::updateTimer), 1);
	isStartedGame = true;
	this->playSound(4);
	popupVisitor->setVisible(false);
}

void LayerMainGame::event_EXT_EVENT_NEXT_TURN()
{
	//update 20/7
	if (this->isStartedGame == false)
		return;

	listUserInfoLayer.at(0)->showAlertAnim(false);
	listUserInfoLayer.at(1)->showAlertAnim(false);

	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> list_time = param->GetUtfString(EXT_FIELD_CURRENT_TIME);
	boost::shared_ptr<long> tft = param->GetInt("tft");
	//event_EXT_EVENT_NEXT_TURN:: name=tester17, list_time=tester17|900;tester16|900, turn_time=90
	if (name != NULL && list_time != NULL && tft != NULL) {
		log("event_EXT_EVENT_NEXT_TURN:: name=%s, list_time=%s, turn_time=%d", name->c_str(), list_time->c_str(), *tft);

		isStartedGame = true;
		if (!isSpectator){
			// vào lại bàn chơi
			//btnReady->setVisible(false);
		}

		nameCurrentTurn = name->c_str();
		if (0 == nameCurrentTurn.compare(myName))//it's my turn
			isYourTurn = true;


		if (strcmp(nameCurrentTurn.c_str(), player2.c_str()) == 0){ /// đến lượt người chơi thứ 2 - chinh la minh
			if (!isSpectator) {
				SceneManager::getSingleton().showToast(dataManager.GetSysString(267));
				isYourTurn = true;
			}
			//update 25/7
			timeForTurnBlack = *tft;
		}
		else if (strcmp(nameCurrentTurn.c_str(), player1.c_str()) == 0){ /// đến lượt người chơi thứ 1 - ở dưới
			//update 25/7
			timeForTurnRed = *tft;
		}

		for (size_t i = 0; i < listUserInfoLayer.size(); i++)
		{
			string _userName = listUserInfoLayer.at(i)->getMyName();
			if (0 == _userName.compare(nameCurrentTurn))
			{
				//update 25/7
				this->listUserInfoLayer[i]->resumeTimer(this->totalTurnTime, *tft);
			}
			else{
				listUserInfoLayer.at(i)->stopTimer();
			}
		}

		// ptich chuoi thanhhv|900;thanhhv2|900;
		string lsTime = list_time->c_str();
		if (lsTime.length() < 2) return;
		vector<string> arrLsTime = mUtils::splitString(lsTime, ';');

		// Player 1
		vector<string> arrInfo_1 = mUtils::splitString(arrLsTime[0], '|');
		string name = arrInfo_1[0];
		string totalTime = arrInfo_1[1];
		if (strcmp(name.c_str(), player2.c_str()) == 0) {
			timeRestBlack = std::atoi(totalTime.c_str());
		}
		else if (strcmp(name.c_str(), player1.c_str()) == 0)
			timeRestRed = std::atoi(totalTime.c_str());

		// Player 2
		vector<string> arrInfo_2 = mUtils::splitString(arrLsTime[1], '|');
		name = arrInfo_2[0];
		totalTime = arrInfo_2[1];
		if (strcmp(name.c_str(), player2.c_str()) == 0) {
			timeRestBlack = std::atoi(totalTime.c_str());
		}
		else if (strcmp(name.c_str(), player1.c_str()) == 0)
			timeRestRed = std::atoi(totalTime.c_str());

		//update 25/7
		this->lblTime1->setString(GameUtils::convertTimer(this->timeRestRed).c_str());
		this->lblTime2->setString(GameUtils::convertTimer(this->timeRestBlack).c_str());
	}
}

void LayerMainGame::event_EXT_EVENT_END()
{
	log("event_EXT_EVENT_END");
	this->resetGame();
	//this->scheduleOnce(schedule_selector(LayerPlayGameChinessChess::resetGame), TIME_RESET_GAME);
	logicChess->loadNewGame();
	int foldPos_[90] = {
		BLACK, BLACK, BLACK, BLACK, KING, BLACK, BLACK, BLACK, BLACK,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY,
		BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		RED, EMPTY, RED, EMPTY, RED, EMPTY, RED, EMPTY, RED,
		EMPTY, RED, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, RED, EMPTY,
		EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
		RED, RED, RED, RED, KING, RED, RED, RED, RED
	};
	memcpy(foldPos, foldPos_, 90 * sizeof(int));

	nameCurrentTurn = "";
	isStartedGame = false;

	// reset
	indexCurrent = -1;
	indexTarget = -1;
	//update 10/7
	this->listStore.clear();
	this->tblChat->reloadData();
	//update 20/7
	this->isStartedGame = false;
}

void LayerMainGame::event_EXT_EVENT_READY_NTF()
{
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	if (name == NULL) return;
	log("event_EXT_EVENT_READY_NTF:: name=%s", name->c_str());

	if (strcmp(name->c_str(), player2.c_str()) == 0){
	}
	else if (strcmp(name->c_str(), player1.c_str()) == 0)
	{

	}
	if (strcmp(name->c_str(), myName.c_str()) == 0)
	{
		// 		layerTimer->stopTimer();
		// 		btnReady->setVisible(false);
	}
}

void LayerMainGame::event_EXT_EVENT_GAME_RESULT()
{
	boost::shared_ptr<string> name = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<string> result = param->GetUtfString(EXT_FIELD_RESULT_GAME);
	boost::shared_ptr<long> wt = param->GetInt("wt");
	//btnBack->setEnabled(true);
	if (name != NULL && result != NULL && wt != NULL) {
		//name=thanhhv2, result=-0|0 - name la thang thua
		string nameLoose = name->c_str();
		string resultStr = result->c_str();
		int winType = *wt;
		log("event_EXT_EVENT_GAME_RESULT:: name=%s, result=%s, wt=%d", name->c_str(), result->c_str(), *wt);
		this->unschedule(schedule_selector(LayerMainGame::updateTimer));

		popupKingCheck->reset();
		this->showTurn(3);
		for (ssize_t i = 0; i < listUserInfoLayer.size(); i++)
		{
			listUserInfoLayer.at(i)->stopTimer();
		}
		if (isSpectator) {
			log("You are spectator");
			return;
		}
		//hide popup king check
		if (0 == nameLoose.compare(GameUtils::getMyName()) && winType != 1)
		{
			LayerPopupLose* loseLayer = LayerPopupLose::create();
			pnlMain->addChild(loseLayer);
			this->playSound(0);
			return;
		}

		// 			// reason for result
		switch (winType) {
		case 1:
			// hòa
		{
				  LayerPopupPeace* loseLayer = LayerPopupPeace::create();
				  pnlMain->addChild(loseLayer);

		}
			break;
		case 0:
			// thắng bình thường
		case 2:
			// thắng do người kia thoát game
		case 3:
			// thắng vì 1 người kia hết time
		case 4:
			// thắng vì người kia xin thua
		case 5://WIN_TYPE_CHAP_HOA = 5;
		{
				   LayerPopupWin* loseLayer = LayerPopupWin::create();
				   pnlMain->addChild(loseLayer);
				   this->playSound(1);
		}
			break;
		}
	}
}

void LayerMainGame::event_EXT_EVENT_ERROR_READY_NTF()
{

}

void LayerMainGame::event_EXT_EVENT_MOVE_NTF()
{
	//update 20/7
	if (this->isStartedGame == false)
		return;

	boost::shared_ptr<long> mf = param->GetInt(EXT_FIELD_MOVEFROM);
	boost::shared_ptr<long> mt = param->GetInt(EXT_FIELD_MOVETO);
	boost::shared_ptr<long> fct = param->GetInt(EXT_FIELD_FROM_CHESS_TYPE);//EXT_FIELD_FROM_CHESS_TYPE ="fct";
	boost::shared_ptr<string> uid = param->GetUtfString(EXT_FIELD_USERID);


	if (mf != NULL && mt != NULL && uid != NULL) {
		if (strcmp(uid->c_str(), myName.c_str()) == 0) ///neu la minh thi khong thuc hien di co
			return;
		log("event_EXT_EVENT_MOVE_NTF:: moveFrom=%ld, moveTo=%ld, from FCT: %ld - UID: %s", *mf, *mt, fct == NULL ? -1 : *fct, uid->c_str());
		int moveFrom = *mf;
		int moveTo = *mt;
		int fct_int = fct == NULL ? -1 : *fct;
		if (this->currGameID == kGameCoUp){
			if (fct != NULL){
				log("MOVE CO_UP FCT = %d ", *fct);
				moveChess(convertIDByRow(moveFrom), convertIDByRow(moveTo), true, fct_int);
			}

		}
		else if (this->currGameID == kGameCoTuong){
			moveChess(convertIDByRow(moveFrom), convertIDByRow(moveTo), true, -1);
		}
	}
}

void LayerMainGame::event_EXT_EVENT_FOR_PEACE_NTF(boost::shared_ptr<ISFSObject> param)
{
	boost::shared_ptr<string> uidptr = param->GetUtfString(EXT_FIELD_USERID);
	if (uidptr != NULL)
	{
		string strAlert = StringUtils::format("Đối thủ %s xin hòa", uidptr->c_str());
		this->createLayerNotification(dataManager.GetSysString(273), strAlert, TAG_CALLBACK::TAG_PEACE_RESPONSE);
	}
}

void LayerMainGame::event_EXT_EVENT_UNDO_MOVE_REQ()
{

}

void LayerMainGame::event_EXT_EVENT_UNDO_MOVE_NTF()
{

}

void LayerMainGame::event_EXT_EVENT_CHESS_TABLE_NTF()
{
	this->schedule(schedule_selector(LayerMainGame::updateTimer), 1);
	log("event_EXT_EVENT_CHESS_TABLE_NTF");

	boost::shared_ptr<string> tblc = param->GetUtfString(EXT_FIELD_TABLE_LIST_CHESS); //list_cac quan co tren ban
	boost::shared_ptr<long> tft = param->GetInt(EXT_FIELD_TIME_FOR_TURN);//thoi gian 1 turn danh
	boost::shared_ptr<string> tfg = param->GetUtfString(EXT_FIELD_TIME_FOR_GAME);//tong thoi gian
	boost::shared_ptr<string> els = param->GetUtfString("els");

	if (tblc == NULL || tfg == NULL) return;
	//timeRestBlack = *tfg;
	timeForTurnBlack = (int)*tft;
	timeForTurnRed = (int)*tft;
	vector<string> list_split = mUtils::splitString(*tfg, '|');
	for (size_t i = 0; i < list_split.size(); ++i) {
		vector<string> time_split = mUtils::splitString(list_split[i], ';');
		int total_time = atoi(time_split.at(1).c_str());
		string name_player = time_split.at(0);
		if (name_player == player1) {
			timeRestBlack = total_time;
		}
		else {
			timeRestRed = total_time;
		}
	}
	log("TBLC = %s", tblc->c_str());
	log("Player1= %s, Player2= %s, tft = %d , tfg = %s", player1.c_str(), player2.c_str(), (int)*tft, tfg->c_str());

	isStartedGame = true;
	//update 25/7
	this->countGame = 1;

	string tb = tblc->c_str();

	vector<string> arr = mUtils::splitString(tb, ';');
	int i;

	for (size_t i = 0; i < this->arrChess.size(); i++) {
		this->arrChess.at(i)->setIDPos(-1);
		this->arrChess.at(i)->hideShadow(true);
	}
	logicChess->setChessIsFold(this->currGameID == kGameCoUp);
	vector<string> arr_info;
	int leng = 0;
	int name = 0, _name = 0;// la id quan co chua random
	int randomName = -1;// la id cua quan co thuc su da bi random
	int side = 0;
	bool isFolded = false;
	string nameString = "";
	Chess *chess = NULL;
	string listChess = "";
	string arrPosUsed = "";
	bool isBossRoom = GameUtils::isBossRoom(listUserPlay);

	for (size_t i = 0; i < arr.size(); i++) {
		arr_info = mUtils::splitString(arr.at(i), '_');
		leng = arr_info.size();
		if (leng < 2) return;
		_name = name = std::atoi(arr_info.at(0).c_str());
		side = std::atoi(arr_info.at(1).c_str());

		if (this->currGameID == kGameCoUp){
			isFolded = false;
			if (arr_info.size() >= 3)
			{
				isFolded = std::atoi(arr_info.at(2).c_str()) == 1;
			}
			if (arr_info.size() >= 4){
				randomName = std::atoi(arr_info.at(3).c_str());
			}
			if (isFolded)
				name = FOLD;
			//name = randomName;

		}

		int pos = i;

		switch (name) {
		case PAWN:
			nameString = "|  tot   |";
			break;
		case BISHOP:
			nameString = "|   si   |";
			break;
		case ELEPHANT:
			nameString = "| tuongj |";
			break;
		case KNIGHT:
			nameString = "|  ma    |";
			break;
		case CANNON:
			nameString = "|  phao  |";
			break;
		case ROOK:
			nameString = "|  xe    |";
			break;
		case KING:
			nameString = "|  tuong |";
			break;
		case FOLD:
			nameString = "|  Up    |";
			break;
		default:
			nameString = "|        |";
			break;
		}
		//nameString.append(StringUtils::format("/side %d", side));
		//if (side != 3) log("id= %d, name= %s, side=%d", i, nameString.c_str(), side);
		listChess += nameString;
		if (i % 9 == 8) {
			log("%s", listChess.c_str());
			listChess = "";
		}
		pos = convertIDByRow(pos);

		logicChess->setChessIDBySide(_name, side, convertIDByCol(i));

		int sideCheck = side;
		//update 10/7
		if ((isBossRoom || this->isSpectator) && side != 0)
			sideCheck = side == 1 ? 2 : 1;

		if (this->currGameID == kGameCoTuong)
			chess = getChessByName_Side(name, sideCheck);
		else{//Co Up
			//get valid chess
			chess = NULL;
			for (int ii = 0; ii < arrChess.size(); ii++)
			{
				Chess *_chess = arrChess.at(ii);
				if (_chess->getSide() != sideCheck)
					continue;
				if (_chess->getIDPos() >= 0)
					continue;
				chess = _chess;
				break;
			}
		}

		if (chess != NULL) {
			pos = convertIDByCol(pos);
			chess->setIDPos(pos);
			chess->setName(name);
			chess->hideShadow(false);
			chess->setFakeType(randomName);
		}

	}

	drawChess();
	if (els != NULL){
		log("ELS = %s", els->c_str());
		this->showEatChess(els->c_str());
	}
}

void LayerMainGame::event_EXT_EVENT_UNREADY_NTF()
{

}

void LayerMainGame::event_EXT_EVENT_USER_INFO()
{
	boost::shared_ptr<string> accPIS_ptr = param->GetUtfString("accPIs");
	if (accPIS_ptr == NULL)
		return;
	string str = accPIS_ptr->c_str();
	//cheat
	log("LIST_INFO = %s", str.c_str());
	if (str.at(4) == ';')
		str = str.substr(5, str.length() - 5);
	vector<string> listInfo = mUtils::splitString(str, ';');

	if (listInfo.size() == 0)
		return;
	//HoangDDs lstInfo.at(i) error
	try{
		for (unsigned int i = 0; i < listInfo.size(); ++i){
			vector<string> info = mUtils::splitString(listInfo.at(i), '|');
			if (info.size() < 2) break;
			string linkAvatar;
			if (info.size() >2)
				linkAvatar = info.at(2);
			else
				linkAvatar = info.at(1);
			int vipType = -1;
			string acc_name = "";
			boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(info.at(0));
			if (userInfo != NULL)
			{
				vipType = *userInfo->GetVariable("accVipType")->GetIntValue();
				boost::shared_ptr<UserVariable> aN = userInfo->GetVariable("aN");
				if (aN != NULL)
				{
					acc_name.append(aN->GetStringValue()->c_str());
				}
			}
		}
	}
	catch (const std::out_of_range &e){
		log("Out of range exception");
	}
}

void LayerMainGame::event_EXT_EVENT_AUTOKICK()
{

}

void LayerMainGame::event_EXT_EVENT_GAME_LEAVE_RES(boost::shared_ptr<ISFSObject> param)
{
	int gameID = SceneManager::getSingleton().getGameID();
	boost::shared_ptr<long> rscode = param->GetInt("errc");
	if (rscode == NULL) return;
	int int_res = (int)*rscode;
	switch (int_res)
	{
	case 31:
		this->createLayerNotification(dataManager.GetSysString(270), dataManager.GetSysString(271), TAG_CALLBACK::TAG_LEAVE_GAME);
		break;
	}
}

bool LayerMainGame::checkSpectator()
{
	vector<string> arr = mUtils::splitString(this->listUserPlay, ';');
	int size = arr.size();
	for (int i = 0; i < size; i++){
		vector<string> arrInfo = mUtils::splitString(arr.at(i), '|');
		if (arrInfo.size() < 2) continue;
		string name = arrInfo.at(0);
		log("Sam::isSpectator(): name= %s", name.c_str());
		if (0 == name.compare(myName)) return false;
	}

	return true;
}

void LayerMainGame::updateTimer(float dt)
{
	//update 8/7
	if (this->isStartedGame)
	{
		if (strcmp(this->nameCurrentTurn.c_str(), this->player2.c_str()) == 0 && this->timeForTurnBlack > 0 && this->timeRestBlack > 0) {
			this->timeForTurnBlack--;
			this->timeRestBlack--;
			//update 25/7
			this->lblTime2->setString(GameUtils::convertTimer(this->timeRestBlack).c_str());
			this->showTurn(0);
			//update 17/7
			if (this->timeForTurnBlack <= TIME_ALERT_CHESS && this->timeForTurnBlack > 0)
			{
				this->playSound(6);
				this->listUserInfoLayer.at(0)->showAlertAnim(true);
			}

			if (this->timeRestBlack <= TOTAL_TIMER_ALERT_CHESS)
			{
				//update 25/7
				MyAnimation::createActionTintLoop(this->lblTime2, Color3B::RED);
			}
		}
		if (strcmp(this->nameCurrentTurn.c_str(), this->player1.c_str()) == 0 && this->timeRestRed > 0 && this->timeRestRed > 0) {
			this->timeForTurnRed--;
			this->timeRestRed--;
			//update 25/7
			this->lblTime1->setString(GameUtils::convertTimer(this->timeRestRed).c_str());
			this->showTurn(1);
			if (this->timeForTurnRed <= TIME_ALERT_CHESS)
			{
				this->playSound(6);
				this->listUserInfoLayer.at(1)->showAlertAnim(true);
			}
			//update 17/7
			if (this->timeRestRed <= TOTAL_TIMER_ALERT_CHESS && this->timeRestRed > 0)
			{
				//update 25/7
				MyAnimation::createActionTintLoop(this->lblTime1, Color3B::RED);
			}
		}
	}
}

void LayerMainGame::resetGame()
{
	listUserInfoLayer.at(0)->showAlertAnim(false);
	listUserInfoLayer.at(1)->showAlertAnim(false);
	this->showTurn(3);

	timeRestBlack = DEFAULT_TIME_TOTAL;
	timeRestRed = DEFAULT_TIME_TOTAL;

	isYourTurn = false;
	vector<int> arrID;
	drawCanMove(arrID);
	//Dat lai cac quan co
	if (this->currGameID == kGameCoUp){
		int rawDatas[] = { ROOK, KNIGHT, ELEPHANT, BISHOP, KING, BISHOP, ELEPHANT, KNIGHT, ROOK, CANNON, CANNON, PAWN, PAWN, PAWN, PAWN, PAWN
			, ROOK, KNIGHT, ELEPHANT, BISHOP, KING, BISHOP, ELEPHANT, KNIGHT, ROOK, CANNON, CANNON, PAWN, PAWN, PAWN, PAWN, PAWN };

		for (size_t i = 0; i < arrChess.size(); i++){
			if (rawDatas[i] != KING){
				arrChess.at(i)->setName(FOLD);
			}
			else{
				arrChess.at(i)->setName(KING);
			}
		}
	}
	prevPoint->setVisible(false);
	//justPoint->setVisible(false);
	popupKingCheck->reset();

	logicChess->setChessIsFold(this->currGameID == kGameCoUp);
	//SceneManager::getSingleton().hidePopupSetting();
	layerTimer->hideTimer();
	this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
	//SceneManager::getSingleton().hidePopupSetting();
	this->refreshChess();
	this->loadConfigRoom(paramConfig);
	this->unschedule(schedule_selector(LayerMainGame::updateTimer));
	//update 8/7
	this->lblTime1->stopAllActions();
	this->lblTime2->stopAllActions();
	this->lblTime1->setTextColor(Color4B::WHITE);
	this->lblTime2->setTextColor(Color4B::WHITE);
}

void LayerMainGame::drawCanMove(vector<int> arrPos)
{
	for (int i = 0; i < 90; i++) {
		setPointVisible(i, false);
	}

	for (size_t i = 0; i < arrPos.size(); i++) {
		int pos = arrPos.at(i);
		if (pos >= 0 && pos < 90) setPointVisible(pos, true);
	}
}

void LayerMainGame::setPointVisible(int id, bool isShow)
{
	Node* nodeChild = spTable->getChildByTag(2373 + id);
	if (!nodeChild){
		return;
	}

	Node* c = nodeChild->getChildByTag(id);
	if (!nodeChild){
		return;
	}
	c->setVisible(isShow);
}

Chess* LayerMainGame::getChessByID(int id)
{
	for (Chess *chess : arrChess){
		if (chess->getIDPos() == id){
			return chess;
		}
	}
	return NULL;
}

void LayerMainGame::moveChess(int fromID, int toID, bool isCheckChieuTuong, int fromChessType /*= -1*/)
{
	if (!logicChess->isPosHasPiece(convertIDByRow(fromID)))
		return;
	logicChess->Move(convertIDByRow(fromID), convertIDByRow(toID));
	//del fold pos
	if (fromChessType != -1)
		logicChess->setChessIDBySide(fromChessType, convertIDByRow(toID));
	foldPos[convertIDByRow(fromID)] = EMPTY;
	foldPos[convertIDByRow(toID)] = EMPTY;
	//
	vector<int> arrID;
	drawCanMove(arrID);
	//Chess* chessBuff = NULL;
	for (size_t i = 0; i < arrChess.size(); i++) {
		Chess *chess = arrChess.at(i);
		chess->setClick(false);
		chess->showKingCheck(false, Vec2::ZERO);
		chess->showIsChecked(false);
		chess->hideJustPoint(true);
		int id_pos = chess->getIDPos();
		if (id_pos == fromID){
			//re-init chess
			if (chess->isFolded() && fromChessType != -1)
				chess->setName(fromChessType);
			chess->setIDPos(toID);
			chess->setLocalZOrder(10);
			chess->moveChess(getPositionFromIndex(toID));
			this->playSound(8);
		}
		else if (id_pos == toID){
			//hide ate chess
			chess->setVisible(false);
			chess->hideShadow(true);
			//update 10/7
			this->moveEatChess(chess, 0.3);
			chess->setIDPos(-1);
			this->playSound(3);
		}
	}
	this->showHistoryPoint(fromID, toID, isCheckChieuTuong);
	// thong tin so quan co con lai tren ban cua moi user
	// xe_phao_ma_tuongj_si_tot (vd: 1_0_1_0_0_3)
	// 	int RED = 1;
	// 	int BLACK = 2;

	if (isCheckChieuTuong){
		bool isBossRoom = GameUtils::isBossRoom(listUserPlay);
		//update 17/7
		if (strcmp(nameCurrentTurn.c_str(), player2.c_str()) == 0 || this->isSpectator) {
			if (!logicChess->IsKingSafe(SIDE::BLACK)) {
				//black chieu tuong red
				log("Chieu tuong red");
				popupKingCheck->showKingCheck(nameCurrentTurn);
				//do thằng chơi ( k phải chủ phòng chỉ đổi màu quân, side vẫn thế nên get king ngược lại - side - black)
				int sideKing = SIDE::RED;
				if (!isBossRoom && !this->isSpectator)
					sideKing = SIDE::BLACK;
				Chess* kingRed = getChessByName_SideForCheck(KING, sideKing);
				Vec2 posKingRed = kingRed->getPosition();
				//update 10/7
				vector<int> arrPieceCheck = logicChess->getKingCheckPieceArray(SIDE::BLACK);
				for (int i = 0; i < arrPieceCheck.size(); i++)
				{
					Chess* chessBuff = getChessByID(convertIDByRow(arrPieceCheck[i]));
					if (chessBuff != NULL)
					{
						chessBuff->showKingCheck(true, posKingRed);
					}
				}

				kingRed->showIsChecked(true);
				this->playSound(5);
			}
		}
		//update 17/7
		else if (strcmp(nameCurrentTurn.c_str(), player1.c_str()) == 0 || this->isSpectator) {
			if (!logicChess->IsKingSafe(SIDE::RED)) {
				//red chieu tuong black
				log("chieu tuong black");
				popupKingCheck->showKingCheck(nameCurrentTurn);
				int sideKing = SIDE::BLACK;
				if (!isBossRoom && !this->isSpectator)
					sideKing = SIDE::RED;
				Chess* kingBlack = getChessByName_SideForCheck(KING, sideKing);
				Vec2 posKingBlack = kingBlack->getPosition();

				//update 10/7
				vector<int> arrPieceCheck = logicChess->getKingCheckPieceArray(SIDE::RED);
				for (int i = 0; i < arrPieceCheck.size(); i++)
				{
					Chess* chessBuff = getChessByID(convertIDByRow(arrPieceCheck[i]));
					if (chessBuff != NULL)
					{
						chessBuff->showKingCheck(true, posKingBlack);
					}
				}
				kingBlack->showIsChecked(true);
				this->playSound(5);
			}
		}
	}
}

void LayerMainGame::showHistoryPoint(int fromID, int toID, bool isCheck)
{
	prevPoint->setVisible(true);
	//justPoint->setVisible(true);
	prevPoint->setPosition(this->getPositionFromIndex(fromID));
	//justPoint->setPosition(Vec2(this->getPositionFromIndex(toID).x ,
	//this->getPositionFromIndex(toID).y));
}

float LayerMainGame::getDistant2Point(Point p1, Point p2)
{
	return sqrtf(powf(p1.x - p2.x, 2) + powf(p1.y - p2.y, 2));
}

void LayerMainGame::showCurrentPoint(int currIndex)
{
	// 	justPoint->setVisible(true);
	// 	justPoint->setPosition(Vec2(this->getPositionFromIndex(currIndex).x,
	// 		this->getPositionFromIndex(currIndex).y));
}

void LayerMainGame::event_EXT_EVENT_MOVE_ERROR()
{
	SceneManager::getSingleton().showToast(dataManager.GetSysString(268));
	// 	moveChess(convertIDByRow(indexTargetHis), convertIDByRow(indexCurrentHis), true, -1);
	// 	logicChess->undoMove();
}

void LayerMainGame::event_EXT_EVENT_START_READY_TIMER_NOTIF()
{
	boost::shared_ptr<long> typePtr = param->GetInt(EXT_FIELD_TYPE);
	boost::shared_ptr<long> duration_timePtr = param->GetInt(EXT_FIELD_TIME_WAIT);
	if (isSpectator)
		return;
	if (typePtr && duration_timePtr)
	{
		int _type = *typePtr;
		int duration_time = *duration_timePtr;
		layerTimer->setTimeCount(duration_time);
		log("Time = %d / type = %d", duration_time, _type);
		if (_type == 0) //bat dau van moi
		{
			layerTimer->showTimer();
		}
		else{//đếm ngược sau khi config lại bàn
			layerTimer->hideTimer();
			this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->runTimer(45);
			popupAlert->setTagCallBack(TAG_CALLBACK::TAG_READY_SETUP);
			popupAlert->setContentMessage(dataManager.GetSysString(276), dataManager.GetSysString(277));
			popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		}
	}

}

Chess* LayerMainGame::getChessByName_Side(int name, int side)
{
	int i, leng = this->arrChess.size();
	Chess *chess;

	if (side == 3) return NULL;

	for (i = 0; i < leng; i++)
	{
		chess = this->arrChess.at(i);
		if (chess->getName() == name && chess->getSide() == side && chess->getIDPos() < 0) {
			return chess;
		}
	}

	return NULL;
}

Chess* LayerMainGame::getChessByName_SideForCheck(int name, int side)
{
	int i, leng = this->arrChess.size();
	Chess *chess;

	if (side == 3) return NULL;

	for (i = 0; i < leng; i++)
	{
		chess = this->arrChess.at(i);
		if (chess->getName() == name && chess->getSide() == side) {
			return chess;
		}
	}

	return NULL;
}

Chess* LayerMainGame::getChessByRawName_Side(int name, int side)
{
	int i, leng = arrChess.size();
	Chess *chess;
	//log("get by raw %d_%d", name, side);
	if (side == 3) return NULL;

	for (i = 0; i < leng; i++)
	{
		chess = arrChess.at(i);
		//log("chess:  %d_%d_%d", chess->getRawName(), chess->getSide(), chess->getIDPos());
		if (chess->getRawName() == name && chess->getSide() == side) {
			return chess;
		}
	}

	return NULL;
}

Chess* LayerMainGame::getAteChessByRawName_Side(int name, int side)
{
	int i, leng = arrChess.size();
	Chess *chess;

	if (side == 3) return NULL;

	for (i = 0; i < leng; i++)
	{
		chess = arrChess.at(i);
		if (chess->getIDPos() < 0 && chess->getSide() == side) {//do luc khoi tao ban co khong setname cu the,
			//nen dk chi can idpos <0 va side dung la duoc
			chess->setName(name);
			chess->setFakeType(name);
			return chess;
		}
	}

	return NULL;
}

void LayerMainGame::drawChess()
{
	Chess *chess;
	int id_pos;
	for (ssize_t i = 0; i < arrChess.size(); i++) {
		chess = arrChess.at(i);

		id_pos = chess->getIDPos();
		if (id_pos < 0 || id_pos > 89) {
			chess->setVisible(false);
		}
		else{
			log("chess =%d/pos init =  %d", chess->getInitName(), chess->getIDPos());
			chess->setVisible(true);
			chess->setPosition(getPositionFromIndex(id_pos));
			//chess->setLocalZOrder(0);
			chess->setClick(false);
			chess->hideShadow(false);
		}
	}
}

void LayerMainGame::onButtonReady(Ref* pSender)
{
	// 	layerTimer->stopTimer();
	// 	//resetGame();
	// 	btnReady->setVisible(false);
	//btnUnReady->setVisible(false);

	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
}

int LayerMainGame::convertIDByRow(int id)
{
	// xac dinh hang va cot cua id tu duoi len
	int row = id / 9;
	int col = id - row * 9;

	int rowC = 0;
	int colC = col;

	if (row <= 4) rowC = 5 + (4 - row);
	else rowC = 4 - (row - 5);

	return (rowC * 9 + colC);
}

int LayerMainGame::convertIDByCol(int id)
{
	// xac dinh hang va cot cua id tu duoi len
	int row = id / 9;
	int col = id - row * 9;

	int rowC = row;
	int colC = col;

	// convert them 1 lan nua, lan theo theo chieu doc
	if (colC <= 3) colC = 8 - colC;
	else colC = 4 - (colC - 4);

	return (rowC * 9 + colC);
}

void LayerMainGame::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerMainGame::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	return Size(pnlChat->getContentSize().width, FONT_SIZE_ITEM * 2);
}

cocos2d::extension::TableViewCell* LayerMainGame::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();

	Color3B colorName = COLOR_MYNAME_CELL;
	string _userName = listStore.at(idx).userName;
	string _namePlay = GameUtils::getNamePlayer(_userName);
	string content_msg = _namePlay + " : " + listStore.at(idx).userMsg;

	if (0 != listStore.at(idx).userName.compare(GameUtils::getMyName())
		||
		0 != listStore.at(idx).userName.compare("Hệ thống"))
	{
		colorName = COLOR_DEFAULT_CELL;
	}

	if (!cell)
	{
		cell = new(std::nothrow) TableViewCell();
		cell->autorelease();

		//cell->removeAllChildren();
		int startX = 10;
		if (listStore.size() > 0)
		{
			Label *labelContent = Label::createWithTTF(content_msg, "__Roboto-Bold.ttf", FONT_SIZE_ITEM);
			labelContent->setClipMarginEnabled(true);
			labelContent->setAnchorPoint(Vec2::ZERO);
			labelContent->setPosition(Vec2(startX, 0));
			labelContent->setColor(Color3B::WHITE);
			labelContent->setLineBreakWithoutSpace(false);
			labelContent->setMaxLineWidth(pnlChat->getContentSize().width - 110);
			labelContent->setTag(123);
			cell->addChild(labelContent);
			int _length = StringUtils::getCharacterCountInUTF8String(listStore.at(idx).userName);
			for (unsigned int i = 0; i < _length; i++)
			{
				Sprite *spriteText = labelContent->getLetter(i);
				if (spriteText)
				{
					spriteText->setColor(Color3B::YELLOW);
				}
			}
		}
	}
	else{
		Label* labelContent = (Label*)cell->getChildByTag(123);
		if (labelContent != NULL)
		{
			labelContent->setString(content_msg);
			int _length = StringUtils::getCharacterCountInUTF8String(listStore.at(idx).userName);
			for (unsigned int i = 0; i < _length; i++)
			{
				Sprite *spriteText = labelContent->getLetter(i);
				if (spriteText)
				{
					spriteText->setColor(Color3B::YELLOW);
				}
			}
		}
	}

	return cell;
}

ssize_t LayerMainGame::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	return listStore.size();
}

void LayerMainGame::hideCurrentPoint()
{
	//justPoint->setVisible(false);
}

void LayerMainGame::OnSmartFoxRoomVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueRoom = (*ptrEvetnParams)["room"];
	boost::shared_ptr<Room> room = ((boost::static_pointer_cast<Room>(ptrEventParamValueRoom)));
	//
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	boost::shared_ptr<RoomVariable> uidPtr = room->GetVariable("uid");
	if (rv != NULL){
		string s = *rv->GetStringValue();
		bool isBossRoom = GameUtils::isBossRoom(listUserPlay);//check xem co phai chu phong hay khong, neu khong phai thi phai reverse lai
		log("Room %s update RoomVariables: %s", room->Name()->c_str(), s.c_str());
		if (this->numUser <= 1 && this->paramConfig.empty())
			this->loadConfigRoom(s);
	}
	if (uidPtr)
	{
		string uid = uidPtr->GetStringValue()->c_str();
	}
	int specCount = room->SpectatorCount();
	//lblViews->setString(StringUtils::format("%d", specCount));
}

void LayerMainGame::requestChangeSetting()
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom != NULL){
		boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_CHANGE_SETTING_REQUEST, params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerMainGame::event_EXT_EVENT_CHANGE_SETTING()
{
	if (isStartedGame)
		return;
	if (isSpectator)
		return;
	boost::shared_ptr<string> userPtr = param->GetUtfString(EXT_FIELD_USERID);
	if (userPtr)
	{
		string uid = userPtr->c_str();
		if (0 != uid.compare(GameUtils::getMyName()))
		{
			layerTimer->hideTimer();
			this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->runTimer(45);
			popupAlert->setTagCallBack(TAG_CALLBACK::TAG_QUIT);
			popupAlert->setContentMessage(dataManager.GetSysString(276), dataManager.GetSysString(277));
			popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		}
	}
}

void LayerMainGame::loadConfigRoom(string roomConfig)
{
	/////Room P107_8 update RoomVariables: 2000@0@1@tester15@600@90@1-4,0-7@3@0-0@1@1
	//money-isPlaying-numOfPlayer-uid-tong_thoigian-thoi gian 1 luot - chap quan- chap tien - cuoc quan - chap thoi gian 
	//- chap hoa cho duoc - cho phep nguoi khac thay doi thiet lap phong
	paramConfig = roomConfig;
	vector<string> listConfig = mUtils::splitString(roomConfig, '@');
	if (listConfig.size() == 13)
	{
		int betValue = atoi(listConfig.at(0).c_str());
		string uidConfig = listConfig.at(3);
		bool isAllow = atoi(listConfig.at(11).c_str());
		string configChapQuan = listConfig.at(6);
		int totalTime = atoi(listConfig.at(4).c_str());
		lblTime1->setString(GameUtils::convertTimer(totalTime).c_str());
		lblTime2->setString(GameUtils::convertTimer(totalTime).c_str());
		lblBetValue->setString(mUtils::convertMoneyEx(betValue));
		//update 25/7
		this->totalTurnTime = atoi(listConfig.at(5).c_str());
		this->timeForTurnBlack = this->totalTurnTime;
		this->timeForTurnRed = this->totalTurnTime;

		bool isChuPhong = GameUtils::isBossRoom(listUserPlay);
		if (!isChuPhong)
		{
			if (isAllow)
			{
				this->isShowSetting = true;
			}
			else{
				this->isShowSetting = false;
			}
		}
		else{
			this->isShowSetting = true;
		}
		//set table from config
		vector<string> lstcfgChapQuan = mUtils::splitString(configChapQuan, ',');
		int startJumpUnit = 0;//neu la chinh minh thi gia tri = 0, neu la nguoi khac gia tri = 100, tuc la nua ban co phia tren ( xem ham initTableChess())
		if (0 != GameUtils::getMyName().compare(uidConfig))
		{
			startJumpUnit = 100;
			countChapTien = atoi(listConfig.at(7).c_str());
		}
		//update 21/7
		if (0 == this->bossName.compare(uidConfig) && this->isSpectator)
		{
			startJumpUnit = 0;
		}
		this->refreshChess();

		for (size_t i = 0; i < lstcfgChapQuan.size(); i++)
		{
			vector<string> cfg = mUtils::splitString(lstcfgChapQuan.at(i), '-');
			if (cfg.size() == 2)
			{
				int idChess = atoi(cfg.at(0).c_str());
				int posChess = atoi(cfg.at(1).c_str());
				if (posChess != 0)
				{
					for (size_t j = 0; j < arrChess.size(); j++)
					{
						//an quan trong config chap quan
						if (this->arrChess.at(j)->getName() == idChess &&
							this->arrChess.at(j)->getPosConfig() == (posChess + startJumpUnit))
						{
							this->arrChess.at(j)->setVisible(false);
							this->arrChess.at(j)->hideShadow(true);
							this->logicChess->setChessIDBySide(EMPTY, 0, convertIDByRow(this->arrChess.at(j)->getIDPos()));
							this->arrChess.at(j)->setIDPos(-1);
						}
					}
				}
			}
		}
		this->betValue = betValue;
	}
	//update 20/7
	//this->lblRoomName->setString(GameUtils::getRoomNameForChess());
}

void LayerMainGame::callBackFromAlert(LayerPopupAlert* sender, int tag)
{
	log("TAG CALLBACK %d", tag);
	switch ((TAG_CALLBACK)tag)
	{
	case TAG_CALLBACK::TAG_QUIT:
	{
								   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
								   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
								   if (lastRoom == NULL)
									   return;
								   parameter->PutInt("cf", 1);
								   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
								   GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_CALLBACK::TAG_READY_SETUP:
	{
										  log("ready request");
										  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
										  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
										  if (lastRoom == NULL)
											  return;
										  boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
										  GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_CALLBACK::TAG_SETUP_DONE:
	{
										 log("ready request");
										 boost::shared_ptr<ISFSObject> parameter(new SFSObject());
										 boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
										 if (lastRoom == NULL)
											 return;
										 boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
										 GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_LEAVE_GAME:
	{
						   SceneManager::getSingleton().showLoading();
						   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
						   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
						   if (lastRoom == NULL)
							   return;
						   parameter->PutInt("cf", 1);
						   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
						   GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_PEACE_RESPONSE:
	{
							   // gửi lại sv là mình đồng ý (rep=1)
							   boost::shared_ptr<ISFSObject> parameter(new SFSObject());
							   parameter->PutInt("rep", 1);
							   boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
							   if (lastRoom == NULL)
								   return;
							   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REPLY_FOR_PEACE, parameter, lastRoom));
							   GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_PEACE_REQUEST:
	{
							  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
							  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
							  if (lastRoom == NULL)
								  return;
							  boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_FOR_PEACE, parameter, lastRoom));
							  GameServer::getSingleton().Send(request);
	}
		break;
	case TAG_LOOSE:
	{
					  boost::shared_ptr<ISFSObject> parameter(new SFSObject());
					  boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
					  if (lastRoom == NULL)
						  return;
					  boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_IM_LOSE, parameter, lastRoom));
					  GameServer::getSingleton().Send(request);
	}
		break;

	case 37:
	{
			   this->btnSetting->setEnabled(false);
			   boost::shared_ptr<ISFSObject> params(new SFSObject());
			   boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			   if (lastRoom != NULL){
				   boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PRE_CHANGE_SETTING_REQUEST, params, lastRoom));
				   GameServer::getSingleton().Send(request);
			   }
	}
		break;
	default:
		break;
	}

}

void LayerMainGame::event_EXT_EVENT_CHANGE_SETTING_NOTIF()
{
	if (isStartedGame)
		return;
	boost::shared_ptr<string> uidPtr = param->GetUtfString(EXT_FIELD_USERID);
	boost::shared_ptr<long> duration_timePtr = param->GetInt(EXT_FIELD_TIME_WAIT);

	if (uidPtr)
	{
		string _uid = uidPtr->c_str();
		int _duration = 45;
		if (duration_timePtr != NULL)
			_duration = *duration_timePtr;
		log("event_EXT_EVENT_CHANGE_SETTING_NOTIF uid = %s/duration = %d, param = %s", _uid.c_str(), _duration, param->GetUtfString("params")->c_str());
		layerTimer->hideTimer();
		this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
		layerTimer->setTimeCount(_duration);

		//update 25/7
		boost::shared_ptr<string> paramPtr = param->GetUtfString("params");
		if (paramPtr != NULL)
		{
			this->loadConfigRoom(paramPtr->c_str());
		}

		//update 25/7
		if (0 == _uid.compare("") && !this->isSpectator && this->numUser > 1)//rong la khi mới tạo bàn
		{
			layerTimer->showTimer();
			return;
		}

		//update 14/7
		// 		if (this->isFirst&& !this->isSpectator && this->countGame == 0)
		// 		{
		// 			//this->isShowSetting = false;
		// 			this->isFirst = false;
		// 			layerTimer->setTimeCount(_duration);
		// 			layerTimer->showTimer();
		// 			return;
		// 		}

		if (numUser < 2)
			return;

		//update 17/7
		if (0 != _uid.compare(GameUtils::getMyName()))
		{
			//update 25/7
			string strConfig = GameUtils::convertFullConfigToString(paramPtr->c_str());
			if (strConfig.compare("") == 0)
				return;

			//hiện thông báo cài đặt đã bị thay đổi
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->runTimer(_duration);
			popupAlert->setTagCallBack(TAG_CALLBACK::TAG_READY_SETUP);
			popupAlert->setContentMessage(dataManager.GetSysString(31), strConfig);
			if (this->isSpectator)
			{
				//update 21/7
				popupAlert->setTitleConfirmButton("Đóng");
				popupAlert->showOnlyButtonAccept();
			}
			else{
				popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
				popupAlert->setTitleCloseButton("Từ chối");
				popupAlert->setCallBackClose(CC_CALLBACK_2(LayerMainGame::callBackFromAlertClose, this));
			}
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		}
		else{
			//hiện thông báo chờ người chơi khác sẵn sàng
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->setTagCallBack(TAG_CALLBACK::TAG_SETUP_DONE);
			popupAlert->showOnlyButtonAccept();
			popupAlert->runTimer(_duration);
			//popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
			popupAlert->setContentMessage(dataManager.GetSysString(276), dataManager.GetSysString(278));
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
			//update 25/7
			log("ready request");
			boost::shared_ptr<ISFSObject> parameter(new SFSObject());
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_READY_REQ, parameter, lastRoom));
			GameServer::getSingleton().Send(request);
		}

	}
}

void LayerMainGame::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void LayerMainGame::event_EXT_EVENT_PRE_CHANGE_SETTING_NOTIF()
{
	// 	if (SceneGame::getSingleton().getFirstConfig())
	// 		return;
	//thời gian chờ sẵn sàng sẽ tính gộp luôn với thời gian setting
	if (this->isSpectator)
		return;
	boost::shared_ptr<string> uidPtr = param->GetUtfString(EXT_FIELD_USERID);
	if (uidPtr != NULL)
	{
		log("event_EXT_EVENT_PRE_CHANGE_SETTING_NOTIF uid = %s", uidPtr->c_str());
		layerTimer->hideTimer();
		if (0 != uidPtr->compare(GameUtils::getMyName()) && this->numUser > 1)
		{
			int timerCount = layerTimer->getTimeCount();
			this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->runTimer(timerCount);
			popupAlert->setTagCallBack(TAG_CALLBACK::TAG_QUIT);
			popupAlert->setContentMessage("Cài đặt trận đấu", "Trận đấu đang được cài đặt lại,vui lòng chờ trong giây lát");
			popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_MONEYCHARGE, LAYER_POPUP_ID::POPUP_ALERT);

			//update 14/7
			popupAlert->setTitleConfirmButton("Rời bàn");
		}
	}
}

void LayerMainGame::loadChessTableByType()
{
	int idChessBlack = GameUtils::getIDChessUser(player2);
	int idChessRed = GameUtils::getIDChessUser(player1);
	if (0 == player1.compare(player2))
		idChessRed = -1;
	//check to reload side red or black for user
	bool isBossRoom = GameUtils::isBossRoom(listUserPlay);
	//default side 1 - black, side 2 = red
	int side1 = SIDE::BLACK;
	int side2 = SIDE::RED;
	if (!isBossRoom)
	{
		side1 = SIDE::RED;
		side2 = SIDE::BLACK;
	}
	if (isSpectator)
	{
		side1 = SIDE::BLACK;
		side2 = SIDE::RED;
		//int idTg = idChessBlack;
		//idChessBlack = idChessRed;
		//idChessRed = idTg;//switch id chess for spectator
	}

	for (ssize_t i = 0; i < arrChess.size(); i++)
	{
		if (i < 16)
		{
			arrChess.at(i)->setSide(side1);
			arrChess.at(i)->setImageChess((CHESS_TYPE)idChessBlack);
		}
		else{
			arrChess.at(i)->setSide(side2);
			arrChess.at(i)->setImageChess((CHESS_TYPE)idChessRed);
		}
	}
}

void LayerMainGame::event_EXT_EVENT_WARNING_NOTIF()
{
	boost::shared_ptr<string> messPtr = param->GetUtfString(EXT_FIELD_MESSAGE_STRING);
	if (messPtr)
	{
		SceneManager::getSingleton().showPopupNotify(messPtr->c_str());
	}
}

Chess* LayerMainGame::getChessByInitName_Side(int name, int side)
{
	int i, leng = arrChess.size();
	Chess *chess;

	if (side == 3) return NULL;

	for (i = 0; i < leng; i++)
	{
		chess = arrChess.at(i);
		if (chess->getIDPos() < 0 && chess->getInitName() == name && chess->getSide() == side) {
			return chess;
		}
	}

	return NULL;
}

cocos2d::Vec2 LayerMainGame::getPositionFromIndexServer(int id)
{
	//start index tag from 220
	Node* nodeChild = spTable->getChildByTag(220 + id / 9);
	if (!nodeChild){
		return Vec2(0, 0);
	}
	return Vec2(nodeChild->getPosition().x, nodeChild->getPosition().y - 10);
}

void LayerMainGame::playSound(int _type)
{
	if (!mUtils::isSoundOn())
		return;
	string imgName = "Lose.mp3";
	switch (_type)
	{
	case 0://loose
		break;
	case 1:
		imgName = "Win.mp3";
		break;
	case 2:
		imgName = "New.mp3";
		break;
	case 3:
		imgName = "AnQuan.mp3";
		break;
	case 4:
		imgName = "StartGame.mp3";
		break;
	case 5:
		imgName = "GeneralChecked.mp3";
		break;
	case 6:
		imgName = "TimeCountDown.mp3";
		break;
	case 7:
		imgName = "MatQuan.mp3";
		break;
	case 8:
		imgName = "ChessMove.mp3";
		break;
	case 9:
		imgName = "Click.mp3";
		break;
	default:
		break;
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(imgName.c_str(), false);
}

void LayerMainGame::testReconnect()
{
	string listTable = "5_1;3_1;2_1;1_1;6_1;1_1;2_1;3_1;5_1;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;4_1;7_0;7_0;7_0;7_0;7_0;4_1;7_0;0_1;7_0;0_1;7_0;0_1;7_0;0_1;7_0;0_1;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;0_2;7_0;0_2;7_0;0_2;7_0;0_2;7_0;0_2;7_0;4_2;7_0;7_0;7_0;7_0;7_0;4_2;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;7_0;5_2;3_2;2_2;1_2;6_2;1_2;2_2;3_2;5_2;";

	isStartedGame = true;

	vector<string> arr = mUtils::splitString(listTable, ';');
	int i;

	for (size_t i = 0; i < this->arrChess.size(); i++) {
		this->arrChess.at(i)->setIDPos(-1);
		this->arrChess.at(i)->hideShadow(true);
	}
	logicChess->setChessIsFold(this->currGameID == kGameCoUp);
	vector<string> arr_info;
	int leng = 0;
	int name = 0, _name = 0;// la id quan co chua random
	int randomName = -1;// la id cua quan co thuc su da bi random
	int side = 0;
	bool isFolded = false;
	string nameString = "";
	Chess *chess = NULL;
	string listChess = "";
	string arrPosUsed = "";
	bool isBossRoom = GameUtils::isBossRoom(listUserPlay);

	for (size_t i = 0; i < arr.size(); i++) {
		arr_info = mUtils::splitString(arr.at(i), '_');
		leng = arr_info.size();
		if (leng < 2) return;
		_name = name = std::atoi(arr_info.at(0).c_str());
		side = std::atoi(arr_info.at(1).c_str());

		if (this->currGameID == kGameCoUp){
			isFolded = false;
			if (arr_info.size() >= 3)
			{
				isFolded = std::atoi(arr_info.at(2).c_str()) == 1;
			}
			if (arr_info.size() >= 4){
				randomName = std::atoi(arr_info.at(3).c_str());
			}
			if (isFolded)
				name = FOLD;
			//name = randomName;

		}

		int pos = i;

		switch (name) {
		case PAWN:
			nameString = "|  tot   |";
			break;
		case BISHOP:
			nameString = "|   si   |";
			break;
		case ELEPHANT:
			nameString = "| tuongj |";
			break;
		case KNIGHT:
			nameString = "|  ma    |";
			break;
		case CANNON:
			nameString = "|  phao  |";
			break;
		case ROOK:
			nameString = "|  xe    |";
			break;
		case KING:
			nameString = "|  tuong |";
			break;
		case FOLD:
			nameString = "|  Up    |";
			break;
		default:
			nameString = "|        |";
			break;
		}
		//nameString.append(StringUtils::format("/side %d", side));
		//if (side != 3) log("id= %d, name= %s, side=%d", i, nameString.c_str(), side);
		listChess += nameString;
		if (i % 9 == 8) {
			log("%s", listChess.c_str());
			listChess = "";
		}
		pos = convertIDByRow(pos);

		logicChess->setChessIDBySide(_name, side, convertIDByCol(i));

		if (this->currGameID == kGameCoTuong)
			chess = getChessByName_Side(name, side);
		else{//Co Up
			//get valid chess
			//chess = getChessByInitName_Side(randomName, side);
			chess = NULL;
			for (ssize_t ii = 0; ii < this->arrChess.size(); ii++)
			{
				Chess *_chess = this->arrChess.at(ii);
				//neu la chu phong phai revert side, check nguoc lai, 1 se la RED va 2 se la BLACK
				// 				if (isBossRoom)
				// 				{
				// 					if (_chess->getSide() == side || side == 0)
				// 						continue;
				// 					if (_chess->getIDPos() >= 0)
				// 						continue;
				// 					chess = _chess;
				// 					break;
				// 				}
				// 				else{
				if (_chess->getSide() != side)
					continue;
				if (_chess->getIDPos() >= 0)
					continue;
				chess = _chess;
				break;
				//}
			}
		}

		if (chess != NULL) {
			if (!isSpectator)
				pos = convertIDByCol(pos);
			else{
				if (side == 2)
					pos = convertIDByRow(pos);
			}
			chess->setIDPos(pos);
			chess->setName(name);
			chess->hideShadow(false);
			chess->setFakeType(randomName);
		}

	}

	drawChess();
}

void LayerMainGame::OnSmartFoxUserEnterRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEvetnParams)["user"];
	boost::shared_ptr<User> user = ((boost::static_pointer_cast<User>(ptrEventParamValueUser)));
	string username = user->Name()->c_str();

	bool isSpec = user->IsSpectator();
	if (isSpec)
	{
		if (listStore.size() > MAX_CHAT_LENGTH)
			listStore.pop_back();
		// 		string mesage = StringUtils::format("Người chơi %s đã vào phòng", username);
		// 		listStore.push_back(ChatInfoStruct("Hệ thống", mesage));

		string mesage = StringUtils::format("Người chơi %s đã vào phòng", username.c_str());
		listStore.push_back(ChatInfoStruct("Hệ thống", mesage));
		tblChat->reloadData();
		tblChat->setContentOffset(Vec2::ZERO);
		//layerChat->setListChat("Hệ thống", mesage);
	}
}

void LayerMainGame::initBottomInGame()
{
	this->btnBack = (Button*)pnlMain->getChildByName("btnBack");
	this->btnSound = (Button*)pnlMain->getChildByName("btnSound");
	this->btnPeace = (Button*)pnlMain->getChildByName("btnPeace");
	this->btnSetting = (Button*)pnlMain->getChildByName("btnSetting");
	this->btnLose = (Button*)pnlMain->getChildByName("btnLose");
	Button* btnRecharge = (Button*)pnlMain->getChildByName("btnRecharge");
	this->btnSpec = (Button*)pnlMain->getChildByName("btnSpec");
	this->bt_sound2_5 = (Sprite*)this->btnSound->getChildByName("bt_sound2_5");
	this->bt_sound2_6 = (Sprite*)this->btnSound->getChildByName("bt_sound2_6");
	this->bt_text_am_8 = (Sprite*)this->btnSound->getChildByName("bt_text_am_8");
	this->tat_am_4 = (Sprite*)this->btnSound->getChildByName("tat_am_4");

	btnBack->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonBack, this));
	btnSetting->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonSetting, this));
    btnSetting->setVisible(false);
	btnPeace->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonPeace, this));
    btnPeace->setVisible(false);
	btnLose->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonLose, this));
    btnLose->setVisible(false);
	btnSound->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonSound, this));
    btnSound->setVisible(false);
	btnRecharge->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonCharge, this));
    btnRecharge->setVisible(false);
	btnSpec->addClickEventListener(CC_CALLBACK_1(LayerMainGame::onButtonSpector, this));
    btnSpec->setVisible(false);

	bool isOn = true;
	if (mUtils::isSoundOn())
	{
		bt_sound2_5->setVisible(true);
		bt_text_am_8->setVisible(true);
		bt_sound2_6->setVisible(false);
		tat_am_4->setVisible(false);
	}
	else{
		bt_sound2_5->setVisible(false);
		bt_text_am_8->setVisible(false);
		bt_sound2_6->setVisible(true);
		tat_am_4->setVisible(true);
	}

}

void LayerMainGame::onButtonBack(Ref* pSender)
{
	boost::shared_ptr<ISFSObject> parameter(new SFSObject());
	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL){
		return;
	}
	boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
	GameServer::getSingleton().Send(request);
}

void LayerMainGame::onButtonSetting(Ref* pSender)
{
	bool isStartGame = GameUtils::isStartedGame();
	if (isStartGame)
	{
		//update 25/7
		this->btnSetting->setEnabled(true);
		LayerCreateRoomChess* layerCreateRoom = LayerCreateRoomChess::create();
		layerCreateRoom->setPopupInGame(true);
		layerCreateRoom->setListUser(this->listUserPlay);
		this->addChild(layerCreateRoom, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_CONFIG_INGAME);
		layerCreateRoom->setConfigFromString(this->paramConfig);
		layerCreateRoom->setCanSetting(false);
		return;
	}
	if (isShowSetting)
	{
		// 		this->btnSetting->setEnabled(false);
		// 		boost::shared_ptr<ISFSObject> params(new SFSObject());
		// 		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		// 		if (lastRoom != NULL){
		// 			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PRE_CHANGE_SETTING_REQUEST, params, lastRoom));
		// 			GameServer::getSingleton().Send(request);
		// 		}

		//update 25/7
		bool isChuPhong = GameUtils::isBossRoom(this->listUserPlay);
		if (isChuPhong)
		{
			this->btnSetting->setEnabled(false);
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom != NULL){
				boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_PRE_CHANGE_SETTING_REQUEST, params, lastRoom));
				GameServer::getSingleton().Send(request);
			}
		}
		else{
			LayerPopupAlert* popupAlert = LayerPopupAlert::create();
			popupAlert->setTagCallBack(37);
			popupAlert->setContentMessage("Thông báo", "Bạn muốn thay đổi cài đặt hay\nxem thông tin cài đặt");
			popupAlert->setTitleConfirmButton("Cài đặt");
			popupAlert->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
			popupAlert->setTitleCloseButton("Xem");
			popupAlert->setCallBackClose(CC_CALLBACK_2(LayerMainGame::callBackFromAlertClose, this));
			this->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
			this->addChild(popupAlert, ZORDER_LIST::ZORDER_POPUP_MONEYCHARGE, LAYER_POPUP_ID::POPUP_ALERT);
		}
	}
	else{
		//update 25/7
		this->btnSetting->setEnabled(true);
		LayerCreateRoomChess* layerCreateRoom = LayerCreateRoomChess::create();
		layerCreateRoom->setPopupInGame(true);
		layerCreateRoom->setListUser(this->listUserPlay);
		this->addChild(layerCreateRoom, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_CONFIG_INGAME);
		layerCreateRoom->setConfigFromString(this->paramConfig);
		layerCreateRoom->setCanSetting(this->isShowSetting);
	}
}

void LayerMainGame::onButtonPeace(Ref* pSender)
{
	if (GameUtils::isStartedGame())
		this->createLayerNotification(dataManager.GetSysString(273), dataManager.GetSysString(273), TAG_CALLBACK::TAG_PEACE_REQUEST);
}

void LayerMainGame::onButtonLose(Ref* pSender)
{
	if (GameUtils::isStartedGame())
		this->createLayerNotification(dataManager.GetSysString(272), dataManager.GetSysString(271), TAG_CALLBACK::TAG_LOOSE);
}

void LayerMainGame::onButtonSound(Ref* pSender)
{
	bool isOn = true;
	if (mUtils::isSoundOn())
	{
		isOn = false;
		bt_sound2_5->setVisible(false);
		bt_text_am_8->setVisible(false);
		bt_sound2_6->setVisible(true);
		tat_am_4->setVisible(true);
	}
	else{
		bt_sound2_5->setVisible(true);
		bt_text_am_8->setVisible(true);
		bt_sound2_6->setVisible(false);
		tat_am_4->setVisible(false);
	}
	mUtils::setSoundConfig(isOn);
}

void LayerMainGame::event_EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE(boost::shared_ptr<ISFSObject> param)
{
	this->btnSetting->setEnabled(true);
	LayerCreateRoomChess* layerCreateRoom = LayerCreateRoomChess::create();
	layerCreateRoom->setPopupInGame(true);
	layerCreateRoom->setListUser(this->listUserPlay);
	this->addChild(layerCreateRoom, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_CONFIG_INGAME);
	layerCreateRoom->setConfigFromString(this->paramConfig);
	layerCreateRoom->setCanSetting(this->isShowSetting);
}

void LayerMainGame::createLayerNotification(string title, string content_mess, int tag)
{
	LayerPopupAlert* layerPopup = LayerPopupAlert::create();
	Scene* currScene = Director::getInstance()->getRunningScene();
	if (currScene)
	{
		currScene->removeChildByTag(LAYER_POPUP_ID::POPUP_ALERT);
		currScene->addChild(layerPopup, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_ALERT);
		layerPopup->setContentMessage(title, content_mess);
		layerPopup->setTagCallBack(tag);
		if (tag == TAG_CALLBACK::TAG_PEACE_RESPONSE)
			layerPopup->runTimer();
		layerPopup->setCallBackConfirm(CC_CALLBACK_2(LayerMainGame::callBackFromAlert, this));
	}
}

void LayerMainGame::showSpectatorMode(bool isSpec)
{
	this->btnPeace->setEnabled(!isSpec);
	this->btnSetting->setEnabled(!isSpec);
	this->btnLose->setEnabled(!isSpec);
}

void LayerMainGame::onButtonCharge(Ref* pSender)
{
	SceneManager::getSingleton().showPopupMoneyCharge();
}

void LayerMainGame::onButtonSpector(Ref* pSender)
{
	if (this->isStartedGame)
	{
		SceneManager::getSingleton().showToast("Trận đấu đang diễn ra. Vui lòng đợi hết trận đấu");
		return;
	}


	if (this->isSpectator)
	{
		boost::shared_ptr<IRequest> request(new SpectatorToPlayerRequest());
		GameServer::getSingleton().Send(request);
	}
	else{
		if (this->numUser == 1)
			return;
		boost::shared_ptr<IRequest> request(new PlayerToSpectatorRequest());
		GameServer::getSingleton().Send(request);
	}
}

void LayerMainGame::showTurn(int type)
{
	if (type == 0)
	{
		this->timeSpriteTurn1->setVisible(false);
		this->timeSpriteTurn2->setVisible(true);
	}
	else if (type == 1){
		this->timeSpriteTurn1->setVisible(true);
		this->timeSpriteTurn2->setVisible(false);
	}
	else{
		this->timeSpriteTurn1->setVisible(false);
		this->timeSpriteTurn2->setVisible(false);
	}
}

vector<EatPos> LayerMainGame::createArrPos(Vec2 startPoint)
{
	float chess_w = 95;
	float chess_h = 93;
	vector<EatPos> arrPos;
	///for player1
	for (int i = 0; i < 16; ++i){
		EatPos ep;
		int id;
		int status = 0;
		Vec2 pos;
		if (i < 8){
			id = i;
			pos = Vec2(startPoint.x + chess_w * i, startPoint.y);
		}
		else{
			id = i - 8;
			pos = Vec2(startPoint.x + chess_w* (i - 8), startPoint.y - chess_h);
		}
		ep.id = id;
		ep.status = status;
		ep.pos = pos;
		arrPos.push_back(ep);
	}
	return arrPos;
}

void LayerMainGame::moveEatChess(Chess* chess, float duration)
{
	// 	if (this->isSpectator)////neu la khach thi khoi show cho met
	// 		return;

	//update 25/7
	chess->setVisible(true);
	chess->setIDPos(10000);//set lai idpos tranh truong hop get cung 1 quan co do idpos <-1 -> animation sai
	int side_chess = chess->getSide();
	if (side_chess == 1){ // player1 - se hien thi quan an duoc tu thang 2
		for (int i = 0; i < this->arrEatPosPlayer1.size(); ++i){
			if (this->arrEatPosPlayer1.at(i).status == 0){
				this->arrEatPosPlayer1.at(i).status = 1;
				if (chess->getName() == FOLD){
					if (!GameUtils::isBossRoom(this->listUserPlay) && this->isSpectator == false)//la khach
						chess->setName(chess->getFakeType());
					chess->setOpacity(150);
				}
				chess->runAction(MoveTo::create(duration, this->arrEatPosPlayer1.at(i).pos));
				break;
			}
		}
	}
	else{ //player2 - hien thi quan an duoc tu thang 1
		for (int i = 0; i < arrEatPosPlayer2.size(); ++i){
			if (this->arrEatPosPlayer2.at(i).status == 0){
				this->arrEatPosPlayer2.at(i).status = 1;
				if (chess->getName() == FOLD){
					if (GameUtils::isBossRoom(this->listUserPlay) && this->isSpectator == false)//la khach
						chess->setName(chess->getFakeType());
					chess->setOpacity(150);
				}
				chess->runAction(MoveTo::create(duration, this->arrEatPosPlayer2.at(i).pos));
				break;
			}
		}
	}

}

void LayerMainGame::showEatChess(string listChess)
{
	///co tuong: ELS = bot_108,3, 4|bot_109,0, 1
	///co up: ELS = bot_109,5:0, 0:1|bot_108,3:1, 3:1
	///"bot_108,4|bot_109,3, 3, 1, 1";
	vector<string> arrInfo = mUtils::splitString(listChess, '|');

	if (arrInfo.size() <= 1) return;
	int side;
	int side_get;
	for (int i = 0; i < arrInfo.size(); ++i){
		vector<string> info = mUtils::splitString(arrInfo.at(i), ',');
		string user_name = info.at(0);
		if (info.at(1).compare(" ") == 0) continue;//phong khi bi loi luc bat dau game
		int side = 1;

		//update 25/7
		if (0 == user_name.compare(this->bossName))
		{
			side = SIDE::RED;
		}
		else{
			side = SIDE::BLACK;
		}

		if (info.size() > 1){
			for (int j = 1; j < info.size(); ++j){ /// thong tin cac quan co da an cua 1 nguoi choi
				vector<string> chess_info = mUtils::splitString(info.at(j), ':');
				if (chess_info.at(0).compare("") == 0)
					continue;
				Chess *chess = NULL;
				int isFold = 0;
				int id_chess = std::atoi(chess_info.at(0).c_str());
				log("eatChess = %d_%d", id_chess, side);
				if (chess_info.size() > 1){
					chess = this->getAteChessByRawName_Side(id_chess, side);
					isFold = std::atoi(chess_info.at(1).c_str());
				}
				else{
					chess = this->getChessByName_Side(id_chess, side);
				}
				if (chess == NULL){
					log("chess = NULL");
					continue;
				}

				//update 25/7
				if (isFold == 1)// 0 la up, nhung chi check quan cua thang tren minh an nen check co bang 1 hay ko?
				{
					if ((chess->getSide() == side && this->isSpectator == false) || this->isSpectator == true)
						chess->setName(FOLD);
				}
				else{
					chess->setName(chess->getFakeType());
				}

				moveEatChess(chess, 0);
			}
		}
	}
}

void LayerMainGame::callBackFromAlertClose(LayerPopupAlert* sender, int tag)
{
	//update 25/7
	if (tag == TAG_CALLBACK::TAG_READY_SETUP)
	{

		bool isBoss = GameUtils::isBossRoom(listUserPlay);
		if (isBoss)//kich thang khac ra
		{
			boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom != NULL)
			{
				boost::shared_ptr<ISFSObject> params(new SFSObject());
				params->PutUtfString("id", this->enemyName);
				params->PutUtfString("uid", this->enemyName);
				boost::shared_ptr<IRequest> request(new ExtensionRequest("kckrq", params, lastRoom));
				GameServer::getSingleton().Send(request);
			}
		}
		else{
			boost::shared_ptr<ISFSObject> parameter(new SFSObject());
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL){
				return;
			}
			boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_GAME_LEAVE_REQ, parameter, lastRoom));
			GameServer::getSingleton().Send(request);
		}
	}
	else if (tag == 37)
	{
		this->btnSetting->setEnabled(true);
		LayerCreateRoomChess* layerCreateRoom = LayerCreateRoomChess::create();
		layerCreateRoom->setPopupInGame(true);
		layerCreateRoom->setListUser(this->listUserPlay);
		this->addChild(layerCreateRoom, ZORDER_LIST::ZORDER_POPUP_INGAME, LAYER_POPUP_ID::POPUP_CONFIG_INGAME);
		layerCreateRoom->setConfigFromString(this->paramConfig);
		layerCreateRoom->disableSetting();
	}

}
//update 25/7
void LayerMainGame::checkShowSetting()
{
	bool isChuPhong = GameUtils::isBossRoom(this->listUserPlay);
	if (isChuPhong)
		isShowSetting = true;
	else
		isShowSetting = false;
}

void LayerMainGame::initPanelPos()
{
	//update 25/7
	this->arrEatPosPlayer1.clear();
	this->arrEatPosPlayer2.clear();

	bool isBossRoom = GameUtils::isBossRoom(this->listUserPlay);
	if (isBossRoom || this->isSpectator)
	{
		this->arrEatPosPlayer1 = this->createArrPos(Vec2(-680, 860));
		this->arrEatPosPlayer2 = this->createArrPos(Vec2(-680, 340));
	}
	else if (this->isSpectator == false && !isBossRoom){
		this->arrEatPosPlayer1 = this->createArrPos(Vec2(-680, 340));
		this->arrEatPosPlayer2 = this->createArrPos(Vec2(-680, 860));
	}

}









