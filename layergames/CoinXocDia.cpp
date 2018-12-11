#include "CoinXocDia.h"
#include "TaiXiuMessageDefs.h"
#include "mUtils.h"
#include "LayerGameXocDia.h"
#include "../SceneManager.h"
CoinXocDia::CoinXocDia(string type, string normal, string disable)
{
	pType = type;
	pImageNormal = normal;
	pImageDisable = disable;
}

CoinXocDia::~CoinXocDia()
{

}

CoinXocDia* CoinXocDia::create(string type, string normal, string disable)
{
	CoinXocDia *coinXocDia = new CoinXocDia(type, normal, disable);
	if (coinXocDia && coinXocDia->init()){
		coinXocDia->autorelease();
		return coinXocDia;
	}
	else{
		delete coinXocDia;
		coinXocDia = NULL;
		return NULL;
	}
}

bool CoinXocDia::init()
{
	if (!Button::init())
		return false;
	this->loadTextureNormal(pImageNormal);
	return true;
}

void CoinXocDia::SetSelected(bool state)
{
	if (state)
	{
		this->loadTextureNormal(pImageNormal);
	}
	else {
		this->loadTextureNormal(pImageDisable);
	}
}

std::string CoinXocDia::getType()
{
	return pType;
}

int CoinXocDia::getCoinVal()
{
	if (0 == pType.compare(BET_VALUE_1K))
		return 1000;
	else if (0 == pType.compare(BET_VALUE_2K))
		return 2000;
	else if (0 == pType.compare(BET_VALUE_5K))
		return 5000;
	else if (0 == pType.compare(BET_VALUE_10K))
		return 10000;
	else if (0 == pType.compare(BET_VALUE_50K))
		return 50000;
	else if (0 == pType.compare(BET_VALUE_100K))
		return 100000;
	else if (0 == pType.compare(BET_VALUE_500K))
		return 500000;
	else if (0 == pType.compare(BET_VALUE_1M))
		return 1000000;
	else
		return 0;
}

std::string CoinXocDia::getSrc()
{
	return pImageNormal;
}

cocos2d::Vec2 CoinXocDia::getPositionCoin()
{
	return this->getPosition();
}


LayerAnimationXocDia::LayerAnimationXocDia(Vec2 pos, int type)
{
	posFrame = pos;
	curType = type;
	fnTarget = NULL;
	//this->listener = NULL;
}

LayerAnimationXocDia::~LayerAnimationXocDia()
{
	this->stopAllActions();
}

LayerAnimationXocDia* LayerAnimationXocDia::create(Vec2 pos, int type)
{
	LayerAnimationXocDia *layerAnim = new LayerAnimationXocDia(pos, type);
	if (layerAnim && layerAnim->init()){
		layerAnim->autorelease();
		return layerAnim;
	}
	else{
		delete layerAnim;
		layerAnim = NULL;
		return NULL;
	}
}

bool LayerAnimationXocDia::init()
{
	if (!Layer::init())
		return false;

	if (curType == 1){

		cocos2d::Vector<SpriteFrame*> animFrames;
		SpriteFrame *frame;
		Animation *animFrame;
		string str = "";
		for (int i = 2; i >= 1; i--){
			str = StringUtils::format("dia-4.png", i);
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
		}
		animFrame = Animation::createWithSpriteFrames(animFrames, 1);
		Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.1, 1);

		auto animCache = AnimationCache::getInstance();
		animCache->addAnimation(animation, "dance");
		Animation* normal = animCache->getAnimation("dance");
		normal->setRestoreOriginalFrame(true);

		Animate* animN = Animate::create(normal);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("dia-4.png");

		Sprite *_sprite = Sprite::createWithSpriteFrame(frame);
		_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_sprite->setPosition(Vec2(posFrame.x + 10, posFrame.y + 5));
		_sprite->setScale(1.2);
		this->addChild(_sprite);

		CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerAnimationXocDia::CallFuncRemove, this));
		Action* move = MoveTo::create(0.7, Vec2(posFrame.x - 300, posFrame.y));
		Action* scale = ScaleTo::create(0.2, 1.2);
		_sprite->runAction(Sequence::create(animN, move, call, nullptr));
	}
	else{
		cocos2d::Vector<SpriteFrame*> animFrames;
		SpriteFrame *frame;
		Animation *animFrame;
		string str = "";
		for (int i = 3; i >= 1; i--){
			str = StringUtils::format("dia-%d.png", i);
			frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
		}
		animFrame = Animation::createWithSpriteFrames(animFrames, 1);
		Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.1, 8);

		auto animCache = AnimationCache::getInstance();
		animCache->addAnimation(animation, "dance");
		Animation* normal = animCache->getAnimation("dance");
		normal->setRestoreOriginalFrame(true);

		Animate* animN = Animate::create(normal);
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("dia-3.png");

		Sprite *_sprite = Sprite::createWithSpriteFrame(frame);
		_sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_sprite->setPosition(posFrame);
		_sprite->setScale(1.2);
