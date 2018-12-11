#include "LayerRankContent.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SceneManager.h"
#include "Requests/ExtensionRequest.h"
#include "../layers/LayerRank.h"
#include "../mUtils.h"
#include "AllData.h"
#include "layergames/PhomMessDef.h"
#include "../Common.h"
USING_NS_CC;
#define TAG_LEFT_CELL 246
#define TAG_RIGHT_CELL 247
LayerRankContent::LayerRankContent()
{
	GameServer::getSingleton().addListeners(this);
}


LayerRankContent::~LayerRankContent()
{
	GameServer::getSingleton().removeListeners(this);
	lstRanks.clear();
	lstTops.clear();
}

bool LayerRankContent::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto rootNode = CSLoader::getInstance()->createNode("LayerRankMiddle.csb");

	nodeTopCaoThu = Node::create();
	nodeTopDaigia = Node::create();

	ui::Helper::doLayout(rootNode);
	rootNode->setPosition(Point());
	nodeTopCaoThu->addChild(rootNode);
	this->addChild(nodeTopCaoThu);

    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        isWebView = false;
    }

	this->loadDatas();
	auto ratio = visibleSize.height / DESIGN_RESOLUTION_HEIGHT;
	panelLeft = static_cast<Layout*>(rootNode->getChildByName("panelLeft"));
	panelRight = static_cast <Layout*>(rootNode->getChildByName("panelRight"));
	panelLeft->setVisible(false);
	panelRight->setVisible(false);
	panelLeft->setContentSize(Size(panelLeft->getContentSize().width, visibleSize.height - TOP_HEIGHT * 2 + BORDER_TOP_MENU * ratio));
	panelRight->setContentSize(Size(panelRight->getContentSize().width, visibleSize.height - TOP_HEIGHT * 2 + BORDER_TOP_MENU * ratio));

	tblLeft = TableView::create(this, Size(panelLeft->getContentSize().width, panelLeft->getContentSize().height));
	tblLeft->setAnchorPoint(panelLeft->getAnchorPoint());
	tblLeft->setContentSize(panelLeft->getContentSize());
	tblLeft->setDirection(TableView::Direction::VERTICAL);
	tblLeft->setPosition(Vec2(panelLeft->getPositionX(), panelLeft->getPositionY() + 50));
	tblLeft->setDelegate(this);
	tblLeft->setTag(tagTableLeft);
	tblLeft->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	rootNode->addChild(tblLeft);

	tblRight = TableView::create(this, Size(panelRight->getContentSize().width, panelRight->getContentSize().height));
	tblRight->setAnchorPoint(panelRight->getAnchorPoint());
	tblRight->setContentSize(panelRight->getContentSize());
	tblRight->setDirection(TableView::Direction::VERTICAL);
	tblRight->setPosition(Vec2(panelRight->getPositionX(), panelRight->getPositionY() + 50));
	tblRight->setDelegate(this);
	tblRight->setTag(tagTableRight);
	tblRight->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	rootNode->addChild(tblRight);


	tblTopDaiGia = TableView::create(this, Size(this->getContentSize().width, panelRight->getContentSize().height));
	tblTopDaiGia->setAnchorPoint(panelRight->getAnchorPoint());
	tblTopDaiGia->setDirection(TableView::Direction::VERTICAL);
	tblTopDaiGia->setPosition(Vec2(0, panelRight->getPositionY() + 50));
	tblTopDaiGia->setDelegate(this);
	tblTopDaiGia->setTag(tagTableTopDaiGia);
	tblTopDaiGia->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	nodeTopDaigia->addChild(tblTopDaiGia);

	this->addChild(nodeTopDaigia);
	this->nodeTopDaigia->setVisible(false);
    
    if (isWebView){
        
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        webView = cocos2d::experimental::ui::WebView::create();
        webView->setPosition(Point(0, 0));
        webView->setAnchorPoint(Vec2(0,0));
        webView->setContentSize(Size(visibleSize.width, visibleSize.height - 137));
        SceneManager::getSingleton().showLoading();
        //webView->setScaleZ(2);
        webView->loadURL(LINK_BXH);
        webView->setScalesPageToFit(true);
        webView->setLocalZOrder(99);
        this->addChild(webView);
        webView->setVisible(false);
        
        webView->setOnShouldStartLoading(CC_CALLBACK_2(LayerRankContent::onWebViewShouldStartLoading, this));
        webView->setOnDidFinishLoading(CC_CALLBACK_2(LayerRankContent::onWebViewDidFinishLoading, this));
        webView->setOnDidFailLoading(CC_CALLBACK_2(LayerRankContent::onWebViewDidFailLoading, this));
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		Application::getInstance()->openURL(LINK_BXH);
#endif
                      
        return true;
    }
	
	this->loadFirst();
	//add event listenner

	return true;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
