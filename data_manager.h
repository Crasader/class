#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include <string>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;
using namespace cocos2d;

struct LevelInfo{
	int level;
	int currExp;
	int needExp;
	LevelInfo(){
		level = currExp = needExp = -1;
	}
};

struct AppConfig{
	std::string version_android;
	std::string version_ios;
	std::string version_win;
	std::string url_notification;
    std::string url_notification_cam;
    std::string url_notification_eng;
    std::string url_notification_cn;
	std::string ip_default;
	std::string url_list_ip;
	std::string url_list_ip_1;
	std::string callcenter;
    std::string callcenter2;
	std::string url_ios_store;
	std::string url_android_store;
	std::string url_upload;
	std::string url_avatar_upload;
	std::string url_giftcode;
	std::string sListHiddenGame;
    std::string sListVip;
    std::string moneyFB;
    std::string sListQuayThuong;
    std::string url_event;
    std::string url_event_cam;
    std::string url_event_eng;
    std::string url_event_cn;
    std::string loginType;
    std::string url_charge;
    
    std::string linkshare;
	std::string textshare;
    std::string titleshare;
    std::string imageshare;

    
    bool isLoginFisrt;
    bool isLoginFisrtDay;
    bool isreceivedRegisterFace;
    bool isreceivedLinkFace;
	bool isHideBilling;
	bool isHideTransfer;
	bool isHideExchange;
	bool isHideSMS;
	bool isHideTheCao;
    bool isOpenChest;
    bool isHideXacThucSDT;
    int NUM_REGISTER_DEVICE;
    bool isMissionFacebookClick;
    string strMinTranfer;
    string minjoin;
    string maintenancestr;
    string force_update;
    string versionAndroid;
    string versionIOS;
    string urlBonusIMG;
    string strListSMS;
    string huthuong;
    int delay_reqot;
    int delay_reqdt;
	int invitetime;//MINHDV - changes 4/5 - thoi gian hien thi popup thong bao moi choi lan dau tien
	int chatlevel;//check vip for game bai p2 - vip 0 khong duoc su dung chat, vip 1 thi duoc su dung emo, > vip2 duoc dung het
	string mailSup;//only for baimoc
	//config thoi gian cho viec mo quy tom cua ca
	string notifiQT;
	int timeToQT;
	int timetoNextWeek;
	int firstBetTCC;
	int tccLimitCount;//cờ cho việc được đặt tối đa bao nhiêu o tôm cua cá - default = 3

