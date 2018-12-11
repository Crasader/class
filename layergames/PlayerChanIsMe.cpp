#include "PlayerChanIsMe.h"
#include "BoBaiChan.h"
#include "Requests/ExtensionRequest.h"
#include "ChanOrderAlg.h"

PlayerChanIsMe::PlayerChanIsMe() : PlayerChan()
{

}

PlayerChanIsMe::~PlayerChanIsMe()
{

}

void PlayerChanIsMe::RefreshListCards()
{
    PlayerChan::RefreshListCards();
   // log("size this->BaiTrenTay = %ld", this->BaiTrenTay.size());
    for(int i = 0; i < this->BaiTrenTay.size(); ++i) {
        try{
            this->BaiTrenTay.at(i)->SetSTT(i);
        }
        catch(std::out_of_range& e){

        }
        catch(std::exception& e){

        }
    }

    ExecuteInfo info;
    info.beginX = this->X_BeginVisible;
    info.beginY = this->Y_BeginVisible;
    info.startPos = (float)(this->BaiTrenTay.size() - 1);
    info.cornor = GOC;
    info.width = 0;
    info.height = H_CARD_HAND / 6;
    info.duration_ = 0.4;
    for_each(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::RotateCardChan), info));
}

// Check Co the
bool PlayerChanIsMe::CheckU(const CardOrg& card)
{
    bool bRet = true;
    ChanOrderAlg* alg = new ChanOrderAlg();

    vector<string> arrHand = mUtils::splitString(this->sListOnHand, ';');
    for(int i = 0; i < arrHand.size(); ++i){
        try{
            vector<string> info = mUtils::splitString(arrHand.at(i), ':');
            int nu = atoi(info.at(1).c_str());
            int su = atoi(info.at(2).c_str());
            alg->Push(nu, su);
        }catch(...){

        }
    }

    vector<string> duoitay = mUtils::splitString(this->sListDuoiTay,';');
    for(int i = 0; i < duoitay.size(); ++i){
        try{
            vector<string> info = mUtils::splitString(duoitay.at(i), ':');
            int nu = atoi(info.at(1).c_str());
            int su = atoi(info.at(2).c_str());
            alg->Push(nu, su);
        }catch(...){

        }
    }

    bRet = alg->CheckU();
    delete alg;

    return bRet;
}

void PlayerChanIsMe::ActionDiscardByServer(const CardOrg& card)
{
    if(this->pDisCards.flag)
    {
        if(this->pDisCards.cardnu == card.cardnu && this->pDisCards.cardsu == card.cardsu)
        {
            this->pDisCards.flag = false;
            return;
        }
        else
        {
            CardChan* pCard = this->BaiCuaChi.back();
            if (pCard->GetNumber() == card.cardnu && pCard->GetSuite() == card.cardsu) {
                this->BaiCuaChi.pop_back();
                this->BaiTrenTay.push_back(pCard);
                this->RefreshListCards();
            }
        }
    }

    FindCard *info = new FindCard(true, card.cardnu, card.cardsu);
    vector<CardChan*>::iterator iter = std::find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::FindCardIsClick), info));
    if(iter == this->BaiTrenTay.end())
    {
        iter = find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), bind2nd(mem_fun(&CardChan::FindCardIfNotFoundClick), info));
    }

    if(iter != this->BaiTrenTay.end()) {
        CardChan* pCard = (*iter);
        pCard->stopAllActions();
        RotateTo *rotateTo = RotateTo::create(this->duration, 0);
        ScaleBy *scaleBy = ScaleBy::create(this->duration, W_CARD / W_CARD_HAND, H_CARD / H_CARD_HAND);
        pCard->runAction(rotateTo);
        pCard->runAction(scaleBy);

        //
        MoveTo* moveTo = MoveTo::create(this->duration, this->PosCardCuaChi());
        CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(PlayerChanIsMe::ResortCardCuaChi, this, pCard, this));
        DelayTime* delay = DelayTime::create(this->duration);

        this->BaiCuaChi.push_back(pCard);
        pCard->runAction(Sequence::create(delay, moveTo, callfun, NULL));

        this->BaiTrenTay.erase(iter);
        this->RefreshListCards();
    }
    this->pDisCards.flag = false;
    delete info;
}

