#ifndef __iCasinov2__ChanUtils__
#define __iCasinov2__ChanUtils__

#include "ChanClientMessageDef.h"
#include "GameServer.h"
#include "AllData.h"
#include "cocos2d.h"
#include "mUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include <boost/algorithm/string/trim.hpp>

#define SIZE_FONT_CHAN 33
#define W_CARD 56.125
#define H_CARD 213.25
#define W_CARD_HAND 75.25
#define H_CARD_HAND 270.625
#define GOC 12.157894737
#define betweentb 60

enum class PlayingState{
    BEGIN = 1,
    DISCARD = 2,
    DRAWCARD = 3,
    TRACUA = 4,
    ANBAI = 5,
    CHIUBAI = 6,
    CHO_U = 7,
};

enum class CardState{
    OK = 0,
    FAILED = 1,
    BEGIN = 2,
    CANCELLED = 3,
    PENDING = 4,
    END = 5,
};

class CardOrg {
public:
    std::string fuser;
    std::string tuser;
    int fpos;
    int tpos;
    int cardnu;
    int cardsu;
    int crdorg;
    int count;
    CardOrg():fuser(""), tuser(""), fpos(-1), tpos(-1), cardnu(-1), cardsu(-1), crdorg(-1), count(1) {

    }

    CardOrg(const std::string& fuser, const std::string& tuser, const int& fpos, const int& tpos, const int& cardnu, const int& cardsu, const int& crdorg, const int& count)
        :fuser(fuser),
         tuser(tuser),
         fpos(fpos),
         tpos(tpos),
         cardnu(cardnu),
         cardsu(cardsu),
         crdorg(crdorg),
         count(count) {
    }

    ~CardOrg(){
        //cocos2d::log("Card Origination Destructor !!!");
    }

    CardOrg(const CardOrg& p) {
        tuser = p.tuser;
        fuser = p.fuser;
        fpos = p.fpos;
        tpos = p.tpos;
        cardnu = p.cardnu;
        cardsu = p.cardsu;
        crdorg = p.crdorg;
        count = p.count;
    }

    CardOrg & operator = (const CardOrg &p) {
        tuser = p.tuser;
        fuser = p.fuser;
        fpos = p.fpos;
        tpos = p.tpos;
        cardnu = p.cardnu;
        cardsu = p.cardsu;
        crdorg = p.crdorg;
        count = p.count;
        return *this;
    }

    void reset(){
        fuser = "";
        tuser = "";
        fpos = -1;
        tpos = -1;
        cardnu = -1;
        cardsu = -1;
        crdorg = -1;
        count = 1;
    }

    const void show() const {
        ////cocos2d::log("from %s to %s, fpos %s to : tpos %s", fuser.c_str(), tuser.c_str(), get(fpos), get(tpos));
    }

    const char* get(const int& pos) const {
        switch (pos) {
        case kUserMe:
            return "kUserMe";
        case kUserLeft:
            return "kUserLeft";
        case kUserRight:
            return "kUserRight";
        case kUserTop:
            return "kUserTop";
        case kUserBot:
            return "kUserBot";
        }
        return "unknown";
    }
};

struct CardDetail{
    int Chan;
    int Ca;
    int BaDau;
    int Que;
    CardDetail(){
        Chan = 0;
        Ca = 0;
        BaDau = 0;
        Que = 0;
    }
    void reset(){
        Chan = 0;
        Ca = 0;
        BaDau = 0;
        Que = 0;
    }
};

struct PlayerChanInfo{
    int pos;
    std::string aI;
    std::string name;
    double money;
    std::string aal;
    bool isReady;
    bool isBoss;
    int vip;
    PlayerChanInfo():pos(-1), aI(""), name(""), money(0), aal(""), isReady(false), isBoss(false), vip(0){}
    ~PlayerChanInfo(){
        //cocos2d::log("Destructor PlayerChanInfo");
    }
};

struct CuocSac {
    std::string ten;
    int nhom;
    int uutien;

    CuocSac(const std::string& _ten, const int& _nhom, const int& _uutien){
        ten = _ten;
        nhom = _nhom;
        uutien = _uutien;
    }

    CuocSac& operator =(const CuocSac& a)
    {
        ten = a.ten;
        nhom = a.nhom;
        uutien = a.uutien;
        return *this;
    }

    CuocSac(){
        ten = "";
        nhom = 0;
        uutien = 0;
    }
};

