project(UbuntuSettingsPrintersQml)

add_definitions(-DUBUNTUSETTINGSPRINTERS_LIBRARY)

include_directories(${CUPS_INCLUDE_DIR})

find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Qml REQUIRED)

add_library(UbuntuSettingsPrintersQml SHARED
    cups/cupsfacade_impl.cpp
    cups/pkhelper.cpp
    models/printermodel.cpp
    printer/printer.cpp
    printer/printerinfo_impl.cpp
    printers/printers.cpp
    plugin.cpp
)

target_link_libraries(UbuntuSettingsPrintersQml
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
    ${CUPS_LIBRARIES}
)

add_usc_plugin(Ubuntu.Settings.Printers 0.1 Ubuntu/Settings/Printers TARGETS UbuntuSettingsPrintersQml)
