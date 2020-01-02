include(FetchContent)

FetchContent_Declare(imgui-sfml
    GIT_REPOSITORY https://github.com/eliasdaler/imgui-sfml.git
    GIT_TAG 488c321155547cb499697dac155aa6269d53c21f # v2.1
)

FetchContent_GetProperties(imgui
    SOURCE_DIR imgui_SOURCE_DIR
)

set(IMGUI_SFML_FIND_SFML FALSE)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

FetchContent_MakeAvailable(imgui-sfml)
