//
//  _CardBaCay_.h
//  iCasino_v2
//
//  Created by DauA on 6/24/14.
//
//

#ifndef __iCasino_v2__CardBaCay__
#define __iCasino_v2__CardBaCay__

#include <iostream>
#include "cocos2d.h"
#include "AllData.h"
#include "_Card_.h"

using namespace cocos2d;
using namespace std;

class CardBaCay : public Layer {
public:
    CardBaCay();
    ~CardBaCay();
    bool init();
    CREATE_FUNC(CardBaCay);

    void ResetGame();
	void resetCardChuong();

    void GivePocker();
	void GivePokerFindChuong(std::string _lcChuong);
	void GivePockerNextFindChuong(std::string _card, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count);
    void GivePockerNext(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count);

    //turn up all cards
    void TurnUpAllCards(string lc, const int& pos);
    void TurnUpAllCardsAtPos(string& lc, std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom);

    void CreateCardBack(const int& pos);
    void createListCard_Back(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom);
    void deleteAllCards_FromArray(std::vector<Card*>&P);

    string FindTypeCardByID(string& card);

    void SetMyName(string name);

    void PlaySound(const string& pathSound);


    void SetCallbackFunc(Ref* target, SEL_CallFuncN callfun);
    bool IsSpect();

    void setListUser(string lu);
    bool isRunningAction();
    
    int GetPosUserByName(string uid);

    std::vector<Card*>& getListCards_0();
    std::vector<Card*>& getListCards_1();
    std::vector<Card*>& getListCards_2();
    std::vector<Card*>& getListCards_3();
    std::vector<Card*>& getListCards_4();
    std::vector<Card*>& getListCards_5();
	std::vector<Card*>& getListCards_6();
	std::vector<Card*>& getListCardsByPos(int pos);

    void setPlayerMap(const std::map<std::string, int>& mmap);

	void callbackWhenGivePokerDone();
    void FlipEachCard(string& lc);

	void setPositionCardMe(float posX, float posY);

private:
    std::vector<string> arrCardTypes;
    std::string MyAI;

    //
    std::map<std::string, int> mapPlayers;
    std::vector<int> posgive;

    string ListUsers;
    
    //
	float posxChiabai, posYChiabai;
	int poskUser;
    float xCardPos_me;
    float yCardPos_me;
    float xCardPos_u1;
    float yCardPos_u1;
    float xCardPos_u2;
    float yCardPos_u2;
    float xCardPos_u3;
    float yCardPos_u3;
    float xCardPos_u4;
    float yCardPos_u4;
    float xCardPos_u5;
    float yCardPos_u5;
    float xCardPos_u6;
    float yCardPos_u6;

    float h_card_me;
    float h_card_notme;
    float w_card_me;
    float w_card_notme;

    float Duration_Action;
    int Count_Give_Card;

    std::vector<Card*>CARD_0;
    std::vector<Card*>CARD_1;
    std::vector<Card*>CARD_2;
    std::vector<Card*>CARD_3;
    std::vector<Card*>CARD_4;
    std::vector<Card*>CARD_5;
    std::vector<Card*>CARD_6;

	std::vector<Card*>CardChuong;
	vector<std::string> _cardChuong;

    Ref* m_callback;
    SEL_CallFuncN m_callbackListener;
};

class LayerCardLieng : public Layer{
public:
	LayerCardLieng();
	~LayerCardLieng();
	bool init();
	CREATE_FUNC(LayerCardLieng);

	void ResetGame();

	void GivePocker();
	void GivePockerNext(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom, const int& count);

	//turn up all cards
	void TurnUpAllCards(string lc, const int& pos);
	void TurnUpAllCardsAtPos(string& lc, std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom);

	void CreateCardBack(const int& pos);
	void createListCard_Back(std::vector<Card*>&P, const float& _width, const float& _height, const float& _left, const float& _bottom);
	void deleteAllCards_FromArray(std::vector<Card*>&P);

	string FindTypeCardByID(string& card);

	void SetMyName(string name);

	void PlaySound(const string& pathSound);


	void SetCallbackFunc(Ref* target, SEL_CallFuncN callfun);
	bool IsSpect();

	void setListUser(string lu);
	bool isRunningAction();

	int GetPosUserByName(string uid);

	std::vector<Card*>& getListCards_0();
	std::vector<Card*>& getListCards_1();
	std::vector<Card*>& getListCards_2();
	std::vector<Card*>& getListCards_3();
	std::vector<Card*>& getListCards_4();
	std::vector<Card*>& getListCards_5();
	std::vector<Card*>& getListCards_6();
	std::vector<Card*>& getListCardsByPos(int pos);

	void setPlayerMap(const std::map<std::string, int>& mmap);

	void callbackWhenGivePokerDone();

	void setRejoinSizeCardMe();
private:
	std::vector<string> arrCardTypes;
	std::string MyAI;

	//
	std::map<std::string, int> mapPlayers;
	std::vector<int> posgive;

	string ListUsers;

	//
	float xCardPos_me;
	float yCardPos_me;
	float xCardPos_u1;
	float yCardPos_u1;
	float xCardPos_u2;
	float yCardPos_u2;
	float xCardPos_u3;
	float yCardPos_u3;
	float xCardPos_u4;
	float yCardPos_u4;
	float xCardPos_u5;
	float yCardPos_u5;
	float xCardPos_u6;
	float yCardPos_u6;

	float h_card_me;
	float h_card_notme;
	float w_card_me;
	float w_card_notme;

	float Duration_Action;
	int Count_Give_Card;

	std::vector<Card*>CARD_0;
	std::vector<Card*>CARD_1;
	std::vector<Card*>CARD_2;
	std::vector<Card*>CARD_3;
	std::vector<Card*>CARD_4;
	std::vector<Card*>CARD_5;
	std::vector<Card*>CARD_6;

	Ref* m_callback;
	SEL_CallFuncN m_callbackListener;
	string getURL_byID_TienLen(string pid, string pType);
};

#endif
