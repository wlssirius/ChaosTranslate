#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RealTimeTranslator.h"

class RealTimeTranslator : public QMainWindow
{
	Q_OBJECT

public:
	RealTimeTranslator(QWidget *parent = Q_NULLPTR);

private:
	Ui::RealTimeTranslatorClass ui;
};
