
#ifndef GLOSSARY_MANAGER_HEADER
#define GLOSSARY_MANAGER_HEADER

#include "GlossaryDialog.h"
#include "qonlinetranslator.h"
#include <map>

class RealTimeTranslator;
class GlossaryManager: public QObject
{
	Q_OBJECT
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
	EncodeResult encode(QString text, LanguagePair language);
	QString decode(QString text, const Dict& dict);
	void setSourceLanguage(QOnlineTranslator::Language lan);
	void setTargetLanguage(QOnlineTranslator::Language lan);
private:
	void saveDictionary();
	void loadDictionary();
	void addEntry(QString key, QString value);
	void deleteEntry(QString key, QString value);

	QOnlineTranslator::Language m_sourceLanguage;
	QOnlineTranslator::Language m_targetLanguage;
	std::map<LanguagePair, Dict> m_dictionaries;
	GlossaryDialog* m_dialog = nullptr;
	std::vector<QString> m_codes;
	static constexpr char* m_dictionaryString = "Dictionary";
	static constexpr char* m_entryString = "Entry";
	static constexpr char* m_sourceLanString = "SourceLanguage";
	static constexpr char* m_targetLanString = "TargetLanguage";
	static constexpr char* m_keyString = "EntryKey";
	static constexpr char* m_valueString = "EntryValue";
};

#endif