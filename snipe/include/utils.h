#pragma once
#ifndef UTILS_H
#define UTILS_H

#define _CRT_SECURE_NO_WARNINGS

#include <algorithm>
#include <Windows.h>
#include <string>
#include <locale>
#include <map>
#include <vector>
#include <ostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <fstream>

#include <stb_image/stb_image_write.h>

#include "draw.h"
#include "math_utils.h"
#include "singleton.h"

namespace utils {
    static int WINDOW_WIDTH, WINDOW_HEIGHT;
    static int WINDOW_WIDTH_ORIGIN, WINDOW_HEIGHT_ORIGIN;
    static std::string WINDOW_TITLE;
}

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

inline float clamp(float a, float low, float high) { return max(low, min(a, high)); }
inline float lerp(float from, float to, float weight) { return from * (1 - weight) + to * weight; }


static double getRand(double min, double max) {
    static const double fraction = 1.0 / (RAND_MAX + 1.0);
    return min + ((max - min + 1) * (std::rand() * fraction));
}

static void alert(std::wstring msg) {
    MessageBox(NULL, msg.c_str(), L"Error", MB_ICONERROR | MB_OK);
}

static std::wstring s2ws(std::string s) {
    return std::wstring(s.begin(), s.end());
}

static std::string ws2s(std::wstring s) {
    return std::string(s.begin(), s.end());
}

static void writeFile(std::string filePath, std::string content) {
    std::ofstream writeFile(filePath.data());
    if (writeFile.is_open()) {
        writeFile << content;
        writeFile.close();
    }
}

static void writeFile(std::string filePath, std::wstring content) {
    std::wofstream writeFile(filePath);
    if (writeFile.is_open()) {
        writeFile << content;
        writeFile.close();
    }
}

static std::string readFile(std::string filePath) {
    std::ifstream openFile(filePath.data());
    std::string content = "";
    if (openFile.is_open()) {
        std::string line;
        while (getline(openFile, line)) {
            content += line + "\n";
        }
        openFile.close();
    }
    return content;
}

inline std::wstring erasedFront(std::wstring str, int index) {
    if (index >= str.size()) return L"";
    while (index--) {
        str.erase(str.begin());
    }
    return str;
}

inline std::vector<std::wstring> split(std::wstring input, wchar_t delimiter) {
    std::vector<std::wstring> answer;
    std::wstringstream ss(input);
    std::wstring temp;
    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }
    return answer;
}

static std::wstring eraseBeginEnd(std::wstring s) {
    s.erase(s.begin());
    s.erase(s.end()-1);
    return s;
}

static std::wstring eraseLineBreak(std::wstring str) {
    std::wstring::iterator iter = str.begin();
    while (iter != str.end()) {
        if (*iter == L'\n') {
            iter = str.erase(iter);
        }
        else {
            iter++;
        }
    }
    return str;
}

static std::vector<std::wstring> eraseLineBreak(std::vector<std::wstring> arr) {
    std::vector<std::wstring> result;
    for (auto& p : arr) {
        if (p != L"") {
            result.push_back(p);
        }
    }
    return result;
}

static std::wstring eraseWhiteSpace(std::wstring str) {
    std::wstring::iterator iter = str.begin();
    while (iter != str.end()) {
        if (*iter == L' ') {
            iter = str.erase(iter);
        }
        else {
            iter++;
        }
    }
    return str;
}

static std::wstring eraseQuote(std::wstring str) {
    str.erase(str.begin());
    str.erase(str.end() - 1);
    return str;
}

static size_t GetSizeOfFile(const std::wstring& path) {
    struct _stat fileinfo;
    _wstat(path.c_str(), &fileinfo);
    return fileinfo.st_size;
}


static std::wstring readFileW(const std::wstring& filename) {
    std::wstring buffer;
    FILE* f;
    _wfopen_s(&f, filename.c_str(), L"rtS, ccs=UTF-8");
    if (f == NULL) {
        return buffer;
    }
    size_t filesize = GetSizeOfFile(filename);
    if (filesize > 0) {
        buffer.resize(filesize);
        size_t wchars_read = fread(&(buffer.front()), sizeof(wchar_t), filesize, f);
        buffer.resize(wchars_read);
        buffer.shrink_to_fit();
    }
    fclose(f);
    return buffer;
}


static std::vector<std::wstring> getFilenamesInFolder(std::wstring folder) {
    std::vector<std::wstring> names;
    std::wstring search_path = folder + L"/*.*";
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                names.push_back(fd.cFileName);
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
    return names;
}

