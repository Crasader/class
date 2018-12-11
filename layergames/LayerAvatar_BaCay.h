//
//  LayerAvatar_BaCay.h
//  iCasino_v2
//
//  Created by DauA on 6/26/14.
//  4 avatar for game
//
//
//

#ifndef __iCasino_v2___LayerAvatar_BaCay___
#define __iCasino_v2___LayerAvatar_BaCay___

#include <iostream>
#include "cocos2d.h"
#include "Avatar52La.h"
#include "GameServer.h"
#include <map>

using namespace cocos2d;
using namespace std;

class LayerBaCayAvatar : public Layer {
public:
    LayerBaCayAvatar();
    ~LayerBaCayAvatar();
    virtual bool init();
    CREATE_FUNC(LayerBaCayAvatar);

    void SetUrlAvatar(const int& pos, char *url);
    void SetFlag(const int& pos, bool isShow);
    void SetName(const int& pos, string& name);
    void SetName(const int& pos, const char* name);
    void SetMoney(const int& pos, string& money);
    void SetMoney(const int& pos, const char* money);
    void SetMoney(const int& pos, const int& money);
    void SetReady(const int& pos, bool isReady);
    void SetVisibleLayerInvite(const int& pos, bool isShow);
    void setResult(int pos, int rescode, int type, bool isOnGame, double money);
    void ResetAll();
    void SetPosChuong(const int& pos);
	void setPosBien(int _pos, std::string _valueBien);

    void UnReadyAllUser();
    void ResetGame();

    void SetListUser(const string& listUser);

    Avatar52La* GetUserByPos(const int& pos);
    int GetPosByName(const string& uid);
    //

    void RunTimer(const int& PosPlayer);
    void RunAllTimer();
    void StopAllTimer();
    bool IsSpect();
    bool IsStartedGame();
    bool IsPlaying(string listUser);
    std::map<std::string, int>& GetMapPlayer();
    void setListUserGuess(string _list, char charSplit);
    Avatar52La* getEmptyAvatar();
    void resetGuessUser();
    void showAvatarMe(int tag, string userName);
    //ham xu ly check ip cua tat ca thang choi
    void checkSameIP();

	//update for new version - lieng p1
	void hideAllBet(bool isHide);
	void resetAllBets();
	void setBetInfo(double bet, int pos);
	void removeResult();

	ImageView* getImageChuong();
	void resetBienButton();
private:
	ImageView* imgBienActive;
	Label* lblValueBien;
    ui::ImageView*	Chuong;
    ui::Button*		btnNgoiChoi;
    string			ListUsers;
    string			MyAI;
    bool			IsSpector;
    bool            HasPlayed;
    std::map<std::string, int> mapPlayers;

    void SetMapPlayer(std::string& listplayers);

    bool b_IsSpect;
    bool b_IsPlaying;
    vector<string> arrAI;
};

#endif
