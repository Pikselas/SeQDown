#pragma once
#include<queue>
#include<fstream>
#include<optional>
#include<algorithm>
#include<iterator>
#include<filesystem>
#include<thread>
#include<mutex>
#include<future>
#include<atomic>
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
	const std::string frontComponent;
	const std::string backComponent;
	const std::string cookie;
	const int use_threads;
	const size_t file_size;
	int count;
private:
	std::mutex mtx_file;
	std::mutex mtx_status;
private:
	unsigned char progress = 0;
	std::queue<std::string> StatusQueue;
	//std::queue<std::pair<std::string,std::string>> Failed;
private:
	std::atomic_bool stop = false;
private:
	void operator()();
public:
	Downloader(const std::string& file_path, const std::string& dest, const std::string& searchStart, const::std::string& searchEnd ,const int useThreads = 1 , const std::string& frontComponent = "", const std::string& backComponent = "", const Naming naming = Naming::SameAsURL, const std::string& name_last = "", const int count = 1 , const std::string& cookie = "");
	~Downloader();
public:
	unsigned char GetProgress() const;
	void Stop();
	std::optional<std::string> GetStatus();
	std::vector<std::future<std::optional<std::exception>>> Download();
	std::optional<std::string> GetNextLink(const std::string& searchBegin ,const std::string& searchEnd);
};