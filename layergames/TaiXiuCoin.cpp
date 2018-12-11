#include "TaiXiuCoin.h"



TaiXiuCoin::TaiXiuCoin(Button* item, const std::string& name, const std::string& type, const int& tag)
:pMenuItem(item), pName(name), pType(type), pTag(tag)
{
	if (this->pMenuItem){
		this->pMenuItem->setTag(tag);
		this->pMenuItem->setPressedActionEnabled(true);
	}
}

TaiXiuCoin::~TaiXiuCoin() {
	log("destructor %s", this->pType.c_str());
}

std::string TaiXiuCoin::GetType() const {
	return this->pType;
}

int TaiXiuCoin::GetTag() const {
	return this->pTag;
}

Vec2 TaiXiuCoin::GetPos() const
{
	return this->pMenuItem->getPosition();
}

void TaiXiuCoin::SetSelected(bool select) {
	if (select){
		this->pMenuItem->loadTextureNormal(this->GetValue().imgnormal);
        Sprite* child = (Sprite*)this->pMenuItem->getChildByTag(124);
        if (!child) {
            Sprite* sp=Sprite::create("tx-chip-touched.png");
            this->pMenuItem->addChild(sp);
            sp->setScale(0.98);
            sp->setAnchorPoint(Vec2(0.5, 0.5));
            sp->setPosition(Vec2(this->pMenuItem->getContentSize().width/2,this->pMenuItem->getContentSize().height/2));
            sp->setTag(124);
        }
        
	}
	else
	{
		this->pMenuItem->loadTextureNormal(this->GetValue().imgselect);
        this->pMenuItem->removeChildByTag(124);
	}
}

Vec2 TaiXiuCoin::GetPosWorld(){
	return this->pMenuItem->getParent()->convertToWorldSpace(this->pMenuItem->getPosition());
}

//
TaiXiuCoin1::TaiXiuCoin1(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag)
{
}

CoinStruct TaiXiuCoin1::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-01B.png";
	a.imgselect = "tx-chip-01B.png";
	a.value = 1;
	return a;
}
//
TaiXiuCoin100::TaiXiuCoin100(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag)
{
}

CoinStruct TaiXiuCoin100::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-100B.png";
	a.imgselect = "tx-chip-100B.png";
	a.value = 100;
	return a;
}
//
//
TaiXiuCoin500::TaiXiuCoin500(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag)
{
}

CoinStruct TaiXiuCoin500::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-500B.png";
	a.imgselect = "tx-chip-500B.png";
	a.value = 500;
	return a;
}
//
TaiXiuCoin1K::TaiXiuCoin1K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag)
{
}

CoinStruct TaiXiuCoin1K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-01.png";
	a.imgselect = "tx-chip-01.png";
	a.value = 1000;
	return a;
}
//
TaiXiuCoin2K::TaiXiuCoin2K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag)
{
}

CoinStruct TaiXiuCoin2K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-2k.png";
	a.imgselect = "tx-chip-2k.png";
	a.value = 2000;
	return a;
}
//
TaiXiuCoin5K::TaiXiuCoin5K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin5K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-02.png";
	a.imgselect = "tx-chip-02.png";
	a.value = 5000;
	return a;
}

//
TaiXiuCoin10K::TaiXiuCoin10K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin10K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-03.png";
	a.imgselect = "tx-chip-03.png";
	a.value = 10000;
	return a;
}

//
TaiXiuCoin50K::TaiXiuCoin50K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin50K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-04.png";
	a.imgselect = "tx-chip-04.png";
	a.value = 50000;
	return a;
}

//
TaiXiuCoin100K::TaiXiuCoin100K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin100K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-05.png";
	a.imgselect = "tx-chip-05.png";
	a.value = 100000;
	return a;
}

//
TaiXiuCoin500K::TaiXiuCoin500K(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin500K::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-06.png";
	a.imgselect = "tx-chip-06.png";
	a.value = 500000;
	return a;
}

//
TaiXiuCoin1M::TaiXiuCoin1M(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag) {
}

CoinStruct TaiXiuCoin1M::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-07.png";
	a.imgselect = "tx-chip-07.png";
	a.value = 1000000;
	return a;
}

//
TaiXiuCoin5M::TaiXiuCoin5M(Button* item, const std::string& name, const std::string& type, const int& tag)
:TaiXiuCoin(item, name, type, tag){
}

CoinStruct TaiXiuCoin5M::GetValue()
{
	CoinStruct a;
	a.imgnormal = "tx-chip-08.png";
	a.imgselect = "tx-chip-08.png";
	a.value = 5000000;
	return a;
}

TaiXiuCoin *TaiXiuCoinFactory::CreateTaiXiuCoin(Button *item, const std::string &name, const std::string &type, const int &tag)
{
	if (0 == type.compare(BET_VALUE_1)){
		return new TaiXiuCoin1(item, name, type, tag);
	}
	else  if (0 == type.compare(BET_VALUE_100)){
		return new TaiXiuCoin100(item, name, type, tag);
	}
	else  if (0 == type.compare(BET_VALUE_500)){
		return new TaiXiuCoin500(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_1K)){
		return new TaiXiuCoin1K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_2K)){
		return new TaiXiuCoin2K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_5K)){
		return new TaiXiuCoin5K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_10K)){
		return new TaiXiuCoin10K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_50K)){
		return new TaiXiuCoin50K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_100K)){
		return new TaiXiuCoin100K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_500K)){
		return new TaiXiuCoin500K(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_1M)){
		return new TaiXiuCoin1M(item, name, type, tag);
	}
	else if (0 == type.compare(BET_VALUE_5M)){
		return new TaiXiuCoin5M(item, name, type, tag);
	}
	return NULL;
}
