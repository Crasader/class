
#include "LayerGameChan_XuongU.h"
#include "mUtils.h"
#include "CustomTableViewCell.h"
#include "Requests/ExtensionRequest.h"
#include "SceneManager.h"
#include "ChanUtils.h"
#include "xmllite/xmlParser.h"

using namespace cocos2d;

LayerGameChan_XuongU::LayerGameChan_XuongU()
	:LblTime(NULL),
	LblTitle(NULL),
    CountTimer(0),
    _scaleText(2.3),
    _addY(0)
{
    this->arrListCuoc.clear();
    this->arrIdCuoc.clear();
    this->XuongArray.clear();

	arrIdCuoc.push_back(U_XUONG);
	arrIdCuoc.push_back(U_THONG);
	arrIdCuoc.push_back(U_CHI);
	arrIdCuoc.push_back(U_THIEN_U);
	arrIdCuoc.push_back(U_DIA_U);
	arrIdCuoc.push_back(U_TOM);
	arrIdCuoc.push_back(U_LEO);
	arrIdCuoc.push_back(U_BACH_DINH);
	arrIdCuoc.push_back(U_TAM_DO);
	arrIdCuoc.push_back(U_KINH_TU_CHI);
	arrIdCuoc.push_back(U_THAP_THANH);
	arrIdCuoc.push_back(U_CO_THIEN_KHAI);
	arrIdCuoc.push_back(U_AN_BON);
	arrIdCuoc.push_back(U_BON);
	arrIdCuoc.push_back(U_CO_CHIU);
	arrIdCuoc.push_back(U_CHIU_U);
	arrIdCuoc.push_back(U_BACH_THU);
	arrIdCuoc.push_back(U_BACH_THU_CHI);
	arrIdCuoc.push_back(U_HOA_ROI_CUA_PHAT);
	arrIdCuoc.push_back(U_NLXH_HRCP);
	arrIdCuoc.push_back(U_CA_LOI_SAN_DINH);
	arrIdCuoc.push_back(U_CA_NHAY_DAU_THUYEN);
    arrIdCuoc.push_back(U_CHUA_DO_NAT_HOA);

    vector<int> doiarray;
    vector<int> valarray;

    for (int i = 1; i < this->arrIdCuoc.size(); ++i) {
        doiarray.push_back(this->arrIdCuoc.at(i));
    }
    valarray.push_back(U_XUONG);
    ButtonXuong* Xuong = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(Xuong);

    doiarray.clear();
    valarray.clear();
    doiarray.push_back(U_XUONG);
    valarray.push_back(U_THONG);
    ButtonXuong* Thong = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(Thong);


    doiarray.clear();
    valarray.clear();
    doiarray.push_back(U_XUONG);
    valarray.push_back(U_CHI);
    ButtonXuong* Chi = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(Chi);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_CHI, U_DIA_U, U_BACH_THU, U_BACH_THU_CHI,
                U_AN_BON, U_AN_2_BON, U_AN_3_BON, U_AN_4_BON, U_BON, U_CHIU_U, U_CO_CHIU, U_CO_2_CHIU, U_CO_3_CHIU, U_CO_4_CHIU, U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP,
                U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA};
    valarray.push_back(U_THIEN_U);
    ButtonXuong* ThienU = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(ThienU);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_THIEN_U, U_AN_BON, U_AN_2_BON, U_AN_3_BON, U_AN_4_BON,
                U_CO_CHIU, U_CO_2_CHIU, U_CO_3_CHIU, U_CO_4_CHIU, U_HOA_ROI_CUA_PHAT, U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA};
    valarray.push_back(U_DIA_U);
    ButtonXuong* DiaU = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(DiaU);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG};
    valarray = {U_TOM, U_DOI_TOM, U_TOM_3, U_TOM_4};
    ButtonXuong* Tom = ButtonXuong::createButton(4, valarray, doiarray);
    this->XuongArray.push_back(Tom);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_DINH};
    valarray = {U_LEO, U_DOI_LEO, U_LEO_3, U_LEO_4};
    ButtonXuong* Leo = ButtonXuong::createButton(4, valarray, doiarray);
    this->XuongArray.push_back(Leo);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_LEO, U_DOI_LEO, U_LEO_3, U_LEO_4, U_TAM_DO, U_DOI_TAM_DO, U_KINH_TU_CHI};
    valarray = {U_BACH_DINH};
    ButtonXuong* BachDinh = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(BachDinh);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_DINH, U_KINH_TU_CHI};
    valarray = {U_TAM_DO};
    ButtonXuong* TamDo = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(TamDo);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_LEO, U_DOI_LEO, U_LEO_3, U_LEO_4, U_BACH_DINH, U_TAM_DO};
    valarray = {U_KINH_TU_CHI};
    ButtonXuong* KinhTuChi = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(KinhTuChi);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU, U_BACH_THU_CHI, U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP,
                U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA};
    valarray = {U_THAP_THANH};
    ButtonXuong* ThapThanh = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(ThapThanh);


    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG};
    valarray = {U_CO_THIEN_KHAI, U_CO_2_THIEN_KHAI, U_CO_3_THIEN_KHAI, U_CO_4_THIEN_KHAI};
    ButtonXuong* ThienKhai = ButtonXuong::createButton(4, valarray, doiarray);
    this->XuongArray.push_back(ThienKhai);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_DIA_U, U_THIEN_U};
    valarray = {U_AN_BON, U_AN_2_BON, U_AN_3_BON, U_AN_4_BON};
    ButtonXuong* AnBon = ButtonXuong::createButton(4, valarray, doiarray);
    this->XuongArray.push_back(AnBon);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_DIA_U, U_THIEN_U, U_CHIU_U};
    valarray = {U_BON};
    ButtonXuong* UBon = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(UBon);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_DIA_U, U_THIEN_U};
    valarray = {U_CO_CHIU, U_CO_2_CHIU, U_CO_3_CHIU, U_CO_4_CHIU};
    ButtonXuong* CoChiu = ButtonXuong::createButton(4, valarray, doiarray);
    this->XuongArray.push_back(CoChiu);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BON, U_THIEN_U};
    valarray = {U_CHIU_U};
    ButtonXuong* ChiuU = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(ChiuU);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_THIEN_U, U_THAP_THANH, U_BACH_THU_CHI};
    valarray = {U_BACH_THU};
    ButtonXuong* BachThu = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(BachThu);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU, U_THAP_THANH, U_BACH_DINH, U_THIEN_U, U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP,
                U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA};
    valarray = {U_BACH_THU_CHI};
    ButtonXuong* BachthuChi = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(BachthuChi);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU_CHI, U_DIA_U, U_THIEN_U, U_THAP_THANH,
                U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA};
    valarray = {U_HOA_ROI_CUA_PHAT};
    ButtonXuong* HRCP = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(HRCP);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU_CHI, U_DIA_U, U_THIEN_U, U_THAP_THANH,
                U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN, U_CHUA_DO_NAT_HOA, U_HOA_ROI_CUA_PHAT};
    valarray = {U_NLXH_HRCP};
    ButtonXuong* NLXH_HRCP = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(NLXH_HRCP);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU_CHI, U_DIA_U, U_THIEN_U, U_THAP_THANH,
                U_CHUA_DO_NAT_HOA, U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP};
    valarray = {U_CA_LOI_SAN_DINH};
    ButtonXuong* CaLoi = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(CaLoi);

    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU_CHI, U_DIA_U, U_THIEN_U, U_THAP_THANH,
                U_CHUA_DO_NAT_HOA, U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP};
    valarray = {U_CA_NHAY_DAU_THUYEN};
    ButtonXuong* CaNhay = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(CaNhay);


    doiarray.clear();
    valarray.clear();
    doiarray = {U_XUONG, U_BACH_THU_CHI, U_DIA_U, U_THIEN_U, U_THAP_THANH,
                U_HOA_ROI_CUA_PHAT, U_NLXH_HRCP, U_CA_LOI_SAN_DINH, U_CA_NHAY_DAU_THUYEN};
    valarray = {U_CHUA_DO_NAT_HOA};
    ButtonXuong* ChuaDo = ButtonXuong::createButton(1, valarray, doiarray);
    this->XuongArray.push_back(ChuaDo);

    GameServer::getSingleton().addListeners(this);
}

