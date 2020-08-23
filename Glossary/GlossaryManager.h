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

#ifndef GLOSSARY_MANAGER_HEADER
#define GLOSSARY_MANAGER_HEADER

#include "GlossaryModel.h"
#include "GlossaryDialog.h"
#include "qonlinetranslator.h"
#include <map>

class ChaosTranslate;
class GlossaryManager: public QObject
{
	Q_OBJECT

	enum ERROR_CODE
	{
		EMPTY_DICTIONARY = 0,
		INVALID_DICTIONARY,
		ERROR_COUNT
	};

	QString ERROR_MESSAGE[ERROR_CODE::ERROR_COUNT] = {
		tr("Dictionary is empty!"), //EMPTY_DICTIONARY
		tr("Invalid dictionary file!") //INVALID_DICTIONARY
	};

public:
	using Dict = std::map<QString, QString>;
	using LanguagePair = std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language>;
	struct EncodeResult
	{
		QString encodedText;
		std::map<QString, QString> dictionary;
		EncodeResult(QString text, std::map<QString, QString> dict) :
			encodedText(text), dictionary(dict) {}
	};
	GlossaryManager();
	GlossaryManager(LanguagePair languages);
	~GlossaryManager();
	void showDialog();
	void closeDialog();
	EncodeResult encode(QString text, LanguagePair language);
	QString decode(QString text, const Dict& dict);
	void setSourceLanguage(QOnlineTranslator::Language lan);
	void setTargetLanguage(QOnlineTranslator::Language lan);
private:
	void saveDictionary();
	void loadDictionary();
	void addEntry(QString key, QString value);
	void deleteEntry(QModelIndex idx);
	Dict getCurrentDict();
	void resumeDict(const Dict& dict);

	QOnlineTranslator::Language m_sourceLanguage;
	QOnlineTranslator::Language m_targetLanguage;
	std::map<LanguagePair, Dict> m_dictionaries;
	GlossaryDialog* m_dialog = nullptr;
	GlossaryModel* m_model = nullptr;
	std::vector<QString> m_codes;
	static constexpr char* m_dictionaryString = "Dictionary";
	static constexpr char* m_entryString = "Entry";
	static constexpr char* m_sourceLanString = "SourceLanguage";
	static constexpr char* m_targetLanString = "TargetLanguage";
	static constexpr char* m_keyString = "EntryKey";
	static constexpr char* m_valueString = "EntryValue";
};

#endif