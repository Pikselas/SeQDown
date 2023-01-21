#pragma once
#include<queue>
#include<fstream>
#include<optional>
#include<algorithm>
#include<iterator>
#include<filesystem>
#include<thread>
#include<mutex>
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
	const std::string last_name;
	const std::string searchStart;
	const std::string searchEnd;
	int count;
private:
	std::mutex mtx;
private:
	std::queue<std::pair<std::string,std::string>> Failed;
private:
	void operator()();
public:
	Downloader(const std::string& file_path, const std::string& dest, const std::string& searchStart, const::std::string& searchEnd , const Naming naming = Naming::SameAsURL, const std::string& name_last = "", const int count = 0);
	~Downloader();
public:
	std::optional<std::string> GetNextLink(const std::string& searchBegin ,const std::string& searchEnd);
	void Download();
};