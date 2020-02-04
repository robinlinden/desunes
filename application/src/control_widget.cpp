#include "control_widget.h"

#include <stdexcept>

#include <imgui.h>

ControlWidget::ControlWidget(
        n_e_s::nes::Nes *const nes,
        bool *running) :
    nes_(nes),
    running_(running) {}

void ControlWidget::update() {
    ImGui::Begin("NES control");
    try {
        if (ImGui::Button("Step")) {
            nes_->execute();
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 10")) {
            for (size_t i = 0; i < 10; ++i) {
                nes_->execute();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 100")) {
            for (size_t i = 0; i < 100; ++i) {
                nes_->execute();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 1000")) {
            for (size_t i = 0; i < 1000; ++i) {
                nes_->execute();
            }
        }

        ImGui::SameLine();
        if (!*running_) {
            if (ImGui::Button("Run")) {
                *running_ = true;
            }
        } else {
            if (ImGui::Button("Stop")) {
                *running_ = false;
            }
        }

        if (ImGui::Button("Step 10000")) {
            for (size_t i = 0; i < 10000; ++i) {
                nes_->execute();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 100000")) {
            for (size_t i = 0; i < 100000; ++i) {
                nes_->execute();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset")) {
            nes_->reset();
        }
    } catch (const std::logic_error &e) {
        last_exception_ = e.what();
        *running_ = false;
    }
    ImGui::Text(last_exception_.c_str());
    ImGui::End();
}
