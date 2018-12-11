#include "LayerTaiXiuPopupWinners.h"
#include "mUtils.h"
#include "../Common.h"
#include "../GameServer.h"
#include "CircleAvatar.h"
#include "ChanUtils.h"

#include <boost/algorithm/string/trim.hpp>

LayerTaiXiuPopupWinners::LayerTaiXiuPopupWinners()
{
    this->CurrentHistory = 0;
    this->SizeListTops = 0;
    this->WinnerArray.clear();
    this->tblTopWinner = NULL;
    this->lblPopupWinner = NULL;
}

LayerTaiXiuPopupWinners::LayerTaiXiuPopupWinners(const std::vector<string> &P)
{
    this->CurrentHistory = 0;
    this->SizeListTops = 0;
    this->tblTopWinner = NULL;
    this->lblPopupWinner = NULL;
    this->SetListWinners(P);
}

LayerTaiXiuPopupWinners* LayerTaiXiuPopupWinners::createInstace(const vector<string> & P){
    LayerTaiXiuPopupWinners* instance = new LayerTaiXiuPopupWinners(P);
    if(instance && instance->init()){
        instance->autorelease();
        return instance;
    }
    else
    {
        delete instance;
        instance = 0;
        return NULL;
    }
}

void LayerTaiXiuPopupWinners::SetListWinners(const vector<string> &P) {
    this->WinnerArray.clear();
    for (int i = 0; i < P.size(); ++i) {
        string str = P.at(i);
        vector<string> arr =  mUtils::splitString(str, ';');
        if (arr.size() != 0)
            this->WinnerArray.push_back(arr);
    }

    this->SizeListTops = this->WinnerArray.size();
}


LayerTaiXiuPopupWinners::~LayerTaiXiuPopupWinners()
{
   // log("destructor ~LayerTaiXiuPopupWinners");
}

bool LayerTaiXiuPopupWinners::init()
{
    if(!Layer::init())
        return false;

    this->setAnchorPoint(Vec2::ZERO);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Node* history = CSLoader::getInstance()->createNode("LayerTaiXiuPopupWinners.csb");

    Layout* layout = static_cast<Layout*>(history->getChildByName("Panel_1"));

    if(layout != NULL){
        layout->setVisible(true);
        layout->setOpacity(150);
        layout->setPosition(Vec2(-mUtils::getSizePos().width, -mUtils::getSizePos().height));
    }

    Sprite* sprPopupBackground_0 = static_cast<Sprite*>(history->getChildByName("sprPopupBackground_0"));
    if(sprPopupBackground_0 != NULL) {
        this->tblTopWinner = TableView::create(this, Size(410, 740));
        this->tblTopWinner->setDirection(TableView::Direction::VERTICAL);
        this->tblTopWinner->setPosition(Vec2(34, 30));
        this->tblTopWinner->setDelegate(this);
        this->tblTopWinner->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        sprPopupBackground_0->addChild(this->tblTopWinner);
    }

    Button* btnBack = static_cast<Button*>(history->getChildByName("btnBack"));
    Button* btnNext = static_cast<Button*>(history->getChildByName("btnNext"));

    btnBack->setPressedActionEnabled(true);
    btnNext->setPressedActionEnabled(true);

    btnBack->addTouchEventListener(CC_CALLBACK_2(LayerTaiXiuPopupWinners::OnBtnPrevClick, this));
    btnNext->addTouchEventListener(CC_CALLBACK_2(LayerTaiXiuPopupWinners::OnBtnNextClick, this));

    this->lblPopupWinner =  static_cast<Text*>(history->getChildByName("lblPopupWinner"));
    this->lblPopupWinner->setString("1:" + boost::to_string(this->SizeListTops));

    history->setContentSize(visibleSize);
    ui::Helper::doLayout(history);
    this->addChild(history);

    auto listener1 = EventListenerTouchOneByOne::create();
    listener1->setSwallowTouches(true);

    listener1->onTouchBegan = CC_CALLBACK_2(LayerTaiXiuPopupWinners::onTouchBegan, this);
    listener1->onTouchEnded = CC_CALLBACK_2(LayerTaiXiuPopupWinners::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, sprPopupBackground_0);

    return true;
}

