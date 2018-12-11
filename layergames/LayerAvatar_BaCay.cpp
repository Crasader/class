//
//  LayerAvatar_BaCay.cpp
//  iCasino_v2
//
//  Created by DauA on 6/26/14.
//
//
//

#include "LayerAvatar_BaCay.h"
#include "SceneManager.h"
#include "mUtils.h"
#include "AllData.h"
#include "Requests/SpectatorToPlayerRequest.h"
#include "Requests/PlayerToSpectatorRequest.h"
#include "ChanUtils.h"
#include "PhomMessDef.h"

LayerBaCayAvatar::LayerBaCayAvatar()
:Chuong(NULL),
btnNgoiChoi(NULL),
MyAI(""),
ListUsers(""),
b_IsSpect(true),
b_IsPlaying(false) {
	this->mapPlayers.clear();
}

LayerBaCayAvatar::~LayerBaCayAvatar() {
	log("Destructor ~LayerAvatar_BaCay");
}

bool LayerBaCayAvatar::init() {
	if (!Layer::init())
		return false;

	this->MyAI = SceneManager::getSingleton().getMyName().c_str();

	this->setAnchorPoint(Point(0, 0));
	this->setPosition(Point(0, 0));

	Avatar52La *UserMe = Avatar52La::create(false, 0);
	UserMe->displayButtonRechard(true);
	Avatar52La *User1 = Avatar52La::create(false, 0);
	Avatar52La *User2 = Avatar52La::create(false, 1);
	Avatar52La *User3 = Avatar52La::create(false, 0);
	Avatar52La *User4 = Avatar52La::create(false, 1);
	Avatar52La *User5 = Avatar52La::create(false, 1);
	Avatar52La *User6 = Avatar52La::create(false, 0);

	//Size convert
	Size _size = ChanUtils::getSizePos();


	//update for new version, only for lieng p1
	if (SceneManager::getSingleton().getGameID() == kGameBaCayChuong || SceneManager::getSingleton().getGameID() == kGameLieng){
		UserMe->setPosition(Point(WIDTH_DESIGN / 2 - WIDTH_DESIGN / 7, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 4));
		User1->setPosition(Point(WIDTH_DESIGN / 8, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7));
		User2->setPosition(Point(WIDTH_DESIGN / 8, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7));
		User3->setPosition(Point(WIDTH_DESIGN / 2 - WIDTH_DESIGN / 4 + 45.0, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 + 25.0));
		User4->setPosition(Point(WIDTH_DESIGN / 2 + WIDTH_DESIGN / 4 - 45.0, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 3 + 25.0));
		User5->setPosition(Point(WIDTH_DESIGN / 9 * 8, HEIGHT_DESIGN / 2 + HEIGHT_DESIGN / 7));
		User6->setPosition(Point(WIDTH_DESIGN / 9 * 8, HEIGHT_DESIGN / 2 - HEIGHT_DESIGN / 7));
	}
	else{
		UserMe->setPosition(Point((WIDTH_DESIGN / 2), 140 + _size.height));
		User1->setPosition(Point(WIDTH_DESIGN / 6, 220 + _size.height));
		User2->setPosition(Point(WIDTH_DESIGN / 9, (HEIGHT_DESIGN / 2 - 30) + _size.height));
		User3->setPosition(Point(WIDTH_DESIGN / 6, (HEIGHT_DESIGN - 280) + _size.height));
		User4->setPosition(Point(WIDTH_DESIGN / 6 * 5, (HEIGHT_DESIGN - 280) + _size.height));
		User5->setPosition(Point(WIDTH_DESIGN / 9 * 8, (HEIGHT_DESIGN / 2 - 30) + _size.height));
		User6->setPosition(Point(WIDTH_DESIGN / 6 * 5, 220 + _size.height));
	}

	UserMe->setTag(kuser0);
	User1->setTag(kuser1);
	User2->setTag(kuser2);
	User3->setTag(kuser3);
	User4->setTag(kuser4);
	User5->setTag(kuser5);
	User6->setTag(kuser6);

	//    UserMe->setScale(0.7);
	//    User1->setScale(0.7);
	//    User2->setScale(0.7);
	//    User3->setScale(0.7);
	//    User4->setScale(0.7);
	//    User5->setScale(0.7);
	//    User6->setScale(0.7);

	this->addChild(UserMe);
	this->addChild(User1);
	this->addChild(User2);
	this->addChild(User3);
	this->addChild(User4);
	this->addChild(User5);
	this->addChild(User6);

	this->Chuong = ui::ImageView::create();
	this->Chuong->loadTexture("Chuong.png");
	this->Chuong->setScale(1.5);
	this->setTag(152);
	this->Chuong->setTouchEnabled(false);
	this->Chuong->setVisible(false);
	this->Chuong->setAnchorPoint(Point(0, 0));
	this->Chuong->setPosition(Point(0, 0));
	this->addChild(this->Chuong);

	this->imgBienActive = ui::ImageView::create("moc3-cuoc-bien-success.png");
	this->imgBienActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->imgBienActive->setVisible(false);
	//this->addChild(this->imgBienActive);

	this->lblValueBien = Label::createWithTTF("", "__Roboto-Regular.ttf", 20);
	this->lblValueBien->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->imgBienActive->addChild(this->lblValueBien);
	this->lblValueBien->setPosition(Point(this->imgBienActive->getContentSize().width / 2,
		this->imgBienActive->getContentSize().height / 2));

	//update for new version, only for lieng p1
	if (SceneManager::getSingleton().getGameID() == kGameLieng)
	{
		UserMe->createFrameBetLieng();
		User1->createFrameBetLieng();
		User2->createFrameBetLieng();
		User3->createFrameBetLieng();
		User4->createFrameBetLieng();
		User5->createFrameBetLieng();
		User6->createFrameBetLieng();
	}

	return true;
}


