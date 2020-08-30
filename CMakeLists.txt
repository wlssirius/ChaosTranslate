cmake_minimum_required(VERSION 3.1.0)

project(ChaosTranslate)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Widgets Core GUI Multimedia Network websockets winextras REQUIRED)
find_package(tesseract)
find_package(leptonica)

file(GLOB RealTimeTranslator_SRC
    "src/*.h"
    "src/*.cpp"
)

file(GLOB Glossary_SRC
    "src/Glossary/*.h"
    "src/Glossary/*.cpp"
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
include_directories("src/Glossary")

# Link with libraries
target_link_libraries(ChaosTranslate ${Leptonica_LIBRARIES})
target_link_libraries(ChaosTranslate ${Tesseract_LIBRARIES})

# Post build events
add_custom_command(TARGET ChaosTranslate POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/src/Resources/Translation $<TARGET_FILE_DIR:ChaosTranslate>/Resources/Translation)
add_custom_command(TARGET ChaosTranslate POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/submodules/tessdata_best/chi_sim.traineddata 
	$<TARGET_FILE_DIR:ChaosTranslate>/tessdata_best)
add_custom_command(TARGET ChaosTranslate POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/submodules/tessdata_best/eng.traineddata 
	$<TARGET_FILE_DIR:ChaosTranslate>/tessdata_best)
