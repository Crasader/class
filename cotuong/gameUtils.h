#ifndef gameUtils_h__
#define gameUtils_h__

#include "cocos2d.h"
#include <boost/algorithm/string/trim.hpp>
#include "ServerMessageDef.h"
#include "../GameServer.h"
#include "mUtils.h"
#include "CommonChess.h"
#include "layergames/PhomMessDef.h"

using namespace std;
using namespace cocos2d;

class GameUtils{
private:
	enum CONFIG_ID{
		TOT = 0,
		SI,
		TUONGJ,
		MA,
		PHAO,
		XE
	};

public:
	static std::string getNamePlayer(const string& uid) {
		boost::shared_ptr<User> user = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(uid);
		if (user == NULL)
			return uid;
		boost::shared_ptr<UserVariable> usrVar = user->GetVariable("adn");
		if (usrVar == NULL)
			return uid;
		boost::shared_ptr<string> name = usrVar->GetStringValue();
		string username = "";
		if (name)
		{
			username = *name;
			boost::algorithm::trim(username);
			if (0 == username.compare(""))
				return uid;
		}
		else{
			return uid;
		}
		return username;
	}

	static std::string getMyName()
	{
		if (GameServer::getSingleton().getSmartFox()->MySelf() == NULL)
			return "";
		return (GameServer::getSingleton().getSmartFox()->MySelf()->Name())->c_str();
	}

