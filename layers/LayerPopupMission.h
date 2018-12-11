//
//  LayerPopupMission.hpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/8/16.
//
//

#ifndef LayerPopupMission_hpp
#define LayerPopupMission_hpp

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
class LayerItemDiemDanh : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemDiemDanh);
	void setDatas(int day, int status, string tien, string exp, string moruong, string quay);
	void resetItem();
private:
	Sprite* sprTypeBonus;
	Text* lblBonus;
	Text* lblDay;
	Layout* layoutBonus;
	Sprite* isCom;
};
class LayerItemDiemDanhP2 : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemDiemDanhP2);
	void setDatas(string stt, string phanthuong, bool isCheck);
	void resetItem();
private:
	Layout* pnlType1;
	Text* txt1Type1;
	Layout* pnlType2;
	Text* txt1Type2;
	Layout* pnlType3;
	Text* txt1Type3;
	Text* txt2Type3;
	Layout* pnlType4;
	Text* txt1Type4;
	Text* txt2Type4;
	Text* txtSTT;
	ImageView* imgcheck;
};
class LayerItemNhiemVuMotLan : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemNhiemVuMotLan);
	void setDatas(string id, string name, string status, string type, string bonus, bool isShowcoin = true);
    void setDatasTanThu(string id, string name, string status, string type, string bonus, bool isShowcoin);
	void onBtnGetFBBonus(Ref* pSender);
	void resetItem();
private:
	LoadingBar* ldStatus;
	Text* txtName;
	Text* txtStatus;
	Text* txtBonus;
	ImageView* imgCheck;
	Sprite* bgStatus;
	ImageView* coin;
	Button* btnRegard;
	ImageView* imgStatus1Bonus;
	ImageView* imgStatus2Bonus;
	Text* txt2BonusCoin;
	Text* txt2BonusKey;
	ImageView* imgCheck2Bonus;
	ImageView* imgStatusBonusKey;
	Text* txt1BonusKey;
	ImageView* imgCheck1BonusKey;
};
class LayerItemTanThu : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemTanThu);
    void setDatas(string id, string name, string status, string bonus,string receive,string type);
    void onBtnReward(Ref* pSender);
    void resetItem();
private:
    Text* txtName;
    Text* txtBonus;
    ImageView* imgCheck1;
    Text* lblBonus;
    ImageView* imgCheck2;
    Text* silver2;
    Text* gold2;
    Button* btnRegard1;
    Button* btnRegard2;
    ImageView* bg1;
    ImageView* bg2;
    ImageView* imgCoin;

};
class LayerItemThanhTuu : public Layer
{
public:
    virtual bool init();
    CREATE_FUNC(LayerItemThanhTuu);
    void setDatas(string id, string name, string star1,string star2,string star3,string star4,string star5 ,string bonus,string currStar,string getStar);
    void onBtnReward(Ref* pSender);
    void loadStar(string star);
    void resetItem();
private:
    Text* txtName;
    Text* txtBonus;
    Text* txtStats;
    Button* btnRegard1;
    vector<ImageView*> lstStar;
    LoadingBar* lbStatus;
    ImageView* imgCoin;
    ImageView* imgCheck;
      int currgetStar = 0;
    
};
class LayerItemNhiemVuNgayNew : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemNhiemVuNgayNew);
	void setDatas(string id, string name, string socap, string khoinghiep, string trieuphu, string daigia, string levelhientai, string tonglevel, string tiendo, string tongtiendo, string currpoint);
	void onBtnGetFBBonus(Ref* pSender);
	void resetItem();
private:
	Layout* pnlType1;
	Layout* pnlType2;

	Text* txtMissionNameType1;
	Text* txtDanhHieu1Type1;
	Text* txtDanhHieu2Type1;
	Text* txtDanhHieu3Type1;
	Text* txtDanhHieu4Type1;

	ImageView* danhnhieu1Type1;
	ImageView* danhnhieu2Type1;
	ImageView* danhnhieu3Type1;
	ImageView* danhnhieu4Type1;

	vector<ImageView*> lstImgDanhHieu;
	vector<Text*> lstTxtDanhHieu;

	Text* txtTienDoType1;
	Text* txtDiemType1;

	Text* txtMissionNameType2;
	Text* txtDanhHieu1Type2;
	ImageView* danhnhieu1Type2;

	Text* txtTienDoType2;
	Text* txtDiemType2;

};

