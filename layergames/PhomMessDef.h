#ifndef PhomMessDef_h__
#define PhomMessDef_h__
#include <string>
#include "cocos2d.h"
#include "mUtils.h"
#include "LayerGa.h"
#include "data_manager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "SFSUserVariable.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "Entities/Variables/SFSUserVariable.h"
#endif

#include "Requests/SetUserVariablesRequest.h"
using namespace std;
USING_NS_CC;
#define EXT_HEATBEAT "hbc"
/************************************************************************/
/* Phom client mess def                                                                     */
/************************************************************************/
#define EXT_SRVNTF_PUBLIC_MESSAGE "ntfpubmsg"
#define EXT_SRVNTF_PRIVATE_MESSAGE "ntfprvmsg"
#define EXT_SRVNTF_GAME_MASTER_INFO "ntfgminfo"
#define EXT_SRVNTF_READY "rsr"
#define EXT_SRVNTF_PLAYER_LIST "ntfpllst"
#define EXT_SRVNTF_RECOMMENDED_CARDSETS "ntfrcrds"
#define EXT_EVENT_START "s"
#define EXT_EVENT_END "e"
#define EXT_EVENT_GAME_RESULT "grs"
#define EXT_EVENT_LISTCARD_NTF "lcntf"
#define EXT_EVENT_COIN_NTF "ntfcoinch"
#define EXT_SRVNTF_CURRENT_PLAYING_USER "ntfcurpu"
#define EXT_SRVNTF_END_PLAYING_GAME "ntfepla"
#define EXT_SRVNTF_GAME_INFO "ntfginfo"
#define EXT_EVENT_READY_RES "rdres"
#define EXT_EVENT_READY_REQ "rr"
#define EXT_EVENT_UNREADY_RES "rsur"
#define EXT_EVENT_UNREADY_REQ "urr"
#define EXT_SRVNTF_USER_READY "ntfurd"
#define EXT_SRVNTF_CARD_ORIGINATION "ntfcorg"
#define EXT_EVENT_RES_DRAW_CARD "rsdrwc"
#define EXT_EVENT_RES_TAKE_CARD "rstkc"
#define EXT_EVENT_RES_DISCARD "rshofc"
#define EXT_EVENT_RES_HANDOFF_PHOM "rshofp"
#define EXT_EVENT_RES_END_PLAYING "rsenpl"
#define EXT_EVENT_RES_SET_BET "rssbet"
#define EXT_EVENT_RES_U "rsU"
#define EXT_SRVNTF_CAN_TAKE "ntfcantk"
#define EXT_SRVNTF_PLAYER_REPLAY "ntfrpl"
#define EXT_SRVNTF_U_9 "ntfu9"
#define EXT_SRVNTF_U  "ntfu" // cac loai u
#define EXT_SRVNTF_COIN_CHANGE ""
#define EXT_EVENT_RES_ORDER_CARDS "rsodrc"
#define EXT_EVENT_RES_PUSH_CARD "rspusc"
#define EXT_EVENT_CHICKEN_UPDATE_NTF "chkntf"
#define EXT_EVENT_REQ_JOIN_GAME "rqjg"
#define EXT_EVENT_RES_JOIN_GAME "rsjg"
#define EXT_EVENT_REQ_LEAVE_GAME "rqlg"
#define EXT_EVENT_RES_LEAVE_GAME "rslg"
#define EXT_SRVNTF_PLAYER_KICKOUT "ntfko"
#define EXT_EVENT_REQ_ORDER_CARDS "rodrc"
#define EXT_EVENT_REQ_DRAW_CARD "rqdrwc"
#define EXT_EVENT_REQ_DISCARD "rqhofc"
#define EXT_EVENT_REQ_TAKE_CARD "rqtkc"
#define EXT_EVENT_REQ_HANDOFF_PHOM "rqhofp"
#define EXT_EVENT_REQ_PUSH_CARD "rqpusc"
#define EXT_EVENT_REQ_U "rqU"
#define EXT_EVENT_UNREADY_NTF "rdtntf"
#define EXT_EVENT_NTF_TIMER "tupd"                    // timer update
#define EXT_EVENT_REQ_UPDATE_GAME_CONFIG  "rqgcfg" //bat tat ga
#define EXT_EVENT_RES_UPDATE_GAME_CONFIG "rsgcfg"     // reponse update game cfg
#define EXT_SRVNTF_NOC_COUNT "ntfnoccnt"
#define EXT_EVENT_REGISTER_QUEUE_NTF "rqntf" //"rqntf";//ban tin chau ria 
#define EXT_SRVNTF_RECOMMENDED_PUSH_CARDS "ntfpucs" // goi y gui phom ntfpucs

#define CARD_ORIGINATION_BY_TAKEN  1 // ăn được
#define CARD_ORIGINATION_BY_DRAWN  2 // bốc từ nọc
#define CARD_ORIGINATION_BY_TRANSFERED  3 // quân bài chuyển
#define CARD_ORIGINATION_BY_DEALT       4 // chia bài
#define CARD_ORIGINATION_BY_HANDOFF 5 // chia bài
#define CARD_ORIGINATION_BY_PUSHED  6 // chia bài
#define CARD_ORIGINATION_BY_UNKOWN  7 // chia bài

