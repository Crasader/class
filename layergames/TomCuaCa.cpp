//
//  TomCuaCa.cpp
//  iCasino_v2
//
//  Created by Tuan on 6/23/14.
//
//
#include "TomCuaCa.h"
#include "_Avatar_.h"
#include "_Background_inGame_.h"
#include "_Button_inGame_.h"
#include "Requests/ExtensionRequest.h"
#include "_Number_inTomCuaCa.h"
#include "mUtils.h"
#include "_Chat_.h"
#include "SceneManager.h"
#include <iostream>
#include "SliderCustomLoader.h"
#include "Requests/LeaveRoomRequest.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "data_manager.h"
#include "LayerBet_BaCayChuong.h"

enum tagBet
{
	t_nai = 1,
	t_ruou,
	t_ga,
	t_ca,
	t_cua,
	t_tom,

};
const int TAG_PLUS_BUTTON = 192;
const int TAG_PLUS_BET = 246;
enum TCC_REPONSE {
	EXT_EVENT_USER_JOIN_NOTIF,      // jrntf
	EXT_EVENT_READY_NTF,
	EXT_EVENT_ERROR_READY_NTF,      // = "e_rntf";
	EXT_EVENT_UNREADY_NTF,          // = "urntf";
	EXT_EVENT_READY_RES,            // = "rdres";
	EXT_EVENT_LIST_USER_UPDATE,     // = "luu";
	EXT_EVENT_START,				// = "s";
	EXT_EVENT_USER_LEAVE_NOTIF,     // = "lrntf";
	EXT_EVENT_END,					// = "e";
	EXT_EVENT_AMF_TEST_NOTIF,       // = "cblltf";
	EXT_EVENT_VICTORY_NOTIF,        // = "vicntf";
	EXT_EVENT_READY_REQ,            // = "rr";
	EXT_EVENT_UNREADY_REQ,			// = "urr"
	EXT_EVENT_GAME_RESULT,			// = "grs"
	EXT_EVENT_GAME_BET_NTF,			//= "gb_ntf"
	EXT_REQUEST_TOKEN,				// = "ire"
	EXT_EVENT_START_READY_TIMER_NOTIF, //rdtntf
	EXT_EVENT_AUTO_KICK,
	EXT_FIELD_TOTAL_BET_OF_USER     //"tbou";
};
int TomCuaCa::convertResponseToInt(string inString) {
	if (inString == "jrntf")    return EXT_EVENT_USER_JOIN_NOTIF;
	if (inString == "e_rntf")   return EXT_EVENT_ERROR_READY_NTF;
	if (inString == "rntf")		return EXT_EVENT_READY_NTF;
	if (inString == "grs")		return EXT_EVENT_GAME_RESULT;
	if (inString == "urntf")    return EXT_EVENT_UNREADY_NTF;
	if (inString == "rdres")    return EXT_EVENT_READY_RES;
	if (inString == "urr")		return EXT_EVENT_UNREADY_REQ;
	if (inString == "luu")      return EXT_EVENT_LIST_USER_UPDATE;
	if (inString == "s")		return EXT_EVENT_START;
	if (inString == "gb_ntf")	return EXT_EVENT_GAME_BET_NTF;

	if (inString == "e")		return EXT_EVENT_END;
	if (inString == "cblltf")   return EXT_EVENT_AMF_TEST_NOTIF;
	if (inString == "vicntf")   return EXT_EVENT_VICTORY_NOTIF;
	if (inString == "ire")		return EXT_REQUEST_TOKEN;

	//
	if (inString == "rr")       return EXT_EVENT_READY_REQ;

	if (inString == "rdtntf")	return EXT_EVENT_START_READY_TIMER_NOTIF;
	if (inString == "kkntf")	return EXT_EVENT_AUTO_KICK;
	if (inString == "tbou")		return EXT_FIELD_TOTAL_BET_OF_USER;

	return -1;
}
string TomCuaCa::convertResponseToString(int inInt) {
	if (inInt == EXT_EVENT_USER_JOIN_NOTIF)     return "jrntf";
	if (inInt == EXT_EVENT_GAME_BET_NTF)     return "gb_ntf";
	if (inInt == EXT_EVENT_ERROR_READY_NTF)     return "e_rntf";

	if (inInt == EXT_EVENT_UNREADY_NTF)         return "urntf";
	if (inInt == EXT_EVENT_READY_RES)           return "rdres";
	if (inInt == EXT_EVENT_LIST_USER_UPDATE)    return "luu";
	if (inInt == EXT_EVENT_START)				return "s";
	if (inInt == EXT_EVENT_GAME_RESULT)			return "grs";
	if (inInt == EXT_EVENT_USER_LEAVE_NOTIF)    return "lrntf";
	if (inInt == EXT_EVENT_END)					return "e";
	if (inInt == EXT_EVENT_AMF_TEST_NOTIF)      return "cblltf";
	if (inInt == EXT_EVENT_VICTORY_NOTIF)       return "vicntf";

	//
	if (inInt == EXT_EVENT_READY_REQ)           return "rr";
	if (inInt == EXT_EVENT_UNREADY_REQ)         return "urr";
	if (inInt == EXT_EVENT_READY_NTF)           return "rntf";
	if (inInt == EXT_REQUEST_TOKEN)				return "ire";
	if (inInt == EXT_EVENT_START_READY_TIMER_NOTIF) return "rdtntf";
	if (inInt == EXT_EVENT_AUTO_KICK)			return "kkntf";
	if (inInt == EXT_FIELD_TOTAL_BET_OF_USER)	return "tbou";
	return "";
}
float TomCuaCa::convertResult(string rs)
{
	if (rs == "1") return _kqNai;
	if (rs == "2") return _kqRuou;
	if (rs == "3") return _kqGa;
	if (rs == "4") return _kqCa;
	if (rs == "5") return _kqCua;
	if (rs == "6") return _kqTom;
	return -1;
}
TomCuaCa::TomCuaCa(){
	myReady = false;
	myName = *GameServer::getSingleton().getSmartFox()->MySelf()->Name();
	_count = 100;
	numberUser = 0;

	uLayer = Layer::create();
	Layout *widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("TomCuaCa/scroll/TomCuaCa_1_1.json"));
	//uLayer->addChild(GUIReader::getInstance()->widgetFromJsonFile("TomCuaCa/scroll/TomCuaCa_1_1.json"));
	uLayer->addChild(widget);

	btnReady = Button::create();
	btnReady->loadTextures("ready.png", "ready_selected.png", "");
	btnReady->setTitleText(dataManager.GetSysString(195));
	//btnReady->setTitleFontName(kNormalFont);
	btnReady->setTag(t_ready);
	btnReady->setAnchorPoint(Point(0, 0));
	btnReady->setPosition(Point(WIDTH_DESIGN - btnReady->getContentSize().width - 20, 10));
	btnReady->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	btnReady->setTitleFontSize(20);
	btnReady->setTitleColor(Color3B::RED);

	uLayer->addChild(btnReady);

	btnUnReady = Button::create();
	btnUnReady->loadTextures("ready.png", "ready_selected.png", "");
	btnUnReady->setTitleText(dataManager.GetSysString(196));
	btnUnReady->setAnchorPoint(Point(0, 0));
	btnUnReady->setPosition(Point(WIDTH_DESIGN - btnUnReady->getContentSize().width - 20, 10));
	//btnUnReady->setTitleFontName(kNormalFont);
	btnUnReady->setTitleFontSize(20);
	btnUnReady->setTitleColor(Color3B::RED);
	btnUnReady->setVisible(false);
	btnUnReady->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	btnUnReady->setTag(t_unReady);
	uLayer->addChild(btnUnReady);

	//BUTTON
	btnTom = Button::create();

	btnTom->loadTextures("TomCuaCa/Tom.png", "TomCuaCa/Tom.png", "TomCuaCa/Tom_payment.png");
	btnTom->setPosition(Point(260, 280));
	btnTom->setTouchEnabled(false);
	btnTom->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	btnTom->setTag(t_tom + TAG_PLUS_BUTTON);
	uLayer->addChild(btnTom);

	btnCua = Button::create();

	btnCua->loadTextures("TomCuaCa/Cua.png", "TomCuaCa/Cua.png", "TomCuaCa/Cua_payment.png");
	btnCua->setPosition(Point(400, 280));
	btnCua->setTouchEnabled(false);
	btnCua->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	btnCua->setTag(t_cua + TAG_PLUS_BUTTON);
	uLayer->addChild(btnCua);

	btnCa = Button::create();

	btnCa->loadTextures("TomCuaCa/Ca.png", "TomCuaCa/Ca.png", "TomCuaCa/Ca_payment.png");
	btnCa->setPosition(Point(540, 280));
	btnCa->setTouchEnabled(false);
	btnCa->setTag(t_ca + TAG_PLUS_BUTTON);
	btnCa->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	uLayer->addChild(btnCa);

	btnGa = Button::create();

	btnGa->loadTextures("TomCuaCa/Ga.png", "TomCuaCa/Ga.png", "TomCuaCa/Ga_payment.png");
	btnGa->setPosition(Point(260, 160));
	btnGa->setTouchEnabled(false);
	btnGa->setTag(t_ga + TAG_PLUS_BUTTON);
	btnGa->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	uLayer->addChild(btnGa);

	btnRuou = Button::create();
	btnRuou->loadTextures("TomCuaCa/Ruou.png", "TomCuaCa/Ruou.png", "TomCuaCa/Ruou_payment.png");
	btnRuou->setPosition(Point(400, 160));
	btnRuou->setTouchEnabled(false);
	btnRuou->setTag(t_ruou + TAG_PLUS_BUTTON);
	btnRuou->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	uLayer->addChild(btnRuou);

	btnNai = Button::create();
	btnNai->loadTextures("TomCuaCa/Nai.png", "TomCuaCa/Nai.png", "TomCuaCa/Nai_payment.png");
	btnNai->setPosition(Point(540, 160));
	btnNai->setTouchEnabled(false);
	btnNai->setTag(t_nai + TAG_PLUS_BUTTON);
	btnNai->addTouchEventListener(CC_CALLBACK_2(TomCuaCa::clickBtn, this));
	uLayer->addChild(btnNai);
	//add to array
	arrButton.push_back(btnTom);
	arrButton.push_back(btnCua);
	arrButton.push_back(btnCa);
	arrButton.push_back(btnGa);
	arrButton.push_back(btnRuou);
	arrButton.push_back(btnNai);
	createArrayFrameResult();
	//loading
	loading = LoadingBar::create();
	loading->loadTexture("TomCuaCa/loading.png");
	loading->setPosition(Point(400, 370));
	loading->setPercent(100);
	uLayer->addChild(loading);

	// scroll

	scroll1 = static_cast<ui::ScrollView*>(widget->getChildByName("scrollView_kq1"));
	scroll2 = static_cast<ui::ScrollView*>(widget->getChildByName("scrollView_kq2"));
	scroll3 = static_cast<ui::ScrollView*>(widget->getChildByName("scrollView_kq3"));
	scroll1->setTouchEnabled(false);
	scroll2->setTouchEnabled(false);
	scroll3->setTouchEnabled(false);



	//frame bet
	betTom = FrameBet::create();
	betTom->setPosition(218, 238);
	betTom->SetValueBet("  Cược");
	betTom->setTag(t_tom + TAG_PLUS_BET);
	uLayer->addChild(betTom);

	betCua = FrameBet::create();
	betCua->setPosition(358, 238);
	betCua->SetValueBet("  Cược");
	betCua->setTag(t_cua + TAG_PLUS_BET);
	uLayer->addChild(betCua);

	betCa = FrameBet::create();
	betCa->setPosition(498, 238);
	betCa->setTag(t_ca + TAG_PLUS_BET);
	betCa->SetValueBet("  Cược");
	uLayer->addChild(betCa);

	betGa = FrameBet::create();
	betGa->setPosition(218, 118);
	betGa->setTag(t_ga + TAG_PLUS_BET);
	betGa->SetValueBet("  Cược");
	uLayer->addChild(betGa);

	betRuou = FrameBet::create();
	betRuou->setPosition(358, 118);
	betRuou->setTag(t_ruou + TAG_PLUS_BET);
	betRuou->SetValueBet("  Cược");
	uLayer->addChild(betRuou);

	betNai = FrameBet::create();
	betNai->setTag(t_nai + TAG_PLUS_BET);
	betNai->setPosition(498, 118);
	betNai->SetValueBet("  Cược");
	uLayer->addChild(betNai);

	GameServer::getSingleton().addListeners(this);
	createBackgrounds();
	createAvatars();
	createButtons();
	createTimer();

	//log("Touch piority button:%d  avatar:%d", lButton->getTouchPriority(), lAvatar->getTouchPriority());
	this->addChild(uLayer);
	_id_me = ((boost::shared_ptr<string>)(GameServer::getSingleton().getSmartFox()->MySelf()->Name()));
	if (lAvatar->isStartedGame() == true)
		btnReady->setVisible(false);
}
vector<string> TomCuaCa::TCCsplit(string &S, const char &str){
	vector<string> arrStr;
	string::iterator t, t2;
	for (t = S.begin(); t < S.end();){
		// Lặp từ vị trí bắt đầu
		t2 = find(t, S.end(), str); // Tìm vị trí space ' ' đầu tiên
		//kể từ vị trí t
		if (t != t2)
			arrStr.push_back(string(t, t2));
		if (t2 == S.end())
			break;
		t = t2 + 1; // chuyển sang vị trí sau
	}
	return arrStr;
}
void TomCuaCa::updateUser(string list){

	boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (ro == NULL || ro->GetVariable("params")==NULL) return;
	vector<string> rParams = mUtils::splitString(*ro->GetVariable("params")->GetStringValue(), '@');
	log("rParammmmmmmm---%s", rParams.at(1).c_str());
	lAvatar->setListUser(list);
	vector<string> listUser;
	listUser = TCCsplit(list, ';');
	numberUser = listUser.size();
	log("Do dai: %d", listUser.size());

	boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (lastRoom == NULL)
		return;
	isSpector = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())->IsSpectator();
	isSpector = lAvatar->isSpect();

	// 	if (isStartedGame()){
	// 		btnReady->setVisible(false);
	// 		layerTimer->stopTimer();
	// 	}
	// 	else{
	// 		btnReady->setVisible(true);
	// 	}
	// 
	// 	if (strcmp(myName.c_str(), (find_ChuPhong(list)).c_str()) == 0){
	// 		btnReady->setTitleText(dataManager.GetSysString(197));
	// 		layerTimer->stopTimer();
	// 	}
	// 	else{
	// 		btnReady->setTitleText(dataManager.GetSysString(195));
	// 	}

	if (isSpector) /// nếu là khách
	{
		lAvatar->getUserByPos(kuser0)->hideFlagIsMe();
		layerTimer->stopTimer();
		lAvatar->btn_vaochoi->setVisible(true);
		btnReady->setVisible(false);
		lButton->SetIsGuess(true);
		_time = 1;
		nameGame->setString(dataManager.GetSysString(515));
// 		if (!isStartedGame() && listUser.size() < 7){
// 			nameGame->setString(result.c_str());
// 			lAvatar->specToPlayer();
// 		}

	}
	else
	{
		if (lAvatar->btn_vaochoi->isEnabled()){
			layerTimer->startTimer();
		}
		lAvatar->getUserByPos(kuser0)->showFlagIsMe();		
		lAvatar->btn_vaochoi->setVisible(false);
		lButton->SetIsGuess(false);
		btnReady->setVisible(true);
		_time = 0;
		nameGame->setString(result.c_str());
		// 		if (listUser.size() > 1 && lAvatar->isStartedGame() != true)
		// 		{
		// 			lAvatar->playerToSpec();
		// 		}
		// 		else 
		if (listUser.size() < 1 ||
			strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(), (find_ChuPhong(list).c_str())) == 0){
			layerTimer->stopTimer();
			myReady = false;
			_time = 1;
			lAvatar->setFlag(kuser0, true);
			btnReady->setTitleText(dataManager.GetSysString(197));
		}
		else if (strcmp(myName.c_str(), (find_ChuPhong(list)).c_str()) != 0){
			btnReady->setTitleText(dataManager.GetSysString(195));
		}


		// 		btnReady->setTouchEnabled(true);
		// 		//btnUnReady->setTouchEnabled(true);

	}

	if (strcmp(list.c_str(), "") == 0)
	{
		log("return here");
		lAvatar->setPosChuong(-1);
		return;
	}
	lAvatar->setPosChuong(lAvatar->getPosByName(find_ChuPhong(list)));
	// 	if (strcmp(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str(), (find_ChuPhong(list).c_str())) == 0)
	// 	{
	// 		_time = 1;
	// 		lAvatar->setFlag(kuser0, true);
	// 		this->stopTimer();
	// 		btnReady->setTitleText(dataManager.GetSysString(197));
	// 	}
	// 	else
	// 	{
	// 		if (!myReady)
	// 			this->startTimer();
	// 		else this->stopTimer();
	// 		btnReady->setTitleText(dataManager.GetSysString(195));
	// 	}
	// 	if (isStartedGame()){
	// 		this->stopTimer();
	// 	}

}
string TomCuaCa::find_ChuPhong(string listUser){
	if (listUser.size() == 0) return "";
	vector<string> arrUser = TCCsplit(listUser, ';');
	vector<string> info = TCCsplit(arrUser[0], '|');
	string boosId = info[0];
	return boosId;
}
int	 TomCuaCa::getPosUserByName(string uid, string _list_user){
	int vt = -1;
	vector<string> list;
	if (_list_user.c_str() != NULL && _list_user != ""){
		list = TCCsplit(_list_user, ';');
	}
	//Tìm vị trí của mình trong list user
	for (int i = 0; i < list.size(); i++){
		string _id = TCCsplit(list[i], '|')[0];
		if (strcmp(_id.c_str(), _id_me->c_str()) == 0){
			vt = i;
			break;
		}
		else
			vt = 0;
	}
	// từ đó tìm vị trí của id truyền vào (so với mình)
	for (int k = 0; k < list.size(); k++){
		if (strcmp(list[k].c_str(), "") == 0)
			continue;
		string player = list[k];
		vector<string> n = TCCsplit(player, '|');
		if (strcmp(n[0].c_str(), uid.c_str()) == 0){
			if (k == vt){
				return kuser0;
			}
			else if (k == (vt + 1) % 6){
				return kuser1;
			}
			else if (k == (vt + 2) % 6){
				return kuser2;
			}
			else if (k == (vt + 3) % 6){
				return kuser3;
			}
			else if (k == (vt + 4) % 6){
				return kuser4;
			}
			else if (k == (vt + 5) % 6){
				return kuser5;
			}
			break;
		}
	}
	return 0;
}
void TomCuaCa::whenUserReady(string uid){
	if (strcmp(uid.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
		layerTimer->stopTimer();
		myReady = true;
		btnReady->setVisible(false);
		/*btnReady->setVisible(false);*/
		// hiện unready
		//btnUnReady->setVisible(true);
		/*btnUnReady->setVisible(true);*/
		lAvatar->getUserByPos(kuser0)->setReady(true);
	}
	else{
		switch (getPosUserByName(uid, _list_user)) {
		case kuser3:
			lAvatar->getUserByPos(kuser3)->setReady(true);
			break;
		case kuser4:
			lAvatar->getUserByPos(kuser4)->setReady(true);
			break;
		case kuser1:
			lAvatar->getUserByPos(kuser1)->setReady(true);
			break;
		case kuser2:
			lAvatar->getUserByPos(kuser2)->setReady(true);
			break;
		case kuser5:
			lAvatar->getUserByPos(kuser5)->setReady(true);
			break;
		default:
			break;
		}
	}
}
void TomCuaCa::whenUserUnready(string uid){

	if (strcmp(uid.c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
		layerTimer->startTimer();
		btnUnReady->setVisible(false);
		// 		btnUnReady->setTouchEnabled(false);
		// 		btnUnReady->setVisible(false);
		//hiện ready
		lAvatar->setReady(kuser0, false);
		btnReady->setVisible(true);
		// 		btnReady->setTouchEnabled(true);
		// 		btnReady->setVisible(true);

	}
	else{
		switch (getPosUserByName(uid, _list_user)) {
		case kuser3:
			lAvatar->setReady(kuser3, false);
			break;
		case kuser4:
			lAvatar->setReady(kuser4, false);
			break;
		case kuser1:
			lAvatar->setReady(kuser1, false);
			break;
		case kuser2:
			lAvatar->setReady(kuser2, false);
			break;
		case kuser5:
			lAvatar->setReady(kuser5, false);
			break;
		default:
			break;
		}
	}

	if (strcmp(btnReady->getTitleText().c_str(),
		dataManager.GetSysString(197).c_str()) == 0)
		layerTimer->stopTimer();
}
void TomCuaCa::whenGameStart(){
	if (isSpector) this->resetGame();
	myReady = false;
	layerTimer->stopTimer();
	// 	btnReady->setTouchEnabled(false);
	// 	btnReady->setVisible(false);
	// 	btnUnReady->setTouchEnabled(false);
	// 	btnUnReady->setVisible(false);
	btnUnReady->setVisible(false);
	btnReady->setVisible(false);
	lAvatar->setUnReadyAllUser();

	log("%d", _time);

	if (strcmp(myName.c_str(), (find_ChuPhong(_list_user)).c_str()) != 0){
		if (_time == 1)
		{
			btnTom->setTouchEnabled(false);
			btnCua->setTouchEnabled(false);
			btnCa->setTouchEnabled(false);
			btnGa->setTouchEnabled(false);
			btnRuou->setTouchEnabled(false);
			btnNai->setTouchEnabled(false);
		}
		else
		{
			btnTom->setTouchEnabled(true);
			btnCua->setTouchEnabled(true);
			btnCa->setTouchEnabled(true);
			btnGa->setTouchEnabled(true);
			btnRuou->setTouchEnabled(true);
			btnNai->setTouchEnabled(true);
		}
	}
	else {
		disableAllBet();
	}


	//Director::getInstance()->getScheduler()->scheduleUpdate();
	showChat(dataManager.GetSysString(62));

	log("Game start");

}
void TomCuaCa::whenResuiltGame(string rg){
	this->removeChildByTag(24);
	showChat(dataManager.GetSysString(64));
	btnTom->setTouchEnabled(false);
	btnCua->setTouchEnabled(false);
	btnCa->setTouchEnabled(false);
	btnGa->setTouchEnabled(false);
	btnRuou->setTouchEnabled(false);
	btnNai->setTouchEnabled(false);
	loading->setPercent(0);
	//playSound("sounds/game_tomcuaca/quay.mp3");

	vector<string> resuilt = TCCsplit(rg, '_');

	scroll1->scrollToPercentVertical(convertResult(resuilt[0]), 10, true);

	scroll2->scrollToPercentVertical(convertResult(resuilt[1]), 11, true);

	scroll3->scrollToPercentVertical(convertResult(resuilt[2]), 12, true);

	this->runAction(Sequence::create(DelayTime::create(8), 
		CallFunc::create(CC_CALLBACK_0(TomCuaCa::hienOketqua, this)), nullptr));
	this->runAction(Sequence::create(DelayTime::create(9), 
		CallFunc::create(CC_CALLBACK_0(TomCuaCa::hienKetQua, this)), nullptr));



}
void TomCuaCa::whenGameEnd(){
	createPool();
	arrGameResult.clear();
	NumberInTomCuaCa *layerNumbers = NumberInTomCuaCa::create();
	this->addChild(layerNumbers);
	_count = 100;
	loading->setPercent(_count);
	// 	btnReady->setTouchEnabled(true);
	// 	btnReady->setVisible(true);
	// 	btnUnReady->setTouchEnabled(false);
	// 	btnUnReady->setVisible(false);
	btnReady->setVisible(true);
	lAvatar->hideAllBet();
	lAvatar->setReady(kuser0, false);
	lAvatar->setReady(kuser1, false);
	lAvatar->setReady(kuser3, false);
	lAvatar->setReady(kuser4, false);
	lAvatar->setReady(kuser2, false);

	if (strcmp(find_ChuPhong(_list_user).c_str(), myName.c_str()) != 0){
		layerTimer->startTimer();
	}
	else if (isSpector){
		layerTimer->stopTimer();
		btnReady->setVisible(false);
		lAvatar->vaoBanChoi(NULL, Widget::TouchEventType::ENDED);
	}
}
TomCuaCa::~TomCuaCa(){
	arrButton.erase(arrButton.begin(), arrButton.end());
	arrTimer.erase(arrTimer.begin(), arrTimer.end());
	GameServer::getSingleton().removeListeners(this);
	// 	lAvatar->removeFromParentAndCleanup(true);
	// 	uLayer->removeChild(lButton);
	// 	uLayer->removeChild(layerChat);
	//uLayer->removeAllChildrenWithCleanup(true);
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic("sounds/game_tomcuaca/back.mp3");
	//this->removeAllChildren();
	//this->removeAllComponents();

}
bool TomCuaCa::init(){
	if (!Layer::init()){
		return false;
	}
	log("on init");
	for (int i = 0; i < arrName.size(); i++) {
		lAvatar->setMoney(lAvatar->getPosByName(arrName[i]), arrMoneyDouble[i]);
	}
	isSpector = false;
	arrName.clear();
	arrMoney.clear();
	arrMoneyDouble.clear();
	lAvatar->setUnReadyAllUser();
	// 	if (isStartedGame()){
	// 		layerTimer->stopTimer();
	// 	}

	createPool();
	return true;

}
void TomCuaCa::createBackgrounds(){

	Sprite *bg = Sprite::create("back.png");
	bg->setPosition(Point(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 - 20));
	this->addChild(bg);
	// thông tin bàn chơi và mức cược
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
		|| GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")==NULL)
		return;
	int id = std::atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0,3).c_str());
	boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
	if (param == NULL) return;
	string paramString = param->c_str();
	vector<string> arrInfo = TCCsplit(paramString, '@');
	string money = arrInfo.at(0);

	string *name = mUtils::getGameNameByID(id);

	string moneyConvert = mUtils::convertMoneyEx(std::atoi(money.c_str()));

	result = "";
	if (name->length() > 0 && moneyConvert.length() > 0)
	{
		result = *name;
		result += (" - cược:" + moneyConvert);
	}
	nameGame = Label::createWithSystemFont(result, "", 16);
	nameGame->setPosition(Point(400, 222));
	nameGame->setColor(Color3B::WHITE);
	nameGame->setOpacity(150);
	uLayer->addChild(nameGame);


}
void TomCuaCa::createButtons(){

	lButton = LayerButtonInGame::create();
	uLayer->addChild(lButton);

}
void TomCuaCa::createAvatars(){
	//MY info

	lAvatar = AvatarInTomCuaCa::create();
	lAvatar->resetAll();
	lAvatar->setCallBackToPlayer(this, callfunc_selector(TomCuaCa::funCallBackToPlayer));
	uLayer->addChild(lAvatar);
}

