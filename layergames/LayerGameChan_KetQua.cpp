#include "LayerGameChan_KetQua.h"
#include "mUtils.h"
#include "AllData.h"
#include "ChanUtils.h"
#include "../SceneManager.h"
#include "PhomMessDef.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../CrossPlatform/Android/Android.h"
#endif

#define kTag_btnClose 100
#define kTag_btnShare 101

LayerGameChan_KetQua::LayerGameChan_KetQua(const std::string& result, LayerMocChatInGame* layerChat):
     TableContainer(NULL),
     m_callback(NULL),
     m_callbackListener(NULL),
     Counter(0),
     Result(result),
     layerChat(layerChat)
{

}

LayerGameChan_KetQua::~LayerGameChan_KetQua() {
   // log("Jumpe to Deconstructor LayerGameChan_KetQua");
}

LayerGameChan_KetQua* LayerGameChan_KetQua::createInstance(const std::string& result, LayerMocChatInGame* layerChat){
    LayerGameChan_KetQua* kq = new LayerGameChan_KetQua(result, layerChat);
    if(kq && kq->init()){
        kq->autorelease();
        return kq;
    }
    else
    {
        delete kq;
        kq = 0;
        return NULL;
    }
}

void LayerGameChan_KetQua::SetLayerChat(LayerMocChatInGame* layerChat){
    this->layerChat = layerChat;
}

bool LayerGameChan_KetQua::init() {
    if (!Layer::init())
        return false;

    this->setAnchorPoint(Vec2(0,0));
    this->setPosition(Vec2(0,0));

    this->TableContainer = Sprite::createWithSpriteFrameName("frame_thongbao.png");
    this->TableContainer->setAnchorPoint(Vec2(0.5, 0));
    this->TableContainer->setPosition(Vec2(WIDTH_DESIGN >> 1, 90));
    this->addChild(this->TableContainer);

    if (SceneManager::getSingleton().getGameVersion() == GameVersion::p2){
         Button* btnClose = this->CreateButton("", Vec2((WIDTH_DESIGN >> 1), 50), kTag_btnClose);
        this->addChild(btnClose);
    }else{
         Button* btnClose = this->CreateButton(dataManager.GetSysString(583).c_str(), Vec2((WIDTH_DESIGN >> 1), 75), kTag_btnClose);
        this->addChild(btnClose);
    }
   
   // Button* btnShare = this->CreateButton("Chia sẻ", Vec2((WIDTH_DESIGN >> 1) + 120, 87), kTag_btnShare);

    
//    this->addChild(btnShare);

    this->DisplayResult(this->Result);

    return true;
}

void LayerGameChan_KetQua::OnButtonClick(Ref *sender, ui::Widget::TouchEventType pType) {
    if (pType == ui::Widget::TouchEventType::ENDED) {
        Button* btn = (Button*)sender;
        if (btn) {
            if (btn->getTag() == kTag_btnClose) {
                if (m_callback && m_callbackListener)
                    (m_callback->*m_callbackListener)(this);
                Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameChan_KetQua::AutoClose),this);
                this->removeFromParentAndCleanup(true);
            }

            else if (btn->getTag() == kTag_btnShare)
                this->CaptureScreenAndShare();
        }
    }
}

