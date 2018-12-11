//
//  LayerPopupTimBan.cpp
//  iCasino_v4
//
//  Created by NoBody aka HoangDD on 7/13/16.
//
//

#include "LayerPopupTimBan.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "ChanUtils.h"
#include "../Common.h"
#include "../SceneManager.h"
#include "../mUtils.h"
#include "PhomMessDef.h"
#include "Requests/ExtensionRequest.h"
#include "boost/smart_ptr/make_shared_object.hpp"
#include "../cotuong/gameUtils.h"
#include "Requests/JoinRoomRequest.h"
#include "../scenes/SceneMain.h"
LayerPopupTimBan::LayerPopupTimBan()
{
    GameServer::getSingleton().addListeners(this);
}


LayerPopupTimBan::~LayerPopupTimBan()
{
    GameServer::getSingleton().removeListeners(this);
    
}

bool LayerPopupTimBan::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto distance = (visibleSize.height - DESIGN_RESOLUTION_HEIGHT) / 2;
    auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
    
    auto rootNode = CSLoader::getInstance()->createNode("LayerPopupTimBan.csb");
    auto pnlB = static_cast<Layout*>(rootNode->getChildByName("Panel_3"));
    
    pnlB->setScale(visibleSize.height / DESIGN_RESOLUTION_HEIGHT);
    rootNode->setAnchorPoint(Point(0.5, 0.5));
    rootNode->setPosition(Point(visibleSize / 2));
    Size sizeAdd;
//    if (SceneManager::getSingleton().getGameID() != kGameChan && SceneManager::getSingleton().getGameID() != kGameXiTo){
        sizeAdd = ChanUtils::getSizePos();
        
//    }else{
//        sizeAdd = Size(0,0);
//    }
    Size sizeAdd2 = Size(0,0);
    rootNode->setPosition(Point(visibleSize.width/2,visibleSize.height/2-sizeAdd2.height));
    
    ui::Helper::doLayout(rootNode);
    
    this->addChild(rootNode);
    
    auto pnlBg = static_cast<Layout*>(rootNode->getChildByName("Panel_2"));
    auto lblTitle = static_cast<Text*> (pnlBg->getChildByName("Text_1"));
    lblTitle->setString(dataManager.GetSysString(871));
    auto lblLabel = static_cast<Text*> (pnlBg->getChildByName("txtLabel_0"));
    lblLabel->setString(dataManager.GetSysString(872));
    Button* btnClose = dynamic_cast<Button*>(pnlBg->getChildByName("btnClose"));
    if(btnClose){
        btnClose->addClickEventListener(CC_CALLBACK_1(LayerPopupTimBan::onBtnClose, this));
    }

    
    Button* btnTimBan = dynamic_cast<Button*>(pnlBg->getChildByName("btnTimBan"));
    if(btnTimBan){
        btnTimBan->addClickEventListener(CC_CALLBACK_1(LayerPopupTimBan::onBtnTimBan, this));
        btnTimBan->setTitleText(dataManager.GetSysString(863));

    }
    
    this->tfBanChoi = dynamic_cast<TextField*>(pnlBg->getChildByName("tfBanChoi"));
    this->tfBanChoi->setPlaceHolder(dataManager.GetSysString(873));

    this->tfBanChoi->setInputMode(ui::EditBox::InputMode::NUMERIC);
    this->tfBanChoi->setMaxLength(4);
    this->tfMatKhau = dynamic_cast<TextField*>(pnlBg->getChildByName("tfMatKhau"));
    this->tfMatKhau->setPlaceHolder(dataManager.GetSysString(633));
    
    this->imgMatKhau = dynamic_cast<ImageView*>(pnlBg->getChildByName("imgMatKhau"));
    
//    this->tfMatKhau->setVisible(false);
//    this->imgMatKhau->setVisible(false);

    this->setScale(1);
    //this->runAction(Sequence::create(ScaleTo::create(0.2, 1.1),ScaleTo::create(0.2, 1),NULL));
    return true;
}


