#ifndef LayerPopupAlert_h__
#define LayerPopupAlert_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "CommonChess.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerPopupAlert : public cocos2d::Layer
{
public:
	virtual bool init() override;
	LayerPopupAlert();
	~LayerPopupAlert();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerPopupAlert);
	void setContentMessage(std::string _title,std::string _mess);
	//void setCallBackConfirm(std::function<void(int)> _listener);

	typedef std::function<void(LayerPopupAlert*, int)> popupAlertCallback;
	void setCallBackConfirm(const popupAlertCallback& callback);
	void runTimer(const float& timeOut = DEFAULT_TIME_RESPONSE);
	
	virtual void onEnter();
	virtual void onExit();
	void showOnlyButtonAccept();
	CC_SYNTHESIZE(int, tagCb, TagCallBack);
	//update 14/7
	void setTitleCloseButton(string title);
	void setTitleConfirmButton(string title);
	void setCallBackClose(const popupAlertCallback& callback);
private:
	Text* lblContent, *lblTitle;
	Text* lblTimer;//text bitmap tren nut cancel
	bool isOnlyButton;

	Button* btnClose, *btnConfirm;
	int timeBuff = DEFAULT_REMOVE_SELF;

	void onButtonConfirm(Ref* pSender);
	void onButtonClose(Ref* pSender);
	popupAlertCallback _callback;
	void startTimer(float dt);
	//update 14/7
	popupAlertCallback _callbackClose;
};

class LayerPopupRoomFind :public Layer{

public:
	LayerPopupRoomFind();
	~LayerPopupRoomFind();

	virtual bool init();
	CREATE_FUNC(LayerPopupRoomFind);
private:
	TextField* TextField_1;

	void onButtonConfirm(Ref* pSender);
	void onButtonClose(Ref* pSender);
};

#endif // LayerPopupAlert_h__
















