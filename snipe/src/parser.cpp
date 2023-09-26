#include "script/parser.h"

std::vector<Token> getParameter(std::wstring src) {
	src.erase(src.begin());
	src.erase(src.end() - 1);
	std::vector<Token> result;
	std::vector<Token> temp = tokenize(src);
	std::vector<std::vector<Token>> splitted = splitVector(temp, { TokenKind::Comma, L"," });
	for (auto& p : splitted) {
		std::wstring element = L"";
		for (auto& o : p) {
			element += o.data();
		}
		result.push_back(eval(element));
	}
	return result;
}

std::vector<Token> arraytoTokens(std::wstring src) {
	src.erase(src.begin());
	src.erase(src.end() - 1);
	std::vector<Token> result{};
	std::vector<Token> temp = tokenize(src);
	std::vector<std::vector<Token>> splitted = splitVector(temp, { TokenKind::Comma, L"," });
	for (auto& p : splitted) {
		std::wstring element = L"";
		for (auto& o : p) {
			element += o.data();
		}
		Token t = eval(element);
		if (t.type != TokenKind::String) {
			t.raw_data += L" ";
		}
		result.push_back(t);
	}
	return result;
}


std::wstring tokenstoArray(std::vector<Token> t) {
	std::wstring result = L"";
	result += L'[';
	for (auto& p : t) {
		result += p.data() + L",";
	}
	if (result[result.size() - 1] == L',') {
		result.erase(result.end() - 1);
	}
	result += L']';
	return result;
}

TokenKind getVariantType(std::wstring varName) {
	if (Sgt::instance().varExists(varName) == false) assert(false);
	std::wstring target = Sgt::instance().varMap[varName];
	if (startsWith(target, L"[") && endsWith(target, L"]")) {
		return TokenKind::Array;
	}
	if (getCharType(target[0]) == CharType::NumberLiteral || target[0] == L'-') {
		if (target.find(L'.') != std::wstring::npos) return TokenKind::Float;
		else return TokenKind::Int;
	}
	if (target == L"true") return TokenKind::True;
	if (target == L"false") return TokenKind::False;
	if (startsWith(target, L"vec2")) return TokenKind::Vector2;
	if (startsWith(target, L"vec4")) return TokenKind::Vector4;
	if (startsWith(target, L"rect")) return TokenKind::Rect;
	return TokenKind::String;
}

