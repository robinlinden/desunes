#include "control_widget.h"

#include <stdexcept>

#include <imgui.h>

ControlWidget::ControlWidget(
        n_e_s::nes::Nes *const nes,
        bool *running,
        int *step_running) :
    nes_(nes),
    running_(running),
    step_running_(step_running) {}

void ControlWidget::update() {
    ImGui::Begin("NES control");
    try {
        if (ImGui::Button("Step")) {
            *step_running_ = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 10")) {
            *step_running_ = 10;
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 100")) {
            *step_running_ = 100;
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 1000")) {
            *step_running_ = 1000;
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
            *step_running_ = 10000;
        }
        ImGui::SameLine();
        if (ImGui::Button("Step 100000")) {
            *step_running_ = 100000;
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
