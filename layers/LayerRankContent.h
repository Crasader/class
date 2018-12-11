#ifndef __iCasino_v2__LayerRankContent__
#define __iCasino_v2__LayerRankContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameServer.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../layergames/StructHistory.h"
#include "../layergames/CustomTableViewCell.h"
#include "AllData.h"
#include "../layergames/CircleAvatar.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;
class LayerRankRightCell : public Layer {
public:
    LayerRankRightCell();
    ~LayerRankRightCell();
	virtual bool init();
    CREATE_FUNC(LayerRankRightCell)
    void setDatas(const int& index, const infoUser& data);
private:
    CircleAvatar* circleAvatar;
	Text* lblTop;
	Text* txtName;
	Text* txtMoney;
	Text* txtLevel;
	Sprite* spriteAvatar;
	ImageView* avtVip;
};

class LayerRankTopDaiGia :public Layer{
public:
    LayerRankTopDaiGia();
    ~LayerRankTopDaiGia();
	virtual bool init();
    CREATE_FUNC(LayerRankTopDaiGia)
    void setDatas(const int& index, const infoUser& data);
private:
    CircleAvatar* circleAvatar;
	Text* lblTop;
	Text* txtName;
	Text* txtMoney;
	Text* txtLevel;
	Sprite* spriteAvatar;
	ImageView* avtVip;
};

class LayerRankLeftCell : public Layer
{
public:
	ImageView* bgPressed;
    Text* txtGameName;
	virtual bool init();
	CREATE_FUNC(LayerRankLeftCell);
	void setDatas(int gameID,string gameName);
private:
	ImageView* btnIconGame;
	
};
class LayerRankContent : 
	public cocos2d::Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerRankContent();
	~LayerRankContent();

	TableViewCell* createCell4Left(TableView *table, int idx);
	TableViewCell* createCell4Right(TableView *table, int idx);
	TableViewCell* createCell4TopDaiGia(TableView *table, int idx);

	void loadDatas();
	void loadFirst();
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override ;
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override;
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;
	
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
	virtual bool init();
	void loadTopDaiGia();
	CREATE_FUNC(LayerRankContent);

	Node* nodeTopCaoThu;
	Node* nodeTopDaigia;
public:
    bool isWebView = true;
private:
	enum tagTableView{
		tagTableLeft = 0,
		tagTableRight = 1,
		tagTableTopDaiGia = 2
	};
	struct StructRank{
		int ID;
		std::string* Name;
	};
	struct StructInfo{

	};
	TableView* tblLeft;
	TableView* tblRight;
	TableView* tblTopDaiGia;
	vector<infoUser> lstTops;
	vector<StructRank> lstRanks;
	Layout* panelLeft;
	Layout* panelRight;
	int idxChoosed = 0;
    
    vector<int> lstGameID;

};
#endif //__iCasino_v2__LayerRankContent__

