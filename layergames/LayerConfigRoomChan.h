#ifndef ___LayerConfigRoomChan_H___
#define ___LayerConfigRoomChan_H___
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocosbuilder/CocosBuilder.h"
#include "TextInputTest.h"
#include "mUtils.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace cocos2d::extension;
using namespace cocosbuilder;

struct ConfigRoomChan
{
	bool checkAnBaoWhenU;
	bool checkULao;
	bool choiNuoiGa;
	bool choiUXuong;
	int choiDiemToiThieu;
	string koChoiCacCuocSac;
	int playerReadyTimeout;
	int discardTimeout	;
	int turnTimeout;
	int expectingUTimeout;
	int xuongCuocSacTimeout;
	int stepTimeout	;
	int waitForChiuTimeout;
	ConfigRoomChan()
		:checkAnBaoWhenU(false),
		checkULao(false),
		choiNuoiGa(false),
		choiUXuong(false),
		choiDiemToiThieu(0),
		koChoiCacCuocSac(""),
		playerReadyTimeout(60),
		discardTimeout(30),
		turnTimeout(90),
		expectingUTimeout(10),
		xuongCuocSacTimeout(60),
		stepTimeout(30),
        waitForChiuTimeout(5){ }
};

class LayerConfigRoomChan : public Layer
{
public:
	LayerConfigRoomChan(const bool& isBoss);
	~LayerConfigRoomChan();
	static LayerConfigRoomChan* create(const bool& isBoss);
	bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	void menuItemCallback(Ref* pSender);
    void OnCheckBoxClick(Ref* pSend, CheckBox::EventType type);
	void OnOptionClick(Ref* pSend);

	MenuItemImage* CreateButton(const char* mName, const int& mTag, const bool& isBoss = false);
	string	CreateStringConfig(const ConfigRoomChan& config);

    void SendRequestConfig();

    void setParentBG(ImageView* bg);

private:
	bool IsBossRoom;
    ImageView* _bgParent;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    TextFieldTTFDefaultTest* txtTimeoutStep;
#else
	ui::EditBox* txtTimeoutStep;
#endif

	CheckBox* cbxNuoiGa;
	CheckBox* cbxChoiUXuong;
	CheckBox* cbxChoiDuoi4Diem;

	vector<CheckBox*> ArrOption;
	cocos2d::EventListener* _touchListener;
};

#endif
