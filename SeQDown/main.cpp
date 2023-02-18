#include"Label.h"
#include"Window.h"
#include"TextEntry.h"
#include"TextArea.h"
#include"TextButton.h"
#include"WindowItems.h"
#include"RangeButton.h"
#include"DropDownSelect.h"
#include"ProgressBar.h"

#include"Downloader.h"

#include<memory>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window("SeQDown", 400, 450);
	
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

	Label t_label(window, "Threads: 1", 35, 373, 75, 20);
	RangeButton threads(window, 1, 10, 115, 370, 200, 25);

	threads.OnSlide = [&](auto& thr)
	{
		t_label.SetText("Threads: " + std::to_string(thr.GetCurrentPos()));
	};
	
	dropDownSelect.OnSelect = [&](auto& dd)
	{
		if (dd.GetSelection() == "Yes")
		{
			count.SetText("0");
		}
	};
	
	TextButton textButton3(window, "Start", 150, 410, 100, 25);
	
	textButton3.OnClick = [&](auto)
	{
		std::thread([&]() 
			{
			try 
			{
				Downloader downloader
				(
					source.GetText(), destination.GetText(), startStr.GetText(), endStr.GetText(),
					threads.GetCurrentPos(),
					frontLink.GetText(), dropDownSelect.GetSelection() == "Yes"
					?
					Downloader::Naming::SameAsURL
					:
					Downloader::Naming::CountStart,
					last_name.GetText(), std::stoi(count.GetText())
				);
				Window progress("Progress", 400, 300);
				ProgressBar progressBar(progress, 0, 100, 10, 10, 380, 25);
				TextArea status(progress, 10, 40, 380, 250,false,true);
				for (auto& thrd : downloader.Download())
				{
					while (!thrd._Is_ready() && progress.IsOpen())
					{
						progressBar.SetProgress(downloader.GetProgress());
						if (auto stat = downloader.GetStatus())
						{
							status.AppendText("\r\n" + *stat);
						}
						/*
						* Mainloop in main thread would not be able to see the progress window events
						* as creating a window in a new thread also creates a new message queue
						*/
						progress.ProcessEvents(Window::ProcessWindowEventsNonBlocking);
					}
					if (!progress.IsOpen())
					{
						downloader.Stop();
						break;
					}
					else if (auto err = thrd.get())
					{
						MessageBox(progress.window_handle, err->what(), "Error", MB_ICONERROR);
					}
				}
				if (progress.IsOpen())
				{
					MessageBox(progress.window_handle, "Completed", "Information", MB_ICONINFORMATION);
				}
			}
			catch (std::exception e)
			{
				MessageBox(window.window_handle, e.what(), "Error", MB_ICONERROR);
			}
			}).detach();
	};
	
	/*
	*  keep alive until all windows are closed
	*  process events for all windows created in current thread
	*/
	
	while (Window::GetWindowCount() > 0)
	{
		if (window.IsOpen())
		{
			Window::ProcessWindowEvents();
		}
	}
	return 0;
}