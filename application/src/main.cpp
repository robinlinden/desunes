#include <cinttypes>
#include <sstream>

#include <core/imos6502.h>

#include <nes/nes.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

std::string to_string(const n_e_s::core::CpuRegisters &r) {
    using namespace n_e_s::core;

    std::stringstream ss;
    ss << "PC: " << +r.pc;
    ss << " SP: " << +r.sp;
    ss << " A: " << +r.a;
    ss << " X: " << +r.x;
    ss << " Y: " << +r.y;
    ss << " P: ";
    ss << (r.p & N_FLAG ? "N" : "-");
    ss << (r.p & V_FLAG ? "V" : "-");
    ss << "-";
    ss << (r.p & B_FLAG ? "B" : "-");
    ss << (r.p & D_FLAG ? "D" : "-");
    ss << (r.p & I_FLAG ? "I" : "-");
    ss << (r.p & Z_FLAG ? "Z" : "-");
    ss << (r.p & C_FLAG ? "C" : "-");
    return ss.str();
}

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
            static std::string info_text{};

            ImGui::Begin("Load rom");
            static char rom_path[64];
            ImGui::InputText("", rom_path, IM_ARRAYSIZE(rom_path));
            ImGui::SameLine();
            try {
                if (ImGui::Button("Load")) {
                    nes.load_rom(rom_path);
                    info_text = "Rom loaded";
                }
            } catch (const std::invalid_argument &e) {
                info_text = e.what();
            }

            ImGui::Text(info_text.c_str());
            ImGui::End();
        }

        {
            static std::string last_exception{};

            ImGui::Begin("NES control");
            static bool running = false;
            try {
                if (ImGui::Button("Step")) {
                    nes.execute();
                }
                ImGui::SameLine();
                if (ImGui::Button("Step 10")) {
                    for (size_t i = 0; i < 10; ++i) {
                        nes.execute();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Step 100")) {
                    for (size_t i = 0; i < 100; ++i) {
                        nes.execute();
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Step 1000")) {
                    for (size_t i = 0; i < 1000; ++i) {
                        nes.execute();
                    }
                }

                ImGui::SameLine();
                if (!running) {
                    if (ImGui::Button("Run")) {
                        running = true;
                    }
                } else {
                    if (ImGui::Button("Stop")) {
                        running = false;
                    }
                }

                if (running) {
                    nes.execute();
                }
            } catch (const std::logic_error &e) {
                last_exception = e.what();
                running = false;
            }
            ImGui::Text(last_exception.c_str());
            ImGui::End();
        }

        {
            ImGui::Begin("CPU info");
            ImGui::Text("Cycle: %" PRIu64, nes.current_cycle());
            ImGui::Text("Registers:");
            ImGui::SameLine();
            ImGui::Text(to_string(nes.cpu_registers()).c_str());
            ImGui::End();
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
