#ifndef ChanMessageDefs_h
#define ChanMessageDefs_h

#include "ClientMsgDefs.h"

#define NOCCOUNT_STOP_GAME 1

enum
{
	ANBAO_REASON_NO_PROBLEM = 0,
	ANBAO_REASON_AN_CA_DOI_CHO,
	ANBAO_REASON_BO_AN_CUATREN_BUT_AN_CUATRI,
	ANBAO_REASON_DISCARD_SAMEAS_CUATREN_CUATRI_DUOITAY,
	ANBAO_REASON_ANCA_DANHCA,
	ANBAO_TREOTRANH,
	ANBAO_DANH_1_CA_CHI_DUOC_AN_CHAN,
	ANBAO_REASON_BOCHAN_ANCA,
	ANBAO_REASON_DANHBAI_GIONG_CHANCA_DA_AN,
	ANBAO_REASON_DOI_U_BACHTHUCHI,
	ANBAO_REASON_BOCHAN_DANHCHAN,   // bỏ không ăn 1 quân làm chắn, sau lại đánh đi đúng cây đó.
	ANBAO_REASON_BOCHAN_ANCHAN,
	ANBAO_REASON_BOCA_ANCA,
	ANBAO_REASON_DANHCA_ANCA,        // đánh đi 1 cạ sau đó lại ăn 1 cạ khác
	ANBAO_REASON_XECA_ANCA,           // xé cạ ăn cạ
	ANBAO_REASON_XECHAN_ANCA,        // xé chắn, ăn chắn
	ANBAO_REASON_DANH_ROILAI_AN,
	ANBAO_REASON_DANH_DI_DOI_CHAN,
	ANBAO_REASON_AN_ROILAI_DANH,
	ANBAO_REASON_ANCA_ROILAI_DANH_QUAN_CUNG_HANG,
	ANBAO_REASON_CHIUDUOC_NHUNG_LAI_ANTHUONG,
	ANBAO_REASON_AN_CHON_CA,
	ANBAO_REASON_CO_CHAN_CAU_CA,     // có chắn cấu cạ: Lấy 1 quân trong chắn sẵn có để ăn cạ.
	ANBAO_REASON_U_NHUNG_KHONG_XUONG,  // Có thể Ù nhưng lại quên, hoặc ko xướng -> ngồi im không được xướng Ù nữa.
	ANBAO_REASON_HO_U_LAO,           // chưa Ù đã hô Ù - bị đền làng
	ANBAO_REASON_XUONG_SAI_CUOC,
    ANBAO_REASON_U_BAO = 26,
    ANBAO_REASON_BO_U_ROI_LAI_U = 27,
    ANBAO_REASON_ANCA_ROILAI_AN_QUAN_CUNG_HANG = 28
};

// CardSuite Defs
enum
{
	CARDSUIT_VANV = 0,          // vạn
	CARDSUIT_VANC,          // văn
	CARDSUIT_SACH,        // sách
	CARDSUIT_CHI,        // chichi
};

// end game Reson
#define GAME_ERS_UNKOWN -1
#define GAME_ERS_HOA_CA_LANG 0
#define GAME_ERS_HAS_WINNER 1
#define GAME_ERS_HAS_ONE_DEN_LANG 2  // xướng sai cước, hoặc ko chịu xướng cước
#define GAME_ERS_HAS_TREOTRANH 3 // kết thúc game có người bị treo tranh, vẫn đc hô thông
#define GAME_ERS_HAS_TREOTRANH_PHAT_8DO2LEO 4  // Kết thúc game do có người Ù, nhưng do ăn treo tranh và ko được hô thông
#define GAME_ERS_KHONG_XUONG_CUOC 5 // kết thúc game, do có người báo Ù nhưng ko xướng cước (timeout)
#define GAME_ERS_VI_PHAM_CHOI_DIEM_TOI_THIEU 6   // Ù nhưng vi phạm luật chơi điểm tối thiểu
#define GAME_ERS_HAS_ONE_BO_CUOC 7

