#include "LayerTCC_OneDiceHistory.h"
#include "PhomMessDef.h"
#include "TCCMsgDef.h"

bool LayerTCC_OneDiceHistory::init()
{
	if (!Layer::init())
		return false;
	this->createDices();
	return true;
}

void LayerTCC_OneDiceHistory::createDices()
{
	Size layerSize = Size(135, 60);
	for (int i = 0; i < 6; i++)
	{
		LayerItemTCC_OneDicesHis* layerDice = LayerItemTCC_OneDicesHis::create(i);
		layerDice->setAnchorPoint(Vec2::ZERO);
		layerDice->setPosition(Vec2(15, layerSize.height * i+13));
		layerDice->setName(getBetTypeById(i));
		this->addChild(layerDice);
	}
}

void LayerTCC_OneDiceHistory::setDataHis(string _data)
{
	//"glb":""BAU|4;TOM|6;CUA|14;CA|6;HUOU|18;GA|0"
	vector<string> arr_data = mUtils::splitString(_data, ';');
	for (int i = 0; i < arr_data.size(); i++) {
		vector<string> arrInfo = mUtils::splitString(arr_data.at(i), '|');
		if (arrInfo.size() == 2)
		{
			string bettype = arrInfo[0];
			string res = arrInfo[1];
			LayerItemTCC_OneDicesHis* layerDice = dynamic_cast<LayerItemTCC_OneDicesHis*>(this->getChildByName(bettype));
			if (layerDice) {
				layerDice->setDataDice(res);
			}
		}
		
	}
}

bool LayerItemTCC_OneDicesHis::init()
{
	if (!Layer::init())
		return false;
	auto root = CSLoader::getInstance()->createNode("LayerTCC_ItemOneDice.csb");
	ui::Helper::doLayout(root);
	this->addChild(root);
	this->setContentSize(root->getContentSize());
	this->setAnchorPoint(Vec2::ZERO);
	this->imgDice = (ImageView*)root->getChildByName("imgDice");
	this->lblCount = (Text*)root->getChildByName("lblCount");
	if (this->imgDice){
		string url = getImgDiceX2ById(this->_id);
		this->imgDice->loadTexture(url);
	}

	return true;
}

void LayerItemTCC_OneDicesHis::setDataDice(string _result)
{
	this->lblCount->setString(_result);
}

LayerItemTCC_OneDicesHis::LayerItemTCC_OneDicesHis(int _id)
{
	this->_id = _id;
}

LayerItemTCC_OneDicesHis::~LayerItemTCC_OneDicesHis()
{

}

LayerItemTCC_OneDicesHis* LayerItemTCC_OneDicesHis::create(int _id)
{
	LayerItemTCC_OneDicesHis* layerItem = new LayerItemTCC_OneDicesHis(_id);
	if (layerItem && layerItem->init())
	{
		layerItem->autorelease();
		return layerItem;
	}
	else{
		delete layerItem;
		layerItem = NULL;
		return NULL;
	}
}