static inline void printAllChildrenTypes(Node* node, int level)
{
    std::string spaces(level, ' ');
   // log("%s %02d : %s - name: %s", spaces.c_str(), level, typeid(*node).name(), node->getName().c_str());

    ++level;
    int index = 0;

    while (index < node->getChildren().size()){
        Node* obj = node->getChildren().at(index);
        printAllChildrenTypes(static_cast<Node*>(obj), level);
        index++;
    }
}

bool LayerGameChan_XuongU::init(){

    if(!Layer::init())
        return false;

    this->_addY = 200;

    Sprite* bg = Sprite::create("xuong-bg.png");
    bg->setPosition(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 + this->_addY);
    this->addChild(bg);

    this->LblTitle = Label::createWithTTF("",kBoldFont, 25);
    this->LblTitle->setString("Xuông");
    this->LblTitle->setPosition(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 + 200 + this->_addY);
    this->LblTitle->setColor(Color3B::RED);
    this->LblTitle->setScale(this->_scaleText);
    this->addChild(this->LblTitle);

    Button* btnXuong = Button::create();
    btnXuong->loadTextures("xuong-btn.png", "xuong-btn_press.png", "");
    btnXuong->setPressedActionEnabled(true);
    btnXuong->setPosition(Vec2(WIDTH_DESIGN / 2, HEIGHT_DESIGN / 2 - 308 + this->_addY));
    btnXuong->addTouchEventListener(CC_CALLBACK_2(LayerGameChan_XuongU::OnButtonXuongUClick, this));
    this->addChild(btnXuong);

    // Lbl time

    Vec2 pos = btnXuong->getPosition();
    Sprite* frametime = Sprite::create("xuong-time.png");

    Size __size = frametime->getContentSize();

    frametime->setAnchorPoint(Vec2(0, 0.5));
    frametime->setPosition(Vec2(pos.x - 180 - __size.width, pos.y));
    this->addChild(frametime);

    this->LblTime = Label::createWithBMFont(AndrogenFnt, "");
    this->LblTime->setAnchorPoint(Vec2(0.5, 0.5));
    this->LblTime->setColor(Color3B(123, 1, 0));
    this->LblTime->setPosition(Vec2(__size.width / 2, __size.height / 2));
    this->LblTime->setScale(1.6);
    frametime->addChild(this->LblTime);

    this->CountTimer = 60;
    boost::shared_ptr<Room> room = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();

    if (room != NULL) {
        boost::shared_ptr<RoomVariable> rv = room->GetVariable("params");
        string StrParam = *rv->GetStringValue();
        vector<string> ArrParams = mUtils::splitString(StrParam, '@');
        if(ArrParams.size() > 3)
        {
            string xml = "<?xml version=\"1.0\"?>" + ArrParams.back();

            XMLResults xe;
            XMLNodeEx xMainNode = XMLNodeEx::parseString(xml.c_str(), NULL, &xe);
            if(xe.error == XMLError::eXMLErrorNone)
            {
                const char* StrVar = "";
                StrVar = xMainNode.getChildNodeByPath("tableconfig.timeout.xuongCuocSacTimeout", true, '.').getText();
                this->CountTimer = atoi(StrVar);
            }
        }
    }

    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameChan_XuongU::CountDownTimer),this,1, false);

    int CountIndex = 0;
    int Cols = 0;
    int Rows = 0;

    float StartLeft = 400;
    float SpaceX = 300;
    float SpaceY = 60;
    float yPos = yPos = (HEIGHT_DESIGN >> 1) - 200 + SpaceY * 5 + this->_addY;

    while (CountIndex < this->arrIdCuoc.size()) {
        if (Cols < 3)
            Rows = 6;
        else
            Rows = 5;
        for (int i = 0; i < Rows; ++i) {

            ButtonXuong* btnx = this->XuongArray.at(CountIndex);
            btnx->SetID(this->arrIdCuoc.at(CountIndex));
            //btnx->loadTextures("itemXuong.png", "", "", Widget::TextureResType::PLIST);
            btnx->loadTextures("xuong-item-bg.png", "", "", Widget::TextureResType::LOCAL);
            btnx->setAnchorPoint(Vec2::ZERO);
            btnx->setPosition(Vec2(SpaceX * Cols + StartLeft, yPos - i % Rows * SpaceY));

            if (this->arrIdCuoc[CountIndex] == U_AN_BON ||
                this->arrIdCuoc[CountIndex] == U_TOM ||
                this->arrIdCuoc[CountIndex] == U_CO_CHIU ||
                this->arrIdCuoc[CountIndex] == U_CO_THIEN_KHAI ||
                this->arrIdCuoc[CountIndex] == U_LEO) {
                btnx->SetIsMulti(true);
                btnx->SetMaxValues(4);
            }
            else {
                btnx->SetIsMulti(false);
                btnx->SetMaxValues(1);
            }

            std::string text = ChanUtils::GetNameCuocSacById(this->arrIdCuoc[CountIndex]);
            btnx->SetStringTitle(text);
            btnx->SetTitleText(text);

            btnx->setEnabled(true);
            btnx->addTouchEventListener(CC_CALLBACK_2(LayerGameChan_XuongU::ItemXuong_Click, this));
            this->arrListCuoc.push_back(btnx);
            this->addChild(btnx);
            CountIndex++;
            if (CountIndex >= this->arrIdCuoc.size()) {
                CountIndex = this->arrIdCuoc.size();
                break;
            }
        }
        Cols++;
    }

   // log("%ld", this->arrIdCuoc.size());
   // log("%ld", this->arrListCuoc.size());

    this->onEnter();
    return true;
}

