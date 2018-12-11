#include "Layer_GivePocker_Chan.h"
#include "Requests/ExtensionRequest.h"
#include "mUtils.h"
#include "AllData.h"
#include "ChanUtils.h"

using namespace Sfs2X;

Layer_GivePocker_Chan::Layer_GivePocker_Chan()
    :Toast(NULL),
     uLayer(NULL),
     Noc(NULL),
     Noc2(NULL),
     B1(NULL),
     B2(NULL),
     B3(NULL),
     B4(NULL),
     B5(NULL),
     B6(NULL),
     B7(NULL),
     B8(NULL),
     B9(NULL),
     B10(NULL),
     Timer(NULL),
     m_callback(NULL),
     m_callbackListener(NULL),
     PlayerBocCai(""),
     PlayerStart(""),
     StartTimmer(45),
     flag_ChonNoc(false),
     IsPlayerBocCai(false),
     flagBocCai(false) {

}

bool Layer_GivePocker_Chan::init() {
    if ( !Layer::init() ) return false;

    this->uLayer = Layer::create();
    this->uLayer->setAnchorPoint(Vec2(0, 0));
    this->uLayer->setPosition(Vec2(0, 0));
    this->addChild(this->uLayer);

    //Bat dau o day
    this->CountLeft = 0;
    this->CountRight = 0;
    this->Index_Noc = -1;

    this->arrTag.push_back(9);

    this->c1_leftX = 600;
    this->c1_leftY = 600;

    this->c2_leftX = 675;
    this->c2_leftY = 450;

    this->c3_leftX = 750;
    this->c3_leftY = 300;

    this->c4_leftX = 325;
    this->c4_leftY = 370;

    this->c5_leftX = 450;
    this->c5_leftY = 220;

    this->c1_rightX = WIDTH_DESIGN - this->c1_leftX;
    this->c1_rightY = HEIGHT_DESIGN - this->c1_leftY;

    this->c2_rightX = WIDTH_DESIGN - this->c2_leftX;
    this->c2_rightY = HEIGHT_DESIGN - this->c2_leftY;

    this->c3_rightX = WIDTH_DESIGN - this->c3_leftX;
    this->c3_rightY = HEIGHT_DESIGN - this->c3_leftY;

    this->c4_rightX = WIDTH_DESIGN - this->c4_leftX;
    this->c4_rightY = HEIGHT_DESIGN - this->c4_leftY;

    this->c5_rightX = WIDTH_DESIGN - this->c5_leftX;
    this->c5_rightY = HEIGHT_DESIGN - this->c5_leftY;

    this->B1 = this->Create5Cards(this->c1_leftX, this->c1_leftY, 0);
    this->B2 = this->Create5Cards(this->c2_leftX, this->c2_leftY, 0);
    this->B3 = this->Create5Cards(this->c3_leftX, this->c3_leftY, 0);
    this->B4 = this->Create5Cards(this->c4_leftX, this->c4_leftY, 0);
    this->B5 = this->Create5Cards(this->c5_leftX, this->c5_leftY, 0);

    this->B6 = this->Create5Cards(this->c1_rightX, this->c1_rightY, 3);
    this->B7 = this->Create5Cards(this->c2_rightX, this->c2_rightY, 5);
    this->B8 = this->Create5Cards(this->c3_rightX, this->c3_rightY, 4);
    this->B9 = this->Create5Cards(this->c4_rightX, this->c4_rightY, 2);
    this->B10 = this->Create5Cards(this->c5_rightX, this->c5_rightY, 1);

    this->arrNocs.push_back(B10);
    this->arrNocs.push_back(B9);
    this->arrNocs.push_back(B6);
    this->arrNocs.push_back(B7);
    this->arrNocs.push_back(B8);
    //10-9-6-8-7

    this->uLayer->addChild(this->B1);
    this->uLayer->addChild(this->B2);
    this->uLayer->addChild(this->B3);
    this->uLayer->addChild(this->B4);
    this->uLayer->addChild(this->B5);
    this->uLayer->addChild(this->B6);
    this->uLayer->addChild(this->B7);
    this->uLayer->addChild(this->B8);
    this->uLayer->addChild(this->B9);
    this->uLayer->addChild(this->B10);

    this->InitAllCards();

    this->Noc = ImageView::create();
    this->Noc->loadTexture("card_back_chan.png", Widget::TextureResType::PLIST);
    this->Noc->setAnchorPoint(Vec2(0.5, 0.5));
    this->Noc->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
    this->Noc->setRotation(20);
    this->Noc->setScale(0.9);
    this->Noc->setRotation(-45);
    this->Noc->setTouchEnabled(true);
    this->Noc->addTouchEventListener(CC_CALLBACK_2(Layer_GivePocker_Chan::BocNoc, this));
    this->uLayer->addChild(this->Noc);

    this->Noc2 = ImageView::create();
    this->Noc2->loadTexture("card_back_chan.png", Widget::TextureResType::PLIST);
    this->Noc2->setAnchorPoint(Vec2(0.5, 0.5));
    this->Noc2->setPosition(Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
    this->Noc2->setRotation(20);
    this->Noc2->setScale(0.9);
    this->Noc2->setRotation(-45);
    this->Noc2->setTouchEnabled(true);
    this->Noc2->addTouchEventListener(CC_CALLBACK_2(Layer_GivePocker_Chan::BocNoc, this));
    this->uLayer->addChild(this->Noc2);

    this->Timer = Label::createWithBMFont(RobotoFnt, "45");
    this->Timer->setColor(Color3B::RED);
    this->Timer->setScale(1.7);
    this->Timer->setVisible(false);
    this->uLayer->addChild(this->Timer);

    GameServer::getSingleton().addListeners(this);
    return true;
}

Layer_GivePocker_Chan::~Layer_GivePocker_Chan() {
    GameServer::getSingleton().removeListeners(this);
    this->stopAllActions();
    this->arrNocs.clear();
    this->removeAllChildrenWithCleanup(true);
}

void Layer_GivePocker_Chan::onExit() {
    GameServer::getSingleton().removeListeners(this);
    this->stopAllActions();
    this->removeAllChildrenWithCleanup(true);
    Layer::onExit();
}

void Layer_GivePocker_Chan::SetMapPlayer(const std::map<std::string, int>& mapp) {
    this->mapPlayers = mapp;
}

void Layer_GivePocker_Chan::SetIsPlayerBocCai(const bool& isFlag) {
    this->IsPlayerBocCai = isFlag;
}

void Layer_GivePocker_Chan::SetPlayerBocCai(const string& player) {
    this->PlayerBocCai = player;
}

ImageView* Layer_GivePocker_Chan::Create5Cards(const float& posX, const float& posY, const int& mTag) {
    ImageView *img = ImageView::create();
    img->loadTexture("image_transparent.png", Widget::TextureResType::PLIST);
    img->setAnchorPoint(Vec2(0.5, 0.5));
    img->setPosition(Vec2(posX, posY));
    img->setScale(0.9);
    img->setTag(mTag);
    img->setTouchEnabled(true);
    img->addTouchEventListener(CC_CALLBACK_2(Layer_GivePocker_Chan::NocTouch, this));
    img->setRotation(16.0);
    return img;
}

void Layer_GivePocker_Chan::NocTouch(Ref *pSender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        if(!this->IsPlayerBocCai)
            return;
        if (this->flagBocCai) {
            ImageView *img = (ImageView*)pSender;
            if (img != NULL) {
                this->B1->setVisible(false);
                this->B2->setVisible(false);
                this->B3->setVisible(false);
                this->B4->setVisible(false);
                this->B5->setVisible(false);
               // log("Tag = %d", img->getTag());
                this->Noc2->setVisible(false);

                if (!this->flag_ChonNoc) {
                    this->Index_Noc = img->getTag();
                    img->runAction(MoveTo::create(0.3, Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1)));
                    this->flag_ChonNoc = true;
                }

                else
                {
                    this->flagBocCai = false;
                    CallFunc *callfun = CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::SendRequsetBocNoc, this));
                    MoveTo *moveTo = MoveTo::create(0.5, img->getPosition());
                    this->Index_Choose = img->getTag();
                    this->Noc->runAction(Sequence::create(moveTo, callfun, NULL));
                }
            }
        }
    }
}

