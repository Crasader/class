//
//  _Chess_.cpp
//  iCasinov2
//
//  Created by Thanhhv on 6/5/14.
//
//

#include "Chess.h"
#include "MyAnimation.h"

Chess::Chess(int side, int name, int id_pos)
:side(side),
name(name),
id_pos(id_pos),
id_pos_init(id_pos)
{
}

Chess::Chess(int side, int name, int id_pos, CHESS_TYPE type)
:side(side),
name(name),
id_pos(id_pos),
id_pos_init(id_pos),
type(type)
{

}

bool Chess::init(){
	if (!Sprite::init())
		return false;
	this->initWithSpriteFrameName(getUrlByType(type).c_str());
	this->setInitName(this->getName());
	//this->setScale(1.18);
	this->setLocalZOrder(9);
	shadowSprite = NULL;
	_streak = NULL;
	isUndoMove = false;

	justPoint = Sprite::create("vong-luu-nuoc-di.png");
	justPoint->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	justPoint->setVisible(false);
	this->addChild(justPoint);

	return true;
}

Chess::~Chess(){
}


// setter, getter
void Chess::setIDName(int id) {
	this->id_name = id;
}

int  Chess::getIDName() {
	return this->id_name;
}

void Chess::setIDPos(int id) {
	this->id_pos = id;
}

int  Chess::getIDPos() {
	return this->id_pos;
}

int  Chess::getIDPosInit() {
	return this->id_pos_init;
}

int  Chess::getName() {
	return this->name;
}



int  Chess::getSide() {
	return this->side;
}

void Chess::setClick(bool isClick) {
	if (isClick) {
		//spHover->setVisible(true);
		this->setColor(Color3B(255, 77, 0));
	}
	else {
		//spHover->setVisible(false);
		this->setColor(Color3B::WHITE);
	}
}

Chess* Chess::create(int side, int name, int id_pos)
{
	Chess* chess = new Chess(side, name, id_pos);
	if (chess && chess->init()){
		chess->autorelease();
		return chess;
	}
	else
	{
		delete chess;
		chess = NULL;
		return NULL;
	}
}

Chess* Chess::create(int side, int name, int id_pos, CHESS_TYPE type)
{
	Chess* chess = new Chess(side, name, id_pos, type);
	if (chess && chess->init()){
		chess->autorelease();
		return chess;
	}
	else
	{
		delete chess;
		chess = NULL;
		return NULL;
	}
}

bool Chess::isFolded()
{
	return this->name == FOLD;
}

void Chess::setName(int id)
{
	this->name = id;
	//update 25/7
	if (this->name < 0)
		return;
	this->initWithSpriteFrameName(getUrlByType(type).c_str());
}

void Chess::setRawName(int id)
{
	rawName = id;
}

int Chess::getRawName()
{
	return rawName;
}

string Chess::getURL() {
	string sideString;
	string nameString;

	switch (this->side) {
	case 1:
		sideString = "Black-";
		break;
	case 2:
		sideString = "Red-";
		break;
	}

	/*int PAWN = 0;   // tốt
	int BISHOP = 1;   // sỹ
	int ELEPHANT = 2; // tượng
	int KNIGHT = 3;   // mã
	int CANNON = 4;	  // pháo
	int ROOK = 5;	  // xe
	int KING = 6;	  // tướng
	int EMPTY = 7;*/
	switch (this->name) {
	case PAWN:
		nameString = "Tot";
		break;
	case BISHOP:
		nameString = "Si";
		break;
	case KING:
		nameString = "TUONG";
		break;
	case KNIGHT:
		nameString = "Ma";
		break;
	case CANNON:
		nameString = "Phao";
		break;
	case ROOK:
		nameString = "Xe";
		break;
	case ELEPHANT:
		nameString = "Tuongj";
		break;
	case FOLD:
		nameString = "COUP";
		break;
	}
	if ((int)FOLD == this->name && type != EMPTY_TYPE)
	{
		return nameString + ".png";
	}
	return sideString + nameString + ".png";
}