class LayerItemFriendFacebook :
	public cocos2d::Layer,
	public PlayerCallBack{
public:
	virtual bool init();
	CREATE_FUNC(LayerItemFriendFacebook);
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

class LayerPopupMission :
	public cocos2d::Layer,
	public TableViewDataSource,
	public TableViewDelegate,
	public PlayerCallBack
{
public:
	LayerPopupMission();
	~LayerPopupMission();
	virtual bool init();
	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;
	void onBtnClose(Ref* pSender);
	void onBtnTabNhiemVu(Ref* pSender);
	void onBtnTabDiemDanh(Ref* pSender);
	void onBtnTabNhiemVuNgay(Ref* pSender);
	void onBtnTabCongDong(Ref* pSender);
	void onBtnDoiDiem(Ref* pSender);
	void onBtnNhanKey(Ref* pSender);
	void onBtnTheLe(Ref* pSender);
	void doDiemDanh();
	void doGetBonusFB();
	void getPartnerInfo();
	string getCurrentTime();
	int getCellDiemDanh();
	void onItemFriendFaceClicked(Ref* pSender);
	void onBtnNhanMoiFB(Ref* pSender);
	void onBtnNhanShareFB(Ref* pSender);
	void onBtnChonTatCa(Ref* pSender);
	void CountDownTimerInvite(float dt);
	void showTimerInvite();
	void CountDownTimerShare(float dt);
	void showTimerShare();
    void showFriend(Node* sender, void* data);
    void onBtnHuongDanDiemDanh(Ref* pSender);
    void onBtnNhiemVuTanThu(cocos2d::Ref *pSender);


	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	TableViewCell* createCell4LstDiemDanh(TableView *table, ssize_t idx);
	TableViewCell* createCell4LstNhiemVu1Lan(TableView *table, ssize_t idx);
	TableViewCell* createCell4LstNhiemVuNgay(TableView *table, ssize_t idx);
	TableViewCell* createCell4LstFriendFacebook(TableView *table, ssize_t idx);
    TableViewCell* createCell4LstNhiemVuTanThu(TableView *table, ssize_t idx);

	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

	CREATE_FUNC(LayerPopupMission);

private:

	struct Item{
		string tien;
		string exp;
		string moruong;
		string quay;
		int status;
	};
	enum{
		tag_tbl_diemdanh = 10,
		tag_tbl_nhiemvu1lan,
		tag_tbl_nhiemvungay,
		tag_tbl_friendfb,
        tag_tbl_nhiemvutanthu
	};
	enum{
		tag_cell_nhiemvu1lan = 20,
		tag_cell_nhiemvuhangngay
	};

	struct MissionNew{
		string id;
		string name;
		string socap;
		string khoinghiep;
		string trieuphu;
		string daigia;
		string capdohientai;
		string tongcapdo;
		string tiendohientai;
		string tongtiendo;
		string diem;
	};
	struct Mission{
		string id;
		string name;
		string status;
		string bonus;
		string type;
	};
    
    struct TanThu{
        string id;
        string name;
        string bonus;
        string status;
        string receive;
        string type;
    };

	struct DiemDanhP2{
		string stt;
		string phanthuong;
		bool isCheck;
	};

    struct ThanhTuu{
        string id;
        string name;
        string star1;
        string star2;
        string star3;
        string star4;
        string star5;
        string bonus;
        string currStar;
        string getStar;
        
    };
	//test display
	/*struct facebookFriendInfo{
		string urlImg;
		string nameFace;
		};*/
    vector<ThanhTuu> lstThanhTuu;
	vector<DiemDanhP2> lstDiemDanhP2;
	Button* btnTabNhiemVu;
	Button* btnTabNhiemVuNgay;
	Button* btnTagDiemDanh;
    Button* btnNhiemVuTanThu;
	Layout* layoutDiemDanh;
	TableView* tblListDiemDanh;
	TableView* tblListNhiemVu1Lan;
	TableView* tblListNhiemVuNgay;
    TableView* tblListNhiemVuTanThu;
	Text* lblDes;
	//vector<facebookFriendInfo> lstFriend;
	vector<Item> lstDiemDanh;
	vector<Mission> lstNhiemvu1Lan;
	vector<Mission> lstNhiemvuNgay;
	vector<MissionNew> lstNhiemvuNgayNew;
    vector<TanThu> lstTanThu;
	vector<int> lstNotShow;
	string partnerID;
	string srcID;
	int osid;
	string devID;

	int clicked_amount = 0;
	int curr_tag;
    int curr_tab = 1;

	Layout* layoutDiemDanhNew;
	Layout* pnlFriend;

	Layout* pnlNhiemVuNgay;
	Text* txtDiemTuanNhiemVuNgay;
	Text* txtDiemNgayNhiemVuNgay;
	Text* txtTienDoNhiemVuCoDinh;
	Text* txtStatusNhiemVuCoDinh;

	Layout* pnlDiemDanhNew;
	Text* txtDiemTuanDiemDanhNew;
	Text* txtDiemNgayDiemDanhNew;
	Text* txtCapVipDiemDanh;
	Text* txtDesCapVip;

	Text* txtDiemDiemDanh1;
	Text* txtDiemDiemDanh2;
	Text* txtDiemDiemDanh3;
	Text* txtDiemDiemDanh4;
	Text* txtDiemDiemDanh5;
	Text* txtDiemDiemDanh6;
	Text* txtDiemDiemDanh7;

	vector<Text*> lstTxtDiemDiemDanh;

	ImageView* imgBgDaDiemDanh1;
	ImageView* imgBgDaDiemDanh2;
	ImageView* imgBgDaDiemDanh3;
	ImageView* imgBgDaDiemDanh4;
	ImageView* imgBgDaDiemDanh5;
	ImageView* imgBgDaDiemDanh6;
	ImageView* imgBgDaDiemDanh7;

	vector<ImageView*> lstBgDaDiemDanh;

	Text* txtExpDiemDanh1;
	Text* txtExpDiemDanh2;
	Text* txtExpDiemDanh3;
	Text* txtExpDiemDanh4;
	Text* txtExpDiemDanh5;
	Text* txtExpDiemDanh6;
	Text* txtExpDiemDanh7;

	vector<Text*> lstTxtExpDiemDanh;

	Button* btnNhanKeyNhiemVuNgay;

	//diem danh bai moc
	Text* txt1Ngay7;
	Text* txt2Ngay7;
	ImageView* imgCheckNgay7;

	Layout* pnlCongDong;
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
    bool isDiemDanh = false;
private:
	void fakeList();
};



#endif /* LayerPopupMission_hpp */
