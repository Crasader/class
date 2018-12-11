/*
 * ButtonChan.h
 *
 *  Created on: Nov 12, 2015
 *      Author: daua
 */

#ifndef BUTTONCHAN_H_
#define BUTTONCHAN_H_

#include "cocos2d.h"
#include <set>
#include "cocos-ext.h"
#include "ChanUtils.h"
#include "_Button_inGame_.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

class ButtonChan;
class ButtonChanNotification{
private:
	set<ButtonChan*> observers;
    bool HasAnBao;
    ButtonChanNotification(){
        this->HasAnBao = false;
    }

public:
    static ButtonChanNotification* iInstance;
    static ButtonChanNotification* GetInstance();
    static void DestroyInstance();

    ~ButtonChanNotification(){
        this->observers.clear();
    }
    void SetAnBao(bool anbao);
	void AddObserver(ButtonChan* o);
	void RemoveObserver(ButtonChan* o);
	void Trigger(PlayingState state);
    void ResetButton();
    void DisableAllButton();
    void ShowChiu(bool show = true);
    void RemoveAllObserver();

    //Duoi truoc:
    void ClientDuoiCard();
    void ClientDisCard();
};

class ButtonChan : public Button{
protected :
	ButtonChanNotification* layerButton;
	std::string pNormalImg;
	std::string pSelectedImg;
	std::string pDisableImg;
	std::string pTitle;
	Vec2 Position;
	int childType;
    Label* lbl;
public:
	ButtonChan();
	ButtonChan(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	virtual ~ButtonChan();
	bool init();
	//
	// Hien thi va bam duoc vao button
	void Enable();

	//Disable nhung van hien thi, chi ko touch duoc vao button
	void Disable();

    // Disable va hide luon
    void DisableAndHide();

	void Reset();

	virtual void Notify(PlayingState state) = 0;

    void ShowIfChiu(bool show);
};

//! Button U
class ButtonU : public ButtonChan{
private:
public:
	ButtonU(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonU();
	void Notify(PlayingState state);
};

//! Button Chiu
class ButtonChiu : public ButtonChan{
private:
public:
	ButtonChiu(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonChiu();
	void Notify(PlayingState state);
};

//! Button An
class ButtonEate : public ButtonChan{
private:
public:
	ButtonEate(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonEate();
	void Notify(PlayingState state);
};


//! Button Danh
class ButtonTake : public ButtonChan{
private:
public:
	ButtonTake(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonTake();
	void Notify(PlayingState state);
    void Enable();
    void Disable();
};


//! Button Duoi
class ButtonDuoi : public ButtonChan{
private:
public:
	ButtonDuoi(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonDuoi();
	void Notify(PlayingState state);

    void Enable();
    void Disable();
};

//! Button Boc
class ButtonBoc : public ButtonChan{
private:
public:
	ButtonBoc(const std::string& pTile, const std::string& pNormal, const std::string& pSelect, const std::string& pDisable, const Vec2& pPos, const int& type, ButtonChanNotification* layer);
	~ButtonBoc();
	void Notify(PlayingState state);
};


#endif /* BUTTONCHAN_H_ */
