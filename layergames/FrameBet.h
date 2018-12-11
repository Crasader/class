//
//  FrameBet.h
//  iCasinov2
//
//  Created by DauA on 5/26/14.
//
//

#ifndef __iCasinov2__FrameBet__
#define __iCasinov2__FrameBet__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "mUtils.h"
#include "Requests/ExtensionRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

class FrameBet : public Layer
{
private:
	Sprite *iconXu;
	ui::ImageView *frame;
	Label *money;

public:
	FrameBet();
	~FrameBet();

	bool init();
	CREATE_FUNC(FrameBet);

	void SetFrameSize(const float& toSize);
	void SetValueBet(const std::string& value);
	float GetKc_Width() const;
};

class LayerBien : public Layer, public PlayerCallBack{
private:
	Button *btnBienx1, *btnBienx2;
	double money_bet;
	std::string myName, lstUser;
	vector<std::string> arrList;

public:
	LayerBien();
	~LayerBien();
	bool init();
	bool init(double _money, std::string _lstUser);
	static LayerBien* create();
	static LayerBien* create(double _money, std::string _lstuser);

	void processBienx1Touched(Ref *sender, ui::Widget::TouchEventType type);
	void processBienx2Touched(Ref *sender, ui::Widget::TouchEventType type);
	void buttonBienx1LoadTexture();
	void buttonBienx2LoadTexture();
	void resetBien();
	void setArrList(std::string _list);
};

#endif /* defined(__iCasinov2__FrameBet__) */
