project(UbuntuSettingsPrintersQml)

add_definitions(-DUBUNTUSETTINGSPRINTERS_LIBRARY)

include_directories(${CUPS_INCLUDE_DIR})

find_package(Qt5Gui REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Concurrent REQUIRED)

add_library(UbuntuSettingsPrintersQml SHARED
    backend/backend.cpp
    backend/backend_cups.cpp
    backend/backend_pdf.cpp
    cups/cupsfacade.cpp
    cups/cupspkhelper.cpp
    enums.h
    i18n.cpp
    models/drivermodel.cpp
    models/jobmodel.cpp
    models/printermodel.cpp
    printer/printer.cpp
    printer/printerjob.cpp
    printers/printers.cpp
    plugin.cpp
    structs.h
    utils.h
)

target_link_libraries(UbuntuSettingsPrintersQml
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
    Qt5::Concurrent
    ${CUPS_LIBRARIES}
)

add_usc_plugin(Ubuntu.Settings.Printers 0.1 Ubuntu/Settings/Printers TARGETS UbuntuSettingsPrintersQml)
