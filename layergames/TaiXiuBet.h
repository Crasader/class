#ifndef TAIXIUBET_H
#define TAIXIUBET_H

#include "cocos-ext.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "TaiXiuCoin.h"
#include "../SceneManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

class SuggestTaiXiu : public ImageView{
private:
	Label* lblTotal;
	Label* lblPlayerBet;
public:
	CREATE_FUNC(SuggestTaiXiu)
		void SetInfo(long totalbet, long playerbet);
	void loadTexture(const std::string &fileName);
protected:
	SuggestTaiXiu();
	~SuggestTaiXiu();
	bool init();
};

class TaiXiuBet
{
protected:
	Button* pButton;
	std::string Type;
	std::string Name;
	int Tag;

	bool _mySelected;

	//
	long totalBet;
	long playerBet;

	SuggestTaiXiu* Suggest;
	Vec2 pButtonPos;

	//
	std::string pImageNormal;
	std::string pImageSelected;
	std::string pImageActive;
	std::string pImageEat;

	std::vector<ui::ImageView*> arrCoin;

	Vec2 __Pos;
	Vec2 __WorldPos;

	Node* _target;
	bool isAnimation;

public:
	TaiXiuBet(Button* item, std::string& name, const int& tag, std::string& type);
	~TaiXiuBet();
	std::string GetType() const;
	int GetTag() const;
	Button* GetMenu() const;

	void SetImage(const std::string& pNormal, const std::string& pSelect, const std::string& active, const std::string& eat);

	void Selected();
	void UnSelected();
	void MySelect();

	void CanEat(Vec2 _dest, Vec2 _pos);
	void CanEat();
	void setIsAnimation(bool _animation);
	bool getIsAnimation();
	//void processAnimationWin(Vec2 _dest, Vec2 _pos);
	Vec2 getpButtonPos();

	void SetTotalBet(const long& totalbet);
	void SetPlayerBet(const long& mybet);
	void IncreTotalBet(const long& add);

	void ClientBet(const long& money);

	void Reset();

	void SetTarget(Node* target);

	const std::vector<ui::ImageView*>& GetArrayCoin() const;

	void AddCoin(ui::ImageView* img);
	void ExecuteRefresh();
	void reAddCoinMove(const int& count, const Vec2& posfirst, const char* src);

	const long& GetTotalBet() const;
	const long &GetPlayerBet() const;

	void SetSuggest(SuggestTaiXiu* suggest);
	void DisplayCurrentBet();
	void HideCurrentBet();

	void SetWorldPos(const Vec2& pos);
	Vec2 GetWorldPos();

	void SetPositionCoin(const Vec2& pos);

	virtual Vec2 GetPostoMoveCoin() = 0;
    Vec2 getPointCoin();
};

class TXBetTaiXiu : public TaiXiuBet{
public:
	TXBetTaiXiu(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetTaiXiu();
	Vec2 GetPostoMoveCoin();
};

// Bo Doi 2-2, 3-3
class TXBetBoDoi : public TaiXiuBet{
public:
	TXBetBoDoi(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetBoDoi();
	Vec2 GetPostoMoveCoin();
};

// Bo Ba 3-3-3, 6-6-6
class TXBetBoBa : public TaiXiuBet{
public:
	TXBetBoBa(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetBoBa();
	Vec2 GetPostoMoveCoin();
};

// Co 1 trong cac bo ba
class TXBetAnyBa : public TaiXiuBet{
public:
	TXBetAnyBa(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetAnyBa();
	Vec2 GetPostoMoveCoin();
};

// Diem tu 4 - 17
class TXBetDiem : public TaiXiuBet{
public:
	TXBetDiem(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetDiem();
	Vec2 GetPostoMoveCoin();
};


// Cap doi 1-2, 2-3, 4-5
class TXBetCapDoi : public TaiXiuBet{
public:
	TXBetCapDoi(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetCapDoi();
	Vec2 GetPostoMoveCoin();
};

// Don le 1, 2, 3
class TXBetDonLe : public TaiXiuBet{
public:
	TXBetDonLe(Button* item, std::string& name, const int& tag, std::string& type);
	~TXBetDonLe();
	Vec2 GetPostoMoveCoin();
};

class TaiXiuBetFactory{
public:
	static TaiXiuBet *CreateTXBet(const int& group, Button* button, const char* name, const int& tag, const char* type);
};

#endif // TAIXIUBET_H
