//
//  AllData.h
//  iCasino_v2
//
//  Created by Thanhhv on 5/14/14.
//
//

#ifndef iCasino_v2_AllData_h
#define iCasino_v2_AllData_h

#include <iostream>
#include <vector>
using namespace std;

static string LINK_CONFIG = "https://gist.githubusercontent.com/kimnamcham/a55d4e41daa6761d97eb/raw/87d514d80e3f24e49784dea46d22f20a96cea61f/ip.txt";

#define WIDTH_DESIGN  1920
#define HEIGHT_DESIGN 1080
//he so so tien toi thieu khi tao ban
static int RATIO_MONEY_VAL = 4;
static float TIME_RESET_GAME = 4;
static int TIME_BAO_SAM = 15; //tg bao sam la 15s
static int kNormalFontSize = 20;
static int TIME_ALERT_CHESS = 20;
static string listf[] = {"sảnh rồng",
    "sanh rong",
    "sảnh rong",
    "sanh rồng",
    "dkm",
    "vkl",
    "vãi lồn",
    "địt cụ mày",
    "vãi cả lồn",
    "dis bà mày",
    "dis cụ mày",
    "dis mẹ mày",
    "địt mẹ mày",
    "sanhrong.com",
    "sanhrong",
    "sảnh rông",
    "sanh rông",
    "sảnhrồng",
    "sảnhrông",
    "sanhrông",
    "sanhrong",
    "sảnhrong"};
static string listemo[] = {":chat1:",
    ":chat2:",
   ":chat3:",
    ":chat4:",
    ":chat5:",
    ":chat6:",
    ":chat7:",
    ":chat8:",
    ":chat9:",
    ":chat10:",
    ":chat11:",
    ":chat12:",};
struct infoUser
{
	string userId;
	string userName;
	string userMoney;
	string userLv;
	string userRateWin;
	string userAvatar;

    infoUser(){
        userId = "";
        userName = "";
        userMoney = "";
        userLv = "";
        userRateWin = "";
        userAvatar = "";
    }
};

struct infoCat
{
	string name;
	int idGame;
	string url;
};

struct RejoinInfoStruct{
	string gameID;
	int roomID;
	string roomName;
	string pass;
	bool isNeedRejoin;
	RejoinInfoStruct(){
		gameID = "";
		pass = "";
		roomID = -1;
		roomName = "";
		isNeedRejoin = false;
	}
};

enum
{
    kGameBaCay = 100,
    kGameBaCayChuong,
    kGamePhom,
    kGameXiTo,
    kGameTienLenMienBac,
    kGameTienLenMienNam,
    kGameLieng,
    kGameCoTuong,
    kGameTomCuaCa,
    kGameChan,
	kGameCoUp,// = 110,
	kGameSam ,//= 111,
	kGameTLMNDemLa ,//= 112,
	kGameTaiXiu, //= 113,
	kGameXocDia,
	kGameQuayThuong = 115,
	kGameDemLaSolo = 116, //= 116,
	kGameSamSolo = 117,// = 117
    kGameTaiXiuMini = 128,
	kGameBauTom = 133,
    kGameMauBinh = 137,
    kGamePoker = 138,
    kGameSlot = 139,
    kGameCatte = 140,
    kGame3Doi = 250
};

// User
enum
{
    kUserLeft = 0,
    kUserRight, // 1
    kUserTop,   // 2
    kUserBot,   // 3
    kUserMe,     // 4
    kUserTopLeft,   // 5
    kUserTopRight,   // 6
    kUserBotRight   //7
};

// Button tag
enum
{
    //phom
    kTagButtonReady = 0,
	kTagButtonUnReady,
    kTagButtonSort,
    kTagButtonHit,
    kTagButtonGive,
    kTagButtonEat,
    kTagButtonHaPhom,
    kTagButtonPush,
    kTagButtonU,
	kTagButtonPushMulti, // cac button nay cho phep User lua chon gui vao phom nao
    //tienlen
    kTagButtonNextTurn,
	kTagButtonSitting,
	kTagButtonStandUp,
	kTagButtonBack,
	kTagButtonBaoSam,
	kTagButtonHuySam,
	// CoTuong
	kXeXanh,
	kMaXanh,
	kTuongjXanh,
	kSiXanh,
	kTuongXanh,
	kPhaoXanh,
	kTotXanh,
	kUpXanh,
	kXeDo,
	kMaDo,
	kTuongjDo,
	kSiDo,
	kTuongDo,
	kPhaoDo,
	kTotDo,
	kUpDo,
    kTagButtonFold,
    kTagButtonDraw
};

//Const of dautv
enum 
{
	USER_ME,
	USER_RIGHT_BOT,
	USER_RIGHT_TOP,
	USER_LEFT_TOP,
	USER_LEFT_BOT
};

