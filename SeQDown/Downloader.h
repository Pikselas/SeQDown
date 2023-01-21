#pragma once
#include<queue>
#include<fstream>
#include<optional>
#include<algorithm>
#include<iterator>
#include<filesystem>
#include"httpClient.h"

class Downloader
{
private:
	std::ifstream file;
	const std::string dest;
public:
	enum class Naming
	{
		SameAsURL,
		CountStart
	};
private:
	const Naming naming;
	const std::string extention;
	int count;
public:
	Downloader(const std::string& file_path, const std::string& dest, const Naming naming = Naming::SameAsURL, const std::string& extention = "", const int count = 0);
	~Downloader();
public:
	std::optional<std::string> GetNextLink(const std::string& searchBegin ,const std::string& searchEnd);
};