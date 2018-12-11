#include "Nan3Cay.h"
#include <algorithm>
#include "mUtils.h"
#include "AllData.h"
#include "ChanUtils.h"
#include "SceneManager.h"
#include "PhomMessDef.h"
Nan3Cay::Nan3Cay()
{
	arrCardTypes.clear();
	CardOnHand.clear();
	log("Nan3Cay Constructor");
}

Nan3Cay::~Nan3Cay()
{
	arrCardTypes.clear();
	CardOnHand.clear();
	log("Nan3Cay Destructor");
}

bool Nan3Cay::init()
{
	if (!Layer::init())
		return false;

	this->setAnchorPoint(Point(0, 0));
	this->setPosition(Point(0, 0));
	this->sizeScreen = Director::getInstance()->getVisibleSize();
	auto bg = ImageView::create("moc3-bg-nan-bai.png");
	bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bg->setPosition(Point(this->sizeScreen / 2));
	this->addChild(bg);

	arrCardTypes.push_back("s");
	arrCardTypes.push_back("c");
	arrCardTypes.push_back("d");
	arrCardTypes.push_back("h");

	sizePos = ChanUtils::getSizePos();

	float scaleX = sizeScreen.width / WIDTH_DESIGN;
	float scaleY = sizeScreen.height / HEIGHT_DESIGN;
	float scaleMin = (scaleX < scaleY) ? scaleX : scaleY;
	scaleApp = scaleMin;

	startLeft = (sizeScreen.width - WIDTH_DESIGN) / 2;
	startTop = (sizeScreen.height - HEIGHT_DESIGN) / 2;

	dx = 0;
	dy = 0;
	_index = -1;
	_where = -1;
	isTouched = false;

	deltaX = 0;
	deltaY = 0;
	sX = 0;
	sY = 0;
	xcu = 0;
	ycu = 0;

	cardIndex = 2;

	//Touch move
	cardIndex_under = 0;
	flag_move = false;
	flag_left = flag_right = -1;
	count_move = 0;

	//touch end
	vt = -1;


	//	auto tipNan3Cay = Sprite::create("3cay-tips.png");
	//	this->addChild(tipNan3Cay);
	//	tipNan3Cay->setAnchorPoint(Vec2(0, 1));
	//	tipNan3Cay->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN - 100 + sizePos.height));
	//	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
	//		tipNan3Cay->setVisible(false);

	this->bottomCard = HEIGHT_DESIGN / 2 - 280;
	this->leftCard = WIDTH_DESIGN / 2 - 214;
	this->rightCard = this->leftCard + 214;

	this->widthCard = 428;
	this->heightCard = 560;

	return true;
}

Point& Nan3Cay::ConvertPoint(Point& pPoint)
{
	float xCenter = this->sizeScreen.width / 2;
	float yCenter = this->sizeScreen.height / 2;

	float xTap = pPoint.x;
	float yTap = pPoint.y;

	float x1, x2;
	float y1, y2;

	float A = ((powf(xCenter - xTap, 2) + powf(yCenter - yTap, 2))) / powf(scaleApp, 2);
	float B = powf((yCenter - yTap) / (xCenter - xTap), 2) + 1;

	x1 = xCenter + sqrtf(A / B);
	x2 = xCenter - sqrtf(A / B);

	y1 = yCenter + (yCenter - yTap) * (x1 - xCenter) / (xCenter - xTap);
	y2 = yCenter + (yCenter - yTap) * (x2 - xCenter) / (xCenter - xTap);

	// "điểm cần convert" = A
	// Bởi vì A và Tap nằm ở cùng 1 phía so với Center nên nếu xTap < xCenter thì xA < xCenter và ngược lại
	if ((xTap < xCenter && x1 < xCenter) || (xTap > xCenter && x1 > xCenter))
	{
		x1 -= startLeft;
		y1 -= startTop;

		pPoint.x = x1;
		pPoint.y = y1;
	}
	else if ((xTap < xCenter && x2 < xCenter) || (xTap > xCenter && x2 > xCenter))
	{
		x2 -= startLeft;
		y2 -= startTop;

		pPoint.x = x2;
		pPoint.y = y2;
	}
	else
	{
		log("No define POINT CONVERT");
	}

	return pPoint;
}

void Nan3Cay::InitListCardHand(string& lsCards)
{

	Label* lblNanbai = Label::createWithSystemFont(dataManager.GetSysString(280), "", 60, Size(750, 200), TextHAlignment::CENTER, TextVAlignment::CENTER);
	lblNanbai->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lblNanbai->setColor(Color3B::WHITE);
	lblNanbai->setPosition(Point(this->sizeScreen.width / 2, this->sizeScreen.height / 2 + this->sizeScreen.height / 3));
	this->addChild(lblNanbai);

	log("lsCards: %s", lsCards.c_str());
	vector<string> arr = mUtils::splitString(lsCards, '-');
	for (unsigned short i = 0; i < arr.size(); ++i)
	{
		vector<string> info = mUtils::splitString(arr[i], '_');
		string SrcCard;
		try{
			SrcCard = "card_" + FindTypeCard(info[0]) + "_" + arrCardTypes[atoi(info[1].c_str())] + "_big.png";
			log("card: %s", SrcCard.c_str());
		}
		catch (std::out_of_range& e){

		}
		catch (std::exception& e){

		}
		catch (...){

		}

		auto pCard = Sprite::createWithSpriteFrameName(SrcCard.c_str());

		pCard->setPosition(Point(leftCard, bottomCard));
		pCard->setLocalZOrder(i + 1);
		pCard->setScale(2);
		pCard->setPosition(Vec2(WIDTH_DESIGN / 2 + 20 * i, HEIGHT_DESIGN / 2 - 100 + sizePos.height));
		this->addChild(pCard);
		CardOnHand.push_back(pCard);
	}

	if (CardOnHand.size() == 0)
		return;
	cardIndex = (int)CardOnHand.size() - 1;
}

