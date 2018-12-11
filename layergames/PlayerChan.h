#ifndef __ICASINO_CHAN_PLAYER___
#define __ICASINO_CHAN_PLAYER___

#include <vector>
#include "CardChan.h"
#include "ChanUtils.h"

using namespace std;

struct DanhBaiData{
    bool flag;
    int id;
    int cardnu;
    int cardsu;
    int beforeAction; //1 = minh vua chiu, 2 = minh vua an
    int frompos;
    std::string fromuser;
    DanhBaiData(){
        flag = false;
        id = -1;
        cardnu = -1;
        cardsu = -1;
        beforeAction = -1;
        frompos = -1;
        fromuser = "";
    }
};

enum{
    CLIENT_RC_FAILED = -1,
    CLIENT_RC_OK = 0,
    CLIENT_RC_CHUA_CHON_LABAI_DANH = 1,
    CLIENT_RC_CHUA_CHON_LABAI_CHIU = 2,
    CLIENT_RC_CHUA_CHON_LABAI_AN = 3,
    CLIENT_RC_LASTROOM_NULL = 4,
    CLIENT_RC_CARD_NOT_IN_HAND = 5,
    CLIENT_RC_ACTION_PENDING = 6, // action đang được thực hiện, chờ xác nhận.
    CLIENT_RC_AN_KHONG_DUNG_CHAN_CA = 7
};

class PlayerChan{
protected:
    float duration;

    // Nhung nguoi choi xung quanh.
    //! Cac object nay duoc huy trong layer card chan. Do not modify
    PlayerChan* PlayerLeft;
    PlayerChan* PlayerRight;
    PlayerChan* PlayerTop;

    std::string aI;

    float X_CardCuaChi;
    float Y_CardCuaChi;
    float X_CardDuoiTay;
    float Y_CardDuoiTay;
    float X_BeginVisible;
    float Y_BeginVisible;
    float Distance;
    int CountChiu;
    int Singe;
    int SingeBot;

    // Xac dinh xem nguoi nay co choi hay khong.
    bool IsPlayer;

    std::vector<CardChan*> BaiTrenTay;
    std::vector<CardChan*> BaiCuaChi;
    std::vector<CardChan*> BaiDuoiTay1;
    std::vector<CardChan*> BaiDuoiTay2;

    // Luu con bai danh ra
    DanhBaiData pDisCards;
    // Luu con bai an
    DanhBaiData pEateCards;
    // Luu con bai chuyen xuong
    DanhBaiData pTranferCards;

    // Thêm mới
    std::string sListOnHand;
    std::string sListCuaChi;
    std::string sListDuoiTay;

public:
    PlayerChan();
    ~PlayerChan();

    //
    virtual void Reset();
    void HideCardCuaChi();
    void HideCardInList(vector<CardChan*>& P);

    //Add Cards
    void AddCardToBaiTrenTay(CardChan* pCard);
    void AddCardToCuaChi(CardChan* pCard);
    void AddCardToDuoiTay1(CardChan* pCard);
    void AddCardToDuoiTay2(CardChan* pCard);

    cocos2d::Vec2 PosCardCuaChi();
    cocos2d::Vec2 GetPosBegin();
    cocos2d::Vec2 PosCardDuoiTay1();
    cocos2d::Vec2 PosCardDuoiTay2();

    //Position When Chiu (Three cards vertial)
    cocos2d::Vec2 PosChiu();

    //
    void SwapZorder(bool type);
    void QuicksortZOder(vector<CardChan*>& A, int l, int r);
    void RefreshListCards();

    //
    void ResortCardCuaChi(Ref* sender, PlayerChan* player);

    //Di Chuyen bai cua tri xuong phia duoi mot chut khi U
    void MoveCardCuaChi();

    void RotateCardWhenFinish(vector<CardChan*>& P);

    void ActionAnCuaTrenByServer(const CardOrg& card);
    void ActionAnCuaChiByServer(const CardOrg& card);
    void ActionChiuBaiByServer(const CardOrg& card);

    CardChan* GetCardTakeFromPlayer(const CardOrg& card);
    CardChan* GetCardChiuFromPlayer(const CardOrg& card);

    //
    PlayerChan* GetPlayer(const std::string& aI);

    // Kiem tra xem bai cua chi co dang running hay khong.
    bool CheckingRunningCuaChi();

    // Cac thao tac bai tren tay
    virtual void ActionDiscardByServer(const CardOrg& card) = 0;
    virtual void ActionTraCuaByServer(const CardOrg& card) = 0;
    virtual void ActionChuyenBaiByServer(const CardOrg& card) = 0;
    virtual void ActionChuyenBaiChiuByServer(const CardOrg& card) = 0;

    virtual int ActionDisCardByClient() = 0;
    virtual int ActionChiuCardByClient() = 0;
    virtual int ActionTakeCardByClient(const CardOrg& lastCard) = 0;

    virtual int SetListCards(std::string& listcards) = 0;

    //Getter and Setter
    void SetOtherPlayer(PlayerChan* right, PlayerChan* top, PlayerChan* left);
    void SetPlayerLeft(PlayerChan* player);
    void SetPlayerTop(PlayerChan* player);
    void SetPlayerRight(PlayerChan* player);

    PlayerChan* GetPlayerLeft() const;
    PlayerChan* GetPlayerRight() const;
    PlayerChan* GetPlayerTop() const;

    void SetAI(const string& aI);
    const string& GetAI() const;

    void SetPosCardCuaChi(const float& x, const float& y);
    void SetPosCardDuoiTay(const float& x, const float& y);
    void SetPosBeginVisible(const float& x, const float& y);

    void SetSigne(int cuachi, int duoitay);

    void SetIsPlayer(bool isplayer);
    bool GetIsPlayer() const;

    int GetCountCardTrenTay() const;

    //
    void ClearAIOwner();

    //Set List Cards for player
    virtual void SetPlayerListCards(std::string& lstCards, bool hasU = false);

    //
    bool CheckCardRunning();
    bool checkRunningInArray(vector<CardChan*>& P);

    virtual void RePainCuaChi();
    virtual void RePainDuoiTay();
    virtual void RePainTrenTay();

private:
    vector<RePainObj> PrepareDuoiTay();
};

#endif
