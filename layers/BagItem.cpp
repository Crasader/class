#include "BagItem.h"
#include "../SceneManager.h"
#include "../layergames/PhomMessDef.h"
bool BagItem::init()
{
	if (!Layer::init())
	{
		return false;
	}

    bg = Sprite::create("top-badge.png");
	this->addChild(bg);
	this->setContentSize(bg->getContentSize());

	labelNumber = Label::createWithTTF("1", "__Roboto-Bold.ttf", 30);
	bg->addChild(labelNumber);
	labelNumber->setAnchorPoint(Vec2(0.5, 0.5));
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
        labelNumber->setPosition(Vec2(bg->getContentSize().width / 2-1, bg->getContentSize().height / 2+1));
    }else{
        labelNumber->setPosition(Vec2(bg->getContentSize().width / 2-1, bg->getContentSize().height / 2));
    }
	return true;
}

void BagItem::setNumber(string msg)
{
	labelNumber->setString(msg);
}
void BagItem::runActionNoti(){
    bg->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.4),ScaleTo::create(0.8, 1.0), NULL)));
}
