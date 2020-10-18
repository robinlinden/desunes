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
    ss << "ctrl: " << std::hex << std::showbase << +r.ctrl
            << " mask: " << +r.mask << " status: " << +r.status
            << " oamaddr: " << +r.oamaddr << std::endl << "x_scroll: "
            << +r.fine_x_scroll << " vram_addr: " << r.vram_addr << "/"
            << r.temp_vram_addr << " write_toggle: " << r.write_toggle;

    return ss.str();
}
} // namespace

InfoWidget::InfoWidget(n_e_s::nes::Nes *const nes) : nes_(nes) {}

void InfoWidget::update() {
    ImGui::Begin("Info");
    ImGui::Text("Cycle: %" PRIu64, nes_->current_cycle());
    ImGui::TextUnformatted(to_string(nes_->cpu_registers()).c_str());
    ImGui::TextUnformatted(to_string(nes_->ppu_registers()).c_str());
    ImGui::End();
}
