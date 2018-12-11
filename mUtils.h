//
//  mUtils.h
//  demo
//
//  Created by AlexDao on 5/25/14.
//
//

#ifndef demo_mUtils_h
#define demo_mUtils_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "2d/CCActionInstant.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>
//#include <boost/date_time/gregorian/gregorian.hpp>
#include <locale>
#include <ctime>
#include "data_manager.h"
#include "AllData.h"
#include "boost/asio/detail/strand_service.hpp"
#include "boost/exception/errinfo_errno.hpp"
#include "md5.h"

USING_NS_CC;

#define V_REGISTER_LOADER_GLUE(NODE_LIBRARY, CLASS) NODE_LIBRARY->registerNodeLoader(#CLASS, CLASS##Loader::loader())

class mUtils{
private:
    struct currencyStruct{
        double valueStruct;
        string symbolStruct;
    };
public:
    //Sá»‘ báº¯t Ä‘áº§u cá»§a GameID
    static const int numBeginGameID = 100;
    //Sá»‘ lÆ°á»£ng game
    static const int numOfGame = 10;
    
    
    
    //Láº¥y tÃªn game dá»±a vÃ o gameID
    static std::string* getGameNameByID(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(168));
            case 101:
                return new string(dataManager.GetSysString(169));
            case 102:
                return new string(dataManager.GetSysString(170));
            case 103:
                return new string(dataManager.GetSysString(171));
            case 104:
                return new string(dataManager.GetSysString(172));
            case 105:
                return new string(dataManager.GetSysString(173));
            case 107:
                return new string(dataManager.GetSysString(174));
            case 108:
                return new string(dataManager.GetSysString(175));
            case 109:
                return new string(dataManager.GetSysString(176));
            case 106:
                return new string(dataManager.GetSysString(177));
            case 110:
                return new string(dataManager.GetSysString(1204));
            case 111:
                return new string(dataManager.GetSysString(1206));
            case 112:
                return new string(dataManager.GetSysString(1205));
            case 113:
                return new string(dataManager.GetSysString(1207));
            case 114:
                return new string(dataManager.GetSysString(1260));
            case 115:
                return new string(dataManager.GetSysString(1261));
            case kGameDemLaSolo:
                return new string(dataManager.GetSysString(1257));
            case kGameSamSolo:
                return new string(dataManager.GetSysString(1258));
            case kGamePoker:
                return new string(dataManager.GetSysString(885));
            case kGameMauBinh:
                return new string(dataManager.GetSysString(932));
            case kGameCatte:
                return new string(dataManager.GetSysString(915));
            case kGame3Doi:
                return new string(dataManager.GetSysString(916));
            default:
                break;
        }
        return new string("Unknown gid");
    }
    static std::string* getGameNameByIDCam(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(12391));
            case 101:
                return new string(dataManager.GetSysString(12401));
            case 102:
                return new string(dataManager.GetSysString(12411));
            case 103:
                return new string(dataManager.GetSysString(12421));
            case 104:
                return new string(dataManager.GetSysString(12431));
            case 105:
                return new string(dataManager.GetSysString(12441));
            case 107:
                return new string(dataManager.GetSysString(12451));
            case 108:
                return new string(dataManager.GetSysString(12461));
            case 109:
                return new string(dataManager.GetSysString(12471));
            case 106:
                return new string(dataManager.GetSysString(12481));
            case 110:
                return new string(dataManager.GetSysString(12491));
            case 111:
                return new string(dataManager.GetSysString(12511));
            case 112:
                return new string(dataManager.GetSysString(12501));
            case 113:
                return new string(dataManager.GetSysString(12521));
            case 114:
                return new string(dataManager.GetSysString(19921));
            case 115:
                return new string(dataManager.GetSysString(12611));
            case kGameDemLaSolo:
                return new string(dataManager.GetSysString(19901));
            case kGameSamSolo:
                return new string(dataManager.GetSysString(19911));
            case kGamePoker:
                return new string(dataManager.GetSysString(19941));
            case kGameMauBinh:
                return new string(dataManager.GetSysString(19951));
            case kGameCatte:
                return new string(dataManager.GetSysString(19961));
            case kGame3Doi:
                return new string(dataManager.GetSysString(19971));
            case kGameBauTom:
                return new string("Kla Klouk");
            default:
                break;
        }
        return new string("Unknown gid");
    }
    static std::string* getGameNameByIDEng(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(12392));
            case 101:
                return new string(dataManager.GetSysString(12402));
            case 102:
                return new string(dataManager.GetSysString(12412));
            case 103:
                return new string(dataManager.GetSysString(12422));
            case 104:
                return new string(dataManager.GetSysString(12432));
            case 105:
                return new string(dataManager.GetSysString(12442));
            case 107:
                return new string(dataManager.GetSysString(12452));
            case 108:
                return new string(dataManager.GetSysString(12462));
            case 109:
                return new string(dataManager.GetSysString(12472));
            case 106:
                return new string(dataManager.GetSysString(12482));
            case 110:
                return new string(dataManager.GetSysString(12492));
            case 111:
                return new string(dataManager.GetSysString(12512));
            case 112:
                return new string(dataManager.GetSysString(12502));
            case 113:
                return new string(dataManager.GetSysString(12522));
            case 114:
                return new string(dataManager.GetSysString(19922));
            case 115:
                return new string(dataManager.GetSysString(12612));
            case kGameDemLaSolo:
                return new string(dataManager.GetSysString(19902));
            case kGameSamSolo:
                return new string(dataManager.GetSysString(19912));
            case kGamePoker:
                return new string(dataManager.GetSysString(19942));
            case kGameMauBinh:
                return new string(dataManager.GetSysString(19952));
            case kGameCatte:
                return new string(dataManager.GetSysString(19962));
            case kGame3Doi:
                return new string(dataManager.GetSysString(19972));
            case kGameBauTom:
                return new string("Kla Klouk");
            default:
                break;
        }
        return new string("Unknown gid");
    }
    static std::string* getGameNameByIDCN(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(12393));
            case 101:
                return new string(dataManager.GetSysString(12403));
            case 102:
                return new string(dataManager.GetSysString(12413));
            case 103:
                return new string(dataManager.GetSysString(12423));
            case 104:
                return new string(dataManager.GetSysString(12433));
            case 105:
                return new string(dataManager.GetSysString(12443));
            case 107:
                return new string(dataManager.GetSysString(12453));
            case 108:
                return new string(dataManager.GetSysString(12463));
            case 109:
                return new string(dataManager.GetSysString(12473));
            case 106:
                return new string(dataManager.GetSysString(12483));
            case 110:
                return new string(dataManager.GetSysString(12493));
            case 111:
                return new string(dataManager.GetSysString(12513));
            case 112:
                return new string(dataManager.GetSysString(12503));
            case 113:
                return new string(dataManager.GetSysString(12523));
            case 114:
                return new string(dataManager.GetSysString(19923));
            case 115:
                return new string(dataManager.GetSysString(12613));
            case kGameDemLaSolo:
                return new string(dataManager.GetSysString(19903));
            case kGameSamSolo:
                return new string(dataManager.GetSysString(19913));
            case kGamePoker:
                return new string(dataManager.GetSysString(19943));
            case kGameMauBinh:
                return new string(dataManager.GetSysString(19953));
            case kGameCatte:
                return new string(dataManager.GetSysString(19963));
            case kGame3Doi:
                return new string(dataManager.GetSysString(19973));
            case kGameBauTom:
                return new string("螃蟹");
            default:
                break;
        }
        return new string("Unknown gid");
    }
    static std::string* getGameNameByIDViet(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(1239));
            case 101:
                return new string(dataManager.GetSysString(1240));
            case 102:
                return new string(dataManager.GetSysString(1241));
            case 103:
                return new string(dataManager.GetSysString(1242));
            case 104:
                return new string(dataManager.GetSysString(1243));
            case 105:
                return new string(dataManager.GetSysString(1244));
            case 107:
                return new string(dataManager.GetSysString(1245));
            case 108:
                return new string(dataManager.GetSysString(1246));
            case 109:
                return new string(dataManager.GetSysString(1247));
            case 106:
                return new string(dataManager.GetSysString(1248));
            case 110:
                return new string(dataManager.GetSysString(1249));
            case 111:
                return new string(dataManager.GetSysString(1251));
            case 112:
                return new string(dataManager.GetSysString(1250));
            case 113:
                return new string(dataManager.GetSysString(1252));
            case 114:
                return new string(dataManager.GetSysString(1992));
            case 115:
                return new string(dataManager.GetSysString(1261));
            case kGameDemLaSolo:
                return new string(dataManager.GetSysString(1990));
            case kGameSamSolo:
                return new string(dataManager.GetSysString(1991));
            case kGamePoker:
                return new string(dataManager.GetSysString(1994));
            case kGameMauBinh:
                return new string(dataManager.GetSysString(1995));
            case kGameCatte:
                return new string(dataManager.GetSysString(1996));
            case kGame3Doi:
                return new string(dataManager.GetSysString(1997));
            case kGameBauTom:
                return new string("Kla Klouk");
            default:
                break;
        }
        return new string("Unknown gid");
    }
    static string getUrlImageGame(int gameID) {
        auto sUrl = "";
        string _url = StringUtils::format("%d.png", gameID);
        return _url;
        // 		switch (gameID) {
        // 		case kGameBaCayChuong:
        // 			sUrl = "icon-3caychuong.png";
        // 			break;
        // 		case kGameBaCay:
        // 			sUrl = "icon-3caythuong.png";
        // 			break;
        // 		case kGameChan:
        // 			sUrl = "icon-chan.png";
        // 			break;
        // 		case kGameCoTuong:
        // 			sUrl = "icon-cotuong.png";
        // 			break;
        // 		case kGameCoUp:
        // 			sUrl = "icon-coup.png";
        // 			break;
        // 		case kGameLieng:
        // 			sUrl = "icon-lieng.png";
        // 			break;
        // 		case kGameSam:
        // 			sUrl = "icon-samloc.png";
        // 			break;
        // 		case kGameTaiXiu:
        // 			sUrl = "icon-taixiu.png";
        // 			break;
        // 		case kGamePhom:
        // 			sUrl = "icon-tala.png";
        // 			break;
        // 		case kGameTienLenMienBac:
        // 			sUrl = "icon-tlmb.png";
        // 			break;
        // 		case kGameTienLenMienNam:
        // 			sUrl = "icon-tlmn.png";
        // 			break;
        // 		case kGameTLMNDemLa:
        // 			sUrl = "icon-tienlendemla.png";
        // 			break;
        // 		case kGameTomCuaCa:
        // 			sUrl = "icon-tomcuaca.png";
        // 			break;
        // 		case kGameXiTo:
        // 			sUrl = "icon-xito.png";
        // 			break;
        // 		case kGameDemLaSolo:
        // 			sUrl = "icon-tienlendemlasolo.png";
        // 			break;
        // 		default:
        // 			break;
        // 		}
        // 		return sUrl;
    }
    static std::string* getGameNameUpperCaseByID(int gid){
        switch (gid) {
            case 100:
                return new string(dataManager.GetSysString(168));
            case 101:
                return new string(dataManager.GetSysString(169));
            case 102:
                return new string(dataManager.GetSysString(170));
            case 103:
                return new string(dataManager.GetSysString(171));
            case 104:
                return new string(dataManager.GetSysString(172));
            case 105:
                return new string(dataManager.GetSysString(173));
            case 107:
                return new string(dataManager.GetSysString(174));
            case 108:
                return new string(dataManager.GetSysString(175));
            case 109:
                return new string(dataManager.GetSysString(176));
            case 106:
                return new string(dataManager.GetSysString(177));
            case 110:
                return new string(dataManager.GetSysString(1204));
            case 111:
                return new string(dataManager.GetSysString(1206));
            case 112:
                return new string(dataManager.GetSysString(1205));
            case 113:
                return new string(dataManager.GetSysString(1207));
            case kGamePoker:
                return new string(dataManager.GetSysString(885));
            case kGame3Doi:
                return new string(dataManager.GetSysString(916));
            case kGameCatte:
                return new string(dataManager.GetSysString(915));
            default:
                break;
        }
        return new string("Unknown gid: %d", gid);
    }
    /*
     Format money
     */
    static std::string* format_money(long long money){
        char sMoneyTmp[30], sMoney[30], s[30];
        
        sprintf(sMoneyTmp, "%lld", money);
        int len = strlen(sMoneyTmp);
        
        int i = len - 1, counterSeg = 0, counter = 0;
        
        while (i >= 0){
            sMoney[counter++] = sMoneyTmp[i];
            counterSeg++;
            if (counterSeg == 3){
                counterSeg = 0;
                if (i != 0)
                    sMoney[counter++] = ',';
            }
            i--;
        }
        
        for (i = counter - 1; i >= 0; i--){
            s[counter - i - 1] = sMoney[i];
        }
        s[counter] = 0;
        return new string(s);
    }
    /*
     is charactor
     */
    static bool isCharacter(const char Character)
    {
        return ((Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z'));
    }
    /*
     is email invalid
     */
    static bool isValidEmailAddress(const char * email)
    {
        if (!email)
            return 0;
        if (!mUtils::isCharacter(email[0]))
            return 0;
        int AtOffset = -1;
        int DotOffset = -1;
        unsigned int Length = strlen(email);
        
        for (unsigned int i = 0; i < Length; i++)
        {
            if (email[i] == '@')
                AtOffset = (int)i;
            else if (email[i] == '.')
                DotOffset = (int)i;
        }
        
        if (AtOffset == -1 || DotOffset == -1)
            return 0;
        if (AtOffset > DotOffset)
            return 0;
        return !(DotOffset >= ((int)Length - 1));
        
    }
    
    /*
     Tach xau
     */
    static vector<string> splitString(string &S, const char &str){
        vector<string> arrStr;
        string::iterator t, t2;
        for (t = S.begin(); t < S.end();){
            t2 = find(t, S.end(), str);
            if (t != t2)
                arrStr.push_back(string(t, t2));
            else  if (t2 != S.end())
                arrStr.push_back("");
            if (t2 == S.end())
                break;
            t = t2 + 1;
        }
        return arrStr;
    }
    
    /*
     Get current date time
     */
    static string getCurrentDateTime(const char* fmt)
    {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        strftime(buffer, 80, fmt, timeinfo);
        std::string str(buffer);
        return str;
    }
    
    /*
     Get name of trans type
     */
    static string getExplainOfTransType(const char* fmt)
    {
        if (strcmp(fmt, "VT") == 0){
            return dataManager.GetSysString(1188);
        }
        else if (strcmp(fmt, "CT") == 0){
            return dataManager.GetSysString(1189);
        }
        else if (strcmp(fmt, "TT") == 0){
            return dataManager.GetSysString(1191);
        }
        else if (strcmp(fmt, "NT") == 0){
            return dataManager.GetSysString(1190);
        }
        return "Unknown";
    }
    
    static string convertMoneyEx(long long money){
        char sMoneyTmp[30], sMoney[30], s[30];
        
        sprintf(sMoneyTmp, "%lld", money);
        int len = strlen(sMoneyTmp);
        
        int i = len - 1, counterSeg = 0, counter = 0;
        
        while (i >= 0){
            sMoney[counter++] = sMoneyTmp[i];
            counterSeg++;
            if (counterSeg == 3){
                counterSeg = 0;
                if (i != 0)
                    sMoney[counter++] = ',';
            }
            i--;
        }
        
        for (i = counter - 1; i >= 0; i--){
            s[counter - i - 1] = sMoney[i];
        }
        s[counter] = 0;
        
        return string(s);
    }
    
    static string convertMoneyFromDoubleToString(double money){
        char ch[100];
        sprintf(ch, "%.0f", money);
        string _amf = string(ch);
        if (money > 0) {
            _amf = "+" + _amf;
        }
        
        return _amf;
    }
    
    static Action* getActionOpenPopup(){
        return Sequence::create(ScaleTo::create(0, 1.0, 1.0)
                                // 			ScaleTo::create(0, 0.06f, 1.05f),
                                // 			ScaleTo::create(0, 0.08f, 0.95f),
                                // 			ScaleTo::create(0, 0.08f, 1.0f)
                                , nullptr);
    }
    
    static vector<string> splitStringByListRegex(string S, vector<string> lstRegex, bool isKeepRegex = false){
        vector<string> arrStr;
        do{
            //TÃ¬m substr gáº§n nháº¥t
            int minPos = INT_MAX;
            string currRegx = "";
            //Find
            for (int i = 0; i < (int)lstRegex.size(); i++){
                std::size_t vt1 = S.find(lstRegex.at(i));
                if (vt1 != std::string::npos && minPos >(int)vt1){ //Find it
                    minPos = vt1;
                    currRegx = lstRegex.at(i);
                }
            }
            if (minPos == INT_MAX){
                //End
                arrStr.push_back(S);
                break;
            }
            else{
                //Push to vector and continues
                if (minPos != 0)
                    arrStr.push_back(S.substr(0, minPos));
                if (isKeepRegex)
                    arrStr.push_back(currRegx);
                S = S.substr(minPos + currRegx.length(), S.length() - minPos - currRegx.length());
            }
        } while (S.length() > 0);
        
        return arrStr;
    }
    static bool isSoundOn(){
        UserDefault *def = UserDefault::getInstance();
        return def->getBoolForKey("sound", false);
    }
    static bool isVibrateOn(){
        UserDefault *def = UserDefault::getInstance();
        return def->getBoolForKey("vibrate", true);
    }
    static string getFileName(string fullpath){
        unsigned  found = fullpath.find_last_of("/");
        if (found != -1)
            return fullpath.substr(found + 1);
        return "";
    }
    
    static string getErrorSmartfoxExplain(int errorCode){
        switch (errorCode) {
            case 0:
                return dataManager.GetSysString(178);
            case 1:
                return dataManager.GetSysString(179);
            case 2:
                return dataManager.GetSysString(180);
            case 3:
                return dataManager.GetSysString(181);
            case 4:
                return dataManager.GetSysString(182);
            case 5:
                return dataManager.GetSysString(183);
            case 6:
                return dataManager.GetSysString(184);
            case 7:
                return dataManager.GetSysString(183);
            case 8:
                return dataManager.GetSysString(185);
            case 9:
                return dataManager.GetSysString(186);
            case 10:
                return dataManager.GetSysString(187);
            case 11:
                return dataManager.GetSysString(188);
            case 12:
                return dataManager.GetSysString(189);
            case 13:
                return dataManager.GetSysString(190);
            case 14:
                return dataManager.GetSysString(191);
            case 15:
                return dataManager.GetSysString(192);
            case 16:
                return dataManager.GetSysString(193);
            case 17:
                return dataManager.GetSysString(194);
            case 18:
                return dataManager.GetSysString(195);
            case 19:
                return dataManager.GetSysString(196);
            case 20:
                return dataManager.GetSysString(197);
            case 21:
                return dataManager.GetSysString(198);
            case 22:
                return dataManager.GetSysString(199);
            case 23:
                return dataManager.GetSysString(1100);
            case 24:
                return dataManager.GetSysString(1101);
            case 25:
                return dataManager.GetSysString(1102);
            case 26:
                return dataManager.GetSysString(1102);
            case 27:
                return dataManager.GetSysString(1102);
            case 28:
                return dataManager.GetSysString(1142);
            case 29:
                return dataManager.GetSysString(1103);
            case 30:
                return dataManager.GetSysString(1104);
            case 31:
                return dataManager.GetSysString(1105);
            case 32:
                return dataManager.GetSysString(1106);
            case 33:
                return dataManager.GetSysString(1107);
            case 34:
                return dataManager.GetSysString(1108);
            case 35:
                return dataManager.GetSysString(1109);
            case 36:
                return dataManager.GetSysString(1110);
            case 37:
                return dataManager.GetSysString(1111);
            case 38:
                return dataManager.GetSysString(1112);
            case 39:
                return dataManager.GetSysString(1113);
            case 40:
                return dataManager.GetSysString(1114);
            case 41:
                return dataManager.GetSysString(1115);
            case 42:
                return dataManager.GetSysString(1116);
        }
        return "";
    }
    
    static string getErrorZoneExplain(int errorCode){
        switch (errorCode){
            case 0:	 return dataManager.GetSysString(1117);
            case 10: return dataManager.GetSysString(1118);
            case 11: return dataManager.GetSysString(1119);
            case 12: return dataManager.GetSysString(1120);
            case 13: return dataManager.GetSysString(1121);
            case 14: return dataManager.GetSysString(1122);
            case 15: return dataManager.GetSysString(1123);
            case 16: return dataManager.GetSysString(1124);
            case 17: return dataManager.GetSysString(1125);
            case 18: return dataManager.GetSysString(1126);
            case 19: return dataManager.GetSysString(1127);
            case 20: return dataManager.GetSysString(1128);
            case 21: return dataManager.GetSysString(1129);
            case 22: return dataManager.GetSysString(1130);
            case 23: return dataManager.GetSysString(1131);
            case 24: return dataManager.GetSysString(1132);
            case 25: return dataManager.GetSysString(1133);
            case 26: return dataManager.GetSysString(1134);
            case 27: return dataManager.GetSysString(1118);
            case 28: return dataManager.GetSysString(1135);
            case 29: return dataManager.GetSysString(1136);
            case 30: return dataManager.GetSysString(1137);
            case 31: return dataManager.GetSysString(1138);
            case 32: return dataManager.GetSysString(1139);
            case 34: return dataManager.GetSysString(1140);
            case 36: return dataManager.GetSysString(1141);
            case 101: return dataManager.GetSysString(1142);
            case 111: return dataManager.GetSysString(1143);
            case 112: return dataManager.GetSysString(1119);
            case 113: return dataManager.GetSysString(1132);
            case 114: return dataManager.GetSysString(1144);
            case 115: return dataManager.GetSysString(1145);
            case 116: return dataManager.GetSysString(1146);
            case 117: return dataManager.GetSysString(1147);
            case 118: return dataManager.GetSysString(1148);
            case 119: return dataManager.GetSysString(1149);
            case 120: return dataManager.GetSysString(181);
            case 121: return dataManager.GetSysString(1150);
            case 122: return dataManager.GetSysString(1151);
            case 127: return dataManager.GetSysString(1152);
            case 123: return dataManager.GetSysString(1153);
            case 124: return dataManager.GetSysString(1154);
            case 126: return dataManager.GetSysString(1155);
            case 125: return dataManager.GetSysString(1155);
            case 37: return dataManager.GetSysString(1156);
            case 38: return dataManager.GetSysString(1157);
            case 39: return dataManager.GetSysString(1158);
            case 128: return dataManager.GetSysString(1159);
            case 129: return dataManager.GetSysString(1160);
            case 130: return dataManager.GetSysString(1161);
            case 1001: return dataManager.GetSysString(1162);
            case 131: return dataManager.GetSysString(1163);
            case 132: return dataManager.GetSysString(1164);
            case 133: return dataManager.GetSysString(1165);
            case 134: return dataManager.GetSysString(1166);
            case 135: return dataManager.GetSysString(1167);
            case 136: return dataManager.GetSysString(1170);
            case 138: return dataManager.GetSysString(1168);
            case 137: return dataManager.GetSysString(1169);;
        }
        return "";
    }
    
    static string trimString(string &src){
        //HoangDDs
        // 		src.erase(src.begin(), std::find_if(src.begin(), src.end(),
        // 			std::not1(std::ptr_fun<int, int>(std::isspace))));
        return src;
    }
    ///tim ki tu dac biet cho phep dau cach
    static bool findSpecialCharSpace(string src){
        if (src.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_ ") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    ///tim ki tu dac biet khong cho phep dau cach
    static bool findSpecialCharNotSpace(string src){
        if (src.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    static bool findContainChar(string src){
        if (src.find_first_not_of("0123456789") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    //check email không cho có các kí tự đặc biệt
    static bool findSpecialCharInEmail(string src){
        if (src.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_@.") != std::string::npos)
        {
            return true;
        }
        return false;
    }
    ///resize string
    static string resizeStr(string src, int len){
        if (((int)src.size()) > len){
            src.resize(len);
        }
        return src;
    }
    
    static string getSrcVip(int vipType){
        
        switch (vipType) {
            case 0:
                return "moc3-vip-00.png";
            case 1:
                return "moc3-vip-01.png";
            case 2:
                return "moc3-vip-02.png";
            case 3:
                return "moc3-vip-03.png";
            case 4:
                return "moc3-vip-04.png";
            case 5:
                return "moc3-vip-05.png";
            case 6:
                return "moc3-vip-06.png";
            case 7:
                return "moc3-vip-07.png";
            case 8:
                return "moc3-vip-08.png";
            case 9:
                return "moc3-vip-09.png";
            case 10:
                return "moc3-vip-10.png";
            case 11:
                return "moc3-vip-11.png";
            case 12:
                return "moc3-vip-12.png";
            default:
                return "moc3-vip-10.png";
        }
        
    }
    
    static cocos2d::Size getSizePos() {
        cocos2d::Size size = Director::getInstance()->getVisibleSize();
        
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
    }
    
    static int getMaximumValue4Slider(long long maxMoney){
        if (maxMoney <= 1000)
            return 1;
        int counter = 0;
        long long step = 1000;
        for (long long i = 1000; i <= maxMoney; i += step)
        {
            long long max = 100000000000;//100,000,000,000
            do{
                if ((i + step)<(max + step)){
                    max /= 10;
                    continue;
                }
                //else
                step = max;
                if (step>1000)
                    step /= 2;
                break;
            } while (true);
            //printf("counter: %d i: %lld step: %lld\n",counter , i, step );
            counter++;
        }
        return counter - 1;
    }
    
    static long long getMoneyFromSliderValue(int currValue){
        long long money = 1000;
        long long step = 1000;
        if (currValue == 0)
            return 0;
        if (currValue == 1)
            return 1000;
        int counter = 1;
        do{
            long long max = 1000;//100,000,000,000
            do{
                if ((money + step) > (max * 10)){
                    max *= 10;
                    continue;
                }
                //else
                step = max;
                if (step > 1000)
                    step /= 2;
                break;
            } while (true);
            money += step;
            counter++;
            //printf("counter: %d i: %lld step: %lld\n",counter , money, step );
        } while (counter <= currValue);
        
        return money;
    }
    
    static string replaceStrChar(string str, const string& replace, char ch){
        size_t i = str.find_first_of(replace);
        while (i != string::npos){
            str[i] = ch;
            i = str.find_first_not_of(replace, i + 1);
        }
        return str;
    }
    
    static string getGroupIDByMoney(string gameID, long money) {
        //Chia ra lam 3 muc: Binh dan, Dai Gia, Ty Phu
        if (money < 5500) {
            return gameID + "1";
        }
        else if (money < 95000) {//dai Gia
            return gameID + "5";
        }
        else if (money < 500500) { //Ty phu
            return gameID + "10";
        }
        return gameID + "";
    }
    static long getMinMoneyCreateRoomByBet(int gameID, long currBet) {
        switch (gameID) {
            case kGameBaCayChuong:
                return currBet * 4;
                break;
            case kGameBaCay:
                return currBet * 4;
                break;
            case kGameChan:
                return currBet * 25;
                break;
            case kGameCoTuong:
                return currBet * 4;
                break;
            case kGameCoUp:
                return currBet * 4;
                break;
            case kGameLieng:
                return currBet * 4;
                break;
            case kGameSam:
                return currBet * 30;
                break;
            case kGameTaiXiu:
                return currBet * 4;
                break;
            case kGamePhom:
                return currBet * 15;
                break;
            case kGameTienLenMienBac:
                return currBet * 4;
                break;
            case kGameTienLenMienNam:
                return currBet * 4;
                break;
            case kGameTLMNDemLa:
                return currBet * 20;
                break;
            case kGameTomCuaCa:
                return currBet * 4;
                break;
            case kGameXiTo:
                return currBet * 4;
                break;
            default:
                break;
        }
        return currBet * 4;
    }
    static string getTypeRoom(double money){
        if (money < 5500){
            return "Bình dân";
        }
        else if (money < 95000){
            return "Triệu phú";
        }
        else if (money < 500500){
            return "Đại gia";
        }
        return "";
    }
    static string filterForbidenChat(string mess) {
        std::transform(mess.begin(), mess.end(), mess.begin(), ::tolower);
        for (int i = 0; i < 22; i++) {
            while (strstr(mess.c_str(),listf[i].c_str())){
                mess = replaceAll( mess, listf[i], string("***"));
                continue;
            }
        }
        return mess;
    }
    static std::string replaceAll( std::string const& original, std::string const& from, std::string const& to )
    {
        std::string results;
        std::string::const_iterator end = original.end();
        std::string::const_iterator current = original.begin();
        std::string::const_iterator next = std::search( current, end, from.begin(), from.end() );
        while ( next != end ) {
            results.append( current, next );
            results.append( to );
            current = next + from.size();
            next = std::search( current, end, from.begin(), from.end() );
        }
        results.append( current, next );
        return results;
    }
    
    static std::string getRoomIDByName(string sName){
        vector<string> lstInfo = mUtils::splitString(sName, '_');
        if (lstInfo.size() != 3)
            return "";
        return lstInfo.at(2);
    }
    static bool isNumber(const std::string& s)
    {
        return( strspn( s.c_str(), "-.0123456789" ) == s.size() );
    }
    static bool checkAlpha(const string &str){
        if (std::string::npos != str.find_first_of("0123456789")) {
            return true;
        }
        return false;
    }
    static string getMD5KeyHis(string username){
        string key = username + "2" + "81d2f3518c8595d6dc1a1c7fc842d5c0" + "1986";
        string strKey = md5(key);
        return strKey;
    }
    static string getMD5KeyInApp(string tk){
        string strKey = md5(tk);
        return strKey;
    }
    
    static string formatMoneySymbol(double money)
    {
        if (money < 10000)
        {
            string rs = mUtils::convertMoneyEx(money);
            return rs;
        }
        int digits = 1;
        vector<currencyStruct> arrSi;
        for (int i = 0; i < 2; i++)
            arrSi.push_back(currencyStruct());
        
        arrSi[1].symbolStruct = "K";
        arrSi[1].valueStruct = 1000;
        
        arrSi[0].symbolStruct = "M";
        arrSi[0].valueStruct = 1000000;
        
        for (unsigned int i = 0; i < arrSi.size(); ++i)
        {
            if (money >= arrSi.at(i).valueStruct)
            {
                float val = money / arrSi.at(i).valueStruct;
                if (fmodf(money, 1000) == 0)
                {
                    if (val - (int)val > 0.0)//have decimal point
                        return StringUtils::format("%.1f", val) + " " + arrSi[i].symbolStruct;
                    else
                        return StringUtils::format("%.f", val) + " " + arrSi[i].symbolStruct;
                }
                
                return StringUtils::format("%.2f", val) + " " + arrSi[i].symbolStruct;
            }
        }
        return boost::to_string(money);
    }
    
    static void setSoundConfig(bool isOn)
    {
        UserDefault::getInstance()->setBoolForKey("sound", isOn);
        UserDefault::getInstance()->flush();
    }
    static bool isHaveEvent()
    {
        if (dataManager.getAppConfig().events == "")
            return 0;
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (lstEvent.size()>0)
            return true;
        else
            return false;
    }
    static int eventNum()
    {
        if (dataManager.getAppConfig().events == "")
            return 0;
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        return lstEvent.size();
    }
    static bool isEventThangGame(int num)
    {
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return false;
            else{
                auto dieukien = mUtils::splitString(eventDetail[5],'_');
                for (int j = 0; j < dieukien.size(); j++){
                    if (dieukien[j] == "1"){
                        return true;
                    }
                }
            }
            
        }
        return false;
    }
    static bool isEventThuagGame(int num)
    {
        
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return false;
            else{
                auto dieukien = mUtils::splitString(eventDetail[5],'_');
                for (int j = 0; j < dieukien.size(); j++){
                    if (dieukien[j] == "2"){
                        return true;
                    }
                }
            }
            
        }
        return false;
    }
    static bool isEventNoHuGame(int num)
    {
        if (dataManager.getAppConfig().events == "")
            return 0;
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return false;
            else{
                auto dieukien = mUtils::splitString(eventDetail[5],'_');
                for (int j = 0; j < dieukien.size(); j++){
                    if (dieukien[j] == "3"){
                        return true;
                    }
                }
            }
            
        }
        return false;
    }
    static int getEventType(int num)
    {
        if (dataManager.getAppConfig().events == "")
            return 0;
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return 0;
            else{
                return atoi(eventDetail[0].c_str());
            }
    }
    }
    static string getEventName(int num)
        {
            if (dataManager.getAppConfig().events == "")
                return "";
            //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
            auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
            if (num < lstEvent.size()){
                auto eventDetail = mUtils::splitString(lstEvent[num],'|');
                if (eventDetail.size()<6)
                    return "";
                else{
                    return eventDetail[2];
                }
        }
    }
            static int getEventTypeReward(int num)
            {
                if (dataManager.getAppConfig().events == "")
                    return 0;
                //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
                auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
                if (num < lstEvent.size()){
                    auto eventDetail = mUtils::splitString(lstEvent[num],'|');
                    if (eventDetail.size()<6)
                        return 0;
                    else{
                        return atoi(eventDetail[1].c_str());
                    }
                }
            }
    static string getEventDescription(int num)
    {
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return "";
            else{
                return eventDetail[3];
            }
        }
    }
    static string getEventImage(int num)
    {
        //loại event | loại nhận thưởng | tên event | mô tả event | ảnh event | điều kiện event;
        auto lstEvent = mUtils::splitString(dataManager.getAppConfig().events,';');
        if (num < lstEvent.size()){
            auto eventDetail = mUtils::splitString(lstEvent[num],'|');
            if (eventDetail.size()<6)
                return "";
            else{
                return eventDetail[4];
            }
        }
    }
};
#endif
