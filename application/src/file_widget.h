#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>

class FileWidget {
public:
    FileWidget();

    void update();
    void add_fileopen_action(const std::function<void(void)> &action);
    void add_cancel_action(const std::function<void(void)> &action);
    std::string get_selection();

private:
    void update_dir_content();

    std::function<void(void)> open_action_{};
    std::function<void(void)> cancel_action_{};
    
    std::filesystem::path current_path_{};
    std::vector<std::filesystem::directory_entry> dir_content_;
    std::filesystem::path selected_file_{};
};
