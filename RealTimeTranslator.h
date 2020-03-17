#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RealTimeTranslator.h"
#include "qpushbutton.h"
#include "qtextedit.h"

class RealTimeTranslator : public QMainWindow
{
	Q_OBJECT

public:
	RealTimeTranslator(QWidget *parent = Q_NULLPTR);

	void translate(bool clicked);

private:
	Ui::RealTimeTranslatorClass ui;
	QTextEdit* m_originalTextEdit;
};
