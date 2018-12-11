#include "LayerNotification.h"
#include "ui/CocosGUI.h"
#include "ui/UIWebView.h"
#include "../SceneManager.h"

LayerNotification::LayerNotification()
{
}

LayerNotification::~LayerNotification()
{
}

void LayerNotification::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerNotification::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;

}

void LayerNotification::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

bool LayerNotification::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(true);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	webView = cocos2d::experimental::ui::WebView::create();
	webView->setPosition(Point(0, 0));
	webView->setAnchorPoint(Vec2(0,0));
	webView->setContentSize(Size(visibleSize.width, visibleSize.height - 137));
    //webView->setScaleZ(2);
	//webView->loadURL("http://gamebai.net/thong-bao.php?embed=1");
	webView->setScalesPageToFit(true);
	webView->setLocalZOrder(99);
	this->addChild(webView);
    webView->setVisible(false);

	webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerNotification::onWebViewShouldStartLoading, this));
	webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerNotification::onWebViewDidFinishLoading, this));
	webView->setOnDidFailLoading(CC_CALLBACK_2(LayerNotification::onWebViewDidFailLoading, this));
#endif

	this->setContentSize(visibleSize);
	layerNotificationTop = LayerNotificationTop::create();
	layerNotificationTop->setAnchorPoint(Vec2(0, 0));
	layerNotificationTop->setTag(0);
	layerNotificationTop->setLocalZOrder(100);
	layerNotificationTop->setPosition(Point(0, visibleSize.height - layerNotificationTop->getContentSize().height));
	this->addChild(layerNotificationTop);

	return true;
}

bool LayerNotification::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
	return true;
}

void LayerNotification::reloadData(std::string url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->setVisible(false);
	webView->loadURL(url);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerNotification::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
	return true;

}

void LayerNotification::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
	SceneManager::getSingleton().hideLoading();
    webView->setVisible(true);
}

void LayerNotification::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif
void LayerNotification::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		layerNotificationTop->onButtonClose(this);
	}
}