struct CurrencyStr{
    double valueStruct;
    string symbolStruct;
    CurrencyStr(){
        valueStruct = 0;
        symbolStruct = "";
    }
};

#define CHIUCARD 1
#define CHANCA 0

struct RePainObj {
    std::string key;
    std::vector<std::string> arr;
    int type;
    RePainObj(){
        key = "";
        type = CHANCA;
    }
};

struct InfoCardU {
    std::string uid;
    std::string cardu;
    InfoCardU(){
        uid = "";
        cardu = "";
    }
};

class ChanUtils {
public:
    static std::string GetStringAnBao(const long& reson) {
        switch(reson) {
        case ANBAO_REASON_NO_PROBLEM:
            return "";
        case ANBAO_REASON_AN_CA_DOI_CHO:
            return dataManager.GetSysString(480);
        case ANBAO_REASON_BO_AN_CUATREN_BUT_AN_CUATRI:
            return "Không ăn cửa trên nhưng lại ăn cửa trì";
        case ANBAO_REASON_DISCARD_SAMEAS_CUATREN_CUATRI_DUOITAY:
            return dataManager.GetSysString(573);
        case ANBAO_REASON_ANCA_DANHCA:
            return dataManager.GetSysString(481);
        case ANBAO_TREOTRANH:
            return dataManager.GetSysString(100);
        case ANBAO_DANH_1_CA_CHI_DUOC_AN_CHAN:
            return dataManager.GetSysString(574);
        case ANBAO_REASON_BOCHAN_ANCA:
            return dataManager.GetSysString(492);
        case ANBAO_REASON_DANHBAI_GIONG_CHANCA_DA_AN:
            return dataManager.GetSysString(575);
        case ANBAO_REASON_DOI_U_BACHTHUCHI:
            return dataManager.GetSysString(576);
        case ANBAO_REASON_BOCHAN_DANHCHAN:
            return dataManager.GetSysString(493);
        case ANBAO_REASON_BOCHAN_ANCHAN:
            return dataManager.GetSysString(494);
        case ANBAO_REASON_BOCA_ANCA:
            return dataManager.GetSysString(495);
        case ANBAO_REASON_DANHCA_ANCA:
            return dataManager.GetSysString(577);
        case ANBAO_REASON_XECA_ANCA:
            return dataManager.GetSysString(703);
        case ANBAO_REASON_XECHAN_ANCA:
            return dataManager.GetSysString(704);
        case ANBAO_REASON_DANH_ROILAI_AN:
            return dataManager.GetSysString(578);
        case ANBAO_REASON_DANH_DI_DOI_CHAN:
            return dataManager.GetSysString(579);
        case ANBAO_REASON_AN_ROILAI_DANH:
            return dataManager.GetSysString(482);
        case ANBAO_REASON_ANCA_ROILAI_DANH_QUAN_CUNG_HANG:
            return dataManager.GetSysString(483);
        case ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG:
            return dataManager.GetSysString(538);
        case ANBAO_REASON_AN_CHON_CA:
            return dataManager.GetSysString(484);
        case ANBAO_REASON_CO_CHAN_CAU_CA:
            return dataManager.GetSysString(539);
        case ANBAO_REASON_U_NHUNG_KHONG_XUONG:
            return dataManager.GetSysString(540);
        case ANBAO_REASON_HO_U_LAO:
            return dataManager.GetSysString(711);
        case ANBAO_REASON_XUONG_SAI_CUOC:
            return dataManager.GetSysString(705);
        case ANBAO_REASON_U_BAO:
            return dataManager.GetSysString(706);
        case ANBAO_REASON_BO_U_ROI_LAI_U:
            return dataManager.GetSysString(496);
        case ANBAO_REASON_ANCA_ROILAI_AN_QUAN_CUNG_HANG:
            return dataManager.GetSysString(485);
        }
        return "";
    }

    static std::string GetNameCuocSacById(const std::string& idx) {
        int idx_ = atoi(idx.c_str());
        return GetNameCuocSacById(idx_);
    }