void LayerBaCayAvatar::SetUrlAvatar(const int& pos, char *url) {
	if (this->GetUserByPos(pos) == NULL) return;
	this->GetUserByPos(pos)->setIcon(url);
}

void LayerBaCayAvatar::SetFlag(const int& pos, bool isShow) {
	if (this->GetUserByPos(pos) == NULL) return;
	this->GetUserByPos(pos)->setFlag(false);
	if (pos == kuser0) {
		this->GetUserByPos(kuser1)->setMeIsBoss(false);
		this->GetUserByPos(kuser2)->setMeIsBoss(false);
		this->GetUserByPos(kuser3)->setMeIsBoss(false);
		this->GetUserByPos(kuser4)->setMeIsBoss(false);
		this->GetUserByPos(kuser5)->setMeIsBoss(false);
		this->GetUserByPos(kuser6)->setMeIsBoss(false);
	}
}

void LayerBaCayAvatar::SetName(const int& pos, const char* name) {
	string strName = boost::to_string(name);
	this->SetName(pos, strName);
}

void LayerBaCayAvatar::SetName(const int& pos, string& name) {
	if (this->GetUserByPos(pos) == NULL) return;
	this->GetUserByPos(pos)->setName(name);
}

void LayerBaCayAvatar::SetMoney(const int& pos, string& money) {
	if (this->GetUserByPos(pos) == NULL) return;
	this->GetUserByPos(pos)->setMoney(money);
}

void LayerBaCayAvatar::SetMoney(const int& pos, const char* money) {
	string strMoney = boost::to_string(money);
	this->SetMoney(pos, strMoney);
}

void LayerBaCayAvatar::SetMoney(const int& pos, const int& money) {
	if (this->GetUserByPos(pos) == NULL)
		return;
	this->GetUserByPos(pos)->setMoney(money);
}

void LayerBaCayAvatar::SetReady(const int& pos, bool isReady) {
	// if (this->GetUserByPos(pos) == NULL) return;
	//this->GetUserByPos(pos)->setReady(isReady);
}

void LayerBaCayAvatar::SetVisibleLayerInvite(const int& pos, bool isShow) {
	Avatar52La *user = this->GetUserByPos(pos);
	if (user != NULL)
		user->setVisibleLayerInvite(isShow);
}

void LayerBaCayAvatar::ResetAll() {
	this->GetUserByPos(kuser1)->resetAll();
	this->GetUserByPos(kuser2)->resetAll();
	this->GetUserByPos(kuser3)->resetAll();
	this->GetUserByPos(kuser4)->resetAll();
	this->GetUserByPos(kuser5)->resetAll();
	this->GetUserByPos(kuser6)->resetAll();
}

