project(desunes_application)

add_executable(${PROJECT_NAME}
    src/control_widget.cpp
    src/control_widget.h
    src/file_widget.cpp
    src/file_widget.h
    src/info_widget.cpp
    src/info_widget.h
    src/main.cpp
    src/rom_widget.cpp
    src/rom_widget.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ImGui-SFML::ImGui-SFML
        n_e_s::core
        n_e_s::nes
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_17
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
