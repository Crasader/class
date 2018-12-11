//
//  LayerPopupEvent.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/13/16.
//
//

#include "LayerPopupEvent.h"
#include "cocostudio/CocoStudio.h"
#include "../Common.h"
#include "ui/CocosGUI.h"
#include "../SceneManager.h"
#include "../data_manager.h"
#include "../mUtils.h"
#include "../layergames/ChanUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupLuckyNumber.h"
#include "../scenes/ScenePickRoom.h"
#include "../scenes/SceneMain.h"
USING_NS_CC;
using namespace cocos2d::ui;

LayerPopupEvent::LayerPopupEvent()
{

}


LayerPopupEvent::~LayerPopupEvent()
{

}

bool LayerPopupEvent::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupEvent.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(ratio);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	auto pnlBg1 = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
	Button* btnClose = dynamic_cast<Button*>(pnlBg1->getChildByName("btnClose"));
	if (btnClose){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupEvent::onBtnClose, this));
	}
	loadEvent();


	pnlWebView = dynamic_cast<Layout*>(pnlBg1->getChildByName("Panel_3"));
	pnlWebView->setVisible(false);

	sprHeader = static_cast<Sprite*>(pnlBg1->getChildByName("Sprite_1"));
	//if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		pnlHeaderTabs = dynamic_cast<Layout*>(pnlBg1->getChildByName("pnl_header_tabs"));
		pnlHeaderTabs->setVisible(true);
	//}
	Button* btnAutoLogin = static_cast<ui::Button*>(pnlBg1->getChildByName("btnCheck"));
	if (btnAutoLogin != NULL){
		btnAutoLogin->addClickEventListener(CC_CALLBACK_1(LayerPopupEvent::onButtonAutoLogin, this));
	}
	imgAutoLogin = static_cast<ui::ImageView*>(pnlBg1->getChildByName("imgCheck"));

	bool isAuto = UserDefault::getInstance()->getBoolForKey("isShowEventDay");
	if (isAuto)
		imgAutoLogin->loadTexture("checkbox-active.png");
	else
		imgAutoLogin->loadTexture("checkbox.png");
		
	UserDefault::getInstance()->setBoolForKey("isShowEvent", false);
	isDestroy = false;
//	Text* txt = static_cast<ui::Text*>(pnlBg1->getChildByName("txt"));
//	if (txt)
//	{
//		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2)
//		{
//			txt->setVisible(false);
//			imgAutoLogin->setVisible(false);
//			UserDefault::getInstance()->setBoolForKey("isShowEventDay", true);
//		}
//	}

	//setValueChest("21238712387","300");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	auto posX = pnlWebView->getPositionX();
	auto posY = pnlWebView->getPositionY() + distance;

	auto sizeAdd = Size(0, 0);
	//    if (SceneManager::getSingleton().getGameID() == kGameChan || SceneManager::getSingleton().getGameID() == kGameCoTuong || SceneManager::getSingleton().getGameID() == kGameCoUp)
	//        sizeAdd = ChanUtils::getSizePos();

	webView = cocos2d::experimental::ui::WebView::create();
	webView->setAnchorPoint(Vec2(0, 0));
	webView->setPosition(Vec2(posX , posY - sizeAdd.height + 10));
	webView->setContentSize(Size(pnlWebView->getContentSize().width, pnlWebView->getContentSize().height));
	//webView->loadURL(TYGIA_LINK);
	//webView->setContentSize(Size(lsvHistories->getContentSize().width * ratio, lsvHistories->getContentSize().height * ratio));
	webView->setScalesPageToFit(true);
	this->addChild(webView);
	webView->setVisible(false);

	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupEvent::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupEvent::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupEvent::onWebViewDidFailLoading, this));
#endif
    this->setScale(1);
    ////this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
	return true;
}
bool LayerPopupEvent::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}
void LayerPopupEvent::onBtnClose(Ref* pSender){
    SceneMain* parent = (SceneMain*)this->getParent();
    if (parent){
        parent->isNoEvent = true;
    }
    if (!dataManager.getAppConfig().isLoginFisrtDay){
        boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
        if (mySelf)
        {
            boost::shared_ptr<UserVariable> rotatebonus = mySelf->GetVariable("rotatebonus");
            if (rotatebonus)
            {
                int rotatebonus_value = *rotatebonus->GetIntValue();
                if (rotatebonus_value <= 0){
                    boost::shared_ptr<ISFSObject> params(new SFSObject());
                    params->PutUtfString("aI", SceneManager::getSingleton().getMyName());
                    boost::shared_ptr<IRequest> request(new ExtensionRequest("rgaci", params));
                    GameServer::getSingleton().Send(request);
                    
                }
            }
        }
    }
    //dataManager.getAppConfig().isLoginFisrtDay = false;
    
    lstButtons.clear();
    lstLinks.clear();
    SceneManager::getSingleton().showPopupVongQuay();

	this->removeFromParent();
}
void LayerPopupEvent::onButtonAutoLogin(Ref* pSender)
{
	bool isAuto = UserDefault::getInstance()->getBoolForKey("isShowEventDay");
	if (!isAuto){
		imgAutoLogin->loadTexture("checkbox-active.png");
		UserDefault::getInstance()->setBoolForKey("isShowEventDay", true);
	}

	else{
		imgAutoLogin->loadTexture("checkbox.png");
		UserDefault::getInstance()->setBoolForKey("isShowEventDay", false);
	}

}