void LayerBaCayAvatar::SetPosChuong(const int& pos) {
	//Size convert
	Size _size = ChanUtils::getSizePos();
	switch (pos) {
	case kuser1:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser1)->getPositionX() + 10, this->GetUserByPos(kuser1)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser2:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser2)->getPositionX() + 10, this->GetUserByPos(kuser2)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser3:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser3)->getPositionX() + 10, this->GetUserByPos(kuser3)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser4:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser4)->getPositionX() + 10, this->GetUserByPos(kuser4)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser5:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser5)->getPositionX() + 10, this->GetUserByPos(kuser5)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser6:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser6)->getPositionX() + 10, this->GetUserByPos(kuser6)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case kuser0:
		this->Chuong->setPosition(Point(this->GetUserByPos(kuser0)->getPositionX() + 10, this->GetUserByPos(kuser0)->getPositionY() + H_MONEY_BG / 2 - 10));
		this->Chuong->setVisible(true);
		break;
	case 100:
		this->Chuong->setVisible(false);
		break;
	default:
		break;
	}
	if (0 == ListUsers.compare("")) {
		log("list users: %s, pos: %d", this->ListUsers.c_str(), pos);
		this->Chuong->setVisible(false);
	}
}
void LayerBaCayAvatar::setResult(int pos, int rescode, int type, bool isOnGame, double money)
{
	Avatar52La *user = GetUserByPos(pos);
	if (user == NULL)
		return;
	user->setResult(rescode, type, isOnGame, money);
	user->stopTimer();
}
void LayerBaCayAvatar::UnReadyAllUser() {
	//    this->SetReady(kuser0, false);
	//    this->SetReady(kuser1, false);
	//    this->SetReady(kuser2, false);
	//    this->SetReady(kuser3, false);
	//    this->SetReady(kuser4, false);
	//    this->SetReady(kuser5, false);
	//    this->SetReady(kuser6, false);
	if (this->GetUserByPos(kuser0)->isChauRia() == false)
		this->GetUserByPos(kuser0)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser1)->isChauRia() == false)
		this->GetUserByPos(kuser1)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser2)->isChauRia() == false)
		this->GetUserByPos(kuser2)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser3)->isChauRia() == false)
		this->GetUserByPos(kuser3)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser4)->isChauRia() == false)
		this->GetUserByPos(kuser4)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser5)->isChauRia() == false)
		this->GetUserByPos(kuser5)->setVisbleLabelGuess(false);
	if (this->GetUserByPos(kuser6)->isChauRia() == false)
		this->GetUserByPos(kuser6)->setVisbleLabelGuess(false);

	this->GetUserByPos(kuser0)->stopTimer();
	this->GetUserByPos(kuser1)->stopTimer();
	this->GetUserByPos(kuser2)->stopTimer();
	this->GetUserByPos(kuser3)->stopTimer();
	this->GetUserByPos(kuser4)->stopTimer();
	this->GetUserByPos(kuser5)->stopTimer();
	this->GetUserByPos(kuser6)->stopTimer();

	this->GetUserByPos(kuser0)->removeResultBaCay();
	this->GetUserByPos(kuser1)->removeResultBaCay();
	this->GetUserByPos(kuser2)->removeResultBaCay();
	this->GetUserByPos(kuser3)->removeResultBaCay();
	this->GetUserByPos(kuser4)->removeResultBaCay();
	this->GetUserByPos(kuser5)->removeResultBaCay();
	this->GetUserByPos(kuser6)->removeResultBaCay();
}

void LayerBaCayAvatar::ResetGame() {
	this->UnReadyAllUser();
}

int LayerBaCayAvatar::GetPosByName(const string& uid) {
	return this->mapPlayers.find(uid) != this->mapPlayers.end() ? this->mapPlayers[uid] : -1;
}

