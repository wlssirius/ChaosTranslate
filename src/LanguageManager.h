/*
*   Copyright (C) 2019-2020  Wei Lisi (Willis) <weilisi16@gmail.com>
*	This file is part of ChaosTranslate
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "qonlinetranslator.h"
#include "LanguageMapping.h"

class LanguageManager
{
public:
    LanguageManager();
    const std::vector<std::string>& getLanguages() const { return m_tessLanguages; }
    bool isSupported(std::string language);
    QString qtToTess(QOnlineTranslator::Language language)const ;
    QOnlineTranslator::Language tessToQt(QString language)const ;
    QOnlineTranslator::Language getQtLanguage(int idx)const;
    int getLanguageIdx(QOnlineTranslator::Language language)const;
private:
    void initTessLanguages();
    void generateTessQtMapping();
    std::vector<std::string> m_tessLanguages;
    std::unordered_map<QString, QOnlineTranslator::Language> m_tessToQtMap;
    std::string m_tessDataPath = "tessdata_best";
    std::string m_extension = ".traineddata";
};