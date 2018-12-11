//
//  BangCuocMauBinh.cpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/2/17.
//
//

#include "BangCuocMauBinh.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MauBinhLayerXepBai.h"
BangCuocMauBinh::BangCuocMauBinh()
{
}


BangCuocMauBinh::~BangCuocMauBinh()
{

}

bool BangCuocMauBinh::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;

	this->pnlBg = CSLoader::getInstance()->createNode("BangCuocMauBinh.csb");
	ui::Helper::doLayout(this->pnlBg);

	this->addChild(this->pnlBg);

	this->imgBangCuoc = static_cast<ImageView*>(this->pnlBg->getChildByName("imgBangCuoc"));
	auto txtcuoc1 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc1"));
    txtcuoc1->setString(dataManager.GetSysString(649));
	this->lstCuoc.push_back(txtcuoc1);
	auto txtcuoc2 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc2"));
    txtcuoc2->setString(dataManager.GetSysString(586));
	this->lstCuoc.push_back(txtcuoc2);
	auto txtcuoc3 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc3"));
    txtcuoc3->setString(dataManager.GetSysString(674));
	this->lstCuoc.push_back(txtcuoc3);
	auto txtcuoc4 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc4"));
    txtcuoc4->setString(dataManager.GetSysString(647));
	this->lstCuoc.push_back(txtcuoc4);
	auto txtcuoc5 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc5"));
    txtcuoc5->setString(dataManager.GetSysString(648));
	this->lstCuoc.push_back(txtcuoc5);
	auto txtcuoc6 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc6"));
    txtcuoc6->setString(dataManager.GetSysString(675));
	this->lstCuoc.push_back(txtcuoc6);
	auto txtcuoc7 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc7"));
    txtcuoc7->setString(dataManager.GetSysString(557));
	this->lstCuoc.push_back(txtcuoc7);
	auto txtcuoc8 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc8"));
    txtcuoc8->setString(dataManager.GetSysString(676));
	this->lstCuoc.push_back(txtcuoc8);
	auto txtcuoc9 = static_cast<Text*>(this->imgBangCuoc->getChildByName("txtcuoc9"));
    txtcuoc9->setString(dataManager.GetSysString(677));
	this->lstCuoc.push_back(txtcuoc9);


	this->btnBai = static_cast<ImageView*>(this->pnlBg->getChildByName("btnMucCuoc"));
	this->btnBai->addClickEventListener(CC_CALLBACK_1(BangCuocMauBinh::onBtnMucCuoc, this));

	return true;
}
void BangCuocMauBinh::showBoBai(){
	this->isShowBoBai = true;
}
void BangCuocMauBinh::hideBangCuoc(){
	this->imgBangCuoc->setVisible(false);
	this->btnBai->setVisible(true);
}
void BangCuocMauBinh::reloadValueCuoc(int cuoc1, int cuoc2, int cuoc3){
	for (auto i = 0; i < this->lstCuoc.size(); i++){
		this->lstCuoc[i]->setColor(Color3B::GRAY);

	}
	if (cuoc1 < 0)
		return;




	this->lstCuoc[this->loadSapXepCuoc(cuoc3)]->setColor(Color3B::WHITE);

	this->lstCuoc[this->loadSapXepCuoc(cuoc2)]->setColor(Color3B::GREEN);

	this->lstCuoc[this->loadSapXepCuoc(cuoc1)]->setColor(Color3B::YELLOW);
}
int BangCuocMauBinh::loadSapXepCuoc(int cuoc){
	switch (cuoc){
	case MB_MAU_THAU:
		return 0;
	case MB_DOI:
		return 1;
	case MB_SAM_CO:
		return 3;
	case MB_SANH:
		return 4;
	case MB_THUNG:
		return 5;
	case MB_THU:
		return 2;
	case MB_CU_LU:
		return 6;
	case MB_TU_QUY:
		return 7;
	case MB_THUNG_PHA_SANH:
		return 8;
	default:
		return 9;
	}

}
void BangCuocMauBinh::onBtnMucCuoc(Ref* pSender){
	if (this->imgBangCuoc->isVisible()) {
		this->imgBangCuoc->setVisible(false);
		//this->btnBai->setVisible(false);
	}
	else {
		//         if (this->isShowBoBai)
		//             this->btnBai->setVisible(true);

		this->imgBangCuoc->setVisible(true);
	}
}
