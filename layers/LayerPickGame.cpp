#include "LayerPickGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;


LayerPickGame::LayerPickGame()
{
}


LayerPickGame::~LayerPickGame()
{
}

bool LayerPickGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto rootNode = CSLoader::getInstance()->createNode("LayerMainContent.csb");

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	this->setContentSize(rootNode->getContentSize());

	return true;
}
