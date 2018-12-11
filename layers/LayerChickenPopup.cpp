#include "LayerChickenPopup.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

LayerChickenPopup::LayerChickenPopup()
{
}


LayerChickenPopup::~LayerChickenPopup()
{
}

bool LayerChickenPopup::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerChickenPopup.csb");

	ui::Helper::doLayout(rootNode);

	this->addChild(rootNode);

	mListViewChicken = dynamic_cast<ListView*>(rootNode->getChildByName("lsvChicken"));

	loadAllDatas();

	return true;
}

bool LayerChickenPopup::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	this->removeFromParentAndCleanup(true);
	return true;
}

void LayerChickenPopup::onEnter()
{
	Layer::onEnter();
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerChickenPopup::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerChickenPopup::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}

void LayerChickenPopup::loadAllDatas()
{
	if (mListViewChicken == NULL)
		return;
	for (int i = 0; i < 4; i++){
		Button* _btn = Button::create();
		_btn->setTitleText(StringUtils::format("account %d", i + 1));
		_btn->setTitleColor(Color3B::BLACK);
		_btn->setContentSize(Size(300, 30));

		Layout *custom_item = Layout::create();
		custom_item->setContentSize(Size(300, 30));
		//_layer->setPosition(Vec2(custom_item->getContentSize().width / 2.0f, custom_item->getContentSize().height / 2.0f));
		custom_item->addChild(_btn);

		mListViewChicken->addChild(custom_item);
	}
}
