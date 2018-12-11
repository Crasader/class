#include "LayerTCC_SlotMachine.h"

LayerTCC_SLotMachine::LayerTCC_SLotMachine()
{
	numItem = 0;
	rewardItem = -1;
	rewardPosY = 0;
	slotSize = Size(200, 200);
	mTarget = NULL;
	mFunctionSelector = NULL;
}

LayerTCC_SLotMachine::~LayerTCC_SLotMachine()
{

}

bool LayerTCC_SLotMachine::init()
{
	if (!Layer::init())
		return false;
	scrollView = ui::ScrollView::create();
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setTouchEnabled(false);
	scrollView->setContentSize(Size(170, 300));
	scrollView->setAnchorPoint(Vec2::ZERO);
	scrollView->setPosition(Vec2::ZERO);
	this->addChild(scrollView);
	innerContainer = scrollView->getInnerContainer();
	//scrollView->setvi

	return true;
}

void LayerTCC_SLotMachine::onEnter()
{
	Layer::onEnter();
}

void LayerTCC_SLotMachine::onExit()
{
	Layer::onExit();
}

void LayerTCC_SLotMachine::setCallBack(const std::function<void()> &listener)
{
	this->listener = listener;
}

void LayerTCC_SLotMachine::setCallBack(Ref* target, SEL_CallFuncND selector)
{
	this->mTarget = target;
	this->mFunctionSelector = selector;
}

// void LayerTCC_SLotMachine::setCallBack(const std::function<void()> &listener, Ref *target)
// {
// 	this->listener = listener;
// 	this->target = target;
// }

void LayerTCC_SLotMachine::setShowSize(Size _size)
{
	scrollView->setContentSize(_size);
}

void LayerTCC_SLotMachine::insertSlot(vector<string> listRes, int repeatSlot)
{
	int nloop = 1;
	if (repeatSlot > 1)
		nloop = repeatSlot;
	numItem = listRes.size();
	int dem = 0;
	for (int i = 0; i < nloop; i++){
		for (int k = 0; k < listRes.size(); k++){
			Sprite* spriteSlot = Sprite::create(listRes.at(k));
			spriteSlot->setAnchorPoint(Vec2::ZERO);
			spriteSlot->setPosition(Vec2(0, slotSize.height*dem));
			scrollView->addChild(spriteSlot);
			dem++;
		}
	}
	scrollView->setInnerContainerSize(Size(slotSize.width, slotSize.height / 2 * repeatSlot * numItem));
}

void LayerTCC_SLotMachine::setRewardItem(int idx)
{
	rewardItem = idx;
	caculatePosition();
}

void LayerTCC_SLotMachine::caculatePosition()
{
	float containnerHeight = innerContainer->getContentSize().height;
	float currPosY = innerContainer->getPositionY();
	rewardPosY = -(containnerHeight - (numItem - rewardItem) * (slotSize.height));
}

void LayerTCC_SLotMachine::setFirstSlot(int idx)
{
	innerContainer->setPositionY(-(idx*slotSize.height));
}

void LayerTCC_SLotMachine::startTurn(float time)
{
	auto action = MoveTo::create(time, Vec2(0, rewardPosY));
	auto easeOut = EaseCircleActionOut::create(action);
	innerContainer->runAction(Sequence::create(easeOut,
		CallFunc::create(CC_CALLBACK_0(LayerTCC_SLotMachine::onRunSlotDone, this)), NULL));
}

void LayerTCC_SLotMachine::setResPath(string path)
{
	resPath = path;
}


void LayerTCC_SLotMachine::onRunSlotDone()
{
	int _tag = this->getTag();
	//this->runAction(CallFunc::create(listener));
	if (mTarget && mFunctionSelector)
		(mTarget->*mFunctionSelector)(this, (void*)_tag);
}

void LayerTCC_SLotMachine::setSlotSize(Size _size)
{
	slotSize = _size;
}
