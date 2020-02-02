#pragma once

#include <string>

#include <nes/nes.h>

class ControlWidget {
public:
    ControlWidget(n_e_s::nes::Nes *nes);
    void update();

private:
    n_e_s::nes::Nes *const nes_;
    bool running_{false};
    std::string last_exception_{};
};
