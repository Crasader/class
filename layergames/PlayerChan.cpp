
#include "PlayerChan.h"
#include "Requests/ExtensionRequest.h"
#include "BoBaiChan.h"

PlayerChan::PlayerChan()
    :PlayerLeft(NULL),
    PlayerRight(NULL),
    PlayerTop(NULL),
    sListOnHand(""),
    sListCuaChi(""),
    sListDuoiTay("")
{
    this->IsPlayer = false;
    this->SingeBot = 1;
    this->X_CardCuaChi = 0;
    this->Y_CardCuaChi = 0;
    this->X_CardDuoiTay = 0;
    this->Y_CardDuoiTay = 0;
    this->X_BeginVisible = 0;
    this->Y_BeginVisible = 0;
    this->CountChiu = 0;
    this->Singe = 1;
    this->Distance = W_CARD;
    this->BaiTrenTay.clear();
    this->BaiCuaChi.clear();
    this->BaiDuoiTay1.clear();
    this->BaiDuoiTay2.clear();
    this->duration = 0.2;
    this->aI = "";
}

PlayerChan::~PlayerChan(){
    this->BaiTrenTay.clear();
    this->BaiCuaChi.clear();
    this->BaiDuoiTay1.clear();
    this->BaiDuoiTay2.clear();
   // log("Player Chan Destructor %s", this->aI.c_str());
}

void PlayerChan::SetOtherPlayer(PlayerChan *right, PlayerChan *top, PlayerChan *left)
{
    this->PlayerRight = right;
    this->PlayerTop = top;
    this->PlayerLeft = left;
}

void PlayerChan::Reset()
{
    this->CountChiu = 0;
    this->Distance = W_CARD;
    this->BaiCuaChi.clear();
    this->BaiTrenTay.clear();
    this->BaiDuoiTay1.clear();
    this->BaiDuoiTay2.clear();
    this->pDisCards.flag = false;
    this->pEateCards.flag = false;
    this->pTranferCards.flag = false;

    //reset
    this->sListOnHand = "";
    this->sListCuaChi = "";
    this->sListDuoiTay = "";
}

void PlayerChan::HideCardCuaChi()
{
    this->HideCardInList(this->BaiCuaChi);
}

void PlayerChan::HideCardInList(vector<CardChan*>& P){
    std::for_each(P.begin(), P.end(), std::mem_fun(&CardChan::ResetCard));
    P.clear();
}

Vec2 PlayerChan::PosCardCuaChi()
{
    return Vec2(this->X_CardCuaChi + this->BaiCuaChi.size() * this->Distance * this->Singe, this->Y_CardCuaChi);
}

Vec2 PlayerChan::GetPosBegin()
{
    return Vec2(this->X_BeginVisible, this->Y_BeginVisible);
}

Vec2 PlayerChan::PosCardDuoiTay1()
{
    return Vec2(this->X_CardDuoiTay + (float)this->BaiDuoiTay1.size() * W_CARD * this->SingeBot, this->Y_CardDuoiTay);
}

Vec2 PlayerChan::PosCardDuoiTay2()
{
    int tmp = 0;
    tmp = this->BaiDuoiTay2.size() - (this->CountChiu / 3) * 2;
    float toX = this->X_CardDuoiTay + tmp * W_CARD * this->SingeBot;
    float toY = this->Y_CardDuoiTay - betweentb;
    return Vec2(toX, toY);
}

cocos2d::Vec2 PlayerChan::PosChiu(){
    int lan_chiu_trong_van = this->CountChiu / 3;
    int stt_trong_lan_chiu = this->CountChiu % 3;
    float toX = this->X_CardDuoiTay + (this->BaiDuoiTay2.size() - lan_chiu_trong_van * 2 - stt_trong_lan_chiu) * W_CARD * this->SingeBot;
    float toY = this->Y_CardDuoiTay - (betweentb / 2) * (stt_trong_lan_chiu + 1);
    this->CountChiu++;
    return Vec2(toX, toY);
}