//RESCODE
#define PHOM_RSCODE_OK  0                                 // OK, success
#define PHOM_RSCODE_FAILED  PHOM_RSCODE_OK - 1              // FAILED
#define PHOM_RSCODE_ERROR_ROOM_FULL  PHOM_RSCODE_OK + 1   // ROOM FULL
#define PHOM_RSCODE_ERROR_GAME_FULL  PHOM_RSCODE_OK + 2   // GAME FULL
#define PHOM_RSCODE_ERROR_CARD_NOT_IN_ON_HAND_LIST  PHOM_RSCODE_OK + 3    // card không nằm trong trên tay list
#define PHOM_RSCODE_ERROR_HANDOFF_CARD_NOT_ALLOW  PHOM_RSCODE_OK + 4      // không được phép discard 
#define PHOM_RSCODE_ERROR_PLAYER_IN_END_STATE  PHOM_RSCODE_OK + 5         // player đã ở trạng thái kết thúc 
#define PHOM_RSCODE_ERROR_NOT_A_CARDSET  PHOM_RSCODE_OK + 6               // danh sách quân bài không hợp thành 1 phỏm
#define PHOM_RSCODE_ERROR_NO_MELD_YET  PHOM_RSCODE_OK + 7                 // player chưa có phỏm nào (chưa hạ, hoặc bị móm)
#define PHOM_RSCODE_ERROR_OWN_CARDS_NUM_LESS_THAN_10  PHOM_RSCODE_OK + 8  // số lượng bài trên tay (bao gồm cả phỏm đã hạ, bài đã gửi) nhơ hơn 10 cây
#define PHOM_RSCODE_U_KHAN  PHOM_RSCODE_OK + 9                            // Ù khan
#define PHOM_RSCODE_U_9  PHOM_RSCODE_OK + 10          // Ù 9 cay  
#define PHOM_RSCODE_U_10  PHOM_RSCODE_OK + 11         // Ù 10 cây - Ù trên tay
#define PHOM_RSCODE_U_0  PHOM_RSCODE_OK + 12         // Ù 0 điểm
#define PHOM_RSCODE_NOT_U  PHOM_RSCODE_OK + 13        // không Ù
#define PHOM_RSCODE_INVALIDE_STATE  PHOM_RSCODE_OK + 14                   // action của player không được phép tại thời điểm đó
#define PHOM_RSCODE_CARD_ISNOT_LAST_DISCARD  PHOM_RSCODE_OK + 15          // Card không phải là quân bài cuối cùng đc đánh ra trên bàn chơi
#define PHOM_RSCODE_INVALID_CARDSET  PHOM_RSCODE_OK + 16;                  // danh sách quân bài không phải là phỏm
#define PHOM_RSCODE_INVALID_TURN  PHOM_RSCODE_OK + 17                     // player đánh bài không phải lượt (ko cho phép)
#define PHOM_RSCODE_ONHAND_CARD_SIZE_NOT_10  PHOM_RSCODE_OK + 18          // số lượng bài trên tay ko phải 10
#define PHOM_RSCODE_ERROR_OWN_CARDS_NUM_EQUAL_10  PHOM_RSCODE_OK + 19     // số lượng bài trên tay = 10
#define PHOM_RSCODE_ERROR_NO_MORE_CARD  PHOM_RSCODE_OK + 20               // không còn bài trên nọc
#define PHOM_RSCODE_ERROR_PLAYER_NOT_IN_END_STATE  PHOM_RSCODE_OK + 21    // player ko ở trạng thái kết thúc (đã oánh 4 cây đi)
#define PHOM_RSCODE_ERROR_PLAYER_ENDED_STATE_BEFORE  PHOM_RSCODE_OK + 22  // player đã kết thúc chơi trước đây
#define PHOM_RSCODE_ERROR_PLAYER_ISNOT_ROOM_MASTER  PHOM_RSCODE_OK + 23   // player không phải chủ bàn
#define PHOM_RSCODE_BETVAL_TOO_BIG  PHOM_RSCODE_OK + 24                   // giá trị bet quá lớn
#define PHOM_RSCODE_HAVE_JOINED_BEFRORE  PHOM_RSCODE_OK + 25              // đã join (room or game) trước đây
#define PHOM_RSCODE_HAVENT_JOINED  PHOM_RSCODE_OK + 26                    // chưa join (room or game) trước đây
#define PHOM_RSCODE_UNDER_MINIMUM_BALANCE PHOM_RSCODE_OK + 28
#define PHOM_RSCODE_VALID_MULTI_PUSHCARD PHOM_RSCODE_OK + 29
//RESULT GAME
#define RANK_U_0_DIEM 0
#define RANK_U_TRON RANK_U_0_DIEM + 1
#define RANK_U_9_CAY RANK_U_0_DIEM + 2
#define RANK_U_KHAN RANK_U_0_DIEM + 3
#define RANK_NHAT RANK_U_0_DIEM + 4
#define RANK_NHI RANK_U_0_DIEM + 5
#define RANK_BA RANK_U_0_DIEM + 6
#define RANK_BET RANK_U_0_DIEM + 7
#define RANK_CHAY RANK_U_0_DIEM + 8
#define RANK_U_DEN RANK_U_0_DIEM + 9
#define RANK_BO_CUOC RANK_U_0_DIEM + 10
#define RANK_U_LAO RANK_U_0_DIEM + 11
#define RANK_U_KHAN_10_CAY RANK_U_0_DIEM + 12
#define RANK_U_DEN_10_CAY RANK_U_0_DIEM + 13

