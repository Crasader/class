#include "LayerStep.h"
#include "ui/UIHelper.h"
#include "editor-support/cocostudio/ActionTimeline/CSLoader.h"
#include "data_manager.h"
USING_NS_CC;

// on "init" you need to initialize your instance
bool LayerStep::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	//    auto rootNode = CSLoader::createNode("LayerChonGame.csb");
	//    this->addChild(rootNode);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerStep.csb");

	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	lblTime = (Text*)rootNode->getChildByName("lblTime");

	Button* btnAdd = (Button*)rootNode->getChildByName("btnAdd");
	Button* btnSub = (Button*)rootNode->getChildByName("btnSub");

	btnAdd->addClickEventListener(CC_CALLBACK_1(LayerStep::onButtonAdd, this));
	btnSub->addClickEventListener(CC_CALLBACK_1(LayerStep::onButtonSub, this));

	return true;
}

LayerStep::LayerStep()
{

}

LayerStep::~LayerStep()
{

}

void LayerStep::setData(vector<int> _data)
{
	dataStep = _data;
}

void LayerStep::setTypeStep(TYPE_STEP _type)
{
	//update 25/7
	this->typeStep = _type;

	switch (_type)
	{
	case LayerStep::TIME:
		subText = dataManager.GetSysString(265);
		jumpUnit = 60;
		break;
	case LayerStep::CHAP_TIEN:
		subText = dataManager.GetSysString(266);
		jumpUnit = 1;
		break;
	default:
		break;
	}
}

void LayerStep::loadData()
{
	currIdx = 0;
	this->loadLabel();
}

void LayerStep::onButtonSub(Ref* pSender)
{
	if (currIdx == 0)
		return;
	currIdx--;
	this->loadLabel();
}

void LayerStep::onButtonAdd(Ref* pSender)
{
	if (currIdx >= (dataStep.size() - 1))
		return;
	currIdx++;
	this->loadLabel();
}

int LayerStep::getValueConfig()
{
	return (dataStep.at(currIdx)*jumpUnit);
}

void LayerStep::loadLabel()
{
	//update 25/7
	if (this->typeStep == TYPE_STEP::CHAP_TIEN)
	{
		string arrConfig[] = { "Không chấp", "Chấp tiên", "2 tiên", "3 tiên", "4 tiên" };
		this->lblTime->setString(arrConfig[this->currIdx]);
	}
	else{
		lblTime->setString(StringUtils::format("%d %s", dataStep.at(currIdx), subText.c_str()));
	}
}
//update 11/7
void LayerStep::setSelectedIndex(int idx)
{
	this->currIdx = idx;
	this->loadLabel();
}

void LayerStep::loadIndexFromValue(int value)
{
	for (int i = 0; i < this->dataStep.size(); i++)
	{
		if (this->dataStep[i] == value)
		{
			this->currIdx = i;
		}
	}
	this->loadLabel();
}

