//
//  LayerDatCuocBaCay.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 3/11/16.
//
//

#ifndef LayerDatCuocBaCay_hpp
#define LayerDatCuocBaCay_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;

class LayerDatCuocBaCay : public cocos2d::Layer
{
public:
    LayerDatCuocBaCay();
    ~LayerDatCuocBaCay();
    
    virtual bool init();
    CREATE_FUNC(LayerDatCuocBaCay);

	virtual void onEnter() override;
	virtual void onExit() override;

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void setInfoBet(double minbet);
	void setInfoTest(double minbet, double maxbet);
	void setCallBack(const std::function<void()> &_listener);
	Button* getButtonConfirm();
	double getValueBet();
	void resetSlider();
private:
	double maxValue = 0;
	double minValue = 0;
	//double currBetValue;
	double valueBet = 0;
	double userAmf;
	int gameId;

	Button* btnConfirm;
	ui::Text* labelMinValue;
	ui::Text* labelCurrBet;
	Sprite* imgMoney;
	Slider* slider;
	std::function<void()> listener;
	ui::Text* lblPoint75;
	ui::Text* lblPoint50;
	ui::Text* lblPoint25;

	void sliderEvent(Ref *pSender, Slider::EventType type);
	void onButtonClick(Ref* pSender);
	float formatValue(float value);
	void showToast(std::string msg);
	void setPercentPoint(float dis);
	float money_round(float num);
};


#endif /* LayerDatCuocBaCay_hpp */
