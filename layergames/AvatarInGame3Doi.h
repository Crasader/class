//
//  _Avatar_inGame_.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/15/14.
//  4 avatar for game
//
// // parent->this===>setListUserBy...->formatAndStore->updateUsers.
//

#ifndef AvatarInGame3Doi_h__
#define AvatarInGame3Doi_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AllData.h"
#include "Avatar52La.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
#define  tag_layer_avatar_52  1309
class LayerAvatarInGame3Doi : public Layer
{
public:
	LayerAvatarInGame3Doi();
	~LayerAvatarInGame3Doi();

	virtual bool init();
	CREATE_FUNC(LayerAvatarInGame3Doi);

	void setIcon(const int& pos, char *url);
	void setFlag(const int& pos, bool isShow);
	void setName(const int& pos, string name);
	void setMoney(const int& pos, string money);
	void setMoney(const int& pos, int money);
	void setMoney(const int& pos, double money);
	void setReady(const int& pos, bool isReady);
	void setVisibleLayerInvite(const int& pos, bool isShow);

	void resetAll();

	void setUnReadyAllUser();
	void resetGame();

	void setListUserByPhom(string listUser);
	void setListUserByTienLen(string listUser);
	//
	void formatAndStore(const char &c1, const char &c2);

	Avatar52La* getUserByPos(const int& pos);

	int getIndexInArrByName(string& name) const;
	int getIndexInArrByAccountID(string aI);
	//
	int getPosByName(string name);
	int getPosByAccountID(string aI);
	string getNameByPos(int pos);
	string getAccountIDByPos(int pos);
	//
	void updateUsers();

	// Time
	void runTimer(const int& posUser);
	void runTimerWithTime(const int& posUser, const float& time);
	void stopAllTimer();

	// dựa vào listUser và myAI, xác định xem mình là khách hay là người chơi
	bool isSpectator();
	bool isStartedGame();

	//stop Blink Avatar
	void stopAllBlinkAvatar();
	CC_SYNTHESIZE(int, _count, UserCount);
	CC_SYNTHESIZE(int, _guess, GuessCount);
	void removeUserFromList(string _name);
	void showAvatarMe(int tag, string userName);
	void setResult(int pos, int rescode, int type, bool isOnGame, double money);
	void setListUserGuess(string _list, char charSplit);

private:
	enum NAME_GAME {
		PHOM = 1,
		TIEN_LEN
	};

	int typeGame;
	bool isGuess;
	bool isSpector;
	string myName;
	string myAI;
	string listUser;
	char char1;
	char char2;
	// thanhhv1:10000;thanhhv2:20000;   ==> char1=';'    char2=':'

	// mảng chứa
	vector<string> arrName;
	vector<string> arrFlag;
	vector<string> arrURL;
	vector<string> arrAI;
	vector<bool> arrRD;
	vector<int> arrMoney;
	vector<double> arrMoneyDouble;
	void resetGuessUser();
	Avatar52La* getEmptyAvatar();

	//ham xu ly check ip cua tat ca thang choi
	void checkSameIP();
};

#endif // AvatarInGame3Doi_h__


