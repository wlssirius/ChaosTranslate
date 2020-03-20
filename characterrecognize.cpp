#include "characterrecognize.h"
#include <leptonica/allheaders.h>
#include <iostream>
QString ocr()
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

    auto pix = pixRead("D:/TestFile.bmp");
    tess.SetImage(pix);
    tess.Recognize(0);
    auto text = tess.GetUTF8Text();
    QString result = QString::fromUtf8(text);

    // cleanup
    tess.Clear();
    pixDestroy(&pix);
    
    return result;
}