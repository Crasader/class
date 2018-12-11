#include "LayerToast.h"
#include "CommonChess.h"

LayerToast::LayerToast(const string& msg)
{
	_message = msg;
}

LayerToast::~LayerToast()
{

}

bool LayerToast::init()
{
	if (!Layer::init())
	{
		return false;
	}
	float _maxWidth = 600;
	float _maxWidthBlocks = 650;
	Size _sizeBlocks = Size(_maxWidthBlocks, FONT_SIZE_NORMAL_POPUP* 2.5);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 	Label* label = Label::createWithTTF(_message, "__Roboto-Bold.ttf", FONT_SIZE_ITEM,
	// 		Size(_maxWidth, FONT_SIZE_NORMAL_POPUP* 2.5),
	// 		TextHAlignment::CENTER, TextVAlignment::CENTER);
	Label* label = Label::createWithSystemFont(_message, "", FONT_SIZE_ITEM);
	label->setHorizontalAlignment(TextHAlignment::CENTER);
	label->setLineBreakWithoutSpace(false);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	Scale9Sprite* blocks = Scale9Sprite::create("s_9sprite_toast.png");
	//blocks->setContentSize(_sizeBlocks);
	blocks->setContentSize(Size(label->getContentSize().width + 50, _sizeBlocks.height));
	blocks->setOpacity(200);
	blocks->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	blocks->setPosition(Vec2::ZERO);
	this->addChild(blocks);

	label->setPosition(blocks->getContentSize().width / 2, _sizeBlocks.height / 2);
	blocks->addChild(label);
	this->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	return true;
}

void LayerToast::showToast(float duration)
{
	Node* _currToast = Director::getInstance()->getRunningScene()->getChildByTag(LAYER_POPUP_ID::POPUP_TOAST);
	if (_currToast != NULL)
	{
		_currToast->runAction(Sequence::create(FadeOut::create(0.02), RemoveSelf::create(), nullptr));
	}
	this->runAction(Sequence::create(FadeIn::create(0.01), DelayTime::create(duration), RemoveSelf::create(), nullptr));
}

LayerToast* LayerToast::create(const string &pMes)
{
	LayerToast* _toast = new LayerToast(pMes);
	if (_toast && _toast->init())
	{
		_toast->autorelease();
		return _toast;
	}
	else{
		delete _toast;
		_toast = NULL;
		return NULL;
	}
}
