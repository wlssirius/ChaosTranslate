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

#include "SettingManager.h"
#include <vector>
using namespace rapidxml;
void SettingManager::set(SETTING setting, QString value)
{
	m_settings[m_enumName.at(setting)] = value;
}

QString SettingManager::get(SETTING setting)
{
	if (m_settings.find(m_enumName.at(setting)) != m_settings.end())
	{
		return m_settings[m_enumName.at(setting)];
	}
	return QString{};
}

void SettingManager::saveSetting()
{
	xml_document<> doc;

	xml_node<>* root = doc.allocate_node(node_element, m_root);
	std::vector<QByteArray> keys;
	std::vector<QByteArray> values;
	for (const auto& kvp : m_settings)
	{
		keys.emplace_back(kvp.first.toLocal8Bit());
		values.emplace_back(kvp.second.toLocal8Bit());
		root->append_attribute(doc.allocate_attribute(keys.back().data(), values.back().data()));
	}
	doc.append_node(root);

	// Convert doc to string if needed
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	// Save to file
	std::ofstream file_stored(m_fileName);
	file_stored << doc;
	file_stored.close();
	doc.clear();
}

bool SettingManager::loadSetting()
{
	std::ifstream fileSteam(m_fileName);
	std::vector<char> buffer((std::istreambuf_iterator<char>(fileSteam)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	xml_document<> doc;
	xml_node<>* rootNode;
	try
	{
		doc.parse<0>(&buffer[0]);
		rootNode = doc.first_node(m_root);
		if (rootNode == nullptr)
		{
			throw std::exception("root node not found");
		}
		for (auto kvp : m_enumName)
		{
			QByteArray ba = kvp.second.toLocal8Bit();
			if (!rootNode->first_attribute(ba.data()))
			{
				throw std::exception("setting not found");
			}
			const char* val = rootNode->first_attribute(ba.data())->value();
			m_settings.emplace(kvp.second, QString(val));
		}
	}
	catch (std::exception& e)
	{
		return false;
	}
	return true;
}
