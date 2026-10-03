#pragma once

#include "esphome.h"

namespace esphome {
namespace wandering_light_fx {

inline void color_wandering(
    light::AddressableLight *light,
    light::LightState *state,
    Color current_color,
    bool initial_run,
    uint32_t effect_length) {

    static uint32_t start_time = 0;

    if (initial_run) {
        start_time = millis();
        light->all() = Color::BLACK;
        light->schedule_show();
        return;
    }

    uint32_t elapsed = millis() - start_time;

    // Effect finished
    if (elapsed >= effect_length) {
        light->all() = Color::BLACK;
        light->schedule_show();

        auto call = state->turn_off();
        call.set_transition_length(0);
        call.perform();

        return;
    }

    const int size = light->size();

    if (size <= 0) {
        return;
    }

    // Progress from 0.0 to 1.0
    const float progress =
        (float) elapsed / (float) effect_length;

    // Current position of the light center
    const float position =
        progress * (float) (size - 1);

    // Radius of the light glow in LEDs.
    // At radius 4:
    // center = 100%
    // 1 LED away = 75%
    // 2 LEDs away = 50%
    // 3 LEDs away = 25%
    // 4 LEDs away = 0%
    const float radius = 4.0f;

    for (int i = 0; i < size; i++) {

        const float distance =
            fabsf(position - (float) i);

        if (distance < radius) {

            // Symmetrical brightness ramp:
            // 0% -> 100% -> 0%
            const float intensity =
                1.0f - (distance / radius);

            const uint8_t scale =
                (uint8_t) (intensity * 255.0f);

            light->get(i) = current_color * scale;

        } else {
            light->get(i) = Color::BLACK;
        }
    }

    light->schedule_show();
}

}  // namespace wandering_light_fx
}  // namespace esphome