bool LayerRankContent::onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().showLoading();
    webView->setVisible(false);
    return true;
    
}

void LayerRankContent::onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
    webView->setVisible(true);
}

void LayerRankContent::onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url)
{
    SceneManager::getSingleton().hideLoading();
}
#endif
void LayerRankContent::loadDatas()
{
    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
        vector<int> lstgid;
        
        lstgid.push_back(112);
        lstgid.push_back(111);
        lstgid.push_back(102);
        lstgid.push_back(109);
        lstgid.push_back(116);
        lstgid.push_back(117);
        lstgid.push_back(100);
        lstgid.push_back(103);
        lstgid.push_back(106);
        
        for (int i = 0; i<lstgid.size();i++){
            int gid = lstgid[i];
            if (gid == 113 || gid == 114 || gid==115 || gid == 104|| gid == 105|| gid == 101|| gid == 107|| gid == 108 || gid == 110) continue;
            if (strstr(dataManager.getAppConfig().sListHiddenGame.c_str(),boost::to_string(i).c_str()))
                continue;
            StructRank rank;
            rank.ID = gid;
            rank.Name = mUtils::getGameNameByID(gid);
            lstRanks.push_back(rank);
        }
    }else{
        for (int i = 100; i<=117;i++){
            if (i == 113 || i == 114 || i==115 || i == 104|| i == 105|| i == 101|| i == 106|| i == 107|| i == 108 || i == 110) continue;
            if (strstr(dataManager.getAppConfig().sListHiddenGame.c_str(),boost::to_string(i).c_str()))
                continue;
            StructRank rank;
            rank.ID = i;
            rank.Name = mUtils::getGameNameByID(i);
            lstRanks.push_back(rank);
        }
    }
   
}

void LayerRankContent::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
	boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
	boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
	boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));

	boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	if (strcmp("ftbfree", cmd->c_str()) == 0){
		SceneManager::getSingleton().hideLoading();
		lstTops.clear();
		vector<string> rankMoneys = mUtils::splitString(*param->GetUtfString("ftbfree"), '|');
		for (int i = 0; i < rankMoneys.size(); i++)
		{
			infoUser _i;
			vector<string> infos = mUtils::splitString(rankMoneys.at(i), ',');
			for (int j = 0; j < infos.size(); j++)
			{
				_i.userName = infos.at(0);
				_i.userLv = infos.at(1);
				_i.userMoney = infos.at(2);
				_i.userRateWin = "";
			}
			lstTops.push_back(_i);
		}
		tblTopDaiGia->reloadData();
		return;
	}
	else if (strcmp("rahbg", cmd->c_str()) == 0)
	{
		lstTops.clear();
		SceneManager::getSingleton().hideLoading();
		vector<string> rankGame = mUtils::splitString(*param->GetUtfString("rahbg"), '|');
		for (int i = 0; i < rankGame.size(); i++)
		{
			infoUser _i;
			vector<string> info = mUtils::splitString(rankGame.at(i), ',');
			_i.userId = info.at(0);
			_i.userLv = info.at(1);
			_i.userRateWin = "";
			_i.userName = info.at(0);
			long long a = atoll(info.at(2).c_str());
			long long b = atoll(info.at(3).c_str());
			auto c = a - b > 0 ? a - b : 0;
			_i.userMoney = mUtils::convertMoneyEx(c);
			this->lstTops.push_back(_i);
		}
		tblRight->reloadData();
	}
}

