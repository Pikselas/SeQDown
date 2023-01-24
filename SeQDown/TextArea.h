#pragma once
#include<string>
#include"WindowComponent.h"

class TextArea : public WindowComponent
{
public:
	std::function<void(TextArea&)> OnTextChange;
public:
	TextArea(WindowT auto& parent_window, int x, int y, int w, int h,bool autohScroll = true , bool autovScroll = true, const char* default_text = "")
		:
		WindowComponent(parent_window, "EDIT", WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_MULTILINE | (autohScroll? ES_AUTOHSCROLL : 0) | (autovScroll? ES_AUTOVSCROLL : 0), default_text, x, y, w, h, this)
	{}
	void PerformCommand(int wparam)
	{
		if (HIWORD(wparam) == EN_CHANGE)
		{
			if (OnTextChange)
			{
				OnTextChange(*this);
			}
		}
	}
	void SetText(const std::string& text)
	{
		SetWindowText(component_handle, text.c_str());
	}
	void AppendText(const std::string& text)
	{
		unsigned int nLength = GetWindowTextLength(component_handle);
		SendMessage(component_handle, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
		SendMessage(component_handle, EM_REPLACESEL, 0, (LPARAM)text.c_str());
	}
	const std::string GetText() const
	{
		std::vector<char> text(GetWindowTextLength(component_handle) + 1);
		GetWindowText(component_handle, (LPSTR)text.data(), text.size());
		return std::string(text.data());
	}
};