#ifndef LayerPopupProfile_h__
#define LayerPopupProfile_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "../Common.h"
#include "../layergames/NativeHelper.h"
#include "../layergames/StructHistory.h"
#include "../layergames/CircleAvatar.h"
#include "../scenes/SceneLogin.h"
#include <vector>

#include "network/HttpClient.h"

#include "GameServer.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace std;

class LayerThanhTichCell : public Layer
{
public:
	LayerThanhTichCell(void);
	~LayerThanhTichCell(void);

	virtual bool init(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc);
	static LayerThanhTichCell* create(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc);

	void setDatas(int idGame, int soTranThang, int soTranHoa, int soTranThua, int soTranBoCuoc);
private:
	ImageView* avatarGame;
	Text* labelTenGame;
	Text* labelSoTranThang;
	Text* labelSoTranHoa;
	Text* labelSoTranThua;
	Text* labelSoTranBoCuoc;
};

class ChangeAvatar;
class LayerPopupProfile : public LayerColor, public TableViewDataSource, public TableViewDelegate
	, public PlayerCallBack
{
protected:
	
	//Server
	void OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
public:
	LayerPopupProfile();
	~LayerPopupProfile();

	static LayerPopupProfile* instance;

	void SetLinkAvatarToServer(std::string linkAvatar);

	void loadAllDatas();

    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)override {}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)override {}
	virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)override;
	virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)override;
	virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table)override;

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
	virtual void onEnter() override;
	virtual void onExit() override;

	void onButtonLichSu(Ref* pSender);
    void onButtonCuaHang(Ref* pSender);
	void onButtonQuaTang(Ref* pSender);
    void onButtonHopThu(Ref* pSender);
	void onButtonDoiMatKhau(Ref* pSender);
	void onButtonClose(Ref* pSender);
    void onButtonBaoMat(Ref* pSender);

	void onButtonDoiAvatar(Ref* pSender);
	void onButtonNaptien(Ref* pSender);
	void onButtonUpdateInfo(Ref* pSender);

	void onButtonBackInfo(Ref* pSender);
	void onButtonUpdateProfile(Ref* pSender);
	void onButtonDetailGoldHistory(Ref* pSender);

	void onButtonUpdatePassword(Ref* pSender);
    void onButtonLogout(Ref* pSender);
    void onButtonLKFacebook(Ref* pSender);

    void onButtonDoiTien(Ref* pSender);
    void doLienKetFB(Node* sender, void* data);
    void onButtonGold(Ref* pSender);
    void onButtonSilver(Ref* pSender);
    void onButtonDoiSTK(Ref* pSender);
    std::string tail(std::string const& source, size_t const length);
    
    void OnSmartFoxAdminMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
    cocos2d::experimental::ui::WebView *webView;
    bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
    void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif

	virtual bool init();
    CREATE_FUNC(LayerPopupProfile)

private:
	cocos2d::EventListener* _touchListener;
	Button* btnLichSu;
    Button* btnBaoMat;
	Button* btnCuaHang;
    Button* btnQuaTang;
	Button* btnHopThu;
    
    Text* txtName;
    Text* txtGold;
    Text* txtSilver;
    Text* txtVip;
    Text* txtLevel;
    Text* txtEmail;
    Text* txtNgayThamGia;
    Text* txtSoTranThang;
    Text* txtSTKNganHang;
    Text* txtTenHienThi;

//	Node* nodeCaNhan;
//
    CircleAvatar* circleAvatar;
//
//	Node* nodeThanhTich;
//	Node* nodeDoiMatKhau;
//	Node* nodeUpdateProfile;
//
//    Node* nodeChangeAvatar;
//
//	TextField* txtOldPass;
//	TextField* txtmatkhauMoi;
//	TextField* txtnhaplaiMatKhauMoi;
//	//label error change password
//	Text *lblError;
//
//	TextField* txtinfoEmail;
//	Text* lblErrorEmail;
//	Text* lblErrorPhone;
//	TextField* txtinfoPhone;
//    TextField* txtinfoPhoneOLD;
//
//    Text* labelCaNhanVip;
//    LoadingBar* ldbVip;
//    Text* labelDescLevel;
//    Text* labelDescVip;
//	Text* labelCanhanEmail;
//	Text* labelCanhanPhone;
//	Text* labelCanhanName;
//	Text* labelCanhanLevel;
//	Text* lblGold;
//    Text* lblSilver;
//	Sprite* spriteCanhanAvatar;
//	Sprite* spriteCanhanLevel;
//	ImageView* avtVip;
//	vector<StructHistory> lstHistorys;
//string _name = "";
//	TableView* tblHistories;
//	LoadingBar* ldbLevel;
//	string _oldPass;
//    string email;
//    vector<int> lstGameID;
//
//    bool isEmailEdited;
//
	void showToast(string mess);
//
//    //
//    Button* btnChangeAvatarOK;
//    Button* btnChangeAvatarCancel;
	Button* btnDoiAvatar;
    Button* btn_lkfb;
//
//    TableView* tableListAvatar;
//
//    int tag_Table_History;
//    int tag_Table_Avatar;
//
    vector<ChangeAvatar*> arrAvatarChange;
    vector<double> listVip;
//
    void onChangeAvatarClick(Ref* pSender, ui::Widget::TouchEventType type);
    void onChangeAvatarCancel(Ref* pSender, ui::Widget::TouchEventType type);
//
    void initListAvatar();
//    void GetAllUrl();
    void changeAvatarCallBack(Ref *pSend);
    bool isLoadUrl;
    //moc3
//    
//    int level;
//    int vip;
//    bool isRemove;
//    string phoneNumber;
//    
//    //
//    Text* txtSoDuKetSat;
//    string thanhtich;
//    ImageView* bgOldPass;
//    Text* txtOldpassLabel;
  
};

class ChangeAvatar : public Layer{
private:
    std::string url;
    SEL_CallFuncO mCallBackListener;
    Ref *mCallBack;
    Sprite* iconCheck;
    CircleAvatar* circleAvatar;
    bool isChoose;

public:
    ChangeAvatar(const string& url);
    ~ChangeAvatar();

    static ChangeAvatar* create(const string& url);

    bool init();
    void setIcon(string& url);
    string& getUrl () ;
    bool getIsChoose ();
    void visibleIconCheck(bool state) ;
    void onCallBack(Ref *target, SEL_CallFuncO listener);
    void onAvatarTouches(Ref* pSender, ui::Widget::TouchEventType type);
};

#endif // LayerPopupProfile_h__