void LayerGameChan_XuongU::OnButtonXuongUClick(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {

        boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
        boost::shared_ptr<ISFSObject> params (new SFSObject());

        vector<string> arrCuoc;
        std::string StrIdXuong = "";
        int Count = 0;

        for (int i = 0; i < this->arrListCuoc.size(); ++i) {
            ButtonXuong* btnXuong = this->arrListCuoc.at(i);
            if (btnXuong->GetClicked()) {
                if (btnXuong->GetIsMultiValues()) {
                    int Id = this->GetIdMultiCuoc(btnXuong->GetID(), btnXuong->GetCounter());
                    arrCuoc.push_back(boost::to_string(Id));
                }
                else {
                    arrCuoc.push_back(boost::to_string(btnXuong->GetID()));
                }

                Count++;
            }
        }

        if (Count <= 0) {
           // log("Chưa chọn cước xướng !");
            return;
        }
        else {

            bool chk = this->CheckHoaNhaCa(arrCuoc);
            if(chk){
                bool chkChi = false;
                bool chkBachThu = false;
                for (int i = 0; i < arrCuoc.size(); ++i) {
                    if (atoi(arrCuoc.at(i).c_str()) == U_BACH_THU) {
                        chkBachThu = true;
                    }
                    if (atoi(arrCuoc.at(i).c_str()) == U_CHI) {
                        chkChi = true;
                    }
                }

                if (!chkChi) {
                    arrCuoc.push_back(boost::to_string(U_CHI));
                }
                if (!chkBachThu) {
                    arrCuoc.push_back(boost::to_string(U_BACH_THU));
                }
            }

            for (int i = 0; i < arrCuoc.size(); ++i) {
                StrIdXuong += arrCuoc[i];
                if (i != arrCuoc.size() - 1)
                    StrIdXuong += ":";
            }
        }

       // log("Các cước xướng : %s", StrIdXuong.c_str());
        params->PutUtfString(EXT_FIELD_CUOCSAC_LST, StrIdXuong);
        boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_U_ANNOUCE, params, lastRoom));
        GameServer::getSingleton().Send(request);
        this->removeFromParentAndCleanup(true);

    }
}

