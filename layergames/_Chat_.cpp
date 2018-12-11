
#include "_Chat_.h"
#include "mUtils.h"
#include "PhomMessDef.h"
#include "ChanUtils.h"

enum {
	kTagLabel,
	kOrderLabel
};

Chat::Chat(const string& pMes, const int& posUser, const bool& isChan)
:mes(pMes),
posUser(posUser),
isChanGame(isChan)
{

}

Chat::Chat(const string& pMes, const string& chatFrame) :
mes(pMes),
chatFrame(chatFrame),
posUser(0),
isChanGame(false)
{

}

Chat* Chat::create(const string& pMes, const int& posUser, const bool& isChan){
	Chat* chat = new Chat(pMes, posUser, isChan);
	if (chat && chat->init()){
		chat->autorelease();
		return chat;
	}
	else
	{
		delete chat;
		chat = NULL;
		return NULL;
	}
}

Chat* Chat::create(const string& pMes, const string& chatFrame)
{
	Chat* chat = new Chat(pMes, chatFrame);
	if (chat && chat->init())
	{
		chat->autorelease();
		return chat;
	}
	else
	{
		delete chat;
		chat = NULL;
		return NULL;
	}
}

Chat::Chat()
{
	this->mes = "";
	this->posUser = -1;
	this->isChanGame = false;
}



bool Chat::init() {

	if (!Layer::init()) {
		return false;
	}

	// 	for (int i = 0; i < 12; i++) {
	// 		if (strstr(mes.c_str(), listemo[i].c_str())){
	// 			emo = Sprite::create(StringUtils::format("emotions/chat%d.png", i + 1));
	// 			emo->setScale(2);
	// 
	// 			Vec2 point = Vec2::ZERO;
	// 
	// 			if (posUser == -1) {
	// 				point = Vec2(WIDTH_DESIGN / 2 - sizeThis.width / 2, HEIGHT_DESIGN / 4);
	// 			}
	// 
	// 			if (this->isChanGame){
	// 				point = Vec2(0.5, 0.5);
	// 			}
	// 
	// 			emoChat = true;
	// 			emo->setPosition(point);
	// 			emo->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(true), nullptr));
	// 			this->setLocalZOrder(100);
	// 			this->addChild(emo);
	// 
	// 			log("Here emotion");
	// 
	// 			return true;
	// 		}
	// 	}

	//neu la game chan, chi hien thi emotion.
	if (this->isChanGame)
		return false;
	//int MAX_LENTH_STR = 11;//qua 20ki tu se xuong dong
	this->setAnchorPoint(Vec2(0.5, 0));
	this->setPosition(Vec2(0, 0));
	Vec2 point = Vec2::ZERO;
    
    this->setTag(9153);

	Label *label = Label::createWithSystemFont(mes, "", FONT_SIZE_NORMAL+5);
	Size sizeDesign = Size(214, 97);
	//int gameID = SceneManager::getSingleton().getGameID();
	// 	if (gameID != kGameChan && gameID != kGameXiTo && gameID != kGameTaiXiu && gameID > 0)
	// 	{
	// 		label->setMaxLineWidth(300);
	// 	}
	// 	if (mes.size() > MAX_LENTH_STR)
	// 	{
	// 		
	// 		//label->setDimensions(MAX_LENTH_STR * FONT_SIZE_NORMAL, FONT_SIZE_NORMAL * 2);
	// 		//sizeDesign.setSize(label->getContentSize().width, label->getContentSize().height);
	// 	}
	//cheat check for long mess

	label->setHorizontalAlignment(TextHAlignment::CENTER);
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setLocalZOrder(1000000);
	label->setColor(Color3B::WHITE);
	float wLabel = label->getContentSize().width;
	float hLabel = label->getContentSize().height;
	Size winsize = Director::getInstance()->getVisibleSize();
	// background_a
	Rect rectBatchNode = Rect(10, 10, sizeDesign.width - 20, sizeDesign.height - 20);
	Rect origRect = Rect(0, 0, sizeDesign.width, sizeDesign.height);
	bool isChatOnGame = false;
	if (chatFrame.empty())
	{
		chatFrame = "alert_status.png";
	}
	else{
		isChatOnGame = true;
		sizeDesign = Size(248, 187);
		rectBatchNode = Rect(sizeDesign.width, 0, 0, 0);
		origRect = Rect(0, 0, sizeDesign.width, sizeDesign.height);
	}

	Sprite *batchNode = Sprite::create(chatFrame);
	blocks = ui::Scale9Sprite::create();
	blocks->updateWithSprite(batchNode, origRect, false, rectBatchNode);
	emoChat = false;
	this->sizeThis = Size(wLabel + 10, hLabel + 5);
	if (isChatOnGame)
	{
		label->setMaxLineWidth(sizeDesign.width);
		label->setLineBreakWithoutSpace(false);
		blocks->setAnchorPoint(Point(0.5, 0.5));
		label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		label->setPosition(Vec2(blocks->getContentSize().width / 2, blocks->getContentSize().height / 1.5));
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
			label->setTextColor(Color4B::BLACK);
	}
	else{
		blocks->setContentSize(sizeThis);
		blocks->setAnchorPoint(Point(0.5, 0));
		label->setPosition(Vec2(blocks->getContentSize().width / 2, blocks->getContentSize().height / 2));
	}

	Size sizeAd = ChanUtils::getSizePos();
	if (posUser == -1){
		point = Vec2(winsize.width / 2, HEIGHT_DESIGN / 2.5 + sizeAd.height);
	}

	blocks->addChild(label);
	blocks->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(true), nullptr));
	this->setPosThis(point);
	this->setLocalZOrder(100);
	this->addChild(blocks);
	return true;
}

void Chat::setStatusByServer(bool isServer) {
	Label *label = (Label*) this->getChildByTag(kTagLabel);

	if (label == NULL)
		return;

	//     if (isServer) {
	//         label->setColor(Color3B::WHITE);
	//     }
	//     else {
	//         label->setColor(Color3B::WHITE);
	//     }
}


Size Chat::getSize() {
	return this->sizeThis;
}

void Chat::formatMes() {
	int MAX_LENGTH_LINE = 30;
	int length = this->mes.length();

	int row = length / MAX_LENGTH_LINE;

	if (row > 1) {
		string newMes = "";
		int startSub, endSub;

		for (int i = 0; i <= row; i++) {
			startSub = i * MAX_LENGTH_LINE;
			endSub = (i + 1) * MAX_LENGTH_LINE;

			if (endSub > length) {
				endSub = length;
			}

			newMes += " " + this->mes.substr(startSub, endSub - startSub) + " ";
			if (i < row) newMes += "\n";
		}

		this->mes = newMes;
	}
	else {
		this->mes = "  " + this->mes + "  ";
	}

}

void Chat::releaseSafe() {
	this->removeFromParentAndCleanup(true);
}

Chat::~Chat()
{
	log("Chat Destructor");
}

void Chat::setPosThis(Vec2 point)
{
	if (emoChat){
		emo->setPosition(point);
		emo->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.1, Vec2(point.x + 5, point.y)), MoveTo::create(0.1, Vec2(point.x - 5, point.y)), NULL)));
	}

	else
		blocks->setPosition(point);
}