	static double getAmfGoldUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> amfPtr = mySelf->GetVariable(EXT_FIELD_USER_AMF_GOLD);
		if (amfPtr == NULL)
			return 0;
		double amf = *amfPtr->GetDoubleValue();
		return amf;
	}

	static double getAmfGoldUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> amfPtr = mySelf->GetVariable(EXT_FIELD_USER_AMF_GOLD);
		if (amfPtr == NULL)
			return 0;
		double amf = *amfPtr->GetDoubleValue();
		return amf;
	}

	static double getAmfSilverUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> amfPtr = mySelf->GetVariable(EXT_FIELD_USER_AMF_SILVER);
		if (amfPtr == NULL)
			return 0;
		double amf = *amfPtr->GetDoubleValue();
		return amf;
	}

	static double getAmUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> amfPtr = mySelf->GetVariable(EXT_FIELD_USER_AM);
		if (amfPtr == NULL)
			return 0;
		double amf = *amfPtr->GetDoubleValue();
		return amf;
	}

	static int getLevelUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> lvlPtr = mySelf->GetVariable(EXT_FIELD_USER_LEVEL);
		if (lvlPtr == NULL)
			return 0;
		int level = *lvlPtr->GetIntValue();
		return level;
	}

	static int getLevelUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> lvlPtr = mySelf->GetVariable(EXT_FIELD_USER_LEVEL);
		if (lvlPtr == NULL)
			return 0;
		int level = *lvlPtr->GetIntValue();
		return level;
	}

	static int getExpUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> expPtr = mySelf->GetVariable(EXT_FIELD_USER_EXP);
		if (expPtr == NULL)
			return 0;
		int expUser = *expPtr->GetIntValue();
		return expUser;
	}

	static string getAvatarUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return "";
		boost::shared_ptr<UserVariable> urlPtr = mySelf->GetVariable(EXT_FIELD_USER_AVATAR);
		if (urlPtr == NULL)
			return "";
		string urlUser = *urlPtr->GetStringValue();
		if (0 == urlUser.compare(""))
			return "";
		return urlUser;
	}

	static string getAvatarUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return "";
		boost::shared_ptr<UserVariable> urlPtr = mySelf->GetVariable(EXT_FIELD_USER_AVATAR);
		if (urlPtr == NULL)
			return "";
		string urlUser = *urlPtr->GetStringValue();
		if (0 == urlUser.compare(""))
			return "";
		return urlUser;
	}

	static int getTypeVipUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> vipPtr = mySelf->GetVariable(EXT_FIELD_USER_VIP_TYPE);
		if (vipPtr == NULL)
			return 0;
		int viptype = *vipPtr->GetIntValue();
		return viptype;
	}

	static int getTypeVipUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> vipPtr = mySelf->GetVariable(EXT_FIELD_USER_VIP_TYPE);
		if (vipPtr == NULL)
			return 0;
		int viptype = *vipPtr->GetIntValue();
		return viptype;
	}

	static string getVipImage(int _vipType){
		log("VIP TYPE %d", _vipType);
		if (_vipType == 0)
			return "input_transparent.png";
		string imgPath = StringUtils::format("vip%d.png", _vipType);

		return imgPath;
	}

	static int getIDHeroUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return -1;
		boost::shared_ptr<UserVariable> heroPtr = mySelf->GetVariable(EXT_FIELD_HERO_ID);
		if (heroPtr == NULL)
			return -1;
		int idHero = *heroPtr->GetIntValue();
		return idHero;
	}

	static int getIDHeroUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return -1;
		boost::shared_ptr<UserVariable> heroPtr = mySelf->GetVariable(EXT_FIELD_HERO_ID);
		if (heroPtr == NULL)
			return -1;
		int idHero = *heroPtr->GetIntValue();
		return idHero;
	}

	static int getIDChessUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return -1;
		boost::shared_ptr<UserVariable> chessPtr = mySelf->GetVariable(EXT_FIELD_CHESS_ID);
		if (chessPtr == NULL)
			return -1;
		int idChess = *chessPtr->GetIntValue();
		return idChess;
	}

	static int getIDChessUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return -1;
		boost::shared_ptr<UserVariable> chessPtr = mySelf->GetVariable(EXT_FIELD_CHESS_ID);
		if (chessPtr == NULL)
			return -1;
		int idChess = *chessPtr->GetIntValue();
		return idChess;
	}

	static int getEloUser(){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->MySelf();
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> chessPtr = mySelf->GetVariable(EXT_FIELD_ELO_POINT);
		if (chessPtr == NULL)
			return 0;
		int idChess = *chessPtr->GetIntValue();
		return idChess;
	}

	static int getEloUser(string _userName){
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> chessPtr = mySelf->GetVariable(EXT_FIELD_ELO_POINT);
		if (chessPtr == NULL)
			return 0;
		int idChess = *chessPtr->GetIntValue();
		return idChess;
	}

	static string getRoomName(){
		string roomName = "";
		if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
			int id = std::atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
			boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
			if (param != NULL){
				string paramString = param->GetStringValue()->c_str();
				vector<string> arrInfo = mUtils::splitString(paramString, '@');
				boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
				if (lastRoom != NULL){
					//HoangDD Change
					roomName = mUtils::getRoomIDByName(*lastRoom->Name());
				}
			}
		}
		return roomName;
	}

	static double getRoomBet(){
		double money_bet = 0;
		if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() != NULL){
			int id = std::atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0, 3).c_str());
			boost::shared_ptr<RoomVariable> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params");
			if (param != NULL){
				string paramString = param->GetStringValue()->c_str();
				vector<string> arrInfo = mUtils::splitString(paramString, '@');
				string money = arrInfo.at(0);

				money_bet = atol(money.c_str());
			}
		}
		return money_bet;
	}

	static string getRoomName(int _idRoom)
	{
		string _name = "";
		boost::shared_ptr<Room> lastJoinRoom = GameServer::getSingleton().getSmartFox()->RoomManager()->GetRoomById(_idRoom);
		if (lastJoinRoom != NULL)
		{
			boost::shared_ptr<RoomVariable> param = lastJoinRoom->GetVariable("params");
			if (param != NULL){
				string paramString = param->GetStringValue()->c_str();
				vector<string> arrInfo = mUtils::splitString(paramString, '@');
				boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
				if (lastRoom != NULL){
					//HoangDD Change
					_name = mUtils::getRoomIDByName(*lastRoom->Name());
				}
			}
		}

		return _name;
	}

	static bool isBossRoom(string listUser){
		bool isBoss = false;
		string myName = GameUtils::getMyName();
		if (listUser.size() == 0) return isBoss;
		vector<string> arrUser = mUtils::splitString(listUser, ';');
		vector<string> info = mUtils::splitString(arrUser[0], '|');
		if (0 == info.at(0).compare(myName))
			isBoss = true;
		return isBoss;
	}

	static string convertTimer(int time) {
		int minute = time / 60;
		int second = time - minute * 60;

		string mString = boost::to_string(minute);
		if (minute < 10)
			mString = "0" + mString;

		string sString = boost::to_string(second);
		if (second < 10)
			sString = "0" + sString;

		string resutl = mString + ":" + sString;
		return (resutl);
	}

	static bool isStartedGame(){
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (room == NULL)
			return false;
		boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
		if (rv == NULL) return false;
		string s = *rv->GetStringValue();

		vector<string> lstBet = mUtils::splitString(s, '@');
		bool isStarted = false;
		lstBet.at(1).compare("1") == 0 ? (isStarted = true) : (isStarted = false);
		// 		if (isStarted) {
		// 			log("Ban dang choi!");
		// 		}
		// 		else log("Ban chua choi!");

		return isStarted;
	}

	//conver config chap quan
	static string convertConfigToString(string _strConfig)
	{
		string strConfig = "";
		vector<string> arrConfig = mUtils::splitString(_strConfig, ',');
		for (size_t i = 0; i < arrConfig.size(); i++)
		{
			vector<string> arrInfo = mUtils::splitString(arrConfig.at(i), '-');
			if (arrInfo.size() == 2)
			{
				int _id = atoi(arrInfo.at(0).c_str());
				int _num = atoi(arrInfo.at(1).c_str());
				if (_num >0)
				{
					switch (_id)
					{
					case GameUtils::TOT:
						strConfig.append(StringUtils::format("%s Tốt,", arrInfo.at(1).c_str()));
						break;
					case GameUtils::SI:
						strConfig.append(StringUtils::format("%s Sĩ,", arrInfo.at(1).c_str()));
						break;
					case GameUtils::TUONGJ:
						strConfig.append(StringUtils::format("%s Tượng,", arrInfo.at(1).c_str()));
						break;
					case GameUtils::MA:
						strConfig.append(StringUtils::format("%s Mã,", arrInfo.at(1).c_str()));
						break;
					case GameUtils::PHAO:
						strConfig.append(StringUtils::format("%s Pháo,", arrInfo.at(1).c_str()));
						break;
					case GameUtils::XE:
						strConfig.append(StringUtils::format("%s Xe", arrInfo.at(1).c_str()));
						break;
					default:
						break;
					}
				}

			}


		}

		return strConfig;
	}

	static string convertFullConfigToString(string strConfig)
	{
		//200@0@1@tester02@300@90@5-9,4-8,3-8@0@0-0@1@1 - 1 là có, 0 là không
		//các config cần quan tâm: tiền cược(0), tổng thời gian(4), chấp tiên(7), chấp hòa(9), chấp quân(6)
		string _strConfig = "Cài đặt trận đấu đã thay đổi\n";
		vector<string> arrConfig = mUtils::splitString(strConfig, '@');
		if (arrConfig.size() > 11)
		{
			int betvalue = atoi(arrConfig.at(0).c_str());
			int totalTime = atoi(arrConfig.at(4).c_str()) / 60;
			int chaptien = atoi(arrConfig.at(7).c_str());
			int chaphoa = atoi(arrConfig.at(10).c_str());
			string chapQuan = arrConfig.at(6);

			string configGen = "";
			string arrCompare[6] = { "Tốt:", "Sĩ:", "Tượng:", "Mã:", "Pháo:", "Xe:" };
			string arrChessName[6] = { "Tốt:", "Sĩ:", "Tượng:", "Mã:", "Pháo:", "Xe:" };
			for (int i = 0; i < 6; i++)
			{
				vector<string> arrChess = mUtils::splitString(chapQuan, ',');
				for (size_t j = 0; j < arrChess.size(); j++)
				{
					vector<string> arrInfo = mUtils::splitString(arrChess.at(j), '-');
					if (arrInfo.size() == 2)
					{
						int idChess = atoi(arrInfo.at(0).c_str());
						int posChess = atoi(arrInfo.at(1).c_str());
						if (idChess == i && posChess != 0)
						{
							arrChessName[i].append(StringUtils::format("%d-", posChess));
						}
					}
				}
			}
			//re generate
			for (int i = 0; i < 6; i++)
			{
				if (0 != arrChessName[i].compare(arrCompare[i]))
				{
					configGen.append(StringUtils::format("%s,", arrChessName[i].c_str()));
				}
			}
			if (configGen.size() >2)
			{
				configGen.pop_back();
				configGen.pop_back();
			}

			// 			size_t f = _strConfig.find_last_of(',');
			// 			if (f != std::string::npos)
			// 			{
			// 				_strConfig.erase(_strConfig.begin() + (_strConfig.length() - 1));
			// 			}

			string isChapHoa = chaphoa == 1 ? "Có" : "Không";
			_strConfig.append(StringUtils::format(" Mức cược: %s vàng \n- Thời gian: %d phút\n- Chấp tiên: %d nước\n- Chấp Hòa: %s\n - Chấp Quân: %s",
				mUtils::convertMoneyEx(betvalue).c_str(), totalTime, chaptien, isChapHoa.c_str(), configGen.c_str()));
		}
		return _strConfig;
	}

	static string convertPointToRateString(int delayCount){
		if (delayCount >= 0 && delayCount <= 3)
		{
			return "3 Sao";
		}
		else if (delayCount >= 4 && delayCount <= 9){
			return "2 Sao";
		}
		else if (delayCount > 9){
			return "1 Sao";
		}
	}

	static int convertPointToRateInt(int delayCount)
	{
		if (delayCount >= 0 && delayCount <= 3)
		{
			return 3;
		}
		else if (delayCount >= 4 && delayCount <= 9){
			return 2;
		}
		else if (delayCount > 9){
			return 1;
		}
	}

	static string getChessTextPathByType(int _type)
	{
		string typeString = "";
		switch ((CHESS_TYPE)_type)
		{
		case SIMPLE:
			typeString = "text-cothuong.png";
			break;
		case STONE:
			typeString = "text-coda.png";
			break;
		case WOODEN:
			typeString = "text-cogo.png";
			break;
		case COPPER:
			typeString = "text-codong.png";
			break;
		case IRON:
			typeString = "text-cosat.png";
			break;
		case SILVER:
			typeString = "text-cobac.png";
			break;
		case GOLD:
			typeString = "text-covang.png";
			break;
		case GLASS:
			typeString = "text-cothuytinh.png";
			break;
		case FIRE:
			typeString = "text-covanlua.png";
			break;
		default:
			typeString = "text-cothuong.png";
			break;
		}

		return typeString;
	}

	static string getHeroTextPathByType(int _type)
	{
		string typeString = "";
		switch (_type)
		{
		case 1:// thanh giong
			typeString = "text-thanhgiong.png";
			break;
		case 2:
			typeString = "text-sontinh.png";
			break;
		case 3:
			typeString = "text-thuytinh.png";
			break;
		case 4:
			typeString = "text-auco.png";
			break;
		case 5:
			typeString = "text-laclongquan.png";
			break;
		case 6:
			typeString = "text-anduongvuong.png";
			break;
		case 7:
			typeString = "text-haibatrung.png";
			break;
		case 8:
			typeString = "text-quantrung.png";
			break;
		case 9:
			typeString = "text-tranhungdao.png";
			break;
		case 10:
			typeString = "text-ngoquyen.png";
			break;
		case 11:
			typeString = "text-lythuongkiet.png";
			break;
		case 12:
			typeString = "text-dinhbolinh.png";
			break;
		}

		return typeString;
	}

	static string getHeroCheckPathByType(int _type)
	{
		string typeString = "";
		switch (_type)
		{
		case 1:// thanh giong
			typeString = "chieutuong-thanhgiong.png";
			break;
			// 		case 2:
			// 			typeString = "text-sontinh.png";
			// 			break;
		case 3:
			typeString = "chieutuong-thuytinh.png";
			break;
			// 		case 4:
			// 			typeString = "text-auco.png";
			// 			break;
			// 		case 5:
			// 			typeString = "text-laclongquan.png";
			// 			break;
		case 6:
			typeString = "chieutuong-anduongvuong.png";
			break;
		case 7:
			typeString = "chieutuong-haibatrung.png";
			break;
			// 		case 8:
			// 			typeString = "text-quantrung.png";
			// 			break;
			// 		case 9:
			// 			typeString = "text-tranhungdao.png";
			// 			break;
			// 		case 10:
			// 			typeString = "text-ngoquyen.png";
			// 			break;
			// 		case 11:
			// 			typeString = "text-lythuongkiet.png";
			// 			break;
			// 		case 12:
			// 			typeString = "text-dinhbolinh.png";
			// 			break;
		}

		return typeString;
	}

	static string getChessImagePathByType(int _type)
	{
		string sideString;
		string nameString;
		string typeString = "";
		switch ((CHESS_TYPE)_type)
		{
		case SIMPLE:
			break;
		case STONE:
			typeString = "Coda-";
			break;
		case WOODEN:
			typeString = "Cogo-";
			break;
		case COPPER:
			typeString = "Copper-";
			break;
		case IRON:
			typeString = "Cosat-";
			break;
		case SILVER:
			typeString = "Silver-";
			break;
		case GOLD:
			typeString = "Gold-";
			break;
		case GLASS:
			typeString = "Thuytinh-";
			break;
		case FIRE:
			typeString = "Fire-";
			break;
		default:
			typeString = "";
			break;
		}

		sideString = "Black-";
		nameString = "TUONG";
		return typeString + sideString + nameString + ".png";
	}

	static string getHeroInfoByID(int _id)
	{
		switch (_id)
		{
		case 1:// thanh giong
			return dataManager.GetSysString(3737);
		case 3:
			return dataManager.GetSysString(3739);
		case 6:
			return dataManager.GetSysString(3742);
		case 7:
			return dataManager.GetSysString(3743);
		default:
			return "";
		}
	}

	static TimeStruct getDateFromInt(int _time)
	{
		int _day = _time / (60 * 60 * 24);
		int _hour = (_time / (60 * 60)) % 24;
		int _min = (_time / (60)) % 60;
		int _se = _time % 60;
		return TimeStruct(_day, _hour, _min, _se);
	}

	static string getUserIP(string _userName)
	{
		boost::shared_ptr<User> mySelf = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(_userName);
		if (mySelf == NULL)
			return 0;
		boost::shared_ptr<UserVariable> ipPtr = mySelf->GetVariable("aIp");
		if (ipPtr == NULL)
			return 0;
		string ipUser = ipPtr->GetStringValue()->c_str();
		return ipUser;
	}
	static string getRoomNameForChess(){
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
		if (room != NULL){
			int mCurrGameID = std::atoi(room->GroupId()->substr(0, 3).c_str());
			boost::shared_ptr<RoomVariable> param = room->GetVariable("params");
			if (param != NULL){
				string paramString = param->GetStringValue()->c_str();
				vector<string> arrInfo = mUtils::splitString(paramString, '@');
				string money = arrInfo.at(0);
				string moneyConvert = mUtils::convertMoneyEx(std::atoi(money.c_str()));
				string str_title = mUtils::getGameNameByID(mCurrGameID)->c_str();
				string typeTable = mUtils::getTypeRoom(atoi(money.c_str()));
				int room_id = room->Id();
				//update 11/7
				string sRoomIdDisplay = "";
				vector<string> lstInfo = mUtils::splitString(*room->Name(), '_');
				if (lstInfo.size() == 2)
					sRoomIdDisplay = lstInfo[1];
				else if (lstInfo.size() == 3)
					sRoomIdDisplay = lstInfo[2];
				//string sRoomIdDisplay = mUtils::getRoomIDByName(*room->Name());
				//
				int currRoomType = SceneManager::getSingleton().getCurrRoomType();
				str_title += StringUtils::format(" - Phòng : %s %s Cược %s %s",
					sRoomIdDisplay.c_str(), typeTable.c_str(), moneyConvert.c_str(), SceneManager::getSingleton().getCoin(currRoomType).c_str());
				return str_title;
			}
		}
		return "";
	}

	static string getRoomNameByID(int idRoom){
		boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->RoomManager()->GetRoomById(idRoom);
		if (room != NULL){
			//HoangDD Change
			//update 11/7
			string sRoomIdDisplay = "";
			vector<string> lstInfo = mUtils::splitString(*room->Name(), '_');
			if (lstInfo.size() == 2)
				sRoomIdDisplay = lstInfo[1];
			else if (lstInfo.size() == 3)
				sRoomIdDisplay = lstInfo[2];
			return sRoomIdDisplay;
		}
		return "";
	}

	static double getMinBetByGame(int gameID, double amf){
		double betUser = 0;
		vector<strGameCF> arrBet = SceneManager::getSingleton().getArrConfig();
		int bet_ratio = 1;
		for (int i = 0; i < arrBet.size(); ++i)
		{
			if (arrBet.at(i).gameid == gameID)
			{
				bet_ratio = arrBet.at(i).ratioBet;
				break;
			}
		}
		betUser = amf / bet_ratio;
		return betUser;
	}
};
#endif // gameUtils_h__
