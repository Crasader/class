#include "LayerFriendTop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../scenes/SceneMain.h"
#include "LayerPopupInviteFriend.h"
using namespace ui;
LayerFriendTop::LayerFriendTop()
{

}

LayerFriendTop::~LayerFriendTop()
{

}


bool LayerFriendTop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerFriendsTop.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	btnThongtin = dynamic_cast<Button*>(rootNode->getChildByName("btnThongtin"));
	if (btnThongtin != NULL)
	{
		btnThongtin->addClickEventListener(CC_CALLBACK_1(LayerFriendTop::onButtonThongtin, this));
	}
	btnInvite = dynamic_cast<Button*>(rootNode->getChildByName("btnInvite"));
	if (btnInvite != NULL)
	{
		btnInvite->addClickEventListener(CC_CALLBACK_1(LayerFriendTop::onButtonMoiBanBe, this));
	}
	auto layerTopMenuOnlyTop = dynamic_cast<Node*>(rootNode->getChildByName("LayerTopMenuOnlyBack"));
	if (layerTopMenuOnlyTop != NULL) {
		auto btn_back = dynamic_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnBack"));
		auto btnOut = dynamic_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnOut"));
		if (btnOut)
		{
			btnOut->addClickEventListener(CC_CALLBACK_1(LayerFriendTop::onButtonBack, this));
		}
	}
	onButtonThongtin(NULL);

	return true;
}


void LayerFriendTop::onButtonThongtin(Ref* pSender)
{
	Color3B _colorPress = Color3B(175, 8, 0);
	Color3B _colorNormal = Color3B(255, 236, 192);
	btnThongtin->setEnabled(false);
	btnThongtin->setTitleColor(_colorPress);
	btnInvite->setEnabled(true);
	btnInvite->setTitleColor(_colorNormal);
}

void LayerFriendTop::onButtonMoiBanBe(Ref* pSender)
{
	auto layer = LayerPopupInviteFriend::create();
	SceneMain* _parent = (SceneMain*)this->getParent()->getParent();
	if (_parent != NULL){
		_parent->addChild(layer, 10000);
	}
}

void LayerFriendTop::onButtonBack(Ref* pSender)
{
	
	if (this->getParent() == NULL)
		return;
	SceneMain* _parent = (SceneMain*)this->getParent()->getParent();
	if (_parent != NULL){
		_parent->gotoPickGame();
	}
}