void Layer_GivePocker_Chan::SendRequsetBocNoc() {
    boost::shared_ptr<ISFSObject> params (new SFSObject());
    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (lastRoom == NULL)
        return;
    boost::shared_ptr<IRequest> request(new ExtensionRequest(EXT_EVENT_REQ_BOC_CAI, params, lastRoom));
    GameServer::getSingleton().Send(request);
   // log("Send Request Bốc Cái !");
}

void Layer_GivePocker_Chan::BocNoc(Ref *pSender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED)
        if (this->flagBocCai)
            this->Noc2->setVisible(false);
}

void Layer_GivePocker_Chan::InitAllCards() {
    this->AddVirtualCards(this->B1);
    this->AddVirtualCards(this->B2);
    this->AddVirtualCards(this->B3);
    this->AddVirtualCards(this->B4);
    this->AddVirtualCards(this->B5);
    this->AddVirtualCards(this->B6);
    this->AddVirtualCards(this->B7);
    this->AddVirtualCards(this->B8);
    this->AddVirtualCards(this->B9);
    this->AddVirtualCards(this->B10);
}

void Layer_GivePocker_Chan::AddVirtualCards(ImageView *img) {
    for (unsigned short i = 0; i < 5; ++i) {
        ImageView *pCards = ImageView::create();
        pCards->setAnchorPoint(Vec2(0.5, 0.5));
        pCards->loadTexture("card_back_chan.png", Widget::TextureResType::PLIST);
        int Rote = rand() % (15) - 10;
        pCards->setRotation((float)Rote);
        pCards->setPosition(Vec2(img->getContentSize().width / 2, img->getContentSize().height / 2));
        pCards->setTag(i);
        pCards->setVisible(false);
        img->addChild(pCards);
    }
}

