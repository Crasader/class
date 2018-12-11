#include "LayerCreateRoom.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Requests/ExtensionRequest.h"

#include "../SceneManager.h"
#include "../AllData.h"
#include "../layergames/_Chat_.h"
#include "../data_manager.h"
#include "../mUtils.h"

#include "../scenes/ScenePickRoom.h"
#include "layergames/ChanUtils.h"

USING_NS_CC;


LayerCreateRoom::LayerCreateRoom() : currID(0)
{
}


LayerCreateRoom::~LayerCreateRoom()
{
}

bool LayerCreateRoom::init()
{
	if (!Layer::init())
	{
		return false;
	}
	createDataCuoc();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupTaoBanChoi.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);
	//add event listenner
	btnCancel = dynamic_cast<Button*>(rootNode->getChildByName("btnCancel"));
	if (btnCancel != NULL){
		btnCancel->addClickEventListener(CC_CALLBACK_1(LayerCreateRoom::onButtonClose, this));
		btnCancel->setPressedActionEnabled(true);
	}
	btnCreate = dynamic_cast<Button*>(rootNode->getChildByName("btnCreate"));
	if (btnCreate != NULL){
		btnCreate->addClickEventListener(CC_CALLBACK_1(LayerCreateRoom::onButtonCreate, this));
		btnCreate->setPressedActionEnabled(true);
	}
	txtPassword = dynamic_cast<TextField*>(rootNode->getChildByName("txtPassword"));
	if (txtPassword)
	{
		mOldPosEditbox = txtPassword->getPosition();
	}
	auto ratio = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratioFreq = visibleSize / DESIGN_RESOLUTION_HEIGHT;

	auto btnMuccuoc = dynamic_cast<Button*>(rootNode->getChildByName("btnMuccuoc"));
	//btnMuccuoc->setAnchorPoint(Point(0.5, 0));
	if (btnMuccuoc != NULL){
		btnMuccuoc->addClickEventListener(CC_CALLBACK_1(LayerCreateRoom::onButtonChonCuoc, this));
	}
	txtBetValue = dynamic_cast<Text*>(rootNode->getChildByName("lblBetValue"));
	if (txtBetValue != NULL){
		txtBetValue->setFontSize(30);
		txtBetValue->setContentSize(Size(402, 30));
	}
	Size sizeAd = ChanUtils::getSizePos();
	lblMinMoney = dynamic_cast<Text*>(rootNode->getChildByName("lblMinMoney"));
	mLayerContainTable = Layout::create();
	mLayerContainTable->setContentSize(Size(402, 500));
	mLayerContainTable->setAnchorPoint(Vec2(0.5, 1));
	mLayerContainTable->setPosition(Vec2(btnMuccuoc->getPositionX() - 20,
		btnMuccuoc->getPositionY() - 160 + sizeAd.height));
	mLayerContainTable->setVisible(false);
	this->addChild(mLayerContainTable);

	
	spriteChonCuoc = ImageView::create();
	spriteChonCuoc->loadTexture("design/guifiles/f-03-03-dropbg.png");
	spriteChonCuoc->setScale9Enabled(true);
	spriteChonCuoc->setCapInsets(Rect(150, 150, 150, 150));
	spriteChonCuoc->setAnchorPoint(Vec2(0, 0));
	spriteChonCuoc->setPosition(Vec2(0, 0));
	spriteChonCuoc->setScaleY(2.2);
	mLayerContainTable->addChild(spriteChonCuoc);

	// 	spriteChonCuoc->setAnchorPoint(Vec2(0.5, 1));
	// 	spriteChonCuoc->setPosition(Point(btnMuccuoc->getPositionX(), btnMuccuoc->getPositionY() + ratio));
	//spriteChonCuoc->setVisible(false);

	tableListCuoc = TableView::create(this, Size(402, 500));
	tableListCuoc->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
	tableListCuoc->setPosition(Vec2(20, 80));
	tableListCuoc->setDelegate(this);
	tableListCuoc->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
	mLayerContainTable->addChild(tableListCuoc, 100);
	tableListCuoc->reloadData();
	//Myself
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	float _amf = 0;
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") != NULL) {
            _amf = (double)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetDoubleValue();
            int gameID = SceneManager::getSingleton().getGameID();
            double betValue = this->getMaxMoneyCreateRoomByBet(gameID, _amf);
            if (betValue == 0)
            {
                betValue = 1000;
                currID = listCuoc.size() - 1;
            }
            txtBetValue->setString(*mUtils::format_money(betValue));
            loadMinMoney4CreateRoom();
        }

    }else{
        if (GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") != NULL) {
            _amf = (double)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs")->GetDoubleValue();
            int gameID = SceneManager::getSingleton().getGameID();
            double betValue = this->getMaxMoneyCreateRoomByBet(gameID, _amf);
            if (betValue == 0)
            {
                betValue = 1000;
                currID = listCuoc.size() - 1;
            }
            txtBetValue->setString(*mUtils::format_money(betValue));
            loadMinMoney4CreateRoom();
        }

    }
	
	return true;
}

