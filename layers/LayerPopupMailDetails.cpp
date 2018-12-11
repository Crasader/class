#include "LayerPopupMailDetails.h"
#include "LayerSendMessage.h"
#include "LayerMailInbox.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Requests/ExtensionRequest.h"  
#include "../SceneManager.h"
#include "LayerSendMessage.h"
#include "AllData.h"
#include "../layergames/PhomMessDef.h"
using namespace cocos2d::ui;

LayerPopupMailDetails::LayerPopupMailDetails()
{
    //this->lblSender = NULL;
    this->lblMailContent = NULL;
    this->lblDatetime = NULL;
    // 	this->btnReply = NULL;
    this->btnDelMail = NULL;
    GameServer::getSingleton().addListeners(this);
}

LayerPopupMailDetails::~LayerPopupMailDetails()
{
    GameServer::getSingleton().removeListeners(this);
}

bool LayerPopupMailDetails::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupMailInboxDetail.csb");
    auto pnlBg = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_1"));
    pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    this->addChild(rootNode);
    
    auto btnClose = dynamic_cast<ui::Button*>(rootNode->getChildByName("btnClose"));
    if (btnClose != NULL){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupMailDetails::onButtonClose, this));
        btnClose->setPressedActionEnabled(true);
    }
    
    auto txtFrom = static_cast<Text*> (rootNode->getChildByName("Text_1"));
    txtFrom->setString(dataManager.GetSysString(758));
    if (SceneManager::getSingleton().isNagaWin){
        txtFrom->setString(dataManager.GetSysString(1046));

    }
    
    auto txtDate = static_cast<Text*> (rootNode->getChildByName("lblDate"));
    txtDate->setString(dataManager.GetSysString(759));
    
    //this->lblSender = static_cast<Text*>(rootNode->getChildByName("lblName"));
    auto pnlMailContent = dynamic_cast<Layout*>(rootNode->getChildByName("pnlContent"));
    ui::ListView* listview = ListView::create();
    this->addChild(listview);
    listview->setContentSize(pnlMailContent->getContentSize()); //give it whatever size
    listview->setPosition(pnlMailContent->getPosition()); //give it whatever size
    listview->setAnchorPoint(pnlMailContent->getAnchorPoint());
    this->lblMailContent = ui::Text::create("[multiline content]", "", 50);
    this->lblMailContent->setTextAreaSize({pnlMailContent->getContentSize().width, 0}); //use that same size, but use 0 height. This auto sets overflow and wrap in the backend
    listview->addChild(this->lblMailContent);
    listview->requestDoLayout();

    //this->lblMailContent = dynamic_cast<Text*>(pnlMailContent->getChildByName("lblContent"));
    this->lblDatetime = dynamic_cast<Text*>(rootNode->getChildByName("lblDate"));
    this->btnDelMail = dynamic_cast<Button*>(rootNode->getChildByName("btnDelete"));
    if (btnDelMail != NULL){
        btnDelMail->addClickEventListener(CC_CALLBACK_1(LayerPopupMailDetails::onButtonDelete, this));
        btnDelMail->setPressedActionEnabled(true);
        btnDelMail->setTitleText(dataManager.GetSysString(760));
    }
//    if (lblMailContent)
//    {
////        lblMailContent->setTouchEnabled(true);
////        lblMailContent->addTouchEventListener(CC_CALLBACK_2(LayerPopupMailDetails::onLabelMailTouch, this));
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        webView = cocos2d::experimental::ui::WebView::create();
//        webView->setAnchorPoint(lblMailContent->getAnchorPoint());
//        webView->setPosition(lblMailContent->getPosition());
//        webView->setContentSize(lblMailContent->getContentSize());
//        
//        webView->setScalesPageToFit(true);
//        pnlMailContent->addChild(webView);
//        //webView->setVisible(false);
//        //string htmlstring = StringUtils::format("<html><body style=\"background-color: rgba(0, 0, 0, 0.8);\"><p style=\"color:#ffffff\">%s</p></body></html>", "day la noi dung test");
//        //webView->loadHTMLString(htmlstring);
//        webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerPopupMailDetails::onWebViewShouldStartLoading, this));
//        webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerPopupMailDetails::onWebViewDidFinishLoading, this));
//        webView->setOnDidFailLoading(CC_CALLBACK_2(LayerPopupMailDetails::onWebViewDidFailLoading, this));
//#endif
//
//    }
//    
    return true;
}