void Layer_GivePocker_Chan::StartGivePocker() {
    this->Noc->setRotation(20);
    this->GivePocker(0);
    this->runAction(Sequence::create(
                        DelayTime::create(0.4),
                        CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::DelayTimeturn2, this)),
                        NULL));
}

void Layer_GivePocker_Chan::DelayTimeturn2() {
    this->Noc2->setRotation(20);
    this->GivePocker(1);
}

void Layer_GivePocker_Chan::GivePocker(const int& Pos) {
    //ben trai
    if (0 == Pos) {
        int z = this->CountLeft / 5;
        if (z <= 4) {
            int t = this->CountLeft % 5;
            switch(t) {
            case 0:
                this->GiveToPosition(c1_leftX, c1_leftY, 0);
                break;
            case 1:
                this->GiveToPosition(c2_leftX, c2_leftY, 0);
                break;
            case 2:
                this->GiveToPosition(c3_leftX, c3_leftY, 0);
                break;
            case 3:
                this->GiveToPosition(c4_leftX, c4_leftY, 0);
                break;
            case 4:
                this->GiveToPosition(c5_leftX, c5_leftY, 0);
                break;
            default:
                break;
            }
        }
        return;
    }

    //ben phai
    if (Pos == 1) {
        int z = this->CountRight / 5;
        if (z <= 4) {
            int t = this->CountRight % 5;
            switch(t) {
            case 0:
                this->GiveToPosition(c1_rightX, c1_rightY, 1);
                break;
            case 1:
                this->GiveToPosition(c2_rightX, c2_rightY, 1);
                break;
            case 2:
                this->GiveToPosition(c3_rightX, c3_rightY, 1);
                break;
            case 3:
                this->GiveToPosition(c4_rightX, c4_rightY, 1);
                break;
            case 4:
                this->GiveToPosition(c5_rightX, c5_rightY, 1);
                break;
            default:
                break;
            }
        }
        return;
    }
}

void Layer_GivePocker_Chan::GiveToPosition(const float& toX, const float& toY, const int& pos) {
    int *value = new int(pos);
    CallFunc *callfun = CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::test, this, Noc, pos));
    ActionInterval *moveTo = MoveTo::create(0.2, Vec2(toX, toY));
    if (0 == pos) {
        this->CountLeft++;
        if (this->CountLeft < 25) {
            this->Noc->runAction(Sequence::create(DelayTime::create(0.1), moveTo, callfun, NULL));
        } else {
            MoveTo *moveTo = MoveTo::create(0.3, Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
            this->Noc->runAction(moveTo);
        }
    } else {
        this->CountRight++;
        if (this->CountRight < 25) {
            Noc2->runAction(Sequence::create(DelayTime::create(0.1), moveTo, callfun, NULL));
        } else {
            MoveTo *moveTo = MoveTo::create(0.3, Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN >> 1));
            Noc2->runAction(moveTo);

            B10->runAction(MoveTo::create(0.5, B1->getPosition()));
            B9->runAction(MoveTo::create(0.5, B2->getPosition()));

            CallFunc *callfunc = CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::DelayTimeEnd, this));
            this->runAction(Sequence::create(DelayTime::create(0.5), callfunc, NULL));
        }
    }
}

