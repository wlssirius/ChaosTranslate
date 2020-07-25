cmake_minimum_required(VERSION 3.1.0)

project(ChaosTranslate)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core GUI Multimedia Network websockets winextras REQUIRED)
find_package(tesseract)
find_package(leptonica)

file(GLOB RealTimeTranslator_SRC
    "*.h"
    "*.cpp"
)

file(GLOB Glossary_SRC
    "Glossary/*.h"
    "Glossary/*.cpp"
)

file(GLOB OnlineTranslator_SRC 
    "submodules/QOnlineTranslator/src/*.h"
    "submodules/QOnlineTranslator/src/*.cpp"
)

add_executable(ChaosTranslate 
    ${RealTimeTranslator_SRC} 
    ${Glossary_SRC}
    ${OnlineTranslator_SRC}
)

target_link_libraries(ChaosTranslate Qt5::Widgets)
target_link_libraries(ChaosTranslate Qt5::Core)
target_link_libraries(ChaosTranslate Qt5::Gui)
target_link_libraries(ChaosTranslate Qt5::Multimedia)
target_link_libraries(ChaosTranslate Qt5::Network)
target_link_libraries(ChaosTranslate Qt5::WebSockets)
target_link_libraries(ChaosTranslate Qt5::WinExtras)

# Include directories
include_directories(${Tesseract_INCLUDE_DIRS})
include_directories(${Leptonica_INCLUDE_DIRS})
include_directories("submodules/QOnlineTranslator/src")
include_directories("submodules/RapidXML")
include_directories("Glossary")

# Link with libraries
target_link_libraries(ChaosTranslate ${Leptonica_LIBRARIES})
target_link_libraries(ChaosTranslate ${Tesseract_LIBRARIES})