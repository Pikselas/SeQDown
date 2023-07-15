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
private:
	CURL* curl;
public:
	httpClient()
	{
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	}
	~httpClient()
	{
		curl_easy_cleanup(curl);
	}
public:
	void SetCookie(const std::string& cookie)
	{
		curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
	}
	void Get(const std::string& url)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		
		// pass this pointer to callback function for access to OnData
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

		// redirects if needed
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			throw std::runtime_error(curl_easy_strerror(res));
		}
	}
};