void PlayerChanIsMe::ActionTraCuaByServer(const CardOrg& card)
{
    FindCard *info = new FindCard(true, card.cardnu, card.cardsu);
    vector<CardChan*>::iterator iter = std::find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::FindCardIsClick), info));
    CardChan* pCard = NULL;
    if(iter != this->BaiTrenTay.end()) {
        pCard = *iter;
        this->BaiTrenTay.erase(iter);
    } else {
        vector<CardChan*>::iterator iter2 = std::find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::FindCardIfNotFoundClick), info));
        if(iter2 != this->BaiTrenTay.end()) {
            pCard = *iter2;
            this->BaiTrenTay.erase(iter2);
        }
    }

    delete info;
    if(pCard == NULL)
        return;

    PlayerChan* player = this->GetPlayer(card.tuser);
    if(player == NULL)
        return;

    pCard->stopAllActions();

   // log("Tôi trả cửa cho %s", player->GetAI().c_str());

    RotateTo *rotateTo = RotateTo::create(this->duration, 0);
    ScaleBy *scaleBy = ScaleBy::create(this->duration, W_CARD / W_CARD_HAND, H_CARD / H_CARD_HAND);

    MoveTo *moveTo = MoveTo::create(this->duration, player->PosCardCuaChi());
    player->AddCardToCuaChi(pCard);

    pCard->runAction(moveTo);
    pCard->runAction(rotateTo);
    pCard->runAction(scaleBy);

    this->RefreshListCards();
}

void PlayerChanIsMe::ActionChuyenBaiByServer(const CardOrg& card)
{
    if(this->pTranferCards.flag)
    {
        if(this->pTranferCards.cardnu == card.cardnu && this->pTranferCards.cardsu == card.cardsu){
            this->pTranferCards.flag = false;
            return;
        }
    }

    FindCard *info = new FindCard(true, card.cardnu, card.cardsu);

    vector<CardChan*>::iterator iter = find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), bind2nd(mem_fun(&CardChan::FindCardIsClick), info));
    if(iter == this->BaiTrenTay.end())
    {
        iter = find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), bind2nd(mem_fun(&CardChan::FindCardIfNotFoundClick), info));
    }

    if(iter != this->BaiTrenTay.end()) {

        CardChan* pCard = (*iter);

        if(pCard->GetNumber () == card.cardnu && pCard->GetSuite () == card.cardsu)
        {
            MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay2());
            RotateTo *rotateTo = RotateTo::create(this->duration, 0);
            ScaleBy *scaleBy = ScaleBy::create(this->duration, W_CARD / W_CARD_HAND, H_CARD / H_CARD_HAND);

            pCard->stopAllActions();

            this->BaiDuoiTay2.push_back(pCard);
            this->BaiTrenTay.erase(iter);

            pCard->SetNumber(card.cardnu);
            pCard->SetSuite(card.cardsu);
            pCard->runAction(moveTo);
            pCard->runAction(rotateTo);
            pCard->runAction(scaleBy);
            pCard->setTouchEnabled(false);

            this->SwapZorder(false);

            this->RefreshListCards();
            this->pDisCards.flag = false;
        }
    }
    this->pTranferCards.flag = false;
    delete info;
}

void PlayerChanIsMe::ActionAnCuaTrenByServer(const CardOrg &card)
{
   // log("Toi an bai");
    PlayerChan::ActionAnCuaTrenByServer(card);
}

void PlayerChanIsMe::ActionChuyenBaiChiuByServer(const CardOrg &card)
{
    int cardnu = card.cardnu;
    int cardsu = card.cardsu;

    vector<CardChan*> cardchiu;
    for(vector<CardChan*>::iterator it = this->BaiTrenTay.begin(); it != this->BaiTrenTay.end(); ++it){
        CardChan* pCard = (*it);
        if(pCard->GetNumber() == cardnu && pCard->GetSuite() == cardsu){
            cardchiu.push_back(pCard);
            /*this->BaiTrenTay.erase(it);
            it--;*/
        }
    }
	this->BaiTrenTay.erase(std::remove_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(),
		[&](CardChan* card){
		return card->GetNumber() == cardnu && card->GetSuite() == cardsu;
	}),
		this->BaiTrenTay.end());
    this->RefreshListCards();

    for(int i = 0; i < cardchiu.size(); ++i){

        CardChan* pCard = cardchiu.at(i);
        pCard->SetSizeCard(W_CARD, H_CARD);

        pCard->stopAllActions();

        //Hien thi
        pCard->setPosition(this->GetPosBegin());
        pCard->setVisible(true);

        // Di chuyen xuong duoi tay
        MoveTo *moveto = MoveTo::create(this->duration, this->PosChiu());
        RotateTo *rotateTo = RotateTo::create(this->duration, 0);
        CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(PlayerChanIsMe::RefreshListCards, this));

        this->AddCardToDuoiTay2(pCard);

        pCard->runAction(rotateTo);
        pCard->runAction(Sequence::create(moveto, callfunc, NULL));
    }
}