//        if (SceneManager::getSingleton().isNagaWin){
//            _sprite->setScale(2.4);
//
//        }
		this->addChild(_sprite);

		CallFunc* call = CallFunc::create(CC_CALLBACK_0(LayerAnimationXocDia::CallFuncRemove, this));
		Action* move = MoveTo::create(0.7, Vec2(posFrame.x - 300, posFrame.y));
		Action* scale = ScaleTo::create(0.2, 1.2);
		_sprite->runAction(Sequence::create(animN, call, nullptr));
	}
	return true;
}

void LayerAnimationXocDia::setTarget(Ref* _target)
{
	fnTarget = _target;
}

void LayerAnimationXocDia::CallFuncRemove()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
	//update for bautom
	if (this->listener != NULL)
	{
		this->runAction(Sequence::create(CallFunc::create(listener), DelayTime::create(0.2), FadeOut::create(0.1), RemoveSelf::create(), NULL));
		return;
	}


	LayerGameXocDia* parent = dynamic_cast<LayerGameXocDia*> (this->getParent());
	if (parent)
	{
		parent->CallBackFromAnim();
	}
	this->removeFromParent();

}

void LayerAnimationXocDia::setCallBackListener(const std::function<void()> &listener)
{
	this->listener = listener;
}

LayerHistoryXocDia::LayerHistoryXocDia()
{

}

LayerHistoryXocDia::~LayerHistoryXocDia()
{

}

bool LayerHistoryXocDia::init()
{
	if (!Layer::init())
		return false;
	auto rootNode = CSLoader::getInstance()->createNode("LayerHistoryXocDia.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);
	this->setContentSize(rootNode->getContentSize());

	this->pnlHistory = dynamic_cast<Layout*>(rootNode->getChildByName("pnlHistory"));
	this->btnClose = dynamic_cast<Button*>(rootNode->getChildByName("btnClose"));
	/*moc3- change begin*/
	/*
	ImageView *history = ImageView::create("moc3-history-bg.png");
	history->setAnchorPoint(Vec2::ZERO);
	history->setPosition(Vec2::ZERO);
	this->addChild(history);
	this->setContentSize(history->getContentSize());

	Label* lblChan = Label::createWithBMFont("robotofont.fnt","Chẵn");
	lblChan->setAnchorPoint(Vec2(0, 0.5));
	lblChan->setPosition(Vec2(20, 33 * 3 + 33/ 2 + 5));
	this->addChild(lblChan);

	lblNumChan = Label::createWithBMFont("robotofont.fnt", "");
	lblNumChan->setAnchorPoint(Vec2(0, 0.5));
	lblNumChan->setPosition(Vec2(20, 33 * 2 + 33/ 2 + 5));
	this->addChild(lblNumChan);

	Label* lblLe = Label::createWithBMFont("robotofont.fnt", "Lẻ");
	lblLe->setAnchorPoint(Vec2(0, 0.5));
	lblLe->setPosition(Vec2(20, 33 + 33 / 2));
	lblLe->setColor(Color3B::BLACK);
	this->addChild(lblLe);

	lblNumLe = Label::createWithBMFont("robotofont.fnt", "");
	lblNumLe->setAnchorPoint(Vec2(0, 0.5));
	lblNumLe->setPosition(Vec2(20, 33 / 2));
	this->addChild(lblNumLe);*/
	/*moc3-end change*/
	return true;
}

void LayerHistoryXocDia::setListHistory(string lst)
{
	for (int i = 0; i < arrResult.size(); i++)
	{
		arrResult.at(i)->removeFromParentAndCleanup(true);
	}
	arrResult.clear();
	int chan = 0;
	int le = 0;
	vector<string> arr = mUtils::splitString(lst, ';');
	int start = this->pnlHistory->getPositionX();
	for (int i = 0; i < arr.size(); i++)
	{
		vector<string> info = mUtils::splitString(arr.at(i), '_');
		int _odd = 0;
		for (int j = 0; j < info.size(); j++)
		{
			string str = "moc3-item-red-history.png";
			if (atoi(info.at(j).c_str()) == XOC_DIA_CHAN)
			{
				str = "moc3-item-red-history.png";
				_odd++;
			}
			else{
				str = "moc3-item-yellow-history.png";
			}

			Sprite* img = Sprite::create(str);
			img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			img->setPosition(Vec2(start + i * 50 + 50 / 2, this->pnlHistory->getPositionY() + 50 / 2 + 50 * j));
			this->addChild(img);
			arrResult.push_back(img);
		}
		if (_odd % 2 == 0)
			chan++;
		else
			le++;
	}
	/*lblNumChan->setString(boost::to_string(chan));
	lblNumLe->setString(boost::to_string(le));*/
}

Button* LayerHistoryXocDia::getButtonClose()
{
	return this->btnClose;
}
