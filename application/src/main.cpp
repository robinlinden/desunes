#include <nes/nes.h>
#include <imgui-SFML.h>
#include <imgui.h>
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        {
            ImGui::Begin("Load rom");
            static char rom_path[64];
            ImGui::InputText("", rom_path, IM_ARRAYSIZE(rom_path));
            ImGui::SameLine();
            if (ImGui::Button("Load")) {
                nes.load_rom(rom_path);
            }

            ImGui::End();
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
