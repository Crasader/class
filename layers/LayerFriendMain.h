#ifndef LayerFriendMain_h__
#define LayerFriendMain_h__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "../Common.h"
#include "../AllData.h"
#include "ui/CocosGUI.h"
#include "../layergames/StructHistory.h"
#include "../layergames/CustomTableViewCell.h"
#include "../layergames/CircleAvatar.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::ui;
using namespace std;

class LayerFriendCell : public Layer
{
public:
	virtual bool init();
    CREATE_FUNC(LayerFriendCell)
	void setDatas(string nameFriend, bool isOnline);
    void setDatas(bool isOnline, const infoUser& data);
	ImageView* bgPressed;

private:
    CircleAvatar* circleAvatar;
	ImageView* spriteAvatar;
	Text* labelName;
	Text* labelOnlineStatus;
	Sprite* spriteOnliseStatus;
	ImageView* avtVip;
};

class LayerFriendDetailCell : public Layer
{
public:
	virtual bool init();
    CREATE_FUNC(LayerFriendDetailCell)
	void setDatas(int idGame, string soTranThangThua);
private:
	Text* labelTenGame;
	Text* labelSoTranThangThua;
};
class LayerFriendMain : 
	public Layer, 
	public TableViewDataSource, 
	public TableViewDelegate, 
	public PlayerCallBack
{
public:
	LayerFriendMain();
	~LayerFriendMain();

	TableViewCell* createCell4History(TableView *table, int idx);
	TableViewCell* createCell4Friends(TableView *table, int idx);

    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxBuddyRemove(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnSmartFoxBuddyListInit(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void loadFirstFriend();
	bool init();
	void onButtonMessage(Ref* pSender);
	void onButtonUnFriend(Ref* pSender);
    CREATE_FUNC(LayerFriendMain)

private:
	enum tagTableView{
		tag_Historys = 0,
		tag_Friends = 1
	};

	enum tagLabelInCell{
		tag_NameGame,
		tag_Level,
		tag_WinLose
	};

	enum tagFriendCell{
		tag_avatar,
		tag_NameFriend,
		tag_OnlineState,
		tag_OnlineStateImage
	};

	enum tag_SpriteClick{
		tag_friendInfoDisable = 0,
		tag_inviteDisable = 1,
		tag_friendInfoEnable = 2,
		tag_inviteEnable = 3
	};

	int mCurrentRoomIndex;
	StructHistory historys;
	cocos2d::EventListener* _touchListener;
	vector<StructHistory>lstHistorys;
	string currFriendID;
	TableView* tblFriendList;
	TableView* tblFriendDetailList;
	Sprite* nodeFriends;
	Layout* nodeHistorys;

	Text* lblName;
	Text* lblXu;
	Text* lblLevel;
	Text* lblOnlineStatus;
	Sprite* spriteAvatar;
	Sprite* spriteTT;

	Button* btnMessage;
	Button* btnUnFriend;

	Node* rootNode;

	ImageView* avtVip;
	int idxChoosed = 0;
	LoadingBar* ldbLevel;
    
    vector<int> lstGameID;
};

#endif // LayerFriendMain_h__