void TomCuaCa::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){


	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));


	if (strcmp("hbc", cmd->c_str()) == 0){
		return;
	}
	if (strcmp("nem_ntf", cmd->c_str()) == 0){
		string gameID = SERVICE_TOM_CUA_CA;
		SceneManager::getSingleton().gotoMain();
		//HoangDD
		//SceneManager::getSingleton().gotoChonBanChoi(atol(gameID.c_str()));
	}

	log("---------- %s", cmd->c_str());

	switch (convertResponseToInt(cmd->c_str()))
	{
	case EXT_EVENT_LIST_USER_UPDATE:
	{
		boost::shared_ptr<string> lu = param->GetUtfString("lu");
		if (lu != NULL) {
			log("List user: %s", lu->c_str());
			_list_user = *lu;
			updateUser(_list_user);


		}
		else{
			_list_user = "";
			log("Không nhận được listuser");
			break;
		}
	}
	case EXT_EVENT_READY_NTF:
	{
		boost::shared_ptr<string> uid = param->GetUtfString("uid");
		if (uid != NULL){
			whenUserReady(*uid);

		}
		break;
	}
	case EXT_EVENT_ERROR_READY_NTF:
	{
		boost::shared_ptr<long> errc = param->GetInt("errc");
		if (errc != NULL)
		{
			if (*errc == 30)
			{
				showChat(dataManager.GetSysString(57));
			}
			if (*errc == 29)
			{
				showChat(dataManager.GetSysString(63));
			}
		}
		break;
	}
	case EXT_EVENT_UNREADY_NTF:
	{
		boost::shared_ptr<string> uid = param->GetUtfString("uid");
		if (uid != NULL){
			whenUserUnready(*uid);
		}
		break;
	}
	case EXT_EVENT_START:
	{
		whenGameStart();
		break;
	}
	case EXT_EVENT_END:
	{

		whenGameEnd();
		break;
	}
	case EXT_EVENT_GAME_RESULT:
	{
		boost::shared_ptr<string> resul = param->GetUtfString("rg");
		log("--------%s", resul->c_str());
		kq = param->GetUtfString("rgu");
		log("--------%s", kq->c_str());
		arrGameResult = TCCsplit(*resul, '_');
		// 		kq1 = gameRs[0];
		// 		kq2 = gameRs[1];
		// 		kq3 = gameRs[2];
		whenResuiltGame(*resul);
		break;
	}
	case EXT_EVENT_GAME_BET_NTF:
	{
		boost::shared_ptr<long> tienbet = param->GetInt("gbv");
		double _tienBet = double(*tienbet);
		boost::shared_ptr<long> totalbet = param->GetInt("tboc");
		double _totalBet = double(*totalbet);

		boost::shared_ptr<long> oCuoc = param->GetInt("aid");
		log("---O cuoc: %ld ", *oCuoc);
		boost::shared_ptr<long> totalbetofuser = param->GetInt(convertResponseToString(EXT_FIELD_TOTAL_BET_OF_USER));
		double _betOfUser = double(*totalbetofuser);

		int _aid = 0;
		_aid = int(*oCuoc);
		bet(_aid, _tienBet, _totalBet, _betOfUser);

		break;
	}
	case EXT_REQUEST_TOKEN:
	{
		boost::shared_ptr<string> token = param->GetUtfString("ire");
		log("----%s", token->c_str());

		break;
	}
	case EXT_EVENT_START_READY_TIMER_NOTIF:
		layerTimer->startTimer();
		break;
	case EXT_EVENT_AUTO_KICK:
	{
		// 		layerTimer->stopTimer();
		// 		lAvatar->btn_vaochoi->setVisible(true);

		boost::shared_ptr<string> uid = param->GetUtfString("uid");
		if (uid->compare(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0){
			string gameID = SERVICE_TOM_CUA_CA;
			SceneManager::getSingleton().gotoMain();
			//HoangDD
			//SceneManager::getSingleton().gotoChonBanChoi(atol(gameID.c_str()));
		}
	}
	break;
	default:
		break;
	}
}
void TomCuaCa::clickBtn(Ref* obj, Widget::TouchEventType type)
{
	Button* bTag = (Button*)obj;
	int _bTag = bTag->getTag();

	if (type == Widget::TouchEventType::ENDED)
		switch (_bTag)
	{
		case t_ready:
		{
			this->resetGame();
			boost::shared_ptr<ISFSObject> parameter(new SFSObject());
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_READY_REQ), parameter, lastRoom));
			GameServer::getSingleton().Send(request);
			log("CLICK READY");
			break;
		}
		case t_unReady:
		{
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr< Room > lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
			if (lastRoom == NULL)
				return;
			boost::shared_ptr<IRequest> request(new ExtensionRequest(convertResponseToString(EXT_EVENT_UNREADY_REQ), params, lastRoom));
			GameServer::getSingleton().Send(request);
			log("CLICK UNREADY");
			break;
		}
		case (t_tom + TAG_PLUS_BUTTON) :
		case (t_cua + TAG_PLUS_BUTTON) :
		case (t_ca + TAG_PLUS_BUTTON) :
		case (t_ga + TAG_PLUS_BUTTON) :
		case (t_ruou + TAG_PLUS_BUTTON) :
		case (t_nai + TAG_PLUS_BUTTON) :
		{
			clickBet(bTag);
		}
									   break;
	}

}
void TomCuaCa::update(float dt)
{
	_count -= 0.12;
	loading->setPercent(_count);
	if (_count < 0){
		btnTom->setTouchEnabled(false);
		btnCua->setTouchEnabled(false);
		btnCa->setTouchEnabled(false);
		btnGa->setTouchEnabled(false);
		btnRuou->setTouchEnabled(false);
		btnNai->setTouchEnabled(false);
	}
}
void TomCuaCa::clickBet(Button *pSender)
{

	int _tag = pSender->getTag() - TAG_PLUS_BUTTON;
	if (arrMoneyLayerBet.size() > 0)
		createLayerBet(arrMoneyLayerBet.at(_tag - 1), _tag, pSender);
	// 	if (_tag != layerCuoc->getAid())
	// 	{
	// 		layerCuoc->setAid(_tag);
	// 		layerCuoc->setSlideValue(true);
	// 	}
	// 	else{
	// 		layerCuoc->setSlideValue(false);
	// 	}

	// 	switch (_tag)
	// 	{
	// 	case t_tom:
	// 		log("bet Tom");
	// 		break;
	// 	case t_cua:
	// 		log("bet cua");
	// 		break;
	// 	case t_ca:
	// 		log("bet ca");
	// 		break;
	// 	case t_ga:
	// 		log("bet ga");
	// 		break;
	// 	case t_ruou:
	// 		log("bet ruou");
	// 		break;
	// 	case t_nai:
	// 		log("bet nai");
	// 		break;
	// 	}
}

