#include"Window.h"
#include"TextButton.h"
#include"Label.h"
#include"TextEntry.h"
#include"WindowItems.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("SeQDown" ,400, 400);
	Label label(window , "Select File Location", 10, 10, 70, 20);
	TextButton textButton(window, "Browse", 210, 39, 100, 25);
	TextEntry textEntry(window, 10, 40, 200, 25);
	textButton.OnClick = [&textEntry](auto)
	{
		auto result = ShowOpenFileDialog({ {"Text Files","*.txt;*.html"} });
		if (result)
		{
			textEntry.SetText(*result);
		}
	};
	Window::MainLoop(Window::ALL_WINDOWS);
	return 0;
}