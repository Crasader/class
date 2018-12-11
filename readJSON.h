//
//  readJSON.h
//  iCasnio_v3
//
//  Created by Tuan Anh on 11/17/15.
//
//

#ifndef readJSON_h
#define readJSON_h
#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include <fstream>
using std::fstream;
using namespace  rapidjson;
using namespace cocos2d;
using namespace std;

class readJSON{
public:
static void createJSON(std::vector<std::string> data){
    rapidjson::Value array(rapidjson::kArrayType);
    Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
    std::vector<std::string> datas = data;
    for(int i = 0; i<datas.size();i++)
    {
        
       // log("data : %s",datas.at(i).c_str());
        rapidjson::Value vMemberName(rapidjson::kStringType);
        vMemberName.SetString(datas.at(i).c_str(), strlen(datas.at(i).c_str()));
       // log("i: %d",i);
        if (i==0)
        { 
            d.AddMember("packageUrl", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        if (i==1)
        {
            d.AddMember("remoteManifestUrl", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        if (i==2)
        {
            d.AddMember("remoteVersionUrl", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        if (i==3)
        {
            d.AddMember("version", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        if (i==4)
        {
            d.AddMember("engineVersion", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        
    }
    
    
    
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    
    auto path = FileUtils::getInstance()->fullPathForFilename("localedata/project.manifest");
    FILE* file = fopen(path.c_str(), "wb");
    if(file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
    }else{
        fstream files;
        files.open(path,fstream::out);
        files << buffer.GetString();
        files << fflush;
        files.close();
    }
    CCLOG("%s",buffer.GetString());
    
    //getJSONObject();
}
static std::vector<std::string> getJSONObject()
{
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("notification.json");
    
    
    rapidjson::Document readdoc;
    bool bRet = false;
    ssize_t size = 0;
    std::string load_str;
    
    unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
    load_str = std::string((const char*)titlech,size);
    std::vector<std::string> datas;
    //load_str = cocos2d::FileUtils::getInstance()->getStringFromFile("..\\myhero.json");
    readdoc.Parse<0>(load_str.c_str());
    if(readdoc.HasParseError())
    {
        CCLOG("GetParseError%u\n", readdoc.GetParseError());
        return datas;
    }
    
    rapidjson::Value& _array = readdoc["notification"];
    
    if(_array.IsArray())
    {
            for (int i = 0; i< _array.Size(); i++) {
            rapidjson::Value& arraydoc = _array[i];
            
            if(arraydoc.HasMember("date"))
            {
                auto date = arraydoc["date"].GetString();
                datas.push_back(date);
                
            }
            if(arraydoc.HasMember("message"))
            {
                auto message = arraydoc["message"].GetString();
                datas.push_back(message);
                
            }
        }
    }
    return datas;
    }
};
#endif /* readJSON_h */