// Cac cuoc U
#define U_XUONG  0
#define U_THONG 1
#define U_CHI 2
#define U_THIEN_U 3
#define U_DIA_U 4
#define U_TOM 5
#define U_LEO 6
#define U_BACH_DINH 7
#define U_TAM_DO 8
#define U_KINH_TU_CHI 9
#define U_THAP_THANH 10
#define U_CO_THIEN_KHAI 11
#define U_AN_BON 12
#define U_BON 13
#define U_CO_CHIU 14
#define U_CHIU_U 15
#define U_BACH_THU 16
#define U_HOA_ROI_CUA_PHAT 17
#define U_NLXH_HRCP 18
#define U_CA_LOI_SAN_DINH 19
#define U_CA_NHAY_DAU_THUYEN 20
#define U_CHUA_DO_NAT_HOA 21
#define U_DOI_LEO 22
#define U_DOI_TAM_DO 23
#define U_DOI_TOM 24
#define U_BACH_THU_CHI 25
#define U_AN_2_BON 26      // 2 Bòn
#define U_AN_3_BON 27      // 3 Bòn
#define U_AN_4_BON 28      // 4 Bòn
#define U_TOM_3 29         // 3 Tôm
#define U_TOM_4 30         // 4 Tôm
#define U_CO_2_CHIU 31     // 2 chíu
#define U_CO_3_CHIU 32     // 3 chíu
#define U_CO_4_CHIU 33     // 4 chíu
#define U_CO_2_THIEN_KHAI 34   // 2 thiên khai
#define U_CO_3_THIEN_KHAI 35   // 3 thiên khai
#define U_CO_4_THIEN_KHAI 36   // 4 thiên khai
#define U_LEO_3 37             // 3 lèo
#define U_LEO_4 38             //
#define TOTAL_CUOC_U U_LEO_4 + 1

// Cac cuoc co ga
#define GA_U_BON_BACH_THU 0
#define GA_U_BON_BACH_THU_CHI 1
#define GA_THAP_THANH 2
#define GA_KINH_TU_CHI 3
#define GA_BACH_DINH 4
#define GA_TAM_DO 5
#define GA_BACH_THU_CHI 6
#define GA_HOA_ROI_CUA_PHAT 7
#define GA_CA_LOI_SAN_DINH 8
#define GA_NLXH_HRCP 9
#define GA_CHI_BACH_THU 10
#define GA_CHI_BACH_THU_CHI 11
#define GA_NGU_ONG_BAT_CA 12

// CardOriginationDefs
enum
{
	CARD_ORIGINATION_UNKOWN = -1,
	CARD_ORIGINATION_CHIA_BAI = 0,
	CARD_ORIGINATION_BOC_NOC, //bốc nọc
	CARD_ORIGINATION_AN_CUA_TREN,   // ăn thông thường
	CARD_ORIGINATION_AN_CUA_TRI, // ăn cửa chì
	CARD_ORIGINATION_CHIU, // ăn chíu
	CARD_ORIGINATION_TRA_CUA,       // sau khi chiu phai đánh trả cửa
	CARD_ORIGINATION_BY_DISCARD,    // discard
	CARD_ORIGINATION_BY_DUOI,       // đánh đi cây bài vừa bốc nọc ở cửa trì
	CARD_ORIGINATION_BY_AN_DUOI,
	CARD_ORIGINATION_BY_TRANSFER_TREN_2_DUOI,
	CARD_ORIGINATION_AN_BAI_U,
};

