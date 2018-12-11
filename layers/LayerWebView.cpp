//
//  LayerGiftCode.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/24/17.
//
//

#include "LayerWebView.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "LayerPopupThongBao.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"
LayerWebView::LayerWebView(){
    
}
LayerWebView::~LayerWebView(){
    
}
bool LayerWebView::init()
{
    if (!Layer::init())
        return false;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerWebview.csb");
    //rootNode->setAnchorPoint(Point(0.5, 0.5));
   // rootNode->setPosition(Point(visibleSize / 2));
    this->addChild(rootNode);
    
    auto pnlWebView = (Layout*)rootNode->getChildByName("pnlWebView");
 
    if (pnlWebView)
    {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
//        auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
//        auto posX = pnlWebView->getPositionX();
//        auto posY = pnlWebView->getPositionY()
        webView = cocos2d::experimental::ui::WebView::create();
        webView->setAnchorPoint(pnlWebView->getAnchorPoint());
        webView->setPosition(Vec2(pnlWebView->getPosition().x,pnlWebView->getPosition().y));
        webView->setContentSize(pnlWebView->getContentSize());
        webView->setScalesPageToFit(true);
        pnlWebView->addChild(webView);
        webView->setVisible(false);
        
        webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerWebView::onWebViewShouldStartLoading, this));
        webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerWebView::onWebViewDidFinishLoading, this));
        webView->setOnDidFailLoading(CC_CALLBACK_2(LayerWebView::onWebViewDidFailLoading, this));
#endif
    }
    return true;
}
void LayerWebView::hideLayer(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->setVisible(false);
#endif
    this->setVisible(false);
    this->isShouldShow = false;

}
void LayerWebView::showLayer(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->setVisible(true);
    //this->setWebView();
#endif
    this->isShouldShow = true;

    this->setVisible(true);
}
void LayerWebView::setWebView(string urlWeb)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    SceneManager::getSingleton().showLoading();
   // webView->clea
    webView->loadURL(urlWeb);
    webView->setVisible(false);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    Application::getInstance()->openURL(urlWeb);
#endif
}

void LayerWebView::setWebFile(string pathFile)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    webView->loadFile(pathFile);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    Application::getInstance()->openURL(pathFile);
#endif
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerWebView::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    sender->setVisible(false);
    return true;
}

void LayerWebView::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    if (this->isShouldShow)
        webView->setVisible(true);
}

void LayerWebView::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}


#endif
void LayerWebView::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
