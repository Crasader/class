//
//  FileDownloader.cpp
//  iCasino_v2
//
//  Created by HoangDD on 7/10/14.
//
//

#include "FileDownloader.h"
#include "mUtils.h"
#include <fstream>

void FileDownloader::downLoadFile(Ref* target, SEL_CallFuncND callfun, string url, string fileNameEx){
	isDownloading = false;
	log("downLoadFile: %s", url.c_str());
	isErrorNotified = false;
	//Get File name
	string fileName = fileNameEx;
	if (fileNameEx.compare("") == 0){
		fileName = mUtils::getFileName(url);
	}

	//Kiem tra 
	if (url.compare("") == 0 || fileName.compare("") == 0)
	{
		return;
	}

	//
	//Check filename existed ?
	std::string writablePath = FileUtils::getInstance()->getWritablePath();
	writablePath.append(fileName);
	currFilePathDownload = writablePath;
	//Add 2 queue
	FileInfo fileInfo;
	fileInfo._callback = target;
	fileInfo.callbackListener = callfun;
	fileInfo.path = writablePath;
	fileInfo.url = url;
	lstFileInfo.push(fileInfo);
	downloadNextFile();
}

FileDownloader::FileDownloader()
{
	log("FileDownloader Constructor");
//	_downloader = std::shared_ptr<network::Downloader>(new network::Downloader);
//	_downloader->setErrorCallback(std::bind(&FileDownloader::errorCallback, this, std::placeholders::_1));
//	_downloader->setProgressCallback(std::bind(&FileDownloader::progressCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
//	_downloader->setSuccessCallback(std::bind(&FileDownloader::successCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

FileDownloader::~FileDownloader()
{
	log("Destructor FileDownloader");
	//
	//_downloader->~Downloader();
}


//void FileDownloader::errorCallback(const cocos2d::network::Downloader::Error& error){
//	if (lstFileInfo.size() == 0){
//		isDownloading = false;
//		return;
//	}
//	FileInfo fileInfo = lstFileInfo.front();
//	lstFileInfo.pop();
//	//
//	if (fileInfo._callback && fileInfo.callbackListener && !isErrorNotified)
//	{
//		isErrorNotified = true;
//		string str = "error";
//		log("error when download file: %s", error.message.c_str());
//		(fileInfo._callback->*fileInfo.callbackListener)(nullptr, (void*)&str);
//	}
//	isDownloading = false;
//}
//void FileDownloader::progressCallback(double totalToDownload, double nowDownloaded, const std::string& url, const std::string& customId){
//}
//void FileDownloader::successCallback(const std::string& url, const std::string& path, const std::string& customId){
//	if (lstFileInfo.size() == 0){
//		isDownloading = false;
//		return;
//	}
//	FileInfo fileInfo = lstFileInfo.front();
//	lstFileInfo.pop();
//	if (fileInfo._callback && fileInfo.callbackListener)
//	{
//		string str = path.c_str();
//		(fileInfo._callback->*fileInfo.callbackListener)(nullptr, (void*)&str);
//	}
//	isDownloading = false;
//}

std::string FileDownloader::getFileData(string strPath)
{
	return FileUtils::getInstance()->getStringFromFile(strPath);
}

bool FileDownloader::isDownloadProccess()
{
	return isDownloading;
}

std::string FileDownloader::getFilePathDownload()
{
	return currFilePathDownload;
}

void FileDownloader::downloadNextFile()
{
	if (!isDownloading){
		if (lstFileInfo.size() == 0)
			return;
		
		FileInfo fileInfo = lstFileInfo.front();
		currFilePathDownload = fileInfo.path;

		//_downloader->downloadAsync(fileInfo.url, fileInfo.path, "files");
		HttpRequest* request = new (std::nothrow) HttpRequest();
		request->setUrl(fileInfo.url.c_str());
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(FileDownloader::onHttpRequestCompleted, this));
		HttpClient::getInstance()->send(request);
		request->release();

		isDownloading = true;
	}
}

void FileDownloader::onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
	if (lstFileInfo.size() == 0){
		isDownloading = false;
		return;
	}
	//
	FileInfo fileInfo = lstFileInfo.front();
	lstFileInfo.pop();
	//
	log("FileDownloader::onHttpRequestCompleted");
	if (response)
	{
		long statusCode = response->getResponseCode();
		char statusString[64] = {};
		if (response->isSucceed())
		{
			// dump data
			std::vector<char> *buffer = response->getResponseData();
			string sData = buffer[0].data();
			sData = sData.substr(0, buffer[0].size());
			log("FileDownloader: datas: %s", sData.c_str());
			//
			if (fileInfo._callback && fileInfo.callbackListener)
			{
				(fileInfo._callback->*fileInfo.callbackListener)(nullptr, (void*)&sData);
			}
		}
		else{
			if (fileInfo._callback && fileInfo.callbackListener)
			{
				string sData = "error";
				(fileInfo._callback->*fileInfo.callbackListener)(nullptr, (void*)&sData);
			}
		}
	}
	//next file
	isDownloading = false;
	downloadNextFile();
}
