#pragma once
#include <Windows.h>
#include <commdlg.h>
#include<string>
#include <optional>
#include<vector>

// file_filter_map: key = file Type name, value = file extension (seperated by ';')
// returns: file path if user selected a file, std::nullopt if user cancelled
// example usage: std::optional<std::string> file_path = OpenFile("Select a file", { {"Text Files", "*.txt"}, {"All Files", "*.*"} });
std::optional<std::string> ShowOpenFileDialog(const std::initializer_list<std::pair<std::string, std::string>>& file_filter_map)
{
    OPENFILENAME ofn;
    //maximum length of a file name
    char szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	std::vector<char> file_filter_buffer;
	for (auto& [type, ext] : file_filter_map)
	{
		file_filter_buffer.insert(file_filter_buffer.end(), type.begin(), type.end());
		file_filter_buffer.push_back('\0');
		file_filter_buffer.insert(file_filter_buffer.end(), ext.begin(), ext.end());
		file_filter_buffer.push_back('\0');
	}
	//when the api detects 2 null characters it stops reading the string
	file_filter_buffer.push_back('\0');
	ofn.lpstrFilter = file_filter_buffer.data();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        return ofn.lpstrFile;
    }
    return std::nullopt;
}