//EXT FIELD
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
#define EXT_FIELD_U_RANK  "urnk"                   // đánh dấu kiểu Ù (Ù 0 điểm, Ù 9 cây, Ù khan, Ù trên tay - Ù 10)
#define EXT_FIELD_GAME_TURN  "gturn"              // trường giá trị vòng chơi hiện tại của game
#define EXT_FIELD_FROM_USERID  "fuid"              // userID nguồn gốc của cây bài
#define EXT_FIELD_TO_USERID  "tuid"              // userID nhận cây bài
#define EXT_FIELD_FROM_USERNAME "funa"			//ten user nguoc goc cay bai
#define EXT_FIELD_TO_USERNAME	"tuna"			//ten user nhan cay bai
#define EXT_FIELD_CARD_ORIGINATION  "crdorg"      // trường đánh dấu xuất xứ của 1 cây bài
#define EXT_FIELD_RESULT_CODE  "rscode"            // trường rs code trả về các bản tin repsonse
#define EXT_FIELD_CAN_TAKE  "cantk"                // chỉ ra quân bài có ăn đc hay ko
#define EXT_FIELD_COIN_CHANGE  "coinch"                // lượng tiền bị trừ hoặc tăng.
#define EXT_FIELD_COIN_CHANGE_REASON "coinchr"			//ly do luong tien thay doi
#define EXT_FIELD_PHOM_ID "phomid"					//id cua phom gui quan toi
#define EXT_FIELD_RECOMMEND_CARDSET "crdss"			//listcard goi y ha phom
#define EXT_FIELD_CHICKEN_VALUE "chkvl"
#define EXT_FIELD_CHICKEN_LEVEL "chklv"
#define EXT_FIELD_LIST_USER_CHICKEN "lug"//danh sach nuoi ga
#define EXT_FIELD_DESTINATION_PHOMS "dstphs"  // ds các phỏm có thể gửi bài
#define EXT_FIELD_TIMER_VAL "tval"
#define EXT_FIELD_KICKOUT_REASON "koutr"
#define EXT_FIELD_NOC_COUNT "noccount"
#define EXT_FIELD_PUSCARDS_DESTINATION "pcrds"
/************************************************************************/
/*Sam + Tien len Mess def                                                                      */
/************************************************************************/
#define EXT_EVENT_USER_JOIN_NOTIF  "jrntf"
#define EXT_EVENT_LISTCARD_NTF "lcntf"
#define EXT_EVENT_ERROR_READY_NTF "e_rntf"
#define EXT_EVENT_USER_READY_NOTIF "rdyntf"
#define EXT_EVENT_UNREADY_NTF "urntf"
#define EXT_EVENT_READY_RES "rdres"
#define EXT_EVENT_LIST_USER_UPDATE "luu"
#define EXT_EVENT_START_GAME_NOTIF "strntf"
#define EXT_EVENT_DEAL_CARD_NOTIF "dlcntf"
#define EXT_EVENT_USER_LEAVE_NOTIF "lrntf"
#define EXT_EVENT_END_GAME_NOTIF "endntf"
#define EXT_EVENT_CHANGE_BALANCE_NOTIFY "cblltf"
#define EXT_EVENT_VICTORY_NOTIF "vicntf"
#define EXT_EVENT_NEAD_PLAY_NOTIF "ndpntf"
#define EXT_EVENT_PLAY_CARD_NOTIF "plcntf"
#define EXT_EVENT_GAME_CHANGE_NOTIF "gchntf"
#define EXT_EVENT_PASS_CARD_NOTIF "pscntf"
#define EXT_EVENT_PLAY_CARD_RES "plcrs"
/*ban tin danh bien 3 cay chuong*/
#define EXT_EVENT_SET_ACCEPT_ENROLL_ALL_REQUEST "saeareq" //request danh bien
#define EXT_EVENT_ENROLL_BET_RESPONSE "ebres" //response danh bien
#define EXT_EVENT_ENROLL_ALL_INFO_NOTIF "eaintf" //danh sach cac nguoi dang danh bien
#define EXT_EVENT_ENROLL_BET_INFO_NTF "ebintf" //danh sach cac nguoi dang dat bien
#define EXT_EVENT_DEAL_CARD_BOSS_NOTIF "dcbntf" // ntf list card chia bai tim chu phong
#define EXT_EVENT_NEW_BOSS_NOTIF "nbntf" // ntf nguoi cam chuong moi
#define EXT_EVENT_SELL_BOSS_REQUEST "sbreq"//request ban chuong
#define EXT_EVENT_BUY_BOSS_NOTIF "bbntf" //ntf mua chuong
#define EXT_EVENT_BUY_BOSS_REQUEST "bbreq" //request mua chuong
#define EXT_EVENT_BUY_BOSS_REPONSE "bbres"//response mua chuong
#define EXT_EVENT_BREAK_GAME_NTF "bgntf"//ntf vo xoi
/*ban tin cam cai xoc dia*/
#define EXT_EVENT_REGISTER_BOSS_REG "regbrq" // dk cam cai
#define EXT_EVENT_REGISTER_BOSS_RES "regbrs"
#define EXT_EVENT_UNREGISTER_BOSS_REG "unregbrq" //huy dk cam cai
#define EXT_EVENT_UNREGISTER_BOSS_RES "unregbrs"
#define EXT_EVENT_GET_LIST_BOSS_REG "glbrq" //danh sach cai
#define EXT_EVENT_GET_LIST_BOSS_RES "glbrs"
#define EXT_EVENT_NEXT_BOSS_NTF "nbntf" //thong tin cai tiep theo
#define EXT_EVENT_DEL_BOSS_NTF "dbntf" //xoa cai khi khong du dk
#define EXT_EVENT_OFFER_BOSS_NTF "bofntf" //neu du dk cam cai thi server tra ve ntf
#define EXT_EVENT_LIST_WIN_NTF "lwntf" //danh sach nguoi thang
//#define EXT_EVENT_UNREADY_REQ "urr"
//#define EXT_EVENT_READY_REQ "rr"
#define EXT_EVENT_PLAY_CARD_REQ "plcrq"
#define EXT_EVENT_PASS_CARD_REQ "pscrq"
#define EXT_EVENT_HIT_CARD_ERROR "pscrs"
#define EXT_EVENT_START_READY_TIMER_NOTIF "rdtntf"
#define EXT_EVENT_SAM_REQ "smrq"
#define EXT_EVENT_SAM_RES "smrs"
#define EXT_EVENT_SAM_NTF "smntf"
#define EXT_EVENT_BAO_NTF "baontf"
#define EXT_EVENT_CHICKEN_UPDATE_NTF "chkntf"
#define EXT_EVENT_REGISTER_QUEUE_NTF "rqntf"
#define EXT_EVENT_UNSAM_REQ "usmrq"
#define EXT_EVENT_UNSAM_NTF "usmntf" //	EXT_EVENT_UNSAM_REQ,// = "usmrq";EXT_EVENT_UNSAM_RES,// = "usmrs";
#define EXT_EVENT_UNSAM_RES "usmrs"
#define EXT_EVENT_GAME_CHIKKEN_CONFIG_REQ "ckcrq"// - require config ga 
#define EXT_EVENT_GAME_CHIKKEN_CONFIG_RES "ckcrs" //- result config ga
#define EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY "ckcntf" //- name extions config ga: 1 la bat, 0 la tat, chi tat dc khi tien ga dang = 0
#define EXT_EVENT_GAME_LEAVE_REQ "lvgrq"
#define EXT_EVENT_GAME_LEAVE_RES "lvgrs" /// th
#define EXT_EVENT_GAME_LEAVE_NOTI "lvgntf"
#define EXT_EVENT_NOT_ENOUGH_MONEY_NOTIF "expmntf"
#define EXT_EVENT_AUTO_KICK "kkntf"
#define EXT_EVENT_PLAY_ROOM_REGISTER "prr"
#define EXT_EVENT_USE_CHICKEN_REQ  "ucr"// ban tin config bat tat ga cho tung user
//ext field
#define EXT_FIELD_USER_AMF_GOLD "amf"
#define EXT_FIELD_USER_AMF_SILVER "amfs"
#define EXT_FIELD_BET_VALUE "betvl"
#define EXT_FIELD_CHANGE_BALANCE_TYPE "cbt"
#define EXT_FIELD_VIC_POS "vtps"
#define EXT_FIELD_TOITRANG_TYPE "ttt"
#define EXT_FIELD_IS_FIRST_ROUND "isfr"
#define EXT_FIELD_DURATION_TIME "drt"
#define EXT_FIELD_COUNT_CARD "cc"
#define EXT_FIELD_GAME_INFO_NOTIFY "ginf"
#define  EXT_FIELD_CONFIRM  "cf"// truong "cf" = 1 la bat, = 0 la tat
//RS CODE
#define SAM_RSCODE_OK  0 
#define SAM_RESULT_RSCODE_THOI_2 SAM_RSCODE_OK + 1
#define SAM_RESULT_RSCODE_AN_THOI_2 SAM_RSCODE_OK + 2
#define SAM_RESULT_RSCODE_CHAN_DUOC_2 SAM_RSCODE_OK + 3
#define SAM_RESULT_RSCODE_BI_BAT_2 SAM_RSCODE_OK + 4
#define SAM_RESULT_RSCODE_CHAN_LAI_2 SAM_RSCODE_OK + 5
#define SAM_RESULT_RSCODE_DEN_LANG SAM_RSCODE_OK + 6
#define SAM_RESULT_RSCODE_DEN_BAO SAM_RSCODE_OK + 7
#define SAM_RESULT_RSCODE_GOP_GA SAM_RSCODE_OK + 8
#define SAM_RESULT_RSCODE_TOI_TRANG SAM_RSCODE_OK + 10
#define SAM_RESULT_RSCODE_THUA_CONG SAM_RSCODE_OK + 11
#define SAM_RESULT_RSCODE_DUT_MU SAM_RSCODE_OK + 12
#define SAM_RESULT_RSCODE_THANG_CONG SAM_RSCODE_OK + 13
#define SAM_RESULT_RSCODE_BO_CUOC SAM_RSCODE_OK + 14
#define SAM_RESULT_RSCODE_THOI_TU_QUY SAM_RSCODE_OK + 15
#define SAM_RESULT_RSCODE_THOI_3_THONG SAM_RSCODE_OK + 16
#define SAM_RESULT_RSCODE_THOI_4_THONG SAM_RSCODE_OK + 17
#define SAM_RESULT_RSCODE_AN_THOI_TU_QUY SAM_RSCODE_OK + 18
#define SAM_RESULT_RSCODE_AN_THOI_3_THONG SAM_RSCODE_OK + 19
#define SAM_RESULT_RSCODE_AN_THOI_4_THONG SAM_RSCODE_OK + 20

///MESS NGOAI GAME
#define REQUEST_AUTO_JOIN_ROOM "rajr"
#define AMOUNT_BET "ab"
#define GAME_ID "gid"
#define AUTO_JOIN_TBL_RESP "ajtresp"
#define RESULT_CODE "rc"
#define SIZE_ADD 80
#define EXT_EVENT_LEAVE_GAME_REQ  "lgrq"
#define EXT_EVENT_LEAVE_GAME_RES  "lgrs"
#define EXT_FIELD_LEAVE_STATUS  "lstt"//nếu lstt = 1: rời ván lstt = 0: hủy rời ván
#define EXT_FIELD_RESULT  "rscode"
#define EXT_FIELD_CHICKEN_SET "chks"
//MESS lay danh sach muc cuoc va ban choi o sanh cho
#define EXT_EVENT_REQ_GET_LIST_BET "rglb"
#define EXT_EVENT_RES_GET_LIST_BET "rsglb"
#define EXT_EVENT_REQ_GET_LIST_ROOM "rglr"
#define EXT_EVENT_RES_GET_LIST_ROOM "rsglr"
#define EXT_FIELD_LIST_BET "listb"
#define EXT_FIELD_DEFAULT_ZONE  "dfz"
#define EXT_FIELD_BET_FROM "betf"
#define EXT_FIELD_BET_TO "bett"
#define EXT_FIELD_LIST_ROOM "listr"
#define EXT_FIELD_GAME_ID "gameid"
//XT MANG TIEN VAO BAN
#define EXT_EVENT_EXPIRE_CHIP_NTF "expcntf"
//ROOM CHAT
#define EXT_EVENT_CHAT_HISTORY_NTF "chhntf"
#define EXT_FIELD_CHAT_HISTORY "chhis"
#define EXT_EVENT_CHAT_REQ "chreq"
#define EXT_FIELD_CHAT_MESSAGE  "chmsg"
#define EXT_EVENT_CHAT_NTF  "chntf"
#define EXT_EVENT_GET_CHAT_HIS_REQ "rgchh"
#define EXT_EVENT_GET_CHAT_HIS_RES "rsgchh"
//TX Mini
#define EXT_EVENT_START_TXMINI "txs"
#define EXT_EVENT_END_TXMINI "txe"
#define EXT_EVENT_GAME_RESULT_TXMINI "txgrs"
#define EXT_EVENT_GAME_BET_ERROR_NOTIFY_TXMINI "txe_bntf"
#define EXT_EVENT_GAME_INFO_NTF_TXMINI "txgi_ntf"
#define EXT_EVENT_GAME_HIS_NTF_TXMINI "txgh_ntf"
#define EXT_EVENT_NOT_ENOUGH_MONEY_NTF_TXMINI "txnem_ntf"
#define EXT_EVENT_GAME_STATE_NTF_TXMINI "txgs_ntf"
#define EXT_EVENT_GAME_BET_NTF_TXMINI "txgb_ntf"
#define EXT_EVENT_GAME_BET_REQ_MINI_TX "txgbr"