void LayerRankContent::tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell)
{
	if (table->getTag() == tagTableLeft) {
		//Get all info
		SceneManager::getSingleton().showLoading();
		boost::shared_ptr<ISFSObject> params(new SFSObject());

		//Rank game

		params->PutInt("gid", lstRanks.at(cell->getIdx()).ID);
		boost::shared_ptr<IRequest> request(new ExtensionRequest("rahbg", params));
		GameServer::getSingleton().Send(request);

		//Uncheck other cells
		for (auto i = 0; i < lstRanks.size(); i++) {
			auto cc = tblLeft->cellAtIndex(i);
			if (cc == NULL)
				continue;
			LayerRankLeftCell* child = (LayerRankLeftCell*)cc->getChildByTag(TAG_LEFT_CELL);
			child->bgPressed->setVisible(false);
            child->txtGameName->setTextColor(Color4B::WHITE);
		}
		LayerRankLeftCell* child = (LayerRankLeftCell*)cell->getChildByTag(TAG_LEFT_CELL);
		child->bgPressed->setVisible(true);
        child->txtGameName->setTextColor(Color4B::BLACK);
		idxChoosed = cell->getIdx();
	}
	else if (table->getTag() == tagTableRight) {

	}
}

cocos2d::Size LayerRankContent::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	if (table->getTag() == tagTableLeft) {
		return Size(panelLeft->getContentSize().width, 175);
	}
	else if (table->getTag() == tagTableTopDaiGia){
		return Size(this->getContentSize().width, 163);
	}
	return Size(panelRight->getContentSize().width, 163);
}

cocos2d::extension::TableViewCell* LayerRankContent::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx)
{
	if (table->getTag() == tagTableLeft)
		return createCell4Left(table, idx);
	else if (table->getTag() == tagTableRight)
		return createCell4Right(table, idx);
	else
		return createCell4TopDaiGia(table, idx);
	return NULL;
}

ssize_t LayerRankContent::numberOfCellsInTableView(cocos2d::extension::TableView *table)
{
	if (table->getTag() == tagTableLeft) {
		return lstRanks.size();//
	}
	return lstTops.size();
}

TableViewCell* LayerRankContent::createCell4Left(TableView *table, int idx)
{
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = new TableViewCell();
		auto rankCell = LayerRankLeftCell::create();
		rankCell->setPosition(Vec2::ZERO);
		rankCell->setTag(TAG_LEFT_CELL);
		rankCell->setDatas(lstRanks.at(idx).ID, lstRanks.at(idx).Name->c_str());
		cell->addChild(rankCell);
		rankCell->bgPressed->setVisible(false);
		cell->setIdx(idx);
	}
	else
	{
		cell->setIdx(idx);
		LayerRankLeftCell* rankCell = (LayerRankLeftCell*)cell->getChildByTag(TAG_LEFT_CELL);
		if (rankCell){
			rankCell->setDatas(lstRanks.at(idx).ID, lstRanks.at(idx).Name->c_str());
			rankCell->bgPressed->setVisible(false);
            rankCell->txtGameName->setTextColor(Color4B::WHITE);
            if (idxChoosed == idx){
                rankCell->bgPressed->setVisible(true);
                rankCell->txtGameName->setTextColor(Color4B::BLACK);
            }

		}
	}
	return cell;
}

TableViewCell* LayerRankContent::createCell4Right(TableView *table, int idx)
{
	TableViewCell* cell = table->dequeueCell();
	try{
		if (!cell)
		{
			cell = new TableViewCell();
			LayerRankRightCell* rankCell = LayerRankRightCell::create();
			rankCell->setPosition(Vec2::ZERO);
			rankCell->setTag(TAG_RIGHT_CELL);
			rankCell->setDatas(idx, this->lstTops.at(idx));
			cell->addChild(rankCell);
		}
		else
		{
			LayerRankRightCell* rankCell = (LayerRankRightCell*)cell->getChildByTag(TAG_RIGHT_CELL);
			rankCell->setDatas(idx, this->lstTops.at(idx));
		}
	}
	catch (std::out_of_range& e) {
		log("%s", e.what());
	}
	catch (...) {

	}
	return cell;
}

