#ifndef ServerMessageDef_h__
#define ServerMessageDef_h__

#define EXT_VAL_ACCOUNT_NAME "aN"
///MESS NGOAI GAME
 #define REQUEST_AUTO_JOIN_ROOM "rajr"
 #define AMOUNT_BET "ab"
 #define GAME_MIN_BET  "mb"
 #define EXT_FIELD_GID_PICKROOM "gid"
 #define AUTO_JOIN_TBL_RESP "ajtresp"
 #define EXT_FIELD_RESULT_CODE "rc"
// #define SIZE_ADD 80
// #define EXT_EVENT_LEAVE_GAME_REQ  "lgrq"
// #define EXT_EVENT_GAME_LEAVE_RES "lvgrs" /// th
// #define EXT_EVENT_GAME_LEAVE_NOTI "lvgntf"
// #define EXT_FIELD_LEAVE_STATUS  "lstt"//nếu lstt = 1: rời ván lstt = 0: hủy rời ván
// #define EXT_FIELD_RESULT  "rscode"
// #define EXT_FIELD_CHICKEN_SET "chks"
// //MESS lay danh sach muc cuoc va ban choi o sanh cho
// #define EXT_EVENT_REQ_GET_LIST_BET "rglb"
// #define EXT_EVENT_RES_GET_LIST_BET "rsglb"
// #define EXT_EVENT_REQ_GET_LIST_ROOM "rglr"
// #define EXT_EVENT_RES_GET_LIST_ROOM "rsglr"
// #define EXT_FIELD_LIST_BET "listb"
// #define EXT_FIELD_DEFAULT_ZONE  "dfz"
// #define EXT_FIELD_BET_FROM "betf"
// #define EXT_FIELD_BET_TO "bett"
// #define EXT_FIELD_LIST_ROOM "listr"
// #define EXT_FIELD_GAME_ID "gameid"
#define EXT_FIELD_ROOM_TYPE "roomType"
//ext field
#define EXT_FIELD_USER_VIP_TYPE "accVipType"
#define EXT_FIELD_USER_AVATAR "aal"
#define EXT_FIELD_USER_EXP "exp"
#define EXT_FIELD_USER_LEVEL "lvl"
#define EXT_FIELD_USER_AMF_GOLD "amf"
#define EXT_FIELD_USER_AM "am"
#define EXT_FIELD_BET_VALUE "betvl"
#define EXT_FIELD_CHANGE_BALANCE_TYPE "cbt"
#define EXT_FIELD_VIC_POS "vtps"
#define EXT_FIELD_TOITRANG_TYPE "ttt"
#define EXT_FIELD_IS_FIRST_ROUND "isfr"
#define EXT_FIELD_DURATION_TIME "drt"
#define EXT_FIELD_COUNT_CARD "cc"
#define EXT_FIELD_GAME_INFO_NOTIFY "ginf"
#define  EXT_FIELD_CONFIRM  "cf"// truong "cf" = 1 la bat, = 0 la tat
//variables user quan co va tuong
#define EXT_FIELD_HERO_ID "heroid"
#define EXT_FIELD_CHESS_ID "chessid"
#define EXT_FIELD_ELO_POINT "elo"

