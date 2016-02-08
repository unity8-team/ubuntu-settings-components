include_directories(
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
)

add_library(UbuntuTestQml MODULE
    testutil.cpp
    plugin.cpp
    TouchEventSequenceWrapper.cpp
)

target_link_libraries(UbuntuTestQml
    Qt5::Core
    Qt5::Quick
    Qt5::Test
)

export_qmlfiles(Ubuntu.Test Ubuntu/Test NO_INSTALL)
export_qmlplugin(Ubuntu.Test 0.1 Ubuntu/Test TARGETS UbuntuTestQml NO_INSTALL)

add_custom_target(UbuntuTest)
add_dependencies(UbuntuTest UbuntuTestQml Ubuntu.Test-qmlfiles)
