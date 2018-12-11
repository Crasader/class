#ifndef ___XiToHelper__h_
#define ___XiToHelper__h_

#include "ClientMsgDefs.h"
#include <string>
#include <iostream>
#include "SceneManager.h"
#include "PhomMessDef.h"
using namespace std;

//Time choose cards xito
#define TIMER_CHOOSE_CARD 30

class XiToInfoPlayer{

public:
    int pos;
    string aI;
    string aN;
    double aM;
    string aal;
    bool isReady;
    bool isBoss;

public:
    XiToInfoPlayer(const int& pos, const string& aI, const string& aN, const double& aM, string& aal, const bool& isReady, const bool& isBoss){
        this->pos = pos;
        this->aI = aI;
        this->aN = aN;
        this->aM = aM;
        this->aal = aal;
        this->isReady = isReady;
        this->isBoss = isBoss;
    }

    ~XiToInfoPlayer(){
        cout << "~~~~ XiToInfoPlayer Destructor" << endl;
    }

    int& Test(){
        return this->pos;
    }
};

class TagName{
public:
    int tag;
    string name;
    TagName(const int& _tag, const string& _name){
        tag = _tag;
        name = _name;
    }
    ~TagName(){
        cout << "~~~~~~~ Destructor of TagName ~~~~~~~" << endl;
    }
};

// Bet Xi To
enum
{
    GAME_TABLE_STATUS_BET_FOLD,
    GAME_TABLE_STATUS_BET_RAISE,
    GAME_TABLE_STATUS_BET_NONE,
    GAME_TABLE_STATUS_BET_QUATER,
    GAME_TABLE_STATUS_BET_HAFT,
    GAME_TABLE_STATUS_BET_FOLLOW,
    GAME_TABLE_STATUS_BET_DOUBLE,
    GAME_TABLE_STATUS_BET_ALL,
};
// Bet Poker
enum
{
    GAME_TABLE_STATUS_BET_POKER_FOLD,
    GAME_TABLE_STATUS_BET_POKER_RAISE,
    GAME_TABLE_STATUS_BET_POKER_CALL,
    GAME_TABLE_STATUS_BET_POKER_ALLIN,
    GAME_TABLE_STATUS_BET_POKER_HAFT,
    GAME_TABLE_STATUS_BET_POKER_FOLLOW,
};

