#include "LayerRankTop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "LayerRank.h"
#include "../layergames/PhomMessDef.h"

#include "../scenes/SceneMain.h"

USING_NS_CC;

LayerRankTop::LayerRankTop()
{
}


LayerRankTop::~LayerRankTop()
{
}

void LayerRankTop::onButtonTopDaiGia(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		btnTopCaoThu->setEnabled(true);
		btnTopDaiGia->setEnabled(false);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			/*btnTopCaoThu->setTitleColor(ccc3(255, 255, 255));
			btnTopDaiGia->setTitleColor(ccc3(26, 26, 26));*/
		}
		LayerRank* layer = (LayerRank*)this->getParent();
		if (layer)
			layer->showTopDaiGia();
	}

}

void LayerRankTop::onButtonTopCaoThu(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		btnTopCaoThu->setEnabled(false);
		btnTopDaiGia->setEnabled(true);
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
			/*btnTopDaiGia->setTitleColor(ccc3(255, 255, 255));
			btnTopCaoThu->setTitleColor(ccc3(26, 26, 26));*/
		}
		LayerRank* layer = (LayerRank*)this->getParent();
		if (layer)
			layer->showTopCaoThu();
	}
}

void LayerRankTop::onButtonBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		if (this->getParent() == NULL)
			return;
		SceneMain* _parent = (SceneMain*)this->getParent()->getParent();
		if (_parent != NULL){
			_parent->gotoPickGame();
		}
	}
}

bool LayerRankTop::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerRankTop.csb");

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
		this->isWebView = false;
	}
	else{
		this->isWebView = true;
	}

	btnTopCaoThu = dynamic_cast<Button*>(rootNode->getChildByName("btnTopCaoThu"));
	if (btnTopCaoThu != NULL){
		btnTopCaoThu->addTouchEventListener(CC_CALLBACK_2(LayerRankTop::onButtonTopCaoThu, this));
	}
	btnTopDaiGia = dynamic_cast<Button*>(rootNode->getChildByName("btnTopDaiGia"));
	if (btnTopDaiGia != NULL){
		btnTopDaiGia->addTouchEventListener(CC_CALLBACK_2(LayerRankTop::onButtonTopDaiGia, this));
	}
	if (isWebView){
		//btnTopCaoThu->setVisible(false);
		btnTopCaoThu->setTitleText("BXH");
		btnTopDaiGia->setVisible(false);
	}
	//get layerTopMenuOnlyTop

	auto layerTopMenuOnlyTop = dynamic_cast<Node*>(rootNode->getChildByName("layerTopMenuOnlyTop"));
	if (layerTopMenuOnlyTop != NULL){
		auto btnClose = dynamic_cast<Button*>(layerTopMenuOnlyTop->getChildByName("btnBack"));
		if (btnClose != NULL){
			btnClose->setPressedActionEnabled(true);
			btnClose->addTouchEventListener(CC_CALLBACK_2(LayerRankTop::onButtonBack, this));
		}

	}
	this->onButtonTopCaoThu(NULL, Widget::TouchEventType::ENDED);

	this->setContentSize(rootNode->getContentSize());

	return true;
}
