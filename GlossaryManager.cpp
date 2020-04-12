#include "GlossaryManager.h"
#include <QStandardItemModel>
#include <QTableView>

GlossaryManager::GlossaryManager()
{
	m_codes.emplace_back("++++");
	m_codes.emplace_back("----");
	m_codes.emplace_back("====");
	m_codes.emplace_back("&&&&");
	m_codes.emplace_back("****");
}

void GlossaryManager::showDialog()
{
	if (m_dialog == nullptr)
	{
		m_dialog = new GlossaryDialog();
	}
	m_dialog->show();
}

QString GlossaryManager::encode(QString text, std::map<QString, QString>& dict)
{
	if (m_dialog == nullptr)
	{
		return text;
	}
	const auto& glossary = m_dialog->getGlossary();
	if (glossary.size() == 0)
	{
		return text;
	}
	int count = 0;
	for (auto entry : glossary)
	{
		const auto& key = entry.first;
		int id = text.indexOf(key);
		while (id != -1)
		{
			text.replace(id, key.size(), m_codes[count]);
			id = text.indexOf(key);
			if (dict.find(m_codes[count]) == dict.end())
			{
				dict[m_codes[count]] = entry.second;
				count++;
			}
		}
	}
	return text;
}

QString GlossaryManager::decode(QString text, const std::map<QString, QString>& dict)
{
	if (m_dialog == nullptr)
	{
		return text;
	}
	const auto& glossary = m_dialog->getGlossary();
	if (glossary.size() == 0)
	{
		return text;
	}
	text.replace("��", "\"");
	text.replace("��", "\"");
	for (auto kvp : dict)
	{
		const auto& key = kvp.first;
		const auto& value = kvp.second;
		int id = text.indexOf(key);
		while (id != -1)
		{
			text.replace(id, key.size(), value);
			id = text.indexOf(key);
		}
	}
	return text;
}
