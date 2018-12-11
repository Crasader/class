//
//  BangCuocMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/2/17.
//
//

#ifndef BangCuocMauBinh_hpp
#define BangCuocMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;

class BangCuocMauBinh : public cocos2d::Layer{
public:
    BangCuocMauBinh();
    ~BangCuocMauBinh();
    virtual bool init();
    
    void onBtnMucCuoc(Ref* pSender);
    void reloadValueCuoc(int cuoc1,int cuoc2,int cuoc3);
    int loadSapXepCuoc(int cuoc);
    void hideBangCuoc();
    void showBoBai();
    CREATE_FUNC(BangCuocMauBinh);
private:
    ImageView* imgBangCuoc;
    vector<Text*> lstCuoc;
    //ImageView* pnlBg;
    ImageView*  btnBai;
    bool  isShowBoBai= false;
    Node* pnlBg;
    
    
};

#endif /* BangCuocMauBinh_hpp */
