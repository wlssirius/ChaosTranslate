#include "ChaosTranslate.h"
#include <QtWidgets/QApplication>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h> // tesseract main header
#include <iostream>
#include <Windows.h>
#include "characterrecognize.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChaosTranslate w;
	w.show();
	return a.exec();
}