void LayerBaCayAvatar::SetListUser(const string& listUser) {

	this->ListUsers = listUser;
	this->SetMapPlayer(this->ListUsers);
	//
	if (0 == this->ListUsers.compare("")) {
		this->GetUserByPos(kuser0)->setVisibleLayerInvite(true);
		return;
	}

	log("ListUsers: %s", this->ListUsers.c_str());

	//! splitString
	vector<string> arrUser = mUtils::splitString(this->ListUsers, ';');
	if (0 == arrUser.size()) return;

	Avatar52La *AvaUserMe = GetUserByPos(kuser0);
	Avatar52La *AvaUser1 = GetUserByPos(kuser1);
	Avatar52La *AvaUser2 = GetUserByPos(kuser2);
	Avatar52La *AvaUser3 = GetUserByPos(kuser3);
	Avatar52La *AvaUser4 = GetUserByPos(kuser4);
	Avatar52La *AvaUser5 = GetUserByPos(kuser5);
	Avatar52La *AvaUser6 = GetUserByPos(kuser6);


	AvaUserMe->removeResult();
	AvaUser1->removeResult();
	AvaUser2->removeResult();
	AvaUser3->removeResult();
	AvaUser4->removeResult();
	AvaUser5->removeResult();
	AvaUser6->removeResult();

	this->UnReadyAllUser();

	this->resetGuessUser();

	AvaUserMe->stopTimer();
	AvaUser1->stopTimer();
	AvaUser2->stopTimer();
	AvaUser3->stopTimer();
	AvaUser4->stopTimer();
	AvaUser5->stopTimer();
	AvaUser6->stopTimer();

	AvaUserMe->setVisible(true);
	// AvaUserMe->setReady(false);
	AvaUserMe->setName("");
	AvaUserMe->setMoney("");
	AvaUserMe->setAI("");

	AvaUser1->setVisibleLayerInvite(true);
	//   AvaUser1->setReady(false);
	AvaUser1->setName("");
	AvaUser1->setMoney("");
	AvaUser1->setAI("");
	//
	AvaUser2->setVisibleLayerInvite(true);
	//   AvaUser2->setReady(false);
	AvaUser2->setName("");
	AvaUser2->setMoney("");
	AvaUser2->setAI("");
	//
	AvaUser3->setVisibleLayerInvite(true);
	//   AvaUser3->setReady(false);
	AvaUser3->setName("");
	AvaUser3->setMoney("");
	AvaUser3->setAI("");
	//
	AvaUser4->setVisibleLayerInvite(true);
	//   AvaUser4->setReady(false);
	AvaUser4->setName("");
	AvaUser4->setMoney("");
	AvaUser4->setAI("");
	//
	AvaUser5->setVisibleLayerInvite(true);
	//    AvaUser5->setReady(false);
	AvaUser5->setName("");
	AvaUser5->setMoney("");
	AvaUser5->setAI("");
	//
	AvaUser6->setVisibleLayerInvite(true);
	//    AvaUser6->setReady(false);
	AvaUser6->setName("");
	AvaUser6->setMoney("");
	AvaUser6->setAI("");
	arrAI.clear();

	/* 32|chinhtt|1|-1, -1, -1;36|vodanh9x|0|36, 13, 32;
		String.format("%d|%s|%d|%s|%d|%d", user.getId(), user.getName(), isBoss?1:0, toStateCardString(turn), this.getTotalEnrollBet(), this.getCurrentbet());
		*/
	for (int i = 0; i < arrUser.size(); ++i) {
		vector<string> info = mUtils::splitString(arrUser[i], '|');

		//! Số lượng thông tin được bóc ra từ 1 arrUsers
		int m_info = info.size();
		int pos = GetPosByName(info[0]);

		this->arrAI.push_back(info[0]);

		if (pos < 0)  continue;

		boost::shared_ptr<User> m_player = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(info[0]);
		double _moneyDouble = 0;
		string _url = "";
		string _name = "";
		if (m_player == NULL){
			auto Users = this->GetUserByPos(pos);
			Users->stopTimer();
			Users->setMoney(-1);
		}
		else if (m_player != NULL) {

			_moneyDouble = 0;
			_url = "";
			_name = info[0];

			/*moc3-change aN=>adn*/
			if (m_player->GetVariable("adn") != NULL) {
				_name = *m_player->GetVariable("adn")->GetStringValue();
			}
			if (SceneManager::getSingleton().getCurrRoomType() == 0){
				if (m_player->GetVariable("amf") != NULL) {
					_moneyDouble = *m_player->GetVariable("amf")->GetDoubleValue();
				}
			}
			else{
				if (m_player->GetVariable("amfs") != NULL) {
					_moneyDouble = *m_player->GetVariable("amfs")->GetDoubleValue();
				}
			}
			if (m_player->GetVariable("aal") != NULL) {
				_url = *m_player->GetVariable("aal")->GetStringValue();
			}
			if (_name.compare("") == 0)
				_name = *m_player->Name();
			//update new version - game lieng
			if (SceneManager::getSingleton().getGameID() == kGameLieng)
			{
				boost::shared_ptr<UserVariable> gameChip_ptr = m_player->GetVariable(EXT_FIELD_GAME_CHIP);
				if (gameChip_ptr != NULL){
					boost::shared_ptr<double> money = gameChip_ptr->GetDoubleValue();
					if (money != NULL)
						_moneyDouble = *money;
				}
			}
		}
		Avatar52La *Users = GetUserByPos(pos);
		Users->setVisibleLayerInvite(false);
		Users->setName(_name);
		Users->setFlag(false);
		Users->setAI(info[0]);
		Users->setIcon(_url);
		Users->setMoney(_moneyDouble);
		Users->showSameIP(false);
		Users->setVisbleLabelGuess(false);
		//
		bool meIsBoss = false;
		if (pos == kuser0 && !this->b_IsSpect) {
			Users->setVisible(true);
			this->GetUserByPos(kuser1)->setMeIsBoss(false);
			this->GetUserByPos(kuser2)->setMeIsBoss(false);
			this->GetUserByPos(kuser3)->setMeIsBoss(false);
			this->GetUserByPos(kuser4)->setMeIsBoss(false);
			this->GetUserByPos(kuser5)->setMeIsBoss(false);
			this->GetUserByPos(kuser6)->setMeIsBoss(false);
		}
		/*
		if (4 == m_info) {
		if (0 == info[m_info - 1].compare("1") && !IsStartedGame()) {
		//Users->setReady(true);
		}
		if (0 == info[m_info - 1].compare("0") || 0 == i) {
		//Users->setReady(false);
		}
		}

		else if (5 == m_info) {
		if (0 == info[m_info - 1].compare("1") && !this->b_IsPlaying) {
		// Users->setReady(true);
		}
		if (0 == info[m_info - 1].compare("0") || 0 == info[1].compare("0")) {
		//Users->setReady(false);
		}
		}

		else {
		if (0 == info[1].compare("1")){
		Users->startTimer(13);
		}

		if (0 == info[2].compare("1") && !this->b_IsPlaying) {
		// Users->setReady(true);
		}
		if (0 == info[2].compare("0") || 0 == i) {
		//Users->setReady(false);
		}
		}*/
	}//for
	checkSameIP();
}

