#include "characterrecognize.h"
#include <leptonica/allheaders.h>
#include <iostream>

QString ocr(PIX* pix)
{
    tesseract::TessBaseAPI tess;
    if (tess.Init("D:/GitHub/tessdata_best/tessdata", "jpn"))
    {
        std::cout << "OCRTesseract: Could not initialize tesseract." << std::endl;
        return 1;
    }
    // setup
    tess.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
    tess.SetVariable("save_best_choices", "T");
    tess.SetVariable("preserve_interword_spaces", "1");

    tess.SetImage(pix);
    tess.Recognize(0);
    auto text = tess.GetUTF8Text();
    QString result = QString::fromUtf8(text); 
    auto thresholded = tess.GetThresholdedImage();
    
    pixWrite("D:/TestFile_Threshold.png", thresholded, IFF_PNG);
    

    // cleanup
    tess.Clear();
    pixDestroy(&pix);
    
    return result;
}

PIX* threshold(PIX* sourceImage, BOX* roi)
{
    PIX* imgCrop = pixClipRectangle(sourceImage, roi, NULL);
    PIX* thresholded = otsu(imgCrop);
    pixWrite("D:/1_localTh.png", thresholded, IFF_PNG);
    PIX* pixg = pixConvertTo1(sourceImage, 0);
    replace(pixg, thresholded, roi);
    return pixg;
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
