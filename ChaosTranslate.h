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

#include <QtWidgets/QMainWindow>
#include "ui_ChaosTranslate.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include "qonlinetranslator.h"
#include "qrubberband.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "QRadioButton"
#include "QTranslator"
#include "ApplicationWatcher.h"
#include "SelectionCanvas.h"
#include "GlossaryManager.h"
#include "AboutDialog.h"
#include "QString"
#include "QColorPicker.h"
#include "SettingManager.h"
#include "memory"

class ChaosTranslate : public QMainWindow
{
	Q_OBJECT

	enum ERROR_CODE
	{
		INVALID_APP = 0,
		MINIMIZED_APP,
		ERROR_COUNT
	};

	QString ERROR_MESSAGE[ERROR_CODE::ERROR_COUNT] = {
		tr("Invalid Application. Please select again."), //INVALID_APP
		tr("Failed to capture. Application is minimized."), //MINIMIZED_APP
	};


public:
	ChaosTranslate(QWidget *parent = Q_NULLPTR);
	~ChaosTranslate();

	void selectApp(bool clicked);
	void captureAndTranslate(bool clicked);
	void translate(bool clicked);
	void selectRoi(bool clicked);
	void selectFontColor(bool clicked);
	void setRoi(RECT roi) { m_roi = roi; }

public slots:
	void setSourceLanguage(int idx);
	void setTargetLanguage(int idx);
	void setEntireAppCapture(bool clicked);
	void setRegionCapture(bool clicked);
	void setAutoDetectFontColor(bool clicked);
	void setManualChooseFontColor(bool clicked);

	void onAppSelected();
	void onInvalidApp();

	void onAboutDialog(bool triggered);

	void onMsgBox(QString str);

signals:
	void setOriginalText(const QString& text);
	void setTranslateText(const QString& text);
	void beginTranslate(bool clicked);
	void showMsgBox(QString str);
	void invalidAppSelected();

protected:
	void changeEvent(QEvent* event);
	void closeEvent(QCloseEvent* event) override;

protected slots:
	void slotLanguageChanged(QAction* action);
	void translateAPIChanged(QAction* action);

private:
	const QString m_translatePath = ("Resources/Translation");

	std::shared_ptr<PIX> captureApp();
	void processImg(std::shared_ptr<PIX> pix);
	void ocrTranslate(std::shared_ptr<PIX> pix);
	bool validROI();

	void thresholdByFontColor(PIX* pix);

	void switchTranslator(QTranslator& translator, const QString& filename);
	void loadLanguage(const QString& rLanguage);
	void createLanguageMenu();
	void createEngineMenu();
	void createAboutMenu();
	void createToolbar();
	void createTextEdit();
	std::shared_ptr<QImage> convertPixToQImage(std::shared_ptr<PIX>& pix);

	Ui::ChaosTranslateClass ui;
	QPushButton* m_selectAppButton = nullptr;
	QPushButton* m_captureButton = nullptr;
	QPushButton* m_roiButton = nullptr;
	QColorPicker* m_fontColorButton = nullptr;
	QPushButton* m_translateButton = nullptr;
	QPushButton* m_glossaryButton = nullptr;
	QLabel* m_imageLabel = nullptr;
	QTextEdit* m_originalTextEdit = nullptr;
	QTextEdit* m_translateTextEdit = nullptr;
	QRadioButton* m_entireAppRadioButton = nullptr;
	QRadioButton* m_regionRadioButton = nullptr;
	QRadioButton* m_autoColorRadioButton = nullptr;
	QRadioButton* m_setColorRadioButton = nullptr;
	QComboBox* m_srcLanguageComboBox = nullptr;
	QComboBox* m_tgtLanguageComboBox = nullptr;

	QActionGroup* m_apiActionGroup;
	QAction* m_googleAction;
	QAction* m_bingAction;
	QAction* m_yandexAction;

	AboutDialog* m_aboutDialog = nullptr;

	QOnlineTranslator m_translator;
	QOnlineTranslator::Language m_sourceLanguage = QOnlineTranslator::Language::Japanese;
	QOnlineTranslator::Language m_targetLanguage = QOnlineTranslator::Language::SimplifiedChinese;
	QOnlineTranslator::Engine m_translateEngine = QOnlineTranslator::Engine::Google;

	QTranslator m_uiTranslator;
	QString m_currUILang;

	ApplicationWatcher m_watcher;
	GlossaryManager m_glossary;
	RECT m_roi;
	QColor m_color;
	QString m_appTitle;
	std::shared_ptr<QImage> m_capturedImage;
	SettingManager m_settingManager;

	bool m_regionalCapture = false;
	bool m_manualSetFontColor = false;
};
