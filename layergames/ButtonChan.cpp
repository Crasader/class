/*
 * ButtonChan.cpp
 *
 *  Created on: Nov 12, 2015
 *      Author: daua
 */

#include "ButtonChan.h"

ButtonChanNotification* ButtonChanNotification::iInstance = NULL;
ButtonChanNotification* ButtonChanNotification::GetInstance()
{
    if (iInstance == NULL) {
        iInstance = new ButtonChanNotification();
    }
    return iInstance;
}

void ButtonChanNotification::DestroyInstance(){
    delete iInstance;
    iInstance = NULL;
}

void ButtonChanNotification::AddObserver(ButtonChan* o) {
	this->observers.insert(o);
}

void ButtonChanNotification::RemoveObserver(ButtonChan* o) {
	this->observers.erase(o);
}

void ButtonChanNotification::Trigger(PlayingState state) {
    if(this->HasAnBao) return;
   // log("state: %d", state);
	set<ButtonChan*>::iterator itr;
	for (itr = observers.begin(); itr != observers.end(); itr++) {
		(*itr)->Notify(state);
	}
}

void ButtonChanNotification::ResetButton(){
   // log("reset button");
    set<ButtonChan*>::iterator itr;
    for (itr = observers.begin(); itr != observers.end(); itr++) {
        (*itr)->Reset();
    }
}

void ButtonChanNotification::DisableAllButton(){
   // log("disable all button");
    set<ButtonChan*>::iterator itr;
    for (itr = observers.begin(); itr != observers.end(); itr++) {
        (*itr)->Disable();
    }
}

void ButtonChanNotification::ShowChiu(bool show){
    if(this->HasAnBao) return;
    set<ButtonChan*>::iterator itr;
    for (itr = observers.begin(); itr != observers.end(); itr++) {
        (*itr)->ShowIfChiu(show);
    }
}

void ButtonChanNotification::RemoveAllObserver()
{
    this->observers.clear();
}

void ButtonChanNotification::ClientDuoiCard()
{
   // log("Client Tu Duoi");
    set<ButtonChan*>::iterator itr;
    for (itr = observers.begin(); itr != observers.end(); itr++) {
        if((*itr)->getTag() != CTAG_BTN_BAO_U){
            (*itr)->Disable();
        }
        else
        {
            (*itr)->Enable();
        }
    }
}

// Goi khi client vua an xong, hien thi button danh vs U.
void ButtonChanNotification::ClientDisCard()
{
    set<ButtonChan*>::iterator itr;
    for (itr = observers.begin(); itr != observers.end(); itr++) {
        if((*itr)->getTag() == CTAG_BTN_BAO_U || (*itr)->getTag() == CTAG_BTN_DISCARD){
            (*itr)->Enable();
        }
        else
        {
            (*itr)->Disable();
        }
    }
}

void ButtonChanNotification::SetAnBao(bool anbao){
    this->HasAnBao = anbao;
}

ButtonChan::ButtonChan() {
	this->layerButton = NULL;
	this->pNormalImg = "";
	this->pSelectedImg = "";
	this->pDisableImg = "";
	this->pTitle = "";
	this->Position = Vec2(0, 0);
	this->childType = 0;
    this->lbl = NULL;
}

ButtonChan::ButtonChan(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) {
	this->pNormalImg = pNormal;
	this->pSelectedImg = pSelect;
	this->pDisableImg = pDisable;
	this->pTitle = pTile;
	this->Position = pPos;
	this->childType = type;
	this->layerButton = layer;
    this->lbl = NULL;
    this->layerButton->AddObserver(this);
	this->init();
}

ButtonChan::~ButtonChan() {

}

bool ButtonChan::init() {
	if (!Button::init())
		return false;
	this->setScale9Enabled(false);
	this->setTouchEnabled(true);
	this->loadTextures(this->pNormalImg, this->pSelectedImg, this->pDisableImg,
			Widget::TextureResType::PLIST);

	this->setAnchorPoint(Vec2(0.5, 0));
	this->setPosition(this->Position);

    this->lbl = Label::createWithTTF(this->pTitle,kBoldFont,30);
    this->lbl->setAnchorPoint(Vec2(0.5, 0.5));
	this->lbl->setColor(Color3B::WHITE);
    this->lbl->setScale(1.8);
    this->lbl->setPosition(
			Vec2(this->getContentSize().width / 2,
					this->getContentSize().height / 2 + 3));
	//button Chiu
	if (1 == this->childType) {
        this->lbl->setPositionX(this->getContentSize().width / 2 - 4);
	}
	//Button Danh, Duoi
	if (2 == this->childType) {
        this->lbl->setPositionX(this->getContentSize().width / 2 + 4.0);
	}
    this->addChild(this->lbl);

	return true;
}

