#include "rom_widget.h"

#include <stdexcept>

#include <imgui.h>

RomWidget::RomWidget(n_e_s::nes::Nes *nes) : nes_(nes) {}

void RomWidget::update() {
    ImGui::Begin("Load rom");
    ImGui::InputText("", rom_path_.data(), rom_path_.size());
    ImGui::SameLine();
    try {
        if (ImGui::Button("Load")) {
            nes_->load_rom(rom_path_.data());
            info_text_ = "Rom loaded";
        }
    } catch (const std::invalid_argument &e) {
        info_text_ = e.what();
    }

    ImGui::Text(info_text_.c_str());
    ImGui::End();
}
