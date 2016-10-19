project(UbuntuSettingsComponentsQml)

pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(
    ${GLIB_INCLUDE_DIRS}
)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

add_library(UbuntuSettingsComponentsQml MODULE
    plugin.cpp
    filepickerhelper.cpp
    serverpropertysynchroniser.cpp
)

target_link_libraries(UbuntuSettingsComponentsQml
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    ${GLIB_LDFLAGS}
)

add_usc_plugin(Ubuntu.Settings.Components 0.1 Ubuntu/Settings/Components TARGETS UbuntuSettingsComponentsQml)
