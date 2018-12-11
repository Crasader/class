
#ifndef __COMMON_H_
#define __COMMON_H_

#include "cocos2d.h"

#define TAG_MOBI 2
#define TAG_VIETTEL 3
#define TAG_VINA 1

#define TAG_LAYER_NAP_TIEN 52345

#define DESIGN_RESOLUTION_WIDTH 1920
#define DESIGN_RESOLUTION_HEIGHT 1079
#define BORDER_TOP_MENU 20
#define TOP_HEIGHT 177

#define CODE_REASON_NAPTHE 1
#define CODE_REASON_LOGIN_FIRST 0

#define WIDTH_HEADER_EVENT_POPUP 116
#define WIDTH_TAB_EVENT_POPUP 262

#define TOP_FRIEND 12
#define TOP_RANK 13
#define TOP_NOTIFICATION 14
#define TOP_MAIL 15
#define BOTTOM_CHAT 16
#define POPUP_THONGBAO 17
#define TOP_RECHARGE 18
//anhnt change home domain
//#define HOME_DOMAIN "https://nagaclub.net/"
#define HOME_DOMAIN "https://winclubgame.info/"
//#define HOME_DOMAIN "https://yakclubs.info"
//#define HOME_DOMAIN "https://winplay.info/"
//#define HOME_DOMAIN "http://megawingame.info"
//isNagaCard
//#define HOME_DOMAIN "http://app.nagacard.com/"
//#define HOME_DOMAIN "https://reahu-game.fun/"
/*link webview baomoc*/
#define TYGIA_P2_LINK "tygia.html?username="
#define CS_P2_VIP "csvip.html?username="
#define GOLD_HISTORY "gold-history.html?username="
#define GUIDE_P2_LINK "guideingame.html?gameId="
#define LS_DOI_THE_P2 "lichsudoivatpham.html?username="
#define LS_CHUYEN_TIEN_P2 "lichsuchuyentien.html?username="
#define LOIICH_P2_VIP "quyenloivip.html"
#define HISTORY_P2_LINK "lichsunapthe.html?username="
#define GUIDE_QUAY_THUONG_HN_P2 "guidlequaythuonghangngay.html?username="
#define DAYSOMAYMAN "daysomayman.html?username="
#define DS_DOITHUONG "dsdoithuong.html?username="
#define QUYDINH_DOITHUONG_P2 "qddt.html?o="
#define SKTX_P2 "sktx.html?username="
#define GOLD_HISTORY "gold-history.html?username="
#define RANK_PICKROOM "https://nagaclub.net/table.html?gameId="



#define NOTIFICATION_P2_SERVICES "https://nagaclub.net/notifyServices?"
#define GETLINK_AVATAR_P2 "https://nagaclub.net/GetAllAvatarLink"
#define HUONGDAN_UNGTIEN "https://gamebai.net/webview/lgui"
#define LICHSU_UNGTIEN "https://gamebai.net/webview/lhis"
#define THELE_NHIEMVU "https://gamebai.net/webview/mter"
#define NOTIFICATION_P2_LINK "https://nagaclub.net/categories"

/*Link c.choibai.info*/
#define TYGIA_LINK "https://gamebai.net/menh-gia/"
#define CS_VIP "https://gamebai.net/webview/vinfo?"
#define GIFTCODE_LINK "https://gamebai.net/giftcodeServies?username="
#define HISTORY_LINK "https://gamebai.net/webview/lsnt?username="
#define GUIDE_LINK "https://gamebai.net/huong-dan-game?gameid="
#define LS_DOI_THE "https://gamebai.net/webview/requestcard?username="
#define NOTIFICATION_LINK "https://gamebai.net/newscateServies"
#define LS_CHUYEN_TIEN "https://gamebai.net/webview/userexchange?username="
#define GT_RUONGVIP "https://gamebai.net/webview/chestinfo"
#define TOP_MORUONG "https://gamebai.net/webview/chesthistory"
#define LOIICH_VIP "https://gamebai.net/webview/benefit?"
#define LINK_BXH "https://gamebai.net/webview/bxh"
#define QUYDINH_DOITHUONG "https://gamebai.net/webview/qddt?o="
#define NOTIFICATION_SERVICES "https://gamebai.net/notifyServices?"
#define SKTX "https://gamebai.net/webview/sktx"
#define GETLINK_AVATAR "https://gamebai.net/avatar_new.php"
#define LINK_EVENT_TCC "https://gamebai.net/webview/tcc"
#define LINK_EVENT_MINIPOKER "https://gamebai.net/webview/skpk"
#define LINK_NHAP_GIFTCODE "https://gamebai.net/giftcodeinput?username="
#define LINK_DS_DAILY "https://gamebai.net/webview/dsdl"
/* Bitmap font of dautv, DO NOT modify */
#define UTM_CafetaTTF "UTM_Cafeta.ttf"
#define LucidaGrandeFnt "LucidaGrande.fnt"
#define ButLong2Fnt "tttt.fnt"
#define TimeNewRomanFnt "font3.fnt"
#define UVNVanFnt "UVNVan.fnt"
#define ChanDefaultFont "__Roboto-Regular.ttf"
#define AndrogenFnt "Androgen.fnt"
#define RobotoFnt "robotofont.fnt"
#define f_font_sfuStaff "sfuStaff-font.fnt"
#define exp_font_export "exp-font-export.fnt"
#define STRING_PATH "strings/"
#define IMAGE_PATH "images/"
#define PATH_POKER "design/ResPoker/"
#define GUESS_OPACITY 100

const cocos2d::Color3B _colorTextWHITE = cocos2d::Color3B::WHITE;
const cocos2d::Color3B _colorTextNormal = cocos2d::Color3B(255, 227, 160);
const cocos2d::Color3B _colorTextInButton = cocos2d::Color3B(123, 1, 0);
const cocos2d::Color3B _colorTextInPopUp = cocos2d::Color3B(68, 63, 47);

class NewCommon{
public:
	static std::string getTypeRoom(long money) {
		if (money < 5500) {
			return "Bình dân";
		}
		else if (money < 95000) {//dai Gia
			return "Triệu Phú";
		}
		else if (money < 500500) { //Ty phu
			return "Đại Gia";
		}
		return "";
	}
};

///enum zorder for add child to running scene
enum ZORDER_LIST
{
	ZORDER_POPUP_WEBVIEW = 100,
	ZORDER_POPUP_NEWBIE = 100,
	ZORDER_POPUP_RANK = 100,
	ZORDER_QUAYTHUONG_HN = 1000,
	ZORDER_POPUP_MAINTAIN = 2000,
	ZORDER_POPUP_MONEYCHARGE = 3000,
	ZORDER_POPUP_INGAME = 4000,
	ZORDER_TOAST_PICKROOM = 5000,
	ZORDER_POPUP_NOTIFICATION = 10000,
	ZORDER_POPUP_ADMIN = 20000,
	ZORDER_TOAST = 20001,
	ZORDER_TAIXIU_MINI = 20002,
	ZORDER_LAYER_LOADING = 20003,
    ZORDER_POPUP_CHONTIEN = 20003,
	ZORDER_POPUP_RECONNECT = 20004,
};

#endif
