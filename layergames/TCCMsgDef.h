#ifndef TCCMsgDef_h__
#define TCCMsgDef_h__

#include "PhomMessDef.h"

/*
* Tom cua ca mini server message define
* */
#define EXT_EVENT_TCC_START  "tccs"
#define EXT_EVENT_TCC_END  "tcce"
#define EXT_EVENT_TCC_JOIN  "tccj"
#define EXT_EVENT_TCC_SWITCH_BET_ROOM_REQ  "tccsbr"
#define EXT_EVENT_TCC_LEAVE  "tccl"
#define EXT_EVENT_TCC_GAME_RESULT  "tccgrs"
#define EXT_EVENT_TCC_GAME_BET_REQ  "tccgbr"
#define EXT_EVENT_TCC_GAME_BET_ERROR_NOTIFY  "tcce_bntf"
#define EXT_EVENT_TCC_GAME_BET_NTF  "tccgb_ntf"
#define EXT_EVENT_TCC_GAME_INFO_NTF  "tccgi_ntf"
#define EXT_EVENT_TCC_GET_GAME_INFO_REQ  "tccggir"
#define EXT_EVENT_TCC_GAME_STATE_NTF  "tccgs_ntf"
#define EXT_EVENT_TCC_GAME_HIS_NTF  "tccgh_ntf"
#define EXT_EVENT_TCC_NOT_ENOUGH_MONEY_NTF  "tccnem_ntf"
#define EXT_EVENT_TCC_USER_HIS_REQ  "tccuhrq"
#define EXT_EVENT_TCC_USER_HIS_RESP "tccuhresp"
#define EXT_EVENT_TCC_GAME_HIS_DETAIL_REQ  "tccghdrq"
#define EXT_EVENT_TCC_GAME_HIS_DETAIL_RESP "tccghdresp"
#define EXT_EVENT_TCC_GAME_HIS_REQ  "tccghrq"
#define EXT_EVENT_TCC_GAME_HIS_RESP  "tccghresp"
#define EXT_EVENT_TCC_RANK_REQ  "tccrrq"
#define EXT_EVENT_TCC_RANK_RESP "tccrresp"
#define EXT_EVENT_TCC_GET_QUY_THUONG_REQ  "tccgqtr"
#define EXT_EVENT_TCC_GET_QUY_THUONG_RESP  "tccgqtresp"
#define EXT_EVENT_TCC_OPEN_QUY_THUONG_NTF "tccoqt_ntf"
#define EXT_EVENT_TCC_OPEN_QUY_THUONG_REQ  "tccoqtreq"
//event field
#define EXT_FIELD_ERROR_CODE  "errc"
#define EXT_FIELD_MATCH_ID  "mid"
#define EXT_FIELD_PLAY_TIME  "pt"
#define EXT_FIELD_END_TIME  "et"
#define EXT_FIELD_RESULT_TOP_10  "rt10"
#define EXT_FIELD_GAME_BET_VALUE  "gbv"
#define EXT_FIELD_GAME_WIN_VALUE  "gwv"
#define EXT_FIELD_GAME_RETURN_VALUE  "grv"
#define EXT_FIELD_GAME_BET_ROOM "gbr"
#define EXT_FIELD_GAME_IS_BOT  "ibt"
#define EXT_FIELD_GAME_TOTAL_BET_VALUE  "gtbv"
#define EXT_FIELD_GAME_TOTAL_PLAYER = "ttp"
#define EXT_FIELD_GAME_TOTAL_BET_VALUE_CELL  "gtbvc"
#define EXT_FIELD_GAME_TOTAL_USER_BET_CELL  "gtbuc"
#define EXT_FIELD_GAME_INFO  "ginfo"
#define EXT_FIELD_GAME_HISTORY_RESULT  "ghs"
#define EXT_FIELD_GAME_HISTORY_TOP_PLAYER  "ghtp"
#define EXT_FIELD_GAME_TIME  "gtime"
#define EXT_FIELD_GAME_PLAY_STATE  "gstate"
#define EXT_FIELD_GAME_BET_EXCHANGE  "gbex"
#define EXT_FIELD_GAME_BET_CAN_EAT  "gbce"
#define EXT_FIELD_GAME_IS_AUTO  "giau"
#define EXT_FIELD_GAME_LIST_CARD  "glstc"
#define EXT_FIELD_GAME_LIST_BET  "glb"
#define EXT_FIELD_GAME_TIME_OPEN_EVENT  "toet"
#define EXT_FIELD_GAME_PERCENT  "gper"
#define EXT_FIELD_GAME_HIS_QUY_THUONG  "ghqt"