void Layer_GivePocker_Chan::DelayTimeEnd() {
    //Chuyen 4 -> 8, 5 - 7, 3 - 6
    this->B4->runAction(MoveTo::create(0.5, this->B8->getPosition()));
    this->B5->runAction(MoveTo::create(0.5, this->B7->getPosition()));
    this->B3->runAction(MoveTo::create(0.5, this->B6->getPosition()));
    this->flagBocCai = true;

    string StrToast = "";
    if (this->IsPlayerBocCai)
        StrToast = dataManager.GetSysString(63);
    else
        StrToast = dataManager.GetSysString(64)+" " + ChanUtils::GetNamePlayer(this->PlayerBocCai) + " "+dataManager.GetSysString(65)+" ";
    this->Toast->ShowToast(StrToast, 3);

    //Add interval đếm ngược ở đây
    SetIntervalBocNoc();
}

void Layer_GivePocker_Chan::SetIntervalBocNoc() {
    //wid_Avata = 90, hei_Avata = 125
    Vec2 pPoint;
    int posBocCai = this->GetPosUserByName(this->PlayerBocCai);
    if(posBocCai == -1) return;
    Size size = ChanUtils::getSizePos();

    if (2 == this->mapPlayers.size()) {
        if (posBocCai == kUserMe)
            pPoint = Vec2((WIDTH_DESIGN / 2) + 150, 150 - size.height);
        else
            pPoint = Vec2((WIDTH_DESIGN / 2) + 150, HEIGHT_DESIGN - 115 + size.height);
    }
    else {
        switch (posBocCai) {
            case kUserMe:
            case kUserBot:
                pPoint = Vec2((WIDTH_DESIGN / 2) + 150, 150 - size.height);
                break;
            case kUserLeft:
                pPoint = Vec2(220 - size.width, HEIGHT_DESIGN / 2);
                break;
            case kUserRight:
                pPoint = Vec2(WIDTH_DESIGN - 220 + size.width, HEIGHT_DESIGN / 2);
                break;
            case kUserTop:
                pPoint = Vec2((WIDTH_DESIGN / 2) + 150, HEIGHT_DESIGN - 115 + size.height);
                break;
        }
    }

    this->Timer->setPosition(pPoint);
    this->Timer->setVisible(true);
    schedule(schedule_selector(Layer_GivePocker_Chan::UpdateTimer), 1);
}

void Layer_GivePocker_Chan::UpdateTimer(float dt) {
    this->StartTimmer -= dt;
    int k = ((int)StartTimmer);
    if (k < 0)
        return;

    char time[10] = {0};
    sprintf(time, "%d", k);
    Timer->setString(time);
}

void Layer_GivePocker_Chan::test(Ref* sender, int data) {
    int pos =  data;
    if (0 == pos) {
        int t = (this->CountLeft - 1) % 5;
        int z = (this->CountLeft - 1) / 5;
        ImageView *img;
        switch(t) {
        case 0:
            img = (ImageView *)B1->getChildByTag(z);
            img->setVisible(true);
            break;
        case 1:
            img = (ImageView*)B2->getChildByTag(z);
            img->setVisible(true);
            break;
        case 2:
            img = (ImageView*)B3->getChildByTag(z);
            img->setVisible(true);
            break;
        case 3:
            img = (ImageView*)B4->getChildByTag(z);
            img->setVisible(true);
            break;
        case 4:
            img = (ImageView*)B5->getChildByTag(z);
            img->setVisible(true);
            break;
        default:
            break;
        }
    } else {
        int t = (this->CountRight - 1) % 5;
        int z = (this->CountRight - 1) / 5;
        ImageView *img;
        switch(t) {
        case 0:
            img = (ImageView*)B6->getChildByTag(z);
            img->setVisible(true);
            break;
        case 1:
            img = (ImageView*)B7->getChildByTag(z);
            img->setVisible(true);
            break;
        case 2:
            img = (ImageView*)B8->getChildByTag(z);
            img->setVisible(true);
            break;
        case 3:
            img = (ImageView*)B9->getChildByTag(z);
            img->setVisible(true);
            break;
        case 4:
            img = (ImageView*)B10->getChildByTag(z);
            img->setVisible(true);
            break;
        default:
            break;
        }
    }

    this->GivePocker(pos);
    //delete data;
}

