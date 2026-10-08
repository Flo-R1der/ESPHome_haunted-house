#pragma once

#include "esphome.h"

namespace esphome {
namespace wandering_light_fx {

inline void color_wandering(
    light::AddressableLight *light,
    light::LightState *state,
    Color current_color,
    bool initial_run,
    int effect_length = 5000,
    bool reverse = false,
    float radius = 4.0f) {

    static uint32_t start_time = 0;

    // init
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

    // Protect against invalid radius values
    if (radius < 0.0f) {
        radius = 0.0f;
    }

    // Progress from 0.0 to 1.0
    const float progress =
        (float) elapsed / (float) effect_length;

    const float start_position =
        reverse
            ? (float) (size - 1) + radius
            : -radius;

    const float end_position =
        reverse
            ? -radius
            : (float) (size - 1) + radius;

    const float travel_distance =
        end_position - start_position;

    // Current position of the light center
    const float position =
        start_position + progress * travel_distance;

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
