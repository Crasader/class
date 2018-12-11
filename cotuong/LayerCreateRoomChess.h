#ifndef LayerCreateRoom_h__
#define LayerCreateRoom_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos/ui/CocosGUI.h"
#include "../GameServer.h"
#include "LayerPopupChapQuan.h"
#include "LayerStep.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

class LayerCreateRoomChess : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	virtual bool init() override;
	LayerCreateRoomChess();
	~LayerCreateRoomChess();

	// implement the "static create()" method manually
	CREATE_FUNC(LayerCreateRoomChess);

	void setPopupInGame(bool _isInGame);//hien trong game se an nut choi nhanh va tao phong
	void setListUser(string listUser);//check kiem tra chu phong
	//update 11/7
	void setConfigFromString(string configStr);//hien thi giao dien theo config san co
	//update 25/7
	void disableSetting();
protected:
	//smartfox event
	//void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
private:
	Layout *pnlContent;
	CheckBox *chbChaphoa, *chbAllow;
	CheckBox* chbView;
	//Slider *sldMoney;
	Text *lblTime, *lblQuanChap;
	//TextBMFont* lblTienCuoc;
	//Sprite* spMoney;
	Button* btnPlay;
	Button* btnCreate;
	Button* btnCreateRoom;
	string configChapQuan;
	LayerStep* layerStepTime, *layerStepChapTien;

	int betValue = 0;
	int maxValue;
	int minValue;
	bool isInGame = false;

	void onButtonInvitePlay(Ref* pSender);
	void onButtonMinusTime(Ref* pSender);
	void onButtonAddTime(Ref* pSender);
	void onButtonChapQuan(Ref* pSender);
	void onButtonCreateRoom(Ref* pSender);
	void onButtonQuickPlay(Ref* pSender);
	void onButtonHelpChapTien(Ref* pSender);
	void onButtonHelpChapHoa(Ref* pSender);
	void sliderEvent(Ref *pSender, Slider::EventType type);
	void onButtonQuit(Ref* pSender);

	void setValueBetFromSlider(int percent);
	float formatValue(float value);

	void initFirst();//ham set gia tri slide, kiem tra xem co phai chu phong thi disable mot vai option

	void loadRoomVariables();
	void showPopupInfo(string title, string message);
	void callBackFromPopup(LayerPopupChapQuan* sender, string _config, string _display);
	string generateConfigString(int betValue, int totalTime, int turnTime, string chapquan, 
		int chaptien, string cuocquan, int chapthoigian, int chaphoa, int allowsetting, int allowview);
	///ui chon tien cuoc moi
	Layout* pnlMain;
	Text* lblBetValue, *lblValueTab1, *lblValueTab2, *lblValueTab3;
	Button* btnTab1, *btnTab2, *btnTab3, *btnClear;
	void initUICuoc();
	vector<int> listBetConfig;
	void onButtonTabBet(Ref* pSender);
	void onButtonClearBet(Ref* pSender);
	double getMinBetByGame(int gameID, double amf);
	//update 10/7
	LayerPopupChapQuan *layerChapQuan;
	//update 17/7
	string enemyUser = "";
	int numUser = 0;

public:
	//update 25/7
	CC_SYNTHESIZE(bool, isCanSetting, CanSetting);
};

#endif 