void LayerBaCayAvatar::SetMapPlayer(std::string& listplayers) {
	if (0 == listplayers.compare("")) return;
	int vt = -1;

	vector<string> list = mUtils::splitString(listplayers, ';');
	for (unsigned int i = 0; i < list.size(); ++i) {
		try{
			vector<string> info = mUtils::splitString(list[i], '|');
			if (0 == info.size()) continue;
			if (0 == info[0].compare(this->MyAI)) {
				this->b_IsSpect = false;
				vt = i;
				break;
			}
		}
		catch (std::out_of_range& e){

		}
		catch (std::exception& e){

		}
		catch (...){

		}

	}

	//! neu vt == -1 thi minh la khach
	if (-1 == vt) {
		this->b_IsSpect = true;
		vt = 0;
	}

	this->mapPlayers.clear();
	for (unsigned int i = 0; i < list.size(); ++i) {
		if (0 == list[i].compare("")) continue;
		vector<string> info = mUtils::splitString(list[i], '|');
		if (0 == i) {
			try{
				this->b_IsPlaying = (0 == info[info.size() - 2].compare("1"));
				log("is playing %s", b_IsPlaying ? "true" : "false");
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		if (i == vt) {
			try{
				this->mapPlayers[info[0]] = kuser0;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 1) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser1;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 2) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser2;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 3) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser3;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 4) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser4;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 5) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser5;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
		else if (i == (vt + 6) % 7) {
			try{
				this->mapPlayers[info[0]] = kuser6;
			}
			catch (std::out_of_range& e){

			}
			catch (std::exception& e){

			}
			catch (...){

			}

		}
	}
}

Avatar52La* LayerBaCayAvatar::GetUserByPos(const int& pos) {
	if (pos == -1)
		return NULL;
	if (this->getChildByTag(pos) == NULL) return NULL;
	return (Avatar52La*) this->getChildByTag(pos);
}

void LayerBaCayAvatar::RunTimer(const int& PosPlayer) {
	this->StopAllTimer();
	if (Avatar52La* user = this->GetUserByPos(PosPlayer)) {
		user->startTimer();
	}
}

