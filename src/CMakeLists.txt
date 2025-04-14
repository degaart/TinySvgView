file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.c")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hpp" "*.h")
#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE -DWIN32_LEAN_AND_MEAN)
target_compile_options(${PROJECT_NAME} PRIVATE /UUNICODE /U_UNICODE)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fmt::fmt
        plutosvg)
