#include "GlossaryDialog.h"
#include "qonlinetranslator.h"
#include <map>

class RealTimeTranslator;
class GlossaryManager: public QObject
{
	Q_OBJECT
private:
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
	EncodeResult encode(QString text, std::pair<QString, QString> language);
	QString decode(QString text, const std::map<QString, QString>& dict);
private:
	using dict = std::map<QString, QString>;
	using languagePair = std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language>;
	std::map<languagePair, dict> m_dictionaries;
	GlossaryDialog* m_dialog = nullptr;
	std::vector<QString> m_codes;
	friend class RealTimeTranslator;
};