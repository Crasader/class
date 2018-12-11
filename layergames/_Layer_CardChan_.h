
#ifndef __iCasinov2__LayerCardChan__
#define __iCasinov2__LayerCardChan__

#include "cocos2d.h"
#include <iostream>
#include "mUtils.h"

#include "CardChan.h"
#include "LayerChanToast.h"
#include "ChanUtils.h"
#include "PlayerChan.h"

using namespace std;
USING_NS_CC;

class _Layer_CardChan_ : public Layer
{
private:

	float BeginX;
	float BeginY;

    PlayerChan* PlayerMe;
    PlayerChan* PlayerLeft;
    PlayerChan* PlayerTop;
    PlayerChan* PlayerRight;
    PlayerChan* PlayerBottom;

    std::string ListCardsNoc;

	Layer *LayerAllCards;
	Layer *LayerCardsOnHandWinner;
	Layer *LayerCardsInNoc;

	Button *DiaNoc;

    Label *LblCountNoc;

    LayerChanToast *Toast;

	float Card_Action_Duration;
	// Điểm bắt đầu và kết thúc touch
    Vec2 startPosTouched;

    vector<CardChan*> arrCardsResult;
	vector<CardChan*> arrCardsNoc;

	//Drag Drop
	Size sizeScreen;
	Size disTouchBegan;
	Vec2 oldPos;

	float scaleApp;
	float startLeft;
	float startTop;

    bool IsDragDrop;
	bool FlagCallback;

	Ref* m_callback;
	SEL_CallFuncO m_callbackListener;

    Ref* m_callback2;
    SEL_CallFuncO m_callbackListener2;

    CardOrg LastCardObj;

    vector<InfoCardU> arrCardU;
    bool hasUSucsses;

    bool FlagAnBao;

public:
	_Layer_CardChan_();
	~_Layer_CardChan_();

	bool init();
    CREATE_FUNC(_Layer_CardChan_)

    void SetLayerChanToast(LayerChanToast* toast);

    void CreateOrRestoreListCards(const int& pos, string& listCards);

	//
	void CreateAllCards();
	void ResetAllCards();

    void SetIsSpect(std::map<std::string, int> mapPlayers);
    void SetMyListCards(string& listCards);

	void CardResultMove(Ref *pSender, ui::Widget::TouchEventType pType);

    //
    void RemoveCardResult();
    void SetPlayerListCards(const int& pos, string& lstCards);

    void AddCard_toCuaTri(Ref* sender, int data);

	void SetCountNoc(const int& count);

	void TakeCards(const CardOrg& card);
	void Action_BocNoc(const CardOrg& card);
	void Action_AnCuaTren(const CardOrg& card);
	void Action_AnCuaTri(const CardOrg& card);
	void Action_ChiuBai(const CardOrg& card);
    void Action_TraCua(const CardOrg& card);

	// Đánh bài
    void ActionDiscards(const CardOrg& card);

    void Action_ChuyenBai(const CardOrg& card);

	void Action_An_U(const CardOrg& card);

	void ScaleCardsHand_WhenU();
    void MoveCardChi_WhenU();
    void DisplayListCardsWinner(const string& uid, string& listCards);
    void ShowLaBaiU(const string& uid);

	void SetListCardsNoc(string& _list);
	void DisplayListCard_Noc(vector<CardChan*>& P);

    void CardNoc_Touch(Ref *pSender, ui::Widget::TouchEventType pType);

	void ActionDisCards();
    int  ActionChiuCard();
    void ActionTakeCard();
	void ActionDuoiCard();

	void OnXemNocClick(Ref *pSender, ui::Widget::TouchEventType pType);

    void PlaySounds(const string& url);

	int GetCountCardsMe();

    void ChiuSuccess(const int& rescode);
    void TakeSuccess(const int& rc);
    void DisSuccess(const int& rescode);

	// touches event
	Vec2& ConvertPoint(Vec2& point);

    void SetCardCanChiu(std::string& ls);

	//
	void SetCallBackListCards(Ref* target, SEL_CallFuncO callfun);
    void SetCallBackDisCards(Ref* target, SEL_CallFuncO callfun);
	void DeleteListCards(vector<CardChan*>& listCards);

    void StartTurn();
    void FinishTurn();

    PlayerChan* GetPlayerChan(const int& pos);
    PlayerChan *GetPlayerChanByAI(const string& aI);

    bool checkIsRunningCard(const int& pos, const int& cardnu, const int& cardsu);
    bool CheckCardRunning(const int& pos);

    bool checkU();

    void SetInfoCardU(std::string& uid, std::string& card);

    void DisplayButtonViewNoc();

    void RemoveCardOnHandWinner();
    void SetFlagAnBao(bool flag);
};

#endif
