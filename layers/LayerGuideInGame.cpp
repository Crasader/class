

#include "LayerGuideInGame.h"
#include "../SceneManager.h"
#include "layergames/PhomMessDef.h"
LayerGuideInGame::LayerGuideInGame(){
	GameServer::getSingleton().addListeners(this);
}
LayerGuideInGame::~LayerGuideInGame()
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
bool LayerGuideInGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->gameID = SceneManager::getSingleton().getGameID();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupHuongDanChoi.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    
    this->lblTitle = static_cast<Text*> (rootNode->getChildByName("Text_1"));
    this->lblTitle->setString(dataManager.GetSysString(887));
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

	auto btnClose = static_cast<ui::Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose != NULL){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerGuideInGame::btnClose, this));
		btnClose->setPressedActionEnabled(true);
	}
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;

	auto pnlHuongDan = static_cast<Layout*>(rootNode->getChildByName("pnlHuongDan"));
	//pnlHuongDan->setBackGroundColor(Color3B::RED);
	std::string urlLink = HOME_DOMAIN + SceneManager::getSingleton().getFolderLangWeb()+GUIDE_P2_LINK + StringUtils::format("%d", SceneManager::getSingleton().getGameID());
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
	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerGuideInGame::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerGuideInGame::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerGuideInGame::onWebViewDidFailLoading, this));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(urlLink);
#endif
    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
	return true;
}
void LayerGuideInGame::setTitle(string str){
    this->lblTitle->setString(str);

}
void LayerGuideInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){

	CCLOG("REceiver call function");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	CCLOG("onExtentions");
	if (strcmp("notiRMFF", cmd->c_str()) == 0){
		btnClose(NULL);

	}
}
void LayerGuideInGame::setLink(string url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SceneManager::getSingleton().showLoading();
	webView->loadURL(url);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif
}
void LayerGuideInGame::btnClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerGuideInGame::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	sender->setVisible(false);
	return true;
}

void LayerGuideInGame::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
   //

	sender->setVisible(true);
}

void LayerGuideInGame::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
    
}
#endif
void LayerGuideInGame::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	this->removeFromParentAndCleanup(true);
}
