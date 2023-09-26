#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <queue>
#include <algorithm>

#include "singleton.h"
#include "utils.h"

enum class CharType {
	Unknown,
	WhiteSpace,
	NumberLiteral,
	StringLiteral,
	IdentifierAndKeyword,
	OperatorAndPunctuator,
};

static CharType getCharType(wchar_t c) {
	if (L' ' == c || L'\t' == c || L'\r' == c || L'\n' == c) {
		return CharType::WhiteSpace;
	}
	if (L'0' <= c && c <= L'9') {
		return CharType::NumberLiteral;
	}
	if (c == L'\"') {
		return CharType::StringLiteral;
	}
	if (L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || c == '_') {
		return CharType::IdentifierAndKeyword;
	}
	if (33 <= c && c <= 47 && c != '\'' ||
		58 <= c && c <= 64 ||
		91 <= c && c <= 96 ||
		123 <= c && c <= 126) {
		return CharType::OperatorAndPunctuator;
	}
	return CharType::Unknown;
}

enum class TokenKind {
	Unknown,
	True, False,
	Int, Float,
	String,
	Vector2, Vector4, Rect,
	Parameter,
	Char, Array,

	Add, Sub, Mul, Div,
	Negative,
	IsEqual, IsNotEqual, IsGreat, IsLess, IsGreatEqual, IsLessEqual,
	LeftParent, RightParent,
	ArrStart, ArrEnd,
	Assign, Comma,

	Or, And, Not,

	Identifier,

	For, While, Break,

	New, Define, Print, Show, Hide, Await, Input, Quit, Save, Load, SaveStart, Persistent,
	Label, Include, Choice, Case, Jump, Call, Return, Scene, Command, Thread, Tween, Audio, CallMain, JumpMain,
	CharDef, ArrDef, IntDef, FloatDef, StringDef, BoolDef, Vector2Def, Vector4Def,
	If, Elif, Else,

	Function, Remove, Empty, Length, Insert, Append, Count, Sort, Delete
};

static std::map<TokenKind, int> evalPriority = {
	{TokenKind::LeftParent, -2},
	{TokenKind::And, -1}, {TokenKind::Or, 0},
	{TokenKind::Not, 1},
	{TokenKind::IsEqual, 2}, {TokenKind::IsNotEqual, 2},
	{TokenKind::IsGreat, 2}, {TokenKind::IsLess, 2},
	{TokenKind::IsGreatEqual, 2}, {TokenKind::IsLessEqual, 2},
	{TokenKind::Add, 3}, {TokenKind::Sub, 3},
	{TokenKind::Mul, 4}, {TokenKind::Div, 4},
	{TokenKind::Negative, 5},
	{TokenKind::Function, 6},

	{TokenKind::Remove, 6},
	{TokenKind::Length, 6},
	{TokenKind::Empty, 6},
	{TokenKind::Insert, 6},
	{TokenKind::Append, 6},
	{TokenKind::Count, 6},
	{TokenKind::Sort, 6},
	{TokenKind::Delete, 6},
};


