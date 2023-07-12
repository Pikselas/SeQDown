#include "WebClient.h"

WebRequest WebClient::request
(
	"/get_contents", 
	[](WebRequest::reader_type read, WebRequest::closer_type close) 
	{
        std::vector<std::string> elems;
        elems.reserve(7);
        std::vector<char> buffer(1024);
        const std::string SEARCH_STRING = "\r\n\r\n";
        int read_size = 0;
        int curr_total_size = 0;
        int last_found_pos = 0;
        int components_left = 9;
        while (components_left > 0 && (read_size = read(std::span<char>(buffer.data() + curr_total_size, buffer.size() - curr_total_size))) != -1)
        {
            auto search_start_it = buffer.begin() + std::clamp((long)(curr_total_size - SEARCH_STRING.size()), (long)last_found_pos, (long)buffer.size());
            const auto search_end_it = buffer.begin() + curr_total_size + read_size;

            while (components_left > 0)
            {
                if (auto it = std::search(search_start_it, search_end_it, SEARCH_STRING.begin(), SEARCH_STRING.end()); it != search_end_it)
                {
                    elems.emplace_back(buffer.begin() + last_found_pos, it);
                    last_found_pos = it - buffer.begin() + SEARCH_STRING.size();
                    search_start_it = it + SEARCH_STRING.size();
                    components_left--;
                }
                else
                {
                    break;
                }
            }

            curr_total_size += read_size;
            buffer.resize(curr_total_size + 1024);
        }
        // remove illegal characters from name
        elems.front().erase(std::remove_if(elems.front().begin(), elems.front().end(), [](char c)
            {
                return c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|';
            }), elems.front().end());
        // adding unique number to name
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 500);
        elems.front() += std::to_string(dist(rng));

        std::ofstream file(elems.front(), std::ios::binary);
        file << std::string_view(buffer.data() + last_found_pos, curr_total_size - last_found_pos);

        while ((read_size = read(buffer)) != -1)
        {
            file << std::string_view(buffer.data(), read_size);
        }
        close();

        std::thread([]
        {
			
		}).detach();

	}
);

void WebClient::Launch()
{
	request.Start();
}