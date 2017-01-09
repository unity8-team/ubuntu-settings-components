include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/Ubuntu/Settings/Printers
)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Test REQUIRED)

add_executable(testPrintersPrinter
    tst_printer.cpp
    mockcupsfacade.h
    mockprinterinfo.h
)
target_link_libraries(testPrintersPrinter UbuntuSettingsPrintersQml Qt5::Test Qt5::Gui)
add_test(tst_printer testPrintersPrinter)
