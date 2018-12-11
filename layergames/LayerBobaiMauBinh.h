//
//  LayerBobaiMauBinh.hpp
//  iCasino_v4
//
//  Created by Sherlock Tuan on 4/20/17.
//
//

#ifndef LayerBobaiMauBinh_hpp
#define LayerBobaiMauBinh_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GameServer.h"
#include "Requests/ExtensionRequest.h"
#include "BaiMauBinh.h"
#include "MauBinhLayerXepBai.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace ui;
using namespace cocos2d;
class LayerBoBaiMauBinh : public cocos2d::Layer
{
public:
    LayerBoBaiMauBinh();
    ~LayerBoBaiMauBinh();
    virtual bool init();
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void actionDealCard(string list);
    BaiMauBinh* getBoBaiByPos(int pos);
    string convertListCard(string lst);
    string convertListXepBai(string lst);
    bool IsMee(string name);
    void resetGame();
    void XoeBoBai(int pos);
    
    void giveCardByPos(int pos);
    void showXepBaiMauBinh();
    void setMyListIDCard(string lst);
    void showMyListCard();
    void latBai(string lst);
    void showSochi(string str);
    void hideXepBaiMauBinh();
    void setTimeXepBai(int timxep);
    void startTimerAvatar(int pos);
    void SoChiByPos(int pos,int chi,string ketqua);
    void showKetQua(string str);
    void soBaChi(string str);
    bool isAnTrang(int code);
    void resetGameByPos(int pos);
    void LatBai(int pos);
    void setIsXepBai(bool _xepbai);
    bool getIsXepBai();
    void beginSoChi();
    void playSound(int type);
    void XoaBaiChia();
    void removeTimer();
    void runTimerReady(int _time);
    void hideClock();
    void setIsSpector(bool _is);
    void LatBaiHoa(string lstcard);
    CREATE_FUNC(LayerBoBaiMauBinh);

	//update 11/10
	void setCardRejoin(string listcard);
private:
    bool isMe = false;
    string myListCard = "";
    string myListIDCard = "";
    struct ketqua{
        string name;
        string baidacbiet;
        string baiantrang;
        string baicacchi;
        string chimoibai;
        string tongtien;
        string sapho;
        string saplang;
        string binhlung;
        string chibinhlung;

        bool sochi;
        bool isBinhLung;
        bool isAnTrang;

    };
    vector <ketqua> lstketqua;
    int tag_xep_bai = 150;
    int tag_ket_qua = 250;
    int timeXepBai = 0;
    Label* txtSoChi;
    string strketqua = "";
    bool _isAnTrang = false;
    bool isXepBai = false;
    bool _isAnTrangThat = false;
    double muccuoc = 0;
    ImageView* clock;
    Label* txtTimer;
    bool isSpector = false;
    ImageView* imgTime;
    MauBinhLayerXepBai* xepbai;
    int tag_thang_trang = 908;

};


#endif /* LayerBobaiMauBinh_hpp */
