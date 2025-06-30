#pragma once
#include "pch.h"
#include <string>
#include <map>
#include "Colors.h"
#include <atlconv.h>
#include "../Base/Path.h"

struct AtomData {
    std::string symbol;     // 예: "H", "C", "O"
    double radius;          // 시각화 반지름
    ColorName color;        // 시각화 색상
};

class MolDBManager {
private:
    std::map<std::string, AtomData> db;

public:
    bool LoadFromIni(const CString& filePath) {
        TCHAR buffer[256];
        TCHAR colorStr[32];

        TCHAR sectionNames[2048] = { 0 };
        GetPrivateProfileSectionNames(sectionNames, 2048, filePath);

        LPCTSTR p = sectionNames;
        while (*p) {
            std::string symbol = std::string(CT2A(p));  // ✅ 핵심 수정

            GetPrivateProfileString(p, _T("Radius"), _T("10.0"), buffer, 256, filePath);
            GetPrivateProfileString(p, _T("Color"), _T("White"), colorStr, 32, filePath);

            AtomData atom;
            atom.symbol = symbol;
            atom.radius = _tstof(buffer);
            atom.color = StringToColorName(std::string(CT2A(colorStr)));  // ✅ 이것도 같이 수정

            db[symbol] = atom;
            p += _tcslen(p) + 1;
        }

        return true;
    }

    const AtomData* GetAtomData(const std::string& symbol) const {
        auto it = db.find(symbol);
        return (it != db.end()) ? &it->second : nullptr;
    }

    const std::map<std::string, AtomData>& GetAll() const { return db; }


    



};

//
// 🎯 전역 인스턴스 선언
//
extern MolDBManager g_molDB;
