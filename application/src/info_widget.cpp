#include "info_widget.h"

#include <cinttypes>
#include <sstream>

#include <nes/core/imos6502.h>
#include <nes/core/ippu.h>

#include <imgui.h>

namespace {
std::string to_string(const n_e_s::core::CpuRegisters &r) {
    using namespace n_e_s::core;

    std::stringstream ss;
    ss << "PC: " << +r.pc;
    ss << " SP: " << +r.sp;
    ss << " A: " << +r.a;
    ss << " X: " << +r.x;
    ss << " Y: " << +r.y;
    ss << " P: ";
    ss << (r.p & N_FLAG ? "N" : "-");
    ss << (r.p & V_FLAG ? "V" : "-");
    ss << "-";
    ss << (r.p & B_FLAG ? "B" : "-");
    ss << (r.p & D_FLAG ? "D" : "-");
    ss << (r.p & I_FLAG ? "I" : "-");
    ss << (r.p & Z_FLAG ? "Z" : "-");
    ss << (r.p & C_FLAG ? "C" : "-");
    return ss.str();
}

std::string to_string(const n_e_s::core::PpuRegisters &r) {
    using namespace n_e_s::core;

    std::stringstream ss;
    ss << "ctrl: " << std::hex << std::showbase << +r.ctrl.value()
            << " mask: " << +r.mask.value() << " status: " << +r.status.value()
            << " oamaddr: " << +r.oamaddr << std::endl
            << "x_scroll: " << +r.fine_x_scroll << " vram_addr: " 
            << r.vram_addr.value() << "/" << r.temp_vram_addr.value() 
            << " write_toggle: " << r.write_toggle;

    return ss.str();
}
} // namespace

InfoWidget::InfoWidget(n_e_s::nes::Nes *const nes,
                       std::string *run_status,
                       std::string *err_status)
: nes_(nes), run_status_(run_status), err_status_(err_status) 
{}

void InfoWidget::update() {
    ImGui::Begin("Info");
    ImGui::Text("Status: %s", run_status_->c_str());
    ImGui::Text("Error: ");
    ImGui::SameLine();
    if (err_status_->empty()) {
        ImGui::Text(" -");
    } else {
        ImGui::Text("%s", err_status_->c_str());
    }
    ImGui::Separator();
    ImGui::Text("Cycle: %" PRIu64, nes_->current_cycle());
    ImGui::TextUnformatted(to_string(nes_->cpu_registers()).c_str());
    ImGui::TextUnformatted(to_string(nes_->ppu_registers()).c_str());
    ImGui::End();
}
