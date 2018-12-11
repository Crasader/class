#include "LayerSendMessage.h"
#include "LayerMailInbox.h"
#include "../mUtils.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

LayerSendMessage::LayerSendMessage()
{

}


LayerSendMessage::~LayerSendMessage()
{

}

bool LayerSendMessage::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupSendMessage.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	_btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (_btnClose != NULL){
		_btnClose->addClickEventListener(CC_CALLBACK_1(LayerSendMessage::onBtnClose, this));
	}

	auto btnSend = static_cast<ui::Button*>(rootNode->getChildByName("btnSend"));
	if (btnSend != NULL){
		btnSend->addClickEventListener(CC_CALLBACK_1(LayerSendMessage::onBtnSend, this));
	}

	this->txtSendTo = static_cast<TextField*>(rootNode->getChildByName("txtSendTo"));
	this->txtContent = static_cast<TextField*>(rootNode->getChildByName("txtContent"));
	auto imgContent = static_cast<ImageView*>(rootNode->getChildByName("Image_5"));
	txtContent->setAnchorPoint(Point(0, 1));
	auto posX = imgContent->getPositionX();
	auto posY = imgContent->getPositionY() + distance * ratio;
	txtContent->setPosition(Point(posX, posY));
	txtContent->setTextVerticalAlignment(TextVAlignment::TOP);
	txtContent->setFontSize(40.0f * ratio);
#if(CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	txtContent->setInputMode(ui::EditBox::InputMode::ANY);
#endif
	txtContent->setMaxLength(120.0f);

	return true;
}

void LayerSendMessage::sendReceiver(std::string name){
	this->txtSendTo->setString(name);
}
bool LayerSendMessage::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}

void LayerSendMessage::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerSendMessage::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;

	GameServer::getSingleton().addListeners(this);
}

void LayerSendMessage::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	GameServer::getSingleton().removeListeners(this);
	Layer::onExit();
}

void LayerSendMessage::onBtnClose(Ref* pSender){
	this->removeFromParentAndCleanup(true);
}

void LayerSendMessage::onBtnSend(Ref* pSender){
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return;
	//std::string strS = string(txtSendTo->getString());

	string str = string(txtContent->getString());
	if (mUtils::trimString(str).length() < 6){
		Chat *toast = Chat::create("Nội dung không được để trống!", -1);
		this->addChild(toast, this->getLocalZOrder() + 1);
		return;
	}
	str = string(txtSendTo->getString());
    
	if (mUtils::trimString(str).length() < 6 || txtSendTo->getString().compare(*GameServer::getSingleton().getSmartFox()->MySelf()->Name()) == 0){
		Chat *toast = Chat::create("Tên người nhận không hợp lệ!", -1);
		this->addChild(toast, this->getLocalZOrder() + 1);
		return;
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutUtfString("aR", txtSendTo->getString());
	params->PutUtfString("scontent", txtContent->getString());
	params->PutUtfString("aI", myself->Name());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("sendmi", params));
	GameServer::getSingleton().Send(request);
}

void LayerSendMessage::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("sendmi", cmd->c_str()) == 0){
		boost::shared_ptr<string> glmi = param->GetUtfString("sendmi");
		if (*param->GetInt("rc") == 0){
			Chat *toast = Chat::create("Gửi tin nhắn thành công!", -1);
			this->getParent()->addChild(toast, this->getZOrder() + 1);
			//
			auto _parent = static_cast<LayerMailInbox*>(this->getParent());
			if (_parent)
				_parent->loadAllDatas();
			//
			this->removeFromParent();
		}
		else{
			Chat *toast = Chat::create(StringUtils::format("Gửi tin nhắn thất bại!\n Kiểm tra lại tên người nhận!", *param->GetInt("rd")), -1);
			this->getParent()->addChild(toast, this->getZOrder() + 1);
		}

	}
}

void LayerSendMessage::setSender(string sender)
{
	this->txtSendTo->setString(sender);
}
