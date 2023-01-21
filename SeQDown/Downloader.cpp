#include "Downloader.h"

Downloader::Downloader(const std::string& file_path)
{
	file.open(file_path);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file");
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