bool LayerGameChan_XuongU::CheckHoaNhaCa(const std::vector<std::string>& arrCuoc){
    for (int i = 0; i < arrCuoc.size(); ++i) {
        if (atoi(arrCuoc.at(i).c_str()) == U_CA_LOI_SAN_DINH || atoi(arrCuoc.at(i).c_str()) == U_NLXH_HRCP ||
            atoi(arrCuoc.at(i).c_str()) == U_HOA_ROI_CUA_PHAT || atoi(arrCuoc.at(i).c_str()) == U_CA_NHAY_DAU_THUYEN ||
            atoi(arrCuoc.at(i).c_str()) == U_CHUA_DO_NAT_HOA) {
            return true;
        }
    }
    return false;
}

LayerGameChan_XuongU::~LayerGameChan_XuongU()
{
	arrListCuoc.clear();
    GameServer::getSingleton().removeListeners(this);
	log("LayerGameChan_XuongU Destructor");
}

void LayerGameChan_XuongU::OnButtonXuong(Ref* pSender)
{
	boost::shared_ptr<Room> lastRoom = GameServer::getSingleton().getSmartFox()->LastJoinedRoom();
 	boost::shared_ptr<ISFSObject> params (new SFSObject());

	vector<string> arrCuoc;
	std::string StrIdXuong = "";
	int Count = 0;

	for(std::vector<ButtonXuong*>::iterator it = arrListCuoc.begin(); it != arrListCuoc.end(); ++it) 
	{
		ButtonXuong* btnXuong = *it;
		if(btnXuong->GetClicked())
		{
			if(btnXuong->GetIsMultiValues())
			{
				int Id = GetIdMultiCuoc(btnXuong->GetID(), btnXuong->GetCounter());
				arrCuoc.push_back(boost::to_string(Id));
			}
			else
				arrCuoc.push_back(boost::to_string(btnXuong->GetID()));
			
			Count++;
		}
	}

	if (Count <= 0)
	{
		log("Chua chon cuoc xuong !");
		return;
	}
	else
	{
		for(std::vector<string>::iterator it = arrCuoc.begin(); it != arrCuoc.end(); ++it)
		{
			StrIdXuong += *it;
			if(it != arrCuoc.end() - 1)
				StrIdXuong += ":";
		}
	}
	
	log("Cac cuoc = %s", StrIdXuong.c_str());
	params->PutUtfString(EXT_FIELD_CUOCSAC_LST, StrIdXuong);
	boost::shared_ptr<IRequest> request (new ExtensionRequest(EXT_EVENT_REQ_U_ANNOUCE, params, lastRoom));
	GameServer::getSingleton().Send(request);
	this->removeFromParentAndCleanup(true);
}



