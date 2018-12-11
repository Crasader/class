#ifndef __LAYER_GIVECARD_CHAN__
#define __LAYER_GIVECARD_CHAN__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "LayerChanToast.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;

class Layer_GivePocker_Chan : public cocos2d::Layer, public PlayerCallBack
{
public:
	Layer_GivePocker_Chan();
	~Layer_GivePocker_Chan();
	virtual bool init();
    CREATE_FUNC(Layer_GivePocker_Chan)
	void onExit();

	static Layer_GivePocker_Chan* createInstance();

	void SetLayerChanToast(LayerChanToast* toast);

	void InitAllCards();
	void AddVirtualCards(ImageView* img);
	//void test(Node* sender, void* data);
	void test(Ref* sender, int data);

	void MoveRandomIsNotPlayerBocCai(std::string cards);
	void SendRequsetBocNoc();

	ImageView* Create5Cards(const float& posX, const float& posY, const int& mTag);

	void MoveCard4Users(std::string& cards, const std::string& user);

	void StartGivePocker();
	void GivePocker(const int& Pos);
	void GiveToPosition(const float& toX, const float& toY, const int& pos);
	void DelayTimeturn2();
	void DelayTimeEnd();

	void SetIntervalBocNoc();
	void UpdateTimer(float dt);

	void NocTouch(Ref *pSender, Widget::TouchEventType type);
	void BocNoc(Ref *pSender, Widget::TouchEventType type);

	void SetIsPlayerBocCai(const bool& isFlag);
	void SetPlayerBocCai(const std::string& player);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	void MoveCardsToPosition();

	int GetPosUserByName(const std::string& uid);
	void SetMapPlayer(const std::map<std::string, int>& mapp);

	std::vector<int> SortArrayTag(std::vector<int> _arr, int position);
	void ChiaBai(std::vector<int> toPos, std::vector<int> allTag);
	void FinishGive(ImageView *img, const int& tpos);
	void SetCallBack_BocBaiIfGuess(Ref* target, SEL_CallFuncN callfun);
	void Call_CallBackWhenEnd();

private:

	Ref* m_callback;
	SEL_CallFuncN m_callbackListener;

	bool IsPlayerBocCai;

	std::string PlayerBocCai;
	std::string PlayerStart;

	std::vector<int> arrTag;

	std::map<std::string, int> mapPlayers;

	Layer *uLayer;
	ImageView *Noc;
	ImageView *Noc2;

	Label *Timer;
	float StartTimmer;

	LayerChanToast *Toast;

	std::vector<ImageView*> arrNocs;

	ImageView *B1;
	ImageView *B2;
	ImageView *B3;
	ImageView *B4;
	ImageView *B5;
	ImageView *B6;
	ImageView *B7;
	ImageView *B8;
	ImageView *B9;
	ImageView *B10;

	bool flag_ChonNoc;
	bool flagBocCai;
	int CountLeft;
	int CountRight;

	int Index_Noc;
	int Index_Choose;

	float c1_leftX;
	float c1_leftY;

	float c2_leftX;
	float c2_leftY;

	float c3_leftX;
	float c3_leftY;

	float c4_leftX;
	float c4_leftY;

	float c5_leftX;
	float c5_leftY;

	float c1_rightX;
	float c1_rightY;

	float c2_rightX;
	float c2_rightY;

	float c3_rightX;
	float c3_rightY;

	float c4_rightX;
	float c4_rightY;

	float c5_rightX;
	float c5_rightY;
};
#endif // __LAYER_GIVECARD_CHAN__
