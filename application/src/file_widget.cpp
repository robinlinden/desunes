#include "file_widget.h"

#include <imgui.h>

FileWidget::FileWidget() {
    current_path_ = std::filesystem::current_path();
    update_dir_content();
}

void FileWidget::update() {
    ImGui::Begin("Select rom file...");
    ImGui::SetWindowSize(ImVec2(350, 320));
    ImGui::Text(std::filesystem::current_path().string().c_str(), 50);
    ImGui::Separator();

    bool dir_changed = false;

    ImGui::ListBoxHeader("", ImVec2(340, 210));
    if (ImGui::Button("..")) {
        current_path_ = std::filesystem::current_path().parent_path();
        dir_changed = true;
    }
    for (const auto &entry : dir_content_) {
        if (entry.is_directory()) {
            if (ImGui::Button(entry.path().filename().string().c_str())) {
                current_path_ = entry.path();
                dir_changed = true;
            }
        } else {
            if (ImGui::Button(entry.path().filename().string().c_str())) {
                selected_file_ = entry.path();
            }
        }
    }
    ImGui::ListBoxFooter();

    if (dir_changed) {
        update_dir_content();
    }

    ImGui::Text(
            "Selected file: %s", selected_file_.filename().string().c_str());
    ImGui::Separator();

    if (ImGui::Button("Open")) {
        if (open_action_) {
            open_action_();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        if (cancel_action_) {
            cancel_action_();
        }
    }
    ImGui::End();
}

void FileWidget::add_fileopen_action(const std::function<void(void)> &action) {
    open_action_ = action;
}

void FileWidget::add_cancel_action(const std::function<void(void)> &action) {
    cancel_action_ = action;
}

std::string FileWidget::get_selection() {
    return selected_file_.string();
}

void FileWidget::update_dir_content() {
    dir_content_.clear();
    for (const auto &entry :
            std::filesystem::directory_iterator(current_path_)) {
        dir_content_.push_back(entry);
    }
    std::filesystem::current_path(current_path_);
}
