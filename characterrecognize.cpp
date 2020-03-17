#include "characterrecognize.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
std::string ocr(QTextEdit * textedit)
{

    tesseract::TessBaseAPI tess;
    if (tess.Init("D:/GitHub/tessdata_best/tessdata", "jpn"))
    {
        std::cout << "OCRTesseract: Could not initialize tesseract." << std::endl;
        return std::string();
    }
    // setup
    tess.SetPageSegMode(tesseract::PageSegMode::PSM_AUTO);
    tess.SetVariable("save_best_choices", "T");
    //tess.SetVariable("preserve_interword_spaces", "1");

    auto pix = pixRead("2.png");
    if (pix == NULL)
    {
        return std::string();
    }
    tess.SetImage(pix);
    tess.Recognize(0);
    SetConsoleOutputCP(CP_UTF8);
    tess.Clear();
    pixDestroy(&pix);
    auto text = tess.GetUTF8Text();
    textedit->setText(QString::fromUtf8(std::unique_ptr<char[]>(tess.GetUTF8Text()).get()));
    return std::string();
}