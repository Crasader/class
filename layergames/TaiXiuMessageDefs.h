
#ifndef _iCasino_TaiXiuMessageDefs_h_
#define _iCasino_TaiXiuMessageDefs_h_

#include "ClientMsgDefs.h"

#define RESCODE_NOT_ENOUGH_MONEY 29
#define  RESCODE_GAME_IS_ENDED  32
#define RESCODE_NOT_BET_MORE_TYPE 34
//****************************BET TYPE*****************************
//BET NUMBER
#define BET_TYPE_NUMBER_4 "BTN4"
#define BET_TYPE_NUMBER_5 "BTN5"
#define BET_TYPE_NUMBER_6 "BTN6"
#define BET_TYPE_NUMBER_7 "BTN7"
#define BET_TYPE_NUMBER_8 "BTN8"
#define BET_TYPE_NUMBER_9 "BTN9"
#define BET_TYPE_NUMBER_10 "BTN10"
#define BET_TYPE_NUMBER_11 "BTN11"
#define BET_TYPE_NUMBER_12 "BTN12"
#define BET_TYPE_NUMBER_13 "BTN13"
#define BET_TYPE_NUMBER_14 "BTN14"
#define BET_TYPE_NUMBER_15 "BTN15"
#define BET_TYPE_NUMBER_16 "BTN16"
#define BET_TYPE_NUMBER_17 "BTN17"
//BET TAI XIU
#define BET_TYPE_XIU "XIU"
#define BET_TYPE_TAI "TAI"
//BET DOI
#define BET_TYPE_DOI_1 "BTD1"
#define BET_TYPE_DOI_2 "BTD2"
#define BET_TYPE_DOI_3 "BTD3"
#define BET_TYPE_DOI_4 "BTD4"
#define BET_TYPE_DOI_5 "BTD5"
#define BET_TYPE_DOI_6 "BTD6"
//BET BA
#define BET_TYPE_BA_1 "BTB1"
#define BET_TYPE_BA_2 "BTB2"
#define BET_TYPE_BA_3 "BTB3"
#define BET_TYPE_BA_4 "BTB4"
#define BET_TYPE_BA_5 "BTB5"
#define BET_TYPE_BA_6 "BTB6"

#define BET_TYPE_ANY_BA "BTAB"

//BET CAP
#define BET_TYPE_CAP_1_2 "BTC12"
#define BET_TYPE_CAP_1_3 "BTC13"
#define BET_TYPE_CAP_1_4 "BTC14"
#define BET_TYPE_CAP_1_5 "BTC15"
#define BET_TYPE_CAP_1_6 "BTC16"
#define BET_TYPE_CAP_2_3 "BTC23"
#define BET_TYPE_CAP_2_4 "BTC24"
#define BET_TYPE_CAP_2_5 "BTC25"
#define BET_TYPE_CAP_2_6 "BTC26"
#define BET_TYPE_CAP_3_4 "BTC34"
#define BET_TYPE_CAP_3_5 "BTC35"
#define BET_TYPE_CAP_3_6 "BTC36"
#define BET_TYPE_CAP_4_5 "BTC45"
#define BET_TYPE_CAP_4_6 "BTC46"
#define BET_TYPE_CAP_5_6 "BTC56"

#define BET_TYPE_DON_LE_1 "BDL1"
#define BET_TYPE_DON_LE_2 "BDL2"
#define BET_TYPE_DON_LE_3 "BDL3"
#define BET_TYPE_DON_LE_4 "BDL4"
#define BET_TYPE_DON_LE_5 "BDL5"
#define BET_TYPE_DON_LE_6 "BDL6"

//****************************BET TYPE*****************************
#define BET_VALUE_1    "1"
#define BET_VALUE_100    "100"
#define BET_VALUE_500    "500"
#define BET_VALUE_1K    "1K"
#define BET_VALUE_2K    "2K"
#define BET_VALUE_5K    "5K"
#define BET_VALUE_10K   "10K"
#define BET_VALUE_50K   "50K"
#define BET_VALUE_100K  "100K"
#define BET_VALUE_500K  "500K"
#define BET_VALUE_1M    "1M"
#define BET_VALUE_5M    "5M"

/**
 * Tag
 */