void PlayerChan::SwapZorder(bool type){
    //D_Bottom vs D_top
    if (!type) {
        vector<CardChan*> arrTemp;
        vector<CardChan*>::iterator it;
        for(it = this->BaiDuoiTay1.begin(); it != this->BaiDuoiTay1.end(); ++it)
            arrTemp.push_back(*it);
        for(it = this->BaiDuoiTay2.begin(); it != this->BaiDuoiTay2.end(); ++it)
            arrTemp.push_back(*it);
        this->QuicksortZOder(arrTemp, 0, arrTemp.size() - 1);
    }

    //D_Top vs D_Bottom
    else {
        if ((this->BaiDuoiTay2.size() - this->CountChiu) <= this->BaiDuoiTay1.size()) {
            CardChan *pCard = this->BaiDuoiTay2.back();
            CardChan *qCard = this->BaiDuoiTay1.at(this->BaiDuoiTay2.size() - 1 - this->CountChiu);
            if (pCard->getLocalZOrder() < qCard->getLocalZOrder()) {
                int _zorder = -1;
                _zorder = pCard->getLocalZOrder();
                pCard->setLocalZOrder(qCard->getLocalZOrder());
                qCard->setLocalZOrder(_zorder);
            }
        }
    }
}

void PlayerChan::QuicksortZOder(vector<CardChan*>& A, int l, int r){
    if(l >= r)
        return;
    int i = l;
    int j = r;
    int zIndex = A.at((l+r) / 2)->getLocalZOrder();
    while(i <= j) {
        while(A.at(i)->getLocalZOrder() < zIndex) i++;
        while(A.at(j)->getLocalZOrder() > zIndex) j--;
        if(i <= j) {
            int temp = A.at(i)->getLocalZOrder();
            A.at(i)->setLocalZOrder(A.at(j)->getLocalZOrder());
            A.at(j)->setLocalZOrder(temp);
            i++;
            j--;
        }
    }
    this->QuicksortZOder(A, l, j);
    this->QuicksortZOder(A, i, r);
}

void PlayerChan::RefreshListCards()
{

}

void PlayerChan::ResortCardCuaChi(Ref* sender, PlayerChan* player)
{
    if(player->BaiCuaChi.size() > 8){

        player->Distance = (W_CARD * 8) / player->BaiCuaChi.size();

        // Sap xep lai
        if(0 == player->BaiCuaChi.size()) return;
        int pCount, pTemp;
        this->BaiCuaChi.at(0)->SetSTT(0);
        for (int i = 1; i < player->BaiCuaChi.size(); i++) {
            try {
                this->BaiCuaChi.at(i)->SetSTT(i);
                pCount = i;
                while (pCount > 0 && player->BaiCuaChi.at(pCount)->getLocalZOrder() < player->BaiCuaChi.at(pCount - 1)->getLocalZOrder()) {
                    pTemp = player->BaiCuaChi.at(pCount)->getLocalZOrder();
                    player->BaiCuaChi.at(pCount)->setLocalZOrder(player->BaiCuaChi.at(pCount - 1)->getLocalZOrder());
                    player->BaiCuaChi.at(pCount - 1)->setLocalZOrder(pTemp);
                    pCount--;
                }
            } catch (std::out_of_range &e) {
               // log("Caught an out_of_range exception: %s", e.what());
            } catch (std::exception &e) {
               // log("Caught an exception of an unexpected type %s", e.what());
            } catch (...) {
               // log("Caught an unknown exception");
            }
        }

        pCount = 0;
        for(vector<CardChan*>::iterator iter = player->BaiCuaChi.begin(); iter != player->BaiCuaChi.end(); ++iter) {
            MoveTo *moveTo = MoveTo::create(0.5, Vec2((player->X_CardCuaChi + player->Distance * pCount * player->Singe), player->Y_CardCuaChi));
            (*iter)->stopAllActions();
            (*iter)->setRotation(0);
            (*iter)->SetSTT(pCount);
            (*iter)->runAction(moveTo);
            pCount++;
        }
    }
    else
    {
        for(int i = 0; i < player->BaiCuaChi.size(); ++i){
            player->BaiCuaChi.at(i)->setRotation(0);
        }
    }
}

