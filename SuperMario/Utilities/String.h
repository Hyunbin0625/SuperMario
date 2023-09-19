#pragma once

class String
{
public:
	static void SplitString(vector<string>& result, string origin, const string& tok);
	static void SplitString(vector<wstring>& result, wstring origin, const wstring& tok);

	static bool StartsWith(const string& str, const string& comp);
	static bool StartsWith(const wstring& str, const wstring& comp);

	static bool Contain(const string& str, const string& comp);
	static bool Contain(const wstring& str, const wstring& comp);

	static void Replace(string& str, const string& comp, const string& rep);
	static void Replace(wstring& str, const wstring& comp, const wstring& rep);

	static string ToString(const Vector2& vec2);

	static wstring ToWString(const string& value);
	static string ToString(const wstring& value);
};
