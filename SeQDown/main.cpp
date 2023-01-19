#include<string>
#include<fstream>
#include"httpClient.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	httpClient client;
	std::ofstream file("D:/Test.png", std::ios::binary);
	client.OnData = [&](const std::string_view data)
	{
		file << data;
	};
	client.Get("https://raw.githubusercontent.com/Pikselas/jellymilk/master/profile_pics/Brandy%20Renee.png");
	file.close();
	return 0;
}