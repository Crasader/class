#include "PromotionHandler.h"

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "SceneManager.h"

PromotionHandler::PromotionHandler()
{

}

PromotionHandler::~PromotionHandler()
{
    lstPromotionInfo.clear();
}

void PromotionHandler::handleDatas(string datas)
{
	unsigned vt = datas.find_last_of(']');
	if (vt != -1)
		datas = datas.substr(0, vt + 1);

	lstPromotionInfo.clear();
	//
	rapidjson::Document readdoc;

	std::string load_str = datas;
	readdoc.Parse<0>(load_str.c_str());
	if (readdoc.HasParseError())
	{
		return;
	}

	//rapidjson::Value& _array = readdoc["notification"];

	if (readdoc.IsArray())
	{
		for (int i = 0; i < readdoc.Size(); i++) {
			rapidjson::Value& arraydoc = readdoc[i];
			PromotionInfo promoInfo;
			if (arraydoc.HasMember("kmdesc"))
			{
				promoInfo.kmdesc = arraydoc["kmdesc"].GetString();
			}
			if (arraydoc.HasMember("kmname"))
			{
				promoInfo.kmname = arraydoc["kmname"].GetString();
			}
			if (arraydoc.HasMember("kmid"))
			{
				promoInfo.kmid = arraydoc["kmid"].GetInt();
			}
			//Options
			if (arraydoc.HasMember("kmopt"))
			{
				rapidjson::Value& _array = arraydoc["kmopt"];
				if (_array.IsArray())
				{
					for (int j = 0; j < _array.Size(); j++) {
						rapidjson::Value& _arrayChilds = _array[j];
						PromotionOptions proOpts;

						if (_arrayChilds.HasMember("gameid"))
						{
							proOpts.gameid = _arrayChilds["gameid"].GetInt();
						}
						if (_arrayChilds.HasMember("numgameplayed"))
						{
							proOpts.numgameplayed = _arrayChilds["numgameplayed"].GetString();
						}
						if (_arrayChilds.HasMember("numgamekm"))
						{
							proOpts.numgamekm = _arrayChilds["numgamekm"].GetString();
						}
						//
						promoInfo.lstPromotionOptions.push_back(proOpts);
                        promoInfo.isUsed = false;
					}
				}
			}
			//
			lstPromotionInfo.push_back(promoInfo);
            SceneManager::getSingleton().setLstPromotionInfo(lstPromotionInfo);
		}
	}
}
void PromotionHandler::updateListPromotionInfo(PromotionInfo info){
//    for (int i = 0; i < lstPromotionInfo.size(); i++){
//        if (lstPromotionInfo.at(i).kmid == info.kmid) {
//            lstPromotionInfo.at(i).at(i).isUsed = true;
//            break;
//        }
//    }
    for (vector<PromotionInfo>::iterator it = lstPromotionInfo.begin();
         it != lstPromotionInfo.end(); ++it) {
        PromotionInfo sinfo = *it;
        if (sinfo.kmid == info.kmid){
            lstPromotionInfo.erase(it);
            lstPromotionInfo.insert(it, info);
        }
    }
    SceneManager::getSingleton().setLstPromotionInfo(lstPromotionInfo);

}
vector<PromotionInfo> PromotionHandler::getListPromotionInfo(){
    return lstPromotionInfo;
}