void LayerGameChan_KetQua::DisplayResult(string& result) {
    Director::getInstance()->getScheduler()->schedule(schedule_selector(LayerGameChan_KetQua::AutoClose),this,1, false);

    vector<string> arrResults;
    this->split(result, ';', arrResults);

    for(int i = 0; i < arrResults.size(); ++i) {
       // log("%d : %s", i, arrResults[i].c_str());
    }

    try {
       // log("Van thu: %s", arrResults.at(0).c_str());
       // log("winner user: %s", arrResults.at(2).c_str());
       // log("den lang user: %s", arrResults.at(3).c_str());
       // log("winner xuong cuoc sac: %s", arrResults.at(4).c_str());
       // log("den lang cuoc sac: %s", arrResults.at(5).c_str());
       // log("Tong diem: %s", arrResults.at(6).c_str());
       // log("danh sach tinh tien: %s", arrResults.at(7).c_str());

        if (arrResults.size() >= 9)
            log("Cac cuoc thieu: %s", arrResults.at(8).c_str());
        if (arrResults.size() >= 10)
            log("Cac cuoc co ga: %s", arrResults.at(9).c_str());

        //! Get end game reason
        int EndReson = atoi(arrResults.at(1).c_str());
        std::string StrResults = "";
        bool b_Check = false;
        float scaleText = 1.15;
        switch(EndReson) {

        //! Hòa cả làng
        case GAME_ERS_HOA_CA_LANG: {
            scaleText = 1.5;
            Label* lblHoaCaLang = Label::createWithTTF(dataManager.GetSysString(610),kBoldFont, 25);
            lblHoaCaLang->setColor(_colorTextInPopUp);
            lblHoaCaLang->setAnchorPoint(Vec2(0.5, 0.5));
            lblHoaCaLang->setScale(scaleText);
            lblHoaCaLang->setPosition(Vec2(this->TableContainer->getContentSize().width / 2, this->TableContainer->getContentSize().height / 2));
            this->TableContainer->addChild(lblHoaCaLang);
            std::string hoa = dataManager.GetSysString(610);
            this->SendMsg(hoa);
        }
        return;

        //! Treo tranh
        case GAME_ERS_HAS_TREOTRANH: {
            StrResults = ChanUtils::GetNamePlayer(arrResults.at(2));
            StrResults = StrResults + " "+dataManager.GetSysString(738);
            //94:9:2:1/Cuu:Vanc
            std::string name = "";
            try {
                std::vector<string> info = mUtils::splitString(arrResults.at(11), '/');
                std::vector<string> treo = mUtils::splitString(info.at(0), ';');
                name = ChanUtils::GetNameCard(treo.at(0));
            } catch(std::out_of_range& e) {
                log("find treo tranh out_of_range %s", e.what());
            } catch(std::exception& e) {
                log("find treo tranh exception %s", e.what());
            } catch(...) {

            }

            if(0 != name.compare("")) {
                StrResults += " "+dataManager.GetSysString(642)+" " + name + "\n";
            }

            this->SendMsg(StrResults);

            StrResults += dataManager.GetSysString(641);

            scaleText = 1.5;

            Label* lblTreoTranh = Label::createWithTTF(StrResults,kBoldFont, 25);
            lblTreoTranh->setColor(_colorTextInPopUp);
            lblTreoTranh->setAnchorPoint(Vec2(0.5, 0.5));
            lblTreoTranh->setScale(scaleText);
            lblTreoTranh->setPosition(Vec2(this->TableContainer->getContentSize().width / 2, this->TableContainer->getContentSize().height / 2));
            this->TableContainer->addChild(lblTreoTranh);
        }
        return;

        //!Có người thắng
        case GAME_ERS_HAS_WINNER: {
            StrResults += ChanUtils::GetNamePlayer(arrResults.at(2)) + " "+dataManager.GetSysString(739)+": \n";
            std::string CuocWin = this->GetCuocSac(arrResults.at(4), true);
            StrResults += this->FormatStringUtf8(CuocWin) + "\n";

            this->SendMsg(StrResults);

            std::string CuocGa = "";
            try {
                if(0 != arrResults.at(9).compare("")) {
                    CuocGa = dataManager.GetSysString(554)+" " + this->GetCuocSac(arrResults.at(9), false);
                }
                if(atoi(arrResults.at(10).c_str()) > 0) {
                    CuocGa += " "+dataManager.GetSysString(555)+" " + arrResults.at(10) + " "+dataManager.GetSysString(556);
                }
            } catch(std::out_of_range& e) {
                log("GAME_ERS_HAS_WINNER out of range: %s", e.what());
            } catch(std::exception& e) {
                log("GAME_ERS_HAS_WINNER exception: %s", e.what());
            } catch(...) {

            }
            if(0 != CuocGa.compare("")) {
                StrResults += this->FormatStringUtf8(CuocGa) + "\n";
            }
        }
        break;

        //!Có người thua
        case GAME_ERS_HAS_ONE_DEN_LANG: {
            StrResults += ChanUtils::GetNamePlayer(arrResults.at(3)) + " "+dataManager.GetSysString(740)+": \n";
            string CuocLose = this->GetCuocSac(arrResults.at(5), true);
            StrResults += this->FormatStringUtf8(CuocLose) + "\n";

            this->SendMsg(StrResults);

            if (arrResults.at(8).compare("") != 0) {
                string CuocThieu = dataManager.GetSysString(708)+" " + this->GetCuocSac(arrResults.at(8), true);
                StrResults +=  this->FormatStringUtf8(CuocThieu) + "\n";
            }
            this->SendMsg(StrResults);
        }
        break;

        //! Vi phạm điểm tối thiểu
        case GAME_ERS_VI_PHAM_CHOI_DIEM_TOI_THIEU: {
            StrResults += ChanUtils::GetNamePlayer(arrResults.at(3)) + " "+dataManager.GetSysString(741)+" \n";
            StrResults += dataManager.GetSysString(715)+" \n";
            this->SendMsg(StrResults);
        }
        break;

        //! Không xướng cước
        case GAME_ERS_KHONG_XUONG_CUOC: {
            StrResults += ChanUtils::GetNamePlayer(arrResults.at(3)) + " "+dataManager.GetSysString(742)+" \n";
            this->SendMsg(StrResults);
        }
        break;

        //! Treo tranh phat 8 do 2 leo: U lao, An ca doi cho, Bo U
        case GAME_ERS_HAS_TREOTRANH_PHAT_8DO2LEO: {
            std::string _rea = "";
            try {
                long a = atol(arrResults.at(12).c_str());
                _rea = ChanUtils::GetStringAnBao(a);

                std::vector<std::string> arrBao = mUtils::splitString(arrResults.at(11), '/');
                if(arrBao.size() > 0 && _rea.compare("") != 0) {
                    _rea += " "+dataManager.GetSysString(642)+" \n";
                    for(int i = 0; i < arrBao.size(); ++i) {
                        _rea += ChanUtils::GetNameCard(arrBao.at(i));
                        if(i != arrBao.size() - 1) {
                            _rea += ", ";
                        }
                    }
                }
            } catch(std::out_of_range& e) {
                log("GAME_ERS_HAS_TREOTRANH_PHAT_8DO2LEO out_of_range %s", e.what());
            } catch(std::exception& e) {
                log("GAME_ERS_HAS_TREOTRANH_PHAT_8DO2LEO exception %s", e.what());
            } catch(...) {
            }

            std::string u = "";
            if(arrResults.at(2).compare("") != 0){
                u = ChanUtils::GetNamePlayer(arrResults.at(2));
            }else{
                u = ChanUtils::GetNamePlayer(arrResults.at(3));
            }
            if(0 != _rea.compare("")) {
                StrResults += u + " " + _rea + "\n";
                StrResults += dataManager.GetSysString(643)+"\n";
            } else {
                StrResults += u + " "+dataManager.GetSysString(743)+" \n";
            }
            b_Check = true;
            this->SendMsg(StrResults);
        }
        break;

        case GAME_ERS_HAS_ONE_BO_CUOC:
        {
            StrResults += ChanUtils::GetNamePlayer(arrResults.at(3)) + " "+dataManager.GetSysString(511)+" \n";
            StrResults += dataManager.GetSysString(643)+" \n";
            this->SendMsg(StrResults);
        }
            break;

        default:
            break;
        }
    

        if(!b_Check) {
            StrResults += dataManager.GetSysString(744)+": " + arrResults.at(6) + "\n";
        }

        //!Tính tiền
        if (arrResults.at(7).compare("") != 0) {
            vector<string> arrTinhtien = mUtils::splitString(arrResults.at(7), '/');
            for(vector<string>::iterator it = arrTinhtien.begin(); it != arrTinhtien.end(); ++it) {
                vector<string> info = mUtils::splitString(*it, ':');
                try {
                    StrResults += ChanUtils::GetNamePlayer(info.at(0)) + ": " + info.at(2) + " "+dataManager.GetSysString(551)+"\n";
                } catch(std::out_of_range& e) {
                    log("Tính tiền out_of_range %s", e.what());
                } catch(std::exception& e) {
                    log("Tính tiền exception %s", e.what());
                }
            }
        }

        //! Label Hien thi
        Label* ct = Label::createWithTTF(StrResults,kBoldFont, 25);
        ct->setColor(_colorTextInPopUp);
        ct->setAnchorPoint(Vec2(0.5, 0.5));
        ct->setScale(scaleText);
        ct->setPosition(Vec2(this->TableContainer->getContentSize().width / 2, this->TableContainer->getContentSize().height / 2));
        this->TableContainer->addChild(ct);

    } catch(std::out_of_range& e) {
        log("DisplayResuilt out_of_range: %s", e.what());
    } catch(std::exception& e) {
        log("DisplayResuilt exception: %s", e.what());
    } catch(...) {

    }
}