// Common rescode
#define GAME_RSCODE_OK 0                 // OK, success
#define GAME_RSCODE_FAILED GAME_RSCODE_OK - 1       // FAILED
#define GAME_RSCODE_ERROR_ROOM_FULL GAME_RSCODE_OK + 1  // ROOM FULL
#define GAME_RSCODE_ERROR_GAME_FULL GAME_RSCODE_OK + 2  // GAME FULL
#define GAME_RSCODE_ERROR_CARD_NOT_IN_ON_HAND_LIST GAME_RSCODE_OK + 3  // card không nằm trong trên tay list
#define GAME_RSCODE_ERROR_PLAYER_IN_END_STATE GAME_RSCODE_OK + 5     // player đã ở trạng thái kết thúc
#define GAME_RSCODE_INVALIDE_STATE GAME_RSCODE_OK + 14          // action của player không được phép tại thời điểm đó
#define GAME_RSCODE_INVALID_TURN GAME_RSCODE_OK + 17           // player đánh bài không phải lượt (ko cho phép)
#define GAME_RSCODE_ERROR_NO_MORE_CARD GAME_RSCODE_OK + 20        // không còn bài trên nọc
#define GAME_RSCODE_ERROR_PLAYER_NOT_IN_END_STATE GAME_RSCODE_OK + 21  // player ko ở trạng thái kết thúc (đã oánh 4 cây đi)
#define GAME_RSCODE_ERROR_PLAYER_ENDED_STATE_BEFORE GAME_RSCODE_OK + 22 // player đã kết thúc chơi trước đây
#define GAME_RSCODE_ERROR_PLAYER_ISNOT_ROOM_MASTER GAME_RSCODE_OK + 23  // player không phải chủ bàn
#define GAME_RSCODE_BETVAL_TOO_BIG GAME_RSCODE_OK + 24          // giá trị bet quá lớn
#define GAME_RSCODE_HAVE_JOINED_BEFRORE GAME_RSCODE_OK + 25       // đã join (room or game) trước đây
#define GAME_RSCODE_HAVENT_JOINED GAME_RSCODE_OK + 26          // chưa join (room or game) trước đây
#define GAME_RSCODE_NOT_CONNECTED_TO_AMSERVICE GAME_RSCODE_OK + 27          // billing man not connect to am service yet
#define GAME_RSCODE_UNDER_MINIMUM_BALANCE GAME_RSCODE_OK + 28          // không đủ tiền để chơi
#define GAME_RSCODE_OWN_CARD_SIZE_NOT_20 GAME_RSCODE_OK + 29          // không đủ quân bài để cho phép đánh
#define GAME_RSCODE_OWN_CARD_SIZE_NOT_19 GAME_RSCODE_OK + 30          // không đủ dk để bốc bài hoặc chíu bài...
#define GAME_RSCODE_OWN_CARD_NOT_EXISTED GAME_RSCODE_OK + 31   // không phải bài trên tay
#define GAME_RSCODE_NOT_CHAN_CA_SET GAME_RSCODE_OK + 32     // không phải là 1 cặp chắn hoặc cạ
#define GAME_RSCODE_IN_CURRENT_GAME_TURN GAME_RSCODE_OK + 33
#define GAME_RSCODE_COULD_NOT_CHIU GAME_RSCODE_OK + 34      // khong đủ điều kiện để chíu
#define GAME_RSCODE_COULD_NOT_SELF_CHIU GAME_RSCODE_OK + 35   // trường hợp đánh bài, hoặc dưới bài đi rồi chíu lại (ko đc phép)
#define GAME_RSCODE_NOT_ENOUGH_CARD_TO_CHIU GAME_RSCODE_OK + 36 // ko đủ bài để chíu
#define GAME_RSCODE_INVALID_CUOC_SAC GAME_RSCODE_OK + 37     // Xướng Ù với các cước sắc không đúng (xướng sai)
#define GAME_RSCODE_GAME_IN_CHIU GAME_RSCODE_OK + 38       // Game đang có người chíu bài (những người khác ko phải đợi chíu xong rồi làm j mới đc làm)
#define GAME_RSCODE_GAME_IN_U_ANNOUNCING GAME_RSCODE_OK + 39   // Game đnag có người xướng Ù (những người khác phải đợi xướng xong)
#define GAME_RSCODE_COULDNOT_ANNOUCING_U GAME_RSCODE_OK + 40   // Player khong được phép xướng Ù, vì chưa bài chưa Ù
#define GAME_RSCODE_WAIT_FOR_ONE_ANBAI_U GAME_RSCODE_OK + 41   // Có 1 người đang đợi ăn cây bài này để Ù.
#define GAME_RSCODE_ONLY_DRAW_N_DUOI GAME_RSCODE_OK + 42     // Chỉ có thể bốc và dưới bài (do đã bỏ lỡ ko Ù, hoặc ăn báo)
#define GAME_RSCODE_PLAYER_IGNORE_U_BACH_THU GAME_RSCODE_OK + 43   // người chơi đã quên không xướng Ù Bạch Thủ
#define GAME_RSCODE_CANNOT_U_BECAUSE_ANBAO GAME_RSCODE_OK + 44    // không thể Ù vì nếu Ù sẽ bị bắt báo
#define GAME_RSCODE_PLAYER_IGNORE_THIEN_U GAME_RSCODE_OK + 45  // người chơi quên không xướng cước Thiên Ù
#define GAME_RSCODE_PLAYER_IGNORE_FORCING_CUOC GAME_RSCODE_OK + 46  // play xướng thiếu các cước bắt buộc phải xướng
#define GAME_RSCODE_U_BAO GAME_RSCODE_OK + 47            // Ù báo: Hô ù khi đã bị báo
#define GAME_RSCODE_U_BUT_LOW_PRIORITY GAME_RSCODE_OK + 48    // người Ù trước có độ ưu tiên cao hơn
#define GAME_RSCODE_USER_NOT_READY GAME_RSCODE_OK + 49      // hiện tại user chưa ready
#define GAME_RSCODE_COULDNOT_U_OVER_U_INTERVAL GAME_RSCODE_OK + 51
// CoinChange Reson
#define CCR_BY_WIN 0                     // thắng trong ván bài, ko bị bắt lỗi
#define CCR_BY_WIN_TREO_TRANH 1          // thắng trong ván bài nhưng bị lỗi treo tranh nhẹ
#define CCR_BY_DEN_LANG_XUONG_SAI 2      // đền làng do xướng sai cước
#define CCR_BY_DEN_LANG_BI_BAO 3         // đền làng do bị báo, thay làng trả tiền cho người Ù
#define CCR_BY_KO_BI_TRU_TIEN 4          // không bị trừ hoặc thêm tiền
#define CCR_BY_DEN_LANG_TREO_TRANH 5         // đền làng, do lỗi bắt báo khi kết thúc ván chơi ( ràng buộc với bài trên tay)
#define CCR_BY_LOSE 6                    // thua ván
#define CCR_BY_ONE_DEN_LANG 7                    // được chia tiền do người Ù bị đền làng
#define CCR_BY_DEN_LANG_U_KHONG_XUONG_CUOC 8     // đền làng do Ù nhưng ko xướng cước.
#define CCR_BY_GOP_GA 9
#define CCR_BY_NGUOI_KHAC_CHIU 10
#define CCR_BY_NHAI 11
#define CCR_BY_VI_PHAM_CHOI_DIEM_TOI_THIEU 12c

// Cac reson kickout
#define KICK_OUT_BY_UNDER_MINIMUM_BALANCE 0      // ko đủ tiền chơi tiếp
#define KICK_OUT_BY_READY_TIMEOUT 1              // timeout do ko bấm sẵn sàng chơi
#define KICK_OUT_DISCONECT_BY_CLIENT 2           // client mất kết nối với server
#define KICK_OUT_BY_MASTER 3                     // bởi chủ phòng


// Định nghĩa thêm các subreson
#define SUBREASON_NO_PROBLEM 0
#define SUBREASON_BO_CARD_CUA_TRI 1        // bỏ không ăn card tại cửa trì
#define SUBREASON_BO_CARD_CUA_TREN 2         // bỏ không ăn card tại cửa trên
#define SUBREASON_BO_CARD_DA_AN_LAM_CA 3     // card này đã ăn làm cạ trước đó

#endif