void LayerGameChan_XuongU::ItemXuong_Click(Ref *sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		ButtonXuong *pButton = (ButtonXuong*)sender;
		log("Ban vua click: %d", pButton->GetID());
		log("Count = %d", pButton->GetCounter());

        for (int i = 0; i < this->XuongArray.size(); ++i) {
            ButtonXuong * xu = this->XuongArray.at(i);
            if (xu != pButton) {
                xu->DetectectCuocDoi(pButton->GetID());
            }
        }

		if(pButton->GetIsMultiValues())
		{
			int Count = pButton->GetCounter();
			if (!pButton->GetClicked())
			{
				pButton->SetCounter(Count + 1);
				string str = boost::to_string(Count + 1);
				str = str + " " + pButton->GetStringTitle();
				pButton->SetTitleText(str);

//				pButton->loadTextures("itemXuong_press.png", "", "", Widget::TextureResType::PLIST);
				pButton->SetClicked(true);
			}
			else
			{
				if(Count < 4)
				{
					pButton->SetCounter(Count + 1);
					string str = boost::to_string(Count + 1);
					str = str + " " + pButton->GetStringTitle();
					pButton->SetTitleText(str);

//					pButton->loadTextures("itemXuong_press.png", "", "", Widget::TextureResType::PLIST);
					pButton->SetClicked(true);
				}
				else
				{
					pButton->SetCounter(0);
					pButton->SetTitleText(pButton->GetStringTitle());
//					pButton->loadTextures("itemXuong.png", "", "", Widget::TextureResType::PLIST);
					pButton->SetClicked(false);
				}
			}
		}

		else
		{
			if (!pButton->GetClicked())
			{
//				pButton->loadTextures("itemXuong_press.png", "", "", Widget::TextureResType::PLIST);
				pButton->SetClicked(true);
			}
			else
			{
//				pButton->loadTextures("itemXuong.png", "", "", Widget::TextureResType::PLIST);
				pButton->SetClicked(false);
			}
		}

		std::string StrTilte = "";
        int _count = 0;
        bool _flag = false;

		for(std::vector<ButtonXuong*>::iterator it = arrListCuoc.begin(); it != arrListCuoc.end(); ++it)
		{
			ButtonXuong* btnXuong = *it;

            int __id = btnXuong->GetID();
            if (__id == U_HOA_ROI_CUA_PHAT ||
                __id == U_NLXH_HRCP ||
                __id == U_CA_LOI_SAN_DINH ||
                __id == U_CA_NHAY_DAU_THUYEN ||
                __id == U_CHUA_DO_NAT_HOA) {
                _flag = true;
            }

			if(btnXuong->GetClicked())
			{
				if (btnXuong->GetIsMultiValues())
				{
					int count = btnXuong->GetCounter();
					int idx = btnXuong->GetID();
					int CuocId = GetIdMultiCuoc(idx, count);
					//StrTilte += GetValuesMultiCuoc(idx, count);
					StrTilte += ChanUtils::GetNameCuocSacById(CuocId);
					if(it != arrListCuoc.end() - 1)
						StrTilte += " ";
				}
				else
				{
					StrTilte += ChanUtils::GetNameCuocSacById(btnXuong->GetID());
					if(it != arrListCuoc.end() - 1)
						StrTilte += " ";
				}
                _count++;
			}
		}

        if (_flag || _count >= 7) {
            this->_scaleText = 1.9;
        } else {
            this->_scaleText = 2.3;
        }

        if(0 == StrTilte.compare ("")){
            StrTilte = dataManager.GetSysString(707);
        }

       // log("%s", StrTilte.c_str());
        this->LblTitle->setScale(this->_scaleText);

        this->LblTitle->setString(StrTilte.c_str());
	}
}

