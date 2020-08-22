
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
		SETTING_COUNT
	};
	const std::map<SETTING, QString> m_enumName = {
		{SETTING::SOURCE_LAN, "source_language"},
		{SETTING::TARGET_LAN, "target_language"},
		{SETTING::UI_LAN, "UI_language"}
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