static std::string getCurrentPath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

static inline bool fileExists(const std::string name) {
    std::ifstream f(name.c_str());
    return f.good();
}

static inline std::string getParentFolderPath(const std::string path) {
    return path.substr(0, path.find_last_of("/\\"));
}

static inline bool endsWith(std::string const& value, std::string const& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static inline bool endsWith(std::wstring const& value, std::wstring const& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static inline bool startsWith(std::string str, std::string prefix) {
    if (prefix.size() > str.size()) return false;
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

static inline bool startsWith(std::wstring str, std::wstring prefix) {
    if (prefix.size() > str.size()) return false;
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

static inline bool tokenStartsWith(std::wstring str, std::wstring prefix) {
    if (prefix.size() > str.size()) return false;
    bool _a = str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    bool _b = true;
    if (str.size() > prefix.size()) {
        wchar_t c = str[prefix.size()];
        if (L'a' <= c && c <= L'z' || L'A' <= c && c <= L'Z' || c == '_') {
            _b = false;
        }
    }
    return _a && _b;
}

static inline std::wstring get1stElement(std::wstring src, wchar_t del) {
    std::wstring result = L"";
    int index = 0;
    while (src[index] == del) {
        index++;
    }
    while (src[index] != del && src[index] != L'\0') {
        result += src[index];
        index++;
    }
    return result;
}

static bool find(std::wstring src, wchar_t target) {
    for (int i = 0; i < src.size(); i++) {
        if (src[i] == target)
            return true;
    }
    return false;
}

static std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

static std::wstring replaceAll(std::wstring str, const std::wstring from, const std::wstring to) {
    if (from.empty())
        return L"";
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

static std::wstring replaceVariable(std::wstring str) {
    std::wstring result = L"";
    std::wstring::iterator iter = str.begin();

    while (iter != str.end()) {
        if ((*iter == L'[' && iter != str.begin() && *(iter - 1) != L'\\')) {
            std::wstring varName = L"";
            if (iter != str.end()) {
                iter += 1;
            }
            while (iter != str.end() && *iter != ']' && *(iter - 1) != L'\\') {
                varName += *iter;
                iter++;
            }
            if (Sgt::instance().varExists(varName) == false) {
                alert(L"cannot replace variable :: there's no var called " + varName);
            }
            result += Sgt::instance().varMap[varName];
        }
        else if (*iter == L'[' && iter == str.begin()) {
            std::wstring varName = L"";
            if (iter != str.end()) {
                iter += 1;
            }
            while (iter != str.end() && *iter != ']') {
                varName += *iter;
                iter++;
            }
            if (Sgt::instance().varExists(varName) == false) {
                alert(L"cannot replace variable :: there's no var called " + varName);
            }
            result += Sgt::instance().varMap[varName];
        }
        else {
            result += *iter;
        }
        iter++;
    }
    return replaceAll(result, L"\\[", L"[");
}

static void setConsoleWindowPosition(int x, int y) {
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

static void setConsoleWindowCursorPosition(int x, int y) {
    SetCursorPos(x, y);
}

static std::string multibyte2utf8(const std::string str) {
    int nLen = str.size();
    wchar_t warr[256];
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, warr, 256);
    char carr[256];
    memset(carr, '\0', sizeof(carr));
    WideCharToMultiByte(CP_UTF8, 0, warr, -1, carr, 256, NULL, NULL);
    return carr;
}

static std::string multibyte2utf8(std::wstring str) {
    char carr[256];
    memset(carr, '\0', sizeof(carr));
    WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, carr, 256, NULL, NULL);
    return carr;
}

static std::wstring checkPath(std::wstring path) {
    if (startsWith(path, L"res:\\")) {
        return s2ws(getCurrentPath()) + L"\\" + erasedFront(path, 5);
    }
    return path;
}

static void screenShot(std::string filePath) {
    BYTE* pixels = new BYTE[4 * utils::WINDOW_WIDTH * utils::WINDOW_HEIGHT];
    glReadPixels(0, 0, utils::WINDOW_WIDTH, utils::WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_flip_vertically_on_write(true);
    stbi_write_png(ws2s(checkPath(s2ws(filePath))).c_str(), utils::WINDOW_WIDTH, utils::WINDOW_HEIGHT, 4, pixels, 4 * utils::WINDOW_WIDTH);
    delete[] pixels;
}


#endif