void LayerBaCayAvatar::StopAllTimer() {
	this->GetUserByPos(kuser0)->stopTimer();
	this->GetUserByPos(kuser1)->stopTimer();
	this->GetUserByPos(kuser2)->stopTimer();
	this->GetUserByPos(kuser3)->stopTimer();
	this->GetUserByPos(kuser4)->stopTimer();
	this->GetUserByPos(kuser5)->stopTimer();
	this->GetUserByPos(kuser6)->stopTimer();
}

bool LayerBaCayAvatar::IsSpect() {
	if (this->ListUsers.compare("") == 0)
		return false;
	auto arr = mUtils::splitString(this->ListUsers, ';');
	auto size = arr.size();
	for (int i = 0; i < size; i++) {
		auto arrInfo = mUtils::splitString(arr[i], '|');
		if (arrInfo.size() < 2) continue;
		auto name = arrInfo[0];
		if (name == this->MyAI) return false;
	}

	return true;
}

bool LayerBaCayAvatar::IsStartedGame() {
	boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
	if (room != NULL) {
		boost::shared_ptr<RoomVariable> roomVars = room->GetVariable("params");
		if (roomVars != NULL) {
			string s = *roomVars->GetStringValue();
			vector<string> lstBet = mUtils::splitString(s, '@');
			if (lstBet.size() < 2) return false;
			return (0 == lstBet[1].compare("1"));
		}
	}
	return false;
}

bool LayerBaCayAvatar::IsPlaying(string listUser) {
	if (listUser.compare("") == 0)
		return false;

	this->ListUsers = listUser;
	vector<string> arr = mUtils::splitString(this->ListUsers, ';');
	if (arr.size() > 0){
		vector<string> arrUser = mUtils::splitString(arr[0], '|');
		if (arrUser.size() > 0){
			if (0 == arrUser.at(1).compare("1"))
				return true;
			return false;
		}
	}
}

std::map<std::string, int>& LayerBaCayAvatar::GetMapPlayer() {
	return this->mapPlayers;
}
void LayerBaCayAvatar::RunAllTimer(){
	vector<string> arrUser = mUtils::splitString(ListUsers, ';');
	if (arrUser.size() == 0)
		return;

	log("arr user length :%d", arrUser.size());

	this->StopAllTimer();
	for (int i = 0; i < arrUser.size(); ++i) {
		vector<string> info = mUtils::splitString(ListUsers, ';');

		// Số lượng thông tin được bóc ra từ 1 arrUsers
		auto CountInfo = info.size();
		auto lstInfo = mUtils::splitString(info.at(i), '|');
		auto pos = this->GetPosByName(lstInfo.at(0));

		auto avatar = this->GetUserByPos(pos);
		if (avatar == NULL) return;
		avatar->startTimer(15);
	}

}
void LayerBaCayAvatar::setListUserGuess(string _list, char charSplit)
{
	vector<string> arrCurrUser = mUtils::splitString(ListUsers, ';');

	int count = arrCurrUser.size();
	for (int i = 0; i < count; i++)
	{
		vector<string> arrUser = mUtils::splitString(arrCurrUser[i], charSplit);
		auto pos = GetPosByName(arrUser[0]);
		auto avatar = GetUserByPos(pos);
		if (avatar)
			avatar->startTimer();
	}
	if (count >= 7 || strcmp(_list.c_str(), "") == 0) return;
	vector<string> arrUser = mUtils::splitString(_list, charSplit);
	//this->setGuessCount(arrUser.size());
	int length = arrUser.size() > (7 - count) ? 7 - count : arrUser.size();
	if (length > 0){
		this->resetGuessUser();
	}
	for (int i = 0; i < length; i++)
	{
		vector<string> info = mUtils::splitString(arrUser.at(i), '|');
		if (info.size() >1)
		{
			string userName = info.at(0);
			Avatar52La *userFree = this->getEmptyAvatar();
			if (userFree != NULL)
			{
				int tag = userFree->getTag();
				this->showAvatarMe(tag, userName);
				userFree->setVisbleLabelGuess(true);
				userFree->hideFrameBet(true);
			}
		}
	}
}
void LayerBaCayAvatar::showAvatarMe(int tag, string userName)
{
	Avatar52La *avaMe = GetUserByPos(tag);
	if (avaMe->getUserName().size() > 0)//da co thong tin
		return;

	boost::shared_ptr<User> userInfo = GameServer::getSingleton().getSmartFox()->UserManager()->GetUserByName(userName);

	if (userInfo == NULL){
		return;
	}

	// url
	boost::shared_ptr<UserVariable> url = userInfo->GetVariable("aal");
	// Money

	boost::shared_ptr<UserVariable> amf;
	if (SceneManager::getSingleton().getCurrRoomType() == 0){
		amf = userInfo->GetVariable("amf");
	}
	else{
		amf = userInfo->GetVariable("amfs");

	}
	// account ID
	if (url != NULL && amf != NULL)
	{
		avaMe->hideLayerInvite();
		avaMe->setIcon(url->GetStringValue()->c_str());
		avaMe->setAI(userName);
		if (SceneManager::getSingleton().getGameID() == kGameLieng)
		{
			boost::shared_ptr<UserVariable> gameChip_ptr = userInfo->GetVariable(EXT_FIELD_GAME_CHIP);
			if (gameChip_ptr != NULL){
				boost::shared_ptr<double> money = gameChip_ptr->GetDoubleValue();
				if (money != NULL)
					avaMe->setMoney(*money);
			}
			else{
				log("Can't get EXT_FIELD_GAME_CHIP");
			}
		}
		else{
			avaMe->setMoney(*amf->GetDoubleValue());
		}
		/*moc3-change aN=>adn*/
		boost::shared_ptr<UserVariable> aN = userInfo->GetVariable("adn");
		if (aN != NULL){
			if (strcmp(aN->GetStringValue()->c_str(), "") == 0)
				avaMe->setName(userName);
			else
				avaMe->setName(aN->GetStringValue()->c_str());
		}
		else{
			avaMe->setName(userName);
		}

	}

}

