include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG e5cb04b132cba94f902beb6186cb58b864777012 # v1.79
)
FetchContent_MakeAvailable(imgui)