void PlayerChan::MoveCardCuaChi()
{
    for_each(this->BaiCuaChi.begin(), this->BaiCuaChi.end(), std::bind2nd(mem_fun(&CardChan::MoveCardCuaChi), (this->Y_CardCuaChi - betweentb)));
}

void PlayerChan::ActionAnCuaTrenByServer(const CardOrg &card)
{
    if(this->pEateCards.flag && this->pEateCards.cardnu == card.cardnu && this->pEateCards.cardsu == card.cardsu){
        this->pEateCards.flag = false;
        return;
    }

    if(this->PlayerLeft == NULL) return;
    if(0 == this->PlayerLeft->BaiCuaChi.size()) return;

    CardChan* fcard = this->PlayerLeft->BaiCuaChi.back();
    if(fcard == NULL)
        return;

    fcard->stopAllActions();

    this->PlayerLeft->BaiCuaChi.pop_back();

    MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay1());
    RotateTo* rotateTo = RotateTo::create(this->duration, 0);

    this->AddCardToDuoiTay1(fcard);
    this->SwapZorder(false);

    fcard->runAction(moveTo);
    fcard->runAction(rotateTo);

    this->pEateCards.flag = false;
}

void PlayerChan::ActionAnCuaChiByServer(const CardOrg& card){
    if(this->pEateCards.flag && this->pEateCards.cardnu == card.cardnu && this->pEateCards.cardsu == card.cardsu){
        this->pEateCards.flag = false;
        return;
    }

    if(0 == this->BaiCuaChi.size()) return;
    CardChan* fcard = this->BaiCuaChi.back();
    if(fcard == NULL)
        return;

    fcard->stopAllActions();

    this->BaiCuaChi.pop_back();

    MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay1());
    RotateTo* rotateTo = RotateTo::create(this->duration, 0);

    this->AddCardToDuoiTay1(fcard);
    this->SwapZorder(false);

    fcard->runAction(moveTo);
    fcard->runAction(rotateTo);

    this->pEateCards.flag = false;
}

void PlayerChan::ActionChiuBaiByServer(const CardOrg& card){
    BoBaiChan::GetInstance()->RemoveElemsChiu(card);
    CardChan* fcard = this->GetCardChiuFromPlayer(card);
    if(fcard == NULL)
        return;

    fcard->stopAllActions();

    //animation
    MoveTo* moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay1());

    this->AddCardToDuoiTay1(fcard);
    this->SwapZorder(false);

    fcard->runAction(moveTo);
}

void PlayerChan::RotateCardWhenFinish(vector<CardChan*>& P){
    for (int i = 0; i < P.size(); ++i) {
        CardChan* pCard = P.at(i);
        if (0 == pCard->getNumberOfRunningActions()) {
            pCard->setRotation(0);
        }
    }
}

CardChan* PlayerChan::GetCardTakeFromPlayer(const CardOrg& card){
   // log("get card in player player: %s", card.fuser.c_str());
    PlayerChan* player = this->GetPlayer(card.fuser);
    if(player != NULL){
        if(0 == player->BaiCuaChi.size())
            return NULL;
        CardChan* fcard = player->BaiCuaChi.back();
        if(fcard->GetNumber() == card.cardnu && fcard->GetSuite() == card.cardsu){
            player->BaiCuaChi.pop_back();
            return fcard;
        }
    }
    return NULL;
}