void TomCuaCa::bet(int aid, double tienBet, double totalBet, double totalBetOfUser)
{
	playSound("sounds/game_tomcuaca/datcuoc.mp3");
	boost::shared_ptr<string> uid = param->GetUtfString("uid");
	if (arrMoneyLayerBet.size() > 0 &&
		strcmp(uid->c_str(), myName.c_str()) == 0)
		arrMoneyLayerBet.at(aid - 1) = tienBet;
	string bet_ = convertMoneyFromDouble(tienBet);
	string strtotalBet = convertMoneyFromDouble(totalBet) ;
	string _totalbetOfUser = convertMoneyFromDouble(totalBetOfUser);
	log("-----%s", bet_.c_str());
	FrameBet *betFrame = (FrameBet *)uLayer->getChildByTag(aid + TAG_PLUS_BET);
	Button *btnCuoc = (Button *)uLayer->getChildByTag(aid + TAG_PLUS_BUTTON);
	//if (!isSpector)
	lAvatar->getUserByPos(lAvatar->getPosByName(*uid))->getLbMoneyBet()->setVisible(true);
	lAvatar->getUserByPos(lAvatar->getPosByName(*uid))->setLbMoneyBet(_totalbetOfUser);

	if (strcmp(find_ChuPhong(_list_user).c_str(), myName.c_str()) == 0 || isSpector){
		btnCuoc->setBright(false);
		betFrame->SetValueBet(strtotalBet.c_str());
	}
	else if (strcmp(uid->c_str(), myName.c_str()) == 0 ){
		btnCuoc->setBright(false);
		betFrame->SetValueBet(bet_.c_str());
	}
}

