#include "control_widget.h"
#include "info_widget.h"
#include "rom_widget.h"

#include <nes/core/immu.h>
#include <nes/core/ippu.h>
#include <nes/nes.h>

#include <fstream>
#include <iostream>

#include <nes/nes.h>
#include <imgui-SFML.h>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace {
sf::Color to_color(uint8_t color_index) {
    // Colours from blargg's full palette demo.
    switch (color_index) {
    case 1: return sf::Color(84, 84, 84);
    case 2: return sf::Color(0, 30, 116);
    case 3: return sf::Color(8, 16, 144);
    default: return sf::Color::Black;
    }
}

// johnor/n_e_s_vis
sf::Texture load_pattern(n_e_s::nes::Nes *nes, uint16_t pos, uint8_t pattern_table) {
    sf::Image image;
    image.create(8, 8, sf::Color(10, 100, 0));

    for (uint8_t row = 0; row < 8; ++row) {
        // Second pattern table starts at 0x1000
        const uint16_t base_address = pattern_table * 0x1000 + pos + row;
        const uint8_t a = nes->ppu_mmu().read_byte(base_address);
        const uint8_t b = nes->ppu_mmu().read_byte(base_address + 8);

        for (uint8_t col = 0; col < 8; ++col) {
            // First column is the leftmost bit.
            const uint16_t mask = 1u << (7 - col);
            const uint8_t color_index = !!(a & mask) + !!(b & mask);
            image.setPixel(col, row, to_color(color_index));
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

static constexpr uint16_t kPatternTableSize{256};
using PatternTable = std::array<sf::Texture, kPatternTableSize * 2>;
PatternTable load_pattern_table(n_e_s::nes::Nes *nes) {
    PatternTable patterns;

    for (uint16_t i = 0; i < kPatternTableSize; ++i) {
        patterns[i] = load_pattern(nes, i * 16, 0);
    }

    for (uint16_t i = 0; i < kPatternTableSize; ++i) {
        patterns[kPatternTableSize + i] = load_pattern(nes, i * 16, 1);
    }

    return patterns;
}
} // namespace

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(640, 480), "desunes");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    n_e_s::nes::Nes nes;

    bool running = false;
    PatternTable patterns{};
    RomWidget rom_widget(&nes);
    ControlWidget ctrl_widget(&nes, &running);
    InfoWidget info_widget(&nes);

    rom_widget.add_load_action([&] {
        patterns = load_pattern_table(&nes);
    });
  
    if (argc > 1) {
        std::ifstream rom_name(argv[1], std::ios::binary);
        nes.load_rom(rom_name);
        patterns = load_pattern_table(&nes);
    }

    while (window.isOpen()) {
        window.clear();
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

        try {
            const uint8_t pattern_table = 
                nes.ppu_registers().ctrl & 0x10 ? 1 : 0;
            for (uint16_t y = 0; y < 30; ++y) {
                for (uint16_t x = 0; x < 32; ++x) {
                    const uint16_t address = 0x2000 + y * 32 + x;
                    const uint8_t index = nes.ppu_mmu().read_byte(address);
                    const uint16_t actual_index = 
                        index + kPatternTableSize * pattern_table;
                    sf::Sprite sprite(patterns[actual_index]);
                    sprite.setScale(2, 2);
                    sprite.setPosition(sf::Vector2f(x * 16, y * 16));
                    window.draw(sprite);
                }
            }
        } catch (...) {
        }

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
