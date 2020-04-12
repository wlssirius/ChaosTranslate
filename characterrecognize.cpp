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

    auto pix = pixRead("D:/1.png");
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

void replaceTest()
{
    char* dirin, * dirout, * infile, * outfile, * tail;
    l_int32      i, nfiles, border, x, y, w, h, xb, yb, wb, hb;
    BOX* box1, * box2;
    BOXA* boxa1, * boxa2;
    PIX* pixs, * pixt1, * pixd;
    SARRAY* safiles;
    static char  mainName[] = "croptext";


    dirin = "D:/atest1/";
    border = 10;
    dirout = "D:/atest1/test1/";

    safiles = getSortedPathnamesInDirectory(dirin, NULL, 0, 0);
    nfiles = sarrayGetCount(safiles);

    for (i = 0; i < nfiles; i++) {
        infile = sarrayGetString(safiles, i, 0);
        splitPathAtDirectory(infile, NULL, &tail);
        outfile = genPathname(dirout, tail);
        pixs = pixRead(infile);
        PIX* newPix = pixClone(pixs);
        l_int32 wpl = pixGetWpl(pixs);
        l_int32 newWpl = pixGetWpl(newPix);
        l_uint32* data = pixGetData(pixs);
        l_uint32* newData = pixGetData(newPix);
        for (int i = 0; i < wpl; i++)
        {
            for (int j = 0; j < pixs->h; j++)
            {
                l_uint32* line = data + j * wpl;
                l_uint32* newLine = newData + j * newWpl;
                l_uint32 value = GET_DATA_BYTE(line, i);
                SET_DATA_BYTE(newLine, i, value);
            }
        }
        pixWrite(outfile, newPix, IFF_PNG);

        pixDestroy(&pixs); 
        pixDestroy(&newPix);
    }
}

void otsuTest()
{

    char       textstr[256];
    l_int32    i, thresh, fgval, bgval;
    l_float32  scorefract;
    L_BMF* bmf;
    PIX* pixs, * pixb, * pixg, * pixp, * pix1, * pix2, * pix3;
    PIXA* pixa1, * pixad;

    setLeptDebugOK(1);
    lept_mkdir("lept/otsu");

    pixs = pixRead("D:/1.png");
    pixg = pixConvertTo8(pixs, 0);
    bmf = bmfCreate(NULL, 8);
    pixad = pixaCreate(0);
    for (i = 0; i < 3; i++) {
        pixa1 = pixaCreate(2);
        scorefract = 0.1 * i;

        /* Get a 1 bpp version; use a single tile */
        pixOtsuAdaptiveThreshold(pixg, 2000, 2000, 0, 0, scorefract,
            NULL, &pixb);
        pixaAddPix(pixa1, pixg, L_COPY);
        pixaAddPix(pixa1, pixb, L_INSERT);

        /* Show the histogram of gray values and the split location */
        pixSplitDistributionFgBg(pixg, scorefract, 1,
            &thresh, &fgval, &bgval, &pixp);
        fprintf(stderr, "thresh = %d, fgval = %d, bgval = %d\n", thresh, fgval,
            bgval);
        pixaAddPix(pixa1, pixp, L_INSERT);

        /* Join these together and add some text */
        pix1 = pixaDisplayTiledInColumns(pixa1, 3, 1.0, 20, 2);
        snprintf(textstr, sizeof(textstr),
            "Scorefract = %3.1f ........... Thresh = %d", scorefract, thresh);
        pix2 = pixAddSingleTextblock(pix1, bmf, textstr, 0x00ff0000,
            L_ADD_BELOW, NULL);


        /* Save and display the result */
        pixaAddPix(pixad, pix2, L_INSERT);
        snprintf(textstr, sizeof(textstr), "D:/%03d.png", i);
        pixWrite(textstr, pix2, IFF_PNG);
        pixDisplay(pix2, 100, 100);
        pixDestroy(&pix1);
        pixaDestroy(&pixa1);
    }

    /* Use a smaller tile for Otsu */
    for (i = 0; i < 2; i++) {
        scorefract = 0.1 * i;
        pixOtsuAdaptiveThreshold(pixg, 300, 300, 0, 0, scorefract,
            NULL, &pixb);
        pix1 = pixAddBlackOrWhiteBorder(pixb, 2, 2, 2, 2, L_GET_BLACK_VAL);
        pix2 = pixScale(pix1, 0.5, 0.5);
        snprintf(textstr, sizeof(textstr),
            "Scorefract = %3.1f", scorefract);
        pix3 = pixAddSingleTextblock(pix2, bmf, textstr, 1,
            L_ADD_BELOW, NULL);
        pixaAddPix(pixad, pix3, L_INSERT);
        pixDestroy(&pixb);
        pixDestroy(&pix1);
        pixDestroy(&pix2);
    }

    fprintf(stderr, "Writing to: /tmp/lept/otsu/result1.pdf\n");
    pixaConvertToPdf(pixad, 75, 1.0, 0, 0, "Otsu thresholding",
        "D:/result1.pdf");
    bmfDestroy(&bmf);
    pixDestroy(&pixs);
    pixDestroy(&pixg);
    pixaDestroy(&pixad);
}