#define RESCODE_NOT_ENOUGH_MONEY  29
#define RESCODE_GAME_IS_ENDED  32


static string formatMoneySymbolTCC(double money){
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
			{
				if (val - (int)val > 0.0)//have decimal point
					return StringUtils::format("%.1f", val) + arrSi[i].symbolStruct;
				else
					return StringUtils::format("%.f", val) + arrSi[i].symbolStruct;
			}

			return StringUtils::format("%.2f", val) + " " + arrSi[i].symbolStruct;
		}
	}
	return boost::to_string(money);
}

static string formatMoneySymbolTCC(string _money){
	double money = atol(_money.c_str());
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
			{
				if (val - (int)val > 0.0)//have decimal point
					return StringUtils::format("%.1f", val) + arrSi[i].symbolStruct;
				else
					return StringUtils::format("%.f", val) + arrSi[i].symbolStruct;
			}

			return StringUtils::format("%.2f", val) + " " + arrSi[i].symbolStruct;
		}
	}
	return boost::to_string(money);
}

static int getIdByBetType(string _name)
{
	if (0 == _name.compare("CA"))
		return 0;
	else if (0 == _name.compare("GA"))
		return 1;
	else if (0 == _name.compare("CUA"))
		return 2;
	else if (0 == _name.compare("HUOU"))
		return 3;
	else if (0 == _name.compare("BAU"))
		return 4;
	else if (0 == _name.compare("TOM"))
		return 5;
	else
		return -1;
}

static string getBetTypeById(int _id){
	switch (_id)
	{
	case 0:
		return "CA";
	case 1:
		return "GA";
	case 2:
		return "CUA";
	case 3:
		return "HUOU";
	case 4:
		return "BAU";
	case 5:
		return "TOM";
	default:
		break;
	}
}

static string getImgDiceById(int _id)
{
	switch (_id)
	{
	case 0:
	case 6:
		return "ca.png";
	case 1:
		return "ga.png";
	case 2:
		return "cua.png";
	case 3:
		return "ho.png";
	case 4:
		return "bau.png";
	case 5:
		return "tom.png";
	}
}

static string getImgDiceX2ById(int _id){
	switch (_id)
	{

	case 0:
		return "ca_x2.png";
	case 1:
		return "ga_x2.png";
	case 2:
		return "cua_x2.png";
	case 3:
		return "ho_x2.png";
	case 4:
		return "bau_x2.png";
	case 5:
		return "tom_x2.png";
	default:
		return "cua_x2.png";
	}
}
//change for game bai2.0
/************************************************************************/
/* cá
sò - gà
cua
sao - hổ
ốc - bầu
tôm
cá - sò - cua - sao - ốc - tôm                                                                     */
/************************************************************************/
static string getNameByBetType(string _name){
	if (0 == _name.compare("CA"))
		return "CÁ";
	else if (0 == _name.compare("GA"))
		return "SÒ";
	else if (0 == _name.compare("CUA"))
		return "CUA";
	else if (0 == _name.compare("HUOU"))
		return "SAO";
	else if (0 == _name.compare("BAU"))
		return "ỐC";
	else if (0 == _name.compare("TOM"))
		return "TÔM";
	else
		return "";
}
#endif // TCCMsgDef_h__