void ButtonChan::Enable() {
    if(this->getTag() == CTAG_BTN_DUOICARD || this->getTag() == CTAG_BTN_DISCARD){
        this->setLocalZOrder(99);
    }
    this->lbl->setColor(Color3B::WHITE);
	this->loadTextureNormal(this->pNormalImg, Widget::TextureResType::PLIST);
	this->setTouchEnabled(true);
	this->setVisible(true);
}

void ButtonChan::Disable() {
    if(this->getTag() == CTAG_BTN_DUOICARD || this->getTag() == CTAG_BTN_DISCARD){
        this->setLocalZOrder(98);
    }
    this->lbl->setColor(Color3B::GRAY);
	this->loadTextureNormal(this->pDisableImg, Widget::TextureResType::PLIST);
	this->setTouchEnabled(false);
	this->setVisible(true);
}

void ButtonChan::DisableAndHide(){
    this->lbl->setColor(Color3B::WHITE);
    this->loadTextureNormal(this->pDisableImg, Widget::TextureResType::PLIST);
    this->setTouchEnabled(false);
    this->setVisible(false);
}

void ButtonChan::Reset() {
    this->lbl->setColor(Color3B::WHITE);
	this->loadTextureNormal(this->pNormalImg, Widget::TextureResType::PLIST);
	this->setTouchEnabled(true);
	this->setVisible(false);
}

void ButtonChan::ShowIfChiu(bool show){
    if(!show) return;
    if(this->getTag() == CTAG_BTN_CHIUCARD){
        this->Enable();
    }
}

//! Button U
ButtonU::ButtonU(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonU::~ButtonU() {
}

void ButtonU::Notify(PlayingState state) {
	if (state == PlayingState::ANBAI || state == PlayingState::CHIUBAI) {
		this->Disable();
	} else {
		this->Enable();
	}
}

//! Button Chiu
ButtonChiu::ButtonChiu(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonChiu::~ButtonChiu() {
}

void ButtonChiu::Notify(PlayingState state) {
	if(state == PlayingState::DISCARD || state == PlayingState::DRAWCARD || state == PlayingState::TRACUA){
		this->Enable();
	} else {
		this->Disable();
	}
}

//! Button Eate
ButtonEate::ButtonEate(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonEate::~ButtonEate() {
}

void ButtonEate::Notify(PlayingState state) {
	if(state == PlayingState::DISCARD || state == PlayingState::DRAWCARD || state == PlayingState::TRACUA){
		this->Enable();
	}else{
		this->Disable();
	}
}

//! Button Take
ButtonTake::ButtonTake(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonTake::~ButtonTake() {
}

void ButtonTake::Notify(PlayingState state) {

	if(state == PlayingState::BEGIN || state == PlayingState::ANBAI || state == PlayingState::CHIUBAI){
		this->Enable();
        this->setLocalZOrder(100);
	} else {
        this->Disable();
        this->setLocalZOrder(98);
    }
}

void ButtonTake::Enable()
{
    ButtonChan::Enable();
    this->setLocalZOrder (100);
}

void ButtonTake::Disable()
{
    ButtonChan::Disable();
    this->setLocalZOrder (98);
}

//! Button Duoi
ButtonDuoi::ButtonDuoi(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonDuoi::~ButtonDuoi() {
}

void ButtonDuoi::Notify(PlayingState state) {
	if(state == PlayingState::DRAWCARD || state == PlayingState::TRACUA){
		this->Enable();
        this->setLocalZOrder(100);
	}else{
		this->Disable();
        this->setLocalZOrder(98);
    }
}

void ButtonDuoi::Enable()
{
    ButtonChan::Enable();
    this->setLocalZOrder (100);
}

void ButtonDuoi::Disable()
{
    ButtonChan::Disable();
    this->setLocalZOrder(98);
}

//! Button Boc
ButtonBoc::ButtonBoc(const std::string& pTile, const std::string& pNormal,
		const std::string& pSelect, const std::string& pDisable,
		const Vec2& pPos, const int& type, ButtonChanNotification* layer) :
		ButtonChan(pTile, pNormal, pSelect, pDisable, pPos, type, layer) {
}

ButtonBoc::~ButtonBoc() {
}

void ButtonBoc::Notify(PlayingState state) {
	if(state == PlayingState::DISCARD){
		this->Enable();
	}else{
		this->Disable();
	}
}