enum
{
	DTAG_BTN_READY = 0,
	DTAG_BTN_UNREADY,
	DTAG_BTN_BET,
	DTAG_BTN_TURNALL,
	DTAG_BTN_VIEW,
	DTAG_BTN_SQUEEZ,
	DTAG_BTN_FOLLOW,
	DTAG_BTN_FOLD,
	DTAG_BTN_BET_HAFT,
	DTAG_BTN_BET_QUATAR,
	DTAG_BTN_BET_X2,
	DTAG_BTN_BET_ALL,
	DTAG_BTN_GIVE,
	DTAG_BTN_COMPLETE,
	DTAG_BTN_VICNTF,
	DTAG_BTN_JOIN,
	DTAG_BTN_STANDUP,
    DTAG_BTN_MIN,
    DTAG_BTN_3BB,
    DTAG_BTN_POT,
    DTAG_BTN_ALLIN

};

enum
{
    DTAG_BTN_POKER_BET=1,
    DTAG_BTN_POKER_FOLLOW =5 ,
    DTAG_BTN_POKER_FOLD=0,
    DTAG_BTN_POKER_BET_ALL=9,
    DTAG_BTN_POKER_GIVE = 2,
    DTAG_BTN_POKER_POT_HALF = 4,
    DTAG_BTN_POKER_POT = 7,
    DTAG_BTN_POKER_MIN = 10

};

enum
{
	CTAG_BTN_READY = 0,
    CTAG_BTN_DISCARD,
    CTAG_BTN_DRAWCARD,
    CTAG_BTN_DUOICARD,
    CTAG_BTN_TAKECARD,
    CTAG_BTN_BAO_U,
    CTAG_BTN_CHIUCARD,
	CTAG_BTN_SIT_DOWN
};

//@ndtuan
//3cay
enum
{
    kuser0 = 0,//me
    kuser1,
    kuser2,
    kuser3,
    kuser4,
    kuser5,
    kuser6    
};

// @thanhhv
// Đặt những hàm được sử dụng thường xuyên ở đây
class DoTogether {
public:
    DoTogether();
    ~DoTogether();
    
    void releaseVector(vector<int> &arr);
    void releaseVector(vector<string> &arr);
};

 //SERVICE GAME NAME
static string SERVICE_PHOM = "102";
static string SERVICE_LIENG = "106";

 
static string SERVICE_BACAY_CHUONG = "101";
static string SERVICE_BACAY_NORMAL = "100";
static string SERVICE_XI_TO = "103";
static string SERVICE_TLMN = "105";
static string SERVICE_TLMB = "104";
static string SERVICE_CHESS = "107";
static string SERVICE_TOM_CUA_CA = "108";
static string SERVICE_CHAN = "109";
static string SERVICE_CO_UP = "110";
static string SERVICE_SAM = "111";
static string SERVICE_TLDL = "112";
static string SERVICE_TAI_XIU = "113";
static string SERVICE_CHAT_ROOM = "118";
 //
