//
//  _Card_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/19/14.
//
//

#ifndef __iCasino_v2___Card_inGame___
#define __iCasino_v2___Card_inGame___

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "AllData.h"
#include "SimpleAudioEngine.h"

#include "_Card_.h"
#include "CardInfo.h"
#include "LogicTienLenMN.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LayerCardInGame : public Layer
{
public:
	LayerCardInGame();
	~LayerCardInGame();
	bool init();
	CREATE_FUNC(LayerCardInGame);

	// touches event
	Point convertPoint(Point point);
	float   getDisPoint(Point p1, Point p2);
	//     virtual void TouchesBegan(Set *pTouches, Event *pEvent);
	//     virtual void TouchesMoved(Set *pTouches, Event *pEvent);
	//     virtual void TouchesEnded(Set *pTouches, Event *pEvent);

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
	//virtual void registerWithTouchDispatcher();
	void initGame();
	void resetGame();
	void playSound(int type);

	//
	void initCardByPhom();
	void initCardByTienLen();

	// actions card
	// chia bai
	void actionDealCard(vector<int> arrCardID, vector<int> arrCardEat = vector<int>());
	void actionDealCardTL(vector<int> arrCardID, vector<int> arrCardEat = vector<int>());

	// xep bai
	void actionSortCard(vector<int> arrCardID);
	void actionSortCardByTienLen(); // tam thoi
	void actionSortCardPhom();
	void delayAllowSortCard(float dt);

	// danh bai
	void actionHitCard(int posUser, int pId);
	void actionHitCardByTienLen(int posUser, vector<int> arrID);

	// boc bai
	void actionGiveCard(int posUser, int pId);
	void callbackGiveCard(Node *pSender);

	// an bai
	void actionEatCard(int fromPosUser, int toPosUser, int pId);
	//hien thi cac quan bai da an sau khi vao lai
	void showEatCard(int posUser, int pId);

	// chuyen bai
	void actionTransferedCard(int fromPosUser, int toPosUser, int pId);

	// gui bai
	void actionPushCard(int fromPosUser, int toPosUser, int pId);

	// gợi ý hạ phỏm
	void actionRecommendCard();
	//gợi ý gửi phỏm
	void actionRecommendPushCard(string _listCard);

	// hạ phỏm
	void actionHaPhom(vector<int> arrID, vector<int> arrIDPhom);
	void actionHaPhomByPos(int pos, vector<int> arrID, vector<int> arrIDPhom);
	void actionHaPhomClient(string listCardID);

	// ----

	// phom
	vector<string> split(string &S, const char &str);
	vector<int> getIDFromString(string pListCard);
	vector<int> getIDFromString_ByPos(string pListCard, int pos);
	vector<int> getIDFromString_Last(string pListCard);
	vector<int> getIDFromString_TienLen(string pList);
	int getCountCardOnHand();
	int getCountCardTaken();

	// event-server
	void eventListcardNTF(int posUser, string lc); // nhận ds các ID, nhiệm vụ: chia bài, xếp bài,

	void event_EXT_SRVNTF_RECOMMENDED_CARDSETS(string lc);

	void refreshCardOnHand();
	void refreshCardOnHand(bool isRefreshTop);
	void refreshCardOnHand(float delay);
	void callbackRefreshCardOnHand();
	void setCardClick(Card* card);
	bool isTouchedCard_Tail(Card *card, Point tap);
	bool isTouchedCard_notTail(Card *card, Point tap);
	string getURL_byID(int pID);
	string getURL_byID_TienLen(int pID);

	vector<int> getIdCardByClicked();

	// hạ phỏm
	// trả về 1 mảng chứa các listID: [0]:="id:id:id"; [1]:="id:id:id"
	vector<string> getID_HaPhom();

	Card* getCardByID(int pID);

	void removeCardOnHandByID(int pID);
	void removeCardByID(int pID);

	void setIdCardCanEat(int id);
	int getIdCardCanEat();

	void setGameStarted(bool isStarted);
	bool getGameStarted();

	void setAllowSortCard(bool isAllow);
	bool getAllowSortCard();

	// Phom
	Point getPointCardInCenter();
	Point getStartPositionCardUserLeft_Hit();
	Point getStartPositionCardUserRight_Hit();
	Point getStartPositionCardUserTop_Hit();
	Point getStartPositionCardUserBot_Hit();
	Point getStartPositionCardByPos(int pos);
	// TienLen
	Point getStartPositionCardUserLeft_Hit_TienLen();
	Point getStartPositionCardUserRight_Hit_TienLen();
	Point getStartPositionCardUserTop_Hit_TienLen();
	Point getStartPositionCardUserBot_Hit_TienLen();
	Point getStartPositionCardByPos_TienLen(int pos);
	// số quân bài còn lại trên tay
	void setCountCardByPos(int pos, int count);

	void showCardOnHandAll();
	void showCardOnHandByPos_Arr(int kUser, vector<int> arrID);
	void showCardOnHandByPos_List(int kUser, string lc);
	void hideCardByArr(vector<int> arrID);
	void setFunCallBack(Ref *target, SEL_CallFuncO listener);
	void showLastCard(string lstCard);
	void hideLastCard();
	bool isRunningAction();

	bool checkCardIsRunning(int cardID);
	bool checkCardIsRunning(string listCard);
	void actionHitCardRollBack(int cardID);// day vao mang arrCard on Hand va xoa quan bai tren ban
	void removeCardByArray(int cardID, vector<int> arrIDCard);//remove card cho tien len
	void actionHitCardTLRollBack(vector<int> arrIDCardRollBack, bool isRefresh);//rollback cho tien len
	vector<int> getListPhomByListID(string listID);
	int getCountCardOfMe();//ham lay so quan bai co tren ban cua minh - danh cho phom

private:
	Size sizeAdd;
	Size winSize;
	cocos2d::EventListener* _touchListener;
	bool mTouchFlag;
	SEL_CallFuncO mCallBackListener;
	Ref *mCallBack;
	Node *paramCallBack;
	// lưu các ID-Card để cuối ván hiển thị
	vector<int> arrIDCardOnHandLeft;
	vector<int> arrIDCardOnHandRight;
	vector<int> arrIDCardOnHandTop;
	vector<int> arrIDCardOnHandBot;

	// lưu các ID-Card đang có trên bàn (để hết ván remove khỏi bàn)
	vector<int> arrIDCardOnTable;
	///MINHDV
	vector<Card*> arrHitCardOnTable;// mang cac quan bai danh ra tren ban danh cho tien len
	vector<int> arrHitCardMe;//mang cac quan bai danh ra tren ban danh cho phom
	bool checkCardWhenSort(vector<int> arrID, vector<int> arrIDTable);
	/// ham kiem tra xem quan danh ra co tren ban hay khong, tranh truong hop mang lag danh ra roi nhung sap xep van tra ve


	float startLeft;
	float startTop;
	float scaleApp;
	float disCards;
	float topCard;

	Size sizeScreen;
	Size disTouchBegan;
	Size sizeCard;
	Point pointTouchBegan;

	// [Can changes]
	// count
	int countCardMe_Take;
	int countCardLeft_Take;
	int countCardRight_Take;
	int countCardTop_Take;
	int countCardBot_Take;
	//
	int countCardMe_Eat;
	int countCardLeft_Eat;
	int countCardRight_Eat;
	int countCardTop_Eat;
	int countCardBot_Eat;

	int countCardMe_Phom;
	int countCardLeft_Phom;
	int countCardRight_Phom;
	int countCardTop_Phom;
	int countCardBot_Phom;

	int countMe_Phom;

	// [Const]
	int ZORDER_PHOM;
	int ZORDER_TAKE;
	int ZORDER_HAND;

	//     CCArray *arrAllCard;
	//     CCArray *arrCardOnHand;
	vector<Card*> arrAllCard;
	vector<Card*> arrCardOnHand;

	int idCardCanEat;
	int countTouched;
	int indexCardCurrent;
	int indexCardTarget;

	bool isClickCard;    // click or move card - default: click card
	bool gameStarted;
	bool allowSortCard;  // kiểm soát khi người dùng ấn "xếp bài"
	bool requestRecommendHaPhom;  // =true thì sẽ gợi ý hạ phỏm

	string lcRecommendHaPhom;

	// số bài trên tay
	Sprite *cardBackLeft, *cardBackRight, *cardBackTop, *cardBackBot;
	CC_SYNTHESIZE(bool, isResume, Resume);//bien kiem tra xem co phai vao lai choi 

	float ANIMATION_DURATION;
	float REFRESH_DURATION;
	float DELAY_DURATION;
	float SCALE_RATIO;
	EventListenerTouchOneByOne *listener_forcard;
	void refreshCardOnHandWhenMove(bool isRefreshTop);
	void actionShowLastCardBot(int pID);
	void hideArrowCardCanEat();
	///MINH DV cac bien khoi tao vi tri ban dau cua cac user, co 3 trang thai la danh, ha, an
	Vec2 startHitBot;
	Vec2 startHandoffBot;
	Vec2 startEatBot;
	Vec2 startHandoffMe;

	Vec2 startHitRight;
	Vec2 startHandoffRight;
	Vec2 startEatRight;

	Vec2 startHitTop;
	Vec2 startHandoffTop;
	Vec2 startEatTop;

	Vec2 startHitLeft;
	Vec2 startHandoffLeft;
	Vec2 startEatLeft;

	/************************************************************************/
	/* danh cho goi y tien len va sam                                                                     */
	/************************************************************************/
	float DEAL_DURATION;
	CC_SYNTHESIZE(bool, _my, MyTurn);//bien kiem tra xem co phai den luot minh danh hay khong
	CC_SYNTHESIZE(bool, _recommend, CanRecommend);//bien kiem tra xem bai tren tay co chan duoc bai duoi ban khong
	LogicTienLenMN *logicTL;
	
	void setPreviousCard(string _list);
	void setCardRecommend(vector<CardInfo*> cards);
	int lastLengthChoose = 0;//bien de kiem tra khi bo va chon quan bai, neu tu 0 tang dan thi goi y, con tu 3 tro len ma giam thi ko goi y
	void flipAction2();
	void callBackDealCard(Node* pSender);
	Card* getCardOnHandByID(int pID);
public:
	//gợi ý đánh bài tiến lên
	void actionRecommendHitCard();
	//new dealcard animation
	void actionDealCardPro(int countCard);
	void setLastLengthChoose(int _length);
    void showArrowInCard(int pID);
    void removeArrow();
    bool isRecommend = false;
	string getListCardOnHand();//ham lay list cac quan bai tren tay
};

#endif /* defined(__iCasino_v2___Card_inGame___) */
