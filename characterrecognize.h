
#ifndef CHARACTER_RECOGNIZE_HEADER
#define CHARACTER_RECOGNIZE_HEADER

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h> // tesseract main header
#include <qstring.h>

QString ocr(PIX* image, QString language);
PIX* threshold(PIX* sourceImage, BOX* box);
void replace(PIX* targetImage, PIX* sourceImage, BOX* box);
PIX* otsu(PIX* originalImage);

#endif