void LayerPopupEvent::popEvent(float dt){

    
	int size = SceneManager::getSingleton().lstEvent.size();
	float orgX;

		if (pnlHeaderTabs == NULL)
			return;
	
		orgX = WIDTH_TAB_EVENT_POPUP / 2;
	
	//auto orgX = DESIGN_RESOLUTION_WIDTH / (size + 1);
	if (isDestroy)
		return;
	if (size == 0)
		return;
	for (int i = 0; i < size; ++i){
		auto button = ui::Button::create();
        
            button->loadTextures("design/guifiles/moc3-btn-tab-normal.png", "design/guifiles/moc3-btn-tab-active.png", "design/guifiles/moc3-btn-tab-active.png");
            button->setAnchorPoint(Point(0, 0));
            button->setContentSize(Size(517, 80));
            string strTitle = SceneManager::getSingleton().lstEvent[i].name;
        //button->setContentSize(Size(45 * strTitle.length(), 80));
            button->setTitleText(strTitle);
            button->setTitleFontSize(40);
            //button->setTitleFontName("__Roboto-Regular.ttf");
			//button->setContentSize(Size(45 * strTitle.length(), 80));
			button->setTag(i);
			auto lbl_size = button->getTitleRenderer()->getContentSize();
			button->setContentSize(Size(lbl_size.width*1.1f, lbl_size.height*1.5f));
			button->setPosition(Vec2(lbl_size.width * i, 35));
			button->addClickEventListener(CC_CALLBACK_1(LayerPopupEvent::onButtonClicked, this));
			pnlHeaderTabs->addChild(button);
            button->setVisible(false);
		
		this->lstButtons.push_back(button);
		this->lstLinks.push_back(SceneManager::getSingleton().lstEvent[i].link);
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerPopupEvent::popEvent),this);

	}
	loadFirst();
	SceneManager::getSingleton().hideLoading();
	if (!isDestroy)
		isDestroy = true;
}
void LayerPopupEvent::loadEvent(){
	//SceneManager::getSingleton().hideLoading();
	Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerPopupEvent::popEvent),this,0.3, false);
	SceneManager::getSingleton().showLoading();
}

void LayerPopupEvent::onButtonClicked(Ref* pSender){
	for (int i = 0; i < this->lstButtons.size(); ++i){
		this->lstButtons.at(i)->setEnabled(true);
		/*if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		this->lstButtons.at(i)->setTitleColor(ccc3(255, 227, 160));
		}*/
		/*this->lstButtons.at(i)->setTitleColor(ccc3(255, 227, 160));*/
	}
	auto button = (ui::Button*)pSender;
	this->lstButtons.at(button->getTag())->setEnabled(false);
	/*this->lstButtons.at(button->getTag())->setTitleColor(ccc3(223, 0, 41));*/
	auto parent = (LayerPopupEvent*)this->getParent();
	SceneManager::getSingleton().showLoading();
	this->reloadData(this->lstLinks.at(button->getTag()));
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	webView->setVisible(false);
#endif
}
void LayerPopupEvent::reloadData(std::string url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	webView->loadURL(url);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif
}
void LayerPopupEvent::loadFirst(){
	onButtonClicked(this->lstButtons.at(0));
	SceneManager::getSingleton().showLoading();
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupEvent::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
	webView->setVisible(false);
	sender->setVisible(false);
	return true;

}

void LayerPopupEvent::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
	webView->setVisible(true);
}

void LayerPopupEvent::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
}
#endif
void LayerPopupEvent::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerPopupEvent::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
	//
	//    GameServer::getSingleton().addListeners(this);
}

void LayerPopupEvent::onExit()
{
	Layer::onExit();
   
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
	//    GameServer::getSingleton().removeListeners(this);
	//    Layer::onExit();
}
void LayerPopupEvent::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	this->removeFromParentAndCleanup(true);
}
