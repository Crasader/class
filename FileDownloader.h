//
//  FileDownloader.h
//  iCasino_v2
//
//  Created by HoangDD on 7/10/14.
//
//

#ifndef __iCasino_v2__FileDownloader__
#define __iCasino_v2__FileDownloader__

#include <iostream>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;
using namespace cocos2d::network;
/*
 *Lớp này có tác dụng download 1 image from http url về máy, và hiển thị lên 1 Node
 */

class FileDownloader : public Ref{
private:
	std::shared_ptr<network::Downloader> _downloader;

	bool isErrorNotified;
	string currFilePathDownload;

	struct FileInfo
	{
		Ref* _callback;
		SEL_CallFuncND callbackListener;
		Node* pNodeTarget;
		string url;
		string path;
	};
	queue<FileInfo> lstFileInfo;

	bool isDownloading;

	void downloadNextFile();
public:
	FileDownloader();
	~FileDownloader();

	void downLoadFile(Ref* target, SEL_CallFuncND callfun, string url, string fileNameEx = "");

	string getFileData(string strPath);

//	void errorCallback(const cocos2d::network::Downloader::Error& error);
//	void progressCallback(double totalToDownload, double nowDownloaded, const std::string& url, const std::string& customId);
//	void successCallback(const std::string& url, const std::string& path, const std::string& customId);

	bool isDownloadProccess();

	string getFilePathDownload();

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
};

#endif /* defined(__iCasino_v2__FileDownloader__) */