int PlayerChanIsMe::ActionDisCardByClient(){
    if(this->pDisCards.flag)
        return CLIENT_RC_ACTION_PENDING;

    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if(lastRoom == NULL)
        return CLIENT_RC_LASTROOM_NULL;

    vector<CardChan*>::iterator it = std::find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), mem_fun(&CardChan::GetFlag));
    if(it != this->BaiTrenTay.end()) {

        CardChan* pCard = (*it);
        int cardnu = pCard->GetNumber();
        int cardsu = pCard->GetSuite();

        pCard->stopAllActions();

        // Neu truoc do minh khong chiu thi thuc hien di chuyen con bai truoc.
        // Neu truoc do minh chiu, thi hanh dong nay la tra cua -> de server lam
        if(this->pDisCards.beforeAction != 1)
        {
            this->BaiTrenTay.erase(it);

            this->pDisCards.flag = true;
            this->pDisCards.cardnu = cardnu;
            this->pDisCards.cardsu = cardsu;

            RotateTo *rotateTo = RotateTo::create(this->duration, 0);
            ScaleBy *scaleBy = ScaleBy::create(this->duration, W_CARD / W_CARD_HAND, H_CARD / H_CARD_HAND);
            pCard->runAction(rotateTo);
            pCard->runAction(scaleBy);

            //
            MoveTo* moveTo = MoveTo::create(this->duration, this->PosCardCuaChi());
            CallFunc* callfun = CallFunc::create(CC_CALLBACK_0(PlayerChanIsMe::ResortCardCuaChi, this, pCard, this));

            this->AddCardToCuaChi(pCard);
            pCard->runAction(Sequence::create(moveTo, callfun, NULL));

            this->RefreshListCards();
        }

        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutByte(EXT_FIELD_CARD_NUMBER, cardnu);
        params->PutByte(EXT_FIELD_CARD_SUIT, cardsu);
        boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_DISCARD, params, lastRoom));
        GameServer::getSingleton().Send(request);

        return CLIENT_RC_OK;
    }
    else
    {
        return CLIENT_RC_CHUA_CHON_LABAI_DANH;
    }
}

int PlayerChanIsMe::ActionChiuCardByClient(){
    //EXT_EVENT_REQ_CHIU_CARD = "rqchiuc";
    boost::shared_ptr<Room> lstRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if(lstRoom == NULL)
        return CLIENT_RC_LASTROOM_NULL;
    vector<CardChan*>::iterator iter = find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), mem_fun(&CardChan::GetFlag));
    if(iter != this->BaiTrenTay.end()) {
        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutByte(EXT_FIELD_CARD_NUMBER, (*iter)->GetNumber());
        params->PutByte(EXT_FIELD_CARD_SUIT, (*iter)->GetSuite());
        boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_CHIU_CARD, params, lstRoom));
        GameServer::getSingleton().Send(request);
        return CLIENT_RC_OK;
    } else {
        return CLIENT_RC_CHUA_CHON_LABAI_CHIU;
    }
}

