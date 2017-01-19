project(UbuntuSettingsPrintersQml)

add_definitions(-DUBUNTUSETTINGSPRINTERS_LIBRARY)

include_directories(${CUPS_INCLUDE_DIR})

find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Qml REQUIRED)

add_library(UbuntuSettingsPrintersQml SHARED
    enums.h
    cups/cupsfacade.h
    cups/cupsfacade_impl.cpp
    cups/cupspkhelper.cpp
    models/printermodel.cpp
    printer/printer.cpp
    printer/printerinfo_allimpl.cpp
    printer/printerinfo_pdfimpl.cpp
    printer/printerinfo_impl.cpp
    printer/printerjob.cpp
    printers/printers.cpp
    plugin.cpp
    utils.h
)

target_link_libraries(UbuntuSettingsPrintersQml
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
    ${CUPS_LIBRARIES}
)

add_usc_plugin(Ubuntu.Settings.Printers 0.1 Ubuntu/Settings/Printers TARGETS UbuntuSettingsPrintersQml)
