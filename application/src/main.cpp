#include "control_widget.h"
#include "info_widget.h"
#include "rom_widget.h"

#include <nes/nes.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "desunes");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    n_e_s::nes::Nes nes;

    if (argc > 1) {
        nes.load_rom(argv[1]);
    }

    RomWidget rom_widget(&nes);
    ControlWidget ctrl_widget(&nes);
    InfoWidget info_widget(&nes);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        rom_widget.update();
        ctrl_widget.update();
        info_widget.update();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
