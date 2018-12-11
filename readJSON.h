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

using namespace  rapidjson;
using namespace cocos2d;
using namespace std;

class readJSON{
public:
static void createJSON(std::vector<std::string> data){
    rapidjson::Document writedoc;
    writedoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
    rapidjson::Value array(rapidjson::kArrayType);
   
    rapidjson::Value object(rapidjson::kObjectType);
    
    std::vector<std::string> datas = data;
    
    for(int i = 0; i<datas.size();i++)
    {
        
       // log("data : %s",datas.at(i).c_str());
        rapidjson::Value vMemberName(rapidjson::kStringType);
        vMemberName.SetString(datas.at(i).c_str(), strlen(datas.at(i).c_str()));
       // log("i: %d",i);
        if (i%2==0)
        { 
            object.AddMember("date", vMemberName, allocator);
            //array.PushBack(object, allocator);
        }
        else
        {
            object.AddMember("message", vMemberName, allocator);
            array.PushBack(object, allocator);
            object = rapidjson::Value(rapidjson::kObjectType);
          //  object.Empty();
        }
        
    }
    
    
    
    writedoc.AddMember("notification", array, allocator);
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    writedoc.Accept(writer);
    
    auto path = FileUtils::getInstance()->getWritablePath();
    path.append("notification.json");
    FILE* file = fopen(path.c_str(), "wb");
    if(file)
    {
        fputs(buffer.GetString(), file);
        fclose(file);
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
