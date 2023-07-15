#pragma once

#include"Window.h"
#include"TextArea.h"
#include"ProgressBar.h"
#include "Downloader.h"

class DownloadProgress
{
private:
	Downloader downloader;
	Window window;
	ProgressBar progressBar;
	TextArea status;
public:
	DownloadProgress(const std::string& title , const std::string& file_path, const std::string& dest, const std::string& searchStart, const::std::string& searchEnd, const int useThreads, const std::string& frontComponent, const std::string& backComponent, const Downloader::Naming naming , const std::string& name_last, const int count , const std::string& cookie = "")
		:
	downloader(file_path,dest,searchStart,searchEnd,useThreads,frontComponent,backComponent,naming,name_last,count , cookie),
	window(title, 400, 300),
	progressBar(window, 0, 100, 10, 10, 380, 25),
	status(window,10, 40, 380, 250, false, true) {}
public:
	void Start()
	{
		window.SetFocus();
		try
		{
			for (auto& thrd : downloader.Download())
			{
				while (!thrd._Is_ready() && window.IsOpen())
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
					window.ProcessEvents(Window::ProcessWindowEventsNonBlocking);
				}
				if (!window.IsOpen())
				{
					downloader.Stop();
					break;
				}
				else if (auto err = thrd.get())
				{
					MessageBox(window.window_handle, err->what(), "Error", MB_ICONERROR);
				}
			}
			if (window.IsOpen())
			{
				MessageBox(window.window_handle, "Completed", "Information", MB_ICONINFORMATION);
			}
		}
		catch (std::exception e)
		{
			MessageBox(window.window_handle, e.what(), "Error", MB_ICONERROR);
		}
	}
};