void LayerCreateRoom::onButtonClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void LayerCreateRoom::onButtonCreate(Ref* pSender)
{
}


bool LayerCreateRoom::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Vec2 locationInNode = this->convertToNodeSpace(pTouch->getLocation());
   // log("x touch:%f,y touch:%f",locationInNode.x,locationInNode.y);
	Rect rect = Rect(mLayerContainTable->getPositionX() - mLayerContainTable->getContentSize().width/2 + tableListCuoc->getPositionX(),
		mLayerContainTable->getPositionY() + tableListCuoc->getPositionY() - mLayerContainTable->getContentSize().height,
		tableListCuoc->getContentSize().width,
		tableListCuoc->getContentSize().height);
	if (rect.containsPoint(locationInNode))
	{
		
       // log("touch layer");
		return true;
	}
    else if(Rect(btnCreate->getPositionX(),btnCreate->getPositionY(),btnCreate->getContentSize().width,btnCreate->getContentSize().height).containsPoint(locationInNode)){
        
        return true;
    }
	else
	{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX )
		txtPassword->setVisibleEditBox(true);
#endif
        mLayerContainTable->setVisible(false);
		btnCancel->setTouchEnabled(true);
		btnCreate->setTouchEnabled(true);
	}
	return true;
}

void LayerCreateRoom::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);
	listener->onTouchBegan = CC_CALLBACK_2(LayerCreateRoom::onTouchBegan, this);
	dispatcher->addEventListenerWithFixedPriority(listener, -128);
	_touchListener = listener;
}

void LayerCreateRoom::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerCreateRoom::OnSmartFoxRoomCreationError(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	SceneManager::getSingleton().hideLoading();
}

void LayerCreateRoom::onButtonChonCuoc(Ref* pSender)
{
	if (mLayerContainTable->isVisible()){
		mLayerContainTable->setVisible(false);
		spriteChonCuoc->setVisible(false);
		btnCancel->setTouchEnabled(true);
		btnCreate->setTouchEnabled(true);
#if(CC_TARGET_PLATFORM!=CC_PLATFORM_LINUX)
		txtPassword->setVisibleEditBox(true);
#endif
	}
	else{
		mLayerContainTable->setVisible(true);
		spriteChonCuoc->setVisible(true);
		btnCancel->setTouchEnabled(false);
		btnCreate->setTouchEnabled(false);
		tableListCuoc->reloadData();
#if(CC_TARGET_PLATFORM!=CC_PLATFORM_LINUX)
		txtPassword->setVisibleEditBox(false);
#endif
	}
}

void LayerCreateRoom::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}
void LayerCreateRoom::tableCellTouched(TableView* table, TableViewCell* cell)
{
	currID = cell->getIdx();
	CCLOG("cell touched at index: %ld", cell->getIdx());
	txtBetValue->setString((mUtils::convertMoneyEx(listCuoc.at(cell->getIdx()))));
	mLayerContainTable->setVisible(false);
	btnCancel->setTouchEnabled(true);
	btnCreate->setTouchEnabled(true);
	loadMinMoney4CreateRoom();
#if(CC_TARGET_PLATFORM!=CC_PLATFORM_LINUX)
	txtPassword->setVisibleEditBox(true);
#endif
}

