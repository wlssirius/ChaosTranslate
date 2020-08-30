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

#include "LanguageManager.h"

LanguageManager::LanguageManager()
{
    initTessLanguages();
    generateTessQtMapping();
}


bool LanguageManager::isSupported(std::string language)
{
    return std::find(m_tessLanguages.begin(), m_tessLanguages.end(),language) != m_tessLanguages.end();
}

QString LanguageManager::qtToTess(QOnlineTranslator::Language language) const
{ 
    return languageMapping::qtToTesseract[language]; 
}
QOnlineTranslator::Language LanguageManager::tessToQt(QString language) const
{
    if (m_tessToQtMap.find(language) == m_tessToQtMap.end())
    {
        return {};
    }
    return m_tessToQtMap.at(language); 
}

QOnlineTranslator::Language LanguageManager::getQtLanguage(int idx) const
{
    if (idx<0 || idx>=m_tessLanguages.size())
    {
        return {};
    }
    return tessToQt(QString::fromStdString(m_tessLanguages[idx]));
}

int LanguageManager::getLanguageIdx(QOnlineTranslator::Language language) const
{
    QString qtLanguage = qtToTess(language);
    for (int i = 0; i < m_tessLanguages.size(); i++)
    {
        if (QString::fromStdString(m_tessLanguages[i]) == qtLanguage)
        {
            return i;
        }
    }
    return -1;
}

void LanguageManager::initTessLanguages()
{
    for (auto& p : std::filesystem::directory_iterator(m_tessDataPath))
    {
        auto e = p.path().extension();
        if (p.path().extension() == m_extension)
        {
            m_tessLanguages.push_back(p.path().filename().replace_extension("").string());
        }
    }
}

void LanguageManager::generateTessQtMapping()
{
    for (int idx = 0; idx <= QOnlineTranslator::Language::Zulu; idx++)
    {
        QString tess = languageMapping::qtToTesseract[idx];
        m_tessToQtMap.emplace(tess, QOnlineTranslator::Language(idx));
    }
}