int PlayerChanIsMe::ActionTakeCardByClient(const CardOrg& lastCard){

    if(this->pEateCards.flag || this->pEateCards.flag){
        return CLIENT_RC_ACTION_PENDING;
    }

    boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    vector<CardChan*>::iterator it = std::find_if(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), mem_fun(&CardChan::GetFlag));
    if (it != this->BaiTrenTay.end()) {

        CardChan* pCard = (*it);

        if(lastCard.crdorg != CARD_ORIGINATION_UNKOWN){

            //Chi an duoc bai o cua chi/
            if(lastCard.crdorg != CARD_ORIGINATION_BOC_NOC &&
                    lastCard.crdorg != CARD_ORIGINATION_BY_DUOI &&
                    lastCard.crdorg != CARD_ORIGINATION_BY_DISCARD &&
                    lastCard.crdorg != CARD_ORIGINATION_TRA_CUA){
                return CLIENT_RC_FAILED;
            }

            if(pCard->GetNumber() != lastCard.cardnu){
                return CLIENT_RC_AN_KHONG_DUNG_CHAN_CA;
            }

            // Chi animation truoc khi la bai nay khong the chiu duoc
            if(!BoBaiChan::GetInstance()->CheckCanChiu(lastCard))
            {
                CardOrg obj = lastCard;
                obj.fpos = lastCard.tpos;
                obj.fuser = lastCard.tuser;

                CardChan* targetCard = this->GetCardTakeFromPlayer(obj);

                if(targetCard == NULL)
                    return CLIENT_RC_CARD_NOT_IN_HAND;

                // check chiu card o day.
                this->pEateCards.flag = true;
                this->pEateCards.cardnu = lastCard.cardnu;
                this->pEateCards.cardsu = lastCard.cardsu;
                this->pEateCards.frompos = obj.fpos;
                this->pEateCards.fromuser = obj.fuser;

                this->pTranferCards.flag = true;
                this->pTranferCards.cardnu = pCard->GetNumber();
                this->pTranferCards.cardsu = pCard->GetSuite();

                //Bat dau animation.
                this->BaiTrenTay.erase(it);

                pCard->stopAllActions();

                RotateTo *rotateTo = RotateTo::create(this->duration, 0);
                ScaleBy *scaleBy = ScaleBy::create(this->duration, W_CARD / W_CARD_HAND, H_CARD / H_CARD_HAND);
                pCard->runAction(rotateTo);
                pCard->runAction(scaleBy);

                //
                MoveTo *moveTo = MoveTo::create(this->duration, this->PosCardDuoiTay2());
                auto callfun = CallFunc::create(CC_CALLBACK_0(PlayerChanIsMe::ResortCardCuaChi, this, pCard, this));
                DelayTime *delay = DelayTime::create(this->duration);

                this->AddCardToDuoiTay2(pCard);
                pCard->runAction(Sequence::create(delay, moveTo, callfun, NULL));
                this->RefreshListCards();

                // Animation target card.
                MoveTo* m2 = MoveTo::create(this->duration, this->PosCardDuoiTay1());
                this->AddCardToDuoiTay1(targetCard);
                this->SwapZorder(false);

                targetCard->stopAllActions();

                targetCard->runAction(m2);
            }
        }

        // gui ban tin di.
        boost::shared_ptr<ISFSObject> params (new SFSObject());
        params->PutByte(EXT_FIELD_CARD_NUMBER, pCard->GetNumber());
        params->PutByte(EXT_FIELD_CARD_SUIT, pCard->GetSuite());
        boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_TAKE_CARD, params, lastRoom));
        GameServer::getSingleton().Send(request);
        return CLIENT_RC_OK;
    } else {
        return CLIENT_RC_CHUA_CHON_LABAI_AN;
    }
}

void PlayerChanIsMe::ScaleCardsHand_WhenU()
{
    if(0 == this->BaiTrenTay.size())
        return;

    Size ssi = ChanUtils::getSizePos();

    float CountCard = (float)(this->BaiTrenTay.size() - 1);
    float Start = (-CountCard / 2) * GOC;
    int Count_ = 0;

    float xOld = WIDTH_DESIGN >> 1;
    float yOld = 250;

    float __goc = 20 * GOC;
    float __iii = (__goc - 180) / 2;
    yOld = (H_CARD + 15) * sin(__iii * PI / 180) + 50 - ssi.height;

    for(vector<CardChan*>::iterator iter = this->BaiTrenTay.begin(); iter != this->BaiTrenTay.end(); ++iter) {
        float rotate = Start + Count_ * GOC;
        rotate = rotate * PI / 180;
        float xNew = xOld + sin(rotate) * H_CARD_HAND / 6;
        float yNew = yOld - (H_CARD_HAND / 6 * ( 1 - cos(rotate)));
        Vec2 pos = Vec2(xNew, yNew);
        MoveTo *moveTo = MoveTo::create(this->duration, pos);
        (*iter)->SetCurrentPos(pos);
        (*iter)->SetSizeCard(W_CARD + 15, H_CARD + 15);
        (*iter)->runAction(moveTo);
        Count_++;
    }
}

void PlayerChanIsMe::ChiuSuccess(const int &rescode)
{
   // log("chiu rescode = %d", rescode);
    this->pDisCards.beforeAction = 1; // vua chiu thanh cong.
    this->pDisCards.flag = false;
    this->RefreshListCards();
}