void LayerPopupTimBan::onBtnTimBan(Ref* pSender){
    
    
    
    if (this->tfBanChoi->getString().length() == 0){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(128));
        return;
    }
    
    if (mUtils::findContainChar(this->tfBanChoi->getString().c_str())){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(560));
        return;
    }
    
    string mk = this->tfMatKhau->getString();
    if (mk == ""){
        SceneManager::getSingleton().showToast(dataManager.GetSysString(129));
        return;
    }
    int idRoom = atoi(this->tfBanChoi->getString().c_str());
    int gameID = SceneManager::getSingleton().getGameID();
    string strGameID = StringUtils::format("%d", gameID).substr(0, 3);
    string sRoomName = StringUtils::format("P%d_%s_%d",SceneManager::getSingleton().getCurrRoomType()  ,strGameID.c_str(), idRoom);
    //SamSoloP1_117_17
    boost::shared_ptr<Room>  roomPtr = GameServer::getSingleton().getSmartFox()->GetRoomByName(sRoomName);
    if (roomPtr)
    {
        if (roomPtr->IsPasswordProtected())
        {
            string mk = this->tfMatKhau->getString();
            if (mk == ""){
                SceneManager::getSingleton().showToast(dataManager.GetSysString(130));
                return;
            }
//            this->tfMatKhau->setVisible(true);
//            this->imgMatKhau->setVisible(true);
            
            int a = -1;
            boost::shared_ptr<RoomVariable> rv = roomPtr->GetVariable("params");
            vector<string> lstParams = mUtils::splitString(*rv->GetStringValue(), '@');
            boost::shared_ptr<long int> id2Left = boost::make_shared<long int>(a);
            int currPlayers = atoi(lstParams.at(2).c_str());
            bool isPlaying = (lstParams.at(1).compare("1") == 0);
            int numOfPlayers = atoi(boost::to_string(roomPtr->MaxUsers()).c_str());
            int realRoomID = roomPtr->Id();
            
            bool isSpectator = (currPlayers == numOfPlayers);
            if (!isSpectator)
                isSpectator = isPlaying;
            
            //update 12/7
            if (currPlayers < numOfPlayers)
            {
                int roomBetVal = 0;
                bool allowView = false;
                if (lstParams.size() >= 1)
                {
                    roomBetVal = atoi(lstParams.at(0).c_str());
                }
                
                int roomType = 1;
                boost::shared_ptr<RoomVariable> typeRoomVar = roomPtr->GetVariable("roomType");
                if (typeRoomVar != NULL)
                {
                    roomType = *(typeRoomVar->GetIntValue());
                }
                double amfUser = SceneManager::getSingleton().getMoneyByType(SceneManager::getSingleton().getCurrRoomType());
                // get idroom to left
                
                double betUser = GameUtils::getMinBetByGame(SceneManager::getSingleton().getGameID(), amfUser);
                
                if (betUser < roomBetVal)
                {
                    SceneManager::getSingleton().showToast(dataManager.GetSysString(131));
                    SceneManager::getSingleton().showPopupMoneyCharge();
                    return;
                }
            }
            
            boost::shared_ptr<IRequest> request(new JoinRoomRequest(realRoomID, mk, id2Left, isSpectator));
            GameServer::getSingleton().Send(request);
            SceneMain::getSingleton().prepareToPickGame();

            //this->removeFromParent();
        }
        else{
            SceneManager::getSingleton().showToast(dataManager.GetSysString(132));

        }
    }
    else{
        SceneManager::getSingleton().showToast(dataManager.GetSysString(133));
//        this->tfMatKhau->setVisible(false);
//        this->imgMatKhau->setVisible(false);
    }

}
void LayerPopupTimBan::onBtnClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}
void LayerPopupTimBan::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    
    if (strcmp(EXT_EVENT_VQ_USER_HIS_RESP, cmd->c_str()) == 0){
        }
}

bool LayerPopupTimBan::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerPopupTimBan::onEnter()
{
    Layer::onEnter();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerPopupTimBan::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
    //
    //    GameServer::getSingleton().addListeners(this);
}

void LayerPopupTimBan::onExit()
{
    Layer::onExit();
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    //    GameServer::getSingleton().removeListeners(this);
    //    Layer::onExit();
}
