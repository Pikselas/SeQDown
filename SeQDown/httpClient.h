#include<curl/curl.h>
#include<string>
#include<functional>
#include<iostream>


class httpClient
{
public:
	std::function<void(const std::string_view)> OnData = nullptr;
private:
	static size_t write_callback(char* ptr, size_t size, size_t nmemb, httpClient *userdata)
	{
		if (userdata->OnData)
			userdata->OnData(std::string_view(ptr, size * nmemb));
		return size * nmemb;
	}
public:

	void Get(const std::string& url)
	{
		CURL* curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		
		// pass this pointer to callback function for access to OnData
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		// redirects if needed
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (res != CURLE_OK)
		{
			throw std::runtime_error(curl_easy_strerror(res));
		}
	}
};