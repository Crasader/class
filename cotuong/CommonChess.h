#ifndef Common_h__
#define Common_h__

#include "cocos2d.h"
using namespace std;


#define FONT_SIZE_NORMAL_POPUP 40
#define FONT_SIZE_ITEM 40
#define FLAMENCO_FONT "UTM Flamenco.ttf"
///enum zorder for add child to running scene
#define DEFAULT_TIME_1TURN 90 //thoi gian default cho 1 luot danh
#define DEFAULT_TIME_TOTAL 80*15
#define DEFAULT_TIME_READY 15
#define DEFAULT_TIME_RESPONSE 15 //thoi gian tra loi co dong y cau hoa hay khong
#define TRANSITION_DURATION 0.2
//////////////////////////////////////////////////////////////////////////
#define TIME_DISABLE_BUTTON 5
#define DEFAULT_REMOVE_SELF 10
#define COLOR_ITEM_MISSION Color4B(241, 205, 99, 255)
#define CHESS_SIZE Size(99, 109)
#define SIZE_AVATAR Size(140, 140)
#define SIZE_ICONVIP Size(66,84)
#define DEFAULT_AVATAR "icon_default.png"
#define GUESS_OPACITY 100
#define MAX_CHAT_LENGTH 20//max so luong dong chat luu lai
#define COLOR_MYNAME_CELL Color3B::YELLOW
#define COLOR_DEFAULT_CELL Color3B::BLUE
#define BET_RATE 4
#define TIME_AUTO_REMOVE 7
#define SIZE_ITEM_CELL Size(120, 120)

static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 1920);

//enum TYPE_CARD{
//    NON = 0,
//    VINA,
//    MOBI,
//    VIETTEL
//};
struct TimeStruct{
	int dayMQ;
	int hourMQ;
	int minuteMQ;
	int secondMQ;
	TimeStruct(int _day, int _hour, int _min, int _se) :
		dayMQ(_day), hourMQ(_hour), minuteMQ(_min), secondMQ(_se){};
};
struct ItemStruct{
	int hero_id;
	string hero_name;
	string hero_desc;
	string hero_path;
	int hero_rate;
	bool hero_isbought;
	bool hero_isPicked;//co duoc chon hay ko
	int hero_cost;

	ItemStruct(){
		hero_id = -1;
		hero_name = "---";
		hero_desc = "---";
		hero_path = "";
		hero_rate = 0;
		hero_isbought = false;
		hero_isPicked = false;
		hero_cost = 1000;
	};
};

struct ChatInfoStruct{
	std::string userName;
	std::string userMsg;
	ChatInfoStruct(std::string _name, std::string _msg) :userName(_name), userMsg(_msg){};
};


enum LAYER_MAIN_ID
{
	LAYER_BOTTOM = 100,
	LAYER_MAINTOP,
	LAYER_HOME,
	LAYER_MISSION,
	LAYER_RANK,
	LAYER_SUPPORT,
	LAYER_MAIL,
	LAYER_USER_HERO,
	LAYER_ACCOUNT,
	LAYER_UPDATE_INFO,
	LAYER_SELECTCO,
	LAYER_PAYMENT,
	LAYER_SHOP,
	LAYER_SHOP_HERO,
	LAYER_SHOP_ITEM,
	LAYER_CREATE_ROOM,
	LAYER_PICKROOM,
	LAYER_FRIENDS,
	LAYER_INVITE_FRIENDS,
	LAYER_RUONG,
	LAYER_WEBVIEW,
	LAYER_POS_LIST,
	LAYER_RANK_COTHE,
	LAYER_MY_POS_LIST,
	LAYER_FIND_POS_LIST,
	LAYER_CREATE_ROOM_FRIEND
};

