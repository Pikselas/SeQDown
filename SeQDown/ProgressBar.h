#pragma once
#include"WindowComponent.h"

class ProgressBar : public WindowComponent
{
public:
	ProgressBar(WindowT auto& Wnd, int min , int max ,int x, int y, int w, int h)
		:
	WindowComponent(Wnd, PROGRESS_CLASS, 0, WS_CHILD | WS_VISIBLE, "" , x, y, w, h, this) 
	{
		SendMessage(component_handle, PBM_SETRANGE, 0, MAKELPARAM(min, max));
	}
	void PerformCommand(int wparam) override {}
	void SetProgress(int pos)
	{
		SendMessage(component_handle, PBM_SETPOS, pos, 0);
	}
};