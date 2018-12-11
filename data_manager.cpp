#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "data_manager.h"
#include <stdio.h>
#include "mUtils.h"
#include "json/rapidjson.h"
#include "json/document.h"

const char* DataManager::unknown_string = "???";
DataManager dataManager;

bool DataManager::LoadStrings(const char* file) {
    //read level info
    string str = FileUtils::getInstance()->getStringFromFile("localconfig.json");
    rapidjson::Document d;

    //     string sFullPathConfig = CCFileUtils::sharedFileUtils()->getWritablePath();
    //     sFullPathConfig += "/localconfig.json";
    //     //Check file existed
    //     if (CCFileUtils::sharedFileUtils()->getStringFromFile(str) == "")
    //         CCFileUtils::sharedFileUtils()->writeStringToFile(str, sFullPathConfig);
    //
    //     str = CCFileUtils::getInstance()->getStringFromFile(sFullPathConfig);
    //    d.Clear();
    d.Parse<0>(str.c_str());
    if (d.IsObject()){
        mAppConfig.version_android = d["version_android"].GetString();
        mAppConfig.version_ios = d["version_ios"].GetString();
        mAppConfig.url_list_ip = d["url_list_ip"].GetString();
        mAppConfig.url_list_ip_1 = d["url_list_ip_1"].GetString();
        mAppConfig.ip_default = d["ip_default"].GetString();
        //mAppConfig.version_win = d["version_win"].GetString();
    }else{
        //ThaoHX for reahu
//        mAppConfig.version_android = "and13112018";
//        mAppConfig.version_ios = "ios07112018";
//        mAppConfig.url_list_ip = "http://config.reahu-game.fun/diamon/level";
//        mAppConfig.url_list_ip_1 = "";
//        mAppConfig.ip_default = "";
        
        //ThaoHX for winclub
        
        mAppConfig.version_android = "and08112018";
        mAppConfig.version_ios = "ios28112018";
        mAppConfig.url_list_ip = "http://config.winclubgame.info/api?cmd=getconfig";
        mAppConfig.url_list_ip_1 = "";
        mAppConfig.ip_default = "";

    }
    
    str = FileUtils::getInstance()->getStringFromFile("levels.json");
    d.Parse<0>(str.c_str());
    if (d.IsArray()){
        rapidjson::SizeType size = d.Size();
        
        for (rapidjson::SizeType i = 0; i < size; ++i){
            LevelInfo _lvl;
            _lvl.level = d[i]["level"].GetInt();
            _lvl.currExp = d[i]["curr_exp"].GetInt();
            _lvl.needExp = d[i]["exp_need"].GetInt();
            lstLevelInfos.push_back(_lvl);
        }
    }
    //Read app config
   
    
	 str = FileUtils::getInstance()->getStringFromFile(file);
	if (str.empty())
		return false;

	for(int i = 0; i < 20480; ++i)
		_sysStrings[i] = "";

	vector<string> lstStrings = mUtils::splitString(str, '\n');
	for (int i = 3; i < lstStrings.size(); i++){
		string sDatas = lstStrings.at(i);
		if (sDatas.at(0) == '#')
			continue;
		size_t f = sDatas.find_first_of(' ');
		if (f == string::npos)
			continue;
		sDatas = sDatas.substr(f + 1, sDatas.size());
		f = sDatas.find_first_of(' ');
		if (f == string::npos)
			continue;

		int val = atoi(sDatas.substr(0, f).c_str());
		_sysStrings[val] = sDatas.substr(f + 1, sDatas.size());
	}

	return true;
}

string DataManager::GetSysString(int code)
{
	if(code < 0 || code >= 20480)
		return unknown_string;
	return _sysStrings[code].c_str();
}

int DataManager::getLevelProgress(int currLevel, int currExp)
{
	for (int i = 0; i < lstLevelInfos.size(); i++){
		LevelInfo _lvl = lstLevelInfos.at(i);
		if (_lvl.level == currLevel){
            float org = _lvl.currExp;
            float to = _lvl.needExp;
			float _delta = (currExp-org) / to;
			return _delta * 100;
		}
	}
	return 0;
}
int DataManager::getExpNeed(int currLevel, int currExp){
    for (int i = 0; i < lstLevelInfos.size(); i++){
        LevelInfo _lvl = lstLevelInfos.at(i);
        if (_lvl.level == (currLevel)){
            float _delta = (_lvl.currExp + _lvl.needExp);
            _delta = _delta - currExp;
            return _delta;
        }
    }
    return 0;
}
AppConfig& DataManager::getAppConfig()
{
	return mAppConfig;
}

void DataManager::saveAppConfig()
{
	rapidjson::Document d;
	d.SetObject();
	/*
	version_android;
	version_ios;
	url_notification;
	ip_default;
	url_list_ip;
	callcenter;
	*/
	Value version_android("version_android");
}
