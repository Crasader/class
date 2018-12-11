//
//  LayerCongDong.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/8/16.
//
//

#ifndef LayerCongDong_hpp
#define LayerCongDong_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "../mFaceBook.h"
#endif
#include "Requests/ExtensionRequest.h"
#include "../SceneManager.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class SpriteEx;


class LayerItemFriendFacebookCongDong :
	public cocos2d::Layer,
	public PlayerCallBack{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemFriendFacebookCongDong);
	void setDatas(string _uid,string urlAvatar, string name, bool isInvite);
	void onInviteFriendCallBack(Ref* target, SEL_CallFuncO listener);
	void onBtnInvitedFriend(Ref* pSender);

private:
	int count_clicked = 0;
	ImageView *urlAvatar;
	ImageView *imgSelected;
	Text *nameFriend;
	Button *btnSelectedInvite;
	SEL_CallFuncO mCallBackListener;
	Ref *mCallBack;
        SpriteEx* _iconSprite;

    string uid = "";
        bool _isInvite = false;
};

class LayerCongDong :
	public cocos2d::Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerCongDong();
	~LayerCongDong();
	virtual bool init();
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;
	void getPartnerInfo();
	string getCurrentTime();
	void onItemFriendFaceClicked(Ref* pSender);
	void onBtnNhanMoiFB(Ref* pSender);
	void onBtnNhanShareFB(Ref* pSender);
	void onBtnChonTatCa(Ref* pSender);
	void CountDownTimerInvite(float dt);
	void showTimerInvite();
	void CountDownTimerShare(float dt);
	void showTimerShare();
    void showFriend(Node* sender, void* data);
    void hideLayer();
    void showLayer();
    void loadEvent();

	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;


	TableViewCell* createCell4LstFriendFacebook(TableView *table, ssize_t idx);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	CREATE_FUNC(LayerCongDong);

private:

	//test display
	/*struct facebookFriendInfo{
		string urlImg;
		string nameFace;
		};*/
	//vector<facebookFriendInfo> lstFriend;

	vector<int> lstNotShow;
	string partnerID;
	string srcID;
	int osid;
	string devID;
	Layout* pnlCongDong;
    Layout* pnlFriend;
	Text* txtGoldMoi;
	Text* txtNhanShare;
	Button* btnNhanMoi;
	Button* btnNhanShare;
	Button* btnTabCongDong;
	TableView* tblFriendFB;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mFaceBook *face;
#endif
	int timerInvite;
	int timerShare;
    int tag_tbl_friendfb = 10;
private:
	void fakeList();
};



#endif /* LayerCongDong_hpp */