void TomCuaCa::setTimer(float dt)
{

}
void TomCuaCa::hienKetQua()
{
	NumberInTomCuaCa *layerNumbers = NumberInTomCuaCa::create();
	this->addChild(layerNumbers);
	vector<string> _kq = TCCsplit(*kq, ';');
	for (int i = 0; i < _kq.size(); i++)
	{
		vector<string> info = TCCsplit(_kq[i], '@');
		string _temp = info[1];

		//double mon =std::atoi(info[1].c_str());
		string _money = "";

		if (_temp[0] != '-'){ _money = "+" + info[1]; }
		else{ _money = info[1]; }

		if (GameServer::getSingleton().getSmartFox()->MySelf()!=NULL && GameServer::getSingleton().getSmartFox()->MySelf()->Name() != NULL && strcmp(info[0].c_str(), GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str()) == 0)
		{
			long num = atol(_money.c_str());
			_money = mUtils::convertMoneyEx(num);
			
			layerNumbers->showNumberByPos(kuser0, _money);
			if (_temp[0] != '-'){
				int numrd = rand() % 4 + 1;
				string path = StringUtils::format("thang_roi_%d.mp3", numrd);
				playSound(path);
				
			}
			else{
				int numrd = rand() % 2 + 1;
				string path = StringUtils::format("thua_roi_%d.mp3", numrd);
				playSound(path);
			}
		}
		else{
			switch (getPosUserByName(info[0], _list_user))
			{
			case kuser3:
				layerNumbers->showNumberByPos(kuser3, _money);
				log("left");
				break;
			case kuser4:
				layerNumbers->showNumberByPos(kuser4, _money);
				log("righ");
				break;
			case kuser1:
				layerNumbers->showNumberByPos(kuser1, _money);
				log("top");
				break;
			case kuser2:
				layerNumbers->showNumberByPos(kuser2, _money);
				log("bot");
				break;
			case kuser5:
				layerNumbers->showNumberByPos(kuser5, _money);
				log("bot");
				break;
			default:
				break;
			}//switch
		}//else
	}//for
}//void
void TomCuaCa::hienOketqua()
{

	log("hien o ket qua");
	// 	if (kq1 == "1" || kq2 == "1" || kq3 == "1")
	// 		btnNai->setColor(Color3B::YELLOW);
	// 	if (kq1 == "2" || kq2 == "2" || kq3 == "2")
	// 		btnRuou->setColor(Color3B::YELLOW);
	// 	if (kq1 == "3" || kq2 == "3" || kq3 == "3")
	// 		btnGa->setColor(Color3B::YELLOW);
	// 	if (kq1 == "4" || kq2 == "4" || kq3 == "4")
	// 		btnCa->setColor(Color3B::YELLOW);
	// 	if (kq1 == "5" || kq2 == "5" || kq3 == "5")
	// 		btnCua->setColor(Color3B::YELLOW);
	// 	if (kq1 == "6" || kq2 == "6" || kq3 == "6")
	// 		btnTom->setColor(Color3B::YELLOW);
	for (int i = 0; i < arrButton.size(); ++i){
		Button *btn = (Button*)arrButton.at(i);
		ProgressTimer *timer = (ProgressTimer*)arrTimer.at(i);
		for (int j = 0; j < arrGameResult.size(); ++j){
			log("TAG _ BUTTOM  = %d : TAG_RESULT: %s", btn->getTag(), arrGameResult.at(j).c_str());

			if ((btn->getTag() - TAG_PLUS_BUTTON) == std::atoi(arrGameResult.at(j).c_str())){ //  kiem tra neu dung tag
				btn->setColor(Color3B::YELLOW);
				timer->setVisible(true);
				auto action = RepeatForever::create(ProgressTo::create(1, 100));
				timer->runAction(action->clone());
				//timer->runAction(repeat);
			}
		}
	}
}

