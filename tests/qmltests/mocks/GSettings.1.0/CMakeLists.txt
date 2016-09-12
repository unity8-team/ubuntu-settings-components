include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(GSettingsQML_SOURCES
)

add_library(FakeGSettingsQml MODULE
  fake_gsettings.cpp
  plugin.cpp
)

target_link_libraries(FakeGSettingsQml
  Qt5::Core
  Qt5::Quick
  Qt5::Qml
)

add_usc_mock(GSettings 1.0 GSettings.1.0 TARGETS FakeGSettingsQml)