CardChan* PlayerChan::GetCardChiuFromPlayer(const CardOrg& card){
    PlayerChan* fromPlayer = this->GetPlayer(card.fuser);
    if(fromPlayer == NULL) return NULL;

    CardChan* pCard = NULL;

    if(fromPlayer->BaiCuaChi.size() > 0)
    {
        pCard = fromPlayer->BaiCuaChi.back();
        if(pCard->GetNumber() == card.cardnu && pCard->GetSuite() == card.cardsu)
        {
            fromPlayer->BaiCuaChi.pop_back();
            return pCard;
        }
    }
    else
    {
        if(0 == fromPlayer->BaiDuoiTay1.size()) return NULL;
        pCard = fromPlayer->BaiDuoiTay1.back();
        if(pCard->GetNumber() == card.cardnu && pCard->GetSuite() == card.cardsu)
        {
            CardChan* restoreCard = fromPlayer->BaiDuoiTay2.back();
            restoreCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
            restoreCard->setTouchEnabled(true);
            fromPlayer->AddCardToBaiTrenTay(restoreCard);
            this->RefreshListCards();

            fromPlayer->BaiDuoiTay1.pop_back();
            fromPlayer->BaiDuoiTay2.pop_back();

            fromPlayer->pEateCards.flag = false;
            fromPlayer->pTranferCards.flag = false;

            return pCard;
        }
    }
    return NULL;
}

PlayerChan* PlayerChan::GetPlayer(const string &aI)
{
    if(0 == aI.compare(this->PlayerLeft->aI)){
        return this->PlayerLeft;
    }
    else if(0 == aI.compare(this->PlayerRight->aI)){
        return this->PlayerRight;
    }
    else if(0 == aI.compare(this->PlayerTop->aI)){
        return this->PlayerTop;
    }
    else if(0 == aI.compare (this->aI)){
        return this;
    }
    return NULL;
}

bool PlayerChan::CheckingRunningCuaChi()
{
    for (vector<CardChan*>::const_iterator it = this->BaiCuaChi.begin(); it != this->BaiCuaChi.end(); ++it) {
        if ((*it)->getNumberOfRunningActions() > 0) {
            return true;
        }
    }
    return false;
}

void PlayerChan::AddCardToBaiTrenTay(CardChan* pCard){
    pCard->setVisible(true);
    this->BaiTrenTay.push_back(pCard);
    //this->RotateCardWhenFinish(this->BaiTrenTay);
}

void PlayerChan::AddCardToCuaChi(CardChan* pCard){
    this->BaiCuaChi.push_back(pCard);
     this->RotateCardWhenFinish(this->BaiCuaChi);
}

void PlayerChan::AddCardToDuoiTay1(CardChan* pCard){
    this->BaiDuoiTay1.push_back(pCard);
     this->RotateCardWhenFinish(this->BaiDuoiTay1);
}

void PlayerChan::AddCardToDuoiTay2(CardChan* pCard){
    this->BaiDuoiTay2.push_back(pCard);
     this->RotateCardWhenFinish(this->BaiDuoiTay2);
}

//
void PlayerChan::SetPlayerLeft(PlayerChan* player){
    this->PlayerLeft = player;
}

void PlayerChan::SetPlayerTop(PlayerChan* player){
    this->PlayerTop = player;
}

void PlayerChan::SetPlayerRight(PlayerChan* player){
    this->PlayerRight = player;
}

PlayerChan* PlayerChan::GetPlayerLeft() const{
    return this->PlayerLeft;
}

PlayerChan* PlayerChan::GetPlayerRight() const{
    return this->PlayerRight;
}

PlayerChan* PlayerChan::GetPlayerTop() const{
    return this->PlayerTop;
}

void PlayerChan::SetAI(const string& aI){
    this->aI = aI;
}

const string& PlayerChan::GetAI() const{
    return this->aI;
}

void PlayerChan::SetPosCardCuaChi(const float& x, const float& y){
    this->X_CardCuaChi = x;
    this->Y_CardCuaChi = y;
}

void PlayerChan::SetPosCardDuoiTay(const float& x, const float& y){
    this->X_CardDuoiTay = x;
    this->Y_CardDuoiTay = y;
}

