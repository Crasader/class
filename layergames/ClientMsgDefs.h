#ifndef _iCasino_ClientMsgDefs_h_
#define _iCasino_ClientMsgDefs_h_

//Event
#define EXT_EVENT_END "e"
#define EXT_EVENT_REQ_HANDOFF_PHOM "rqhofp"                 
#define EXT_EVENT_START_GAME_NOTIF "strntf"
#define EXT_EVENT_SUB_BALANCE_FIRST_NOTIF "sfstntf"
#define EXT_EVENT_RES_JOIN_GAME "rsjg"                      
#define EXT_EVENT_USER_READY_NOTIF "rdyntf"
#define EXT_EVENT_GAME_STATE_NTF "gs_ntf"
#define EXT_EVENT_REQ_END_PLAYING "rqenpl"                  
#define EXT_EVENT_USER_HAVE_SET_NOTIF "hvntf"
#define EXT_EVENT_RES_END_PLAYING "rsenpl"                  
#define EXT_EVENT_GAME_HIS_NTF "gh_ntf"
#define EXT_EVENT_DEAL_CARD_RES "dcrs"
#define EXT_EVENT_NEXT_TURN "nt"
#define EXT_EVENT_LIST_USER_UPDATE "luu"
#define EXT_EVENT_REQ_DISCARD "rqhofc"                      
#define EXT_EVENT_USER_JOIN_NOTIF "jrntf"
#define EXT_EVENT_USER_EXPIRE_MONEY_NOTIF "expmntf"
#define EXT_EVENT_REQ_NOC_DETAIL "rqnocdtl"                 
#define EXT_EVENT_GAME_INFO_NTF "gi_ntf"
#define EXT_EVENT_RES_DUOI_CARD "rsduoic"                   
#define EXT_EVENT_REQ_U "rqU"                               
#define EXT_EVENT_RAISE_REQ "rsrq"
#define EXT_EVENT_RAISE_RES "rsrs"
#define EXT_EVENT_FOLD_REQ "fdrq"
#define EXT_EVENT_OPEN_LAST_CARD_REQ "olstrq"
#define EXT_EVENT_RES_TRENTAY_DETAIL "rsttdtl"              
#define EXT_EVENT_FOLD_RES "fdrs"
#define EXT_EVENT_REQ_LEAVE_GAME "rqlg"                     
#define EXT_EVENT_OPEN_LAST_CARD_RES "olstrs"
#define EXT_EVENT_READY_REQ "rr"
#define EXT_EVENT_RAISE_NOTIF "rsntf"
#define EXT_EVENT_UNREADY_RES "urdres"
#define EXT_EVENT_UNREADY_REQ "urr"
#define EXT_EVENT_REQ_DUOI_CARD "rqduoic"                   
#define EXT_EVENT_REQ_U_ANNOUCE "rquanou"                   
#define EXT_EVENT_USER_LEAVE_NOTIF "lrntf"
#define EXT_EVENT_ERROR_READY_NTF "e_rntf"
#define EXT_EVENT_USER_KICKED_NOTIF "kkntf"
#define EXT_EVENT_REQ_TAKE_CARD "rqtkc"                     
#define EXT_EVENT_OPEN_ALL_CARD_NOTIF "opaltf"
#define EXT_EVENT_GAME_BET_ERROR_NOTIFY "e_bntf"
#define EXT_EVENT_REQ_CHIU_CARD "rqchiuc"                   
#define EXT_EVENT_OPEN_CARD_NOTIF "opcntf"
#define EXT_EVENT_NEAD_RAISE_NOTIF "ndrntf"
#define EXT_EVENT_REQ_PUSH_CARD "rqpusc"                    
#define EXT_EVENT_RES_BOC_CAI "rsbocc"                      
#define EXT_EVENT_RES_U "rsU"                               
#define EXT_EVENT_REQ_SET_BET "rqsbet"                      
#define EXT_EVENT_START "s"
#define EXT_EVENT_DEAL_CARD_NOTIF "dlcntf"
#define EXT_EVENT_GAME_RESULT "grs"
#define EXT_EVENT_START_READY_TIMER_NOTIF "rdtntf"
#define EXT_EVENT_REQ_KICKOUT "kckrq"                       
#define EXT_EVENT_REQ_BOC_CAI "rqbocc"                      
#define EXT_EVENT_KICK_RES "kckrs"
#define EXT_EVENT_KICK_REQ "kckrq"
#define EXT_EVENT_RES_DISCARD "rshofc"                      
#define EXT_EVENT_RES_DRAW_CARD "rsdrwc"                    
#define EXT_EVENT_USER_PLAYING_NOTIF "apin"
#define EXT_EVENT_GAME_BET_REQ "gbr"
#define EXT_EVENT_END_GAME_NOTIF "endntf"
#define EXT_EVENT_READY_NTF "rntf"
#define EXT_EVENT_RES_U_ANNOUCE "rsuanou"                   
#define EXT_EVENT_CHANGE_BALANCE_NOTIF "cblltf"
#define EXT_EVENT_RES_ORDER_CARDS "rsodrc"                  
#define EXT_EVENT_RES_CHIU_CARD "rschiuc"                   
#define EXT_EVENT_REQ_DRAW_CARD "rqdrwc"                    
#define EXT_EVENT_REQ_ORDER_CARDS "rodrc"                   
#define EXT_EVENT_RES_SET_BET "rssbet"                      
#define EXT_EVENT_TURNUP_CARD "tuc"
#define EXT_EVENT_READY_RES "rdres"
#define EXT_EVENT_RES_HANDOFF_PHOM "rshofp"                 
#define EXT_EVENT_LISTCARD_NTF "lcntf"
#define EXT_EVENT_RES_NOC_DETAIL "rsnocdtl"                 
#define EXT_EVENT_RES_LEAVE_GAME "rslg"                     
#define EXT_EVENT_REQ_TRENTAY_DETAIL "rqttdtl"              
#define EXT_EVENT_RES_PUSH_CARD "rspusc"                    
#define EXT_EVENT_NOT_ENOUGH_MONEY_NTF "nem_ntf"
#define EXT_EVENT_VICTORY_NOTIF "vicntf"
#define EXT_EVENT_RES_UPDATE_GAME_CONFIG "rsgcfg"           
#define EXT_EVENT_DEAL_CARD_REQ "dcrq"
#define EXT_EVENT_UNREADY_NTF "urntf"
#define EXT_EVENT_REQ_JOIN_GAME "rqjg" 
#define EXT_EVENT_REQ_UPDATE_GAME_CONFIG "rqgcfg"           
#define EXT_EVENT_GAME_CHANGE_NOTIF "gchntf"
#define EXT_EVENT_SELECT_OPEN_CARD_REQ "slstrq"
#define EXT_EVENT_SELECT_OPEN_CARD_RES "slsrrs"
#define EXT_EVENT_GAME_BET_NTF "gb_ntf"
#define EXT_EVENT_RES_TAKE_CARD "rstkc"
#define EXT_EVENT_CHICKEN_UPDATE_NTF "chkntf"
#define EXT_EVENT_GAME_CHIKKEN_CONFIG_NOTIFY "ckcntf"
#define EXT_EVENT_WAIT_BET_TIMER_NTF "wbntf"
#define EXT_FIELD_CONFIRM "cf"
#define EXT_EVENT_USE_CHICKEN_REQ "ucr"

