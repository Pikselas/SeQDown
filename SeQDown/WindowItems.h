#pragma once
#include <Windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include<string>
#include <optional>
#include<vector>

// file_filter_map: key = file Type name, value = file extension (seperated by ';')
// returns: file path if user selected a file, std::nullopt if user cancelled
// example usage: std::optional<std::string> file_path = OpenFile("Select a file", { {"Text Files", "*.txt"}, {"All Files", "*.*"} });
std::optional<std::string> ShowOpenFileDialogue(const std::initializer_list<std::pair<std::string, std::string>>& file_filter_map , HWND parent = nullptr)
{
    OPENFILENAME ofn;
    //maximum length of a file name
    char szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent;
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

// returns: directory path if user selected a directory, std::nullopt if user cancelled
std::optional<std::string> ShowOpenDirectoryDialogue(HWND parent = nullptr)
{
    // Initialize the BROWSEINFO structure
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(bi));
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    bi.hwndOwner = parent; // hWnd is a handle to the parent window
    //bi.lpszTitle = _T("Select a directory:");

    // Show the dialog box
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != 0)
    {
        // Get the path of the selected directory
        TCHAR path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path))
        {
			return path;
        }

        // Free the ITEMIDLIST
        CoTaskMemFree(pidl);
    }
	return std::nullopt;
}