void TomCuaCa::OnSmartFoxPublicMessage(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueSender = (*ptrEventParams)["sender"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueSender);
	boost::shared_ptr<void> ptrEventParamValueMessage = (*ptrEventParams)["message"];
	boost::shared_ptr<string> ptrNotifiedMessage = ((boost::static_pointer_cast<string>))(ptrEventParamValueMessage);
	//
    if (dataManager.getAppConfig().is24h)
    {
        return;
    }
	log("ptrNotifiedMessage: %s", ptrNotifiedMessage->c_str());
	int pos = getPosUserByName(ptrNotifiedUser->Name()->c_str(), _list_user);
	if (pos == -1)
	{
		return;
	}
	lAvatar->showChatByPos(pos, ptrNotifiedMessage->c_str());
	playSound("sounds/game_tomcuaca/chat.mp3");
}

void TomCuaCa::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{

}

void TomCuaCa::OnSmartFoxUserExitRoom(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	log("User ExitRoom On Room");
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
	boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
	//
	if (ptrNotifiedUser->IsItMe()){
		string gameID = SERVICE_TOM_CUA_CA;
		SceneManager::getSingleton().gotoMain();
		//HoangDD
		//SceneManager::getSingleton().gotoChonBanChoi(atol(gameID.c_str()));
	}
}
// void TomCuaCa::OnSmartFoxUserVariableUpdate(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
// 	//log("Update User Variables");
// 	//boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEventParams = ptrEvent->Params();
// 	//boost::shared_ptr<void> ptrEventParamValueUser = (*ptrEventParams)["user"];
// 	//boost::shared_ptr<User> ptrNotifiedUser = ((boost::static_pointer_cast<User>))(ptrEventParamValueUser);
// 
// 	////string money = boost::to_string(*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
// 	//int    money = (int) (*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
// 	//double moneyDouble = (*ptrNotifiedUser->GetVariable("amf")->GetDoubleValue());
// 	//string name = boost::to_string(*ptrNotifiedUser->Name());
// 	//log("OnSmartFoxUserVariableUpdate: name= %s, money= %d", name.c_str(), money);
// 	//arrName.push_back(name);
// 	//arrMoney.push_back(money);
// 	//arrMoneyDouble.push_back(moneyDouble);
// 	//  isSpector = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(GameServer::getSingleton().getSmartFox()->MySelf()->Name()->c_str())->IsSpectator();
// 	//isSpector = lAvatar->isSpect();
// 
// }
void TomCuaCa::playSound(string soundPath)
{
	if (mUtils::isSoundOn())
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundPath.c_str());
}
string TomCuaCa::convertMoney(int money){

	ostringstream oss;
	/// 
	if (money < 1000) {
		oss.clear();
		oss << money;
		return (oss.str() + "");
	}
	else if (money >= 1000 && money <= 999999) {
		string hangTram;
		string hangNghin;

		ostringstream oss1;
		// Nghin
		oss1.clear();
		oss1 << (money / 1000);
		hangNghin = oss1.str();

		ostringstream oss2;
		// tram
		int hTram = (money % 1000);
		oss2.clear();
		oss2 << hTram;
		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss2.str();

		return (hangNghin + "," + hangTram + "");
	}
	else if (money >= 1000000) {
		string hangTrieu;
		string hangNghin;
		string hangTram;

		ostringstream oss1;
		// Trieu
		oss1.clear();
		oss1 << (money / 1000000);
		hangTrieu = oss1.str();

		// Nghin
		int hNghin = (money - (money / 1000000) * 1000000) / 1000;
		ostringstream oss2;
		oss2.clear();
		oss2 << hNghin;

		if (hNghin < 10)
		{
			hangNghin = "00";
		}
		else if (hNghin >= 10 && hNghin < 100)
		{
			hangNghin = "0";
		}
		hangNghin += oss2.str();

		// Tram
		int hTram = (money % 1000);
		ostringstream oss3;
		oss3.clear();
		oss3 << hTram;

		if (hTram < 10)
		{
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100)
		{
			hangTram = "0";
		}
		hangTram += oss3.str();

		return (hangTrieu + "," + hangNghin + "," + hangTram + "");
	}

	return "";
}