void Layer_GivePocker_Chan::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent) {
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
    if (0 == cmd->compare(EXT_EVENT_END))
        this->removeFromParentAndCleanup(true);

    else if (0 == cmd->compare(EXT_EVENT_RES_BOC_CAI)) {
        boost::shared_ptr<string> nocdetl = param->GetUtfString(EXT_FIELD_NOC_REMAIN_DETAIL);
        boost::shared_ptr<string> usrn = param->GetUtfString(EXT_FIELD_USERNAME);

        if (nocdetl != NULL && usrn != NULL) {
            //"nocdetl":"dautv3;92:9:2:1"
           // log("usrn in boc bai = %s", usrn->c_str());
            vector<string> _nocdetl = mUtils::splitString(*nocdetl, ';');
            this->PlayerStart = _nocdetl.at(0);
           // log("Player Start is %s", this->PlayerStart.c_str());
            if (!this->IsPlayerBocCai)
                this->MoveRandomIsNotPlayerBocCai(_nocdetl.at(1));
            else
                this->MoveCard4Users(_nocdetl.at(1), _nocdetl.at(0));
            CallFunc *callfun = CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::MoveCardsToPosition, this));
            this->runAction(Sequence::create(DelayTime::create(0.3), callfun, NULL));
        }
    }
}

void Layer_GivePocker_Chan::MoveCardsToPosition() {

   // log("Player Start = %s", this->PlayerStart.c_str());
   // log("Choose Index = %d", this->Index_Choose);

    int posuser = this->GetPosUserByName(this->PlayerStart);
    vector<int> pos;
    for(map<string, int>::iterator it = this->mapPlayers.begin(); it != this->mapPlayers.end(); ++it) {
        pos.push_back(it->second);
    }

    //! Sắp xếp lại vị trí các nọc
    int vt = -1;
    for (int i = 0; i < arrTag.size(); i++) {
        if (arrTag.at(i) == Index_Choose) {
            vt = i;
            break;
        }
    }

    if (vt == -1) return;

    vector<int> newTag = this->SortArrayTag(arrTag, vt);
    vector<int> newPos = this->SortArrayTag(pos, posuser);

    this->ChiaBai(newPos, newTag);
}

void Layer_GivePocker_Chan::ChiaBai(vector<int> toPos, vector<int> allTag) {
    for(unsigned int i = 0; i < toPos.size(); ++i) {
        for(vector<ImageView*>::iterator it = arrNocs.begin(); it != arrNocs.end(); ++it) {
            if((*it)->getTag() == allTag.at(i)) {
                this->FinishGive(*it, toPos.at(i));
                break;
            }
        }
    }
}

void Layer_GivePocker_Chan::FinishGive(ImageView *img, const int& tpos) {
    bool Frags = false;
    if (tpos == this->GetPosUserByName(this->PlayerStart)) {
        Frags = true;
    }

    Size SizeAdd = ChanUtils::getSizePos();
    Vec2 pPoint_Me = Vec2(WIDTH_DESIGN >> 1, 150 - SizeAdd.height);
    Vec2 pPoint_Left = Vec2(100 - SizeAdd.width, HEIGHT_DESIGN >> 1);
    Vec2 pPoint_Right = Vec2(WIDTH_DESIGN - 100 + SizeAdd.width, HEIGHT_DESIGN >> 1);
    Vec2 pPoint_Top = Vec2(WIDTH_DESIGN >> 1, HEIGHT_DESIGN - 100 + SizeAdd.height);

    Vec2 pPoint;

    //! Nếu chỉ có 2 người chơi.
    if (2 == this->mapPlayers.size()) {
        if (tpos == kUserMe)
            pPoint = pPoint_Me;
        else
            pPoint = pPoint_Top;
    }

    //! Có hơn hai người chơi.
    else {
        switch(tpos) {
        case kUserMe:
            pPoint = pPoint_Me;
            break;
        case kUserLeft:
            pPoint = pPoint_Left;
            break;
        case kUserTop:
            pPoint = pPoint_Top;
            break;
        case kUserRight:
            pPoint = pPoint_Right;
            break;
        default:
            break;
        }
    }

    MoveTo* moveTo = MoveTo::create(0.4, pPoint);
    DelayTime* delay = DelayTime::create(0.9f);
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(Layer_GivePocker_Chan::Call_CallBackWhenEnd, this));

    img->runAction(Sequence::create(moveTo, delay, callfunc, NULL));
    if (Frags) {
        MoveTo* moveTo2 = MoveTo::create(0.4, pPoint);
        this->Noc->runAction(moveTo2);
    }
}

