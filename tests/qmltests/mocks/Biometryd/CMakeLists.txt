include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(MockBiometryd MODULE
    plugin.cpp
    MockDevice.cpp
    MockFingerprintReader.cpp
    MockObserver.cpp
    MockOperation.cpp
    MockService.cpp
    MockTemplateStore.cpp
    MockUser.cpp
)

target_link_libraries(MockBiometryd
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
)

add_usc_mock(Biometryd 0.0 Biometryd TARGETS MockBiometryd)