static std::map<TokenKind, std::wstring> kind2String = {
	{TokenKind::Unknown, L"Unknown"},
	{TokenKind::True, L"True"},
	{TokenKind::False, L"False"},
	{TokenKind::Int, L"Int"},
	{TokenKind::Float,L"Float"},
	{TokenKind::String, L"String"},
	{TokenKind::Vector2, L"Vector2"},
	{TokenKind::Vector4, L"Vector4"},
	{TokenKind::Rect, L"Rect"},
	{TokenKind::Parameter, L"Parameter"},
	{TokenKind::Char, L"Char"},
	{TokenKind::Array, L"Array"},
	{TokenKind::Add, L"Add"},
	{TokenKind::Sub, L"Sub"},
	{TokenKind::Mul, L"Mul"},
	{TokenKind::Div, L"Div"},
	{TokenKind::Negative, L"Negative"},
	{TokenKind::IsEqual, L"IsEqual"},
	{TokenKind::IsNotEqual, L"IsNotEqual"},
	{TokenKind::IsGreat, L"IsGreat"},
	{TokenKind::IsLess, L"IsLess"},
	{TokenKind::IsGreatEqual, L"IsGreatEqual"},
	{TokenKind::IsLessEqual, L"IsLessEqual"},
	{TokenKind::ArrStart, L"ArrStart"},
	{TokenKind::ArrEnd, L"ArrEnd"},
	{TokenKind::Assign, L"Assign"},
	{TokenKind::Or, L"Or"},
	{TokenKind::And, L"And"},
	{TokenKind::Not, L"Not"},
	{TokenKind::Comma, L"Comma"},
	{TokenKind::New , L"New"},
	{TokenKind::For , L"For"},
	{TokenKind::While , L"While"},
	{TokenKind::Break , L"Break"},
	{TokenKind::Identifier , L"Identifier"},
	{TokenKind::Define , L"Define"},
	{TokenKind::Print , L"Print"},
	{TokenKind::Show , L"Show"},
	{TokenKind::Hide , L"Hide"},
	{TokenKind::Await , L"Await"},
	{TokenKind::Input , L"Input"},
	{TokenKind::Save , L"Save"},
	{TokenKind::SaveStart , L"SaveStart"},
	{TokenKind::Persistent , L"Persistent"},
	{TokenKind::Load , L"Load"},
	{TokenKind::Label, L"Label"},
	{TokenKind::Command, L"Command"},
	{TokenKind::Thread, L"Thread"},
	{TokenKind::Tween, L"Tween"},
	{TokenKind::Audio, L"Audio"},
	{TokenKind::Include , L"Include"},
	{TokenKind::Choice , L"Choice"},
	{TokenKind::Case , L"Case"},
	{TokenKind::Jump , L"Jump"},
	{TokenKind::Call , L"Call"},
	{TokenKind::CallMain , L"CallMain"},
	{TokenKind::JumpMain , L"JumpMain"},
	{TokenKind::Return , L"Return"},
	{TokenKind::Scene , L"Scene"},
	{TokenKind::Quit , L"Quit"},
	{TokenKind::CharDef , L"CharDef"},
	{TokenKind::ArrDef , L"ArrDef"},
	{TokenKind::IntDef , L"IntDef"},
	{TokenKind::FloatDef , L"FloatDef"},
	{TokenKind::StringDef , L"StringDef"},
	{TokenKind::BoolDef , L"BoolDef"},
	{TokenKind::Vector2Def , L"Vector2Def"},
	{TokenKind::Vector4Def , L"Vector4Def"},
	{TokenKind::If , L"If"},
	{TokenKind::Elif , L"Elif"},
	{TokenKind::Else , L"Else"},
	{TokenKind::Remove , L"Remove"},
	{TokenKind::Empty , L"Empty"},
	{TokenKind::Length , L"Length"},
	{TokenKind::Insert , L"Insert"},
	{TokenKind::Append , L"Append"},
	{TokenKind::Count , L"Count"},
	{TokenKind::Sort , L"Sort"},
	{TokenKind::Delete , L"Delete"},
};

struct Token {
	TokenKind type;
	std::wstring raw_data;
	void print() {
		std::wcout << L"[" << kind2String[type] << ", \"" << raw_data << L"\"]\n";
	}
	std::wstring data() {
		if (type == TokenKind::String) {
			return L"\"" + replaceAll(raw_data, L"\"", L"\\\"") + L"\"";
		}
		else {
			return raw_data;
		}
	}
	bool operator==(Token t) {
		if (t.type == TokenKind::String) {
			return t.type == this->type && t.raw_data == this->raw_data;
		}
		else {
			return t.type == this->type && eraseWhiteSpace(t.raw_data) == eraseWhiteSpace(this->raw_data);
		}
	}
};


std::vector<Token> getParameter(std::wstring src);

std::vector<Token> tokenize(std::wstring src, bool neglect_identifier = false);

Token eval(std::wstring src, bool neglect_identifier = false);

