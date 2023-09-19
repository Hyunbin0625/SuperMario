#pragma once

class Path
{
public:
	static bool ExistFile(const string& path);
	static bool ExistFile(const wstring& path);

	static bool ExistDirectory(const string& path);
	static bool ExistDirectory(const wstring& path);

	static string Combine(const string& path1, const string& path2);
	static wstring Combine(const wstring& path1, const wstring& path2);

	static string Combine(const vector<string>& paths);
	static wstring Combine(const vector<wstring>& paths);

	static string GetDirectoryName(string path);
	static wstring GetDirectoryName(wstring path);

	static string GetDirectoryName(string path, int count);
	static wstring GetDirectoryName(wstring path, int count);

	static string GetExtension(string path);
	static wstring GetExtension(wstring path);

	static string GetFileName(string path);
	static wstring GetFileName(wstring path);

	static string GetFileNameWithoutExtension(const string& path);
	static wstring GetFileNameWithoutExtension(const wstring& path);

	const static WCHAR* ImageFilter;
	const static WCHAR* ShaderFilter;
	const static WCHAR* TextFilter;
	const static WCHAR* SoundFilter;
	const static WCHAR* TileMapFilter;

	static void OpenFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd = nullptr);
	static void SaveFileDialog(const wstring& file, const WCHAR* filter, const wstring& folder, function<void(wstring)>& func, const HWND& hwnd = nullptr);

	static void GetFiles(vector<string>& files, const string& path, const string& filter, bool bFindSubFolder);
	static void GetFiles(vector<wstring>& files, const wstring& path, const wstring& filter, bool bFindSubFolder);

	static void CreateFolder(const string& path);
	static void CreateFolder(const wstring& path);

	static void CreateFolders(const string& path);
	static void CreateFolders(wstring path);
};