#define EXT_FIELD_GAME_RETURN_VALUE "grv"
#define EXT_EVENT_TX_REFUND_NTF "txrfntf"

#define EXT_EVENT_GET_GAME_INFO_REQ_TX_MINI "txtgi_ntf"
#define EXT_FIELD_GAME_TOTAL_USER_BET_CELL "gtbuc"
#define EXT_FIELD_GAME_WIN_VALUE "gwv"
#define EXT_FIELD_GAME_RETURN_VALUE "grv"
#define EXT_EVENT_TX_GET_QUY_THUONG_REQ "txgqtr"
#define EXT_EVENT_TX_GET_QUY_THUONG_RESP "txgqtresp"
//field TX
#define EXT_FIELD_GAME_TOTAL_USER_BET_CELL "gtbuc";
#define EXT_FIELD_GAME_BET_TYPE "gbt"
//His TX Mini
#define EXT_EVENT_TX_USER_HIS_REQ "txuhrq"
#define EXT_EVENT_TX_USER_HIS_RESP "txuhresp"
#define EXT_EVENT_TX_GAME_HIS_DETAIL_REQ "txghdrq"
#define EXT_EVENT_TX_GAME_HIS_DETAIL_RESP "txghdresp"
#define EXT_EVENT_TX_GAME_HIS_REQ "txghrq"
#define EXT_EVENT_TX_GAME_HIS_RESP "txghresp"
#define EXT_EVENT_TX_GAME_HIS_DETAIL_REQ "txghdrq"
#define EXT_EVENT_TX_GAME_HIS_DETAIL_RESP "txghdresp"
//rank tx mini
#define EXT_EVENT_TX_RANK_REQ "txrrq"
#define EXT_EVENT_TX_RANK_RESP "txrresp"
//PK Mini
#define EXT_EVENT_GAME_START_PKMINI "pks"
#define EXT_EVENT_GAME_END_PKMINI "pke"
#define EXT_EVENT_GAME_BET_REQ_PKMINI "pkgbr"
#define EXT_EVENT_GAME_RESULT_PKMINI "pkgrs"
#define POKER_THUNG_PHA_SANH_LON 1
#define POKER_THUNG_PHA_SANH_BE 2
#define POKER_TU_QUY 3
#define POKER_CU_LU 4
#define POKER_THUNG 5
#define POKER_SANH 6
#define POKER_SAM 7
#define POKER_HAI_DOI 8
#define POKER_MOT_DOI_J 9
#define POKER_DOI_NHO 10
#define POKER_MOT_SO_7 11
#define POKER_BA_SO_7 12
#define EXT_EVENT_PK_GET_QUY_THUONG_REQ "pkgqtr"
#define EXT_EVENT_PK_GET_QUY_THUONG_RESP "pkgqtresp"
#define EXT_EVENT_GAME_STATE_NTF_MINIPOKER "pkgs_ntf"
#define EXT_EVENT_PK_RANK_REQ "pkrrq"
#define EXT_EVENT_PK_RANK_RESP "pkrresp"
#define EXT_EVENT_PK_USER_HIS_REQ "pkuhrq"
#define EXT_EVENT_PK_USER_HIS_RESP "pkuhresp"
#define EXT_EVENT_PK_AUTO_PLAY_REQ "pkapreq"
#define EXT_FIELD_GAME_IS_AUTO "giau"
#define EXT_EVENT_PK_AUTO_PLAY_RESP "pkapresp"
#define EXT_FIELD_GAME_AUTO_AMOUNT "amt"
#define EXT_FIELD_ERROR_CODE "errc"
#define EXT_EVENT_PK_CONFIG_QUY_THUONG_NTF "pkcqtntf"
//Quay thuong hang ngay
//get trang thai quay thuong
#define EXT_EVENT_GET_STATUS_SPIN "getnumspin"
#define EXT_EVENT_RES_NUM_SPIN "rnumspin"
#define EXT_FIELD_NUM_SPIN "numspin"
#define EXT_FIELD_LOGIN_FIRST "lf"
//get ket qua quay thuong
#define EXT_EVENT_DO_SPIN "reqspin"
#define EXT_EVENT_RES_DO_SPIN "resspin"
#define EXT_FIELD_REWARD_ID "rewardid"
//danh sach diem danh
#define EXT_EVENT_LIST_ATTEND_REQ  "rla"
#define EXT_EVENT_LIST_ATTEND_RES  "rsla"
#define EXT_FILED_NUM_ATTEND "numattend"
#define EXT_FIELD_LIST_ATTEND "listattend"
#define EXT_EVENT_ATTEND_REQ "ra"
#define EXT_EVENT_ATTEND_RES "rsa"
//game cao thap
#define EXT_EVENT_CAOTHAP_START "cts"
#define EXT_EVENT_CAOTHAP_END "cte"
#define EXT_EVENT_CAOTHAP_JOIN_GAME "ctj"
#define EXT_EVENT_CAOTHAP_LEAVE_GAME "ctl"
#define EXT_NOTIFY_CAOTHAP_GAME_STATE "ctgsn"
#define EXT_NOTIFY_CAOTHAP_START_GAME "ctsgn"
#define EXT_NOTIFY_CAOTHAP_END_GAME "ctegn"
#define EXT_EVENT_CAOTHAP_BET_CARD "ctgbr"
#define EXT_EVENT_CAOTHAP_GET_QUY "ctgqtr"
#define EXT_EVENT_CAOTHAP_GET_QUY_RESP "ctgqtresp"
#define EXT_EVENT_CAOTHAP_GET_TOP_WIN "ctrrq"
#define EXT_EVENT_CAOTHAP_GET_TOP_WIN_RESP "ctrresp"
#define EXT_EVENT_CAOTHAP_GET_HIS_PLAYER "ctgthp"
#define EXT_EVENT_CAOTHAP_GET_HIS_PLAYER_RESP "ctgthpr"
#define EXT_EVENT_CAOTHAP_GAME_RESULT "ctgrs"

//Mau Binh
#define EXT_EVENT_LIST_CARD_LINK "lcl"
#define EXT_EVENT_FINISH_CARD_LINK_REQ "efclrq"
#define EXT_EVENT_FINISH_CARD_LINK_NOTIFY "efcln"

//game mini new gb2.0
#define EXT_FIELD_GAME_TYPE_MONEY "tm"

//thanh tuu
#define EXT_EVENT_GET_ARCHIEMENT_REQ "garchiereq"
#define EXT_EVENT_GET_ARCHIEMENT_RESP "garchieresp"
#define ARCHIEMENT_RESULT "archieresult"