//static string EXT_EVENT_LISTCARD_NTF = "lcntf";
//static string EXT_EVENT_START = "s";
//static string EXT_EVENT_END = "e";
//static string EXT_EVENT_READY_REQ = "rr";
//static string EXT_EVENT_READY_RES = "rdres";
//static string EXT_EVENT_UNREADY_REQ = "urr";
//static string EXT_EVENT_ERROR_READY_NTF = "e_rntf";
//static string EXT_EVENT_READY_NTF = "rntf";
//static string EXT_EVENT_UNREADY_NTF = "urntf";
//static string EXT_EVENT_TURNUP_CARD = "tuc";
//static string EXT_EVENT_LIST_USER_UPDATE = "luu";
//static string EXT_EVENT_GAME_RESULT = "grs";
//static string EXT_EVENT_GAME_BET_REQ = "gbr";
//static string EXT_EVENT_GAME_BET_ERROR_NOTIFY = "e_bntf";
//static string EXT_EVENT_GAME_BET_NTF = "gb_ntf";
//static string EXT_EVENT_DEAL_CARD_REQ = "dcrq";
//static string EXT_EVENT_DEAL_CARD_RES = "dcrs";
//static string EXT_EVENT_SELECT_OPEN_CARD_REQ = "slstrq";
//static string EXT_EVENT_SELECT_OPEN_CARD_RES = "slsrrs";
//static string EXT_EVENT_RAISE_REQ = "rsrq";
//static string EXT_EVENT_RAISE_RES = "rsrs";
//static string EXT_EVENT_FOLD_REQ = "fdrq";
//static string EXT_EVENT_FOLD_RES = "fdrs";
/*
 Ti.App.EXT_EVENT_OPEN_LAST_CARD_REQ = "olstrq";
 Ti.App.EXT_EVENT_OPEN_LAST_CARD_RES = "olstrs";
 Ti.App.EXT_EVENT_MOVE = "move";
 Ti.App.EXT_EVENT_MOVE_ERROR = "merr";
 Ti.App.EXT_EVENT_MOVE_NTF = "mntf";
 Ti.App.EXT_EVENT_FOR_PEACE = "fp";
 Ti.App.EXT_EVENT_FOR_PEACE_NTF = "fpntf";
 Ti.App.EXT_EVENT_IM_LOSE = "iml";
 Ti.App.EXT_EVENT_REPLY_FOR_PEACE = "rfp";
 Ti.App.EXT_EVENT_CHESS_TABLE_NTF = "ctbl";
 Ti.App.EXT_EVENT_NEXT_TURN = "nt";
 Ti.App.EXT_EVENT_KICK_REQ = "kckrq";
 Ti.App.EXT_EVENT_KICK_RES = "kckrs";
 Ti.App.EXT_EVENT_VICTORY_NOTIF = "vicntf";
 Ti.App.EXT_EVENT_GAME_CHANGE_NOTIF = "gchntf";
 Ti.App.EXT_EVENT_USER_JOIN_NOTIF = "jrntf";
 Ti.App.EXT_EVENT_USER_LEAVE_NOTIF = "lrntf";
 Ti.App.EXT_EVENT_USER_READY_NOTIF = "rdyntf";
 Ti.App.EXT_EVENT_DEAL_CARD_NOTIF = "dlcntf";
 Ti.App.EXT_EVENT_START_GAME_NOTIF = "strntf";
 Ti.App.EXT_EVENT_NEAD_RAISE_NOTIF = "ndrntf";
 Ti.App.EXT_EVENT_RAISE_NOTIF = "rsntf";
 Ti.App.EXT_EVENT_END_GAME_NOTIF = "endntf";
 Ti.App.EXT_EVENT_OPEN_CARD_NOTIF = "opcntf";
 Ti.App.EXT_EVENT_OPEN_ALL_CARD_NOTIF = "opaltf";
 Ti.App.EXT_EVENT_PLAY_CARD_REQ = "plcrq";
 Ti.App.EXT_EVENT_PLAY_CARD_RES = "plcrs";
 Ti.App.EXT_EVENT_PASS_CARD_REQ = "pscrq";
 Ti.App.EXT_EVENT_PASS_CARD_RES = "pscrs";
 Ti.App.EXT_EVENT_NEAD_PLAY_NOTIF = "ndpntf";
 Ti.App.EXT_EVENT_PLAY_CARD_NOTIF = "plcntf";
 Ti.App.EXT_EVENT_PASS_CARD_NOTIF = "pscntf";
 Ti.App.EXT_EVENT_AMF_TEST_NOTIF = "cblltf";
 Ti.App.EXT_EVENT_USER_EXPIRE_MONEY_NOTIF = "expmntf";
 
 Ti.App.EXT_FIELD_USERID = "uid";
 Ti.App.EXT_FIELD_POSITION = "p";
 Ti.App.EXT_FIELD_IS_TURN_UP_ALL = "tua";
 Ti.App.EXT_FIELD_LIST_CARD = "lc";
 Ti.App.EXT_FIELD_LIST_USER = "lu";
 Ti.App.EXT_FIELD_ERROR_STRING = "eexp";
 Ti.App.EXT_FIELD_RESULT_GAME = "rg";
 Ti.App.EXT_FIELD_GAME_BET_VALUE = "gbv";
 Ti.App.EXT_FIELD_ERROR_CODE = "errc";
 Ti.App.EXT_FIELD_GAME_BET_TYPE = "gbt";
 Ti.App.EXT_FIELD_BET_TYPE = "bet";
 Ti.App.EXT_FIELD_BET_TYPE_ALLOW = "betal";
 Ti.App.EXT_FIELD_BET_VALUE = "betvl";
 Ti.App.EXT_FIELD_BET_TOTAL = "bettt";
 Ti.App.EXT_FIELD_CURRENT_BET = "crbet";
 Ti.App.EXT_FIELD_GAME_ADD_BET_VALUE = "gabv";
 Ti.App.EXT_FIELD_CARD_ID = "crdvl";
 Ti.App.EXT_FIELD_MOVE_FROM = "mf";
 Ti.App.EXT_FIELD_MOVE_TO = "mt";
 Ti.App.EXT_FIELD_REPLY = "rep";
 Ti.App.EXT_FIELD_TABLE_CHESS = "tblc";
 Ti.App.EXT_FIELD_WIN_TYPE = "wt";
 Ti.App.EXT_FIELD_RESULT = "rslt";
 Ti.App.EXT_FIELD_GAME_INFO = "ginf";
 Ti.App.EXT_FIELD_GAME_EVENT = "gchginf";
 Ti.App.EXT_FIELD_CARD_OPEN = "cropn";
 Ti.App.EXT_FIELD_VICTORY_TYPE = "vttp";
 Ti.App.EXT_FIELD_IS_FIRST_ROUND = "isfr";
 Ti.App.EXT_FIELD_VICTORY_POS = "vtps";
 // lý do trừ tiền
 // Ti.App.EXT_FIELD_CHANGE_BALANCE_TYPE = "cbt";
 ////////////////////////////
 // phanpc - Phom
 Ti.App.EXT_EVENT_REQ_TAKE_CARD = "rqtkc";
 // ăn bài
 Ti.App.EXT_EVENT_REQ_DISCARD = "rqhofc";
 // đánh bài
 Ti.App.EXT_EVENT_REQ_HANDOFF_PHOM = "rqhofp";
 // hạ phỏm
 Ti.App.EXT_EVENT_REQ_DRAW_CARD = "rqdrwc";
 // Bốc bài
 Ti.App.EXT_EVENT_REQ_DUOI_CARD = "rqduoic";
 Ti.App.EXT_EVENT_REQ_JOIN_GAME = "rqjg";
 Ti.App.EXT_EVENT_REQ_LEAVE_GAME = "rqlg";
 Ti.App.EXT_EVENT_REQ_ORDER_CARDS = "rodrc";
 Ti.App.EXT_EVENT_REQ_END_PLAYING = "rqenpl";
 Ti.App.EXT_EVENT_REQ_SET_BET = "rqsbet";
 // set bet of the game
 Ti.App.EXT_EVENT_REQ_U = "rqU";
 // send request notification U to server
 Ti.App.EXT_EVENT_REQ_PUSH_CARD = "rqpusc";
 // đẩy quân bài đi - kiểu gửi bài
 
 Ti.App.EXT_EVENT_RES_TAKE_CARD = "rstkc";
 // ăn bài
 Ti.App.EXT_EVENT_RES_DISCARD = "rshofc";
 // đánh bài
 Ti.App.EXT_EVENT_RES_HANDOFF_PHOM = "rshofp";
 // hạ phỏm
 Ti.App.EXT_EVENT_RES_DRAW_CARD = "rsdrwc";
 // Bốc bài
 Ti.App.EXT_EVENT_RES_JOIN_GAME = "rsjg";
 Ti.App.EXT_EVENT_RES_LEAVE_GAME = "rslg";
 Ti.App.EXT_EVENT_RES_ORDER_CARDS = "rsodrc";
 Ti.App.EXT_EVENT_RES_END_PLAYING = "rsenpl";
 Ti.App.EXT_EVENT_RES_SET_BET = "rssbet";
 // set the bet of the game
 Ti.App.EXT_EVENT_RES_U = "rsU";
 // send response notification U to client
 Ti.App.EXT_EVENT_RES_PUSH_CARD = "rspusc";
 // đẩy quân bài đi - kiểu gửi bài
 
 // notify from server
 Ti.App.EXT_SRVNTF_PRIVATE_MESSAGE = "ntfprvmsg";
 // send message to specified user
 Ti.App.EXT_SRVNTF_CURRENT_PLAYING_USER = "ntfcurpu";
 // send message to all users "ntfcurpu"
 Ti.App.EXT_SRVNTF_PUBLIC_MESSAGE = "ntfpubmsg";
 // send message to public msg box
 Ti.App.EXT_SRVNTF_BROADCAST_MESSAGE = "brcstmsg";
 // send message to each user
 Ti.App.EXT_SRVNTF_END_PLAYING_GAME = "ntfepla";
 // send end playing message to user
 Ti.App.EXT_SRVNTF_GAME_INFO = "ntfginfo";
 // send game info to all client
 Ti.App.EXT_SRVNTF_PLAYER_LIST = "ntfpllst";
 // send game's player list
 Ti.App.EXT_SRVNTF_GAME_MASTER_INFO = "ntfgminfo";
 // notify to all user about the master of the game
 Ti.App.EXT_SRVNTF_USER_READY = "ntfurd";
 Ti.App.EXT_SRVNTF_CARD_ORIGINATION = "ntfcorg";
 // public notify user about originating card
 Ti.App.EXT_SRVNTF_CAN_TAKE = "ntfcantk";
 // player replay
 Ti.App.EXT_SRVNTF_PLAYER_REPLAY = "ntfrpl";
 // player U
 Ti.App.EXT_SRVNTF_U = "ntfu";
 Ti.App.EXT_SRVNTF_U_9 = "ntfu9";
 // notify recommended cardsets for user
 Ti.App.EXT_SRVNTF_RECOMMENDED_CARDSETS = 'ntfrcrds';
 // FILED phanpc
 Ti.App.EXT_FIELD_TXT_MSG = "txtmsg";
 Ti.App.EXT_FIELD_CARD_INDEX = "cardidx";
 Ti.App.EXT_FIELD_BET_VAL = "betval";
 Ti.App.EXT_FIELD_U_RANK = "urnk";
 Ti.App.EXT_FIELD_GAME_TURN = "gturn";
 Ti.App.EXT_FIELD_FROM_USERID = "fuid";
 Ti.App.EXT_FIELD_TO_USERID = "tuid";
 Ti.App.EXT_FIELD_FROM_USERNAME = "funa";
 Ti.App.EXT_FIELD_TO_USERNAME = "tuna";
 Ti.App.EXT_FIELD_CARD_ORIGINATION = "crdorg";
 Ti.App.EXT_FIELD_RESULT_CODE = "rscode";
 Ti.App.EXT_FIELD_CARDSETS = 'crdss';
 //
 Ti.App.CARD_ORIGINATION_BY_TAKEN = 1;
 // ăn được
 Ti.App.CARD_ORIGINATION_BY_DRAWN = 2;
 // bốc từ nọc
 Ti.App.CARD_ORIGINATION_BY_TRANSFERED = 3;
 // quân bài chuyển
 Ti.App.CARD_ORIGINATION_BY_DEALT = 4;
 // chia bài
 Ti.App.CARD_ORIGINATION_BY_HANDOFF = 5;
 // chia bài
 Ti.App.CARD_ORIGINATION_BY_PUSHED = 6;
 // chia bài
 Ti.App.CARD_ORIGINATION_BY_UNKOWN = 7;
 // chia bài
 
 //*********************CHẮN MESSAGE************************************/
