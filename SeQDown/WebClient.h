#pragma once
#include<algorithm>
#include<fstream>
#include<string>
#include<vector>
#include<random>
#include<thread>

#include"WebRequest.h"
#include"Downloader.h"

class WebClient
{
private:
	static WebRequest request;
public:
	static void Launch();
};