void LayerRankContent::loadFirst()
{
	tblLeft->reloadData();
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	params->PutInt("gid", lstRanks.at(0).ID);
	boost::shared_ptr<IRequest> request(new ExtensionRequest("rahbg", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();

	//reset all cell
	for (int i = 0; i < lstRanks.size(); ++i)
	{
		auto cell = tblLeft->cellAtIndex(i);
		if (cell)
		{
			LayerRankLeftCell* child = (LayerRankLeftCell*)cell->getChildByTag(TAG_LEFT_CELL);
			if (!child) continue;
			
			if (i == 0)
			{
				child->bgPressed->setVisible(true);
                child->txtGameName->setTextColor(Color4B::BLACK);

			}
			else{
				child->bgPressed->setVisible(false);
                child->txtGameName->setTextColor(Color4B::WHITE);

			}

		}
	}
	idxChoosed = 0;
	
// 	auto cc = tblLeft->cellAtIndex(0);
// 	if (!cc) return;

}

void LayerRankContent::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{

}

void LayerRankContent::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{

}

void LayerRankContent::loadTopDaiGia()
{
	boost::shared_ptr<ISFSObject> params(new SFSObject());
	boost::shared_ptr<IRequest> request(new ExtensionRequest("ftbfree", params));
	GameServer::getSingleton().Send(request);
	SceneManager::getSingleton().showLoading();
}

TableViewCell* LayerRankContent::createCell4TopDaiGia(TableView *table, int idx)
{
	TableViewCell* cell = table->dequeueCell();
	try {
		if (!cell)
		{
			cell = new TableViewCell();
			LayerRankTopDaiGia* rankCell = LayerRankTopDaiGia::create();
			rankCell->setPosition(Vec2::ZERO);
			rankCell->setTag(123);
			rankCell->setDatas(idx, lstTops.at(idx));
			cell->addChild(rankCell);
		}
		else
		{
			LayerRankTopDaiGia* rankCell = (LayerRankTopDaiGia*)cell->getChildByTag(123);
			rankCell->setDatas(idx, lstTops.at(idx));
		}
	}
	catch (std::out_of_range& e) {
		log("%s", e.what());
	}
	catch (std::exception& e) {
		log("%s", e.what());
	}
	catch (...) {

	}
	return cell;
}

LayerRankRightCell::LayerRankRightCell() : circleAvatar(NULL) {

}

LayerRankRightCell::~LayerRankRightCell(){
	log("deconstructor");
}

bool LayerRankRightCell::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::getInstance()->createNode("LayerItemRankRight.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	this->txtName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	this->txtMoney = static_cast<Text*>(rootNode->getChildByName("lblTop"));
	this->txtLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	this->lblTop = dynamic_cast<Text*>(rootNode->getChildByName("lblPos"));
	this->spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("spriteAvatar"));
	this->avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	this->avtVip->setVisible(false);

	ImageView* img = static_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
	if (img != NULL){
		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->hideIconDefault(false);
		float scX = img->getScaleX();
		float scY = img->getScaleY();
		circleAvatar->displayTypeVip(false);
		Size _sizeImg = img->getContentSize();
		Size _sizeAva = this->circleAvatar->GetSizeAvatar();

		scX = _sizeImg.width / _sizeAva.width * scX;
		scY = _sizeImg.height / _sizeAva.height * scY;

		this->circleAvatar->setAnchorPoint(Vec2::ZERO);
		this->circleAvatar->setScale(scX, scY);
		this->circleAvatar->setPosition(img->getPosition());
		rootNode->addChild(this->circleAvatar, 3);
		this->avtVip->setLocalZOrder(4);
	}

	return true;
}

void LayerRankRightCell::setDatas(const int& index, const infoUser& data)
{
	//     this->spriteTop->setVisible(index < 3);
	// 
	//     if (index < 3) {
	// 		string _src = StringUtils::format("top-0%d.png", index + 1);
	//         this->spriteTop->setTexture(_src);
	// 	}
	lblTop->setString(StringUtils::format("%d", index + 1));
	this->txtName->setString(data.userName);
	this->txtMoney->setString(StringUtils::format("Thắng %s %s", data.userMoney.c_str(),SceneManager::getSingleton().getCoin(0).c_str()));
	this->txtLevel->setString("Level: " + data.userLv);

	if (this->circleAvatar != NULL) {
		this->circleAvatar->setAvatarImage(data.userAvatar);
	}
}

bool LayerRankLeftCell::init()
{

	if (!Layer::init())
	{
		return false;
	}
	auto rootNode = CSLoader::getInstance()->createNode("LayerItemRankLeft.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	btnIconGame = static_cast<ImageView*>(rootNode->getChildByName("imgGame"));
	txtGameName = static_cast<Text*>(rootNode->getChildByName("lblGameName"));
	//txtGameName->setFontSize(FONT_SIZE_NORMAL);
	bgPressed = static_cast<ImageView*>(rootNode->getChildByName("imgBg"));
	bgPressed->setVisible(false);
	return true;
}

void LayerRankLeftCell::setDatas(int gameID, string gameName)
{
	txtGameName->setString(gameName);
	if (gameID >= 100)
		btnIconGame->loadTexture(mUtils::getUrlImageGame(gameID));
}

LayerRankTopDaiGia::LayerRankTopDaiGia() : circleAvatar(NULL){

}

LayerRankTopDaiGia::~LayerRankTopDaiGia(){

}

bool LayerRankTopDaiGia::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::getInstance()->createNode("LayerItemRankTopDaiGia.csb");
	ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	this->txtName = static_cast<Text*>(rootNode->getChildByName("lblName"));
	this->txtMoney = static_cast<Text*>(rootNode->getChildByName("lblMoney"));
	this->txtLevel = static_cast<Text*>(rootNode->getChildByName("lblLevel"));
	this->lblTop = dynamic_cast<Text*>(rootNode->getChildByName("lblPos"));
	this->spriteAvatar = dynamic_cast<Sprite*>(rootNode->getChildByName("spriteAvatar"));
	this->avtVip = static_cast<ImageView*>(rootNode->getChildByName("imgAvatarVip"));
	this->avtVip->setVisible(false);

	ImageView* img = static_cast<ImageView*>(rootNode->getChildByName("imgAvatar"));
	if (img != NULL){
		this->circleAvatar = CircleAvatar::create();
		this->circleAvatar->hideIconDefault(false);
		float scX = img->getScaleX();
		float scY = img->getScaleY();

		Size _sizeImg = img->getContentSize();
		Size _sizeAva = this->circleAvatar->GetSizeAvatar();

		scX = _sizeImg.width / _sizeAva.width * scX;
		scY = _sizeImg.height / _sizeAva.height * scY;

		this->circleAvatar->setAnchorPoint(Vec2::ZERO);
		this->circleAvatar->setScale(scX, scY);
		this->circleAvatar->setPosition(img->getPosition());
		rootNode->addChild(this->circleAvatar, 3);
		this->avtVip->setLocalZOrder(4);
	}

	return true;
}

void LayerRankTopDaiGia::setDatas(const int& index, const infoUser& data)
{
	//     this->spriteTop->setVisible(index < 3);
	//     if (index < 3) {
	// 		string _src = StringUtils::format("top-0%d.png", index + 1);
	//         this->spriteTop->setTexture(_src);
	// 	}
	lblTop->setString(StringUtils::format("%d", index + 1));
	this->txtName->setString(data.userName);
	long long b = atoll(data.userMoney.c_str());
	string a = mUtils::convertMoneyEx(b);
	this->txtMoney->setString(a);
	this->txtLevel->setString(data.userRateWin);

	if (this->circleAvatar != NULL){
		this->circleAvatar->setAvatarImage(data.userAvatar);
	}
}
