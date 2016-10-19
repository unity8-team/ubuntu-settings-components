project(UbuntuSettingsMenusQml)

add_definitions(-DUBUNTUSETTINGSCOMPONENTS_LIBRARY)

add_library(UbuntuSettingsMenusQml MODULE
    plugin.cpp
    types.h
    ${UbuntuSettingsMenusQml_SOURCES}
)

target_link_libraries(UbuntuSettingsMenusQml
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
    ${QMENUMODEL_LDFLAGS}
    ${LIBUPSTART_LIBRARIES}
)

add_usc_plugin(Ubuntu.Settings.Menus 0.1 Ubuntu/Settings/Menus TARGETS UbuntuSettingsMenusQml)