std::string Chess::getUrlByType(const CHESS_TYPE& type /*= 0*/)
{
	/************************************************************************/
	/*type = 0 - normal, 1- cờ đá, 2 cờ gỗ, 3 cờ đồng, 4 cờ sắt, 5 cờ bạc
	6 cờ vàng, 7 cờ thủy tinh, 8 cờ lửa                      */
	/************************************************************************/
	string sideString;
	string nameString;
	string typeString = "";
	switch (type)
	{
	case SIMPLE:
		break;
	case STONE:
		typeString = "Coda-";
		break;
	case WOODEN:
		typeString = "Cogo-";
		break;
	case COPPER:
		typeString = "Copper-";
		break;
	case IRON:
		typeString = "Cosat-";
		break;
	case SILVER:
		typeString = "Silver-";
		break;
	case GOLD:
		typeString = "Gold-";
		break;
	case GLASS:
		typeString = "Thuytinh-";
		break;
	case FIRE:
		typeString = "Fire-";
		break;
	default:
		break;
	}

	switch (this->side) {
	case BLACK:
		sideString = "Black-";
		break;
	case RED:
		sideString = "Red-";
		break;
	}

	/*int PAWN = 0;   // tốt
	int BISHOP = 1;   // sỹ
	int ELEPHANT = 2; // tượng
	int KNIGHT = 3;   // mã
	int CANNON = 4;	  // pháo
	int ROOK = 5;	  // xe
	int KING = 6;	  // tướng
	int EMPTY = 7;*/
	switch (this->name) {
	case PAWN:
		nameString = "Tot";
		break;
	case BISHOP:
		nameString = "Si";
		break;
	case KING:
		nameString = "TUONG";
		break;
	case KNIGHT:
		nameString = "Ma";
		break;
	case CANNON:
		nameString = "Phao";
		break;
	case ROOK:
		nameString = "Xe";
		break;
	case ELEPHANT:
		nameString = "Tuongj";
		break;
	case FOLD:
		nameString = "COUP";
		break;
	}
	// 	if ((int)FOLD == this->name && type != SIMPLE)
	// 	{
	// 		return typeString + nameString + ".png";
	// 	}
	return typeString + sideString + nameString + ".png";
}

void Chess::setImageChess(CHESS_TYPE type)
{
	this->setTypeChess(type);
	//log("frame name %s", this->getUrlByType(type).c_str());
	SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(this->getUrlByType(type));
	if (spriteFrame != NULL)
		this->setSpriteFrame(spriteFrame);
}

void Chess::setIDPosInit(int id)
{
	this->id_pos_init = id;
}

std::string Chess::getStr()
{
	string str = "";
	ostringstream oss1;
	oss1 << this->getName();
	str += oss1.str() + "_";

	ostringstream oss2;
	int sideRevert = this->getSide() == 1 ? 2 : 1;
	oss2 << sideRevert;
	str += oss2.str() + "_";

	ostringstream oss3;
	oss3 << this->getPosConfig();
	str += oss3.str();

	return str;

	switch (this->getName()) {
	case PAWN:
		str = "0";
		break;
	case BISHOP:
		str = "1";
		break;
	case ELEPHANT:
		str = "2";
		break;
	case KNIGHT:
		str = "3";
		break;
	case CANNON:
		str = "4";
		break;
	case ROOK:
		str = "5";
		break;
	case KING:
		str = "6";
		break;
	case EMPTY:
		str = "7";
		break;

	default: str = ""; break;
	}
}

std::string Chess::getMapChess()
{
	string str = "";
	ostringstream oss1;
	oss1 << this->getName();
	str += oss1.str() + "_";

	ostringstream oss2;
	oss2 << this->getSide();
	str += oss2.str() + "_";

	ostringstream oss3;
	oss3 << this->getIDPos();
	str += oss3.str();

	return str;

	switch (this->getName()) {
	case PAWN:
		str = "0";
		break;
	case BISHOP:
		str = "1";
		break;
	case ELEPHANT:
		str = "2";
		break;
	case KNIGHT:
		str = "3";
		break;
	case CANNON:
		str = "4";
		break;
	case ROOK:
		str = "5";
		break;
	case KING:
		str = "6";
		break;
	case EMPTY:
		str = "7";
		break;

	default: str = ""; break;
	}
}