string TomCuaCa::convertMoneyFromDouble(double money) {
	// tiền tỷ
	if (money >= 1000000000) {
		int ti = money / 1000000000;
		ostringstream os;
		os << ti;
		return (os.str() + " tỷ");
	}
	else if (money >= 1000000) {
		// tiền triệu
		int ti = money / 1000000;
		ostringstream os;
		os << ti;
		return (os.str() + " triệu");
	}
	else if (money >= 1000) {
		string hangTram;
		string hangNghin;

		// Nghin
		ostringstream oss1;
		oss1.clear();
		oss1 << (int)(money / 1000);
		hangNghin = oss1.str();

		ostringstream oss2;
		// tram
		int hTram = ((int)money % 1000);
		oss2.clear();
		oss2 << hTram;
		if (hTram < 10) {
			hangTram = "00";
		}
		else if (hTram >= 10 && hTram < 100) {
			hangTram = "0";
		}
		hangTram += oss2.str();

		return (hangNghin + "," + hangTram);
	}
	else {
		ostringstream os;
		os << money;
		return (os.str());
	}

	return "!";
}

string TomCuaCa::convertMoneyFromDouble_Detail(double money) {
	if (money < 1000) {
		ostringstream os;
		os << money;
		return (os.str());
	}
	else {
		money = money / 1000;

		if (money > 2000000000) {
			return convertMoneyFromDouble(money * 1000);
		}
		else return (convertMoney((int)money) + "k");
	}


}
void TomCuaCa::specMode()
{
	log("spec mode");


	/*lButton->getButtonByTag(103)->setTouchEnabled(true);*/

	// 	btnReady->setVisible(false);
	// 	btnUnReady->setVisible(false);
	// 	btnReady->setTouchEnabled(false);
	// 	btnUnReady->setTouchEnabled(false);

	btnUnReady->setVisible(false);
	btnReady->setVisible(false);
}