int LayerGameChan_XuongU::GetIdMultiCuoc(const int& idx, const int& count)
{
	switch(idx)
	{
	case U_AN_BON:
		{
			switch(count)
			{
			case 1:
				return U_AN_BON;
			case 2:
				return U_AN_2_BON;
			case 3:
				return U_AN_3_BON;
			case 4:
				return U_AN_4_BON;
			}
		}
	case U_TOM:
		{
			switch(count)
			{
			case 1:
				return U_TOM;
			case 2:
				return U_DOI_TOM;
			case 3:
				return U_TOM_3;
			case 4:
				return U_TOM_4;
			}
		}
	case U_CO_CHIU:
		{
			switch(count)
			{
			case 1:
				return U_CO_CHIU;
			case 2:
				return U_CO_2_CHIU;
			case 3:
				return U_CO_3_CHIU;
			case 4:
				return U_CO_4_CHIU;
			}
		}
	case U_CO_THIEN_KHAI:
		{
			switch(count)
			{
			case 1:
				return U_CO_THIEN_KHAI;
			case 2:
				return U_CO_2_THIEN_KHAI;
			case 3:
				return U_CO_3_THIEN_KHAI;
			case 4:
				return U_CO_4_THIEN_KHAI;
			}
		}
	case U_LEO:
		{
			switch(count)
			{
			case 1:
				return U_LEO;
			case 2:
				return U_DOI_LEO;
			case 3:
				return U_LEO_3;
			case 4:
				return U_LEO_4;
			}
		}
	}
	return -1;
}

