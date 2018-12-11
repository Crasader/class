//
//  LayerMenuNhiemVuInGame.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 4/23/16.
//
//

#include "LayerMenuNhiemVuInGame.h"
#include "../layergames/_Button_inGame_.h"
#include "LayerPopupMission.h"
#include "../mUtils.h"
#include "../data_manager.h"
#include "../SceneManager.h"
#include "../AllData.h"
#include "../layergames/PhomMessDef.h"
LayerMenuNhiemVuInGame::LayerMenuNhiemVuInGame()
{
	currGameForRqot = 0;
	currGameForRqdt = 0;
	GameServer::getSingleton().addListeners(this);
}


LayerMenuNhiemVuInGame::~LayerMenuNhiemVuInGame()
{
	GameServer::getSingleton().removeListeners(this);
	this->lstTaskDone.clear();
}

bool LayerMenuNhiemVuInGame::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto button = Button::create();
	button->setAnchorPoint(Vec2(0, 1));
	button->loadTextures("moc3-xito-btn-add-money.png", "", "");
	button->addClickEventListener(CC_CALLBACK_1(LayerMenuNhiemVuInGame::onButtonClicked, this));
	button->setPressedActionEnabled(true);
	button->setPositionX(10);
	this->addChild(button);
    
    if (SceneManager::getSingleton().getGameID() == kGameChan){
        button->loadTextures("moc3-xito-btn-add-money.png.png", "", "");

    }

	this->bg = Sprite::create("task-notifications.png");
	button->addChild(bg);
	this->bg->setPosition(Vec2(120, 120));

	//    labelNumber = Label::createWithTTF("1", "__Roboto-Bold.ttf", 25);
	//    this->bg->addChild(labelNumber);
	//    labelNumber->setAnchorPoint(Vec2(0.5, 0.5));
	//    labelNumber->setPosition(Vec2(bg->getContentSize().width / 2-2, bg->getContentSize().height / 2-1));
	this->bg->setVisible(false);
	this->setContentSize(button->getContentSize());
	this->isFirst = true;

	if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia || SceneManager::getSingleton().getGameID() == kGameQuayThuong) {
		return true;
	}

	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rldot", params));
	GameServer::getSingleton().Send(request);

	boost::shared_ptr<ISFSObject> params1(new SFSObject());
	boost::shared_ptr<IRequest> request1(new ExtensionRequest("rldt", params1));
	GameServer::getSingleton().Send(request1);


	return true;
}
void LayerMenuNhiemVuInGame::onButtonClicked(Ref* pSender){
	LayerButtonInGame* parent = (LayerButtonInGame*)this->getParent();
	if (parent){
		bg->setVisible(false);
		this->isFirst = true;
		LayerPopupMission* mission = LayerPopupMission::create();
		parent->addChild(mission, 1000);
		if (this->showTab == 1)
		{
			mission->onBtnTabNhiemVuNgay(NULL);
			return;
		}
		else{
			mission->onBtnTabNhiemVu(NULL);
			return;
		}

	}

}
void LayerMenuNhiemVuInGame::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (strcmp("e", cmd->c_str()) == 0 || strcmp("endntf", cmd->c_str()) == 0){
		if (SceneManager::getSingleton().getGameID() == kGameTaiXiu || SceneManager::getSingleton().getGameID() == kGameXocDia || SceneManager::getSingleton().getGameID() == kGameQuayThuong) {
			return;
		}
		isFirst = false;
		currGameForRqot += 1;
		currGameForRqdt += 1;

		if (currGameForRqot >= dataManager.getAppConfig().delay_reqot){
			boost::shared_ptr<ISFSObject> params(new SFSObject());
			boost::shared_ptr<IRequest> request(new ExtensionRequest("rldot", params));
			GameServer::getSingleton().Send(request);
			currGameForRqot = 0;
		}
		if (currGameForRqdt >= dataManager.getAppConfig().delay_reqdt){
			boost::shared_ptr<ISFSObject> params1(new SFSObject());
			boost::shared_ptr<IRequest> request1(new ExtensionRequest("rldt", params1));
			GameServer::getSingleton().Send(request1);
			currGameForRqdt = 0;
		}
	}

	else if (strcmp("rsldt", cmd->c_str()) == 0){

		if (SceneManager::getSingleton().getGameVersion() == GameVersion::p1){
			string strlistTask = *param->GetUtfString("listtask");
			auto lst = mUtils::splitString(strlistTask, '-');
			//1|Hoàn thành .... ván game bất kỳ|4|8|12|16|0|4|0|16|0
			//id|name|socap|khoinghiep|trieuphu|daigia|capdohientai|tongcapdo|tiendo|tongtiendo|diem
			if (lst.size() < 3)
				return;
			string strTask = lst[0];
			auto lstTask = mUtils::splitString(strTask, ';');
			if (this->isFirst){
				this->lstdone.clear();
				for (int i = 0; i < lstTask.size(); i++){
					auto itemTask = mUtils::splitString(lstTask[i], '|');
					if (itemTask.size() < 11)
						return;
					task tk;
					tk.idt = atoi(itemTask[0].c_str());
					tk.lvl = atoi(itemTask[6].c_str());

					this->lstdone.push_back(tk);
				}
			}
			else{
				for (int i = 0; i < lstTask.size(); i++){
					auto itemTask = mUtils::splitString(lstTask[i], '|');
					if (itemTask.size() < 11)
						return;
					bool isShould = true;
					for (int k = 0; k < this->lstdone.size(); k++){
						if (atoi(itemTask[6].c_str()) == this->lstdone[k].lvl)
							isShould = false;
					}
					if (isShould){
						this->notiTaskDone();
						this->showTab = 1;
					}
				}
			}
		}
		else{
			if (param->GetUtfString("listtask") == NULL) return;
			string strlistTask = *param->GetUtfString("listtask");
			auto listTask = mUtils::splitString(strlistTask, ';');

			if (this->isFirst){
				for (int i = 0; i < listTask.size(); i++){
					string lstItemTask = listTask[i];
					auto listItemTask = mUtils::splitString(lstItemTask, '|');
					if (isTaskDone(listItemTask[4])){
						bool isShould = true;
						for (int k = 0; k < this->lstTaskDone.size(); k++){
							if (std::strcmp(listItemTask[0].c_str(), lstTaskDone[k].c_str()) == 0)
								isShould = false;
						}
						if (isShould)
							this->lstTaskDone.push_back(listItemTask[0]);
					}
				}
			}
			else{
				for (int i = 0; i < listTask.size(); i++){
					string lstItemTask = listTask[i];
					auto listItemTask = mUtils::splitString(lstItemTask, '|');
					if (isTaskDone(listItemTask[4])){
						bool isShould = true;
						for (int k = 0; k < this->lstTaskDone.size(); k++){
							if (std::strcmp(listItemTask[0].c_str(), lstTaskDone[k].c_str()) == 0)
								isShould = false;
						}
						if (isShould){
							this->notiTaskDone();
							this->showTab = 1;
						}
					}
				}
			}

		}
	}
	else if (strcmp("rsldot", cmd->c_str()) == 0){

		if (param->GetUtfString("listtask") == NULL) return;
		string strlistTask = *param->GetUtfString("listtask");
		log("%s", strlistTask.c_str());
		auto listTask = mUtils::splitString(strlistTask, ';');
		if (this->isFirst){
			for (int i = 0; i < listTask.size(); i++){
				string lstItemTask = listTask[i];
				auto listItemTask = mUtils::splitString(lstItemTask, '|');
				if (atoi(listItemTask[0].c_str()) == 5)
					continue;

				if (isTaskDone(listItemTask[4])){
					bool isShould = true;
					for (int k = 0; k < this->lstTaskDone.size(); k++){
						if (std::strcmp(listItemTask[0].c_str(), lstTaskDone[k].c_str()) == 0)
							isShould = false;
					}
					if (isShould)
						this->lstTaskDone.push_back(listItemTask[0]);
				}
			}
		}
		else{
			for (int i = 0; i < listTask.size(); i++){
				string lstItemTask = listTask[i];
				auto listItemTask = mUtils::splitString(lstItemTask, '|');
				if (atoi(listItemTask[0].c_str()) == 5)
					continue;

				if (isTaskDone(listItemTask[4])){
					bool isShould = true;
					for (int k = 0; k < this->lstTaskDone.size(); k++){
						if (std::strcmp(listItemTask[0].c_str(), lstTaskDone[k].c_str()) == 0)
							isShould = false;
					}
					if (isShould){
						this->notiTaskDone();
						this->showTab = 2;
					}
				}
			}
		}
	}
}
bool LayerMenuNhiemVuInGame::isTaskDone(string status){
	auto lstStatus = mUtils::splitString(status, '/');
	if (lstStatus.size() > 1){
		float done = atof(lstStatus[0].c_str());
		float to = atof(lstStatus[1].c_str());
		if (done == to)
			return true;
		else
			return false;
	}
	else{
		if (atof(lstStatus[0].c_str()) == 1)
			return true;
		else
			return false;
	}

}
void LayerMenuNhiemVuInGame::notiTaskDone(){
	this->bg->setVisible(true);
	this->bg->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.8, 1.4), ScaleTo::create(0.8, 1.0), NULL)));
}
