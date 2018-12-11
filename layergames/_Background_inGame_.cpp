#include "_Background_inGame_.h"
#include "AllData.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"

bool BackgroundInGame::init() {
	if (!Layer::init())
		return false;
	int FONT_SIZE_TITLE = 50;
	int FONT_SIZE_NORMAL = 40;
	this->setAnchorPoint(Vec2(0, 0));
	int gameID = SceneManager::getSingleton().getGameID();
	Size sizeAdd = ChanUtils::getSizePos();
	Size winsize = Director::getInstance()->getVisibleSize();
	if (gameID == kGameXiTo || gameID == kGameLieng || gameID == kGameBaCayChuong){
		backgroundGame = Sprite::create("moc3-bg-game.jpg");
        backgroundGame->setAnchorPoint(Vec2(0.5, 0.5));
        backgroundGame->setScaleX(winsize.width / backgroundGame->getContentSize().width);
        backgroundGame->setScaleY(winsize.height / backgroundGame->getContentSize().height);
        backgroundGame->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
        
        this->addChild(backgroundGame);
        
        Sprite* table = Sprite::create("girl-table.png");
        table->setAnchorPoint(Vec2(0.5, 0.5));
        table->setPosition(Vec2(winsize.width / 2, winsize.height / 2));

        this->addChild(table);
	}
	else{
        backgroundGame = Sprite::create("moc3-bg-game.jpg");
        backgroundGame->setAnchorPoint(Vec2(0.5, 0.5));
        backgroundGame->setScaleX(winsize.width / backgroundGame->getContentSize().width);
        backgroundGame->setScaleY(winsize.height / backgroundGame->getContentSize().height);
        backgroundGame->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
        
        this->addChild(backgroundGame);
        
        Sprite* table = Sprite::create("nogirl-table.png");
        table->setAnchorPoint(Vec2(0.5, 0.5));
        table->setPosition(Vec2(winsize.width / 2, winsize.height / 2));
        
        this->addChild(table);
	}

	

	/* moc3-remove

	 Sprite* bg_top = Sprite::create("bg_top.png");
	 bg_top->setAnchorPoint(Vec2(0,1));
	 bg_top->setPosition(Vec2(0,winsize.height + sizeAdd.height));
	 this->addChild(bg_top);

	 Sprite* bg_bot = Sprite::create("bg_bot.png");
	 bg_bot->setAnchorPoint(Vec2(0,0));
	 bg_bot->setPosition(Vec2(0,0));
	 this->addChild(bg_bot);

	 string tablePath = "sam-table.png";


	 int sizeAddBg = 0;
	 if (SceneManager::getSingleton().getGameID() == kGameBaCay || SceneManager::getSingleton().getGameID() == kGameBaCayChuong) {
	 tablePath = "sam-table.png";
	 if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	 tablePath = "xito-table.png";

	 }
	 else if (SceneManager::getSingleton().getGameID() == kGameLieng)
	 {
	 bg_top->setTexture("bg_top.png");
	 backgroundGame->setTexture("bg-xito.jpg");
	 bg_bot->setVisible(false);
	 tablePath = "xito-table.png";
	 if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	 {
	 sizeAddBg = 100;
	 }
	 }

	 //}

	 Sprite *backgroundTable = Sprite::create(tablePath);
	 backgroundTable->setAnchorPoint(Vec2(0.5, 0.5));
	 backgroundTable->setPosition(Vec2(winsize.width / 2, winsize.height / 2 + sizeAddBg));
	 this->addChild(backgroundTable);

	 if (SceneManager::getSingleton().getGameID() == kGameLieng){
	 backgroundTable->setPosition(Vec2(winsize.width / 2, winsize.height / 2 + sizeAddBg +100));

	 }
	 */
	int w_btn_setting = 200;
	int space = 10;

	labelGame = Label::createWithSystemFont("", "", FONT_SIZE_NORMAL);
	labelGame->setHorizontalAlignment(TextHAlignment::LEFT);
	labelGame->setVerticalAlignment(TextVAlignment::CENTER);
	labelGame->setAnchorPoint(Vec2(0, 0.5));
	labelGame->setPosition(Vec2(w_btn_setting + sizeAdd.width, winsize.height - FONT_SIZE_NORMAL - 20));

	Sprite *iconCoin = Sprite::create("moc3-icon-hao.png");

	int roomType = SceneManager::getSingleton().getCurrRoomType();

	if (roomType == 1){ //1- hao 0- moc
		iconCoin->setTexture("moc3-icon-hao.png");
		if (SceneManager::getSingleton().getGameID() != kGameLieng){
			/*backgroundGame->setTexture("background-gold.jpg");
			backgroundTable->setTexture("table-gold.png");*/
		}
	}
	else{
		iconCoin->setTexture("moc3-icon-moc.png");
	}

	iconCoin->setAnchorPoint(Vec2(0, 0.5));
	iconCoin->setPosition(Vec2(w_btn_setting + sizeAdd.width,
		winsize.height - FONT_SIZE_NORMAL * 2 - space * 3));

	labelCuoc = Label::createWithSystemFont("", "", FONT_SIZE_NORMAL,
		Size(400, 200),
		TextHAlignment::LEFT, TextVAlignment::CENTER);
	labelCuoc->setAnchorPoint(Vec2(0, 0.5));
	labelCuoc->setPosition(w_btn_setting + 50 + sizeAdd.width,
		winsize.height - FONT_SIZE_NORMAL * 2 - space * 3 - 7);
	labelGame->setColor(Color3B::WHITE);
	labelCuoc->setColor(Color3B(241, 205, 99));

	this->addChild(iconCoin);
	this->addChild(labelCuoc);
	this->addChild(labelGame);

	if (SceneManager::getSingleton().getGameID() == kGameLieng)
	{
		Sprite *totalFrame = Sprite::create("frame_totalbet.png");
		totalFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		totalFrame->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 + sizeAdd.height));
		totalFrame->setTag(246);
		totalFrame->setVisible(false);
		this->addChild(totalFrame);
		Label* lblTotal = Label::createWithSystemFont(dataManager.GetSysString(659), "", 40, Size(150, 90), TextHAlignment::CENTER, TextVAlignment::CENTER);
		lblTotal->setColor(Color3B::WHITE);
		lblTotal->setAnchorPoint(Vec2(0, 0));
		lblTotal->setPosition(Vec2(20, totalFrame->getContentSize().height / 2 - lblTotal->getContentSize().height / 2));
		totalFrame->addChild(lblTotal);

		lblTotalBet = Label::createWithSystemFont("", "", 40,
			Size(250, 90), TextHAlignment::CENTER, TextVAlignment::CENTER);
		lblTotalBet->setColor(Color3B::YELLOW);
		lblTotalBet->setAnchorPoint(Vec2(0, 0));
		lblTotalBet->setPosition(Vec2(lblTotal->getPositionX() + lblTotal->getContentSize().width - 25.0, lblTotal->getPositionY()));
		totalFrame->addChild(lblTotalBet);
	}

	return true;
}

BackgroundInGame::BackgroundInGame()
{
	log("BackgroundInGame Constructor");
}

BackgroundInGame::~BackgroundInGame()
{
	log("BackgroundInGame Destructor");
}


void BackgroundInGame::setNameTable(string _nameGame, int idRoom, double moneyBet)
{
	labelGame->setString(StringUtils::format("%s - P: %d", _nameGame.c_str(), idRoom));
	labelCuoc->setString(formatMoneySymbol(moneyBet));
}

void BackgroundInGame::setTotalBet(double _value)
{
	string strVal = mUtils::convertMoneyEx(_value);
	lblTotalBet->setString(strVal);
}

void BackgroundInGame::hideTotalBet(bool isHide)
{
	auto frameBet = this->getChildByTag(246);
	if (frameBet != NULL)
		frameBet->setVisible(true);
	frameBet->setVisible(!isHide);
}

