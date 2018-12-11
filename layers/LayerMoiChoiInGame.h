#ifndef LayerMoiChoiInGame_h__
#define LayerMoiChoiInGame_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameServer.h"
#include "../SceneManager.h"
#include "../Common.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
class LayerMoiChoiCell :public Layer
{
public:
	virtual bool init();
	void setDatas(string nameFriend, string money);
	CREATE_FUNC(LayerMoiChoiCell);

private:
	ImageView* imgAvatar;
    ImageView* imgAvatarVip;
	Text* lblMoney;
	Text* lblName;
};

class LayerMoiChoiInGame : 
	public Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerMoiChoiInGame();
	~LayerMoiChoiInGame();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	virtual void onEnter() override;
	virtual void onExit() override;

	virtual bool init();
	void onBtnClose(Ref* pSender);
	void onBtnphongcho(Ref* pSender);
	void onBtnBanbe(Ref* pSender);
	void loadAllDatas();
	virtual void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	CREATE_FUNC(LayerMoiChoiInGame);
private:
	Button* btnPhongCho;
	Button* btnBanBe;
    ImageView* imgCoin;
	struct StructFriendInfo{
		int uid;
		string aI;
		string aN;
		string amf;
	};
	vector<StructFriendInfo> lstFriendInfos;
	cocos2d::EventListener* _touchListener;
	Text* lblTitle;
	TableView* tblTable;
	string mListPlayers;
	int gameID;
};

#endif // LayerMoiChoiInGame_h__
