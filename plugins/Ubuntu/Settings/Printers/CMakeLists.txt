project(UbuntuSettingsPrintersQml)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

add_library(UbuntuSettingsPrintersQml MODULE
    plugin.cpp
    cupspkhelpermechanism.cpp
)

find_package(Qt5DBus REQUIRED)

target_link_libraries(UbuntuSettingsPrintersQml
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    Qt5::DBus
)

add_usc_plugin(Ubuntu.Settings.Printers 0.1 Ubuntu/Settings/Printers TARGETS UbuntuSettingsPrintersQml)
