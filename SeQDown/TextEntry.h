#pragma once
#include<vector>
#include"WindowComponent.h"

class TextEntry : public WindowComponent
{
public:
	std::function<void(TextEntry&)> OnTextChange;
private:
	std::string text;
public:
	TextEntry(WindowT auto& parent_window, int x, int y, int w, int h, const char* default_text = "")
		: 
		WindowComponent(parent_window, "EDIT", WS_EX_CLIENTEDGE, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, default_text, x, y, w, h , this)
	{
	}
	void PerformCommand(int wparam)
	{
		if (HIWORD(wparam) == EN_CHANGE)
		{
			int length = GetWindowTextLength(component_handle);
			std::vector<char> buffer(length + 1);
			GetWindowText(component_handle, buffer.data(), length + 1);
			text = buffer.data();
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
	const std::string GetText() const
	{
		return text;
	}
};