void PlayerChan::SetPosBeginVisible(const float& x, const float& y){
    this->X_BeginVisible = x;
    this->Y_BeginVisible = y;
}

void PlayerChan::SetSigne(int cuachi, int duoitay)
{
    this->Singe = cuachi;
    this->SingeBot = duoitay;
}

void PlayerChan::SetIsPlayer(bool isplayer)
{
    this->IsPlayer = isplayer;
}

bool PlayerChan::GetIsPlayer() const
{
    return this->IsPlayer;
}

int PlayerChan::GetCountCardTrenTay() const
{
    return (int)this->BaiTrenTay.size();
}

void PlayerChan::ClearAIOwner()
{
    this->IsPlayer = false;
    this->aI = "";
}

void PlayerChan::SetPlayerListCards(std::string& lstCards, bool hasU){
    vector<string> arrs = mUtils::splitString(lstCards, '/');
    try {
        this->sListOnHand = arrs.at(0);
        if(!hasU){
            this->sListCuaChi = arrs.at(1);
            this->sListDuoiTay = arrs.at(2);
        }
    } catch(std::out_of_range& e) {
       // log("%s", e.what());
    } catch(...){

    }

   // log("set list card for: %s", this->aI.c_str());
   // log("Trên tay: %s", this->sListOnHand.c_str());
   // log("Dưới tay: %s", this->sListDuoiTay.c_str());
   // log("Cửa trì: %s", this->sListCuaChi.c_str());

    this->RePainCuaChi();
    this->RePainDuoiTay();
    this->RePainTrenTay();
}

bool PlayerChan::CheckCardRunning(){
    bool b = false;
    b = this->checkRunningInArray(this->BaiCuaChi);
    if (b) return b;
    b = this->checkRunningInArray(this->BaiDuoiTay1);
    if (b) return b;
    b = this->checkRunningInArray(this->BaiDuoiTay2);
    if (b) return b;
    b = this->checkRunningInArray(this->BaiTrenTay);
    return b;
}

bool PlayerChan::checkRunningInArray(vector<CardChan*>& P){
    for (int i = 0; i < P.size (); ++i) {
        try{
            if (P.at(i)->getNumberOfRunningActions() > 0) {
                return true;
            }
        }catch(...){

        }
    }
    return false;
}

void PlayerChan::RePainCuaChi(){
    if (this->sListCuaChi == "0" || this->sListCuaChi == "")
        return;

    vector<string> arrs = mUtils::splitString(this->sListCuaChi, ';');

    this->HideCardInList(this->BaiCuaChi);

    for (int it = 0; it < arrs.size(); ++it) {
        vector<string> info = mUtils::splitString(arrs.at(it), ':');
        try {

            int cardnu = atoi(info.at(1).c_str());
            int cardsu = atoi(info.at(2).c_str());

            CardChan* pCard = BoBaiChan::GetInstance()->PopCard();
            pCard->loadTexture(ChanUtils::FindTypeCard(cardnu, cardsu), Widget::TextureResType::PLIST);
            pCard->SetNumber(cardnu);
            pCard->SetSuite(cardsu);
            pCard->SetSizeCard(W_CARD, H_CARD);
            pCard->setPosition(this->PosCardCuaChi());

            int crdorg = atoi(info.at(4).c_str());
            if (crdorg == CARD_ORIGINATION_BY_DUOI || crdorg == CARD_ORIGINATION_BOC_NOC) {
                pCard->setColor(Color3B(200, 255, 255));
            } else {
                pCard->setColor(Color3B(255, 255, 255));
            }

            int id = atoi(info.at(0).c_str());
            pCard->SetServId(id);

            pCard->setVisible(true);
            this->AddCardToCuaChi(pCard);

        } catch (...) {
           // log("Caught an out_of_range exception: ");
        }
    }
    //! Resort lai cac la bai o cua chi
    this->ResortCardCuaChi(NULL, this);
}

