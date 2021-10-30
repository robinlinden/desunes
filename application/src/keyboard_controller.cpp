#include "keyboard_controller.h"

KeyboardController::KeyboardController(n_e_s::core::INesController &controller)
: controller_(controller) {}

void KeyboardController::map_key(int key_id,
        n_e_s::core::INesController::Button controller_button) {
    key_button_map_.emplace(key_id, controller_button);
}

void KeyboardController::set_key(int key_id, bool is_down) {
    if (auto key = key_button_map_.find(key_id); key != key_button_map_.end()) {
        controller_.set(key->second, is_down);
    }
}
