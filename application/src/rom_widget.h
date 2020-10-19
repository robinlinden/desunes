#pragma once

#include <array>
#include <functional>
#include <string>

#include <nes/nes.h>

class RomWidget {
public:
    RomWidget(n_e_s::nes::Nes *nes);

    void update();
    void add_load_action(const std::function<void(void)> &action);

private:
    n_e_s::nes::Nes *const nes_;
    std::function<void(void)> load_action_{};

    std::array<char, 64> rom_path_{};
    std::string info_text_{};
};