//nhiem vu tan thu
#define EXT_EVENT_GET_MISSION_REQ "gmissionreq"
#define EXT_EVENT_GET_MISSION_RESP "gmissionresp"
#define MISSION_RESULT_LIST "msl"
#define AWARD_SILVER "awsilver"
#define AWARD_GOLD "awardgold"

//tan thu
#define EXT_EVENT_ARCHIEMENT_REQ "archiereq"
#define ARCHIEMENT_ID "archieid"
#define EXT_EVENT_ARCHIEMENT_RESP "archieresp"

//doi tien
#define EXT_EVENT_EXCHANGE_GOLD2SILVER_REQ "eg2sreq"
#define EXCHANGE_GOLD_VALUE "exgv"
#define EXT_EVENT_EXCHANGE_GOLD2SILVER_RESP "eg2sresp"
#define REAL_RECEIVED_SILVER "rrs"
//game mini new gb2.0
#define EXT_FIELD_GAME_TYPE_MONEY "tm"
//chat tx mini
#define EXT_EVENT_TX_CHAT_REQ "txchtrq"
#define EXT_EVENT_TX_CHAT_NTF "txchtntf"
#define EXT_EVENT_GET_HIS_TX_CHAT_REQ "txehchrq"
#define EXT_EVENT_GET_HIS_TX_CHAT_RESP "txehchrs"
#define EXT_EVENT_HISTORY_ANALYTICS_TX_REQ "eehatrq"
#define EXT_EVENT_HISTORY_ANALYTICS_TX_RESP "eehatrs"
//gio vang
#define EXT_EVENT_GET_EVENT_MISSION_REQ "geventmissionreq"
#define EXT_EVENT_GET_EVENT_MISSION_RESP "geventmissionresp"
#define EVENT_MISSION_RESULT_LIST "emsl"

#define EXT_EVENT_EVENT_MISSION_REQ "eventmissionreq"
#define EXT_EVENT_EVENT_MISSION_RESP "eventmissionresp"
#define ACC_EXP "exp"

// giảm phế
#define EXT_EVENT_GET_DISCOUNT_INF_REQ "gdcinfreq"
#define EXT_EVENT_GET_DISCOUNT_INF_RESP "gdcinfresp"
#define DISCOUNT_INFO_RESULT "dcinfr"
//nhan thuong nv
#define EXT_EVENT_NEW_MISSION_REQ "nmreq"
#define EXT_EVENT_NEW_MISSION_RESP "nmresp"
#define EVENT_MISSION_ID "evmid"

//update bm3
#define EXT_EVENT_UPDATE_DISPLAY_NAME_REQ "udnreq"
#define ACC_DISPLAY_NAME "adn"
#define RESULT_CODE "rc"
#define RESULT_DESC "rd"
#define EXT_EVENT_UPDATE_DISPLAY_NAME_RESP "udnresp"
#define EXT_EVENT_REV_AWARD_ATTENT_REQ "rawardatreq"
#define AWARD_GOLD "awardgold"
#define AWARD_SILVER "awardsilver"
#define AWARD_EXP "awardexp"
#define EXT_EVENT_REV_AWARD_ATTENT_RESP "rawardatresp"
#define EXT_EVENT_GET_FAKE_DAILY_REQ "gfdaireq"
#define EXT_EVENT_GET_FAKE_DAILY_RESP "gfdairesp"
#define EXT_EVENT_UPDATE_BANK_ACC_REQ "ubankaccreq"
#define BANK_ID "bankid"
#define BANK_NUMBER "banknum"
#define BANK_NAME "bankname"
#define BANK_BRANCH "bankbranch"
#define EXT_EVENT_UPDATE_BANK_ACC_RESP "ubankaccresp"
#define EXT_EVENT_GET_ACC_BANK_INFOR_REQ "gaccbankinforeq"
#define EXT_EVENT_GET_ACC_BANK_INFOR_RESP "gaccbankinforesp"
#define EXT_EVENT_GET_SUPPORTED_BANK_REQ "gspbreq"
#define EXT_EVENT_GET_SUPPORTED_BANK_RESP "gspbresp"
#define EXT_EVENT_GOLD2_BANK_CREATE_REQ "g2bcreq"
#define AMOUNT_BET "ab"
#define RECHARGE_DESC "rcdesc"
#define OSID "osid"
#define DEVICE_ID "devid"
#define TAX_VALUE = "tv";
#define REAL_GOLD = "realgold";
#define EXT_EVENT_GOLD2_BANK_CREATE_RESP "g2bcresp"
#define OTP_CODE "otpcode"
#define EXT_EVENT_RECEIVE_AWARD_SILVER_DAILY_REQ "rasdreq"
#define EXT_EVENT_RECEIVE_AWARD_SILVER_DAILY_RESP "rasdresp"

//XD Mini
#define EXT_EVENT_START_XDMINI "xds"
#define EXT_EVENT_END_XDMINI "xde"
#define EXT_EVENT_GAME_RESULT_XDMINI "xdgrs"
#define EXT_EVENT_GAME_BET_ERROR_NOTIFY_XDMINI "xde_bntf"
#define EXT_EVENT_GAME_INFO_NTF_XDMINI "xdgi_ntf"
#define EXT_EVENT_GAME_HIS_NTF_XDMINI "xdgh_ntf"
#define EXT_EVENT_NOT_ENOUGH_MONEY_NTF_XDMINI "xdnem_ntf"
#define EXT_EVENT_GAME_STATE_NTF_XDMINI "xdgs_ntf"
#define EXT_EVENT_GAME_BET_NTF_XDMINI "xdgb_ntf"
#define EXT_EVENT_GAME_BET_REQ_MINI_XD "xdgbr"

#define EXT_FIELD_GAME_RETURN_VALUE "grv"
#define EXT_EVENT_XD_REFUND_NTF "xdrfntf"

#define EXT_EVENT_GET_GAME_INFO_REQ_XD_MINI "xdtgi_ntf"
#define EXT_FIELD_GAME_TOTAL_USER_BET_CELL "gtbuc"
#define EXT_FIELD_GAME_WIN_VALUE "gwv"
#define EXT_FIELD_GAME_RETURN_VALUE "grv"
#define EXT_EVENT_XD_GET_QUY_THUONG_REQ "xdgqtr"
#define EXT_EVENT_XD_GET_QUY_THUONG_RESP "xdgqtresp"
//field xd
#define EXT_FIELD_GAME_TOTAL_USER_BET_CELL "gtbuc";
#define EXT_FIELD_GAME_BET_TYPE "gbt"
//His xd Mini
#define EXT_EVENT_XD_USER_HIS_REQ "xduhrq"
#define EXT_EVENT_XD_USER_HIS_RESP "xduhresp"
#define EXT_EVENT_XD_GAME_HIS_DETAIL_REQ "xdghdrq"
#define EXT_EVENT_XD_GAME_HIS_DETAIL_RESP "xdghdresp"
#define EXT_EVENT_XD_GAME_HIS_REQ "xdghrq"
#define EXT_EVENT_XD_GAME_HIS_RESP "xdghresp"
#define EXT_EVENT_XD_GAME_HIS_DETAIL_REQ "xdghdrq"
#define EXT_EVENT_XD_GAME_HIS_DETAIL_RESP "xdghdresp"
//rank xd mini
#define EXT_EVENT_XD_RANK_REQ "xdrrq"
#define EXT_EVENT_XD_RANK_RESP "xdrresp"
//
#define BET_TYPE_DO "DO"
#define BET_TYPE_DEN "DEN"

//giftcode
#define RECEIVED_GIFT_CODE_AWARD_REQ "rgcawardreq"
#define GIFT_CODE "giftcode"
#define RECEIVED_GIFT_CODE_AWARD_RESP "rgcawardresp"

//txmini new
//Chức năng mở bát: bm 2

