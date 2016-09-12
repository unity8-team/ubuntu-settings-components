project(UbuntuSettingsComponentsQml)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

add_library(UbuntuSettingsComponentsQml MODULE
    plugin.cpp
    serverpropertysynchroniser.cpp
)

target_link_libraries(UbuntuSettingsComponentsQml
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)

add_usc_plugin(Ubuntu.Settings.Components 0.1 Ubuntu/Settings/Components TARGETS UbuntuSettingsComponentsQml)
