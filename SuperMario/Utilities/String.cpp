#include "stdafx.h"
#include "String.h"

void String::SplitString(vector<string>& result, string origin, const string& tok)
{
	result.clear();

	int cutAt = 0;	// 위치
	// 몇번째에 원하는 토큰이 있는 지 반환
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0)
			result.push_back(origin.substr(0, cutAt));

		origin = origin.substr((size_t)cutAt + 1);
	}
	if (origin.length() > 0)
		result.push_back(origin);
}

void String::SplitString(vector<wstring>& result, wstring origin, const wstring& tok)
{
	result.clear();

	int cutAt = 0;	// 위치
	// 몇번째에 원하는 토큰이 있는 지 반환
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0)
			result.push_back(origin.substr(0, cutAt));

		origin = origin.substr((size_t)cutAt + 1);
	}
	if (origin.length() > 0)
		result.push_back(origin);
}

bool String::StartsWith(const string& str, const string& comp)
{
	string::size_type index = str.find(comp);

	if (index != string::npos && (int)index == 0)
		return true;

	return false;
}

bool String::StartsWith(const wstring& str, const wstring& comp)
{
	wstring::size_type index = str.find(comp);

	if (index != wstring::npos && (int)index == 0)
		return true;

	return false;
}

bool String::Contain(const string& str, const string& comp)
{
	size_t found = str.find(comp);

	return found != string::npos;
}

bool String::Contain(const wstring& str, const wstring& comp)
{
	size_t found = str.find(comp);

	return found != string::npos;
}

void String::Replace(string& str, const string& comp, const string& rep)
{
	size_t startPos = 0;
	while ((startPos = str.find(comp, startPos)) != string::npos)
	{
		str.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}
}

void String::Replace(wstring& str, const wstring& comp, const wstring& rep)
{
	size_t startPos = 0;
	while ((startPos = str.find(comp, startPos)) != wstring::npos)
	{
		str.replace(startPos, comp.length(), rep);
		startPos += rep.length();
	}
}

string String::ToString(const Vector2& vec2)
{
	string temp;
	temp = to_string(vec2.x) + to_string(vec2.y);

	return temp;
}

wstring String::ToWString(const string& value)
{
	wstring temp = L"";
	temp.assign(value.begin(), value.end());

	return temp;
}

string String::ToString(const wstring& value)
{
#pragma warning(disable : 4244)
	string temp = "";
	temp.assign(value.begin(), value.end());
#pragma warning(default : 4244)

	return temp;
}