#define EXT_EVENT_MOBAT_NTF_TXMN "txodntf"
#define EXT_EVENT_MOBAT_REGISTER_REQUEST_TXMN "txodrrq"
#define EXT_EVENT_MOBAT_REGISTER_RESPONSE_TXMN "txodrresp"
#define EXT_EVENT_MOBAT_TIME_TXMN "txodtntf"
#define EXT_EVENT_MOBAT_REQUEST_TXMN "txodrq"
#define EXT_EVENT_GET_LIST_MOBAT_REQUEST_TXMN "txglodrq"
#define EXT_EVENT_GET_LIST_MOBAT_RESPONSE_TXMN "txglodresp"
#define EXT_EVENT_GET_LIST_HIS_REQUEST_TXMN "txglhrq"
#define EXT_EVENT_GET_LIST_HIS_RESPONSE_TXMN "txglhresp"
#define EXT_EVENT_HISTORY_ANALYTICS_TX_REQ "eehatrq"
#define EXT_EVENT_HISTORY_ANALYTICS_TX_RESP "eehatrs"

//VQMM
#define EXT_EVENT_VQ_ROTATE_REQ "vqrtreq"
#define EXT_EVENT_VQ_ROTATE_RESP "vqrtresp"
#define EXT_FIELD_GAME_VQ_RESULT "vqrs"
#define EXT_EVENT_GAME_START_VQMM "vqs"
#define EXT_EVENT_GAME_END_VQMM "vqe"
#define EXT_EVENT_GAME_BET_REQ_VQMM "vqgbr"
#define EXT_EVENT_GAME_RESULT_VQMM "vqgrs"
#define EXT_EVENT_VQ_RANK_REQ "vqrrq"
#define EXT_EVENT_VQ_RANK_RESP "vqrresp"
#define EXT_EVENT_VQ_USER_HIS_REQ "vquhrq"
#define EXT_EVENT_VQ_USER_HIS_RESP "vquhresp"
#define EXT_EVENT_GAME_BET_NTF_VQMINI "vqgb_ntf"
#define EXT_EVENT_GAME_STATE_NTF_VQMINI "vqgs_ntf"
#define EXT_EVENT_GAME_BET_ERROR_NOTIFY_VQMINI "vqe_bntf"
#define EXT_FIELD_GAME_HISTORY_TOP_GOLD_PLAYER "ghtgp"
#define EXT_FIELD_GAME_HISTORY_TOP_SILVER_PLAYER "ghtsp"

#define RESCODE_GAME_ALREADY_BUY_LOTTERY 33
#define RESCODE_GAME_IS_PLAYING 31
#define EXT_FIELD_GAME_VQ_RESULT "vqrs"
#define MAT_LUOT_VQMM 5
#define NO_HU_VQMM 1
#define HAU_TO_EXP 2
#define HAU_TO_GOLD 0
#define HAU_TO_SILVER 1
#define EXT_FIELD_GAME_VQ_BOUGHT_ROUND "vqbr"
#define EXT_FIELD_GAME_VQ_PRICE "vqprice"

#define W_MONEY_BG 173
#define H_MONEY_BG 73
#define LANG_CODE "lc"

#define EXT_EVENT_NEXT_DEAL_NTF "nxtdlntf"
#define EXT_EVENT_DEAL_COMMON_CARD_NOTIF "dlcmctf"
#define EXT_EVENT_SMALLBLIND_NOTIF "sbntf"
#define EXT_EVENT_BIGBLIND_NOTIF "bbntf"
#define EXT_FIELD_POKER_DEALER_SB_BB "dsb"
//VQ
#define EXT_EVENT_GAME_INFO_NTF "gi_ntf"
#define EXT_FIELD_GAME_INFO_VQ "ginfo"
#define EXT_FIELD_GAME_INFO_NEW "ginf"
#define EXT_EVENT_GAME_ROTATE_REQ "grreq"
#define EXT_FIELD_SLOT_LINE_COUNT "sllc"
#define EXT_FIELD_GAME_BET_VALUE "gbv"
#define EXT_EVENT_GAME_ROTATE_RES "grres"
#define EXT_FIELD_SLOT_BET_LIST "slbt"
#define EXT_FIELD_SLOT_WIN_VALUE "slwvl"
#define SLOT_777 1
#define SLOT_VUONGMIEN 2
#define SLOT_KIWI 3
#define SLOT_CAM 4
#define SLOT_DUAHAU 5
#define SLOT_BAR 6
#define SLOT_KIMCUONG 7
#define SLOT_RUBY 8
#define SLOT_SAO 9
#define EXT_FIELD_SLOT_WIN_LINE_LIST "slwll"
#define EXT_FIELD_SLOT_SESSION_ID "slsid"
#define EXT_FIELD_SLOT_AUTO_ROTATE "slaurt"
#define EXT_EVENT_SLOT_AUTO_PLAY_REQ "sapreq"
#define EXT_EVENT_GAME_AUTO_RES "sapres"
#define EXT_EVENT_SLOT_GAME_START "esgs"

//catte
#define EXT_FIELD_CARD_OPEN "cropn"
#define ACT_OPEN 1
#define ACT_FOLD 0
#define EXT_EVENT_CHET_TUNG_NOTIF "chtntf"
#define EXT_FIELD_CARD_ID "crdvl"
#define EXT_EVENT_CHUNG_NOTIF "chungntf"
#define EXT_EVENT_OPEN_CARD_NOTIF "opcntf"
#define EXT_FIELD_VICTORY_TYPE "vttp"

//3 doi
#define EXT_EVENT_HIT_BOC_CARD_NOTIF "hitboccmd"
#define EXT_FIELD_ACTION_TYPE "atnotif"
#define EXT_FIELD_CARD_VALUE "cvnotif"
#define HAS_3_DOI 1
#define HAS_3_DOI_THONG 2
#define HAS_TU_QUY 3
#define HAS_CU_LU 4
#define HAS_3_DOI_ON_HAND 5
#define HAS_3_DOI_THONG_ON_HAND 6
#define HAS_TU_QUY_ON_HAND 7
#define EXT_FIELD_PRE_USERID "preuid"
#define EXT_EVENT_STATUS_CARD_NOTIF "sccmd"
#define EXT_EVENT_GHEP_DOI_NOTIF "gdnotif"
#define EXT_FIELD_LIST_CARD_REMAIN "lcrm"

//new payment, cashout CAM
//1. Gửi mã bảo mật:
#define EXT_EVENT_WING_GEN_SECURITY_CODE_REQ "ewgscreq"
//; (String)
#define EXT_FIELD_WING_ACCOUNT "wacc"
//; (String)
#define EXT_FIELD_WING_PHONE "wphone"
#define EXT_FIELD_WING_AMOUNT "wamount"
#define EXT_EVENT_WING_GEN_SECURITY_CODE_RESP "ewgscresp"


//2. Thanh toán:
#define EXT_EVENT_WING_RECHARGE_REQ "ewrcreq"
//; (String)
#define EXT_FIELD_WING_ACCOUNT "wacc"
//(Int)
#define EXT_FIELD_WING_AMOUNT "wamount"
//; (String)
#define EXT_FIELD_WING_SECURITY_CODE "wsecucode"
#define EXT_EVENT_WING_RECHARGE_RESP "ewrcresp"


//3. Cashout:
#define EXT_EVENT_WING_CASHOUT_REQ "ewcoreq"
//; (String)
#define EXT_FIELD_WING_ACCOUNT "wacc"
//; (Int)
#define EXT_FIELD_WING_AMOUNT "wamount"
#define EXT_EVENT_WING_CASHOUT_RESP "ewcoresp"

//dem la cam
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_TU_2 6
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_4_SAM 5
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_6_DOI 4
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_5_THONG 3
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_6_THONG 2
#define GAME_TIENLEN_CAM_TOI_TRANG_TYPE_SANH_RONG 1