    string facebookappid;
	bool isShowLucky;//cờ cho việc show dãy số may mắn
    string urllinksdt;
    int chattimeout;
    int chattimemax;
    int maxxito;
	int maxlieng;
    bool isactiveOTP;
    int pheketsat;
    string versionforceupdate;
    vector<int> minungtien;
    string isfb;
    string tientomocp2;
    int delay_rqoffer;
    string gamemini;
    bool otp;
    bool ketsat;
    bool ungtien;
    bool ruongvip;
    int minVipDoiMoc;
    int minVipBanMoc;
    string phichuyentien;
    bool isNhapPass;
    int mintaoban;
    string hidefb;
    bool is24h;
    bool isSound;
    string list_game;
    string list_new_game;
    string events;
    bool isShowMatKhau;
    bool isShareCancel;
    bool isVN;
    int notifyZen;
    int notifyGold;
    string fanpage;
    int iap_rate ;
    int timeShowTX;
    bool isShowMatKhauDone;
    bool isBetWhenOpenDish;
    string exchangerate;
    string bankaccount;
    int numHuongDanTX;
    bool isPopupHuongDan;
    bool cellcard;
    bool isrecommend;
    bool isDoiThe;
    bool isChip;
    string url_ios_nostore;
    string STICKER_GAME_STR;
    string FISRT_CHARGE_PROMOTION_STR;
    bool isDownLoadResource;
	AppConfig(){
		version_android = "android01082016";
		version_ios = "ios01082016";
		version_win = "win29112016";
		url_notification = "";
        url_notification_cam = "";
        url_notification_eng= "";
        url_charge = "";
		ip_default = "";
		url_ios_store="";
		url_android_store = "";
		url_upload = "";
		url_avatar_upload = "";
		url_giftcode = "";
		isHideBilling = false;
		isHideTransfer = false;
		isHideExchange = false;
        isLoginFisrt = false;
        isLoginFisrtDay = false;
        isreceivedRegisterFace = false;
        isreceivedLinkFace = false;
        isOpenChest = false;
        loginType = "";
		sListHiddenGame = "";
        sListVip = "0,01_1_5_10_20_50_100_200_500_1000";
        moneyFB = "300-300-50";
        url_event = "";
        url_event_cam = "";
        url_event_eng = "";
        NUM_REGISTER_DEVICE = 1;
        isMissionFacebookClick = false;
        strMinTranfer = "3_50000";
        minjoin = "113_10000;114_10000;1_10000;2_10000;3_10000;4_1";
		invitetime = 3;
        maintenancestr = "";
        force_update = "";
        versionAndroid = "";
        versionIOS = "";
        urlBonusIMG = "";
        delay_reqot = 5;
        delay_reqdt = 5;
		chatlevel = 0;
        strListSMS= "1-8990-50000-NAP 50K-30000;2-8990-50000-NAP 50K-30000;3-8990-50000-NAP 50K-30000";
		mailSup = "supportbaimoc@gmail.com";
		notifiQT = "Sự kiện quay thưởng sẽ diễn ra vào 11h59 các ngày chủ nhật";
		timeToQT = 604799;
		timetoNextWeek = 86400;
        facebookappid = "";
		firstBetTCC = 1;
		isShowLucky = true;
        isHideXacThucSDT= false;
        urllinksdt= "";
        chattimeout = 0;
        chattimemax = 0;
		tccLimitCount = 3;
        maxxito = 80;
        maxlieng = 80;
        isactiveOTP = false;
        pheketsat = 10000;
        versionforceupdate = "";
        minungtien.clear();
        isfb = "100_50";
        delay_rqoffer = 1;
        linkshare = "";
        textshare = "";
        titleshare = "";
        imageshare = "";
        tientomocp2= "";
        gamemini = "";
        otp = false;
        ketsat = false;
        ungtien = false;
        ruongvip = false;
        phichuyentien = "";
        huthuong = "";
        minVipDoiMoc = 20000;
        minVipBanMoc = 3000000;
        isNhapPass = true;
        mintaoban = 5;
        callcenter2 = "";
        hidefb= "";
        is24h = false;
        isSound = false;
        list_game = "112_true;116_true;139_true;138_true;140_true;250_true";
        events = "";
        isShowMatKhau = false;
        isShareCancel = true;
        isVN = false;
        fanpage = "fb.com/NagaClub.Fanpage/";
        notifyZen = 10000;
        notifyGold = 10000;
        iap_rate = 2;
        timeShowTX = 5;
        isShowMatKhauDone = false;
        isBetWhenOpenDish = false;
        exchangerate = "";
        bankaccount= "";
        numHuongDanTX= 3;
        isPopupHuongDan= true;
        cellcard = true;
        isrecommend = true;
        url_event_cn= "";
        url_notification_cn = "";
        isDoiThe = false;
        isChip = false;
        url_ios_nostore = "";
        STICKER_GAME_STR = "";
        FISRT_CHARGE_PROMOTION_STR = "";
        isDownLoadResource = false;
	}
};

class DataManager {
public:
	DataManager(){}

	bool LoadStrings(const char* file);

	string GetSysString(int code);

	int getLevelProgress(int currLevel, int currExp);
    int getExpNeed(int currLevel, int currExp);

	AppConfig& getAppConfig();

	void saveAppConfig();

	std::string _sysStrings[20480];
	vector<LevelInfo> lstLevelInfos;
	AppConfig mAppConfig;
	static const char* unknown_string;
};

extern DataManager dataManager;

#endif // DATAMANAGER_H
