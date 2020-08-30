# Introduction
ChaosTranslate is a lightweight OCR and translation tool with GUI. It uses [TesseractOCR](https://github.com/tesseract-ocr/tesseract) to detect characters inside the screenshot of user-selected application and [QOnlineTranslator](https://github.com/crow-translate/QOnlineTranslator) to translate into target language. Please refer to below gif for brief usage, or check [Manual](https://github.com/wlssirius/ChaosTranslate/wiki/Manual) for more details.   
Chaos翻译是一款开源文字识别（OCR）及翻译工具，使用[TesseractOCR](https://github.com/tesseract-ocr/tesseract)进行文字识别，并使用[QOnlineTranslator](https://github.com/crow-translate/QOnlineTranslator)进行在线翻译。 使用方法请参考以下gif，或点击[使用说明](https://github.com/wlssirius/ChaosTranslate/wiki/%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8E)查看更多细节。
![Image of UI](screenshot/english.gif)    

# Installation
Download the latest [release](https://github.com/wlssirius/ChaosTranslate/releases), unzip and run ChaosTranslate.exe to start using. If the translation is not displayed correctly, please run vcredist_x64.exe (included in the zip) and install VC++ 2000 Redistributable.   
下载最新[版本](https://github.com/wlssirius/ChaosTranslate/releases)，解压并运行ChaosTranslate.exe即可开始使用。如果翻译结果显示不正确, 请运行压缩包中的vcredist_x64.exe并安装VC++ 2000 Redistributable.   

# Platform   
Windows   

# Compile Requirements   
1. [CMake](https://cmake.org/install/) (preferably > 3.18.0)      
2. [Qt5](https://github.com/qt/qt5)   
3. [TesseractOCR](https://tesseract-ocr.github.io/tessdoc/Compiling.html#windows)   

# Compile Process
1. Make sure all dependencies in Requirements are installed  
2. Clone this branch and update submodule
  >git submodule update --init --recursive
3. Compile using CMake   
