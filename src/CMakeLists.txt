file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE /DWIN32_LEAN_AND_MEAN)
target_compile_options(${PROJECT_NAME} PRIVATE /UUNICODE /U_UNICODE)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fmt::fmt)