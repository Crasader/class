#include "BetXocDia.h"
#include "TaiXiuMessageDefs.h"
#include "mUtils.h"
#include "GameServer.h"
#include "../Common.h"
#define _colorTextWHITE Color3B::WHITE
#define _colorTextNormal Color3B(255, 227, 160)
#define _colorTextInButton Color3B(123, 1, 0)
#define _colorTextInPopUp Color3B(68, 63, 47)
bool BetXocDiaChanLe::init()
{
	if (!Button::init())
		return false;
	string str;
	Vec2 pos = Vec2::ZERO;
	if (0 == type.compare(BET_TYPE_CHAN))
	{
		str.append(dataManager.GetSysString(536));
		pos.set(304 / 2, 45);
	}
	else if (0 == type.compare(BET_TYPE_LE))
	{
		str.append(dataManager.GetSysString(537));
		pos.set(304 / 2, 45);
	}

	if (0 == type.compare(BET_TYPE_CHAN) || 0 == type.compare(BET_TYPE_LE))
	{
		Label *lbl = Label::createWithSystemFont(str, "", 40);
		lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		lbl->setSystemFontSize(50);
		lbl->setPosition(Vec2(304 / 2, 304 / 2));
		lbl->setColor(ccc3(225, 225, 225));
		this->addChild(lbl);

		Label* lbl2 = Label::createWithBMFont("robotofont.fnt", "(1x1)");
		lbl2->setAnchorPoint(Point(0.5, 0.5));
		lbl2->setPosition(pos);
		lbl2->setBMFontSize(45);
		lbl2->setColor(ccc3(100, 185, 102));
		this->addChild(lbl2);
	}

	return true;
}

BetXocDiaChanLe *BetXocDiaChanLe::create(string _type)
{
	BetXocDiaChanLe *betXocDia = new BetXocDiaChanLe(_type);
	if (betXocDia && betXocDia->init()){
		betXocDia->autorelease();
		return betXocDia;
	}
	else{
		delete betXocDia;
		betXocDia = NULL;
		return NULL;
	}
}

BetXocDiaChanLe::BetXocDiaChanLe(string _type)
{
	type = _type;
}

BetXocDiaChanLe::~BetXocDiaChanLe()
{

}

bool BetXocDiaDiem::init()
{
	if (!Button::init())
		return false;
	vector<string> arr;
	string str = "";
	if (0 == type.compare(BET_TYPE_CAP_0_4))
	{
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-yellow.png");
		str.append("(1x6)");
	}
	else if (0 == type.compare(BET_TYPE_CAP_1_3))
	{
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-yellow.png");
		arr.push_back("xocdia-item-red.png");
		str.append("(1x3)");
	}
	else if (0 == type.compare(BET_TYPE_CAP_3_1))
	{
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-yellow.png");
		str.append("(1x3)");
	}
	else if (0 == type.compare(BET_TYPE_CAP_4_0))
	{
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-red.png");
		arr.push_back("xocdia-item-red.png");
		str.append("(1x6)");
	}

	for (int i = 0; i < arr.size(); i++)
	{
		Sprite* img = Sprite::create(arr.at(i));
		img->setAnchorPoint(Vec2::ZERO);
		if (i == 0 || i == arr.size() - 1){
			img->setPosition(Vec2(209 / 2 - 35 + 17 * i, 10));
		}
		else{
			img->setPosition(Vec2(209 / 2 - 35 + 17 * i, 5));
		}
		this->addChild(img);
	}

	Label* lbl = Label::createWithBMFont("robotofont.fnt", str);
	lbl->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lbl->setPosition(Vec2(209 / 2, 45));
	lbl->setBMFontSize(45);
	//lbl->setScale(0.9);
	lbl->setColor(ccc3(100, 185, 102));
	this->addChild(lbl);

	return true;
}

BetXocDiaDiem::BetXocDiaDiem(string _type)
{
	type = _type;
}

BetXocDiaDiem::~BetXocDiaDiem()
{

}

BetXocDiaDiem* BetXocDiaDiem::create(string _type)
{
	BetXocDiaDiem *betXocDia = new BetXocDiaDiem(_type);
	if (betXocDia && betXocDia->init()){
		betXocDia->autorelease();
		return betXocDia;
	}
	else{
		delete betXocDia;
		betXocDia = NULL;
		return NULL;
	}
}
