cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME small_ledger)
project(${PROJECT_NAME})
set(CMAKE_PREFIX_PATH "/opt/apps/Qt5.14.2/5.14.2/gcc_64")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt5 COMPONENTS Core Charts Sql Widgets LinguistTools REQUIRED)

set(ledger_HEADERS
        login/LoginController.h
        login/LoginMain.h
        utils/DatabaseUtils.h
        utils/PasswordUtils.h
        index/IndexMain.h
        main/MainWidget.h
        history/HistoryList.h
        history/AddHistory.h
        notice/NoticeWidget.h
        utils/CommonUtils.h
        index/MainForm.h
        type/TypeListView.h
        type/TypeDetails.h
        user/UserMain.h
        help/HelpView.h
)

set(ledger_UI
        login/LoginMain.ui
        index/IndexMain.ui
        history/HistoryList.ui
        history/AddHistory.ui
        notice/NoticeWidget.ui
        index/MainForm.ui
        type/TypeListView.ui
        type/TypeDetails.ui
        user/UserMain.ui
        help/HelpView.ui
)

set(ledger_SOURCE
        login/LoginController.cpp
        login/LoginMain.cpp
        utils/source/DatabaseUtils.cpp
        utils/source/PasswordUtils.cpp
        main.cpp
        index/IndexMain.cpp
        main/MainWidget.cpp
        history/HistoryList.cpp
        history/AddHistory.cpp
        notice/NoticeWidget.cpp
        history/ListViewDelegate.cpp
        history/HistoryListDelegate.cpp
        utils/source/CommonUtils.cpp
        index/MainForm.cpp
        type/TypeListView.cpp
        type/TypeListDelegate.cpp
        type/TypeDetails.cpp
        user/UserMain.cpp
        help/HelpView.cpp
)

set(ledger_RESOURCE
    resource/resource.qrc
)

set(ledger_ICO
    resource/ico.rc
)

qt_wrap_cpp(HEAD ${ledger_HEADERS})
qt5_wrap_ui(UI ${ledger_UI})
qt5_add_resources(qrc_FILES ${ledger_RESOURCE} )

add_executable(${PROJECT_NAME} ${HEAD} ${UI} ${ledger_SOURCE} ${qrc_FILES} ${ledger_ICO})
qt5_use_modules(${PROJECT_NAME} Core Charts Widgets Sql)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Charts Qt5::Core Qt5::Sql)