void Nan3Cay::SetCallbackFunc(Ref* target, SEL_CallFuncN callfun)
{
	m_callback = target;
	m_callbackListener = callfun;
}

bool Nan3Cay::onTouchBegan(Touch *touch, Event *pEvent)
{
	Point tap = touch->getLocation();
	tap = ConvertPoint(tap);

	pointTouchBegan = tap;
	disTouchBegan.setSize(0, 0);

	if (tap.x > leftCard && tap.x < leftCard + widthCard &&
		tap.y > bottomCard && tap.y < bottomCard + heightCard)
	{
		isTouched = true;
		dx = tap.x - leftCard;
		dy = tap.y - bottomCard;
		_index = 7;

		sX = tap.x;
		sY = tap.y;

		xcu = tap.x;
		ycu = tap.y;

		disTouchBegan.setSize(tap.x - leftCard, tap.y - bottomCard);
	}
	return true;
}

void Nan3Cay::onTouchMoved(Touch *touch, Event *pEvent)
{
	Point tap = touch->getLocation();
	tap = ConvertPoint(tap);

	if (isTouched)
	{
		deltaX = sX - tap.x;
		deltaY = sY - tap.y;
		sX = tap.x;
		sY = tap.y;

		if (deltaX > 0 && tap.x < xcu)
		{
			count_move++;
			if (count_move == 1)
				flag_left = 0;
			else
				count_move++;
		}

		if (deltaX < 0 && tap.x > xcu)
		{
			count_move--;
			if (count_move == -1)
				flag_right = 0;
			else
				count_move++;
		}

		tap.x -= disTouchBegan.width / 3;
		tap.y -= disTouchBegan.height / 3 - sizePos.height;

		if (flag_left == 0)
		{
			Sprite *pCard = CardOnHand.at(cardIndex);
			pCard->setPosition(tap);
		}

		if (flag_right == 0)
		{
			Sprite *pCard = CardOnHand.at(cardIndex_under);
			pCard->setPosition(tap);
		}
	}
}

void Nan3Cay::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	if (flag_left == 0)
		vt = cardIndex;
	else if (flag_right == 0)
		vt = cardIndex_under;

	if (isTouched)
	{
		if (vt == -1)
			return;

		Sprite *pCard = CardOnHand.at(vt);
		if (pCard->getPositionX() <= (leftCard - widthCard / 2) ||
			pCard->getPositionX() >= (leftCard + widthCard / 2) ||
			pCard->getPositionY() >= (bottomCard + heightCard / 2))
		{
			if (flag_left == 0)
			{
				MovePockerFinish(pCard, vt);
				cardIndex--;
			}
			if (flag_right == 0)
			{
				MovePockerFinish(pCard, vt);
				cardIndex_under++;
			}
		}
		else
		{
			pCard->setPosition(Point(leftCard + 214, bottomCard + 180 + sizePos.height));
		}

	}

	isTouched = false;
	flag_left = flag_right = -1;
	count_move = 0;

	if (cardIndex == cardIndex_under)
	{
		Sprite *pCard = CardOnHand.at(cardIndex);
		MovePockerFinish(pCard, cardIndex);
		this->runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create(CC_CALLBACK_0(Nan3Cay::CloseLayerNanBai, this)),
			NULL));
	}

	//update 21/9
	if (cardIndex_under < 0 || cardIndex_under >= 3)
	{
		cardIndex_under = CardOnHand.size() - 1;
	}
}

//Di chuyển lá bài về vị trí khi đã nặn xong
void Nan3Cay::MovePockerFinish(Sprite *pCard, const int& pos)
{
	float toX = -1;
	float toY = -1;
	toY = bottomCard + 180 + sizePos.height;

	switch (pos)
	{
	case 0:
		toX = this->leftCard + this->widthCard / 2 * 3;
		break;
	case 1:
		toX = this->leftCard + this->widthCard / 2;
		break;
	case 2:
		toX = this->leftCard - this->widthCard / 2;
		pCard->setLocalZOrder(0);
		break;
	default:
		break;
	}

	MoveTo *pMoveBy = MoveTo::create(0.4, Point(toX, toY));
	pCard->runAction(pMoveBy);
}

void Nan3Cay::CloseLayerNanBai()
{
	if (m_callback && m_callbackListener)
		(m_callback->*m_callbackListener)(this);
	this->removeFromParentAndCleanup(true);
}

string& Nan3Cay::FindTypeCard(string& card)
{

	if (card == "11")
		card = "j";
	else if (card == "1")
		card = "1";
	else if (card == "12")
		card = "q";
	else if (card == "13")
		card = "k";
	return card;
}

void Nan3Cay::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Nan3Cay::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Nan3Cay::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Nan3Cay::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(Nan3Cay::onTouchCancelled, this);
	//dispatcher->addEventListenerWithFixedPriority(listener, 1);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void Nan3Cay::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
