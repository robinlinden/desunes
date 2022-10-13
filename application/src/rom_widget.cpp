#include "rom_widget.h"

#include <fstream>
#include <stdexcept>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

RomWidget::RomWidget(n_e_s::nes::Nes *nes) : nes_(nes) {
    file_widget_.add_fileopen_action([this] {
        std::string filename = file_widget_.get_selection();
        rom_path_ = filename;
        file_widget_open_ = false;
    });
    file_widget_.add_cancel_action([this] { file_widget_open_ = false; });
}

void RomWidget::update() {
    ImGui::Begin("Load rom");
    ImGui::InputText("", &rom_path_);
    ImGui::SameLine();
    try {
        if (ImGui::Button("Load")) {
            std::ifstream rom(rom_path_.data(), std::ios::binary);
            nes_->load_rom(rom);
            if (load_action_) {
                load_action_();
            }
            info_text_ = "Rom loaded";
        }
    } catch (const std::invalid_argument &e) {
        info_text_ = e.what();
    }
    try {
        ImGui::SameLine();
        if (ImGui::Button("Browse...")) {
            file_widget_open_ = true;
        }
    } catch (const std::invalid_argument &e) {
        info_text_ = e.what();
    }

    if (file_widget_open_) {
        file_widget_.update();
    }

    ImGui::Text("%s", info_text_.c_str());
    ImGui::End();
}

void RomWidget::add_load_action(const std::function<void(void)> &action) {
    load_action_ = action;
}