void Chess::setSide(int _side)
{
	this->side = _side;
}

void Chess::showKingCheck(bool isCheck, Vec2 destPos)
{
	if (isCheck)
	{
		MyAnimation::createActionTintLoop(this, Color3B(255, 97, 2));
	}
	else{
		this->stopAllActions();
		if (this->shadowSprite != NULL)
		{
			this->shadowSprite->setPosition(Vec2(this->getPosition().x + 15,
				this->getPosition().y - 20));
		}
	}
}

void Chess::showIsChecked(bool isChecked)
{
	if (isChecked)
	{
		MyAnimation::createActionTintLoop(this, Color3B(255, 77, 0));
	}
	else{
		this->stopAllActions();
		if (this->shadowSprite != NULL)
		{
			this->shadowSprite->setPosition(Vec2(this->getPosition().x + 15,
				this->getPosition().y - 20));
		}
	}
}

void Chess::moveChess(Vec2 dest)
{
	if (this->getPosition().equals(dest))
		return;
	this->shadowSprite->setVisible(false);
	this->runAction(Sequence::create(EaseIn::create(MoveTo::create(0.2, dest), 0.3),
		CallFunc::create(CC_CALLBACK_0(Chess::callbackAnim, this)), NULL));
}

void Chess::addMoveSprite()
{
	auto parent = this->getParent();
	if (parent && _streak == NULL && shadowSprite == NULL)
	{
		//add sprite shadow
		this->shadowSprite = Sprite::createWithSpriteFrameName("bong-quan-co.png");
		this->shadowSprite->setAnchorPoint(this->getAnchorPoint());
		this->shadowSprite->setPosition(Vec2(this->getPosition().x + 10,
			this->getPosition().y - 10));
		this->shadowSprite->setLocalZOrder(8);
		parent->addChild(this->shadowSprite);
		//add motion streak
		this->_streak = MotionStreak::create(0.5f, 10, 50, Color3B::WHITE, "hieu-ung-quan-co-di-chuyen.png");
		this->_streak->setFastMode(false);
		this->_streak->followNode(this, Vec2(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f));
		parent->addChild(this->_streak);
	}
}
void Chess::addRecord(int _idPos, float _uiPosX, float _uiPosY)
{
	ChessRecord _record;
	_record.oldID = _idPos;
	_record.uiPosX = _uiPosX;
	_record.uiPosY = _uiPosY;
	this->listRecord.push_back(_record);
}

void Chess::clearRecord()
{
	this->listRecord.clear();
}

void Chess::undoMove()
{
	if (this->listRecord.size() < 1)
		return;
	isUndoMove = true;
	ChessRecord lastRecord = this->listRecord.at(this->listRecord.size() - 1);
	Vec2 oldPos = Vec2(lastRecord.uiPosX, lastRecord.uiPosY);
	this->setIDPos(lastRecord.oldID);
	this->setVisible(true);
	this->moveChess(oldPos);
	this->listRecord.pop_back();
}

void Chess::callbackAnim()
{
	this->shadowSprite->setVisible(true);
	this->shadowSprite->setPosition(Vec2(this->getPosition().x + 15,
		this->getPosition().y - 20));
	if (!isUndoMove)
		justPoint->setVisible(true);
	else
		isUndoMove = false;
	//justPoint->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
}

void Chess::hideShadow(bool isHide)
{
	if (shadowSprite == NULL)
		return;
	shadowSprite->setVisible(!isHide);
	if (!isHide)
	{
		this->shadowSprite->setPosition(Vec2(this->getPosition().x + 15,
			this->getPosition().y - 20));
	}
}

void Chess::hideJustPoint(bool isHide)
{
	justPoint->setVisible(!isHide);
}

