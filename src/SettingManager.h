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

#ifndef SETTING_MANAGER_HEADER
#define SETTING_MANAGER_HEADER


#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "fstream"
#include "sstream"
#include <QString>
#include <map>

class SettingManager
{
public:

	enum class SETTING
	{
		SOURCE_LAN = 0,
		TARGET_LAN,
		UI_LAN,
		TRANSLATE_ENGINE,
		SETTING_COUNT
	};
	const std::map<SETTING, QString> m_enumName = {
		{SETTING::SOURCE_LAN, "source_language"},
		{SETTING::TARGET_LAN, "target_language"},
		{SETTING::UI_LAN, "UI_language"},
		{SETTING::TRANSLATE_ENGINE, "Translate_engine"}
	};

	void set(SETTING setting, QString value);
	QString get(SETTING setting);
	void saveSetting();
	bool loadSetting();

private:
	const char* m_fileName = "setting.xml";
	const char* m_root = "Setting";
	std::map<QString, QString> m_settings;
};




#endif