    static std::string GetNameCuocSacById(const int& idx) {
        switch(idx) {
        case U_XUONG:
            return dataManager.GetSysString(707);
        case U_THONG:
            return dataManager.GetSysString(661);
        case U_CHI:
            return dataManager.GetSysString(541);
        case U_THIEN_U:
            return dataManager.GetSysString(662);
        case U_DIA_U:
            return dataManager.GetSysString(580);
        case U_TOM:
            return dataManager.GetSysString(662);
        case U_LEO:
            return dataManager.GetSysString(664);
        case U_BACH_DINH:
            return dataManager.GetSysString(497);
        case U_TAM_DO:
            return dataManager.GetSysString(665);
        case U_KINH_TU_CHI:
            return dataManager.GetSysString(667);
        case U_THAP_THANH:
            return dataManager.GetSysString(666);
        case U_CO_THIEN_KHAI:
            return dataManager.GetSysString(542);
        case U_AN_BON:
            return dataManager.GetSysString(543);
        case U_BON:
            return dataManager.GetSysString(712);
        case U_CO_CHIU:
            return dataManager.GetSysString(544);
        case U_CHIU_U:
            return dataManager.GetSysString(545);
        case U_BACH_THU:
            return dataManager.GetSysString(498);
        case U_HOA_ROI_CUA_PHAT:
            return dataManager.GetSysString(604);
        case U_NLXH_HRCP:
            return dataManager.GetSysString(625);
        case U_CA_LOI_SAN_DINH:
            return dataManager.GetSysString(546);
        case U_CA_NHAY_DAU_THUYEN:
            return dataManager.GetSysString(547);
        case U_CHUA_DO_NAT_HOA:
            return dataManager.GetSysString(548);
        case U_DOI_LEO:
            return dataManager.GetSysString(605);
        case U_DOI_TAM_DO:
            return dataManager.GetSysString(581);
        case U_DOI_TOM:
            return dataManager.GetSysString(582);
        case U_BACH_THU_CHI:
            return dataManager.GetSysString(499);
        case U_AN_2_BON:
            return dataManager.GetSysString(606);
        case U_AN_3_BON:
            return dataManager.GetSysString(500);
        case U_AN_4_BON:
            return dataManager.GetSysString(501);
        case U_TOM_3:
            return dataManager.GetSysString(502);
        case U_TOM_4:
            return dataManager.GetSysString(503);
        case U_CO_2_CHIU:
            return dataManager.GetSysString(607);
        case U_CO_3_CHIU:
            return dataManager.GetSysString(504);
        case U_CO_4_CHIU:
            return dataManager.GetSysString(505);
        case U_CO_2_THIEN_KHAI:
            return dataManager.GetSysString(608);
        case U_CO_3_THIEN_KHAI:
            return dataManager.GetSysString(506);
        case U_CO_4_THIEN_KHAI:
            return dataManager.GetSysString(507);
        case U_LEO_3:
            return dataManager.GetSysString(508);
        case U_LEO_4:
            return dataManager.GetSysString(509);
        }
        return "";
    }

    static std::string GetNamCuocCoGa(const std::string& cuoc_ga) {
        int Cuoc = atoi(cuoc_ga.c_str());
        switch(Cuoc) {
        case GA_U_BON_BACH_THU:
            return dataManager.GetSysString(713);
        case GA_U_BON_BACH_THU_CHI:
            return dataManager.GetSysString(714);
        case GA_THAP_THANH:
            return dataManager.GetSysString(666);
        case GA_KINH_TU_CHI:
            return dataManager.GetSysString(667);
        case GA_BACH_DINH:
            return dataManager.GetSysString(497);
        case GA_TAM_DO:
            return dataManager.GetSysString(665);
        case GA_BACH_THU_CHI:
            return dataManager.GetSysString(499);
        case GA_HOA_ROI_CUA_PHAT:
            return dataManager.GetSysString(604);
        case GA_CA_LOI_SAN_DINH:
            return dataManager.GetSysString(546);
        case GA_NLXH_HRCP:
            return dataManager.GetSysString(625);
        case GA_CHI_BACH_THU:
            return dataManager.GetSysString(549);
        case GA_CHI_BACH_THU_CHI:
            return dataManager.GetSysString(550);
        case GA_NGU_ONG_BAT_CA:
            return dataManager.GetSysString(626);
        }
        return "";
    }

    static std::string GetNamePlayer(const string& uid) {
		boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(uid);
//         boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
//         if (room == NULL)
//             return uid;
       // boost::shared_ptr<User> user = room->GetUserByName(uid);
        if (user == NULL)
            return uid;
        boost::shared_ptr<UserVariable> usrVar = user->GetVariable("adn");
        if(usrVar == NULL)
            return uid;
        boost::shared_ptr<string> name = usrVar->GetStringValue();

        try{
            std::string _name = *name;
            boost::algorithm::trim(_name);
            if(0 == _name.compare(""))
                return uid;
        }catch(...){
            return uid;
        }

        return *name;
    }

