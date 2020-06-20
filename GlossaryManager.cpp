#include "GlossaryManager.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QFileDialog>
#include "submodules\RapidXML\rapidxml.hpp"
#include "submodules\RapidXML\rapidxml_print.hpp"
#include "fstream"

GlossaryManager::GlossaryManager():
	GlossaryManager(LanguagePair(QOnlineTranslator::Japanese, QOnlineTranslator::SimplifiedChinese))
{
}

GlossaryManager::GlossaryManager(LanguagePair languages)
{
	m_sourceLanguage = languages.first;
	m_targetLanguage = languages.second;
	m_dialog = nullptr;
	m_codes.emplace_back(u8"💫");
	m_codes.emplace_back(u8"⚓");
	m_codes.emplace_back(u8"⚔");
	m_codes.emplace_back(u8"🌟");
	m_codes.emplace_back(u8"🐉"); 
}

GlossaryManager::~GlossaryManager()
{
}

void GlossaryManager::showDialog()
{
	if (m_dialog == nullptr)
	{
		using Lan = QOnlineTranslator::Language;
		m_dialog = new GlossaryDialog(std::pair<Lan, Lan>(m_sourceLanguage, m_targetLanguage));
		connect(m_dialog, &GlossaryDialog::onSaveDictionary, this, &GlossaryManager::saveDictionary);
		connect(m_dialog, &GlossaryDialog::onLoadDictionary, this, &GlossaryManager::loadDictionary);
	}
	m_dialog->show();
}

GlossaryManager::EncodeResult GlossaryManager::encode(
	QString text, LanguagePair languages)
{
	EncodeResult result(text, std::map<QString, QString>());
	if (m_dictionaries.find(languages) == m_dictionaries.end())
	{
		return result;
	}
	Dict dict = m_dictionaries.at(languages);
	if (dict.size() == 0)
	{
		return result;
	}
	int count = 0;
	for (auto entry : dict)
	{
		const auto& key = entry.first;
		int id = text.indexOf(key);
		while (id != -1)
		{
			text.replace(id, key.size(), m_codes[count]);
			id = text.indexOf(key);
			if (result.dictionary.find(m_codes[count]) == result.dictionary.end())
			{
				result.dictionary[m_codes[count]] = entry.second;
				count++;
			}
		}
	}
	return result;
}

void GlossaryManager::setSourceLanguage(QOnlineTranslator::Language lan)
{
	m_sourceLanguage = lan;
	m_dialog->setSourceLanguage(lan);
}

void GlossaryManager::setTargetLanguage(QOnlineTranslator::Language lan)
{
	m_targetLanguage = lan;
	m_dialog->setTargetLanguage(lan);
}

void GlossaryManager::saveDictionary()
{
	QString fileName = QFileDialog::getSaveFileName(m_dialog,
		tr("Save Dictionary"), "", tr("XML Files (*.xml)"));
	using namespace rapidxml;
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	xml_node<>* root = doc.allocate_node(node_element, "rootnode");
	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	root->append_attribute(doc.allocate_attribute("type", "example"));
	doc.append_node(root);

	xml_node<>* child = doc.allocate_node(node_element, "childnode");
	root->append_node(child);

	// Convert doc to string if needed
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	// Save to file
	std::ofstream file_stored("file_stored.xml");
	file_stored << doc;
	file_stored.close();
	doc.clear();

}

void GlossaryManager::loadDictionary()
{
	QString fileName = QFileDialog::getOpenFileName(m_dialog,
		tr("Load Dictionary"), "", tr("XML Files (*.xml)"));
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
	text.replace("“", "\"");
	text.replace("”", "\"");
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
