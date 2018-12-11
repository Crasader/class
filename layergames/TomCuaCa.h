//
//  TomCuaCa.h
//  iCasino_v2
//
//  Created by Tuan on 5/20/14.
//
//

#ifndef __iCasino_v2__TomCuaCa__
#define __iCasino_v2__TomCuaCa__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "AllData.h"
#include "_Avatar_inTomCuaCa.h"
#include "FrameBet.h"
#include "_Number_inGame_.h"
#include "_Chat_inGame_.h"
#include "ImageDownloader.h"
#include "_Button_inGame_.h"
#include "LayerTimer.h"

//ket qua
#define _kqNai 100
#define _kqRuou 95.8 
#define _kqGa 91.75 
#define _kqCa 87.45
#define _kqCua 83.27
#define _kqTom 79.2

//button Tag
#define t_ready 10000
#define t_unReady 10001

using namespace cocos2d;
using namespace cocos2d::ui;
//using namespace Sfs2X;
#include "LayerBet_TomCuaCa.h"
class TomCuaCa : public Layer, public PlayerCallBack{
private:
	float _count;
	boost::shared_ptr<string> _id_me;
	string _list_user;
	vector<string> arrName;
	vector<int> arrMoney;
	vector<double> arrMoneyDouble;
	bool meIsSpector;
	//Các button trong game
	bool myReady;
	LayerTimer *layerTimer;
	void createTimer();
	string myName;

	Layer *uLayer;
	Button *btnReady;
	Button *btnUnReady;
	Button *btnTom;
	Button *btnCua;
	Button *btnCa;
	Button *btnGa;
	Button *btnRuou;
	Button *btnNai;
	ui::ScrollView *scroll1;
	ui::ScrollView *scroll2;
	ui::ScrollView *scroll3;
	LoadingBar *loading;
	AvatarInTomCuaCa* lAvatar;

//	LayerChatInGame *layerChat;

	
	int _time;
	int numberUser;
	int _countTimer; /// bien dem cho timer
// 	string kq1;
// 	string kq2;
// 	string kq3;
	boost::shared_ptr<string> kq;
	boost::shared_ptr<ISFSObject> param;
	void funCallBackLayerBet();
	Layer *LayerCuoc;
	vector<string> arrGameResult;
	vector<double> arrMoneyLayerBet;
	void createPool();
	vector<Button*> arrButton;
	vector<ProgressTimer*> arrTimer;
	void createArrayFrameResult();
	void funCallBackToPlayer();
	void showChat(string mess);
public:
	FrameBet* betTom;
	FrameBet* betCua;
	FrameBet* betCa;
	FrameBet* betGa;
	FrameBet* betRuou;
	FrameBet* betNai;
	LayerButtonInGame* lButton;
	Label *nameGame;
    string result;

	bool isStartedGame();

	TomCuaCa();
	~TomCuaCa();
	CREATE_FUNC(TomCuaCa);
	bool init();

	void funCallBack();
	void createBackgrounds();
	void createAvatars();
	void createButtons();
	void disableAllBet();

	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	virtual void OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	
	vector<string> TCCsplit(string &S,const char &str);
	void updateUser(string list);
	string find_ChuPhong(string listUser);

	void createLayerBet(float _money, int _tag, Button *pSender);
	void whenUserReady(string uid);
	void whenUserUnready(string uid);
	void whenGameStart();
	void resetGame();
	void whenResuiltGame(string rg);
	void whenGameEnd();
	int getPosUserByName(string uid,string _list_user);
	void playSound(string soundPath);
	void clickBtn(Ref* obj, Widget::TouchEventType type);
    bool isSpector;

	int convertResponseToInt(string inString);
	string convertResponseToString(int inInt);
	float convertResult(string rs);
	virtual void update(float dt);
	void clickBet(Button *pSender);
	void bet(int aid, double tienBet, double totalBet, double totalBetOfUser);
	void setTimer(float dt);
	void hienKetQua();
	void hienOketqua();
    void specMode();
	string convertMoney(int money);
	string convertMoneyFromDouble(double money);
	string convertMoneyFromDouble_Detail(double money);
	bool checkPlaying(string _list);
};
#endif /* defined(__iCasino_v2__TomCuaCa__) */
