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

#include "characterrecognize.h"
#include <leptonica/allheaders.h>
#include <iostream>

QString ocr(PIX* pix, QString language)
{
    tesseract::TessBaseAPI tess;
    if (tess.Init("tessdata_best", language.toStdString().c_str()))
    {
        //std::cout << "OCRTesseract: Could not initialize tesseract." << std::endl;
        return 1;
    }
    // setup
    tess.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
    tess.SetVariable("save_best_choices", "T");
    tess.SetVariable("preserve_interword_spaces", "1");

    tess.SetImage(pix);
    if (tess.FindLinesCreateBlockList())
    {
        auto thresholded = tess.GetThresholdedImage();
        //pixWrite("TestFile_Threshold.png", thresholded, IFF_PNG);
        tess.SetImage(thresholded);
    }
    tess.Recognize(0);
    auto text = tess.GetUTF8Text();
    QString result = QString::fromUtf8(text); 
    auto thresholded = tess.GetThresholdedImage();
    
    //pixWrite("TestFile_Threshold.png", thresholded, IFF_PNG);
    

    // cleanup
    tess.Clear();
    
    return result;
}

PIX* threshold(PIX* sourceImage, BOX* roi)
{
    PIX* imgCrop = pixClipRectangle(sourceImage, roi, NULL);
    return imgCrop;
}

void replace(PIX* targetImage, PIX* sourceImage, BOX* box)
{
    l_int32 wpl = pixGetWpl(sourceImage);
    l_int32 targetWpl = pixGetWpl(targetImage);
    l_uint32* data = pixGetData(sourceImage);
    l_uint32* targetData = pixGetData(targetImage);
    for (int i = 0; i < wpl * 4; i++)
    {
        for (int j = 0; j < sourceImage->h; j++)
        {
            l_uint32* line = data + j * wpl;
            l_uint32* targetLine = targetData + (box->y+j) * targetWpl;
            l_uint32 value = GET_DATA_BYTE(line, i);
             SET_DATA_BYTE(targetLine, box->x+i, value);
        }
    }
}

PIX* otsu(PIX* sourceImage)
{
    l_float32  scorefract;
    PIX* pixb, * pixg;

    setLeptDebugOK(1);

    pixg = pixConvertTo8(sourceImage, 0);
    scorefract = 0.1;

    /* Get a 1 bpp version; use a single tile */
    pixOtsuAdaptiveThreshold(pixg, 2000, 2000, 0, 0, scorefract,
        NULL, &pixb);

    return pixb;
}


void thresholdByFontColor(PIX* pix, QColor color)
{
    unsigned char r1 = color.redF() * 255;
    unsigned char g1 = color.greenF() * 255;
    unsigned char b1 = color.blueF() * 255;
    const auto isSimilarColor = [=](l_int32 r, l_int32 g, l_int32 b)
    {
        if ((r - r1) * (r - r1) + (g - g1) * (g - g1) + (b - b1) * (b - b1) <= 1000)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    for (int i = 0; i < pix->w; i++)
    {
        for (int j = 0; j < pix->h; j++)
        {
            l_int32 r = 0;
            l_int32 g = 0;
            l_int32 b = 0;
            pixGetRGBPixel(pix, i, j, &r, &g, &b);
            if (isSimilarColor(r, g, b))
            {
                pixSetRGBPixel(pix, i, j, 255, 255, 255);
            }
            else
            {
                pixSetRGBPixel(pix, i, j, 0, 0, 0);
            }
        }
    }
}