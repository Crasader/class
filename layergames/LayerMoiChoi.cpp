//
//  LayerMoiChoi.cpp
//  iCasinov2
//
//  Created by AlexDao on 6/3/14.
//
//

#include "LayerMoiChoi.h"
#include "mUtils.h"
#include "Requests/ExtensionRequest.h"
#include "GameServer.h"
#include "AllData.h"
#include "../SceneManager.h"

using namespace cocos2d;
//using namespace CocosDenshion;

LayerMoiChoi::LayerMoiChoi()
{
    nodeTable = NULL;
    tblTable = NULL;
	lblTitle = NULL;
    //
    GameServer::getSingleton().addListeners(this);
}

LayerMoiChoi::~LayerMoiChoi()
{
    GameServer::getSingleton().removeListeners(this);
	//
	CC_SAFE_RELEASE(nodeTable);
	CC_SAFE_RELEASE(lblTitle);
	log("LayerMoiChoi Destructor");
}

void LayerMoiChoi::loadAllDatas(){
	//get game id
    if (SceneManager::getSingleton().getCurrRoomType() == 0){
        if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
            || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")==NULL
            || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL|| GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") == NULL)
            return;
        int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0,3).c_str());
        boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
        string paramString = param->c_str();
        vector<string> arrInfo = mUtils::splitString(paramString, '@');
        double gameBet = atof(arrInfo.at(0).c_str());
        int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
        
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("maqp", 10);
        params->PutDouble("mimp", gameBet);
        params->PutInt("amf", amf_value);
        params->PutInt("gid", gid);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
        GameServer::getSingleton().Send(request);
    }else{
        if (GameServer::getSingleton().getSmartFox()->LastJoinedRoom() == NULL
            || GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")==NULL
            || GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf") == NULL|| GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amfs") == NULL)
            return;
        int gid = atoi(GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->substr(0,3).c_str());
        boost::shared_ptr<string> param = GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue();
        string paramString = param->c_str();
        vector<string> arrInfo = mUtils::splitString(paramString, '@');
        double gameBet = atof(arrInfo.at(0).c_str());
        int amf_value = (int)*GameServer::getSingleton().getSmartFox()->MySelf()->GetVariable("amf")->GetIntValue();
        
        //
        boost::shared_ptr<ISFSObject> params(new SFSObject());
        params->PutInt("maqp", 10);
        params->PutDouble("mimp", gameBet);
        params->PutInt("amfs", amf_value);
        params->PutInt("gid", gid);
        boost::shared_ptr<IRequest> request(new ExtensionRequest("rglip", params));
        GameServer::getSingleton().Send(request);
    }
	
}

void LayerMoiChoi::onButtonClose(Ref* pSender){
    this->removeFromParentAndCleanup(true);
}

// CCBSelectorResolver interface
SEL_MenuHandler LayerMoiChoi::onResolveCCBCCMenuItemSelector(Ref *pTarget, const char *pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "btnClose", LayerMoiChoi::onButtonClose);
    return NULL;
}

// CCBMemberVariableAssigner interface
bool LayerMoiChoi::onAssignCCBMemberVariable(Ref *pTarget, const char *pMemberVariableName, cocos2d::Node *pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "nodeTable", Node *, nodeTable);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "lblTitle", Label *, lblTitle);
    return true;
}

void LayerMoiChoi::onNodeLoaded( Node * pNode,  NodeLoader * pNodeLoader)
{
    //Init for table
    tblTable = TableView::create(this, nodeTable->getContentSize());
    tblTable->setDirection(TableView::Direction::VERTICAL);
    tblTable->setAnchorPoint(Point(0, 0));
    tblTable->setPosition(Point(0, 0));
    tblTable->setDelegate(this);
	tblTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//tblTable->setTouchPriority(-128);
    
    nodeTable->addChild(tblTable);
    
    tblTable->reloadData();
    //
    lblTitle->setString(dataManager.GetSysString(620));
	//
    return;
}

