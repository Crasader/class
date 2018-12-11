#include "LayerPopupBattle.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerPopupBattle::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
//    this->addChild(rootNode);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerPopupBattle.csb");
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	Layout* pnlMain = (Layout*)rootNode->getChildByName("pnlMain");
	if (pnlMain)
	{
	}
    return true;
}

LayerPopupBattle::LayerPopupBattle()
{
}

LayerPopupBattle::~LayerPopupBattle()
{

}