Avatar52La* LayerBaCayAvatar::getEmptyAvatar()
{
	for (int i = 6; i >= 1; i--){
		Avatar52La* user = this->GetUserByPos(i);
		if (user == NULL) continue;
		if (strcmp(user->getAI().c_str(), "") == 0)
			return user;
	}
	return NULL;
}

void LayerBaCayAvatar::resetGuessUser()
{
	for (int i = 0; i < 7; i++)
	{
		Avatar52La *user = this->GetUserByPos(i);
		if (user == NULL) continue;
		if (user->isChauRia())
		{
			user->setVisibleLayerInvite(true);
			user->setName("");
			user->setMoney("");
			user->setAI("");
		}
	}

}
void LayerBaCayAvatar::checkSameIP()
{
	for (int i = 0; i < arrAI.size(); i++)
	{
		for (int k = i + 1; k < arrAI.size(); k++)
		{
			int pos1 = this->GetPosByName(arrAI.at(i));
			int pos2 = this->GetPosByName(arrAI.at(k));
			Avatar52La* user1 = GetUserByPos(pos1);
			Avatar52La* user2 = GetUserByPos(pos2);
			if (user1 && user2)
			{
				string username1 = user1->getAI();
				string username2 = user2->getAI();
				if (0 != username1.compare(username2))
				{
					string ip1 = user1->getUserIP();
					string ip2 = user2->getUserIP();
					if (0 == ip1.compare(ip2)
						&&
						0 != ip1.compare("")
						&&
						0 != ip1.compare("undefined")
						&&
						0 != ip2.compare("")
						&&
						0 != ip2.compare("undefined")
						)
					{
						user1->showSameIP(true);
						user2->showSameIP(true);
					}
				}
			}
		}
	}
}

void LayerBaCayAvatar::hideAllBet(bool isHide)
{
	this->GetUserByPos(kuser0)->hideFrameBet(isHide);
	this->GetUserByPos(kuser1)->hideFrameBet(isHide);
	this->GetUserByPos(kuser2)->hideFrameBet(isHide);
	this->GetUserByPos(kuser3)->hideFrameBet(isHide);
	this->GetUserByPos(kuser4)->hideFrameBet(isHide);
	this->GetUserByPos(kuser5)->hideFrameBet(isHide);
	this->GetUserByPos(kuser6)->hideFrameBet(isHide);
}

