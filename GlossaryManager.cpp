#include "GlossaryManager.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QFileDialog>
#include <QMessageBox>
#include "submodules\RapidXML\rapidxml.hpp"
#include "submodules\RapidXML\rapidxml_print.hpp"
#include "fstream"
#include "sstream"

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
		connect(m_dialog, &GlossaryDialog::onAddEntry, this, &GlossaryManager::addEntry);
		connect(m_dialog, &GlossaryDialog::onDeleteEntry, this, &GlossaryManager::deleteEntry);
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
	LanguagePair lanPair(m_sourceLanguage, m_targetLanguage);
	if (m_dictionaries.find(lanPair) == m_dictionaries.end())
	{
		QMessageBox msg;
		msg.setText("Dictionary is empty!");
		msg.exec();
		return;
	}
	QString fileName = QFileDialog::getSaveFileName(m_dialog,
		tr("Save Dictionary"), "", tr("XML Files (*.xml)"));

	using namespace rapidxml;
	xml_document<> doc;

	const auto toConstChar = [](QString str)
	{
		QByteArray array = str.toLocal8Bit();
		const char* buffer = array.data();
		return buffer;
	};

	xml_node<>* root = doc.allocate_node(node_element, m_dictionaryString);
	QByteArray sourceLanguage = QVariant::fromValue(m_sourceLanguage).toString().toLocal8Bit();
	QByteArray targetLanguage = QVariant::fromValue(m_targetLanguage).toString().toLocal8Bit();
	root->append_attribute(doc.allocate_attribute(m_sourceLanString, sourceLanguage.data()));
	root->append_attribute(doc.allocate_attribute(m_targetLanString, targetLanguage.data()));
	doc.append_node(root);
	QString original;
	QByteArray key;
	QByteArray value;
	Dict dict = m_dictionaries.at(lanPair);
	std::vector<QByteArray> keys;
	std::vector<QByteArray> values;
	for (auto& entry : dict)
	{
		xml_node<>* child = doc.allocate_node(node_element, m_entryString);
		original = entry.first;
		keys.emplace_back(original.toLocal8Bit());
		values.emplace_back(entry.second.toLocal8Bit());
		child->append_attribute(doc.allocate_attribute(m_keyString, keys[keys.size()-1].data()));
		child->append_attribute(doc.allocate_attribute(m_valueString, values[keys.size() - 1].data()));
		root->append_node(child);
	}
	
	// Convert doc to string if needed
	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	// Save to file
	QByteArray fileNameArray = fileName.toLocal8Bit();
	std::ofstream file_stored(fileNameArray.data());
	file_stored << doc;
	file_stored.close();
	doc.clear();
}

void GlossaryManager::loadDictionary()
{
	QString fileName = QFileDialog::getOpenFileName(m_dialog,
		tr("Load Dictionary"), "", tr("XML Files (*.xml)"));

	using namespace rapidxml;

	xml_document<> doc;
	xml_node<>* rootNode;
	QByteArray fileNameArray = fileName.toLocal8Bit();
	std::ifstream fileSteam(fileNameArray.data());
	std::vector<char> buffer((std::istreambuf_iterator<char>(fileSteam)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');
	try
	{
		doc.parse<0>(&buffer[0]);
		rootNode = doc.first_node(m_dictionaryString);
		if (rootNode == nullptr)
		{
			throw std::exception("root node not found");
		}
		if (!rootNode->first_attribute(m_sourceLanString) || !rootNode->first_attribute(m_targetLanString))
		{
			throw std::exception("language setting not found");
		}
		char* sourceLan = rootNode->first_attribute(m_sourceLanString)->value();
		char* targetLan = rootNode->first_attribute(m_targetLanString)->value();
		QString sourceLanguageStr = QVariant::fromValue(m_sourceLanguage).toString().toLocal8Bit();
		QString targetLanguageStr = QVariant::fromValue(m_targetLanguage).toString().toLocal8Bit();
		if (QString(sourceLan) != sourceLanguageStr || QString(targetLan) != targetLanguageStr)
		{
			throw std::exception("languages don't match");
		}
		for (xml_node<>* entryNode = rootNode->first_node(m_entryString); entryNode; entryNode = entryNode->next_sibling())
		{
			if (!entryNode->first_attribute(m_keyString) || !entryNode->first_attribute(m_valueString))
			{
				throw std::exception("entry not found");
			}
			char* key = entryNode->first_attribute(m_keyString)->value();
			char* value = entryNode->first_attribute(m_valueString)->value();
			QString keyStr(key);
			QString valueStr(value);
			m_dialog->onLoadNewRow(keyStr, valueStr);
		}
	}
	catch (std::exception e)
	{
		QMessageBox msg;
		msg.setText(QString("Invalid dictionary file! ") + QString(e.what()));
		msg.exec();
		return;
	}

}

void GlossaryManager::addEntry(QString key, QString value)
{
	LanguagePair lanPair(m_sourceLanguage, m_targetLanguage);
	m_dictionaries[lanPair][key] = value;
}

void GlossaryManager::deleteEntry(QString key, QString value)
{
	LanguagePair lanPair(m_sourceLanguage, m_targetLanguage);
	if (m_dictionaries.find(lanPair) != m_dictionaries.end())
	{
		m_dictionaries[lanPair].erase(key);
	}
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
