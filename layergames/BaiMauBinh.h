//
//  BaiMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/20/17.
//
//

#ifndef BaiMauBinh_hpp
#define BaiMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "SimpleAudioEngine.h"
#include "Requests/ExtensionRequest.h"
#define SPECIAL_CARD_TYPE_SANH_RONG_CUON 1
#define SPECIAL_CARD_TYPE_SANH_RONG 2
#define SPECIAL_CARD_TYPE_LUC_PHE_BON 6
#define SPECIAL_5DOI_1SAM 5
#define SPECIAL_CARD_TYPE_3_CAI_SANH 8
#define SPECIAL_CARD_TYPE_3_CAI_THUNG 7
#define SPECIAL_CARD_TYPE_BINH_LUNG -1
#define GAME_CARD_TYPE_SAM_CHI_CUOI 14
#define GAME_CARD_TYPE_CU_LU_CHI_GIUA 15
#define GAME_CARD_TYPE_TU_QUY_CHI_DAU 16
#define GAME_CARD_TYPE_TU_QUY_CHI_GIUA 17
#define GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_DAU 18
#define GAME_CARD_TYPE_THUNG_PHA_SANH_CHI_GIUA 19
#define GAME_CARD_TYPE_MAU_THAU 20
#define GAME_CARD_TYPE_DOI 21
#define GAME_CARD_TYPE_THU 22
#define GAME_CARD_TYPE_SAM_CO 23
#define GAME_CARD_TYPE_SANH 24
#define GAME_CARD_TYPE_THUNG 25
#define GAME_CARD_TYPE_CU_LU 26
#define GAME_CARD_TYPE_TU_QUY 27
#define GAME_CARD_TYPE_THUNG_PHA_SANH 28

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class BaiMauBinh : public cocos2d::Layer
{
public:
    BaiMauBinh();
    ~BaiMauBinh();
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void dealCard(string listCard);
    string getSrcCard(string cardtype);
    void xoebai();
    void setIsme(bool isme);
    void updateStatus(bool isDone);
    void XepBaiAnimation();
    void reloadBoBai(string listCard);
    bool getIsme();
    void soChiEffect(int chi, string rs);
    string getStrBoBai(int code);
    void setChi(string chi);
    void resetGame();
    void bocuoc();
    void latbai();
    void LatBaiCuoiVan();
    void showCard();
    void swapAnimation(float dt);
    string getSoundPath(int code);
    void setPos(int _pos);
    void showChiAvatar(int chi);
    Vec2 getPosByID(int i);
    float getRotationXByID(int i);
    float getRotationYByID(int i);
    void reloadPos();
    void runActionXepBai(vector<Point> lstPoint);
    vector<Point> getListPos();
    vector<float> getListSkewX();
    vector<float> getListSkewY();
    string getListBoBai();

    CREATE_FUNC(BaiMauBinh);
private:
    vector<ImageView*> lstImage;
    vector<ImageView*> lstImageSochi;
    vector<Sprite*> lstCard;
    
    
    vector<Point> lstPointOrg;
    vector<float> lstSkewXOrg;
    vector<float> lstSkewYOrg;

    Node* rootNode;
    Text* txtStatus;
    bool isMe = false;
    ImageView* imgChi1;
    ImageView* imgChi2;
    ImageView* imgChi3;
    
    string currListCard = "";
    Text* txtSoChi;
    int currPos =0;
    bool isXepBai = false;

};
#include <string>
#include <sstream>

#if defined(__ANDROID__)
#define TO_STRING to_stringAndroid

template <typename T>
inline std::string to_stringAndroid(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}


#else

#define TO_STRING std::to_string

#endif

#endif /* BaiMauBinh_hpp */
