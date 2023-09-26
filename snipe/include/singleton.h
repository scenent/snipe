#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include <unordered_map>
#include "math_utils.h"

class Sgt {
private:
    Sgt() {};
public:
    std::unordered_map<std::wstring, character> charMap{};
    std::unordered_map<std::wstring, std::wstring> varMap{};
    std::unordered_map<std::wstring, std::wstring> labelMap{};
    static Sgt& instance() {
        static Sgt instance;
        return instance;
    }

    bool varExists(std::wstring name) {
        return varMap.find(name) != varMap.end();
    }
    bool charExists(std::wstring name) {
        return charMap.find(name) != charMap.end();
    }
    bool labelExists(std::wstring name) {
        return labelMap.find(name) != labelMap.end();
    }
    void printVarMap() {
        for (std::unordered_map<std::wstring, std::wstring>::const_iterator it = varMap.begin(); it != varMap.end(); ++it) {
            std::wcout << it->first << " " << it->second << "\n";
        }
    }
};



#endif