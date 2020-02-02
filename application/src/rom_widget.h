#pragma once

#include <array>
#include <string>

#include <nes/nes.h>

class RomWidget {
public:
    RomWidget(n_e_s::nes::Nes *nes);

    void update();

private:
    n_e_s::nes::Nes *const nes_;
    std::array<char, 64> rom_path_{};
    std::string info_text_{};
};