    static PlayerChanInfo GetPlayerInfo(const string& uid) {
        PlayerChanInfo ret;

        boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        if (room == NULL)
            return ret;

        try {
            ret.aI = uid;
            ret.name = uid;

            boost::shared_ptr<User> user = room->GetUserByName(uid);
            if (user != NULL)
            {
                boost::shared_ptr<UserVariable> usrVar = user->GetVariable("adn");
                if(usrVar != NULL){
                    ret.name = *usrVar->GetStringValue();
                    boost::algorithm::trim(ret.name);
                    if(ret.name == "")
                        ret.name = ret.aI;
                }

                if (SceneManager::getSingleton().getGameID() == kGameXiTo){
                    usrVar = user->GetVariable(EXT_FIELD_GAME_CHIP);
                    if(usrVar != NULL){
                        ret.money = *usrVar->GetDoubleValue();
                    }
                }
                else{
                    
                    if (SceneManager::getSingleton().getCurrRoomType() == 0)
                    {
                        usrVar = user->GetVariable(EXT_VAL_AMOUNT_BALANCE);
                        if(usrVar != NULL){
                            ret.money = *usrVar->GetDoubleValue();
                        }
                    }else{
                        usrVar = user->GetVariable("amfs");
                        if(usrVar != NULL){
                            ret.money = *usrVar->GetDoubleValue();
                        }
                    }
                    
                }
               

                usrVar = user->GetVariable(EXT_VAL_AMOUNT_AVATAR_LINK);
                if(usrVar != NULL){
                    ret.aal = *usrVar->GetStringValue();
                }

                usrVar = user->GetVariable("accVipType");
                if (usrVar != NULL) {
                    ret.vip = (int)*usrVar->GetIntValue();
                }

            }
        } catch (...) {
           // log("exception !");
        }
        return ret;
    }


    static cocos2d::Size getSizePos() {
        cocos2d::Size size = cocos2d::Director::getInstance()->getVisibleSize();

        float scaleX = size.width / WIDTH_DESIGN;
        float scaleY = size.height / HEIGHT_DESIGN;

        float scaleMin = (scaleX < scaleY) ? scaleX : scaleY;
        float scaleMax = (scaleX > scaleY) ? scaleX : scaleY;

        float posXAdd = 0;
        float posYAdd = 0;

        if (scaleMax == scaleX) {
            posXAdd = ((size.width - WIDTH_DESIGN * scaleMin) / 2) / scaleMin;
            posYAdd = 0;
        }
        if (scaleMax == scaleY) {
            posXAdd = 0;
            posYAdd = ((size.height - HEIGHT_DESIGN * scaleMin) / 2) / scaleMin;
        }

        return cocos2d::Size(posXAdd, posYAdd);
//        return Size(0, 0);
    }

    static std::string FindTypeCard(const int& number, const int& suite) {
        return (boost::to_string(number) + "_" + boost::to_string(suite) + ".png");
    }

    static std::string FindTypeCard(const string& number, const string& suite) {
        return (number + "_" + suite + ".png");
    }

