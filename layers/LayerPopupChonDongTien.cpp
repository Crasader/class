
#include "LayerPopupChonDongTien.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../layergames/ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "../layergames/PhomMessDef.h"
#include "Requests/LogoutRequest.h"
#include "../data_manager.h"
#include "../layergames/_Chat_.h"

using namespace ui;
using namespace cocos2d;
LayerPopupChonDongTien::LayerPopupChonDongTien()
{
}


LayerPopupChonDongTien::~LayerPopupChonDongTien()
{

}

bool LayerPopupChonDongTien::init()
{
	return true;
}

bool LayerPopupChonDongTien::init(int _gameId)
{
	if (!Layer::init())
	{
		return false;
	}
	boost::shared_ptr<double> am, amf, amfs;
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	if (myself == NULL)
		return true;
	amf = myself->GetVariable("amf")->GetDoubleValue();
	amfs = myself->GetVariable("amfs")->GetDoubleValue();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->gameId = _gameId;

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupChonHinhThucChoi.csb");
	rootNode->setAnchorPoint(Point(0.5, 0.5));
	rootNode->setPosition(Point(visibleSize / 2));
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
    
    auto lblTitle = static_cast<Text*> (rootNode->getChildByName("Text_15"));
    lblTitle->setString(dataManager.GetSysString(874));
    
    auto lblTip = static_cast<Text*> (rootNode->getChildByName("Text_15_0"));
    lblTip->setString(dataManager.GetSysString(875));

	auto btnClose = static_cast<Button*>(rootNode->getChildByName("btnClose"));
	if (btnClose != NULL){
		btnClose->addTouchEventListener(CC_CALLBACK_2(LayerPopupChonDongTien::onBtnClose, this));
	}

	auto btnMoneyMoc = static_cast<Button*>(rootNode->getChildByName("btnPlayDoiThuong"));
	if (btnMoneyMoc != NULL){
		auto lblMoc = static_cast<Text*>(btnMoneyMoc->getChildByName("lblMoneyDoiThuong"));
		lblMoc->setString(mUtils::convertMoneyEx(*amf).c_str());
		btnMoneyMoc->addTouchEventListener(CC_CALLBACK_2(LayerPopupChonDongTien::onButtonMocTouched, this));
	}

	auto btnMoneyHao = static_cast<Button*>(rootNode->getChildByName("btnPlayFree"));
	if (btnMoneyHao != NULL){
		auto lblHao = static_cast<Text*>(btnMoneyHao->getChildByName("lblMoneyFree"));
		lblHao->setString(mUtils::convertMoneyEx(*amfs).c_str());
		btnMoneyHao->addTouchEventListener(CC_CALLBACK_2(LayerPopupChonDongTien::onButtonHaoTouched, this));
	}
    
    auto Text_15_0_0 = static_cast<Text*>(rootNode->getChildByName("Text_15_0_0"));
    Text_15_0_0->setString(SceneManager::getSingleton().getCoin(0));
    
    auto Text_15_0_0_0 = static_cast<Text*>(rootNode->getChildByName("Text_15_0_0_0"));
    Text_15_0_0_0->setString(SceneManager::getSingleton().getCoin(1));
	return true;
}

void LayerPopupChonDongTien::onBtnClose(Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
	if (type == ui::Widget::TouchEventType::ENDED){
		this->removeFromParentAndCleanup(true);
	}
}

void LayerPopupChonDongTien::onEnter()
{
	Layer::onEnter();

}

void LayerPopupChonDongTien::onExit()
{
	Layer::onExit();
}

LayerPopupChonDongTien* LayerPopupChonDongTien::create()
{
	LayerPopupChonDongTien* pRet = new LayerPopupChonDongTien();
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

LayerPopupChonDongTien* LayerPopupChonDongTien::create(int _gameId)
{
	LayerPopupChonDongTien *pRet = new LayerPopupChonDongTien();
	if (pRet && pRet->init(_gameId)){
		pRet->autorelease();
		return pRet;
	}
	else{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

void LayerPopupChonDongTien::onButtonMocTouched(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		SceneManager::getSingleton().setCurrRoomType(0);
		SceneManager::getSingleton().showLoading();
		SceneManager::getSingleton().gotoPickGame(this->gameId);
		this->removeFromParent();
	}
}

void LayerPopupChonDongTien::onButtonHaoTouched(Ref* pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED){
		SceneManager::getSingleton().setCurrRoomType(1);
		SceneManager::getSingleton().showLoading();
		SceneManager::getSingleton().gotoPickGame(this->gameId);
		this->removeFromParent();
	}
}