static inline int toInt(std::wstring src) { return stoi(eval(src).raw_data); }
static inline float toFloat(std::wstring src) {return stof(eval(src).raw_data);}
static inline bool toBool(std::wstring src) { if (eval(src).raw_data == L"true") return true; return false; }
static inline std::wstring toString(std::wstring src) {if (src.empty()) return L"";return eval(src).raw_data;}
static vec2 toVec2(std::wstring src) { std::wstring real = L""; int index = 0; while (src[index] != L'(') { index++; }index++; while (src[index] != L')') { real += src[index]; index++; } std::vector<std::wstring> elements = split(real, L','); for (auto& p : elements) { p = eraseWhiteSpace(p); }   return vec2(stof(eval(elements[0]).raw_data), stof(eval(elements[1]).raw_data)); }
static vec4 toVec4(std::wstring src) { std::wstring real = L""; int index = 0; while (src[index] != L'(') { index++; }index++; while (src[index] != L')') { real += src[index]; index++; } std::vector<std::wstring> elements = split(real, L','); for (auto& p : elements) { p = eraseWhiteSpace(p); }return vec4(stof(eval(elements[0]).raw_data), stof(eval(elements[1]).raw_data), stof(eval(elements[2]).raw_data), stof(eval(elements[3]).raw_data)); }
static rect toRect(std::wstring src) { std::wstring real = L""; int index = 0; while (src[index] != L'(') { index++; }index++; while (src[index] != L')') { real += src[index]; index++; } std::vector<std::wstring> elements = split(real, L','); for (auto& p : elements) { p = eraseWhiteSpace(p); }return rect(stof(eval(elements[0]).raw_data), stof(eval(elements[1]).raw_data), stof(eval(elements[2]).raw_data), stof(eval(elements[3]).raw_data)); }
static character toChar(std::wstring src) {
	int k = 1;
	std::wstring name = L"";
	while (!(src[k] == L'\"' && src[k - 1] != L'\\')) { name += src[k]; k++; }
	k++;
	vec4 color = toVec4(erasedFront(src, k));
	return character(name, color);
}

TokenKind getVariantType(std::wstring varName);

static inline void print(std::vector<std::wstring> vec) {
	std::wcout << L"-----------\n";
	for (auto& p : vec) {
		std::wcout << p << "\n";
	}
	std::wcout << L"-----------\n";
}

static inline void print(std::vector<Token> vec) {
	std::wcout << L"-----------\n";
	for (auto& p : vec) {
		p.print();
	}
	std::wcout << L"-----------\n";
}


static inline void print(std::queue<int> q) {
	std::wcout << L"-----------\n";
	std::queue<int> temp = q;
	while (temp.empty() == false) {
		std::cout << temp.front() << "\n";
		temp.pop();
	}
	std::wcout << L"-----------\n";
}

static inline void print(std::unordered_map<std::wstring, std::wstring> myMap) {
	for (std::unordered_map<std::wstring, std::wstring>::const_iterator it = myMap.begin(); it != myMap.end(); ++it) {
		std::wcout << it->first << " " << it->second << "\n";
	}
}

static std::wstring vec2toString(vec2 v) {
	return L"vec2(" + std::to_wstring(v.x) + L", " + std::to_wstring(v.y) + L")";
}

static std::wstring vec4toString(vec4 v) {
	return L"vec4(" + std::to_wstring(v.x) + L", " + std::to_wstring(v.y) + L", " + std::to_wstring(v.z) + L", " + std::to_wstring(v.w) + L")";
}

static std::wstring recttoString(rect v) {
	return L"vec4(" + std::to_wstring(v.x) + L", " + std::to_wstring(v.y) + L", " + std::to_wstring(v.w) + L", " + std::to_wstring(v.h) + L")";
}

static std::vector<std::vector<Token>> splitVector(std::vector<Token> vec, Token delimiter) {
	std::vector<std::vector<Token>> result;
	auto it = vec.begin();
	while (it != vec.end()) {
		auto next_it = std::find(it, vec.end(), delimiter);
		result.push_back(std::vector<Token>(it, next_it));
		if (next_it != vec.end()) {
			++next_it;
		}
		it = next_it;
	}
	return result;
}

std::vector<Token> arraytoTokens(std::wstring src);
std::wstring tokenstoArray(std::vector<Token> t);
#endif