void TomCuaCa::resetGame()
{
	for (int i = 0; i < arrTimer.size(); ++i){
		ProgressTimer *timer = (ProgressTimer*)arrTimer.at(i);
		timer->stopAllActions();
		timer->setVisible(false);
		Button *btn = (Button*)arrButton.at(i);
		btn->setColor(Color3B::WHITE);
		btn->setBright(true);
	}
	
	scroll1->scrollToTop(0.1, false);

	scroll2->scrollToTop(0.1, false);

	scroll3->scrollToTop(0.1, false);

	betTom->SetValueBet("  Cược");
	betCua->SetValueBet("  Cược");
	betCa->SetValueBet("  Cược");
	betGa->SetValueBet("  Cược");
	betRuou->SetValueBet("  Cược");
	betNai->SetValueBet("  Cược");

	// 	btnTom->setBright(true);
	// 	btnTom->setColor(Color3B::WHITE);
	// 	betTom->SetCharge(false);
	// 
	// 	btnCua->setBright(true);
	// 	btnCua->setColor(Color3B::WHITE);
	// 	betCua->SetCharge(false);
	// 
	// 	btnCa->setBright(true);
	// 	btnCa->setColor(Color3B::WHITE);
	// 	betCa->SetCharge(false);
	// 
	// 	btnGa->setBright(true);
	// 	btnGa->setColor(Color3B::WHITE);
	// 	betGa->SetCharge(false);
	// 
	// 	btnRuou->setBright(true);
	// 	btnRuou->setColor(Color3B::WHITE);
	// 	betRuou->SetCharge(false);
	// 
	// 	btnNai->setBright(true);
	// 	btnNai->setColor(Color3B::WHITE);
	// 	betNai->SetCharge(false);


}