    static CuocSac GetNameCuocSacById_New(const int& idx) {
        CuocSac cuoc;
        switch (idx) {
            case U_XUONG:
                cuoc = CuocSac(dataManager.GetSysString(707), 0, 0);
                break;

            case U_THONG:
                cuoc = CuocSac(dataManager.GetSysString(661), 1, 1);
                break;

            case U_CHI:
                cuoc = CuocSac(dataManager.GetSysString(541), 2, 2);
                break;

            case U_THIEN_U:
                cuoc = CuocSac(dataManager.GetSysString(662), 3, 3);
                break;

            case U_DIA_U:
                cuoc = CuocSac(dataManager.GetSysString(580), 3, 4);
                break;

            case U_TOM:
                cuoc = CuocSac(dataManager.GetSysString(662), 8, 16);
                break;
            case U_LEO:
                cuoc = CuocSac(dataManager.GetSysString(664), 7, 13);
                break;

            case U_BACH_DINH:
                cuoc = CuocSac(dataManager.GetSysString(497), 6, 11);
                break;

            case U_TAM_DO:
                cuoc = CuocSac(dataManager.GetSysString(665), 6, 12);
                break;

            case U_KINH_TU_CHI:
                cuoc = CuocSac(dataManager.GetSysString(667), 5, 10);
                break;

            case U_THAP_THANH:
                cuoc = CuocSac(dataManager.GetSysString(666), 4, 9);
                break;

            case U_CO_THIEN_KHAI:
                cuoc = CuocSac(dataManager.GetSysString(542), 9, 15);
                break;

            case U_AN_BON:
                cuoc = CuocSac(dataManager.GetSysString(543), 11, 17);
                break;

            case U_BON:
                cuoc = CuocSac(dataManager.GetSysString(712), 3, 6);
                break;

            case U_CO_CHIU:
                cuoc = CuocSac(dataManager.GetSysString(544), 10, 16);
                break;

            case U_CHIU_U:
                cuoc = CuocSac(dataManager.GetSysString(545), 3, 5);
                break;

            case U_BACH_THU:
                cuoc = CuocSac(dataManager.GetSysString(498), 4, 7);
                break;

            case U_HOA_ROI_CUA_PHAT:
                cuoc = CuocSac(dataManager.GetSysString(604), 2, 2);
                break;

            case U_NLXH_HRCP:
                cuoc = CuocSac(dataManager.GetSysString(625), 2, 2);
                break;

            case U_CA_LOI_SAN_DINH:
                cuoc = CuocSac(dataManager.GetSysString(546), 2, 2);
                break;

            case U_CA_NHAY_DAU_THUYEN:
                cuoc = CuocSac(dataManager.GetSysString(547), 2, 2);
                break;

            case U_CHUA_DO_NAT_HOA:
                cuoc = CuocSac(dataManager.GetSysString(548), 2, 2);
                break;

            case U_DOI_LEO:
                cuoc = CuocSac(dataManager.GetSysString(605), 7, 13);
                break;

            case U_DOI_TAM_DO:
                cuoc = CuocSac(dataManager.GetSysString(581), 6, 12);
                break;

            case U_DOI_TOM:
                cuoc = CuocSac(dataManager.GetSysString(609), 8, 14);
                break;

            case U_BACH_THU_CHI:
                cuoc = CuocSac(dataManager.GetSysString(499), 4, 8);
                break;

            case U_AN_2_BON:
                cuoc = CuocSac(dataManager.GetSysString(606), 11, 17);
                break;

            case U_AN_3_BON:
                cuoc = CuocSac(dataManager.GetSysString(500), 11, 17);
                break;

            case U_AN_4_BON:
                cuoc = CuocSac(dataManager.GetSysString(501), 11, 17);
                break;

            case U_TOM_3:
                cuoc = CuocSac(dataManager.GetSysString(502), 8, 14);
                break;

            case U_TOM_4:
                cuoc = CuocSac(dataManager.GetSysString(503), 8, 14);
                break;

            case U_CO_2_CHIU:
                cuoc = CuocSac(dataManager.GetSysString(607), 10, 16);
                break;

            case U_CO_3_CHIU:
                cuoc = CuocSac(dataManager.GetSysString(504), 10, 16);
                break;

            case U_CO_4_CHIU:
                cuoc = CuocSac(dataManager.GetSysString(505), 10, 16);
                break;

            case U_CO_2_THIEN_KHAI:
                cuoc = CuocSac(dataManager.GetSysString(608), 9, 15);
                break;

            case U_CO_3_THIEN_KHAI:
                cuoc = CuocSac(dataManager.GetSysString(506), 9, 15);
                break;

            case U_CO_4_THIEN_KHAI:
                cuoc = CuocSac(dataManager.GetSysString(507), 9, 15);
                break;

            case U_LEO_3:
                cuoc = CuocSac(dataManager.GetSysString(508), 7, 13);
                break;

            case U_LEO_4:
                cuoc = CuocSac(dataManager.GetSysString(509), 7, 13);
                break;
        }
        return cuoc;
    }