vector<int> Layer_GivePocker_Chan::SortArrayTag(vector<int> _arr, int position) {
    vector<int> newTag;
    for (int i = 0; i < _arr.size(); i++) {
        int pos = (i + position) % _arr.size();
        newTag.push_back(_arr[pos]);
    }
    return newTag;
}

void Layer_GivePocker_Chan::MoveCard4Users(string& cards, const string& user) {
    if (this->Index_Noc == -1)
        this->Index_Noc = rand()%(4);

    arrTag.clear();
    for(int i = 1; i <= 5; ++i) {
        if (i != this->Index_Noc)
            arrTag.push_back(i);
    }
    vector<string> detail = mUtils::splitString(cards, ':');
    try {
        this->Noc->loadTexture(ChanUtils::FindTypeCard(detail.at(1), detail.at(2)), Widget::TextureResType::PLIST);
    } catch (std::out_of_range &e) {
       // log("Caught an out_of_range exception MoveCard4Users: %s", e.what());
    } catch (std::exception &e) {
       // log("Caught an exception of an unexpected type MoveCard4Users %s", e.what());
    } catch (...) {
       // log("Caught an unknown exception MoveCard4Users");
    }
}

void Layer_GivePocker_Chan::MoveRandomIsNotPlayerBocCai(string cards) {
    if (!IsPlayerBocCai) {
        this->B1->setVisible(false);
        this->B2->setVisible(false);
        this->B3->setVisible(false);
        this->B4->setVisible(false);
        this->B5->setVisible(false);
        this->Noc2->setVisible(false);

        int a[5] = {1, 2, 3, 5, 4};

        this->Index_Noc = a[rand() % (4)];
        ImageView *img = NULL;

        for(vector<ImageView*>::iterator it = arrNocs.begin(); it != arrNocs.end(); ++it) {
            if((*it)->getTag() == Index_Noc)
                img = *it;
        }

        img->runAction(MoveTo::create(0.3, this->Noc2->getPosition()));

        //! Để lấy mảng các UI hiệu ứng
        arrTag.clear();
        for(int i = 0; i < 5; i++) {
            if (a[i] != Index_Noc)
                arrTag.push_back(a[i]);
        }

        ImageView *img2;
        int y = rand() % (arrTag.size() - 2);
        this->Index_Choose = arrTag.at(y);
        for (int i = 0; i < arrTag.size(); i++) {
            ImageView *imgs = arrNocs.at(i);
            if (imgs->getTag() == this->Index_Choose) {
                img2 = imgs;
                break;
            }
        }

        this->Noc->runAction(MoveTo::create(0.3, img2->getPosition()));

        try {
            vector<string> detail = mUtils::splitString(cards,':');
            this->Noc->loadTexture(ChanUtils::FindTypeCard(detail.at(1), detail.at(2)), Widget::TextureResType::PLIST);
        } catch (std::out_of_range &e) {
           // log("Caught an out_of_range exception: %s", e.what());
        } catch (std::exception &e) {
           // log("Caught an exception of an unexpected type %s", e.what());
        } catch (...) {
           // log("Caught an unknown exception");
        }
    }
}

int Layer_GivePocker_Chan::GetPosUserByName(const string& uid) {
    return this->mapPlayers.find(uid) != this->mapPlayers.end() ? this->mapPlayers[uid] : -1;
}

void Layer_GivePocker_Chan::SetLayerChanToast(LayerChanToast* toast) {
    this->Toast = toast;
}

void Layer_GivePocker_Chan::SetCallBack_BocBaiIfGuess(Ref* target, SEL_CallFuncN callfun) {
    this->m_callback = target;
    this->m_callbackListener = callfun;
}

void Layer_GivePocker_Chan::Call_CallBackWhenEnd() {
    //this->removeFromParentAndCleanup(true);
}

Layer_GivePocker_Chan* Layer_GivePocker_Chan::createInstance() {
    Layer_GivePocker_Chan* giveCards = new Layer_GivePocker_Chan();
    if (giveCards && giveCards->init()) {
        giveCards->autorelease();
        return giveCards;
    } else {
        delete giveCards;
        giveCards = NULL;
        return NULL;
    }
}