string LayerGameChan_XuongU::GetValuesMultiCuoc(const int& idx, const int& count)
{
	switch(idx)
	{
	case U_AN_BON:
		return dataManager.GetSysString(669);
	case U_TOM:
		{
			switch(count)
			{
			case 1:
				return dataManager.GetSysString(663);
			case 2:
				return dataManager.GetSysString(582);
			case 3:
				return dataManager.GetSysString(502);
			case 4:
				return dataManager.GetSysString(503);
			default:
				return "";
			}
		}
	case U_CO_CHIU:
		return dataManager.GetSysString(544);
	case U_CO_THIEN_KHAI:
		return dataManager.GetSysString(542);
	case U_LEO:
		{
			if(count == 1)
				return dataManager.GetSysString(664);
			else
				return dataManager.GetSysString(584);
		}
		return "";
	}
	return "";
}

void LayerGameChan_XuongU::OnExtensionResponse(unsigned long long ptrContext, boost::shared_ptr<BaseEvent> ptrEvent)
{
    boost::shared_ptr<map<string, boost::shared_ptr<void> > > ptrEvetnParams = ptrEvent->Params();
    boost::shared_ptr<void> ptrEventParamValueCmd = (*ptrEvetnParams)["cmd"];
    boost::shared_ptr<string> cmd = ((boost::static_pointer_cast<string>)(ptrEventParamValueCmd));
    
    boost::shared_ptr<void> ptrEventParamValueParams = (*ptrEvetnParams)["params"];
	boost::shared_ptr<ISFSObject> param = ((boost::static_pointer_cast<ISFSObject>(ptrEventParamValueParams)));

	if (cmd->compare(EXT_SRVNTF_ANBAO) == 0)
	{
		boost::shared_ptr<long> anbaores = param->GetInt(EXT_FIELD_ANBAO_REASON);
		if (anbaores != NULL)
		{
			if (*anbaores == ANBAO_REASON_U_NHUNG_KHONG_XUONG)
				this->removeFromParentAndCleanup(true);
		}
	}

	else if (cmd->compare(EXT_EVENT_END) == 0)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void LayerGameChan_XuongU::CountDownTimer(float dt)
{
	CountTimer--;
	string iTime = boost::to_string(CountTimer);
	LblTime->setString(iTime.c_str());
	if(CountTimer == 0)
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameChan_XuongU::CountDownTimer),this);
}


bool ButtonXuong::init()
{
	if (!Button::init())
        return false;

    this->TitleText = Label::createWithTTF("",kBoldFont, 30);
    this->TitleText->setColor(_colorTextInPopUp);
    this->TitleText->setScale(1.3);
    this->TitleText->setAnchorPoint(Vec2(0, 0.5));
    this->TitleText->setPosition(Vec2(0, this->getContentSize().height / 2));
    this->TitleText->setLocalZOrder(3);
    this->addChild(this->TitleText);

    this->Blocks = ui::Scale9Sprite::create("chats/button.png");
    float wLabel = 0;
    float hLabel = 0;
    Size size = Size(wLabel + 10, hLabel + 5);
    Blocks ->setContentSize(size);
    Blocks->setAnchorPoint(Vec2(0, 0.5));
    Blocks->setPosition(Vec2(this->TitleText->getPositionX() + -10, this->TitleText->getPositionY()));

    Blocks->setColor(Color3B(84, 81, 69));
    Blocks->setOpacity(80);
    this->setPosition(Vec2((WIDTH_DESIGN - size.width) / 2, 100));
    this->Blocks->setVisible(false);

    // add
    this->addChild(Blocks);

	return true;
}

