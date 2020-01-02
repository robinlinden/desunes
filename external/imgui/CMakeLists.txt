include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG bdce8336364595d1a446957a6164c97363349a53 # v1.74
)
FetchContent_MakeAvailable(imgui)