std::vector<Token> tokenize(std::wstring src, bool neglect_identifier) {
	if (src == L"") return {};
	std::vector<Token> result;
	int index = 0;
	while (index < src.size()) {
		std::wstring temp = L"";
		if (src.size() > 0 && src[0] == L' ') break;
		switch (getCharType(src[index])) {
		case (CharType::WhiteSpace): {
			index++;
			break;
		}
		case (CharType::NumberLiteral): {
			while (getCharType(src[index]) == CharType::NumberLiteral || src[index] == L'.') { temp += src[index]; index++; }
			if (temp.find(L'.') != std::wstring::npos)
				result.push_back({ TokenKind::Float, temp });
			else
				result.push_back({ TokenKind::Int, temp });
			break;
		}
		case (CharType::StringLiteral): {
			index++;
			while (!(src[index] == L'\"' && src[index - 1] != L'\\')) {
				temp += src[index]; index++;
			}
			index++;
			temp = replaceAll(temp, L"\\\"", L"\"");
			result.push_back({ TokenKind::String, temp });
			break;
		}
		case (CharType::IdentifierAndKeyword): {
			std::wstring rest = erasedFront(src, index);
			if (tokenStartsWith(rest, L"and")) { result.push_back({ TokenKind::And, L"and" }); index += 3; }
			else if (tokenStartsWith(rest, L"or")) { result.push_back({ TokenKind::Or, L"or" }); index += 2; }
			else if (tokenStartsWith(rest, L"not")) { result.push_back({ TokenKind::Not, L"not" }); index += 3; }
			else if (tokenStartsWith(rest, L"true")) { result.push_back({ TokenKind::True, L"true" }); index += 4; }
			else if (tokenStartsWith(rest, L"false")) { result.push_back({ TokenKind::False, L"false" }); index += 5; }
			else if (tokenStartsWith(rest, L"while")) { result.push_back({ TokenKind::While, L"while" }); index += 5; }
			else if (tokenStartsWith(rest, L"break")) { result.push_back({ TokenKind::Break, L"break" }); index += 5; }
			else if (tokenStartsWith(rest, L"for")) { result.push_back({ TokenKind::For, L"for" }); index += 3; }
			else if (tokenStartsWith(rest, L"new")) { result.push_back({ TokenKind::New, L"new" }); index += 3; }
			else if (tokenStartsWith(rest, L"def")) { result.push_back({ TokenKind::Define, L"def" }); index += 3; }
			else if (tokenStartsWith(rest, L"print")) { result.push_back({ TokenKind::Print, L"print" }); index += 5; }
			else if (tokenStartsWith(rest, L"show")) { result.push_back({ TokenKind::Show, L"show" }); index += 4; }
			else if (tokenStartsWith(rest, L"hide")) { result.push_back({ TokenKind::Hide, L"hide" }); index += 4; }
			else if (tokenStartsWith(rest, L"await")) { result.push_back({ TokenKind::Await, L"await" }); index += 5; }
			else if (tokenStartsWith(rest, L"input")) { result.push_back({ TokenKind::Input, L"input" }); index += 5; }
			else if (tokenStartsWith(rest, L"quit")) { result.push_back({ TokenKind::Quit, L"quit" }); index += 4; }
			else if (tokenStartsWith(rest, L"command")) { result.push_back({ TokenKind::Command, L"command" }); index += 7; }
			else if (tokenStartsWith(rest, L"thread")) { result.push_back({ TokenKind::Thread, L"thread" }); index += 6; }
			else if (tokenStartsWith(rest, L"tween")) { result.push_back({ TokenKind::Tween, L"tween" }); index += 5; }
			else if (tokenStartsWith(rest, L"audio")) { result.push_back({ TokenKind::Audio, L"audio" }); index += 5; }
			else if (tokenStartsWith(rest, L"save")) { result.push_back({ TokenKind::Save, L"save" }); index += 4; }
			else if (tokenStartsWith(rest, L"save_start")) { result.push_back({ TokenKind::SaveStart, L"save_start" }); index += 10; }
			else if (tokenStartsWith(rest, L"load")) { result.push_back({ TokenKind::Load, L"load" }); index += 4; }
			else if (tokenStartsWith(rest, L"persistent")) { result.push_back({ TokenKind::Persistent, L"persistent" }); index += 10; }

			else if (tokenStartsWith(rest, L"label")) { result.push_back({ TokenKind::Label, L"label" }); index += 5; }
			else if (tokenStartsWith(rest, L"include")) { result.push_back({ TokenKind::Include, L"include" }); index += 7; }
			else if (tokenStartsWith(rest, L"choice")) { result.push_back({ TokenKind::Choice, L"choice" }); index += 6; }
			else if (tokenStartsWith(rest, L"case")) { result.push_back({ TokenKind::Case, L"case" }); index += 4; }
			else if (tokenStartsWith(rest, L"jump")) { result.push_back({ TokenKind::Jump, L"jump" }); index += 4; }
			else if (tokenStartsWith(rest, L"jump_main")) { result.push_back({ TokenKind::JumpMain, L"jump_main" }); index += 9; }
			else if (tokenStartsWith(rest, L"call")) { result.push_back({ TokenKind::Call, L"call" }); index += 4; }
			else if (tokenStartsWith(rest, L"call_main")) { result.push_back({ TokenKind::CallMain, L"call_main" }); index += 9; }
			else if (tokenStartsWith(rest, L"return")) { result.push_back({ TokenKind::Return, L"return" }); index += 6; }
			else if (tokenStartsWith(rest, L"scene")) { result.push_back({ TokenKind::Scene, L"scene" }); index += 5; }
			else if (tokenStartsWith(rest, L"char")) { result.push_back({ TokenKind::CharDef, L"char" }); index += 4; }
			else if (tokenStartsWith(rest, L"array")) { result.push_back({ TokenKind::ArrDef, L"array" }); index += 5; }
			else if (tokenStartsWith(rest, L"int")) { result.push_back({ TokenKind::IntDef, L"int" }); index += 3; }
			else if (tokenStartsWith(rest, L"float")) { result.push_back({ TokenKind::FloatDef, L"float" }); index += 5; }
			else if (tokenStartsWith(rest, L"string")) { result.push_back({ TokenKind::StringDef, L"string" }); index += 6; }
			else if (tokenStartsWith(rest, L"bool")) { result.push_back({ TokenKind::BoolDef, L"bool" }); index += 4; }
			else if (tokenStartsWith(rest, L"vector2")) { result.push_back({ TokenKind::Vector2Def, L"vector2" }); index += 7; }
			else if (tokenStartsWith(rest, L"vector4")) { result.push_back({ TokenKind::Vector4Def, L"vector4" }); index += 7; }
			else if (tokenStartsWith(rest, L"if")) { result.push_back({ TokenKind::If, L"if" }); index += 2; }
			else if (tokenStartsWith(rest, L"elif")) { result.push_back({ TokenKind::Elif, L"elif" }); index += 4; }
			else if (tokenStartsWith(rest, L"else")) { result.push_back({ TokenKind::Else, L"else" }); index += 4; }
			else if (startsWith(rest, L"vec2")) {
				std::wstring vec2Source = L"";
				int tempIndex = 0;
				while (rest[tempIndex] != L')') {
					vec2Source += rest[tempIndex];
					tempIndex++;
				}
				vec2Source += ')';
				result.push_back({ TokenKind::Vector2, vec2toString(toVec2(vec2Source)) });
				index += vec2Source.size();
			}
			else if (startsWith(rest, L"vec4")) {
				std::wstring vec4Source = L"";
				int tempIndex = 0;
				while (rest[tempIndex] != L')') {
					vec4Source += rest[tempIndex];
					tempIndex++;
				}
				vec4Source += ')';
				result.push_back({ TokenKind::Vector4, vec4toString(toVec4(vec4Source)) });
				index += vec4Source.size();
			}
			else if (startsWith(rest, L"rect")) {
				std::wstring rectSource = L"";
				int tempIndex = 0;
				while (rest[tempIndex] != L')') {
					rectSource += rest[tempIndex];
					tempIndex++;
				}
				rectSource += ')';
				result.push_back({ TokenKind::Vector4, recttoString(toRect(rectSource)) });
				index += rectSource.size();
			}
			else if (startsWith(rest, L"remove") ||
				     startsWith(rest, L"empty")  ||
					 startsWith(rest, L"len")  ||
					 startsWith(rest, L"insert")  ||
					 startsWith(rest, L"append")  ||
					 startsWith(rest, L"count")  ||
				     startsWith(rest, L"sort")  ||
				     startsWith(rest, L"delete")
				) {
				std::vector<std::pair<std::wstring, TokenKind>> tempArr =
				{
					{L"remove", TokenKind::Remove},
					{L"empty", TokenKind::Empty},
					{L"len", TokenKind::Length},
					{L"insert", TokenKind::Insert},
					{L"append", TokenKind::Append},
					{L"count", TokenKind::Count},
					{L"sort", TokenKind::Sort},
					{L"delet", TokenKind::Delete}
				};
				for (auto& p : tempArr) {
					if (startsWith(rest, p.first) == true) {
						result.push_back({ p.second, p.first });
						index += std::wstring(p.first).size();
						break;
					}
				}
				while (src[index] == L' ') {
					index++;
				}
				if (src[index] != L'(') {
					std::wcout << src << "\n";
					std::wcout << src[index] << "\n";
					assert(false);
				}
				index++;
				int startIndex = index;
				int endIndex = 0;
				std::wstring parameterSource = L"";
				std::stack<wchar_t> parentStack;
				parentStack.push(L'(');
				while (index < src.size() && parentStack.empty() == false) {
					if (src[index] == L'(') {
						parentStack.push(L'(');
					}
					else if (src[index] == L')') {
						parentStack.pop();
					}
					index++;
				}
				endIndex = index;
				for (int i = startIndex; i < endIndex - 1; i++) {
					parameterSource += src[i];
				}
				result.push_back({ TokenKind::Parameter, L"(" + parameterSource + L")"});
			}
			else {
				std::wstring identifierSource = L"";
				while (getCharType(src[index]) == CharType::IdentifierAndKeyword) { 
					identifierSource += src[index]; 
					index++; 
				}
				if (Sgt::instance().charExists(identifierSource) == true && neglect_identifier == false) {
					std::wstring name = Sgt::instance().charMap[identifierSource].full_name;
					vec4 color = Sgt::instance().charMap[identifierSource].color;
					std::wstring ans = L"\"" + name + L"\"" + vec4toString(color);
					result.push_back({ TokenKind::Char, ans });
				}
				else if (Sgt::instance().varExists(identifierSource) == true && neglect_identifier == false) {
					std::wstring identifierValue = Sgt::instance().varMap[identifierSource];
					if (getVariantType(identifierSource) == TokenKind::Int) { result.push_back({ TokenKind::Int, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::Float) { result.push_back({ TokenKind::Float, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::True) { result.push_back({ TokenKind::True, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::False) { result.push_back({ TokenKind::False, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::Vector2) { result.push_back({ TokenKind::Vector2, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::Vector4) { result.push_back({ TokenKind::Vector4, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::Rect) { result.push_back({ TokenKind::Rect, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::Array) { result.push_back({ TokenKind::Array, identifierValue }); }
					else if (getVariantType(identifierSource) == TokenKind::String) { result.push_back({ TokenKind::String, identifierValue }); }
				}
				else {
					result.push_back({ TokenKind::Identifier, identifierSource });
				}
			}
			break;
		}
		case (CharType::OperatorAndPunctuator): {
			std::wstring rest = erasedFront(src, index);
			if (src[index] == L'+') { result.push_back({ TokenKind::Add, L"+" }); index++; }
			else if (src[index] == L'-') {
				if (result.empty() || (result[result.size() - 1].type != TokenKind::Int
					&& result[result.size() - 1].type != TokenKind::Float
					&& result[result.size() - 1].type != TokenKind::RightParent)) {
					result.push_back({ TokenKind::Negative, L"-" });
					index++;
				}
				else {
					result.push_back({ TokenKind::Sub, L"-" });
					index++;
				}
			}
			else if (src[index] == L'#') { return result; }
			else if (src[index] == L'*') { result.push_back({ TokenKind::Mul, L"*" }); index++; }
			else if (src[index] == L'/') { result.push_back({ TokenKind::Div, L"/" }); index++; }
			else if (src[index] == L'(') { result.push_back({ TokenKind::LeftParent, L"(" }); index++; }
			else if (src[index] == L')') { result.push_back({ TokenKind::RightParent, L")" }); index++; }
			else if (index + 1 < src.size() && src[index] == L'=' && src[index + 1] == L'=') { result.push_back({ TokenKind::IsEqual, L"==" }); index += 2; }
			else if (index + 1 < src.size() && src[index] == L'!' && src[index + 1] == L'=') { result.push_back({ TokenKind::IsNotEqual, L"!=" }); index += 2; }
			else if (index + 1 < src.size() && src[index] == L'>' && src[index + 1] == L'=') { result.push_back({ TokenKind::IsGreatEqual, L">=" }); index += 2; }
			else if (index + 1 < src.size() && src[index] == L'<' && src[index + 1] == L'=') { result.push_back({ TokenKind::IsLessEqual, L"<=" }); index += 2; }
			else if (src[index] == L'>') { result.push_back({ TokenKind::IsGreat, L">" }); index++; }
			else if (src[index] == L'<') { result.push_back({ TokenKind::IsLess, L"<" }); index++; }
			else if (src[index] == L'=') { result.push_back({ TokenKind::Assign, L"=" }); index++; }
			else if (src[index] == L',') { result.push_back({ TokenKind::Comma, L"," }); index++; }
			else if (src[index] == L'[') {
				std::wstring srcWithoutBrackets = L"[";
				index++;
				while (src[index] != L']') {
					srcWithoutBrackets += src[index];
					index++;
				}
				srcWithoutBrackets += L"]";
				index++;
				result.push_back({ TokenKind::Array, srcWithoutBrackets });
			}
			else {
				std::wcout << src << "\n";
				std::wcout << L"at : " << index << "\n";
				assert(false);
			}
			break;
		}
		}
	}
	return result;
}



Token eval(std::wstring src, bool neglect_identifier){
	if (src.empty()) return { TokenKind::Unknown, L"" };
	std::vector<Token> tokens = tokenize(src, neglect_identifier);
	std::vector<Token> postfix;
	std::stack<Token> stack;
	for (int n = 0; n < tokens.size(); n++) {
		if (tokens[n].type == TokenKind::Int || tokens[n].type == TokenKind::Float || tokens[n].type == TokenKind::String || tokens[n].type == TokenKind::True || tokens[n].type == TokenKind::False || tokens[n].type == TokenKind::Vector2 || tokens[n].type == TokenKind::Vector4 || tokens[n].type == TokenKind::Array
			|| tokens[n].type == TokenKind::Parameter
			) {
			postfix.push_back(tokens[n]);
		}
		else if (tokens[n].type == TokenKind::LeftParent) {
			stack.push(tokens[n]);
		}
		else if (tokens[n].type == TokenKind::RightParent) {
			while (stack.top().type != TokenKind::LeftParent) {
				postfix.push_back(stack.top()); stack.pop();
			}
			stack.pop();
		}
		else {
			while (stack.empty() == false && evalPriority[tokens[n].type] <= evalPriority[stack.top().type]) {
				postfix.push_back(stack.top()); stack.pop();
			}
			stack.push(tokens[n]);
		}
	}
	while (stack.empty() == false) {
		postfix.push_back(stack.top()); stack.pop();
	}
	Token target1, target2;
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i].type == TokenKind::Int || postfix[i].type == TokenKind::Float || postfix[i].type == TokenKind::String || postfix[i].type == TokenKind::True || postfix[i].type == TokenKind::False || postfix[i].type == TokenKind::Vector2 || postfix[i].type == TokenKind::Vector4 || postfix[i].type == TokenKind::Array
			|| postfix[i].type == TokenKind::Parameter
			) {
			stack.push(postfix[i]);
		}
		else if (postfix[i].type == TokenKind::Negative) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float) { stack.push({ TokenKind::Float, L"-" + target1.raw_data }); }
			else if (target1.type == TokenKind::Int) { stack.push({ TokenKind::Int, L"-" + target1.raw_data }); }
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Add) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) + stof(target2.raw_data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.raw_data) + stoi(target2.raw_data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) + stof(target2.raw_data)) });
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String)
				stack.push({ TokenKind::String, target1.raw_data + target2.raw_data });
			else if (target1.type == TokenKind::String && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::String, target1.raw_data + target2.raw_data });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && target2.type == TokenKind::String)
				stack.push({ TokenKind::String, target1.raw_data + target2.raw_data });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Sub) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) - stof(target2.raw_data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.raw_data) - stoi(target2.raw_data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) - stof(target2.raw_data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Mul) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) * stof(target2.raw_data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.raw_data) * stoi(target2.raw_data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) * stof(target2.raw_data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Div) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::Float && target2.type == TokenKind::Float)
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) / stof(target2.raw_data)) });
			else if (target1.type == TokenKind::Int && target2.type == TokenKind::Int)
				stack.push({ TokenKind::Int, std::to_wstring(stoi(target1.raw_data) / stoi(target2.raw_data)) });
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float))
				stack.push({ TokenKind::Float, std::to_wstring(stof(target1.raw_data) / stof(target2.raw_data)) });
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) == stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String) {
				if (target1.raw_data == target2.raw_data)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == target2.type)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::True || target1.type == TokenKind::False) &&
				     (target2.type == TokenKind::Int || target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::Int && target2.type == TokenKind::True) {
					if (stoi(target1.raw_data) != 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::True && target2.type == TokenKind::Int) {
					if (stoi(target2.raw_data) != 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::Int && target2.type == TokenKind::False) {
					if (stoi(target1.raw_data) == 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::False && target2.type == TokenKind::Int) {
					if (stoi(target2.raw_data) == 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
			}
			else {
				std::wcout << target1.raw_data << " " << target2.raw_data << "\n";
				assert(false);			}
		}
		else if (postfix[i].type == TokenKind::IsNotEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) != stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if (target1.type == TokenKind::String && target2.type == TokenKind::String) {
				if (target1.raw_data != target2.raw_data)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type != target2.type)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else if ((target1.type == TokenKind::Int || target1.type == TokenKind::True || target1.type == TokenKind::False) &&
				(target2.type == TokenKind::Int || target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::Int && target2.type == TokenKind::True) {
					if (stoi(target1.raw_data) == 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::True && target2.type == TokenKind::Int) {
					if (stoi(target2.raw_data) == 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::Int && target2.type == TokenKind::False) {
					if (stoi(target1.raw_data) != 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
				if (target1.type == TokenKind::False && target2.type == TokenKind::Int) {
					if (stoi(target2.raw_data) != 0)
						stack.push({ TokenKind::True, L"true" });
					else
						stack.push({ TokenKind::False, L"false" });
				}
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsGreat) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) > stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsLess) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) < stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else {
				target1.print();
				target2.print();
				assert(false);
			}
		}
		else if (postfix[i].type == TokenKind::IsGreatEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) >= stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::IsLessEqual) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::Int || target1.type == TokenKind::Float) && (target2.type == TokenKind::Int || target2.type == TokenKind::Float)) {
				if (stof(target1.raw_data) <= stof(target2.raw_data))
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Or) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::True || target2.type == TokenKind::True)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::And) {
			target2 = stack.top(); stack.pop();
			target1 = stack.top(); stack.pop();
			if ((target1.type == TokenKind::True || target1.type == TokenKind::False) && (target2.type == TokenKind::True || target2.type == TokenKind::False)) {
				if (target1.type == TokenKind::True && target2.type == TokenKind::True)
					stack.push({ TokenKind::True, L"true" });
				else
					stack.push({ TokenKind::False, L"false" });
			}
			else assert(false);
		}
		else if (postfix[i].type == TokenKind::Not) {
			target1 = stack.top(); stack.pop();
			if (target1.type == TokenKind::True) stack.push({ TokenKind::False, L"false" });
			else if (target1.type == TokenKind::False) stack.push({ TokenKind::True, L"true" });
			else if (target1.type == TokenKind::Int) {
				if (toInt(target1.raw_data) == 0) {
					stack.push({ TokenKind::True, L"true" });
				}
				else {
					stack.push({ TokenKind::False, L"false" });
				}
			}
			else {
				print(postfix);
				target1.print();
				assert(false);
			}
		}
		else if (postfix[i].type == TokenKind::Function) {
			target1 = stack.top(); stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Int || params[0].type == TokenKind::Float)) assert(false);
			if (!(params[1].type == TokenKind::Int || params[1].type == TokenKind::Float)) assert(false);
			stack.push({ TokenKind::Float, std::to_wstring(pow(stod(params[0].raw_data), stod(params[1].raw_data))) });
		}
		else if (postfix[i].type == TokenKind::Remove) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			for (int i = 0; i < arr.size(); i++) {
				if (arr[i] == params[1]) {
					arr.erase(arr.begin() + i);
					break;
				}
			}
			stack.push({ TokenKind::Array, tokenstoArray(arr) });
		}
		else if (postfix[i].type == TokenKind::Delete) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			if (!(params[1].type == TokenKind::Int)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			arr.erase(arr.begin() + toInt(params[1].raw_data));
			stack.push({ TokenKind::Array, tokenstoArray(arr) });
		}
		else if (postfix[i].type == TokenKind::Empty) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 1) assert(false);
			if (!(params[0].type == TokenKind::Array)) {
				std::wcout << params[0].raw_data << "\n";
				assert(false);
			}
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			if (arr.empty() == true) {
				stack.push({ TokenKind::True, L"true"});
			}
			else {
				stack.push({ TokenKind::False, L"false"});
			}
		}
		else if (postfix[i].type == TokenKind::Length) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 1) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			stack.push({ TokenKind::Int, std::to_wstring(arr.size())});
		}
		else if (postfix[i].type == TokenKind::Insert) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 3) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			if (!(params[1].type == TokenKind::Int)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			arr.insert(arr.begin() + toInt(params[1].raw_data), params[2]);
			stack.push({ TokenKind::Array, tokenstoArray(arr) });
		}
		else if (postfix[i].type == TokenKind::Append) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			arr.push_back(params[1]);
			stack.push({ TokenKind::Array, tokenstoArray(arr) });
		}
		else if (postfix[i].type == TokenKind::Count) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 2) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			int cnt = 0;
			for (auto& p : arr) {
				if (p == params[1]) cnt++;
			}
			stack.push({ TokenKind::Int, std::to_wstring(cnt)});
		}
		else if (postfix[i].type == TokenKind::Sort) {
			target1 = stack.top();
			stack.pop();
			if (target1.type != TokenKind::Parameter) assert(false);
			std::vector<Token> params = getParameter(target1.raw_data);
			if (params.size() != 1) assert(false);
			if (!(params[0].type == TokenKind::Array)) assert(false);
			std::vector<Token> arr = arraytoTokens(params[0].raw_data);
			if (arr.size() > 0) {
				if (arr[0].type == TokenKind::Int) {
					std::sort(arr.begin(), arr.end(), [](const Token& t1, const Token& t2) {return toInt(t1.raw_data) < toInt(t2.raw_data); });
				}
				else if (arr[0].type == TokenKind::String) {
					std::sort(arr.begin(), arr.end(), [](const Token& t1, const Token& t2) {return t1.raw_data < t2.raw_data; });
				}
			}
			stack.push({ TokenKind::Int, tokenstoArray(arr)});
		}
		else {
			postfix[i].print();
			std::wcout << src << "\n";
			alert(L"error at : parser.cpp 1");
		}
	}
	if (stack.empty() == false) {
		return stack.top();
	}
	else {
		std::wcout << src << "\n";
		alert(L"error at : parser.cpp 2");
		assert(false);
	}
}
