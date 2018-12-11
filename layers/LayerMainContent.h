#ifndef __iCasino_v4__LayerMainMain__
#define __iCasino_v4__LayerMainMain__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/UIPageView.h"
#include <vector>
#include "GameServer.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d::ui;

class LayerMainContentNew :public cocos2d::Layer, public PlayerCallBack{
public:
	LayerMainContentNew();
	~LayerMainContentNew();

    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	virtual bool init();
    void loadHu(float dt);
    void reloadLang();
	CREATE_FUNC(LayerMainContentNew);
private:
	struct iconStruct{
		int gameID;
		bool state;
		string imgNormalPath;
		string imgDisablePath;
		iconStruct(int _gameid, bool _state, string _imgNormal, string _imgDisable) : 
			gameID(_gameid), state(_state), imgNormalPath(_imgNormal), imgDisablePath(_imgDisable){}
	};
	vector<iconStruct> lstIcons;
	PageView* pvMain;
	Button *btnArrow1, *btnArrow2;
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
	void onButtonArrow1(Ref* pSender, ui::Widget::TouchEventType pType);
	void onButtonArrow2(Ref* pSender, ui::Widget::TouchEventType pType);
	void createListData();
    void onBtnNapTien(Ref* pSender);
	void onButtonGame(Ref* pSender, ui::Widget::TouchEventType pType);
	void pageViewEvent(Ref *pSender, ui::PageView::EventType type);
	void createScrollView();
    void getDiscount(float dt);
    void showNapTien(float dt);
    void tangHuTX(float dt);
    void tangHuPK(float dt);
    void tangHuCT(float dt);
    void onScheduleThongBao(float dt);

    void hieuUngHuThuong();
    void createPageView();
    void gotoGameMini(int tag);
    ui::ScrollView* scrollView;
    
    Text* txtHuTX;
    Text* txtHuCT;
    Text* txtHuPK;
    Text* txtHuVQ;
    Layout* pnlBG;
    //132_true;130_true;134_true;129_true
    int kGameVQMM = 132;
    int kGameMNPK = 130;
    int kGameXDMN = 134;
    int kGameCT = 129;
    int kGameTX = 128;
    
    int huPoker = 1457909;
    int huTaiXiu = 1234987;
    
    int fakeHuTX = 623471;
    int fakePoker = 629812;
    Button* btnNapTien;
    Layout* pnlTX;
    Layout* pnlPK;
    Layout* pnlCT;
};
#endif //__iCasino_v2__LayerMainTop__