void PlayerChanIsMe::TakeSuccess(const int& rescode){
   // log("take card rescode = %d", rescode);
        if(rescode == GAME_RSCODE_OK){
            this->pDisCards.beforeAction = 2; // Vua an thanh cong
        }
        else if(this->pEateCards.flag && this->pTranferCards.flag)  {
            CardChan* pCard = this->BaiDuoiTay1.back();
            PlayerChan* player = this->GetPlayer(this->pEateCards.fromuser);
            if(player != NULL)
            {
                //if(pCard == NULL) return;
                pCard->stopAllActions();
                pCard->setPosition(player->PosCardCuaChi());
                player->AddCardToCuaChi (pCard);

                // restore
                CardChan* restoreCard = this->BaiDuoiTay2.back();
                //if(restoreCard == NULL) return;
                restoreCard->stopAllActions();
                restoreCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
                restoreCard->setTouchEnabled(true);
                restoreCard->setVisible(true);
                this->AddCardToBaiTrenTay(restoreCard);
                this->RefreshListCards();

                this->BaiDuoiTay1.pop_back();
                this->BaiDuoiTay2.pop_back();
            }
        }
        this->pEateCards.flag = false;
        this->pTranferCards.flag = false;
}

void PlayerChanIsMe::DisSuccess(const int& rescode){
   // log("discard rescode = %d", rescode);
    if(rescode != GAME_RSCODE_OK && this->pDisCards.flag)
    {
        CardChan* pCard = this->BaiCuaChi.back();
        if(rescode == GAME_RSCODE_ERROR_CARD_NOT_IN_ON_HAND_LIST)
        {
            pCard->ResetCard();
        }
        else
        {
            pCard->stopAllActions();
            pCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
            pCard->setTouchEnabled(true);
            this->AddCardToBaiTrenTay(pCard);
            this->RefreshListCards();
        }
        this->BaiCuaChi.pop_back ();
    }
}

int PlayerChanIsMe::SetListCards(string &listcards)
{
    std::vector<std::string> list = mUtils::splitString(listcards, '/');
    if(0 == list.size()) return -1;

    try{
        this->sListOnHand = list.at(0);
        this->sListCuaChi = list.at(1);
        this->sListDuoiTay = list.at(2);
    } catch(...){
       // log("exception set list cards player is me");
    }

    list.clear();
    list = mUtils::splitString(this->sListOnHand, ';');

    if (1 == list.size() && 0 == list.at(0).compare("0")) {
        return 1;
    }

    if (0 != this->BaiTrenTay.size()) {
        std::for_each(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::mem_fun(&CardChan::ResetCard));
        this->BaiTrenTay.clear();
    }

    unsigned int i = 0;
    bool b = false;

    while (i <= list.size()) {
        if (i < list.size()) {
            try {
                vector<string> info = mUtils::splitString(list.at(i), ':');
                CardChan *pCard = BoBaiChan::GetInstance()->PopCard();
                pCard->SetSTT(i);
                pCard->SetNumber(atoi(info.at(1).c_str()));
                pCard->SetSuite(atoi(info.at(2).c_str()));
                pCard->SetFlag(false);
                pCard->loadTexture(ChanUtils::FindTypeCard(info.at(1), info.at(2)), Widget::TextureResType::PLIST);
                pCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
                pCard->setPosition(Vec2(this->X_BeginVisible, this->Y_BeginVisible));
                pCard->setTouchEnabled(true);
                pCard->setVisible(true);
                pCard->addTouchEventListener(CC_CALLBACK_2(PlayerChanIsMe::CardTouch, this));
                this->AddCardToBaiTrenTay(pCard);
                i++;
            } catch (...) {
               // log("player is me set list cards ca an unknown exception");
            }
        } else {
            this->RotateListCards();
            b = true;
            break;
        }
    }
    if(b) return 0;
    return -1;
}

void PlayerChanIsMe::RotateListCards()
{
    ExecuteInfo info;
    info.beginX = this->X_BeginVisible;
    info.beginY = this->Y_BeginVisible;
    info.startPos = (float)(this->BaiTrenTay.size() - 1);
    info.cornor = GOC;
    info.width = 0;
    info.height = H_CARD_HAND / 6;
    info.duration_ = 0.8;
    for_each(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::RotateCardChan), info));
}