void LayerBaCayAvatar::resetAllBets()
{
	this->GetUserByPos(kuser0)->setBetValue(0);
	this->GetUserByPos(kuser1)->setBetValue(0);
	this->GetUserByPos(kuser2)->setBetValue(0);
	this->GetUserByPos(kuser3)->setBetValue(0);
	this->GetUserByPos(kuser4)->setBetValue(0);
	this->GetUserByPos(kuser5)->setBetValue(0);
	this->GetUserByPos(kuser6)->setBetValue(0);

	this->GetUserByPos(kuser0)->hideFrameResult();
	this->GetUserByPos(kuser1)->hideFrameResult();
	this->GetUserByPos(kuser2)->hideFrameResult();
	this->GetUserByPos(kuser3)->hideFrameResult();
	this->GetUserByPos(kuser4)->hideFrameResult();
	this->GetUserByPos(kuser5)->hideFrameResult();
	this->GetUserByPos(kuser6)->hideFrameResult();
}

void LayerBaCayAvatar::setBetInfo(double bet, int pos)
{
	Avatar52La* user = this->GetUserByPos(pos);
	if (user != NULL)
	{
		//user->setBetValue(bet);
		user->runActionBet(bet);
	}
}

void LayerBaCayAvatar::removeResult()
{
	this->GetUserByPos(kuser0)->removeResultBaCay();
	this->GetUserByPos(kuser1)->removeResultBaCay();
	this->GetUserByPos(kuser2)->removeResultBaCay();
	this->GetUserByPos(kuser3)->removeResultBaCay();
	this->GetUserByPos(kuser4)->removeResultBaCay();
	this->GetUserByPos(kuser5)->removeResultBaCay();
	this->GetUserByPos(kuser6)->removeResultBaCay();
}

ImageView* LayerBaCayAvatar::getImageChuong()
{
	return this->Chuong;
}

void LayerBaCayAvatar::setPosBien(int _pos, std::string _valueBien)
{

	this->imgBienActive = ui::ImageView::create("moc3-cuoc-bien-success.png");
	this->imgBienActive->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->imgBienActive->setVisible(false);
	//this->addChild(this->imgBienActive);

	this->lblValueBien = Label::createWithTTF("", "__Roboto-Regular.ttf", 20);
	this->lblValueBien->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->imgBienActive->addChild(this->lblValueBien);
	this->lblValueBien->setPosition(Point(this->imgBienActive->getContentSize().width / 2,
		this->imgBienActive->getContentSize().height / 2));

	switch (_pos) {
	case kuser1:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser1)->getPositionX() + W_MONEY_BG / 2 - 25.0, this->GetUserByPos(kuser1)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser1 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser2:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser2)->getPositionX() + W_MONEY_BG / 2 - 25.0, this->GetUserByPos(kuser2)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser2 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser3:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser3)->getPositionX() + W_MONEY_BG / 2 - 25.0, this->GetUserByPos(kuser3)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser3 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser4:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser4)->getPositionX() - W_MONEY_BG / 2 + 25.0, this->GetUserByPos(kuser4)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser4 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser5:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser5)->getPositionX() - W_MONEY_BG / 2 + 25.0, this->GetUserByPos(kuser5)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser5 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser6:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser6)->getPositionX() - W_MONEY_BG / 2 + 25.0, this->GetUserByPos(kuser6)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser6 + 100);
		this->addChild(imgBienActive);
		break;
	case kuser0:
		this->imgBienActive->setPosition(Point(this->GetUserByPos(kuser0)->getPositionX() - W_MONEY_BG / 2 + 25.0, this->GetUserByPos(kuser0)->getPositionY() + H_MONEY_BG / 2 - 25.0));
		this->lblValueBien->setString(_valueBien);
		this->imgBienActive->setVisible(true);
		this->imgBienActive->setTag(kuser0 + 100);
		//this->addChild(imgBienActive);
		break;
	default:
		break;
	}
}

void LayerBaCayAvatar::resetBienButton()
{
	if (this->getChildByTag(kuser0 + 100)){
		this->removeChildByTag(kuser0 + 100);
	}
	if (this->getChildByTag(kuser1 + 100)){
		this->removeChildByTag(kuser1 + 100);
	}
	if (this->getChildByTag(kuser2 + 100)){
		this->removeChildByTag(kuser2 + 100);
	}
	if (this->getChildByTag(kuser3 + 100)){
		this->removeChildByTag(kuser3 + 100);
	}
	if (this->getChildByTag(kuser4 + 100)){
		this->removeChildByTag(kuser4 + 100);
	}
	if (this->getChildByTag(kuser5 + 100)){
		this->removeChildByTag(kuser5 + 100);
	}
	if (this->getChildByTag(kuser6 + 100)){
		this->removeChildByTag(kuser6 + 100);
	}
}
