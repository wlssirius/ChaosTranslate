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
#ifndef CHARACTER_RECOGNIZE_HEADER
#define CHARACTER_RECOGNIZE_HEADER

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h> // tesseract main header
#include <qstring.h>
#include <QColor>

QString ocr(PIX* image, QString language);
PIX* threshold(PIX* sourceImage, BOX* box);
void replace(PIX* targetImage, PIX* sourceImage, BOX* box);
PIX* otsu(PIX* originalImage);
void thresholdByFontColor(PIX* pix, QColor color);

#endif