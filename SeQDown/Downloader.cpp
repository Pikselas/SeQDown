#include "Downloader.h"

Downloader::Downloader(const std::string& file_path, const std::string& dest, const std::string& searchStart, const::std::string& searchEnd, const int useThreads, const std::string& frontComponent , const std::string& backComponent, const Naming naming, const std::string& name_last , const int count , const std::string& cookie)
	: 
dest(dest) ,
naming(naming),
last_name(name_last),
searchStart(searchStart),
searchEnd(searchEnd),
use_threads(useThreads),
frontComponent(frontComponent),
backComponent(backComponent),
file_size(std::filesystem::file_size(file_path)),
count(count),
cookie(cookie)
{
	file.open(file_path);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}
	if (!std::filesystem::exists(dest))
	{
		std::filesystem::create_directories(dest);
	}
}

Downloader::~Downloader()
{
	file.close();
}

std::optional<std::string> Downloader::GetNextLink(const std::string& searchBegin, const std::string& searchEnd)
{	
	std::istreambuf_iterator<char> start(file);
	std::istreambuf_iterator<char> end;
	
	auto StartPos = std::search(start, end, searchBegin.begin(), searchBegin.end());

	if (StartPos == end)
	{
		return std::nullopt;
	}

	// using Search automatically advances the file pointer
	// so we can't use the start iterator anymore to find the end 
	// and if we use StartPos for searching we need to go back to the StartPos by decrementing the file pointer
	
	progress = (file.tellg() * 100) / file_size;
	
	std::string Link;
	
	while (file.good())
	{
		file >> Link;
		auto Pos = std::search(Link.rbegin(), Link.rend(), searchEnd.rbegin(), searchEnd.rend());
		if (Pos != Link.rend())
		{
			Link.erase(Pos.base() - searchEnd.size(), Link.end());
			file.seekg(Pos - Link.rbegin(), std::ios::cur);
			return Link;
		}
	}
	return std::nullopt;
}

unsigned char Downloader::GetProgress() const
{
	return progress;
}

void Downloader::Stop()
{
	stop = true;
}

std::optional<std::string> Downloader::GetStatus()
{
	std::unique_lock<std::mutex> lock(mtx_status);
	if (!StatusQueue.empty())
	{
		auto status = StatusQueue.front();
		StatusQueue.pop();
		return status;
	}
	return std::nullopt;
}

std::vector<std::future<std::optional<std::exception>>> Downloader::Download()
{
	std::vector<std::future<std::optional<std::exception>>> threads;
	threads.reserve(use_threads);
	
	for (int i = 0; i < use_threads; ++i)
	{
		threads.emplace_back(std::async(std::launch::async, [&]() -> std::optional<std::exception> 
		{
			try
			{
			   (*this)();
			}
			catch (std::exception e)
			{
				return e;
			}
			return std::nullopt;
		}));
	}
	return threads;
}

void Downloader::operator()()
{
	httpClient client;
	client.SetCookie(cookie);
	while (!stop)
	{
		std::string FileName;
		std::string Link;

		{
			std::unique_lock<std::mutex> lock(mtx_file);

			auto link = GetNextLink(searchStart, searchEnd);
			if (!link)
			{
				break;
			}
			Link = frontComponent + *link + backComponent;
			switch (naming)
			{
			case Naming::SameAsURL:
				lock.unlock();
				FileName = std::filesystem::path(Link).filename().string();
				break;
			case Naming::CountStart:
				FileName = std::to_string(count);
				count++;
				break;
			}
		}
		FileName += last_name;
		if (!std::filesystem::exists(FileName))
		{
			try
			{
				auto Path = std::filesystem::path(dest) / FileName;
				std::ofstream file(Path, std::ios::binary);
				client.OnData = [&](std::string_view data)
				{
					file << data;
				};
				
				std::unique_lock<std::mutex> lock(mtx_status);
				StatusQueue.push("Downloading: " + FileName + " from " + Link);
				lock.unlock();
				
				client.Get(Link);
				
				lock.lock();
				StatusQueue.push("Downloaded: " + FileName);
				lock.unlock();
			}
			catch (const std::exception& e)
			{
				//Failed.emplace(FileName, Link);
				throw e;
			}
		}
	}
}