bool TomCuaCa::isStartedGame()
{
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room == NULL)
		return false;
	boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
	if (rv == NULL) return false;
	string s = *rv->GetStringValue();

	vector<string> lstBet = mUtils::splitString(s, '@');
	bool isStartedGame = false;
	lstBet.at(1).compare("1") == 0 ? (isStartedGame = true) : (isStartedGame = false);
	if (isStartedGame) {
		log("Ban dang choi!");
	}
	else log("Ban chua choi!");

	return isStartedGame;
}

void TomCuaCa::createTimer()
{
	layerTimer = LayerTimer::create();
	layerTimer->setCount(15);
	uLayer->addChild(layerTimer, 10);
	if (!isStartedGame())
		layerTimer->startTimer();
}
bool TomCuaCa::checkPlaying(string _list){
	if (strcmp(_list.c_str(), "") == 0) return false;
	log("Jump to here check playing %s", _list.c_str());
	vector<string> info = mUtils::splitString(_list, ';');
	vector<string> firt = mUtils::splitString(info[0], '|');
	if (strcmp(firt[2].c_str(), "1") == 0)
	{
		return true;
	}
	return false;
}

void TomCuaCa::disableAllBet()
{
	btnTom->setTouchEnabled(false);
	btnCa->setTouchEnabled(false);
	btnCua->setTouchEnabled(false);
	btnGa->setTouchEnabled(false);
	btnNai->setTouchEnabled(false);
	btnRuou->setTouchEnabled(false);

}

void TomCuaCa::funCallBack()
{
	btnReady->setVisible(true);
	btnReady->setVisible(true);
	layerTimer->startTimer();
}

void TomCuaCa::createLayerBet(float _money, int _tag, Button *pSender)
{
	//HoangDD
	cocosbuilder::CCBReader * ccbReader = NULL;
	cocosbuilder::NodeLoaderLibrary * ccNodeLoaderLibrary = SceneManager::getSingleton().getNodeLoaderLibrary();
	// read main layer
	ccbReader = new cocosbuilder::CCBReader(ccNodeLoaderLibrary);

	if (ccbReader)
	{
		LayerBet_BaCayChuong *layerCuoc = (LayerBet_BaCayChuong *)ccbReader->readNodeGraphFromFile("LayerBet_BaCayChuong.ccbi");
		layerCuoc->setPosition(Point(10, 10));
		layerCuoc->setAID(_tag);
		layerCuoc->setMoney(_money);
		layerCuoc->setStyleGame(kGameTomCuaCa);
		layerCuoc->setTag(24);
		this->addChild(layerCuoc);
		ccbReader->release();
	}
}

void TomCuaCa::createPool()
{
	if (strcmp(find_ChuPhong(_list_user).c_str(), myName.c_str()) == 0)
		return;
	arrMoneyLayerBet.clear();
	for (int i = 0; i < 6; ++i){
		arrMoneyLayerBet.push_back(0);
	}
}

void TomCuaCa::createArrayFrameResult()
{
	for (int i = 0; i < arrButton.size(); ++i){
		Sprite *time = Sprite::create("TomCuaCa/progress.png");
		ProgressTimer *timer = ProgressTimer::create(time);
		timer->setType(ProgressTimer::Type::RADIAL);
		timer->setPosition(arrButton.at(i)->getPosition());
		timer->setVisible(false);
		uLayer->addChild(timer, -1);

		arrTimer.push_back(timer);
	}
}

void TomCuaCa::funCallBackToPlayer()
{
	//lấy số người chơi tối đa trong 1 phòng
	boost::shared_ptr<Room> ro = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (ro == NULL)
		return;

	int maxUser = std::atoi(boost::to_string(ro->MaxUsers()).c_str());
	if (numberUser >= maxUser){
		showChat(dataManager.GetSysString(198));
		return;
	}
	else{
		//yêu cầu vào chơi
		boost::shared_ptr<IRequest> request(new SpectatorToPlayerRequest());
		GameServer::getSingleton().Send(request);
		//yêu cầu join game
		boost::shared_ptr<ISFSObject> params(new SFSObject());
		boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();

		boost::shared_ptr<IRequest> req2(new ExtensionRequest("rqjg", params, lastRoom));
		GameServer::getSingleton().Send(req2);
	}
}

void TomCuaCa::OnSmartFoxRoomDestroy(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL){
		/// room destroy
		string gameID = SERVICE_TOM_CUA_CA;
		SceneManager::getSingleton().gotoMain();
		//HoangDD
		//SceneManager::getSingleton().gotoChonBanChoi(atol(gameID.c_str()));
	}
}

void TomCuaCa::showChat(string mess)
{
	Chat *toast = Chat::create(mess, -1);
	this->addChild(toast);
}