/*
//Dưới bài
Ti.App.EXT_EVENT_RES_DUOI_CARD = "rsduoic";
// thông báo có 1 người đang chờ chíu
Ti.App.EXT_SRVNTF_ONE_EXPECTING_CHIU = "ntfoechiu";
// thông báo có 1 người chíu Ù
Ti.App.EXT_SRVNTF_ONE_EXPECTING_CHIU_U = "ntfoechiuu";
//Báo ăn báo
Ti.App.EXT_SRVNTF_ANBAO = "ntfanbao";
//Một người chờ ù
Ti.App.EXT_SRVNTF_ONE_EXPECTING_U = "ntfoeu";
//Số lá bài còn lại trong nọc
Ti.App.EXT_SRVNTF_NOC_COUNT = "ntfnoccnt";
//Chíu bài
Ti.App.EXT_EVENT_REQ_CHIU_CARD = "rqchiuc";
//Xướng Ù
Ti.App.EXT_EVENT_REQ_U_ANNOUCE = "rquanou";
//Server trả về khi xướng
Ti.App.EXT_EVENT_RES_U_ANNOUCE = "rsuanou";
//Yêu cầu xem nọc khi có người Ù
Ti.App.EXT_EVENT_REQ_NOC_DETAIL = "rqnocdtl";
//Số bài còn lại trong nọc
Ti.App.EXT_SRVNTF_NOC_DETAIL = "ntfnocdetl";
//Yêu cầu xem bài trên tay người Ù:
Ti.App.EXT_EVENT_REQ_TRENTAY_DETAIL = "rqttdtl";
//Chi tiết số bài trên tay người Ù
Ti.App.EXT_SRVNTF_TRENTAY_DETAIL = "ntfttdetl";

/**
 *Các lỗi ăn báo
 *
 *
Ti.App.ANBAO_REASON_NO_PROBLEM = 0;
Ti.App.ANBAO_REASON_AN_CA_DOI_CHO = 1;
//
Ti.App.ANBAO_REASON_BO_AN_CUATREN_BUT_AN_CUATRI = 2;
Ti.App.ANBAO_REASON_DISCARD_SAMEAS_CUATREN_CUATRI_DUOITAY = 3;
Ti.App.ANBAO_REASON_ANCA_DANHCA = 4;
Ti.App.ANBAO_TREOTRANH = 5;
// treo tranh
Ti.App.ANBAO_DANH_1_CA_CHI_DUOC_AN_CHAN = 6;
Ti.App.ANBAO_REASON_BOCHAN_ANCA = 7;
Ti.App.ANBAO_REASON_DANHBAI_GIONG_CHANCA_DA_AN = 8;
Ti.App.ANBAO_REASON_DOI_U_BACHTHUCHI = 9;
// Lỗi đang đợi Ù bạch thủ chi chỉ đc ăn chắn CHi
Ti.App.ANBAO_REASON_BOCHAN_DANHCHAN = 10;
// bỏ không ăn 1 quân làm chắn, sau lại đánh đi đúng cây đó.
Ti.App.ANBAO_REASON_BOCHAN_ANCHAN = 11;
// bỏ chắn rồi lại ăn chắn
Ti.App.ANBAO_REASON_BOCA_ANCA = 12;
// bỏ cạ rồi lại ăn cạ
Ti.App.ANBAO_REASON_DANHCA_ANCA = 13;
// đánh đi 1 cạ sau đó lại ăn 1 cạ khác
Ti.App.ANBAO_REASON_XECA_ANCA = 14;
// xé cạ ăn cạ
Ti.App.ANBAO_REASON_XECHAN_ANCA = 15;
// xé chắn, ăn chắn
Ti.App.ANBAO_REASON_DANH_ROILAI_AN = 16;
// đánh đi 1 cây rồi ăn lại chính cây đó
Ti.App.ANBAO_REASON_DANH_DI_DOI_CHAN = 17;
// đánh đi cả 1 chắn (2 cây)
Ti.App.ANBAO_REASON_AN_ROILAI_DANH = 18;
// ăn 1 cậy rồi lại đánh ra chính cây đó
Ti.App.ANBAO_REASON_ANCA_ROILAI_DANH_QUAN_CUNG_HANG = 19;
// ăn 1 cạ rồi lại đánh đi 1 cây cùng hàng
Ti.App.ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG = 20;
// chíu được nhưng lại ăn thường
Ti.App.ANBAO_REASON_AN_CHON_CA = 21;
// Lấy 1 quân trong cạ sẵn có hoặc trong 3 đầu để ăn thành cạ.
Ti.App.ANBAO_REASON_CO_CHAN_CAU_CA = 22;
// có chắn cấu cạ: Lấy 1 quân trong chắn sẵn có để ăn cạ.

//*********************CHẮN RESUILT CODE*******************************
Ti.App.GAME_RSCODE_NOT_CHAN_CA_SET = 32;

/**
 *Nguồn gốc các lá bài chắn
 *
Ti.App.CARD_ORIGINATION_UNKOWN = -1;
Ti.App.CARD_ORIGINATION_CHIA_BAI = 0;
Ti.App.CARD_ORIGINATION_BOC_NOC = 1;
Ti.App.CARD_ORIGINATION_AN_CUA_TREN = 2;
Ti.App.CARD_ORIGINATION_AN_CUA_TRI = 3;
Ti.App.CARD_ORIGINATION_CHIU = 4;
Ti.App.CARD_ORIGINATION_TRA_CUA = 5;
Ti.App.CARD_ORIGINATION_BY_DISCARD = 6;
// discard
Ti.App.CARD_ORIGINATION_BY_DUOI = 7;
// đánh đi cây bài vừa bốc nọc ở cửa trì
// Ti.App.CARD_ORIGINATION_BY_AN_DUOI = 8;
Ti.App.CARD_ORIGINATION_BY_TRANSFER_TREN_2_DUOI = 9;
//*********************GAME_ZONE_SERVICES -- MESSAGE_TYPE****************
Ti.App.REQUEST_RESGISTER = "rr";
Ti.App.REGISTER_CMD = "rg";
//*********************GAME_ZONE_SERVICES -- FIELDS****************
Ti.App.ACC_ID = "aI";
Ti.App.ACC_NAME = "aN";
Ti.App.ACC_PASS = "aP";
Ti.App.ACC_MAIL = "aM";
Ti.App.ACC_TYPE = "aT";
Ti.App.RESULT_CODE = "rc";
Ti.App.RESULT_DESC = "rd";
Ti.App.ACC_NEW_PASS = "aNP";

//GAME_RESULT_CODE
Ti.App.GAME_SUCCESS = 0;
Ti.App.PLAYER_NOT_EXIST = 1;
Ti.App.ROOM_NOT_EXIST = 2;
Ti.App.TABLE_NOT_EXIST = 3;
Ti.App.TABLE_MAX_PLAYER = 4;
Ti.App.ACTION_NOT_ALLOW = 5;
Ti.App.PLAYER_ALREADY_IN_TABLE = 6;
Ti.App.WAIT_OTHER_PLAYER_READY = 7;

//BET_TYPE
Ti.App.GAME_TABLE_STATUS_BET_FOLD = 0;
Ti.App.GAME_TABLE_STATUS_BET_RAISE = 1;
Ti.App.GAME_TABLE_STATUS_BET_NONE = 2;
Ti.App.GAME_TABLE_STATUS_BET_QUATER = 3;
Ti.App.GAME_TABLE_STATUS_BET_HAFT = 4;
Ti.App.GAME_TABLE_STATUS_BET_FOLLOW = 5;
Ti.App.GAME_TABLE_STATUS_BET_DOUBLE = 6;
Ti.App.GAME_TABLE_STATUS_BET_ALL = 7;
//WIN_XITO_TYPE
Ti.App.GAME_CARD_TYPE_NONE = 0;
Ti.App.GAME_CARD_TYPE_MAU_THAU = 1;
Ti.App.GAME_CARD_TYPE_DOI = 2;
Ti.App.GAME_CARD_TYPE_THU = 3;
Ti.App.GAME_CARD_TYPE_SAM_CO = 4;
Ti.App.GAME_CARD_TYPE_SANH = 5;
Ti.App.GAME_CARD_TYPE_THUNG = 6;
Ti.App.GAME_CARD_TYPE_CU_LU = 7;
Ti.App.GAME_CARD_TYPE_TU_QUY = 8;
Ti.App.GAME_CARD_TYPE_THUNG_PA_SANH = 9;
//ERROR CODE
Ti.App.RESCODE_ACTION_OK = 0;
Ti.App.NETADT_RESCODE_FAILED = 1;
Ti.App.NETADT_RESCODE_UNKOWN = 2;
Ti.App.NETADT_RESCODE_ACCMAN_NOT_REACHED = 3;
Ti.App.RESCODE_INVALID_INNER_USER_PASS = 10;
Ti.App.RESCODE_DUPLICATED_ACC = 11;
Ti.App.RESCODE_CREATE_ACCOUNT_FAILED = 12;
Ti.App.RESCODE_NOT_EXISTED_ACC = 13;
Ti.App.RESCODE_UPDATE_ACCOUNT_FAILED = 14;
Ti.App.RESCODE_GET_ACCOUNT_FAILED = 15;
Ti.App.RESCODE_UPDATE_AMOUNT_FAILED = 16;
Ti.App.RESCODE_GET_AMOUNT_FAILED = 17;
Ti.App.RESCODE_LOCK_ACCOUNT_FAILED = 18;
Ti.App.RESCODE_UNLOCK_ACCOUNT_FAILED = 19;
Ti.App.RESCODE_GET_ACCOUNT_STATUS_FAILED = 20;
Ti.App.RESCODE_UPDATE_ACCOUNT_TYPE_FAILED = 21;
Ti.App.RESCODE_GET_ACCOUNT_TYPE_FAILED = 22;
Ti.App.RESCODE_UPDATE_PASS_FAILED = 23;
Ti.App.RESCODE_RESET_PASS_FAILED = 24;
Ti.App.RESCODE_LOGIN_FAILED = 25;
Ti.App.RESCODE_EXISTED_LOGIN_FAILED = 26;
Ti.App.RESCODE_INVALID_USER_PASS = 27;
Ti.App.RESCODE_LOGOUT_FAILED = 28;
Ti.App.RESCODE_NOT_ENOUGH_MONEY = 29;
Ti.App.RESCODE_NOT_ALL_PLAYER_READY = 30;
Ti.App.RESCODE_MOVE_INVALID = 31;

//        RESCODE _ PHANPC
Ti.App.PHOM_RSCODE_OK = 0;
// OK, success
Ti.App.PHOM_RSCODE_FAILED = Ti.App.PHOM_RSCODE_OK - 1;
// FAILED

Ti.App.PHOM_RSCODE_ERROR_ROOM_FULL = Ti.App.PHOM_RSCODE_OK + 1;
// ROOM FULL
Ti.App.PHOM_RSCODE_ERROR_GAME_FULL = Ti.App.PHOM_RSCODE_OK + 2;
// GAME FULL
Ti.App.PHOM_RSCODE_ERROR_CARD_NOT_IN_ON_HAND_LIST = Ti.App.PHOM_RSCODE_OK + 3;
// card không nằm trong trên tay list
Ti.App.PHOM_RSCODE_ERROR_HANDOFF_CARD_NOT_ALLOW = Ti.App.PHOM_RSCODE_OK + 4;
// không được phép discard
Ti.App.PHOM_RSCODE_ERROR_PLAYER_IN_END_STATE = Ti.App.PHOM_RSCODE_OK + 5;
// player đã ở trạng thái kết thúc
Ti.App.PHOM_RSCODE_ERROR_NOT_A_CARDSET = Ti.App.PHOM_RSCODE_OK + 6;
// danh sách quân bài không hợp thành 1 phỏm
Ti.App.PHOM_RSCODE_ERROR_NO_MELD_YET = Ti.App.PHOM_RSCODE_OK + 7;
// player chưa có phỏm nào (chưa hạ, hoặc bị móm)
Ti.App.PHOM_RSCODE_ERROR_OWN_CARDS_NUM_LESS_THAN_10 = Ti.App.PHOM_RSCODE_OK + 8;
// số lượng bài trên tay (bao gồm cả phỏm đã hạ, bài đã gửi) nhơ hơn 10 cây
Ti.App.PHOM_RSCODE_U_KHAN = Ti.App.PHOM_RSCODE_OK + 9;
// Ù khan
Ti.App.PHOM_RSCODE_U_9 = Ti.App.PHOM_RSCODE_OK + 10;
// Ù 9 cay
Ti.App.PHOM_RSCODE_U_10 = Ti.App.PHOM_RSCODE_OK + 11;
// Ù 10 cây - Ù trên tay
Ti.App.PHOM_RSCODE_U_0 = Ti.App.PHOM_RSCODE_OK + 12;
// Ù 0 điểm
Ti.App.PHOM_RSCODE_NOT_U = Ti.App.PHOM_RSCODE_OK + 13;
// không Ù
Ti.App.PHOM_RSCODE_INVALIDE_STATE = Ti.App.PHOM_RSCODE_OK + 14;
// action của player không được phép tại thời điểm đó
Ti.App.PHOM_RSCODE_CARD_ISNOT_LAST_DISCARD = Ti.App.PHOM_RSCODE_OK + 15;
// Card không phải là quân bài cuối cùng đc đánh ra trên bàn chơi
Ti.App.PHOM_RSCODE_INVALID_CARDSET = Ti.App.PHOM_RSCODE_OK + 16;
// danh sách quân bài không phải là phỏm
Ti.App.PHOM_RSCODE_INVALID_TURN = Ti.App.PHOM_RSCODE_OK + 17;
// player đánh bài không phải lượt (ko cho phép)
Ti.App.PHOM_RSCODE_ONHAND_CARD_SIZE_NOT_10 = Ti.App.PHOM_RSCODE_OK + 18;
// số lượng bài trên tay ko phải 10
Ti.App.PHOM_RSCODE_ERROR_OWN_CARDS_NUM_EQUAL_10 = Ti.App.PHOM_RSCODE_OK + 19;
// số lượng bài trên tay = 10
Ti.App.PHOM_RSCODE_ERROR_NO_MORE_CARD = Ti.App.PHOM_RSCODE_OK + 20;
// không còn bài trên nọc
Ti.App.PHOM_RSCODE_ERROR_PLAYER_NOT_IN_END_STATE = Ti.App.PHOM_RSCODE_OK + 21;
// player ko ở trạng thái kết thúc (đã oánh 4 cây đi)
Ti.App.PHOM_RSCODE_ERROR_PLAYER_ENDED_STATE_BEFORE = Ti.App.PHOM_RSCODE_OK + 22;
// player đã kết thúc chơi trước đây
Ti.App.PHOM_RSCODE_ERROR_PLAYER_ISNOT_ROOM_MASTER = Ti.App.PHOM_RSCODE_OK + 23;
// player không phải chủ bàn
Ti.App.PHOM_RSCODE_BETVAL_TOO_BIG = Ti.App.PHOM_RSCODE_OK + 24;
// giá trị bet quá lớn
Ti.App.PHOM_RSCODE_HAVE_JOINED_BEFRORE = Ti.App.PHOM_RSCODE_OK + 25;
// đã join (room or game) trước đây
Ti.App.PHOM_RSCODE_HAVENT_JOINED = Ti.App.PHOM_RSCODE_OK + 26;
// chưa join (room or game) trước đây

/**
 * dautv Update user info
 *
//Request lấy thông tin user
Ti.App.REQUEST_GET_ACC_INFOR = "rgaci";
//Kết quả trả về thông tin user
Ti.App.GETACCINFOR_CMD = "gaic";
//Requast thay đổi mật khẩu
Ti.App.REQUEST_CHANGE_PASS = "rcp";
//Kết quả trả về thay đổi mật khẩu
Ti.App.CHANGEPASS_CMD = "cpc";
//Request Cập nhật tài khoản
Ti.App.REQUEST_UPDATE_ACC = "rua";
//Kết quả trả về khi cập nhật tài khoản
Ti.App.UPDATE_ACC_CMD = "uac";
//Request thêm bạn
Ti.App.REQUEST_ADD_FRIEND = "raaf";
// Response kết bạn
Ti.App.ADD_FRIEND_CMD = "aafc";
//Requset hủy kết bạn
Ti.App.REQUEST_REMOVE_FRIEND = "rarf";
// Kết quả trả về khi hủy kết bạn
Ti.App.REMOVE_FRIEND_CMD = "arfc";
//Request lấy danh sách bạn bè
Ti.App.REQUEST_LIST_FRIEND = "rali";
//Kết quả lấy list friend
Ti.App.LIST_FRIEND_CMD = "alic";

Ti.App.g_sUsername = '';
Ti.App.g_isStartGame = false;
Ti.App.g_isSoundOn = false;
Ti.App.g_isVibrate = true;
/////////////////////////////////////////
var modelLog = require('model/mLog');

//Create instance
Ti.App.g_mUtils = require('model/mUtils');
Ti.App.g_mLog = new modelLog();

var fakeTextField = require('controller/fakeTextField');
Ti.App.g_fakeTextField = new fakeTextField();

var vIndicatorWindow = require('view/vIndicatorWindow');
Ti.App.g_IndicatorWindow = new vIndicatorWindow();
//Create custom Toast
var CustomToast = require('controller/CustomToast');
Ti.App.g_customToast = new CustomToast();
//Create custom sound
var CustomSound = require('controller/CustomSound');
Ti.App.g_customSound = new CustomSound();

// Bien Dat font chu
Ti.App.g_fontSize = Ti.App.g_mUtils.getWidth() / 30;

// @thanhhv - zIndex
// Thời gian chạy
Ti.App.zIndexTimer = 60;
// hội thoại thông báo
Ti.App.zIndexChat = 61;
// view tiền
Ti.App.zIndexMoney = 62;

// size
var SizeMultiScreen = new (require('controller/SizeMultiScreen'))(1);
Ti.App.widthScreen = SizeMultiScreen.widthApp;
Ti.App.heightScreen = SizeMultiScreen.heightApp;
Ti.API.info('wid=' + Ti.App.widthScreen);
Ti.API.info('hei=' + Ti.App.heightScreen);
Ti.App.size = SizeMultiScreen.size;
};

module.exports = InitAllDatas;
*/
#define PI 3.141592653589


#define kNormalFont "__Roboto-Regular.ttf"
#define kBoldFont "__Roboto-Bold.ttf"
#define BET_RATIO 500


static char TL_CHAR_SPLIT_1 = '-';
static char TL_CHAR_SPLIT_2 = ':';
static char PHOM_CHAR_SPLIT_1 = ';';
static char PHOM_CHAR_SPLIT_2 = ':';//    const char c1 = ';';const char c2 = ':';


#endif