void LayerTaiXiuPopupWinners::onEnter(){
    Layer::onEnter();
    this->runAction(mUtils::getActionOpenPopup());
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerTaiXiuPopupWinners::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerTaiXiuPopupWinners::onExit(){
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

bool LayerTaiXiuPopupWinners::onTouchBegan(Touch *pTouch, Event *pEvent){
    return true;
}

void LayerTaiXiuPopupWinners::onTouchEnded(Touch *pTouch, Event *pEvent){
    Sprite* target = static_cast<Sprite*>(pEvent->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    if (rect.containsPoint(locationInNode))
    {
       // log("touch in target");
    }
    else
    {
       // log("touch out target");
        this->removeFromParentAndCleanup(true);
    }
}

void LayerTaiXiuPopupWinners::OnBtnNextClick(Ref *sender,  ui::Widget::TouchEventType pType){
    if(pType == ui::Widget::TouchEventType::ENDED) {
        if (this->CurrentHistory == this->SizeListTops - 1 || this->SizeListTops == 0)
        {
            return;
        }
        else
        {
            this->CurrentHistory++;
            string s1 = boost::to_string(this->CurrentHistory + 1);
            string s2 = boost::to_string(this->SizeListTops);

            string str = s1 + ":" + s2;
            this->lblPopupWinner->setString(str);
            this->tblTopWinner->reloadData();
        }
    }
}

void LayerTaiXiuPopupWinners::OnBtnPrevClick(Ref *sender,  ui::Widget::TouchEventType pType){
    if(pType == ui::Widget::TouchEventType::ENDED) {
        if (this->CurrentHistory == 0) {
            return;
        }
        else {
            this->CurrentHistory--;
            string s1 = boost::to_string(this->CurrentHistory + 1);
            string s2 = boost::to_string(this->SizeListTops);

            string str = s1 + ":" + s2;
            this->lblPopupWinner->setString(str);
            this->tblTopWinner->reloadData();
        }
    }
}

std::string LayerTaiXiuPopupWinners::GetLinkAvatar(const std::string& uid){
    boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
    if (room == NULL)
        return "";
    boost::shared_ptr<User> user = room->GetUserByName(uid);
    if (user == NULL)
        return "";
    boost::shared_ptr<UserVariable> usrVar = user->GetVariable("aal");
    if(usrVar == NULL)
        return "";
    boost::shared_ptr<string> name = usrVar->GetStringValue();

    try{
        std::string _name = *name;
        boost::algorithm::trim(_name);
        if(0 == _name.compare(""))
            return "";
    }catch(...){
        return "";
    }

    return *name;
}

void LayerTaiXiuPopupWinners::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell){

}

cocos2d::Size LayerTaiXiuPopupWinners::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx){
    return Size(416, 144);
}

cocos2d::extension::TableViewCell* LayerTaiXiuPopupWinners::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx){
    TableViewCell *cell = table->dequeueCell();

    try {
        std::vector<std::string> arr1 = this->WinnerArray.at(this->CurrentHistory);
        std::string str1 = arr1.at(idx);

        std::vector<std::string> info = mUtils::splitString(str1, ':');

        Label* lblName = NULL;
        Label* lblMoney = NULL;
        CircleAvatar* ava = NULL;

        std::string money = "0";
        std::string name = info.at(1);
        boost::algorithm::trim(name);
        if(0 == name.compare("")){
            name = info.at(0);
        }

        PlayerChanInfo pInfo = ChanUtils::GetPlayerInfo(info.at(0));

        money = mUtils::convertMoneyEx(atoi(info.at(2).c_str()));

        if (!cell) {
            float y = this->tableCellSizeForIndex(table, idx).height / 2;

            cell = new TableViewCell();
            cell->autorelease();
            cell->setIdx(idx);
            cell->setAnchorPoint(Vec2(0, 0));
            cell->setPosition(Vec2(0, 0));

            Sprite* sprite = Sprite::create("itemTX.png");
            sprite->setAnchorPoint(Vec2(0, 0));
            sprite->setPosition(Vec2(0, 0));
            cell->addChild(sprite);

            ava = CircleAvatar::create();
            ava->setAnchorPoint(Vec2(0, 0));
            ava->setPosition(Vec2(70, y));
            ava->setScale(0.7);
            ava->setTag(212);
            ava->setAvatarImage(pInfo.aal);
            ava->setTypeVip(pInfo.vip);
            cell->addChild(ava);

            lblName = Label::createWithBMFont(LucidaGrandeFnt, info.at(1));
            lblName->setAnchorPoint(Vec2(0, 0.5));
            lblName->setPosition(Vec2(130, y + 30));
            lblName->setTag(123);
            lblName->setScale(1.3);
            cell->addChild(lblName);

            Sprite* icc = Sprite::create("button_xu.png");
            icc->setAnchorPoint(Vec2(0, 0.5));
            icc->setPosition(Vec2(130, y - 30));
            cell->addChild(icc);

            lblMoney = Label::createWithBMFont(LucidaGrandeFnt, money);
            lblMoney->setAnchorPoint(Vec2(0, 0.5));
            lblMoney->setPosition(Vec2(170, y - 30));
            lblMoney->setTag(124);
            lblMoney->setScale(1.2);
            lblMoney->setColor(Color3B::YELLOW);
            cell->addChild(lblMoney);

        } else {
            ava = (CircleAvatar*)cell->getChildByTag(212);
            if(ava != NULL){
                ava->setAvatarImage(pInfo.aal);
                ava->setTypeVip(pInfo.vip);
            }

            lblName = (Label*)cell->getChildByTag(123);
            if(lblName != NULL) {
                lblName->setString(info.at(1));
            }

            lblMoney = (Label*)cell->getChildByTag(124);
            if(lblMoney != NULL) {
                lblMoney->setString(money);
            }
        }

    } catch(std::out_of_range& e){
       // log("%s", e.what());
    } catch(std::exception& e){
       // log("%s", e.what());
    } catch(...){

    }

    return cell;
}

ssize_t LayerTaiXiuPopupWinners::numberOfCellsInTableView(cocos2d::extension::TableView *table){
    try{
        return this->WinnerArray.at(this->CurrentHistory).size();
    }catch(std::out_of_range&e){
        return 0;
    }catch(std::exception& e){
        return 0;
    }catch(...){
        return 0;
    }
}