void ButtonXuong::SetID(const int& idx)
{
	this->Idx = idx;
}

const int& ButtonXuong::GetID() const
{
	return this->Idx;
}

void ButtonXuong::SetClicked(const bool& isClicked)
{
	this->IsClicked = isClicked;
    this->ShowBackground(isClicked);
}

const bool& ButtonXuong::GetClicked() const
{
	return this->IsClicked;
}

ButtonXuong::ButtonXuong(const int& max, vector<int> values, vector<int> cuocdois)
	:TitleText(NULL),
	IsClicked(false),
	Idx(-1),
	IsMulti(false),
	CounterClick(0),
    MaxValues(max)
{
    this->Value = values;
    this->CuocDoi = cuocdois;
}

ButtonXuong* ButtonXuong::createButton(const int& max, vector<int> values, vector<int> cuocdois){
    ButtonXuong* btn = new ButtonXuong(max, values, cuocdois);
    if(btn != NULL && btn->init()){
        btn->autorelease();
        return btn;
    }
    else
    {
        delete btn;
        btn = 0;
        return NULL;
    }
}

ButtonXuong::~ButtonXuong()
{
	log("Button Xuong U Destructor");
}

void ButtonXuong::SetCuocDoi(vector<int> &P) {
    this->CuocDoi = P;
}

void ButtonXuong::PrintCuocDoi() {
    for (int i = 0; i < this->CuocDoi.size(); ++i) {
        string f = ChanUtils::GetNameCuocSacById(this->CuocDoi.at(i));
       // log("i = %d cuoc = %s", i, f.c_str());
    }
}

bool ButtonXuong::DetectectCuocDoi(const int& idx) {
    for (int i = 0; i < this->CuocDoi.size(); ++i) {
        if (idx == this->CuocDoi.at(i)) {
            this->ShowBackground(false);
            this->IsClicked = false;
            this->CounterClick = 0;
            string t = ChanUtils::GetNameCuocSacById(this->Idx);
            this->SetTitleText(t);
            return true;
        }
    }
    return false;
}

void ButtonXuong::ShowBackground(bool state) {
    if (state) {
        float wLabel = this->TitleText->getContentSize().width * 1.3;
        float hLabel = this->TitleText->getContentSize().height * 1.3;
        Size size = Size(wLabel + 20, hLabel + 5);

        this->Blocks->setPosition(Vec2(this->TitleText->getPositionX() - 10, this->TitleText->getPositionY()));

        this->Blocks->setContentSize(size);
        this->Blocks->setVisible(true);
    }
    else {
        this->Blocks->setVisible(false);
    }
}

void ButtonXuong::SetTitleText(std::string& title)
{
	TitleText->setPosition(Point(0, this->getContentSize().height / 2));
	TitleText->setString(title);
}

void ButtonXuong::SetMaxValues(const int& num)
{
	this->MaxValues = num;
}

void ButtonXuong::SetIsMulti(const bool& isMul)
{
	this->IsMulti = isMul;
}

const int& ButtonXuong::GetMaxValues() const
{
	return this->MaxValues;
}

const bool& ButtonXuong::GetIsMultiValues() const
{
	return this->IsMulti;
}

string& ButtonXuong::GetStringTitle()
{
	return this->StrTitleText;
}

void ButtonXuong::SetStringTitle(std::string& text)
{
	this->StrTitleText = text;
}

int& ButtonXuong::GetCounter()
{
	return this->CounterClick;
}

void ButtonXuong::SetCounter(const int& count)
{
	this->CounterClick = count;
}