//new event handle
#define EXTENSION_REQUEST_LIST_EVENT_REQUEST "erlerq"
#define EXTENSION_REQUEST_LIST_EVENT_RESPONSE "erlers"
#define LIST_EVENT_RUNNING "ler"
#define EXTENSION_REQUEST_REWARD_EVENT_REQUEST "errerq"
#define EXTENSION_REQUEST_REWARD_EVENT_REPONSE "erreres"
#define TYPE_EVENT_REWARD "ter"
//notify
#define EVENT_NOTIFY_MESSAGE_HANDLE "enmh"
#define EXT_FIELD_MESSAGE_STRING "notiMSG"

//CellCard
#define EXT_EVENT_FORTUMO_GET_LIST_EXCHANGE_REQ "ftglereq"
#define EXT_EVENT_FORTUMO_GET_LIST_EXCHANGE_RESP "ftgleresp"
#define EXT_EVENT_FORTUMO_EXCHANGE_REQ "ftereq"
#define EXT_EVENT_FORTUMO_EXCHANGE_RESP "fteresp"
#define EXT_FIELD_FORTUMO_VALUE "ftval"
#define EXT_FIELD_FORTUMO_EXCHANGE_RESULT "ftexr"
#define EXT_FIELD_FORTUMO_URL "fturl"
#define EXT_EVENT_FORTUMO_GET_CODE_REQ "gfcreq"
#define EXT_EVENT_FORTUMO_GET_CODE_RESP "gfcresp"
#define EXT_FIELD_FORTUMO_CODE "ftcode"
#define EXT_FIELD_FORTUMO_EXCHANGE_LIST "ftexl"
#define EXT_EVENT_FORTUMO_CHARGE_SUCCESS_NTF "ftcsntf"
#define EXT_FIELD_FORTUMO_MESSAGE "ftmsg"


//new
#define EXT_EVENT_GET_JACKPOT_REQ "gjpreq"
#define EXT_EVENT_GET_JACKPOT_RES "gjpres"
#define EXT_EVENT_OPEN_JACKPOT_NTF "ojpntf"
#define EXT_EVENT_CHANGE_JACKPOT_NTF "cjpntf"
#define EXT_EVENT_NOTIFY_SHOW_NOTICE "eensn"
#define EXT_EVENT_GET_TOP_REQ "gtreq"
#define EXT_EVENT_GET_TOP_RES "gtres"
#define EXT_FIELD_JACKPOT_VALUE "jpvl"
#define EXT_FIELD_SLOT_TOP_LIST "sltl"
#define EXT_EVENT_EAT_JACKPOT_NTF "ejpntf"
#define EXT_FIELD_SLOT_JACKPOT_LINE "sljpl"
#define EXT_FIELD_AMOUNT_MONEY "am"


//
#define GAME_EXT_EVENT_GET_TOP_REQ "toprq"
#define GAME_EXT_EVENT_GET_TOP_RESP "toprs"
#define EXT_FIELD_RESULT_TOP_10 "rt10"

//
#define EXT_EVENT_TCC_JACKPOT_NTF "tccjpntf"
#define EXT_FIELD_GAME_TIME_OPEN_EVENT "toet"
#define EXT_FIELD_GAME_JACKPOT_VALUE "jkpvl"
#define EXT_EVENT_TCC_GET_TOP_POINT_JACKPOT_REQ "tccgtprq"
#define EXT_EVENT_TCC_GET_TOP_POINT_JACKPOT_RES "tccgtpresp"
#define EXT_FIELD_GAME_JACKPOT_TOP_POINT "jkptp"
#define EXT_EVENT_TCC_GET_TOP_OPEN_JACKPOT_REQ "tccgtoprq"
#define EXT_EVENT_TCC_GET_TOP_OPEN_JACKPOT_RES "tccgtopresp"
#define EXT_FIELD_GAME_JACKPOT_TOP_OPEN_MONEY "jkptopm"
#define EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF "tccoqt_ntf"
#define EXT_FIELD_GAME_HIS_QUY_THUONG "ghqt"
#define EXT_EVENT_TCC_JACKPOT_NTF "tccjpntf"

//doi the
#define CELL_CARD 1
#define MET_FONE 2
#define SMART 3
#define EXT_EVENT_GET_INFO_CARD_REQ "eegicr"
#define EXT_EVENT_GET_INFO_CARD_RESP "eegicres"
#define EXT_INFO "ei"

//poker new
//1. Bổ xung thêm bản tin TIP tiền cho dealer

#define EXT_EVENT_TIP_DEALER_REQ "tipdreq"
//ko có tham số gì

#define EXT_EVENT_TIP_DEALER_RES "tipdres"
#define EXT_EVENT_TIP_DEAL_NOTIFY "tipdntf"
//có 2 tham số

#define EXT_FIELD_TIP_VAL "tipvl"
//double là số tiền tip`
#define EXT_FIELD_TIP_MSG "tipsmg"
//String: sẽ vẽ thông điệp này từ mồm dealer

//Khi tip sẽ animation tiền từ user bay tới dealer, và khi bay tới dealer sẽ cảm ơn

//2. Thêm bản tin Set autobet và auto bet notify

//Server sẽ tính các giá trị có thể đặt autobet cho người chơi và client sẽ hiện thị autobet ấy
#define EXT_EVENT_AUTO_BET_NOTIFY "autobtntf"

//Tham số
#define EXT_FIELD_AUTOBET_LIST "autobtlst"
//string, là các giá trị cách nhau bới dấu , có các giá trị autobet như sau

#define POKER_AUTO_FOLD 1
#define POKER_AUTO_CHECK 2
#define POKER_AUTO_CHECKFOLD 3
#define POKER_AUTO_CALL_ANNY 5

//Gửi lên khi chọn một giá trị atuobet
#define EXT_EVENT_AUTO_BET_REQ "autobtreq"
//Chỉ có 1 trường là
#define EXT_FIELD_AUTOBET_VAL "autobt"
//int : giá trị autobet được chon

#define EXT_EVENT_AUTO_BET_RES "autobtres"

//Có 2 trương
#define EXT_FIELD_AUTOBET_VAL "autobt"
#define EXT_FIELD_AUTOBET_LIST "autobtlst"


//3. thêm bản tính bài tốt nhất sau mỗi vòng
#define EXT_EVENT_BEST_CARD_NOTIFY "bstcntf"
//
//endntf.putUtfString(ServerFieldID.EXT_FIELD_USERID, p.getUser().getName());
//endntf.putInt(ServerFieldID.EXT_FIELD_VICTORY_TYPE, p.getMaxCardType());
//endntf.putUtfString(ServerFieldID.EXT_FIELD_LIST_CARD, StringUtil.list2String(p.getLstBestCard()));
//các trường này giống bản tin victory
//
#define EXT_EVENT_CANCEL_AUTO_BET_REQ "cautbtreq"
#define EXT_EVENT_CANCEL_AUTO_BET_RES "cautbtres"



//event stick
#define EXT_EVENT_STICKER_GAME_NTF "stkntf"
#define FISRT_CHARGE_PROMOTION "1stcpro"
#define STICKER_GAME "stkgame"