#define tag_BET_TYPE_NUMBER_4 104
#define tag_BET_TYPE_NUMBER_5 105
#define tag_BET_TYPE_NUMBER_6 106
#define tag_BET_TYPE_NUMBER_7 107
#define tag_BET_TYPE_NUMBER_8 108
#define tag_BET_TYPE_NUMBER_9 109
#define tag_BET_TYPE_NUMBER_10 110
#define tag_BET_TYPE_NUMBER_11 111
#define tag_BET_TYPE_NUMBER_12 112
#define tag_BET_TYPE_NUMBER_13 113
#define tag_BET_TYPE_NUMBER_14 114
#define tag_BET_TYPE_NUMBER_15 115
#define tag_BET_TYPE_NUMBER_16 116
#define tag_BET_TYPE_NUMBER_17 117

//BET TAI XIU
#define tag_BET_TYPE_XIU 101
#define tag_BET_TYPE_TAI 102

//BET DOI
#define tag_BET_TYPE_DOI_1 118
#define tag_BET_TYPE_DOI_2 119
#define tag_BET_TYPE_DOI_3 120
#define tag_BET_TYPE_DOI_4 121
#define tag_BET_TYPE_DOI_5 122
#define tag_BET_TYPE_DOI_6 123

//BET BA
#define tag_BET_TYPE_BA_1 124
#define tag_BET_TYPE_BA_2 125
#define tag_BET_TYPE_BA_3 126
#define tag_BET_TYPE_BA_4 127
#define tag_BET_TYPE_BA_5 128
#define tag_BET_TYPE_BA_6 129
//BET CAP
#define tag_BET_TYPE_CAP_1_2 130
#define tag_BET_TYPE_CAP_1_3 131
#define tag_BET_TYPE_CAP_1_4 132
#define tag_BET_TYPE_CAP_1_5 133
#define tag_BET_TYPE_CAP_1_6 134
#define tag_BET_TYPE_CAP_2_3 135
#define tag_BET_TYPE_CAP_2_4 136
#define tag_BET_TYPE_CAP_2_5 137
#define tag_BET_TYPE_CAP_2_6 138
#define tag_BET_TYPE_CAP_3_4 139
#define tag_BET_TYPE_CAP_3_5 140
#define tag_BET_TYPE_CAP_3_6 141
#define tag_BET_TYPE_CAP_4_5 142
#define tag_BET_TYPE_CAP_4_6 143
#define tag_BET_TYPE_CAP_5_6 144
#define tag_BET_TYPE_ANY_BA 145

#define tag_BET_TYPE_DON_LE_1 146
#define tag_BET_TYPE_DON_LE_2 147
#define tag_BET_TYPE_DON_LE_3 148
#define tag_BET_TYPE_DON_LE_4 149
#define tag_BET_TYPE_DON_LE_5 150
#define tag_BET_TYPE_DON_LE_6 151

#define tag_BET_VALUE_1K    153
#define tag_BET_VALUE_2K    152
#define tag_BET_VALUE_5K    154
#define tag_BET_VALUE_10K   155
#define tag_BET_VALUE_50K   156
#define tag_BET_VALUE_100K  157
#define tag_BET_VALUE_500K  158
#define tag_BET_VALUE_1M    159
#define tag_BET_VALUE_5M    160

//
#define VALUE_TAIXIU 1
#define VALUE_BODOI 2
#define VALUE_BOBA 3
#define VALUE_ANYBA 4
#define VALUE_CAPDOI 5
#define VALUE_DIEM 6
#define VALUE_DONLE 7


//For Xoc Dia
#define  BET_TYPE_CHAN  "CHAN"
#define  BET_TYPE_LE  "LE"
#define  BET_TYPE_CAP_0_4  "BTC04"
#define  BET_TYPE_CAP_1_3  "BTC13"
#define  BET_TYPE_CAP_3_1  "BTC31"
#define  BET_TYPE_CAP_4_0  "BTC40"
#define  tag_XD_BET_TYPE_CHAN  501
#define  tag_XD_BET_TYPE_LE  502
#define  tag_XD_BET_TYPE_CAP_0_4  503
#define  tag_XD_BET_TYPE_CAP_1_3  504
#define  tag_XD_BET_TYPE_CAP_3_1  505
#define  tag_XD_BET_TYPE_CAP_4_0  506
#define  XOC_DIA_CHAN  0
#define  XOC_DIA_LE  1
#define  SCALE_COIN_TAI_XIU  0.50
#define TAG_TAI_XIU_HISTORY  708
#define TAG_TAI_XIU_TOPWINS  709
#define DURATION_ANIMATE_COIN  0.4
#define DURATION_ANIMATE_COIN_CLICK  0.09
#define TAG_TAI_XIU_XO_BA0  706
#define TAG_ANIM_END_GAME  707
#define TAG_ANIM_START_GAME  720
#define TAG_TOAST  721
#endif
