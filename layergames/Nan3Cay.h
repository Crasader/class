//
//  _Card_inGame_.h
//  iCasino_v2
//
//  Created by DauTV on 6/9/14.
//
//

#ifndef __iCasino_v2___NanBaCay___
#define __iCasino_v2___NanBaCay___

#include <iostream>
#include "cocos2d.h"
#include "CardChan.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::extension;

class Nan3Cay : public Layer
{
private:
	Size sizeScreen;
	Size disTouchBegan;
	Point pointTouchBegan;

	float bottomCard;
	float leftCard;
	float rightCard;

	float widthCard;
	float heightCard;

	float scaleApp;
	float startLeft;
	float startTop;

	//Nan 3 Cay
	float dx, dy;
	int _index, _where;
	bool isTouched;

	//Bat dau nan 3 cay
	float deltaX;
	float deltaY;
	float sX;
	float sY;
	float xcu;
	float ycu;

	int cardIndex;

	//Touch move
	int cardIndex_under;
	bool flag_move;
	int flag_left, flag_right;
	int count_move;

	//Touch end
	int vt;

	vector<string> arrCardTypes;
	vector<Sprite*> CardOnHand;

	Ref* m_callback;
	SEL_CallFuncN m_callbackListener;
	cocos2d::EventListener* _touchListener;
    
    Size sizePos;

public:
	~Nan3Cay();
	Nan3Cay();
	bool init();
	CREATE_FUNC(Nan3Cay);

	void InitListCardHand(string& lsCards);

	Point& ConvertPoint(Point& pPoint);
	string& FindTypeCard(string& card);

	void MovePockerFinish(Sprite *pCard, const int& pos);
	void CloseLayerNanBai();

	void SetCallbackFunc(Ref* target, SEL_CallFuncN callfun);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent); 
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	//virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
};

#endif