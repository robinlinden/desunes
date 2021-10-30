#pragma once

#include <nes/core/ines_controller.h>
#include <map>

class KeyboardController {
public:
    KeyboardController(n_e_s::core::INesController &controller);
    void map_key(int key_id,
            n_e_s::core::INesController::Button controller_button);
    void set_key(int key_id, bool is_down);

private:
    n_e_s::core::INesController &controller_;
    std::map<int, n_e_s::core::INesController::Button> key_button_map_{};
};