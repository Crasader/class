#include "LayerFriend.h"
#include "LayerFriendMain.h"
bool LayerFriend::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setKeyboardEnabled(true);

	auto visibleSize = Director::getInstance()->getVisibleSize();

	top = LayerFriendTop::create();
	top->setAnchorPoint(Vec2(0, 0));
	top->setPosition(Vec2(0, visibleSize.height - top->getContentSize().height));
	this->addChild(top);
	this->setTag(10);

	LayerFriendMain* main = LayerFriendMain::create();
	this->addChild(main);
	this->setTag(20);

	return true;

}

void LayerFriend::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event){
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		top->onButtonBack(this);
		
	}
}