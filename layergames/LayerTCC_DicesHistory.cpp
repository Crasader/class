#include "LayerTCC_DicesHistory.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"


bool LayerItemTCC_DicesHis::init()
{
	if (!Layer::init())
		return false;
	root = CSLoader::getInstance()->createNode("LayerTCC_ItemDice.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	this->setAnchorPoint(Vec2::ZERO);

	return true;
}

void LayerItemTCC_DicesHis::setDataDice(string _result)
{
	vector<string> arrStr = mUtils::splitString(_result, '_');
	for (int i = 0; i < arrStr.size(); i++)
	{
		int id = getIdByBetType(arrStr.at(i).c_str());
		if (id == -1)
			continue;
		string url = getImgDiceById(id);
		ImageView* img = (ImageView*)root->getChildByName(StringUtils::format("imgRes%d", (i + 1)));
		if (img)
		{
			img->loadTexture(url);
		}
	}
}

void LayerItemTCC_DicesHis::loadFirst()
{
	for (int i = 0; i < 3; i++)
	{
		ImageView* img = (ImageView*)root->getChildByName(StringUtils::format("imgRes%d", (i + 1)));
		if (img)
		{
			img->loadTexture("dice_empty.png");
		}
	}
}

LayerItemTCC_DicesHis::LayerItemTCC_DicesHis()
{

}

LayerItemTCC_DicesHis::~LayerItemTCC_DicesHis()
{

}


LayerTCC_DicesHistory::LayerTCC_DicesHistory()
{

}

LayerTCC_DicesHistory::~LayerTCC_DicesHistory()
{
	arrDices.clear();
}

bool LayerTCC_DicesHistory::init()
{
	if (!Layer::init())
		return false;

	scrollView = cocos2d::ui::ScrollView::create();
	scrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(220, 365));
	scrollView->setAnchorPoint(Vec2(0, 1));
	scrollView->setPosition(Vec2(0, 365));
	scrollView->setBounceEnabled(true);
	scrollView->setScrollBarEnabled(false);
	this->addChild(scrollView);

	return true;
}

void LayerTCC_DicesHistory::createDices()
{
	Size layerSize = Size(170, 50);
	for (int i = 0; i < 6; i++)
	{
		LayerItemTCC_DicesHis *layerDice = LayerItemTCC_DicesHis::create();
		layerDice->setAnchorPoint(Vec2::ZERO);
		layerDice->setPosition(0, layerSize.height* i);
		arrDices.push_back(layerDice);
		this->addChild(layerDice);
	}
}

void LayerTCC_DicesHistory::setDataHis(string _data)
{
	vector<string> arrHis = mUtils::splitString(_data, ';');
	int lengthh = arrHis.size();
	if (lengthh == 0)
	{
		this->showFirst();
		return;
	}
	if (arrHis.size() > 20)
		lengthh = 20;
	scrollView->removeAllChildrenWithCleanup(true);
	scrollView->setInnerContainerSize(Size(170, 50 * lengthh));
	int posX = 0;
	int posY =50;
	for (int i = 0; i < lengthh; i++)
	{
		vector<string> arrRes = mUtils::splitString(arrHis.at(i), '|');
		if (arrRes.size() == 2)
		{
			string res = arrRes.at(1);
			LayerItemTCC_DicesHis *item = LayerItemTCC_DicesHis::create();
			item->setDataDice(res);
			item->setPosition(30, posY*i);
			scrollView->addChild(item);
		}
	}
}

void LayerTCC_DicesHistory::showFirst()
{
	int posX = 0;
	int posY = 50;
	scrollView->setInnerContainerSize(Size(170, 50 * 6));
	for (int i = 0; i < 6; i++)
	{
		LayerItemTCC_DicesHis *item = LayerItemTCC_DicesHis::create();
		item->loadFirst();
		item->setPosition(0, posY*i);
		scrollView->addChild(item);
	}
}