#define EXT_EVENT_SET_BATTLE_HERO_REQUEST "sbhreq"
#define EXT_EVENT_SET_BATTLE_HERO_RESPONSE "sbhres"
#define EXT_EVENT_SET_BATTLE_CHESS_REQUEST "sbchreq"
#define EXT_EVENT_SET_BATTLE_CHESS_RESPONSE "sbchres"
//ext field chinese
#define EXT_EVENT_CHESS_TABLE_NTF  "ctbl"
#define EXT_EVENT_MOVE_NTF  "mntf"
#define EXT_EVENT_FOR_PEACE_NTF  "fpntf"
#define EXT_EVENT_FOR_PEACE  "fp"
#define EXT_EVENT_IM_LOSE  "iml"
#define EXT_EVENT_MOVE  "move"
#define EXT_EVENT_REPLY_FOR_PEACE  "rfp"
#define EXT_EVENT_UNDO_MOVE_REQ  "umr"
#define EXT_EVENT_REPLY_UNDO_MOVE  "rum"
#define EXT_EVENT_UNDO_MOVE_NTF  "umntf"
#define EXT_EVENT_MOVE_ERROR  "merr"
#define EXT_EVENT_START_READY_TIMER_NOTIF  "rdtntf"
#define EXT_EVENT_USER_INFO  "accPIs"
#define EXT_EVENT_LIST_USER_UPDATE "luu"
#define EXT_EVENT_START "s"
#define EXT_EVENT_END "e"
#define EXT_EVENT_GAME_RESULT "grs"
#define EXT_HEATBEAT "hbc"
#define EXT_FIELD_USERID  "uid"
#define EXT_FIELD_USERNAME  "usrn"
#define EXT_FIELD_POSITION  "p"
#define EXT_FIELD_IS_TURN_UP_ALL  "tua"
#define EXT_FIELD_LIST_CARD  "lc"
#define EXT_FIELD_LIST_USER  "lu"
#define EXT_FIELD_ERROR_STRING  "eexp"
#define EXT_FIELD_RESULT_GAME  "rg"
#define EXT_FIELD_TXT_MSG  "txtmsg"
#define EXT_FIELD_CARD_INDEX  "cardidx"            // id của 1 cây bài trong bộ bài (bộ 52 cây)
#define EXT_FIELD_BET_VAL  "betval"
#define EXT_FIELD_CURRENT_TIME "crt"
#define EXT_EVENT_MOVE "move"
#define EXT_FIELD_MOVETO "mt"
#define EXT_FIELD_MOVEFROM "mf"
#define EXT_EVENT_NEXT_TURN "nt"
#define EXT_EVENT_NOT_ENOUGH_MONEY "nem_ntf"
#define EXT_EVENT_READY_NTF "rntf"
#define EXT_EVENT_AUTO_KICK "kkntf"
#define EXT_EVENT_USER_INFO "accPIs"
#define EXT_FIELD_FROM_CHESS_TYPE  "fct"
#define EXT_FIELD_TABLE_LIST_CHESS "tblc"
#define EXT_FIELD_TIME_FOR_TURN "tft"
#define EXT_FIELD_TIME_FOR_GAME "tfg"
#define EXT_EVENT_READY_REQ "rr"
#define EXT_EVENT_GAME_LEAVE_REQ "lvgrq"
#define EXT_EVENT_GAME_LEAVE_RES "lvgrs" /// th
#define EXT_EVENT_ERROR_READY_NTF "e_rntf"
#define EXT_EVENT_USER_READY_NOTIF "rdyntf"
#define EXT_EVENT_UNREADY_NTF "urntf"
#define EXT_EVENT_READY_RES "rdres"
#define EXT_EVENT_READY_NTF "rntf"
#define EXT_EVENT_WARNING_NOTIF "notiRMFF"
#define EXT_FIELD_MESSAGE_STRING "notiMSG"
//phan chap quan
#define EXT_EVENT_CHANGE_SETTING_REQUEST "csreq"
#define EXT_EVENT_CHANGE_SETTING_RESPONSE "csres"
#define EXT_EVENT_CHANGE_SETTING_NOTIF "csntf"

#define EXT_EVENT_PRE_CHANGE_SETTING_REQUEST  "pcsreq"
#define EXT_EVENT_PRE_CHANGE_SETTING_RESPONSE  "pcsres"
#define EXT_EVENT_PRE_CHANGE_SETTING_NOTIF  "pcsntf"

#define EXT_FIELD_TIME_WAIT  "tw"
#define EXT_FIELD_TYPE  "type"

//History request
#define EXT_EVENT_HISTORY_REQUEST "hisreq "
#define EXT_EVENT_HISTORY_RESPONSE "hisres"
#define EXT_FIELD_NUMPLAY "numplay"
#define EXT_FIELD_WINRATE "winrate"
//Co the
#define EXT_EVENT_GET_LIST_RANK_REQUEST "glrrq"
#define EXT_EVENT_GET_LIST_RANK_RESPONSE "glrres"
#define EXT_FIELD_LIST_RANK "list_rank"

#define EXT_EVENT_SEND_CHESS_POS_REQUEST "schprq"
#define EXT_EVENT_SEND_CHESS_POS_RESPONSE "schpres"
#define EXT_FIELD_POS_NAME "pos_name"
#define EXT_FIELD_POS_DATA "pos_data"

#define EXT_EVENT_GET_MY_CHESS_POS_REQUEST "gmchprq"
#define EXT_EVENT_GET_MY_CHESS_POS_RESPONSE "gmchpres"
#define EXT_FIELD_ID_FROM "startR"
#define EXT_FIELD_ID_TO "endR"
#define EXT_FIELD_LIST_POS "list_pos"

#define EXT_EVENT_GET_LIST_CHESS_POS_REQUEST "glchprq"
#define EXT_EVENT_GET_LIST_CHESS_POS_RESPONSE "glchpres"

#define EXT_EVENT_GET_INFO_CHESS_POS_REQUEST "gichprq"
#define EXT_EVENT_GET_INFO_CHESS_POS_RESPONSE "gichpres"
#define EXT_FIELD_POS_USER "pos_user"

#define EXT_EVENT_UPDATE_CHESS_POS_REQUEST "uchprq"
#define EXT_EVENT_UPDATE_CHESS_POS_RESPONSE "uchpres"
#define EXT_FIELD_ID_POS "id_pos"
#define EXT_FIELD_POINT "point"
#define EXT_FIELD_STAR_POINT "star_point"

#define EXT_EVENT_GET_COUNT_CHESS_POS_REQ "gcchprq"
#define EXT_EVENT_GET_COUNT_CHESS_POS_RES "gcchpres"
#define EXT_FIELD_COUNT "count"

#define EXT_EVENT_SET_RESULT_CHESS_POS_REQ "rchprq"
#define EXT_EVENT_SET_RESULT_CHESS_POS_RES "rchpres"
#define EXT_FIELD_STEP "step"// so lan di chuyen khi giai the
#define EXT_FIELD_DATA "data"//du lieu di chuyen giai the co
#define EXT_FIELD_DELAY_COUNT "delayCount"//so lan hoan nuoc

