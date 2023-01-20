#include"Window.h"
#include"TextButton.h"
#include"Label.h"
#include"TextEntry.h"
#include"WindowItems.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("SeQDown" ,400, 400);
	
	Label(window , "Select File Location", 10, 10,130, 20);
	TextButton textButton(window, "Browse", 210, 39, 100, 25);
	TextEntry textEntry(window, 10, 40, 200, 25);
	
	textButton.OnClick = [&textEntry](auto)
	{
		auto result = ShowOpenFileDialogue({ {"Text Files","*.txt;*.html"} });
		if (result)
		{
			textEntry.SetText(*result);
		}
	};

	Label(window, "Select Destination", 10, 70, 120, 20);
	TextButton textButton2(window, "Browse", 210, 99, 100, 25);
	TextEntry textEntry2(window, 10, 100, 200, 25);
	
	textButton2.OnClick = [&textEntry2](auto)
	{
		auto result = ShowOpenDirectoryDialogue();
		if (result)
		{
			textEntry2.SetText(*result);
		}
	};
	
	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}