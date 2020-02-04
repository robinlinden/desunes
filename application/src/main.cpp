#include "control_widget.h"
#include "info_widget.h"
#include "rom_widget.h"

#include <iostream>

#include <nes/nes.h>
#include <imgui-SFML.h>
#include <SFML/System.hpp>
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

    bool running = false;
    RomWidget rom_widget(&nes);
    ControlWidget ctrl_widget(&nes, &running);
    InfoWidget info_widget(&nes);

    while (window.isOpen()) {
        deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.getElapsedTime());

        rom_widget.update();
        ctrl_widget.update();
        info_widget.update();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();

        constexpr size_t master_clock = 21'477'272; // Hz
        // Tick rates:
        // CPU: every 12 master ticks
        // PPU: every 4
        // APU: every 24
        constexpr size_t n_e_s_clock = master_clock / 4;
        constexpr size_t fps = 60;
        constexpr size_t tick_per_frame = n_e_s_clock / fps;
        if (running) {
            for (size_t i = 0; i < tick_per_frame; ++i) {
                nes.execute();
            }
        }

        const auto frameTime = deltaClock.getElapsedTime();
        if (frameTime.asMilliseconds() > 16) {
            std::cerr << "Things are running slowly: "
                    << frameTime.asMilliseconds() << "ms" << std::endl;
        }

        const auto sleepTime = std::max(0, 16 - frameTime.asMilliseconds());
        sf::sleep(sf::milliseconds(sleepTime));
    }

    ImGui::SFML::Shutdown();

    return 0;
}
