
#include "LayerTaiXiuHistory.h"
#include "AllData.h"
#include "mUtils.h"
#include "../Common.h"

LayerTaiXiuHistory::LayerTaiXiuHistory()
	:tblHistory(NULL)
{
}

LayerTaiXiuHistory::LayerTaiXiuHistory(const std::vector<std::string>& histories)
    :tblHistory(NULL)
{
    this->SetListHistory(histories);
}

LayerTaiXiuHistory::~LayerTaiXiuHistory()
{
	this->ListHistory.clear();
}

LayerTaiXiuHistory* LayerTaiXiuHistory::create(const std::vector<std::string>& P)
{
    LayerTaiXiuHistory* History = new LayerTaiXiuHistory(P);
    if(History && History->init()){
        History->autorelease();
        return History;
    }
    else{
        delete History;
        History = NULL;
        return NULL;
    }
}

static inline void printtff(Node* node, int level)
{
    std::string spaces(level, ' ');
   // log("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());

    ++level;
    int index = 0;

    while (index < node->getChildren().size()){
        Node* obj = node->getChildren().at(index);
        printtff(static_cast<Node*>(obj), level);
        index++;
    }
}

bool LayerTaiXiuHistory::init()
{
	if(!Layer::init()){
		return false;
	}

    this->setAnchorPoint(Vec2::ZERO);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Node* history = CSLoader::getInstance()->createNode("LayerTaiXiuPopupHistory.csb");

    printtff(history, 0);

    Layout* layout = static_cast<Layout*>(history->getChildByName("Panel_1"));

    if(layout != NULL){
        layout->setVisible(true);
        layout->setOpacity(150);
    }

    Sprite* _sprite = static_cast<cocos2d::Sprite*>(history->getChildByName("sprPopupBackground"));
    if(_sprite != NULL){
        this->tblHistory = TableView::create(this, Size(410, 715));
        this->tblHistory->setDirection(TableView::Direction::VERTICAL);
        this->tblHistory->setPosition(Vec2(14, 35));
        this->tblHistory->setDelegate(this);
        this->tblHistory->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
        _sprite->addChild(tblHistory);
    }

    history->setContentSize(visibleSize);
    ui::Helper::doLayout(history);
    this->addChild(history);

    EventListenerTouchOneByOne* listener1 = EventListenerTouchOneByOne::create();
    listener1->setSwallowTouches(true);
    listener1->onTouchBegan = CC_CALLBACK_2(LayerTaiXiuHistory::onTouchBegan, this);
    listener1->onTouchEnded = CC_CALLBACK_2(LayerTaiXiuHistory::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _sprite);

    return true;
}

bool LayerTaiXiuHistory::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
    return true;
}

void LayerTaiXiuHistory::onTouchEnded(Touch *pTouch, Event *pEvent){
    auto target = static_cast<Sprite*>(pEvent->getCurrentTarget());
    Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);

    if (rect.containsPoint(locationInNode))
    {

    }
    else
    {
        this->removeFromParentAndCleanup(true);
    }
}

void LayerTaiXiuHistory::onEnter()
{
    Layer::onEnter();
    this->runAction(mUtils::getActionOpenPopup());
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LayerTaiXiuHistory::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _touchListener = listener;
}

void LayerTaiXiuHistory::onExit()
{
    EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(_touchListener);
    Layer::onExit();
}

void LayerTaiXiuHistory::SetListHistory(const std::vector<std::string>& P)
{
    this->ListHistory.clear();
    for (int i = P.size() - 1; i >= 0; --i) {
        try {
            if (P.at(i) == "")
                continue;
            this->ListHistory.push_back(P.at(i));
        } catch (std::out_of_range& e) {
           // log("%s", e.what());
        }
    }
}

void LayerTaiXiuHistory::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{

}

cocos2d::Size LayerTaiXiuHistory::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    return Size(416, 144);
}

cocos2d::extension::TableViewCell* LayerTaiXiuHistory::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    Label* label;

    ImageView* img1;
    ImageView* img2;
    ImageView* img3;

    string str = "";
    Color3B ccl = Color3B::WHITE;
    vector<string> info = mUtils::splitString(this->ListHistory.at(idx), '_');
    int val = 0;

    for (int i = 0; i < info.size(); ++i) {
        val += atoi(info.at(i).c_str());
    }

    try {
        if (atoi(info.at(0).c_str()) == atoi(info.at(1).c_str()) && atoi(info.at(0).c_str()) == atoi(info.at(2).c_str())) {
            str = "Bão";
        }
        else {
            if (val >= 4 && val <= 10) {
                str = dataManager.GetSysString(709);
            }
            else {
                str = dataManager.GetSysString(671);
            }
        }
    } catch (std::out_of_range& e) {
       // log("%s", e.what());
    } catch(std::exception& e){
       // log("%s", e.what());
    }

    if (!cell) {

        cell = new TableViewCell();
        cell->autorelease();
        cell->setIdx(idx);
        cell->setAnchorPoint(Vec2(0, 0));
        cell->setPosition(Vec2(0, 0));

        Sprite* sprite = Sprite::create("itemTX.png");
        sprite->setAnchorPoint(Vec2(0, 0));
        sprite->setPosition (Vec2(0, 0));
        cell->addChild(sprite);

        float y = this->tableCellSizeForIndex(table, idx).height / 2;

        label = Label::createWithSystemFont(str,"", 25);
        label->setAnchorPoint(Vec2(0, 0.5));
        label->setPosition (Vec2(270, y));
        label->setTag(123);
        label->setScale(1.6);
        label->setColor(ccl);
        cell->addChild(label);

        img1 = ImageView::create();
        img1->loadTexture(this->GetUrlXiNgau(info.at(0)));
        img1->setAnchorPoint(Vec2(0, 0.5));
        img1->setPosition(Vec2(20, y));
        img1->setTag(124);
        img1->setScale(0.8);
        cell->addChild(img1);

        img2 = ImageView::create();
        img2->loadTexture(this->GetUrlXiNgau(info.at(1)));
        img2->setAnchorPoint(Vec2(0, 0.5));
        img2->setPosition(Vec2(100, y));
        img2->setTag(125);
        img2->setScale(0.8);
        cell->addChild(img2);

        img3 = ImageView::create();
        img3->loadTexture(this->GetUrlXiNgau(info.at(2)));
        img3->setAnchorPoint(Vec2(0, 0.5));
        img3->setPosition(Vec2(180, y));
        img3->setTag(126);
        img3->setScale(0.8);
        cell->addChild(img3);

    } else {
        cell->setIdx(idx);
        label = (Label*)cell->getChildByTag(123);
        label->setColor(ccl);
        label->setString(str);

        img1 = (ImageView*)cell->getChildByTag(124);
        img1->loadTexture(this->GetUrlXiNgau(info.at(0)));

        img2 = (ImageView*)cell->getChildByTag(125);
        img2->loadTexture(this->GetUrlXiNgau(info.at(1)));

        img3 = (ImageView*)cell->getChildByTag(126);
        img3->loadTexture(this->GetUrlXiNgau(info.at(2)));
    }
    return cell;
}

ssize_t LayerTaiXiuHistory::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
    try{
        return this->ListHistory.size();
    }catch(...){
        return 0;
    }
}

string LayerTaiXiuHistory::GetUrlXiNgau(string& number){
    string num = "tx-"+ number +"point.png";
    return num;
}

