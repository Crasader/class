#include "LayerPopupLuckyNumber.h"
#include "../SceneManager.h"
#include "data_manager.h"
#include "../mUtils.h"
LayerPopupLuckyNumber::LayerPopupLuckyNumber(){
	GameServer::getSingleton().addListeners(this);
}

//LayerPopupLuckyNumber::LayerPopupLuckyNumber(int reasonCode){
//	this->codeReason = reasonCode;
//	if (reasonCode == CODE_REASON_LOGIN_FIRST)
//		dataManager.getAppConfig().isLoginFisrtDay = false;
//	GameServer::getSingleton().addListeners(this);
//}

LayerPopupLuckyNumber::LayerPopupLuckyNumber(int reasonCode, string _cardCode, string _seri){
	this->codeReason = reasonCode;
	this->cardCode = _cardCode;
	this->seriNumber = _seri;
	if (reasonCode == CODE_REASON_LOGIN_FIRST)
		dataManager.getAppConfig().isLoginFisrtDay = false;
	GameServer::getSingleton().addListeners(this);
}

//LayerPopupLuckyNumber* LayerPopupLuckyNumber::create(int rescode)
//{
//	LayerPopupLuckyNumber* layerLucky = new LayerPopupLuckyNumber(rescode);
//	if (layerLucky && layerLucky->init()) {
//		layerLucky->autorelease();
//		return layerLucky;
//	}
//	else {
//		delete layerLucky;
//		layerLucky = NULL;
//		return NULL;
//	}
//}

LayerPopupLuckyNumber* LayerPopupLuckyNumber::create(int rescode, string _card, string _seri)
{
	LayerPopupLuckyNumber* layerLucky = new LayerPopupLuckyNumber(rescode, _card, _seri);
	if (layerLucky && layerLucky->init()) {
		layerLucky->autorelease();
		return layerLucky;
	}
	else {
		delete layerLucky;
		layerLucky = NULL;
		return NULL;
	}
}


LayerPopupLuckyNumber::~LayerPopupLuckyNumber()
{
	GameServer::getSingleton().removeListeners(this);
}

//LayerGuideInGame* LayerGuideInGame::create(){
//	LayerGuideInGame *pRet = new LayerGuideInGame(this->gameID);
//	if (pRet && pRet->init()) {
//		pRet->autorelease();
//		return pRet;
//	}
//	else {
//		delete pRet;
//		pRet = NULL;
//		return NULL;
//	}
//}
bool LayerPopupLuckyNumber::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->gameID = SceneManager::getSingleton().getGameID();

	auto rootNode = CSLoader::getInstance()->createNode("LayerListNumberLucky.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);

	if (gameID == kGameChan || gameID == kGameXiTo){
		rootNode->setAnchorPoint(Point::ZERO);
		rootNode->setPosition(Point::ZERO);
	}
	else
	{
		rootNode->setAnchorPoint(Point(0.5, 0.5));
		rootNode->setPosition(Point(visibleSize / 2));
	}

	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	auto btnClose = static_cast<ui::Button*>(rootNode->getChildByName("btn_close"));
	if (btnClose != NULL){
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupLuckyNumber::btnClose, this));
		btnClose->setPressedActionEnabled(true);
	}
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;

	auto pnlHuongDan = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	//pnlHuongDan->setBackGroundColor(Color3B::RED);
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL || myself->Name() == NULL)
		return true;
	std::string urlLink = DAYSOMAYMAN;
	urlLink += *myself->Name();
	urlLink += "&checksum=";
	urlLink += mUtils::getMD5KeyHis(*myself->Name());
	urlLink += StringUtils::format("&codeReason=%d", this->codeReason);
	urlLink += "&serial=" + this->seriNumber;
	urlLink += "&code=" + this->cardCode;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	auto posX = pnlHuongDan->getPositionX();
	auto posY = pnlHuongDan->getPositionY() + distance;
	webView = cocos2d::experimental::ui::WebView::create();
	webView->setAnchorPoint(Vec2(0, 1));
	webView->setPosition(Vec2(posX, posY));
	webView->setContentSize(Size(pnlHuongDan->getContentSize().width, pnlHuongDan->getContentSize().height));

	webView->loadURL(urlLink);
	webView->setScalesPageToFit(true);
	webView->setVisible(false);
	this->addChild(webView);
	SceneManager::getSingleton().showLoading();
	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupLuckyNumber::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupLuckyNumber::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupLuckyNumber::onWebViewDidFailLoading, this));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(urlLink);
#endif
	return true;

}

void LayerPopupLuckyNumber::setLink(string url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::getSingleton().showLoading();
	webView->loadURL(url);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif
}
void LayerPopupLuckyNumber::btnClose(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
		this->removeFromParentAndCleanup(true);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupLuckyNumber::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	sender->setVisible(false);
	return true;
}

void LayerPopupLuckyNumber::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
	sender->setVisible(true);
}

void LayerPopupLuckyNumber::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
}
#endif
void LayerPopupLuckyNumber::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}