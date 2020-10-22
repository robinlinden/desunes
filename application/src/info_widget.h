#pragma once

#include <nes/nes.h>

class InfoWidget {
public:
    InfoWidget(n_e_s::nes::Nes *nes, 
               std::string *run_status, 
               std::string *err_status);
    void update();

private:
    n_e_s::nes::Nes *const nes_;
    std::string *const run_status_;
    std::string *const err_status_;
};