enum LAYER_POPUP_ID
{
	POPUP_SETTING = 200,
	POPUP_CHANGE_NAME,
	POPUP_ATTEND,
	POPUP_USER_MAIL,
	POPUP_INFO_HERO,
	POPUP_DIEM_DANH,
	POPUP_CHAT,
	POPUP_INVITE,
	POPUP_SELECT_INVITE,
	POPUP_CHAP_QUAN,
	POPUP_CONFIG_INGAME,
	POPUP_READY,
	POPUP_GUIDE,
	POPUP_MONEYCHARGE,
	POPUP_ALERT,
	POPUP_TOAST,
	POPUP_LOADING,
	POPUP_LIST_THECO,
	POPUP_THE_CUA_TUI,
	POPUP_OPEN_CHEST,
	POPUP_TOP_CHEST,
	POPUP_GIFFCODE,
	POPUP_RECHARGE_INGAME,
	POPUP_POS_ROOM,
	POPUP_AUTO_INVITE,
	POPUP_INVITE_FRIEND,
	POPUP_USER_INFO
};

enum LAYER_MAIN_GAME_ID
{
	LAYER_GAME_BOTTOM = 100,
	LAYER_GAME_MAIN
};

enum CHESS_TYPE{
	SIMPLE,
	STONE,
	WOODEN,
	COPPER,//dong
	IRON, //sat
	SILVER,
	GOLD,
	GLASS,
	FIRE
};
enum SIDE{
	BLACK = 1,
	RED
};

#define	DARK			0
#define	LIGHT			1
#define EMPTY_TYPE			3

enum NAME {
	// 	DARK = 1,
	// 	LIGHT = 2,
	PAWN = 0,
	BISHOP = 1,
	ELEPHANT = 2,
	KNIGHT = 3,
	CANNON = 4,
	ROOK = 5,
	KING = 6,
	EMPTY = 7,
	FOLD = 8
};
//enum login type
enum LOGIN_TYPE{
	NORMAL,
	REGISTER,
	FACEBOOK,
	GMAIL,
	QUICK
};
/************************************************************************/
/* for co the                                                                     */
/************************************************************************/

// tag
#define tagBtn 0
#define tagBtnBack		1
#define tagBtnFacebook	2
#define tagBtnSound		3
#define tagBtnHint		4
#define tagBtnSoundMute 6
#define tagBtnRefresh	5
#define tagBtnDongY		8
#define tagBtnTuChoi	9
#define tagBtnGo		10
#define tagGem			11
#define tagBtnShow		12
#define tagBtnContinous 13
#define tagTable		7
#define tagLayerLoading 14
#define tagBtnChat		15
#define tagBtnGuiLoiGiai 16
#define tagBtnXoaThe 17
#define tagBtnDSLoiGiai 18
#define tagBtnFbOut 19
#define tagBtnInfo 20

// action
#define ACTION_SEND_RESULT	1
#define ACTION_SEND_POS		2

// from & to
#define TAO_THE_CO		1
#define GIAI_THE_CO		2
#define DANH_SACH_THE	3
#define DANH_SACH_THE_CUA_TUI 4
#define FIRST_SCENE 5
#define DANH_SACH_GIAI_THE 6

#define WIN_GEM 500

enum TAGTAG {
	TABLE_XOA_THE_CO = 1,
	DONG_Y_XOA_THE_CO,
	HUY_BO_THAO_TAC
};

/************************************************************************/
/* web link                                                                     */
/************************************************************************/

#define LINK_FIRST_GUIDE "http://sv.trangco.vn/ingame/guide.html"
#define LINK_RANK_OPEN_CHEST "http://sv.trangco.vn/ingame/topmoruong.html"// top mo ruong
#define LINK_RANK_CHESS_POS "http://sv.trangco.vn/ingame/topgiaithe.html"// bang xep hang giai the
#define LINK_RANK_GAME "http://sv.trangco.vn/ingame/topgame.html" //bang xep hang co tuong, co up
#define LINK_NEWS_SHORT "http://sv.trangco.vn/ingame/event.html"//link tin tuc duoi dang ngan gon
#endif // Common_h__
