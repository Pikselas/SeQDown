#pragma once
#include<algorithm>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<thread>

#include"WebRequest.h"
#include"DownloadProgress.h"

class WebClient
{
private:
	static WebRequest request;
private:
	static void DownloadLauncher(WebRequest::reader_type read, WebRequest::closer_type close);
public:
	static void Launch();
};