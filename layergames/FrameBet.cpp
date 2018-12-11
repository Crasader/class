//
//  FrameBet.cpp
//  iCasinov2
//
//  Created by DauA on 5/26/14.
//
//

#include "FrameBet.h"
#include "AllData.h"
#include "../Common.h"
#include "../SceneManager.h"
FrameBet::FrameBet()
:iconXu(NULL),
frame(NULL),
money(NULL)
{

}

FrameBet::~FrameBet(){}

bool FrameBet::init(){
	if (!Layer::init())
		return false;

	this->setAnchorPoint(Vec2(0, 0));

	this->frame = ImageView::create();
	/* this->frame->loadTexture("money-mask.png");*/
	this->frame->loadTexture("moc3-dat-cuoc-moc.png");
	this->frame->setAnchorPoint(Vec2(0, 0));
	this->frame->setPosition(Vec2(10, 0));
	this->addChild(this->frame);

	Size size = this->frame->getContentSize();

	/* this->iconXu = Sprite::create("moc3-icon-moc.png");*/
	int roomType = SceneManager::getSingleton().getCurrRoomType();

	if (roomType == 0){
		this->frame->loadTexture("moc3-dat-cuoc-moc.png");
	}
	else{
		this->frame->loadTexture("moc3-dat-cuoc-hao.png");

	}
	/*this->iconXu->setAnchorPoint(Vec2(0.5, 0.5));
	this->iconXu->setPosition(Vec2(30, size.height / 2));
	this->addChild(this->iconXu);*/

	this->money = Label::createWithTTF("",kBoldFont, 25);
	this->money->setAnchorPoint(Vec2(0, 0.5));
	this->money->setPosition(Vec2(65, size.height / 2));

	//if (this->money->getContentSize().width > size.width) {
	//    //this->frame->setScaleX((this->money->getContentSize().width + 15) / size.width);
	//    //this->money->setScaleX(1 / this->frame->getScaleX());
	//    this->money->setPosition(75, size.height / 2);
	//}
	this->frame->addChild(this->money);

	return true;
}

void FrameBet::SetFrameSize(const float& toSize){
	float y = this->frame->getContentSize().width;
	//if (toSize * 8 > y)
	//{
	//	//this->frame->setScaleX(toSize * 8 / y);
	//	this->frame->setContentSize(Size(toSize * 8, this->frame->getContentSize().height));
	//}
}

void FrameBet::SetValueBet(const std::string& value){
	this->money->setString(value);
	Size sizeMoney = this->money->getContentSize();
	Size sizeFrame = this->frame->getContentSize();

	//   if (sizeMoney.width > sizeFrame.width){
	//       //this->frame->setScaleX((sizeMoney.width + 15) / sizeFrame.width);
	//	//this->money->setScaleX(1 / frame->getScaleX());
	//       this->money->setPosition(Vec2(10 / this->frame->getScaleX(), sizeFrame.height / 2));
	//}
	//else
	//{
	//	this->frame->setScaleX(1);
	//	this->money->setScaleX(1);
	//}
}

float FrameBet::GetKc_Width() const{
	return this->frame->getContentSize().width*frame->getScaleX() + 15;
}

LayerBien::LayerBien()
{
	GameServer::getSingleton().addListeners(this);
}

LayerBien::~LayerBien()
{
	GameServer::getSingleton().removeListeners(this);
}

bool LayerBien::init()
{
	return true;
}

bool LayerBien::init(double _money, std::string _lstUser)
{
	if (!Layer::init())
		return false;
	this->money_bet = _money;
	this->myName = SceneManager::getSingleton().getMyName();
	this->lstUser = _lstUser;

	btnBienx1 = Button::create("moc3-bg-cuoc-bien-active.png", "moc3-bg-cuoc-bien.png", "moc3-bg-cuoc-bien.png");
	btnBienx1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnBienx1->setTitleFontSize(18);
	btnBienx1->setTitleText(mUtils::formatMoneySymbol(_money));
	//btnBienx1->setTitleFontName("__Roboto-Regular.ttf");
	btnBienx1->setEnabled(false);
	btnBienx1->setBright(false);
	btnBienx1->setPosition(Point(btnBienx1->getContentSize().width / 2, btnBienx1->getContentSize().height / 2));

	btnBienx2 = Button::create("moc3-bg-cuoc-bien-active.png", "moc3-bg-cuoc-bien.png", "moc3-bg-cuoc-bien.png");
	btnBienx2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnBienx2->setTitleFontSize(18);
	btnBienx2->setTitleText(mUtils::formatMoneySymbol(_money * 2));
	//btnBienx2->setTitleFontName("__Roboto-Regular.ttf");
	btnBienx2->setEnabled(false);
	btnBienx2->setBright(false);
	btnBienx2->setPosition(Point(btnBienx1->getContentSize().width / 2, btnBienx1->getContentSize().height * 3 / 2));

	this->addChild(btnBienx1);
	this->addChild(btnBienx2);

	//add event
	btnBienx1->addTouchEventListener(CC_CALLBACK_2(LayerBien::processBienx1Touched, this));
	btnBienx2->addTouchEventListener(CC_CALLBACK_2(LayerBien::processBienx2Touched, this));
	return true;
}

LayerBien* LayerBien::create()
{
	LayerBien* pRet = new LayerBien();
	if (pRet && pRet->init()){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

LayerBien* LayerBien::create(double _money, std::string _lstUser)
{
	LayerBien *pRet = new LayerBien();
	if (pRet && pRet->init(_money, _lstUser)){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void LayerBien::processBienx1Touched(Ref *sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		log("bienx1 %s", this->arrList[this->getTag()].c_str());
		int betvl = this->money_bet;
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) return;
		params->PutUtfString("uid_from", this->myName);
		params->PutUtfString("uid_to", this->arrList[this->getTag()]);
		params->PutInt("betvl", betvl);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("ebreq", params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerBien::processBienx2Touched(Ref *sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		log("bienx2 %s", this->arrList[this->getTag()].c_str());
		int betvl = this->money_bet * 2;
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (lastRoom == NULL) return;
		params->PutUtfString("uid_from", this->myName);
		params->PutUtfString("uid_to", this->arrList[this->getTag()]);
		params->PutInt("betvl", betvl);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("ebreq", params, lastRoom));
		GameServer::getSingleton().Send(request);
	}
}

void LayerBien::buttonBienx1LoadTexture()
{
	this->btnBienx1->setEnabled(true);
	this->btnBienx1->setBright(true);
}

void LayerBien::buttonBienx2LoadTexture()
{
	this->btnBienx2->setEnabled(true);
	this->btnBienx2->setBright(true);
}

void LayerBien::resetBien()
{
	btnBienx1->setEnabled(false);
	btnBienx1->setBright(false);
	btnBienx2->setEnabled(false);
	btnBienx2->setBright(false);
}

void LayerBien::setArrList(std::string _list)
{
	this->lstUser = _list;
	vector<std::string> _arr = mUtils::splitString(this->lstUser, ';');
	for (int i = 0; i < _arr.size(); ++i){
		auto _name = mUtils::splitString(_arr[i], '|')[0];
		this->arrList.push_back(_name);
	}
}
