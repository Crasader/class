#include "LayerPopupInviteFriend.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "mFaceBook.h"
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#include "IOS.h"
#include "mFaceBook.h"
#endif
LayerPopupInviteFriend::LayerPopupInviteFriend()
{

}

LayerPopupInviteFriend::~LayerPopupInviteFriend()
{

}

bool LayerPopupInviteFriend::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(false);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupInviteFriend.csb");
	auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_1"));
	pnlBg->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);


	auto btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose){
		btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteFriend::onBtnClose, this));
	}
	auto btnFacebook = dynamic_cast<Button*>(rootNode->getChildByName("btnFacebook"));
	if (btnFacebook){
		btnFacebook->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteFriend::onBtnFaceBook, this));
	}

	auto btnGMail = static_cast<ui::Button*>(rootNode->getChildByName("btnGmail"));
	if (btnGMail){
		btnGMail->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteFriend::onBtnGMail, this));
	}
    
    auto btnSMS = static_cast<ui::Button*>(rootNode->getChildByName("btnMail"));
    if (btnSMS){
        btnSMS->addClickEventListener(CC_CALLBACK_1(LayerPopupInviteFriend::onBtnSMS, this));
    }
	return true;
}

void LayerPopupInviteFriend::onBtnClose(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}
void LayerPopupInviteFriend::onBtnFaceBook(Ref* pSender)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
// 	mFaceBook *faceInvite = new mFaceBook();
// 	faceInvite->onInviteFriends();
#endif
}

void LayerPopupInviteFriend::onBtnGMail(Ref* pSender)
{
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	const char* address = "gamebai.net@gmail.com";
	const char* subject = "Invite play game";
	const char* content = "Play game with us";
	NativeHelper::callSendEmail(address, subject, content);
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    IOS::trySendAnEMail("10", true);
#endif
}
void LayerPopupInviteFriend::onBtnSMS(Ref* pSender)
{
	log("btnSMS press");
	std::string phoneNumber = "";
	std::string content = "Play game with us";
#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
	/*const char* title = subject.c_str();
	const char* msg = content.c_str(); */
	NativeHelper::inviteSMS(phoneNumber.c_str(), content.c_str());
#endif
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    IOS::trySendSMS("", "Let's play GameBai.Net!");
#endif
}