enum{
	GAME_SAM_TOI_TRANG_TYPE_5_DOI = 5,
	GAME_SAM_TOI_TRANG_TYPE_3_SAM = 4,
	GAME_SAM_TOI_TRANG_TYPE_DONG_CHAT = 3,
	GAME_SAM_TOI_TRANG_TYPE_4_HEO = 2,
	GAME_SAM_TOI_TRANG_TYPE_SANH_10 = 1,
	GAME_SAM_TOI_TRANG_NORMAL = 21,
	//8:53 : 37 AM] mrdung Do Tien Dung : Define cho Dem La
	GAME_TIENLEN_TOI_TRANG_TYPE_TU_3 = 7,
	GAME_TIENLEN_TOI_TRANG_TYPE_TU_2 = 6,
	GAME_TIENLEN_TOI_TRANG_TYPE_4_SAM = 5,
	GAME_TIENLEN_TOI_TRANG_TYPE_6_DOI = 4,
	GAME_TIENLEN_TOI_TRANG_TYPE_5_THONG = 3,
	GAME_TIENLEN_TOI_TRANG_TYPE_DONG_MAU = 2,
	GAME_TIENLEN_TOI_TRANG_TYPE_SANH_RONG = 1
};
enum{
    GAME_CATTE_CHET_TUNG = 4,
    GAME_CATTE_TOI_TRANG_TYPE_TU_QUY = 3,
    GAME_CATTE_TOI_TRANG_DONGCHAT = 2,
    GAME_CATTE_TOI_TRANG_TYPE_BE_6 = 1,
    GAME_CATTE_VICTORY_TYPE_THANG_TUNG  = 5
};

static int FONT_SIZE_TITLE = 50;
static int FONT_SIZE_NORMAL = 40;
static int FONT_SIZE_SMALL = 35;

static int widButton1 = 224;
static int widButton2 = 284;
static int W_AVATAR = 159;
static int AVATAR_L_SPACE = 25;
static int AVATAR_T_SPACE = 25;
enum{
	S_EAT_CARD = 0,
	S_UPDATE_LIST,
	S_START_GAME,
	S_END_GAME,
	S_WIN,
	S_HIT_CARD,
	S_GIVE_CARD,
	S_HAND_OFF,
	S_U,
	S_DEAL,
	S_SORT,
	S_HIT_CARD_TL,
	S_NEXT_TURN,
	/// for chinese chess
	S_CHESS_MOVE,
	S_CLICK_CHESS,
	S_KING_CHECK,
	S_LOSE_CHESS,
	S_WIN_CHESS,
	S_START_CHESS,
	S_TIME_COUNT,
};
//gamebai version
enum GameVersion
{
	p1,
	p2
};
struct currencyStruct{
	double valueStruct;
	string symbolStruct;
};
static string convertMoneyExSymbol(double money, int digit){
	char sMoneyTmp[30], sMoney[30], s[30];

	sprintf(sMoneyTmp, "%d", money);
	int len = strlen(sMoneyTmp);

	int i = len - 1, counterSeg = 0, counter = 0;

	while (i >= 0){
		sMoney[counter++] = sMoneyTmp[i];
		counterSeg++;
		if (counterSeg == digit){
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
static string formatMoneySymbolChest(double money)
{
	if (money < 10000)
	{
		string rs = mUtils::convertMoneyEx(money);
		return rs;
	}
	int digits = 1;
	vector<currencyStruct> arrSi;
	for (int i = 0; i < 3; i++)
		arrSi.push_back(currencyStruct());

	arrSi[2].symbolStruct = "K";
	arrSi[2].valueStruct = 1000;

	arrSi[1].symbolStruct = "M";
	arrSi[1].valueStruct = 1000000;

	arrSi[0].symbolStruct = "B";
	arrSi[0].valueStruct = 1000000000;
	//
	// 	arrSi[0].symbolStruct = "T";
	// 	arrSi[0].valueStruct = 1000000000000;

	for (int i = 0; i < arrSi.size(); ++i)
	{
		if (money >= arrSi.at(i).valueStruct)
		{
			float val = money / arrSi.at(i).valueStruct;
			if (fmodf(money, 1000) == 0)
				return StringUtils::format("%.0f", val) + " " + arrSi[i].symbolStruct;
			return StringUtils::format("%.1f", val) + " " + arrSi[i].symbolStruct;
		}
	}
	return boost::to_string(money);
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

	// 	arrSi[1].symbolStruct = "B";
	// 	arrSi[1].valueStruct = 1000000000;
	// 
	// 	arrSi[0].symbolStruct = "T";
	// 	arrSi[0].valueStruct = 1000000000000;

	for (int i = 0; i < arrSi.size(); ++i)
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
static string formatMoneySymbol(string _money)
{
	if (0 == _money.compare(""))
		return "";
	double money = atol(_money.c_str());
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

	// 	arrSi[1].symbolStruct = "B";
	// 	arrSi[1].valueStruct = 1000000000;
	// 
	// 	arrSi[0].symbolStruct = "T";
	// 	arrSi[0].valueStruct = 1000000000000;

	for (int i = 0; i < arrSi.size(); ++i)
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
static string formatMoneyPicker(double money)
{
	if (money < 1000)
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

	for (int i = 0; i < arrSi.size(); ++i)
	{
		if (money >= arrSi.at(i).valueStruct)
		{
			float val = money / arrSi.at(i).valueStruct;
			if (fmodf(money, 1000) == 0)
				return StringUtils::format("%.0f", val)  + arrSi[i].symbolStruct;
			return StringUtils::format("%.1f", val)  + arrSi[i].symbolStruct;
		}
	}
	return boost::to_string(money);
}
static string formatUserName(string _userName, int _length)
{
	string userName = _userName;
	if (userName.size() > _length)
	{
		return userName.replace(_length, userName.size(), "...");
	}
	else
	{
		return userName;
	}
}
static std::string utf8_substr(const std::string& str, unsigned int start, unsigned int leng)
{
	if (leng == 0) { return ""; }
	unsigned int c, i, ix, q, min = std::string::npos, max = std::string::npos;
	for (q = 0, i = 0, ix = str.length(); i < ix; i++, q++)
	{
		if (q == start){ min = i; }
		if (q <= start + leng || leng == std::string::npos){ max = i; }

		c = (unsigned char)str[i];
		if (
			//c>=0   &&
			c <= 127) i += 0;
		else if ((c & 0xE0) == 0xC0) i += 1;
		else if ((c & 0xF0) == 0xE0) i += 2;
		else if ((c & 0xF8) == 0xF0) i += 3;
		//else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
		//else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
		else return "";//invalid utf8
	}
	if (q <= start + leng || leng == std::string::npos){ max = i; }
	if (min == std::string::npos || max == std::string::npos) { return ""; }
	return str.substr(min, max);
}
//ham danh cho viec set bien bat tat ga vao userdefault va uservariable
static void setUckVariable(bool isUse)
{
	UserDefault *def = UserDefault::getInstance();
	def->setBoolForKey("UCK", isUse);
	def->flush();
	boost::shared_ptr<vector<boost::shared_ptr<UserVariable> > > collectionUserVariable(new vector<boost::shared_ptr<UserVariable> >());
	boost::shared_ptr<int> _a(new int(isUse ? LayerGa::CONFIG::ON : LayerGa::CONFIG::OFF));
	boost::shared_ptr<SFSUserVariable> variable(new SFSUserVariable("uck", _a, VARIABLETYPE_INT));
	collectionUserVariable->push_back(variable);	//
	boost::shared_ptr<IRequest> request(new SetUserVariablesRequest(collectionUserVariable));
	GameServer::getSingleton().Send(request);
}
static void setUckToUserDefault(bool isUse)
{
	UserDefault *def = UserDefault::getInstance();
	def->setBoolForKey("UCK", isUse);
	def->flush();
}
static bool isRecommendOn()
{
	//Configuration::getInstance()->setValue("onRecommend", Value("0"));
    if (!dataManager.getAppConfig().isrecommend) return false;
	bool isOn = UserDefault::getInstance()->getBoolForKey("chbRecommend", false);
    //only for test, release = isOn
	return isOn;
	 //	bool _valueConfig = Configuration::getInstance()->getValue("onRecommend", Value("true")).asBool();
	// 	return _valueConfig;
}

#endif // PhomMessDef_h__
