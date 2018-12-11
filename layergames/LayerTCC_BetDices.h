#ifndef LayerTCC_BetDices_h__
#define LayerTCC_BetDices_h__

#include "cocos2d.h"
#include "GameServer.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;

class LayerTCCBet :public cocos2d::Layer{
public:
	LayerTCCBet(string _url, int _tag);
	~LayerTCCBet();
	static LayerTCCBet* create(string _url, int _tag);
	virtual bool init();

	void endGame();
	void resetGame();
	void startGame();
	void showResult();
	void setCurrentBet(double userBet, double totalBet);

private:
	ImageView *Image_2;
	Button *btnBetCoin;
	Sprite* Sprite_2, *imgX2, *imgX3, *imgStroke;
	Text* lblTotalBet, *lblUserBet;
	string _url;
	int _tag;
	bool isInit = false;//danh dau cho viec khoi tao lan dau tien, tranh viec bi an label user

	cocos2d::ProgressTimer *timer;
	int count = 0;
	double MinBetValue = 0;
	double currUserBet = 0;
	double currTotalBet = 0;
	
	Vec2 startPos;

	void onButtonBet(Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void showLabelUser(bool isShow);
	void showX2(bool isShow);
	void showX3(bool isShow);
	void showImgStroke(bool isShow);


	void setClientBet(double userBet, double totalBet, string bettype);
	void setBetUser(double betvl);
	void setBetTotal(double betvl);
	void getMinMoney();

};

class LayerBetDice : public cocos2d::Layer,
	public PlayerCallBack
{
public:
	LayerBetDice();
	~LayerBetDice();
	virtual bool init();
	CREATE_FUNC(LayerBetDice);

	void initBetInfo(string _list);
	void setBetInfoClient(double userbet, double totalbet, string bettype);
	void setBetInfo(string myName, string uid, string userbet, string totalbet, int betroom);
	void rollBackBet(int betroom, string bettype, int betvl);
	void loadBetInfo();

	void startGame();
	void resetGame();
	void endGame();
	void setResult(string _string);
	CC_SYNTHESIZE(double, amfUser, AmfUser);
	//MINHDV for new version 26/9
	bool isOutLimitBet(string bettype);
	void pushFlagBet(string bettype);
    void setTypeGame(int type);

private:
	struct DiceInfo{
		int betroom;
		string bettype;
		double userbet;
		double totalbet;
		DiceInfo(int _betroom, string _bettype, double _userbet, double _totalbet) :
			betroom(_betroom), bettype(_bettype), userbet(_userbet), totalbet(_totalbet){};
	};
	//double amfUser = 0;
	//vector<DiceInfo> listBetInfo;
	// 	long betvalue = 0;
	// 	int betroom = -1;

	CC_SYNTHESIZE(vector<DiceInfo>, listBetInfo, ListBetInfo);
	CC_SYNTHESIZE(int, betvale, BetValue);
    CC_SYNTHESIZE(int, typeMoney, TypeMoney);
	CC_SYNTHESIZE(int, betroom, BetRoom);

	void showToast(string mess);
	LayerTCCBet* getLayerTCCBetByName(string _name);
	void loadAmfUser();
	//MINHDV for new version 26/9
	struct BetCount{
		string bettype;
		int flag;//0 la chua co dat cuoc, 1 la da dat cuoc
		BetCount(string _type, int _flag) :
			bettype(_type), flag(_flag){};
	};
	vector<BetCount> betCounts;
};
#endif // LayerTCC_BetDices_h__
