#include "control_widget.h"
#include "info_widget.h"
#include "keyboard_controller.h"
#include "rom_widget.h"

#include <nes/core/immu.h>
#include <nes/core/invalid_address.h>
#include <nes/core/ippu.h>
#include <nes/nes.h>
#include <imgui-SFML.h>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "desunes");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    n_e_s::nes::Nes nes;

    bool running = false;
    int step_running = 0;
    bool rom_loaded = false;
    std::string run_status = "Stopped";
    std::string err_status = "";

    RomWidget rom_widget(&nes);
    ControlWidget ctrl_widget(&nes, &running, &step_running);
    InfoWidget info_widget(&nes, &run_status, &err_status);

    KeyboardController keyboard_controller1(nes.controller1());
    keyboard_controller1.map_key(
        sf::Keyboard::Key::A, n_e_s::core::INesController::Button::A);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::Z, n_e_s::core::INesController::Button::B);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::S, n_e_s::core::INesController::Button::Select);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::X, n_e_s::core::INesController::Button::Start);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::Up, n_e_s::core::INesController::Button::Up);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::Down, n_e_s::core::INesController::Button::Down);
    keyboard_controller1.map_key(
        sf::Keyboard::Key::Left, n_e_s::core::INesController::Button::Left);
    keyboard_controller1.map_key(sf::Keyboard::Key::Right,
        n_e_s::core::INesController::Button::Right);

    rom_widget.add_load_action([&] {
        rom_loaded = true;
    });

    if (argc > 1) {
        std::ifstream rom{argv[1], std::ios::binary};
        nes.load_rom(rom);
        rom_loaded = true;
    }

    sf::Image screen_image;
    screen_image.create(256, 240, sf::Color(10, 100, 0));

    while (window.isOpen()) {
        window.clear();
        deltaClock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::KeyPressed: {
                    keyboard_controller1.set_key(event.key.code, true);
                    break;
                }
                case sf::Event::KeyReleased: {
                    keyboard_controller1.set_key(event.key.code, false);
                    break;
                }
                default:
                    break;
            }
        }

        ImGui::SFML::Update(window, deltaClock.getElapsedTime());

        if (running) {
            run_status = "Running";
            err_status = "";
        } else if (step_running > 0) {
            run_status = std::string("Running (step: ") + 
                         std::to_string(step_running) + 
                         std::string(")");
            err_status = "";
        } else {
            run_status = "Stopped";
        }

        rom_widget.update();
        ctrl_widget.update();
        info_widget.update();

        sf::Texture texture;
        texture.loadFromImage(screen_image);
        sf::Sprite sprite(texture);
        sprite.setScale(2, 2);
        sprite.setPosition(sf::Vector2f(0, 0));
        window.draw(sprite);

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
        try {
            if (running || step_running > 0) {
                for (size_t i = 0; i < tick_per_frame; ++i) {
                    auto pixel = nes.execute();

                    if (pixel.has_value()) {
                        screen_image.setPixel(
                            pixel->x,
                            pixel->y,
                            sf::Color(pixel->color.r,
                                      pixel->color.g,
                                      pixel->color.b));
                    }

                    if (step_running > 0) {
                        step_running -= 1;
                    }
                    if (!running && step_running == 0) {
                        break;
                    }
                }
            }
        } catch (const std::invalid_argument & e) {
            std::cerr << e.what() << std::endl;
            err_status = e.what();
            running = false;
            step_running = 0;
        } catch (const std::logic_error &e) {
            std::cerr << e.what() << std::endl;
            err_status = e.what();
            running = false;
            step_running = 0;
        } catch (const n_e_s::core::InvalidAddress &e) {
            std::cerr << e.what() << std::endl;
            err_status = e.what();
            running = false;
            step_running = 0;
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
