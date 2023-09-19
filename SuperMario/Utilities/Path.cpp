#include "stdafx.h"
#include "Path.h"

bool Path::ExistFile(const string& path)
{
	return ExistFile(String::ToWString(path));
}

bool Path::ExistFile(const wstring& path)
{
	DWORD fileValue = GetFileAttributes(path.c_str());

	return fileValue < 0xFFFFFFFF;
}

bool Path::ExistDirectory(const string& path)
{
	return false;
}

bool Path::ExistDirectory(const wstring& path)
{
	DWORD attribute = GetFileAttributes(path.c_str());

	return attribute != INVALID_FILE_ATTRIBUTES && (attribute & FILE_ATTRIBUTE_DIRECTORY);
}

string Path::Combine(const string& path1, const string& path2)
{
	return path1 + path2;
}

wstring Path::Combine(const wstring& path1, const wstring& path2)
{
	return path1 + path2;
}

string Path::Combine(const vector<string>& paths)
{
	string temp = "";
	for (const string& path : paths)
		temp += path;

	return temp;
}

wstring Path::Combine(const vector<wstring>& paths)
{
	wstring temp = L"";
	for (const wstring& path : paths)
		temp += path;

	return temp;
}

string Path::GetDirectoryName(string path)
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

wstring Path::GetDirectoryName(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(0, index + 1);
}

string Path::GetDirectoryName(string path, int count)
{
	string tempPath = path;
	String::Replace(path, "\\", "/");
	size_t lastIndex = path.find_last_of('/');
	size_t fastIndex = lastIndex;
	while (count > 0)
	{
		tempPath = tempPath.substr(0, fastIndex);
		fastIndex = tempPath.find_last_of('/');
		--count;
	}

	return  path.substr(fastIndex + 1, lastIndex + 1);
}

wstring Path::GetDirectoryName(wstring path, int count)
{
	wstring tempPath = path;
	String::Replace(path, L"\\", L"/");
	size_t lastIndex = path.find_last_of('/');
	size_t fastIndex = lastIndex;
	while (count > 0)
	{
		tempPath = tempPath.substr(0, fastIndex);
		fastIndex = tempPath.find_last_of('/');
		--count;
	}

	return  path.substr(fastIndex + 1, lastIndex + 1);
}

string Path::GetExtension(string path)
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1);
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1);
}

string Path::GetFileName(string path)	// 파일 이름 확장자 포함
{
	String::Replace(path, "\\", "/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1);
}

wstring Path::GetFileName(wstring path)
{
	String::Replace(path, L"\\", L"/");
	size_t index = path.find_last_of('/');

	return path.substr(index + 1);
}

string Path::GetFileNameWithoutExtension(const string& path)	// 파일 이름 확장자 미포함
{
	string fileName = GetFileName(path);
	size_t index = fileName.find_last_of('.');

	return fileName.substr(0, index);
}

wstring Path::GetFileNameWithoutExtension(const wstring& path)
{
	wstring fileName = GetFileName(path);
	size_t index = fileName.find_last_of('.');

	return fileName.substr(0, index);
}

const WCHAR* Path::ImageFilter = L"Image File (*.png;*.bmp;*.jpg)\0*.png;*.bmp;*.jpg";
const WCHAR* Path::ShaderFilter = L"HLSL File (*.hlsl)\0*.hlsl";
const WCHAR* Path::TextFilter = L"Text File (*.txt)\0*.txt";
const WCHAR* Path::SoundFilter = L"Sound File (*.mp3;*.wav;*.flac)\0*.mp3;*.wav;*.flac";
const WCHAR* Path::TileMapFilter = L"TileMap File (*.tile)\0*.tile";

void Path::OpenFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	String::Replace(tempFolder, L"/", L"\\");

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrTitle = L"불러오기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();	// 창을 켰을때, 초기 경로
	ofn.Flags = OFN_NOCHANGEDIR;	// 경로가 변경되고 다음번에 켰을 경우, 초기 경로 변경 옵션

	if(GetOpenFileName(&ofn))
	{
		if (func != nullptr)
		{
#pragma warning(disable : 6054)
			wstring loadName = name;
			String::Replace(loadName, L"\\", L"/");
			func(loadName);
#pragma warning(default : 6054)
		}
	}
}

void Path::SaveFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd)
{
	WCHAR name[255];
	wcscpy_s(name, file.c_str());

	wstring tempFolder = folder;
	String::Replace(tempFolder, L"/", L"\\");

	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = name;
	ofn.lpstrTitle = L"저장하기";
	ofn.nMaxFile = 255;
	ofn.lpstrInitialDir = tempFolder.c_str();	// 창을 켰을때, 초기 경로
	ofn.Flags = OFN_NOCHANGEDIR;	// 경로가 변경되고 다음뻔에 켰을 경우, 초기 경로 변경 옵션
	if (filter == TextFilter)
		ofn.lpstrDefExt = L".txt";
	else if (filter == ShaderFilter)
		ofn.lpstrDefExt = L".hlsl";

	if (GetOpenFileName(&ofn))
	{
		if (func != nullptr)
		{
#pragma warning(disable : 6054)
			wstring saveName = name;
			String::Replace(saveName, L"\\", L"/");
			func(saveName);
#pragma warning(default : 6054)
		}
	}
}

void Path::GetFiles(vector<string>& files, const string& path, const string& filter, bool bFindSubFolder)
{
	vector<wstring> wFiles;
	wstring wPath = String::ToWString(path);
	wstring wFilter = String::ToWString(filter);

	GetFiles(wFiles, wPath, wFilter, bFindSubFolder);
	for (const auto& str : wFiles)
		files.push_back(String::ToString(str));
}

void Path::GetFiles(vector<wstring>& files, const wstring& path, const wstring& filter, bool bFindSubFolder)
{
	wstring file = path + filter;

	WIN32_FIND_DATA findData;
	HANDLE handle = FindFirstFile(file.c_str(), &findData);
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				if (bFindSubFolder && findData.cFileName[0] != '.')
				{
					wstring folder = path + wstring(findData.cFileName) + L"/";
					GetFiles(files, folder, filter, bFindSubFolder);
				}
			}
			else
			{
				wstring fileName = path + wstring(findData.cFileName);
				files.push_back(fileName);
			}
		} while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
}

void Path::CreateFolder(const string& path)
{
	CreateFolder(String::ToWString(path));
}

void Path::CreateFolder(const wstring& path)
{
	if (!ExistDirectory(path))	// 없을 경우 생성
		CreateDirectory(path.c_str(), nullptr);	// 폴더 생성 함수
}

void Path::CreateFolders(const string& path)
{
	CreateFolders(String::ToWString(path));
}

void Path::CreateFolders(wstring path)
{
	String::Replace(path, L"\\", L"/");

	vector<wstring> folders;
	String::SplitString(folders, path, L"/");

	wstring temp = L"";
	for (const auto& folder : folders)
	{
		temp += folder + L"/";
		CreateFolder(temp);
	}
}
