#pragma once

#include <nes/nes.h>

class ControlWidget {
public:
    ControlWidget(n_e_s::nes::Nes *nes, bool *running, int *step_running);
    void update();

private:
    n_e_s::nes::Nes *const nes_;
    bool *running_;
    int *step_running_;
};