    static string TestGameCuoc(string& lsCuoc) {

        if(0 == lsCuoc.compare(""))
            return  "";

        vector<string> ls = mUtils::splitString(lsCuoc, ':');
        vector<CuocSac> arr;

        for (int i = 0; i < ls.size(); ++i) {
            try {
               // log("for 1 %d", i);
                int t = atoi(ls.at(i).c_str());
                CuocSac a = ChanUtils::GetNameCuocSacById_New(t);
                if (a.ten != "") {
                    arr.push_back(a);
                }
            } catch(std::out_of_range& e) {
               // log("%s", e.what());
            } catch(...){

            }
        }

        for (int i = 0; i < arr.size() - 1; ++i) {
           // log("for 2 %d", i);

            for (int j = i + 1; j < arr.size(); ++j) {
               // log("for 3 %d", j);
                try {
                    if (arr.at(i).uutien > arr.at(j).uutien) {
                        CuocSac temp;
                        temp = arr.at(i);
                        arr.at(i) = arr.at(j);
                        arr.at(j) = temp;
                    }
                } catch (...) {
                   // log("exception: ");
                }
            }
        }

        std::string res = "";
        for (int i = 0; i < arr.size(); ++i) {
           // log("for 4 %d", i);
            try {
                res += arr.at(i).ten;
                if (arr.at(i).ten != "")
                    res += " ";
            } catch (std::out_of_range& e) {
               // log("ex %s", e.what());
            } catch(...){

            }
        }
        return res;
    }

    static std::string GetNameCard(std::string& cards) {
        std::vector<std::string> info = mUtils::splitString(cards, ':');
        try {
            int number = atoi(info.at(1).c_str());
            int suite = atoi(info.at(2).c_str());
            return ChanUtils::GetTypeCardChan(number, suite);
        } catch (std::out_of_range &e) {
           // log("GetNameCard out_of_range exception: %s", e.what());
            return "";
        } catch (std::exception &e) {
           // log("GetNameCard exception of an unexpected type %s", e.what());
        } catch (...) {
           // log("GetNameCard unknown exception");
        }
        return "";
    }

    static string formatMoneySymbol(double money)
    {
        if (money < 10000)
        {
            string rs = mUtils::convertMoneyEx(money);
            return rs;
        }

        try{
            vector<CurrencyStr> arrSi;
            for (int i = 0; i < 2; i++)
                arrSi.push_back(CurrencyStr());

            arrSi.at(1).symbolStruct = "K";
            arrSi.at(1).valueStruct = 1000;

            arrSi.at(0).symbolStruct = "M";
            arrSi.at(0).valueStruct = 1000000;

            for (int i = 0; i < arrSi.size(); ++i)
            {
                if (money >= arrSi.at(i).valueStruct)
                {
                    float val = money / arrSi.at(i).valueStruct;
                    if (fmodf(money, 1000) == 0)
                        return StringUtils::format("%.0f", val) + " " + arrSi.at(i).symbolStruct;
                    return StringUtils::format("%.1f", val) + " " + arrSi.at(i).symbolStruct;
                }
            }
            return boost::to_string(money);

        }catch(std::out_of_range& e){
           // log("%s", e.what());
            string rs = mUtils::convertMoneyEx(money);
            return rs;
        }catch(...){
            string rs = mUtils::convertMoneyEx(money);
            return rs;
        }
        string rs = mUtils::convertMoneyEx(money);
        return rs;
    }

private:
    static std::string GetTypeCardChan(const int& number, const int& suite) {
        std::string str = "";
        //!sui 2:sach 0:vạn 1 văn
        switch (number) {
        case 2:
            str += dataManager.GetSysString(725);
            break;
        case 3:
            str += dataManager.GetSysString(726);
            break;
        case 4:
            str += dataManager.GetSysString(727);
            break;
        case 5:
            str += dataManager.GetSysString(728);
            break;
        case 6:
            str += dataManager.GetSysString(729);
            break;
        case 7:
            str += dataManager.GetSysString(730);
            break;
        case 8:
            str += dataManager.GetSysString(731);
            break;
        case 9:
            str += dataManager.GetSysString(732);
            break;
        default:
            str += dataManager.GetSysString(733);
            break;
        }

        switch (suite) {
        case CARDSUIT_VANV:
            str+= " "+dataManager.GetSysString(734);
            break;
        case CARDSUIT_VANC:
            str+= " "+dataManager.GetSysString(735);
            break;
        case CARDSUIT_SACH:
            str+= " "+dataManager.GetSysString(736);
            break;
        case CARDSUIT_CHI:
            str+= " "+dataManager.GetSysString(737);
        default:
            break;
        }
        return str;
    }
};
#endif
