include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/Ubuntu/Settings/Printers
)

find_package(Qt5Gui REQUIRED)
find_package(Qt5Test REQUIRED)

set(MOCK_SOURCES
    mockcupsfacade.h
    mockprinterinfo.h
)

add_executable(testPrintersPrinter tst_printer.cpp ${MOCK_SOURCES})
target_link_libraries(testPrintersPrinter UbuntuSettingsPrintersQml Qt5::Test Qt5::Gui)
add_test(tst_printer testPrintersPrinter)

add_executable(testPrintersPrinters tst_printers.cpp ${MOCK_SOURCES})
target_link_libraries(testPrintersPrinters UbuntuSettingsPrintersQml Qt5::Test Qt5::Gui)
add_test(tst_printers testPrintersPrinters)

add_executable(testPrintersPrinterModel tst_printermodel.cpp ${MOCK_SOURCES})
target_link_libraries(testPrintersPrinterModel UbuntuSettingsPrintersQml Qt5::Test Qt5::Gui)
add_test(tst_printermodel testPrintersPrinterModel)