void PlayerChan::RePainDuoiTay(){
    vector<RePainObj> lstRet = this->PrepareDuoiTay();

    if (0 == lstRet.size())
        return;

    this->HideCardInList(this->BaiDuoiTay1);
    this->HideCardInList(this->BaiDuoiTay2);
    this->CountChiu = 0;

    for (int i = 0; i < lstRet.size(); ++i) {
        RePainObj obj = lstRet.at(i);
        vector<string> arr = obj.arr;

        /**
         * Bai trên tay 1
         */
        vector<string> eate = mUtils::splitString(obj.key, ':');
        int cardnu = atoi(eate.at(1).c_str());
        int cardsu = atoi(eate.at(2).c_str());

        CardChan* pCard = BoBaiChan::GetInstance()->PopCard();
        pCard->loadTexture(ChanUtils::FindTypeCard(cardnu, cardsu), Widget::TextureResType::PLIST);
        pCard->SetNumber(cardnu);
        pCard->SetSuite(cardsu);
        pCard->SetSizeCard(W_CARD, H_CARD);
        pCard->setPosition(this->PosCardDuoiTay1());

        if (eate.at(3) == "1") {
            pCard->SetIsDraw(true);
            pCard->setColor(Color3B(200, 255, 255));
        }
        else {
            pCard->setColor(Color3B(255, 255, 255));
        }
        pCard->setVisible(true);

        int id = atoi(eate.at(0).c_str());
        pCard->SetServId(id);

        this->AddCardToDuoiTay1(pCard);

        /**
         * Bài dưới tay
         */
        for (int j = 0; j < arr.size(); ++j) {

            vector<string> tran = mUtils::splitString(arr.at(j), ':');
            cardnu = atoi(tran.at(1).c_str());
            cardsu = atoi(tran.at(2).c_str());

            CardChan* qCard = BoBaiChan::GetInstance()->PopCard();
            qCard->loadTexture(ChanUtils::FindTypeCard(cardnu, cardsu), Widget::TextureResType::PLIST);
            qCard->SetNumber(cardnu);
            qCard->SetSuite(cardsu);
            qCard->SetSizeCard(W_CARD, H_CARD);

            if (obj.type == CHIUCARD) {
                qCard->setPosition(this->PosChiu());
            } else {
                qCard->setPosition(this->PosCardDuoiTay2());
            }

            qCard->setColor(Color3B(255, 255, 255));
            qCard->setVisible(true);

            id = atoi(tran.at(0).c_str());
            qCard->SetServId(id);

            this->AddCardToDuoiTay2(qCard);
        }
    }

    this->SwapZorder(false);
}

void PlayerChan::RePainTrenTay(){

}

vector<RePainObj> PlayerChan::PrepareDuoiTay(){
    vector<RePainObj> lstRet;

    if (this->sListDuoiTay == "0" || this->sListDuoiTay == "")
        return lstRet;



    vector<string> arrs = mUtils::splitString(this->sListDuoiTay, ';');

    int i = 0;

    while (i < arrs.size() - 1) {
        try {
            vector<string> info1 = mUtils::splitString(arrs.at(i), ':');
            vector<string> info2 = mUtils::splitString(arrs.at(i + 1), ':');

            RePainObj obj;
            vector<string> arrVals;

            // Đây là chíu
            if (info1.at(1) == info2.at(1) && info1.at(4) == info2.at(4)) {
                for (int k = i; k < i + 4; k++) {
                    if (k != (i + 3)) {
                        arrVals.push_back(arrs.at(k));
                    }
                }

                obj.key = arrs.at(i + 3);
                obj.arr = arrVals;
                obj.type = 1;

                i += 4;
            }
            // Đây là chắn hoặc cạ
            else {
                arrVals.push_back(arrs.at(i));

                obj.key = arrs.at(i + 1);
                obj.arr = arrVals;
                obj.type = 0;

                i += 2;
            }

            lstRet.push_back(obj);

        } catch (...) {
           // log("Exception occured");
        }
    }

    return lstRet;
}
