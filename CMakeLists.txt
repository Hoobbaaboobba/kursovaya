cmake_minimum_required(VERSION 3.16)
project(buyerdatabase VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Charts
    PrintSupport
    LinguistTools
    REQUIRED
)

# Add Qt include directories
include_directories(${Qt6Core_INCLUDE_DIRS}
                  ${Qt6Gui_INCLUDE_DIRS}
                  ${Qt6Widgets_INCLUDE_DIRS}
                  ${Qt6Charts_INCLUDE_DIRS}
                  ${Qt6PrintSupport_INCLUDE_DIRS})

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/models/buyermodel.cpp
    src/models/buyermodel.h
    src/controllers/buyercontroller.cpp
    src/controllers/buyercontroller.h
    src/views/buyerview.cpp
    src/views/buyerview.h
    src/views/buyerdialog.cpp
    src/views/buyerdialog.h
    src/dialogs/aboutdialog.cpp
    src/dialogs/aboutdialog.h
)

add_executable(buyerdatabase ${PROJECT_SOURCES})

target_link_libraries(buyerdatabase PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Charts
    Qt6::PrintSupport
)

# Install translations
set(TRANSLATIONS
    translations/buyerdatabase_en.ts
    translations/buyerdatabase_ru.ts
)

qt6_add_translation(QM_FILES ${TRANSLATIONS})
target_sources(buyerdatabase PRIVATE ${QM_FILES})

# Install
install(TARGETS buyerdatabase
    RUNTIME DESTINATION bin
)

install(FILES ${QM_FILES}
    DESTINATION share/buyerdatabase/translations
) 