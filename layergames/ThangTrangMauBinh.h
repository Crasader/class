//
//  ThangTrangMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 6/2/17.
//
//

#ifndef ThangTrangMauBinh_hpp
#define ThangTrangMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;

class ThangTrangMauBinh : public cocos2d::Layer{
public:
    ThangTrangMauBinh();
    ~ThangTrangMauBinh();
    virtual bool init();
    
    void setThangTrangType(int type);
    void showBoBai();
    void setName(string name);
    void setBoBai(string _lstCard);
    string getSrcCard(string cardtype);
    CREATE_FUNC(ThangTrangMauBinh);
private:
    ImageView* imgResult;
    Text* txtName;
    string lstCard;
    Layout* pnlType1;
    Layout* pnlType2;
    vector<Sprite*> lstSpr1;
    vector<Sprite*> lstSpr2;

};

#endif /* ThangTrangMauBinh_hpp */
