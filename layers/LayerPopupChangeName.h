#ifndef __LayerPopupChangeName_h__
#define __LayerPopupChangeName_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;
using namespace std;

class LayerPopupChangeName : public Layer,
	public PlayerCallBack
{
public:
	enum{
		FLAG_CHANGE_NAME,
		FLAG_CHANGE_PHONE
	};
	LayerPopupChangeName();
	virtual ~LayerPopupChangeName();

	void onButtonOK(Ref* pSender);
    void onPopupCallBack(Ref* pSender);
    void getPartnerInfo();
	virtual bool init();
	CREATE_FUNC(LayerPopupChangeName);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	//MINH DV Change
	CC_SYNTHESIZE(int, _flag, FlagPopup);//biến để phân biệt popup thay đổi tên và popup thay số điện thoại, default là thay đổi tên
	void setContentPopUp(string title, string mess, string strhint);
private:
	Text* lblError;
	TextField* txtNameDisplay;
	Button* btnOK;
	Button* btnClose;
    string partnerID;
    string srcID;
	//MINHDV - change
	Text* lblTitle;
	Text* lblHeader;

	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
};


class LayerPopupChangeMobile : public Layer,
	public PlayerCallBack
{
public:
	enum{
		FLAG_CHANGE_NAME,
		FLAG_CHANGE_PHONE
	};
	LayerPopupChangeMobile();
	virtual ~LayerPopupChangeMobile();

	void onButtonOK(Ref* pSender);
	virtual bool init();
	CREATE_FUNC(LayerPopupChangeMobile);
	//MINH DV Change
	CC_SYNTHESIZE(int, _flag, FlagPopup);//biến để phân biệt popup thay đổi tên và popup thay số điện thoại, default là thay đổi tên
	void setContentPopUp(string title, string mess, string strhint);
private:
	Text* lblError;
	TextField* txtNameDisplay;
	Button* btnOK;
	Button* btnClose;
	string partnerID;
	string srcID;
	//MINHDV - change
	Text* lblTitle;
	Text* lblHeader;

	void onButtonClose(Ref* pSender, ui::Widget::TouchEventType type);
};
#endif // __LayerPopupChangeName_h__
