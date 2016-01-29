include(QmlTest)

# QML tests that require graphical capabilities.
add_custom_target(uitests)
add_custom_target(xvfbuitests)

add_custom_target(alltests)
add_dependencies(alltests uitests)

add_custom_target(xvfballtests)
add_dependencies(xvfballtests xvfbuitests)

add_subdirectory(utils)

# add a graphical qml test
# see QmlTest.cmake for additional options
function(add_usc_qmltest PATH COMPONENT_NAME)
    add_qml_test(${PATH} ${COMPONENT_NAME}
        DEPENDS UbuntuTest
        IMPORT_PATHS ${CMAKE_BINARY_DIR}/tests/utils/modules
        TARGETS uitests
        ${ARGN}
    )
endfunction()

add_subdirectory(imports)
add_subdirectory(qmltests)
