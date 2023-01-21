#include "Downloader.h"

Downloader::Downloader(const std::string& file_path, const std::string& dest, const std::string& searchStart, const::std::string& searchEnd, const Naming naming, const std::string& name_last , const int count)
	: 
dest(dest) ,
naming(naming),
last_name(name_last),
searchStart(searchStart),
searchEnd(searchEnd),
count(count)
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
	
	std::string Link;
	
	while (file.good())
	{
		file >> Link;
		auto Pos = std::search(Link.rbegin(), Link.rend(), searchEnd.rbegin(), searchEnd.rend());
		if (Pos != Link.rend())
		{
			Link.erase(Pos.base() - 1, Link.end());
			file.seekg(Pos - Link.rbegin(), std::ios::cur);
			return Link;
		}
	}
	return std::nullopt;
}

void Downloader::Download()
{
	(*this)();
}

void Downloader::operator()()
{
	while (true)
	{
		std::string FileName;
		std::string Link;
		
		{
			std::unique_lock<std::mutex> lock(mtx);
			
			auto link = GetNextLink(searchStart, searchEnd);
			if (!link)
			{
				break;
			}
			Link = *link;
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
				httpClient client;
				client.OnData = [&](std::string_view data)
				{
					file << data;
				};
				client.Get(Link);
			}
			catch (const std::exception& e)
			{
				Failed.emplace(FileName,Link);
			}
		}
	}
}