// hàm khi click vào 1 hành của table view
void LayerMoiChoi::tableCellTouched(cocos2d::extension::TableView *table, cocos2d::extension::TableViewCell *cell){
   // log("Roomid: %d", cell->getTag());
	int idx = cell->getTag();
	StructFriendInfo sms = lstFriendInfos.at(idx);
// 	if( sms==NULL )
// 		return;
	//
	if( GameServer::getSingleton().getSmartFox()->LastJoinedRoom()==NULL
		|| GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")==NULL)
		return;
	boost::shared_ptr<User> myself = GameServer::getSingleton().getSmartFox()->MySelf();
	int groupId = atoi( GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str() );
	vector<string> lstParams = mUtils::splitString( *GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GetVariable("params")->GetStringValue(), '@' );
	//double mb = atof(lstParams.at(0).c_str());
	//Send request
	boost::shared_ptr<ISFSObject> params (new SFSObject());
	params->PutUtfString("gid", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->GroupId()->c_str());
	params->PutUtfString("mb", lstParams.at(0).c_str());
	params->PutUtfString("roomid", StringUtils::format("%d", GameServer::getSingleton().getSmartFox()->LastJoinedRoom()->Id()) );
	params->PutUtfString("lu", sms.aI.c_str());
	boost::shared_ptr<IRequest> request (new ExtensionRequest("rilu", params));
	GameServer::getSingleton().Send(request);

	//remove it from list
	lstFriendInfos.erase( lstFriendInfos.begin()+idx );
	tblTable->reloadData();
}

// Hàm set giá trị width height cho 1 cell table view
Size LayerMoiChoi::tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx){
    return Size(nodeTable->getContentSize().width, 40);
}

// Hàm này tạo 1 tableview Row để add vào table view
TableViewCell* LayerMoiChoi::tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx){
    TableViewCell *cell = table->dequeueCell();
    StructFriendInfo sms = lstFriendInfos.at(idx);
    if (!cell) {
        cell = new TableViewCell();
        cell->autorelease();
        //Time
		Label *labelName = Label::createWithSystemFont(StringUtils::format("%s - %s xu"
			, sms.aN.c_str(), mUtils::convertMoneyEx(atof(sms.amf.c_str())).c_str())
			, "", 16.0);

        labelName->setPosition(Point(nodeTable->getContentSize().width/2, 20));
		labelName->setAnchorPoint(Point(0.5, 0.5));
		labelName->setTag(1);

		cell->addChild(labelName);
		log("uid: %d", sms.uid);
		cell->setTag(idx);

		Sprite* line = Sprite::createWithSpriteFrameName("assest/background_cell.png");
        line->setPosition(Point(nodeTable->getContentSize().width/2,0));
        line->setScaleX(nodeTable->getContentSize().width/line->getContentSize().width);
        line->setAnchorPoint(Point(0.5,0));
        cell->addChild(line);
    }
    else
    {
		cell->setTag(idx);
		Label *label1 = (Label *)cell->getChildByTag(1);
		if( label1 != NULL )
			label1->setString( StringUtils::format("%s - %s xu", sms.aN.c_str(), mUtils::convertMoneyEx( atof(sms.amf.c_str()) ).c_str() ));
    }
    return cell;
}

// Hàm gán giá trị số hàng của table view
ssize_t LayerMoiChoi::numberOfCellsInTableView(cocos2d::extension::TableView *table){
    return lstFriendInfos.size();
}

void LayerMoiChoi::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent){
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
    boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));
	log("cmd=%s", cmd->c_str());
	if( strcmp( cmd->c_str(), "lip" )==0 ){
		boost::shared_ptr<string> lipv = param->GetUtfString("lipv");
		log("lipv: %s", lipv->c_str());
		if( strlen(lipv->c_str())==0 ){
			//NoBody aka HoangDD
		}else{
			//25,dautv12,dautv12,1315918.0,|
			log("ListFriendInvite: %s", lipv->c_str());
			vector<string> lstFriends = mUtils::splitString(*lipv, '|');
			lstFriendInfos.clear();
			for( int i = 0; i<lstFriends.size(); i++ ){
				if( strcmp( lstFriends.at(i).c_str(), "" )==0 )
					continue;
				vector<string> friendInfo = mUtils::splitString(lstFriends.at(i), ',');
				StructFriendInfo info;
				info.uid = atoi( friendInfo.at(0).c_str() );
				info.aI = friendInfo.at(1);
				info.aN = friendInfo.at(2);
				info.amf = friendInfo.at(3);

				lstFriendInfos.push_back( info );
			}  
			tblTable->reloadData();
		}
	}
}

// void LayerMoiChoi::registerWithTouchDispatcher( void )
// {
// 	Director::getInstance()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
// }

bool LayerMoiChoi::onTouchBegan( cocos2d::Touch *pTouch, cocos2d::Event *pEvent )
{
	return true;
}

void LayerMoiChoi::onEnter()
{
	Layer::onEnter();
	//
	this->runAction(mUtils::getActionOpenPopup());
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(LayerMoiChoi::onTouchBegan, this);
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	_touchListener = listener;
}

void LayerMoiChoi::onExit()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);
	Layer::onExit();
}
