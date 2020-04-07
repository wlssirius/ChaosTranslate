#include "GlossaryDialog.h"
#include "qonlinetranslator.h"
#include <map>

class GlossaryManager: public QObject
{
	Q_OBJECT
public:
	GlossaryManager();
	void showDialog();
	QString encode(QString text, std::map<QString, QString>& dict);
	QString decode(QString text, const std::map<QString, QString>& dict);
private:
	using dict = std::map<QString, QString>;
	using languagePair = std::pair<QOnlineTranslator::Language, QOnlineTranslator::Language>;
	std::map<languagePair, dict> m_dictionaries;
	GlossaryDialog* m_dialog = nullptr;
	std::vector<QString> m_codes;
};