bool LayerPopupMailDetails::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupMailDetails::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupMailDetails::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerPopupMailDetails::onExit()
{
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

void LayerPopupMailDetails::onButtonClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

void LayerPopupMailDetails::setDatas(int id, std::string sender, std::string date, std::string content){
    if (SceneManager::getSingleton().isNagaWin){
        //this->lblSender->setString("Win Club");
    }
    this->lblDatetime->setString(date);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
//    //if(SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
//        string htmlstring = StringUtils::format("<html><body style=\"background-color: rgba(2, 44, 0, 1);\"><p style=\"color:#FFFFFF;font-size:40px\">%s</p></body></html>", content.c_str());
//        webView->loadHTMLString(htmlstring);
//    //}else{
////        string htmlstring = StringUtils::format("<html><body style=\"background-color: rgba(71, 19, 10, 1);\"><p style=\"color:#FFE3A0;font-size:35px\">%s</p></body></html>", content.c_str());
////        webView->loadHTMLString(htmlstring);
////    }
//
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
////    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
//        string htmlstring = StringUtils::format("<html><body style=\"background-color: rgba(2, 44, 0, 1);\"><p style=\"color:#FFFFFF;font-size:20px\">%s</p></body></html>", content.c_str());
//        webView->loadHTMLString(htmlstring);
////    }
////    else{
////        string htmlstring = StringUtils::format("<html><body style=\"background-color: rgba(71,19,10,1);\"><p style=\"color:#FFE3A0;font-size:20px\">%s</p></body></html>", content.c_str());
////        webView->loadHTMLString(htmlstring);
////    }
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	this->lblMailContent->setString(content);
//#endif
    
    setIDMail(id);
    
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself == NULL)
        return;
    //
    // 	if (sender.compare(*myself->Name()) == 0){
    // 		btnReply->setVisible(false);
    // 	}
}

// void LayerPopupMailDetails::onButtonReply(Ref* pSender)
// {
//
//     boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
//     if (myself == NULL)
//         return;
//     int accVipType = *myself->GetVariable("accVipType")->GetDoubleValue();
//     if (accVipType >= 2) {
//         auto layer = LayerSendMessage::create();
//         this->getParent()->addChild(layer);
//         layer->setSender(lblSender->getString());
//     //
//         this->removeFromParentAndCleanup(true);
//     }
//     else{
//         Chat *toast = Chat::create("Chức năng này chỉ dành cho VIP 2 trở lên!", -1);
//         this->addChild(toast);
//     }
//
//
// }

void LayerPopupMailDetails::onButtonDelete(Ref* pSender)
{
    boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
    if (myself != NULL) {
        SceneManager::getSingleton().showLoading();
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutUtfString("aI", myself->Name());
        params->PutInt("miid", getIDMail());
        boost::shared_ptr<IRequest> request(new ExtensionRequest("dmi", params));
        GameServer::getSingleton().Send(request);
    }
}

void LayerPopupMailDetails::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
    if (mySelf == NULL)
        return;
    
    if (strcmp("dmi", cmd->c_str()) == 0){
        Chat *toast = Chat::create(dataManager.GetSysString(441), -1);
        this->addChild(toast);
        //
        auto _parent = static_cast<LayerMailInbox*>(this->getParent());
        if (_parent)
            _parent->loadAllDatas();
        //
        this->removeFromParentAndCleanup(true);
    }
}

void LayerPopupMailDetails::onLabelMailTouch(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        Size tfsize = lblMailContent->getContentSize();
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            tfsize = Size(lblMailContent->getContentSize().width, lblMailContent->getFontSize() + 10);
        cocos2d::ui::EditBox *editbox = cocos2d::ui::EditBox::create(tfsize, "icon_transparent_default.png");
        editbox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        editbox->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2));
        
        editbox->setFont(kNormalFont, lblMailContent->getFontSize());
        editbox->setFontColor(Color3B::WHITE);
        editbox->setInputMode(cocos2d::ui::EditBox::InputMode::ANY);
        editbox->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
        editbox->setText(lblMailContent->getString().c_str());
        editbox->setVisible(false);
        addChild(editbox, 10000);
        editbox->touchDownAction(NULL, ui::Widget::TouchEventType::ENDED);
//        auto glview = Director::getInstance()->getOpenGLView();
//        if(glview)
//        {
//            glview->setIMEKeyboardState(false);
//        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerPopupMailDetails::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    sender->setVisible(false);
    return true;
    
}

void LayerPopupMailDetails::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    webView->setVisible(true);
}

void LayerPopupMailDetails::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    
}
#endif
void LayerPopupMailDetails::OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    this->removeFromParentAndCleanup(true);
}
