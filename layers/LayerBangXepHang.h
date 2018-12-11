//
//  LayerBangXepHang.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 7/20/17.
//
//

#ifndef LayerBangXepHang_hpp
#define LayerBangXepHang_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
USING_NS_CC;
USING_NS_CC_EXT;
#define MOBI 2
#define VINA 1
#define VIETTEL 3
using namespace std;
using namespace ui;
using namespace cocos2d;
class ItemBangXepHang : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,string name,string money);

    CREATE_FUNC(ItemBangXepHang);
private:
    Text* name;
    Text* money;
    Text* txtRank;

    ImageView* imgTop;
};
class LayerBangXepHang: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerBangXepHang();
    ~LayerBangXepHang();
    virtual bool init();
    
   
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTop(int gid, int typeroom);
    
    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerBangXepHang);
private:
    ui::ScrollView* scrollXepHang;
    struct item{
        string name;
        string money;
    };
    vector<item> lstXH;
    string lstop;
};


class ItemBangXepHangTo : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,string name,string money);
    
    CREATE_FUNC(ItemBangXepHangTo);
private:
    Text* name;
    Text* money;
    Text* txtRank;
    
    ImageView* imgTop;
    ImageView* bg;

};
class LayerBangXepHangTo: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerBangXepHangTo();
    ~LayerBangXepHangTo();
    virtual bool init();
    
    
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTop(int gid, int typeroom);
    void onBtnClose(Ref* pSender);
    void loadTopByType();
    void loadTopNoHuBauTom();
    void showTop(string str);

    //bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void onEnter() override;
    virtual void onExit() override;
    
    CREATE_FUNC(LayerBangXepHangTo);
private:
    ui::ScrollView* scrollXepHang;
    struct item{
        string name;
        string money;
    };
    vector<item> lstXH;
    string lstop;
    Text* txtNhan;
};
class ItemXepHangSlot : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,string time,string name,string money,string phien);
    CREATE_FUNC(ItemXepHangSlot);
private:
    Text* name;
    Text* time;
    Text* phien;
    Text* money;
    ImageView* bg;
};
class LayerPopupXepHangSlot: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupXepHangSlot();
    ~LayerPopupXepHangSlot();
    virtual bool init();
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTop();
    void onBtnClose(Ref* pSender);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerPopupXepHangSlot);
private:
    ui::ScrollView* scrollXepHang;
    struct itemxh{
        string name;
        string time;
        string money;
        string phien;
    };
    vector<itemxh> lstXH;
    string lstop;
    int type = 1;
};

class ItemNoHuBauTom : public Layer{
public:
    virtual bool init();
    void setDatas(int idx,string name,string moneyspend,string moneyget);
    CREATE_FUNC(ItemNoHuBauTom);
private:
    Text* txtSTT;
    Text* txtName;
    Text* txtMoneySpend;
    Text* txtMoneyGet;

};
class LayerPopupNoHuBauTom: public cocos2d::Layer, public PlayerCallBack{
public:
    LayerPopupNoHuBauTom();
    ~LayerPopupNoHuBauTom();
    virtual bool init();
    void loadTyGia();
    void OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent);
    void loadTop();
    void onBtnClose(Ref* pSender);
    void onBtnThang(Ref* pSender);
    void onBtnThua(Ref* pSender);
    void showNoHuThuong(string top);
    void onBtnLichSuNoHu(Ref* pSender);
    void setTimeNoHu(int time);
    void updateTimeMoQuy(float dt);
    void updateCountEffect(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(LayerPopupNoHuBauTom);
private:
    ui::ScrollView* scrollXepHang;
    struct itemxh{
        string name;
        string moneyspend;
        string moneyget;
    };
    vector<itemxh> lstXHWin;
    vector<itemxh> lstXHLose;

    string lstop;
    Button* btnThang;
    Button* btnThua;
    ImageView* imgheader;
    Text* txtLabelTimer;
    Text* txtTimer;
    Text* txtNoHu;
    int timeNoHu = 0;
    int timeEffect;
};
#endif /* LayerBangXepHang_hpp */
