#pragma once

#include <functional>
#include <string>

#include <nes/nes.h>

#include "file_widget.h"

class RomWidget {
public:
    RomWidget(n_e_s::nes::Nes *nes);

    void update();
    void add_load_action(const std::function<void(void)> &action);

private:
    n_e_s::nes::Nes *const nes_;
    std::function<void(void)> load_action_{};

    std::string rom_path_{};
    std::string info_text_{};

    bool file_widget_open_{false};
    FileWidget file_widget_{};
};
