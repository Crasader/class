#include "LayerRecharge.h"
#include "ui/CocosGUI.h"
#include "ui/UIWebView.h"
#include "../SceneManager.h"
#include "../mUtils.h"
LayerRecharge::LayerRecharge()
{
}

LayerRecharge::~LayerRecharge()
{
}

void LayerRecharge::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerRecharge::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    
}

void LayerRecharge::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

bool LayerRecharge::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    this->setKeyboardEnabled(true);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto background = Sprite::create("background-in-game.jpg");
    background->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    background->setAnchorPoint(Point(0, 0));
    background->setPosition(Point(0, 0));
    this->addChild(background);
    
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
    
    webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerRecharge::onWebViewShouldStartLoading, this));
    webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerRecharge::onWebViewDidFinishLoading, this));
    webView->setOnDidFailLoading(CC_CALLBACK_2(LayerRecharge::onWebViewDidFailLoading, this));
#endif
    
    layerIAP = LayerPurcharse::create();
    layerIAP->setAnchorPoint(Vec2(0.5,0.5));
    //layerIAP->setPosition(visibleSize/2);
    this->addChild(layerIAP);
    layerIAP->setVisible(false);
    

    
    this->setContentSize(visibleSize);
    rechargeTop = LayerRechargeTop::create();
    rechargeTop->setAnchorPoint(Vec2(0, 0));
    rechargeTop->setTag(0);
    rechargeTop->setLocalZOrder(100);
    rechargeTop->setPosition(Point(0, visibleSize.height - rechargeTop->getContentSize().height));
    this->addChild(rechargeTop);
    
    
    
    return true;
}

bool LayerRecharge::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void LayerRecharge::onBtnShowInApp(){
    layerIAP->setVisible(true);
}
void LayerRecharge::onBtnHideInApp(){
    layerIAP->setVisible(false);
}
void LayerRecharge::showWebView(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->setVisible(true);
  
#endif

}
void LayerRecharge::hideWebView(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->setVisible(false);
    
#endif
}
void LayerRecharge::reloadData(std::string url){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->loadURL(url);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	Application::getInstance()->openURL(url);
#endif

}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerRecharge::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
    return true;
    
}

void LayerRecharge::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    webView->setVisible(true);
}

void LayerRecharge::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif
void LayerRecharge::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        rechargeTop->onButtonClose(this);
    }
}

LayerPurcharse::LayerPurcharse()
{
    
}

LayerPurcharse::~LayerPurcharse()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CC_SAFE_DELETE(iap);
#endif
}
bool LayerPurcharse::init()
{
    if (!Layer::init())
        return false;
    auto root = CSLoader::getInstance()->createNode("LayerPopupPurcharse.csb");
    int gameID = SceneManager::getSingleton().getGameID();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (gameID == kGameChan || gameID == kGameCoTuong || gameID == kGameCoUp){
        root->setAnchorPoint(Point::ZERO);
        root->setPosition(Point::ZERO);
    }
    else{
        root->setAnchorPoint(Point(0.5, 0.5));
        root->setPosition(Point(visibleSize / 2));
    }
    ui::Helper::doLayout(root);
    addChild(root);
    // 	Sprite* Sprite_1 = (Sprite*)root->getChildByName("Sprite_1");
    // 	if (Sprite_1)
    // 		Sprite_1->setVisible(false);
    // 	Label* Text_1 = (Label*)root->getChildByName("Text_1");
    // 	if (Text_1)
    // 		Text_1->setVisible(false);
    // 	Text* lblPhoneHotLine = dynamic_cast<Text*>(root->getChildByName("lblPhoneNumber"));
    // 	if (lblPhoneHotLine)
    // 		lblPhoneHotLine->setVisible(false);
    Layout* panel_bronze = (Layout*)root->getChildByName("panel_bronze");
    int money_rate = 2;
    if (panel_bronze)
    {
        Button* btnBronze = static_cast<Button*>(panel_bronze->getChildByName("btnBronze"));
        if (btnBronze)
        {
            btnBronze->setTitleText("0.99$");
            btnBronze->addTouchEventListener(CC_CALLBACK_2(LayerPurcharse::onButtonBronze, this));
        }
        Text *txtBronze = static_cast<Text*>(panel_bronze->getChildByName("labelBronze"));
        if (txtBronze)
            txtBronze->setString(mUtils::convertMoneyEx(money_rate * 1000));
    }
    
    Layout* panel_silver = (Layout*)root->getChildByName("panel_silver");
    if (panel_silver)
    {
        Button* btnSilver = static_cast<Button*>(panel_silver->getChildByName("btnSilver"));
        if (btnSilver)
        {
            btnSilver->setTitleText("9.99$");
            btnSilver->addTouchEventListener(CC_CALLBACK_2(LayerPurcharse::onButtonSilver, this));
        }
        Text *txtSilver = static_cast<Text*>(panel_silver->getChildByName("labelSilver"));
        if (txtSilver)
            txtSilver->setString(mUtils::convertMoneyEx(money_rate * 1000 * 10));
    }
    
    Layout* panel_gold = (Layout*)root->getChildByName("panel_gold");
    if (panel_gold)
    {
        Button* btnGold = static_cast<Button*>(panel_gold->getChildByName("btnGold"));
        if (btnGold)
        {
            btnGold->setTitleText("$ 99.99");
            btnGold->addTouchEventListener(CC_CALLBACK_2(LayerPurcharse::onButtonGold, this));
        }
        Text *txtGold = static_cast<Text*>(panel_gold->getChildByName("labelGold"));
        if (txtGold)
            txtGold->setString(mUtils::convertMoneyEx(money_rate * 1000 * 100));
    }
    // 	Button* btnClose = static_cast<Button*>(root->getChildByName("btnClose"));
    // 	if (btnClose)
    // 		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupPurcharse::onButtonClose, this));
    // 	Button* btnMuabac = static_cast<Button*>(root->getChildByName("btnMuabac"));
    // 	if (btnMuabac)
    // 		btnMuabac->setTitleText("Mua Bạc");
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
    iap = new mIAP();
    iap->loadIAP();
#endif
    return true;
}

void LayerPurcharse::onButtonBronze(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       // iap->purchaseItem(0);
#endif
    }
}

void LayerPurcharse::onButtonSilver(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS  || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //iap->purchaseItem(1);
#endif
    }
}

void LayerPurcharse::onButtonGold(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
       // iap->purchaseItem(2);
#endif
    }
}

void LayerPurcharse::onButtonClose(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        Layer *parent = static_cast<Layer*>(this->getParent());
        if (parent)
            parent->removeFromParentAndCleanup(true);
    }
}
