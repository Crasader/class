#ifndef __POPUP_CHON_TIEN_LIENG__
#define __POPUP_CHON_TIEN_LIENG__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "../GameServer.h"
#include "../layergames/LayerPlayGameTXMini.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerPopupChonTienLieng : public cocos2d::Layer,public PlayerCallBack
{
public:
	LayerPopupChonTienLieng();
	~LayerPopupChonTienLieng();
    virtual bool init();
    
    bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void OnSmartFoxRoomJoin(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);

    void onBtnOK(Ref* pSender);
    void onBtnCancel(Ref* pSender);
    void onBtnAutoCheckChargeMoney(Ref* pSender);
    void sliderEvent(Ref *pSender, Slider::EventType type);
    float formatValue(float value);
    void setMinMaxValue(float muccuoc, float muctoithieuvaoban);
    void setJoinType(int type);
    
    string getTime(string str);
    
    virtual void onEnter() override;
    virtual void onExit() override;

	void onBtnAddPercent(Ref* pSender);
	void onBtnSubPercent(Ref* pSender);
	void displayMoney(int percent);

	CREATE_FUNC(LayerPopupChonTienLieng);
private:
    Text* lblBacMangVao;
    Text* lblMinMoney;
    Text* lblMaxMoney;
	Text* lblBacConLai;
	Text* lblBetRoom;
    
    ImageView* imgAutoChargeMoney;
    
    Slider* slider;
    float maxValue = 0;
    float minValue = 0;
    float currBacMangVao = 0;
    float bacMangVao = 0;
    
    float myMoney;
    int currentJoinType = 0; // 0 = choi bt, 1 = choi ngay
    int betValue= 0;
    int isAutoChargeMoney = 0;
};


#endif // __POPUP_CHON_TIEN_LIENG__