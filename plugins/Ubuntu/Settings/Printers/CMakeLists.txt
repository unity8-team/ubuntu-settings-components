project(UbuntuSettingsPrintersQml)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

include_directories(${CUPS_INCLUDE_DIR})

find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Gui REQUIRED)

add_library(UbuntuSettingsPrintersQml SHARED
    printer/printer.cpp
    printer/printerinfo_impl.cpp
)

target_link_libraries(UbuntuSettingsPrintersQml
    Qt5::PrintSupport
    Qt5::Gui
    ${CUPS_LIBRARIES}
)

#add_usc_plugin(Ubuntu.Settings.Printers 0.1 Ubuntu/Settings/Printers TARGETS UbuntuSettingsPrintersQml)