#define EXT_EVENT_DELETE_CHESS_POS_REQUEST "dchprq"
#define EXT_EVENT_DELETE_CHESS_POS_RESPONSE "dchpres"

#define EXT_EVENT_DELETE_LIST_CHESS_POS_REQ  "dlchprq"
#define EXT_EVENT_DELETE_LIST_CHESS_POS_RES  "dlchpres"

///SHop request
///vat pham trong game
#define EXT_EVENT_HERO_ITEM_LIST_REQUEST "hilreq"
#define EXT_EVENT_HERO_ITEM_LIST_RESPONSE "hilres"
#define EXT_FIELD_LIST_HERO "list_hero"

#define EXT_EVENT_ITEM_CHESS_LIST_REQUEST "chilreq"
#define EXT_EVENT_ITEM_CHESS_LIST_RESPONSE "chilres"
#define EXT_FIELD_LIST_CHESS "list_chess"

#define EXT_EVENT_PACKAGE_LIST_REQUEST "pilreq"
#define EXT_EVENT_PACKAGE_LIST_RESPONSE "pilres"
#define EXT_FIELD_LIST_PACKAGE "list_package"
//mua vat pham
#define EXT_EVENT_BUY_CHESS_ITEM_REQ "bchireq"
#define EXT_EVENT_BUY_CHESS_ITEM_RES "bchires"

#define EXT_EVENT_BUY_HERO_ITEM_REQ "bhireq"
#define EXT_EVENT_BUY_HERO_ITEM_RES  "bhires"
#define EXT_EVENT_BUY_ITEM_RESPONSE "bires"
#define EXT_FIELD_ITEM_ID "id"
#define EXT_FIELD_RESULT_DESCRIPTION "rd"

#define EXT_FIELD_ACC_CHESS_URL  "chessurl"
#define EXT_FIELD_ACC_HERO_URL  "herourl"
//PACKAGE
#define EXT_FIELD_PACKAGE_ID "packid"
#define EXT_EVENT_BUY_PACK_ITEM_REQ  "bpireq"
#define EXT_EVENT_BUY_PACK_ITEM_RES  "bpires"
//gifcode
#define EXT_EVENT_INPUT_GIFFCODE_REQUEST "igcreq"
#define EXT_EVENT_INPUT_GIFFCODE_RESPONSE "igcres"
#define EXT_FIELD_DAILY_ID "daily_id"
#define EXT_FIELD_DEV_ID "devid"
#define EXT_FIELD_GIFF_CODE "giftcode"
#define EXT_FIELD_MONEY "money"
#define EXT_FIELD_OS_ID "osid"
//reward extension
#define EXT_EVENT_VIEW_ADS_BONUS_REQ  "rvab"
#define EXT_EVENT_VIEW_ADS_BONUS_RES  "rsvab"
#define EXT_FIELD_BONUS "coinBonus"
//TIME Ad chartboost
#define EXT_EVENT_GET_TIMEOUT_ADS_REQ  "gtar"
#define EXT_EVENT_GET_TIMEOUT_ADS_RES  "gtares"
#define EXT_FIELD_TIME "time"
///update 25/5

#define EXT_EVENT_LIKE_POS_CHESS_REQUEST "lpchreq"
#define EXT_EVENT_LIKE_POS_CHESS_RESPONSE "lpchres"
#define EXT_EVENT_GET_NEXT_POS_CHESS_REQUEST "gnpchreq"
#define EXT_EVENT_GET_NEXT_POS_CHESS_RESPONSE "gnpchres"
#define EXT_EVENT_FIND_POS_CHESS_REQUEST "fpchreq"
#define EXT_EVENT_FIND_POS_CHESS_RESPONSE "fpchres"

#define EXT_FIELD_POS_TYPE "pos_type"
#define EXT_FIELD_USER_ID "user_id"
#define EXT_FIELD_POS_LIKE "pos_like"
#define EXT_FIELD_POS_KEY "pos_key"
//ban be
#define LIST_FRIEND_CMD "alic"
#define LIST_FRIEND_RESPONSE_CMD "cmi"
#define ADD_FRIEND_CMD  "aafc"
#define REMOVE_FRIEND_CMD  "arfc"
#define EXT_EVENT_INVITE_FRIEND_BATTLE_REQUEST  "invfbreq"
#define EXT_EVENT_INVITE_FRIEND_BATTLE_REPONSE  "invfbres"
#define INVITE_FRIEND_BATTLE_NOTIFY "invfbntf"
#define CONFIRM_FRIEND_BATTLE_REQUEST "cffbreq"
#define EXT_FIELD_PARAMS "params"
#define EXT_FIELD_CONFIRM_BATTLE "confirm"
#define EXT_FIELD_ACCOUNT_ID "aI"
#define EXT_FIELD_RES_CODE "rescode"
#define EXT_FIELD_LIST_INFO "list_info"
#endif // ServerMessageDef_h__
