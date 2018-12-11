#ifndef LOGFILE
#define LOGFILE

#include <iostream>
#include <fstream>
#include <cocos2d.h>

#include "boost/date_time/posix_time/posix_time.hpp"
namespace pt = boost::posix_time;

using namespace std;



class LogFile{
/*
public:
    static void Write(const std::string& msg){
std::string path = cocos2d::FileUtils::getInstance()->getWritablePath();
        std::string time = boost::to_string(pt::microsec_clock::local_time().date());
        std::string time2 = boost::to_string(pt::microsec_clock::local_time().time_of_day());

        std::string fileName = path + time + "_logs.txt";
        std::ofstream out;
        out.open(fileName, std::ios::app);

        if(out.is_open())
        {
            out << "[" << time2 << "] " << msg << "\n";
            out.close();
        }
        else{
            //cocos2d::log("Khong the mo file");
        }
    }*/
};

#endif

