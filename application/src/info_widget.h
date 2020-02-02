#pragma once

#include <nes/nes.h>

class InfoWidget {
public:
    InfoWidget(n_e_s::nes::Nes *nes);
    void update();

private:
    n_e_s::nes::Nes *const nes_;
};