// Victype
#define VT_MAU_THAU 1
#define VT_DOI 2
#define VT_THU 3
#define VT_SAMCO 4
#define VT_SANH 5
#define VT_THUNG 6
#define VT_CU_LU 7
#define VT_TUQUY 8
#define VT_THUNG_PHA_SANH 9
#define GAME_CARD_TYPE__PA_SANH_LON 10
class XiToHelper{
public:
    static std::string FindTypeCardPoker(const int& idCard)
    {
        ostringstream oss;
        int pID = idCard;
        int num = (pID / 4);
        int type = (pID % 4);
        
        if (type == 0) {
            num--;
            type = 3;
        }
        else
            type--;
        if (pID == 0) {
            num = 0;
            type = 0;
        }
        
        string url = "card_";
        switch (num) {
            case 11:
                url += "j";
                break;
            case 12:
                url += "q";
                break;
            case 13:
                url += "k";
                break;
            case 14:
                url += "a";
                break;
            case 15:
                url += "2";
                break;
            default:
                oss << num;
                url += oss.str();
                break;
        }
        
        url += "_";
        
        // [0, 1, 2, 3] = ['s', 'c', 'd', 'h']
        switch (type) {
            case 0:
                url += "s.png";
                break;
            case 1:
                url += "c.png";
                break;
            case 2:
                url += "d.png";
                break;
            case 3:
                url += "h.png";
                break;
        }
        return url;
    }
    static std::string FindTypeCard(const int& idCard)
    {
		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1)
		{
			switch (idCard) {
			case -1:
				return "card_back.png";
			case 1:
				return "card_7_s.png";
			case 2:
				return "card_7_c.png";
			case 3:
				return "card_7_d.png";
			case 4:
				return "card_7_h.png";
			case 5:
				return "card_8_s.png";
			case 6:
				return "card_8_c.png";
			case 7:
				return "card_8_d.png";
			case 8:
				return "card_8_h.png";
			case 9:
				return "card_9_s.png";
			case 10:
				return  "card_9_c.png";
			case 11:
				return  "card_9_d.png";
			case 12:
				return  "card_9_h.png";
			case 13:
				return  "card_10_s.png";
			case 14:
				return  "card_10_c.png";
			case 15:
				return  "card_10_d.png";
			case 16:
				return  "card_10_h.png";
			case 17:
				return  "card_j_s.png";
			case 18:
				return  "card_j_c.png";
			case 19:
				return  "card_j_d.png";
			case 20:
				return  "card_j_h.png";
			case 21:
				return  "card_q_s.png";
			case 22:
				return  "card_q_c.png";
			case 23:
				return  "card_q_d.png";
			case 24:
				return  "card_q_h.png";
			case 25:
				return  "card_k_s.png";
			case 26:
				return  "card_k_c.png";
			case 27:
				return  "card_k_d.png";
			case 28:
				return  "card_k_h.png";
			case 29:
				return  "card_a_s.png";
			case 30:
				return  "card_a_c.png";
			case 31:
				return  "card_a_d.png";
			case 32:
				return  "card_a_h.png";
			default:
				break;
			}
			return "card_back.png";
		}
		else
		{
			switch (idCard) {
			case -1:
				return "card_back.png";
			case 1:
				return "7-bich.png";
			case 2:		 
				return "7-tep.png";
			case 3:		 
				return "7-ro.png";
			case 4:		 
				return "7-co.png";
			case 5:		 
				return "8-bich.png";
			case 6:		 
				return "8-tep.png";
			case 7:		 
				return "8-ro.png";
			case 8:		 
				return "8-co.png";
			case 9:		 
				return "9-bich.png";
			case 10:	 
				return "9-tep.png";
			case 11:	 
				return "9-ro.png";
			case 12:	 
				return "9-co.png";
			case 13:
				return "10-bich.png";
			case 14:
				return "10-tep.png";
			case 15:
				return "10-ro.png";
			case 16:
				return "10-co.png";
			case 17:
				return "j-bich.png";
			case 18:	 
				return "j-tep.png";
			case 19:	 
				return "j-ro.png";
			case 20:	 
				return "j-co.png";
			case 21:	 
				return "q-bich.png";
			case 22:	 
				return "q-tep.png";
			case 23:	 
				return "q-ro.png";
			case 24:	 
				return "q-co.png";
			case 25:	 
				return "k-bich.png";
			case 26:	 
				return "k-tep.png";
			case 27:	 
				return "k-ro.png";
			case 28:	 
				return "k-co.png";
			case 29:	 
				return "a-bich.png";
			case 30:	 
				return "a-tep.png";
			case 31:	 
				return "a-ro.png";
			case 32:	 
				return "a-co.png";
			default:
				break;
			}
			return "card_back.png";
		}
        
    }

    static string GetTypeListCards(const long& type) {
        switch (type) {
        case 0:
            return dataManager.GetSysString(649);
        case VT_MAU_THAU:
            return dataManager.GetSysString(649);
        case VT_DOI:
            return dataManager.GetSysString(586);
        case VT_THU:
            return dataManager.GetSysString(674);
        case VT_SAMCO:
            return dataManager.GetSysString(647);
        case VT_SANH:
            return dataManager.GetSysString(648);
        case VT_THUNG:
            return dataManager.GetSysString(675);
        case VT_CU_LU:
            return dataManager.GetSysString(557);
        case VT_TUQUY:
            return dataManager.GetSysString(676);
        case VT_THUNG_PHA_SANH:
            return dataManager.GetSysString(677);
        case GAME_CARD_TYPE__PA_SANH_LON:
            return dataManager.GetSysString(931);
        default:
            return "";
        }
    }

    static string GetTypeBet(const long& type){
        switch (type) {
        case GAME_TABLE_STATUS_BET_FOLD:
            return dataManager.GetSysString(230);
        case GAME_TABLE_STATUS_BET_RAISE:
            return dataManager.GetSysString(31);
        case GAME_TABLE_STATUS_BET_NONE:
            return dataManager.GetSysString(631);
        case GAME_TABLE_STATUS_BET_QUATER:
            return dataManager.GetSysString(678);
        case GAME_TABLE_STATUS_BET_HAFT:
            return dataManager.GetSysString(679);
        case GAME_TABLE_STATUS_BET_FOLLOW:
            return dataManager.GetSysString(32);
        case GAME_TABLE_STATUS_BET_DOUBLE:
            return dataManager.GetSysString(680);
        case GAME_TABLE_STATUS_BET_ALL:
            return dataManager.GetSysString(681);
        }
        return "";
    }
};

#endif