void PlayerChanIsMe::BeforeBocBai(){
    if(this->pEateCards.flag && this->pTranferCards.flag)
    {
        CardChan* pCard = this->BaiDuoiTay1.back();
        CardChan* restoreCard = this->BaiDuoiTay2.back();
        if(pCard != NULL && restoreCard != NULL)
        {
            PlayerChan* player = this->GetPlayer(this->pEateCards.fromuser);
            if(player != NULL)
            {
                //
                pCard->stopAllActions();
                pCard->setPosition(player->PosCardCuaChi());
                player->AddCardToCuaChi (pCard);

                // restore
                restoreCard->stopAllActions();
                restoreCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
                restoreCard->setTouchEnabled(true);
                restoreCard->setVisible(true);
                this->AddCardToBaiTrenTay(restoreCard);
                this->RefreshListCards();

                this->BaiDuoiTay1.pop_back();
                this->BaiDuoiTay2.pop_back();
            }
        }
    }
    this->pEateCards.flag = false;
    this->pTranferCards.flag = false;
}

void PlayerChanIsMe::StartTurn()
{
    this->pDisCards.flag = false;
    this->pEateCards.flag = false;
    this->pTranferCards.flag = false;
}

void PlayerChanIsMe::FinishTurn()
{
    this->pDisCards.flag = true;
    this->pEateCards.flag = true;
    this->pTranferCards.flag = true;
}

void PlayerChanIsMe::CardTouch(Ref *pSender, Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED) {
        CardChan *pChan = (CardChan*)pSender;
        for_each(this->BaiTrenTay.begin(), this->BaiTrenTay.end(), std::bind2nd(mem_fun(&CardChan::CardIsClick), pChan));
    }
}

void PlayerChanIsMe::SetListCardFirst(string& lstCards){
    vector<string> arrs = mUtils::splitString(lstCards, '/');
    try {
        this->sListOnHand = arrs.at(0);
        this->sListCuaChi = arrs.at(1);
        this->sListDuoiTay = arrs.at(2);
    } catch (...) {
    }
}

void PlayerChanIsMe::RePainTrenTay() {
    PlayerChan::RePainTrenTay();

    vector<string> list = mUtils::splitString(this->sListOnHand, ';');

    int delta = 0;
    vector<string> info;

    if (this->BaiTrenTay.size() > list.size()) {
        delta = this->BaiTrenTay.size() - list.size();
        while (delta > 0) {
            this->BaiTrenTay.back()->ResetCard();
            this->BaiTrenTay.pop_back();
            delta--;
        }
    }
    else {
        delta = this->BaiTrenTay.size();
        for (int i = delta; i < list.size(); ++i) {
            try {
                info = mUtils::splitString(list.at(i), ':');
                CardChan* pCard = BoBaiChan::GetInstance()->PopCard();
                pCard->SetSTT(i);
                pCard->SetNumber(atoi(info.at(1).c_str()));
                pCard->SetSuite(atoi(info.at(2).c_str()));
                pCard->SetFlag(false);
                pCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
                pCard->loadTexture(ChanUtils::FindTypeCard(info.at(1), info.at(2)), Widget::TextureResType::PLIST);
                this->AddCardToBaiTrenTay(pCard);
            } catch (std::out_of_range& e) {
               // log("out of range: %s", e.what());
            }
        }
    }

    this->RefreshListCards();

    int Index = 0;
    for (int iter = 0; iter < list.size(); ++iter) {
        info = mUtils::splitString(list.at(iter), ':');
        try {
            CardChan* pCard = this->BaiTrenTay.at(Index);
            pCard->SetSTT(Index);
            pCard->SetNumber(atoi(info.at(1).c_str()));
            pCard->SetSuite(atoi(info.at(2).c_str()));
            pCard->SetFlag(false);
            pCard->loadTexture(ChanUtils::FindTypeCard(info.at(1), info.at(2)), Widget::TextureResType::PLIST);
            if (pCard->GetFlag()) {
                pCard->ResetPosition();
            }
            pCard->setColor(Color3B(255, 255, 255));
            pCard->SetSizeCard(W_CARD_HAND, H_CARD_HAND);
            pCard->setTouchEnabled(true);
            pCard->setVisible(true);
            pCard->addTouchEventListener(CC_CALLBACK_2(PlayerChanIsMe::CardTouch, this));

            int id = atoi(info.at(0).c_str());
            pCard->SetServId(id);

            Index++;
        } catch (std::out_of_range& e) {
           // log("RePainTrenTay Player Me exception: %s", e.what());
        }
        info.clear();
    }
}
