#pragma once
#include<queue>
#include<fstream>
#include<optional>
#include<algorithm>
#include<iterator>
#include"httpClient.h"

class Downloader
{
private:
	std::ifstream file;
public:
	Downloader(const std::string& file_path);
	~Downloader();
public:
	std::optional<std::string> GetNextLink(const std::string& searchBegin ,const std::string& searchEnd);
};