// gửi bản tin thông báo kết quả ván chơi xuống các client
// format: gameid;gameEndReason;winnerUserName;denLangUserName;winCuocSacList;denLangCuocSacList;totalScore;danhSáchTínhTiền;cácCướcXướngThiếu;cácCướcGà
// trong đó winCuocSacList, denLangCuocSacList, cácCướcXướngThiếu và cácCướcGà có định dạng: cuoc1:cuoc2:cuocN:...
// danhSáchTínhTiền: cộng trừ tiền cho các player, thông tin mỗi player có format username:coinReason:coinChange, thông tin mỗi player cách nhau bởi kí tự '/'
// mẫu: 1;1;phanpc;;2:6;;6;phanpc:0:27000/phanpc1:6:-30000
//
string LayerGameChan_KetQua::GetCuocSac(string& list_cuoc, const bool& type) {

    if(type){
        return ChanUtils::TestGameCuoc(list_cuoc);
    }

    vector<string> ArrCuoc = mUtils::splitString(list_cuoc, ':');
    string StrCuoc = "";
    for (vector<string>::iterator it = ArrCuoc.begin(); it != ArrCuoc.end(); ++it) {
        if(it == ArrCuoc.end() - 1) {
            if(type)
                StrCuoc += ChanUtils::GetNameCuocSacById(*it);
            else
                StrCuoc += ChanUtils::GetNamCuocCoGa(*it);
        } else {
            if(type)
                StrCuoc += ChanUtils::GetNameCuocSacById(*it) + ", ";
            else
                StrCuoc += ChanUtils::GetNamCuocCoGa(*it) + ", ";
        }
    }
    return StrCuoc;
}

