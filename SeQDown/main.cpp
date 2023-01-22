#include"Label.h"
#include"Window.h"
#include"TextEntry.h"
#include"TextButton.h"
#include"WindowItems.h"
#include"DropDownSelect.h"

#include"Downloader.h"

#include<memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("SeQDown", 400, 400);
	
	Label(window , "Select File Location", 10, 10,130, 20);
	TextButton textButton(window, "Browse", 250, 39, 130, 25);
	TextEntry source(window, 20, 40, 230, 25);
	
	textButton.OnClick = [&](auto)
	{
		auto result = ShowOpenFileDialogue({ {"Text Files","*.txt;*.html"} } , window.window_handle);
		if (result)
		{
			source.SetText(*result);
		}
	};

	Label(window, "Select Destination", 10, 70, 120, 20);
	TextButton textButton2(window, "Browse", 250, 99, 130, 25);
	TextEntry destination(window, 20, 100, 230, 25);
	
	textButton2.OnClick = [&](auto)
	{
		auto result = ShowOpenDirectoryDialogue(window.window_handle);
		if (result)
		{
			destination.SetText(*result);
		}
	};
	
	Label(window, "Parse Link", 10, 130, 110, 20);
	Label(window, "Start string", 15, 160, 80, 20);
	TextEntry startStr(window, 105, 160, 100, 25);
	Label(window, "End string", 15, 195, 80, 20);
	TextEntry endStr(window, 105, 195, 100, 25);
	Label(window, "Front Link", 220, 160, 80, 20);
	TextEntry frontLink(window, 230, 185, 150, 25);

	Label(window, "Naming Items", 10, 230, 110, 20);
	Label(window, "Same as URL?", 50, 260, 100, 20);
	DropDownSelect dropDownSelect(window, 160, 260, 100, 150);
	dropDownSelect.AddItem("Yes");
	dropDownSelect.AddItem("No");

	Label(window, "Count Start", 50, 295, 80, 20);
	TextEntry count(window, 140, 295, 100, 25);
	Label(window, "Last Name ", 50, 330, 80, 20);
	TextEntry last_name(window, 140, 330, 100, 25);

	TextButton textButton3(window, "Start", 150, 360, 100, 25);
	
	textButton3.OnClick = [&](auto) mutable
	{
		Downloader downloader
		(
			source.GetText(), destination.GetText(), startStr.GetText(), endStr.GetText(),
			frontLink.GetText(), dropDownSelect.GetSelection() == "Yes" 
						? 
			Downloader::Naming::SameAsURL 
						: 
			Downloader::Naming::CountStart,
			last_name.GetText() , std::stoi(count.GetText())
		);
			downloader.Download();
	};
	
	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}