Size LayerCreateRoom::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(table->getContentSize().width, 70);
}

TableViewCell* LayerCreateRoom::tableCellAtIndex(TableView *table, ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
	string strValue = mUtils::convertMoneyEx(listCuoc.at(idx));
	if (!cell) {

		cell = new TableViewCell();
		cell->autorelease();

		Label* muccuoc = Label::createWithTTF(strValue, "design/fonts/fonts.ttf", 40);
		muccuoc->setAnchorPoint(Vec2(0, 0));
		muccuoc->setColor(Color3B(117, 100, 60));
		muccuoc->setPosition(Vec2(20, 0));
		muccuoc->setTag(123);
		cell->addChild(muccuoc);
	}
	else
	{
		//Sprite* bg = (Sprite*)cell->getChildByTag(123);
		Label *labelName = (Label*)cell->getChildByTag(123);
		labelName->setString(strValue.c_str());
		//bg->setTexture("design/guifiles/f-02-profile-stats-row.png");
	}

	return cell;
}

ssize_t LayerCreateRoom::numberOfCellsInTableView(TableView *table)
{
	return listCuoc.size();
}

void LayerCreateRoom::createDataCuoc()
{
	int gameID = SceneManager::getSingleton().getGameID();
	if (gameID == kGameCoTuong || gameID == kGameCoUp)
	{
		this->listCuoc.push_back(500000);
		this->listCuoc.push_back(200000);
		this->listCuoc.push_back(100000);
		this->listCuoc.push_back(50000);
		this->listCuoc.push_back(20000);
		this->listCuoc.push_back(10000);
		this->listCuoc.push_back(5000);
		this->listCuoc.push_back(2000);
		this->listCuoc.push_back(1000);
	}
	else
	{
		this->listCuoc.push_back(200000);
		this->listCuoc.push_back(100000);
		this->listCuoc.push_back(50000);
		this->listCuoc.push_back(20000);
		this->listCuoc.push_back(10000);
		this->listCuoc.push_back(5000);
		this->listCuoc.push_back(2000);
		this->listCuoc.push_back(500);
		this->listCuoc.push_back(100);
	}

}

void LayerCreateRoom::loadMinMoney4CreateRoom()
{
	int gameID = SceneManager::getSingleton().getGameID();
	long nCuoc = listCuoc.at(currID);
	lblMinMoney->setString("Cần tối thiểu: "
		+ *mUtils::format_money(mUtils::getMinMoneyCreateRoomByBet(gameID, nCuoc)));
}

int LayerCreateRoom::getMaxMoneyCreateRoomByBet(int gameID, double currBet)
{
	double betUser;
	switch (gameID)
	{
	case kGameChan:
		betUser = currBet / 100;
		break;
	case kGameCoTuong:
		betUser = currBet / 5;
		break;
	case kGameCoUp:
		betUser = currBet / 5;
		break;
	case kGameSam:
		betUser = currBet / 100;
		break;
	case kGamePhom:
		betUser = currBet / 60;
		break;
	case kGameTLMNDemLa:
		betUser = currBet / 100;
		break;
	default:
		betUser = 0;
		break;
	}
	double maxMoney = 0;
	for (int i = 0; i < listCuoc.size(); ++i)
	{
		if (i < listCuoc.size() - 1)
		{
			if (listCuoc.at(i) > betUser && betUser > listCuoc.at(i + 1))
			{
				maxMoney = listCuoc.at(i + 1);
				currID = i + 1;
				break;
			}
			else if (betUser > listCuoc.at(0))
			{
				maxMoney = listCuoc.at(0);
				currID = 0;
				break;
			}
		}
	}
	return maxMoney;
}