void LayerGameChan_KetQua::onEnter() {
    Layer::onEnter();
    this->runAction(mUtils::getActionOpenPopup());
}

Button* LayerGameChan_KetQua::CreateButton(const char* _title, const Vec2& _position, const int& _tag) {
    Button* button = Button::create();
    button->loadTextures("btnChanClose.png", "btnChanClose_press.png", "", Widget::TextureResType::PLIST);
    button->setAnchorPoint(Vec2(0.5, 0));
    button->setPosition(_position);

    auto *title = Label::createWithTTF(_title,kBoldFont, 25);
    title->setAnchorPoint(Vec2(0.5, 0.5));
	title->setColor(Color3B::WHITE);
    title->setScale(1.5);
    title->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2 + 3));
    button->addChild(title);

    button->setEnabled(true);
    button->addTouchEventListener(CC_CALLBACK_2(LayerGameChan_KetQua::OnButtonClick, this));
    button->setTag(_tag);

    return button;
}

int LayerGameChan_KetQua::LengthUtf8String(const char* s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xC0) != 0x80)
            j++;
        i++;
    }
    return j;
}

string& LayerGameChan_KetQua::FormatStringUtf8(string& str, const int& WIDTH) {
    if (LengthUtf8String(str.c_str()) <= WIDTH)
        return str;

    string wrapped = "";
    int len = 0;
    int pos_space = 0;
    int byteCount = 0;
    const char *aStr;

    while(LengthUtf8String(str.c_str()) > WIDTH) {
        len = 0;
        pos_space = 0;
        byteCount = 0;
        aStr = str.c_str();
        while(aStr) {
            if( (*aStr & 0xc0) != 0x80 )
                len += 1;

            if (*aStr == ' ')
                pos_space = byteCount;

            if (len == WIDTH) {
                wrapped.append(str.substr(0, pos_space));
                wrapped.append("\n");
                str = str.substr(pos_space + 1);
                break;
            }
            byteCount ++;
            aStr++;
        }
    }

    wrapped.append(str);
    str = wrapped;
    return str;
}

void LayerGameChan_KetQua::CaptureScreenAndShare() {
   // log("jump to capture screen");
    Size winSize = Director::getInstance()->getWinSize();
    Texture2D::PixelFormat myPixel;
    myPixel = Texture2D::PixelFormat::RGBA8888;

    RenderTexture *tex = RenderTexture::create(
                             (int)winSize.width,
                             (int)winSize.height,
                             myPixel,
                             GL_DEPTH24_STENCIL8);
    tex->setPosition(Point(winSize.width / 2, winSize.height / 2));
    //tex->beginWithClear(0, 0, 0, 0, 1);

    tex->begin();
    Director::getInstance()->getRunningScene()->visit();
    //this->getParent()->visit();
    tex->end();
    int _count = 1;
    std::string P = StringUtils::format("screenshot_%d.png", _count);

   // log("%s", P.c_str());

    std::string str = CCFileUtils::getInstance()->getWritablePath();

    std::stringstream ss;
    std::string s;
    ss << P;
    ss >> s;

    str.append(s);

    const char* ddd = str.c_str();

    const char* file;
    file = P.c_str();
    bool status;
    status = tex->saveToFile(file, Image::Format::PNG);
    tex->removeAllChildrenWithCleanup(true);
   // log("Status :: %d",status);
    if (status == true) {
       // log("%s", ddd);
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     //   captureAndShare(ddd);
        //openURL
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       // log("iOS capture");
        //IOS::postFacebook(ddd);
#endif
        _count ++;
    } else {
       // log("not capture !");
    }
}

void LayerGameChan_KetQua::SetCallBack_Close(Ref* target, SEL_CallFuncO callfun) {
    m_callback = target;
    m_callbackListener = callfun;
}

void LayerGameChan_KetQua::AutoClose(float dt) {
    Counter++;
    if (Counter == 30) {
        Director::getInstance()->getScheduler()->unschedule(schedule_selector(LayerGameChan_KetQua::AutoClose),this);
        if (m_callback && m_callbackListener)
            (m_callback->*m_callbackListener)(this);
        this->removeFromParentAndCleanup(true);
    }
}

std::vector<std::string>& LayerGameChan_KetQua::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void LayerGameChan_KetQua::SendMsg(std::string& msg){
    if(this->layerChat != NULL){
        std::string res = msg;
        this->layerChat->reportResultGame(res);
    }
}