//Them moi
//bản tin cho phép client gửi lên đăng kí chơi ván sau khi user đang là khách.
#define EXT_EVENT_PLAY_REGISTER_REQ "prr"

//Khi Extension nhận đc sẽ kiểm tra nếu số chỗ trống vẫn còn đủ thì response OK,
//còn nếu đã hất chỗ trống cho ván sau thì response lỗi
#define EXT_EVENT_PLAY_REGISTER_RES "prs"

//Trong bản tin LIST_USER_UPDATE = “luu” sẽ bổ xung thêm 1 trường rp chứa danh sách những thằng chờ vào chơi.
#define EXT_FIELD_REG_PLAY "rp"

#define EXT_EVENT_REGISTER_QUEUE_NTF "rqntf"

//
#define EXT_SRVNTF_GAME_INFO "ntfginfo"                     
#define EXT_SRVNTF_U_DE "ntfud"
#define EXT_SRVNTF_ONHAND_DETAILS "ntfohdetl"               
#define EXT_SRVNTF_U_SUCCESS "ntfusc"
#define EXT_SRVNTF_CAN_TAKE "ntfcantk"                      
#define EXT_SRVNTF_ONE_EXPECTING_CHIU_U "ntfoechiuu"        
#define EXT_SRVNTF_PRIVATE_MESSAGE "ntfprvmsg"              
#define EXT_SRVNTF_USER_READY "ntfurd"                      
#define EXT_SRVNTF_RECOMMENDED_PUSH_CARDS "ntfpucs"         
#define EXT_SRVNTF_NOC_COUNT "ntfnoccnt"
#define EXT_SRVNTF_NOC_DETAIL "ntfnocdetl"
#define EXT_SRVNTF_TRENTAY_DETAIL "ntfttdetl"
#define EXT_SRVNTF_BROADCAST_MESSAGE "brcstmsg"             
#define EXT_SRVNTF_ONE_EXPECTING_CHIU "ntfoechiu"           
#define EXT_SRVNTF_ONE_EXPECTING_U "ntfoeu"                 
#define EXT_SRVNTF_CURRENT_PLAYING_USER "ntfcurpu"          
#define EXT_SRVNTF_RECOMMENDED_CARDSETS "ntfrcrds"          
#define EXT_SRVNTF_BOC_CAI "ntfbocc"
#define EXT_SRVNTF_PUBLIC_MESSAGE "ntfpubmsg"               
#define EXT_SRVNTF_BEGIN_ANNOUCING "ntfbgxc"
#define EXT_SRVNTF_ANBAO "ntfanbao"
#define EXT_SRVNTF_U_9 "ntfu9"                              
#define EXT_SRVNTF_PLAYER_LIST "ntfpllst"                   
#define EXT_SRVNTF_CARD_ORIGINATION "ntfcorg"               
#define EXT_SRVNTF_END_PLAYING_GAME "ntfepla"               
#define EXT_SRVNTF_COIN_CHANGE "ntfcoinch"                  
#define EXT_SRVNTF_PLAYER_KICKOUT "ntfko"                   
#define EXT_SRVNTF_GA_NUOI_SCORE "ntfgnsc"
#define EXT_SRVNTF_USER_UNREADY "ntfuurd"                   
#define EXT_SRVNTF_GAME_MASTER_INFO "ntfgminfo"             
#define EXT_SRVNTF_USER_LEAVE_ROOM "lrntf"
#define EXT_SRVNTF_U "ntfu"                                 
#define EXT_SRVNTF_PLAYER_REPLAY "ntfrpl"   
#define EXT_SRVNTF_CHIU_CARD_LIST "ntfchl"    // notify thông báo các quần bài có thể chíu
#define EXT_FIELD_GAME_TOTAL_PLAYER "ttp"
#define EXT_EVENT_LEAVE_GAME_RES "lgrs"
#define EXT_EVENT_GAME_LEAVE_VIP_RES "lvgrs"

