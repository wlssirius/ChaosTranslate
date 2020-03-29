#include "GlossaryManager.h"

void GlossaryManager::showDialog()
{
	if (m_dialog == nullptr)
	{
		m_dialog = new GlossaryDialog();
	}
	m_dialog->show();
}