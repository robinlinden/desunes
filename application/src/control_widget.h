#pragma once

#include <string>

#include <nes/nes.h>

class ControlWidget {
public:
    ControlWidget(n_e_s::nes::Nes *nes, bool *running);
    void update();

private:
    n_e_s::nes::Nes *const nes_;
    bool *running_;
    std::string last_exception_{};
};