//Field
#define EXT_FIELD_CARD_SUIT "cardsu"
#define EXT_FIELD_POSITION "p"
#define EXT_FIELD_CURRENT_BET "crbet"
#define EXT_FIELD_COIN_CHANGE_REASON "coinchr"      // coin change reason.    
#define EXT_FIELD_GAME_HISTORY_RESULT "ghs"
#define EXT_FIELD_DIS_DUOI_CARD_1 "discard1"          // cây bài đã đánh hoặc dưới (nói chung là đã đánh đi)
#define EXT_FIELD_DIS_DUOI_CARD_2 "discard2"          // cây bài đã đánh hoặc dưới (nói chung là đã đánh đi)
#define EXT_FIELD_CARD_ID "crdvl"
#define EXT_FIELD_ALL_PLAYER_CARD "alllc"
#define EXT_FIELD_IS_TURN_UP_ALL "tua"
#define EXT_FIELD_LIST_USER "lu"
#define EXT_FIELD_GAME_TIME "gtime"
#define EXT_FIELD_COIN_CHANGE "coinch"      // lượng tiền bị trừ hoặc tăng.
#define EXT_FIELD_GAME_ID "gameid"
#define EXT_FIELD_EXPECTING_U_REASON "eures"      // Expecting U reason
#define EXT_FIELD_ANBAO_REASON "anbaores"      // thông báo lỗi ăn báo
#define EXT_FIELD_GAME_TURN "gturn"        // trường giá trị vòng chơi hiện tại của game
#define EXT_FIELD_RESULT "rscode" //rslt
#define EXT_FIELD_ERROR_STRING "eexp"
#define EXT_FIELD_BET_TYPE "bet"
#define EXT_FIELD_TAKEN_CARD "tkcard"                // cây bài đã ăn
#define EXT_FIELD_GAME_BET_TYPE "gbt"
#define EXT_FIELD_CUOCSAC_LST "ucsac"
#define EXT_FIELD_GAME_BET_VALUE "gbv"
#define EXT_FIELD_CARD_INDEX "cardidx"      // id của 1 cây bài trong bộ bài (bộ 52 cây)
#define EXT_FIELD_USERID "uid"
#define EXT_FIELD_ANBAO_SUBREASON "anbaosres"         // sub reason để phân biệt 1 số trường hợp ăn báo, treo tranh phức tạp
#define EXT_FIELD_BET_VALUE "betvl"
#define EXT_FIELD_GAME_HISTORY_TOP_PLAYER "ghtp"
#define EXT_FIELD_CARDSETS "crdss"         // danh sách các phỏm recommend
#define EXT_FIELD_GAME_INFO "ginf"
#define EXT_FIELD_GAME_CFG "gcfg"    // game config xml
#define EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL "gtbvc"
#define EXT_FIELD_BET_VAL "betval"
#define EXT_FIELD_FROM_USERNAME "funa"      // userName nguồn gốc của cây bài
#define EXT_FIELD_USERNAME "usrn"
#define EXT_FIELD_TO_USERNAME "tuna"       // userName nhận cây bài
#define EXT_FIELD_GAME_BET_EXCHANGE "gbex"
#define EXT_FIELD_U_RANK "urnk"          // đánh dấu kiểu Ù (Ù 0 điểm, Ù 9 cây, Ù khan, Ù trên tay - Ù 10)
#define EXT_FIELD_NOC_REMAIN_COUNT "noccount"   // số bài còn lại dưới nọc
#define EXT_FIELD_VICTORY_TYPE "vttp"
#define EXT_FIELD_FROM_USERID "fuid"       // userID nguồn gốc của cây bài
#define EXT_FIELD_BOC_CAI_INFO "nocdetl"     // thông tin bốc cái
#define EXT_FIELD_ROOM_ID "roomid"
#define EXT_FIELD_GAME_EVENT "gchginf"
#define EXT_FIELD_GAME_RESULT "rg"
#define EXT_FIELD_PUSH_CARDS "pcrds"         // danh sách các card có thể push recommend
#define EXT_FIELD_UDE_USERID "uduid"     // ù đè User ID
#define EXT_FIELD_GAME_TOTAL_BET_VALUE "gtbv"
#define EXT_FIELD_TO_USERID "tuid"        // userID nhận cây bài
#define EXT_FIELD_BET_TYPE_ALLOW "betal"
#define EXT_FIELD_ONHANDCARD_DETAIL "ohdetl"
#define EXT_FIELD_PLAYING_STATUS "plistatus"
#define EXT_FIELD_ZONE_ID "zoneid"
#define EXT_FIELD_GAME_PLAY_STATE "gstate"
#define EXT_FIELD_RESULT_GAME "rg"
#define EXT_FIELD_CARD_NUMBER "cardnu"        
#define EXT_FIELD_BET_TOTAL "bettt"
#define EXT_FIELD_ERROR_CODE "errc"
#define EXT_FIELD_HA_CARD "hacard"                    // cây bài hạ xuống dưới chiếu hoặc cây bài trên tay lấy ra để ăn
#define EXT_FIELD_GAME_BET_CAN_EAT "gbce"
#define EXT_FIELD_RESULT_CODE "rscode"      // trường rs code trả về các bản tin repsonse
#define EXT_FIELD_UDE_USERNAME "udun"    // ù đè user name
#define EXT_FIELD_RESULT_TOP_10 "rt10"
#define EXT_FIELD_TXT_MSG "txtmsg"
#define EXT_FIELD_LIST_CARD "lc"
#define EXT_FIELD_CARD_OPEN "cropn"
#define EXT_FIELD_IGNORE_CARD "igncrd"               // cây bài đã bỏ không ăn
#define EXT_FIELD_NOC_REMAIN_DETAIL "nocdetl" 
#define EXT_FIELD_CAN_TAKE "cantk"        // chỉ ra quân bài có ăn đc hay ko
#define EXT_FIELD_CARD_ORIGINATION "crdorg"    // trường đánh dấu xuất xứ của 1 cây bài
#define EXT_FIELD_GA_NUOI_SCORE "gnsc"    // điểm gà nuôi
#define EXT_FIELD_KICKOUT_REASON "koutr"     // kickout reason
#define EXT_FIELD_CARD_COUNT "crdco"
#define EXT_FIELD_CHIU_CARDS "chcl"       // các quân bài có thể chíu
#define EXT_FIELD_TAKEN_CARD2 "tkcard2"
#define EXT_VAL_ACCOUNT_NAME "aN"
#define EXT_VAL_AMOUNT_BALANCE "amf"
#define EXT_VAL_CBT "cbt"
#define RES_ASK_ACC_PLAYING "rapi"
#define EXT_VAL_AMOUNT_AVATAR_LINK "aal"
#define CMD_ASK_ACC_PLAYING "aapi"
#define EXT_FIELD_GAME_CHIP "gmch"

//danh sach ban tin game lieng
#define EXT_EVENT_SETUP_CHIP_REQ "stchrq"
#define EXT_EVENT_SETUP_CHIP_RES  "stchrs"
#define EXT_FIELD_GAME_CHIP "gmch"
#define EXT_EVENT_EXPIRE_CHIP_NTF "expcntf"
#define EXT_VAL_MAX_CHIP "mxc"
#define EXT_FIELD_BRING_CHIP "brch"
#define EXT_FIELD_GAME_ACCOUNT_BET_VALUE "gabv"
#define EXT_FIELD_MIN_BET  "mb"

#define GAME_LIENG_BET  1
#define GAME_LIENG_FOLD  2
#define GAME_LIENG_FOLLOW  3
#define GAME_LIENG_NEXT  4;
#define EXT_EVENT_GET_TYPE_AN_REQ "eegtarq"
#define EXT_EVENT_GET_TYPE_AN_RES "eegtars"

#endif
