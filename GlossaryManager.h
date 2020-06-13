#include "GlossaryDialog.h"
#include "qonlinetranslator.h"
#include <map>

class RealTimeTranslator;
class GlossaryManager: public QObject
{
	Q_OBJECT
private:
	using Dict = std::map<QString, QString>;
	using LanguagePair = std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language>;
	struct EncodeResult
	{
		QString encodedText;
		std::map<QString, QString> dictionary;
		EncodeResult(QString text, std::map<QString, QString> dict) :
			encodedText(text), dictionary(dict) {}
	};
public:
	GlossaryManager();
	~GlossaryManager();
	void showDialog();
	EncodeResult encode(QString text, LanguagePair language);
	QString decode(QString text, const Dict& dict);
private:
	std::map<LanguagePair, Dict> m_dictionaries;
	GlossaryDialog* m_dialog = nullptr;
	std::vector<QString> m_codes;
	friend class RealTimeTranslator;
};