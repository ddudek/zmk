/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/rgb_layer_status.h>
#include <zmk/rgb_underglow.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <init.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <dt-bindings/zmk/rgb.h>

#include <zmk/split/bluetooth/central.h>

#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/behavior_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/sensor_event.h>

void set_rgb_color_both_split(struct zmk_led_hsb color) {
    LOG_DBG("LAyERS invoke_layer_color_event h:%d, s:%d, b:%d", color.h, color.s, color.b);

    // invoke locally
    zmk_rgb_underglow_set_hsb(color);

    // invoke slave
    struct zmk_behavior_binding binding = {
            .param1 = RGB_COLOR_HSB_CMD,
            .param2 = RGB_COLOR_HSB_VAL(color.h, color.s, color.b),
            .behavior_dev = "RGB_UG",
        };

    struct zmk_behavior_binding_event event = {
        .layer = 0, // not used?
        .position = 0, // not used?
        .timestamp = k_uptime_get(),
    };
    bool pressed = true;
    uint8_t source = 0;
    zmk_split_bt_invoke_behavior(source, &binding, event, pressed);
}


void set_rgb_enabled_both_split(bool on) {
    // invoke locally
    if(on) {
        zmk_rgb_underglow_on();
    } else {
        zmk_rgb_underglow_off();
    }

    // invoke slave
    struct zmk_behavior_binding binding = {
            .param1 = on?RGB_ON_CMD:RGB_OFF_CMD,
            .param2 = 0,
            .behavior_dev = "RGB_UG",
        };

    struct zmk_behavior_binding_event event = {
        .layer = 0, // not used?
        .position = 0, // not used?
        .timestamp = k_uptime_get(),
    };
    bool pressed = true;
    uint8_t source = 0;
    zmk_split_bt_invoke_behavior(source, &binding, event, pressed);
}

void set_layer_color()
{
    int active_layer_index = zmk_keymap_highest_layer_active();
    LOG_DBG("Layer changed to %i", active_layer_index);

    struct zmk_led_hsb color;
    color.s = 100;
    color.b = 100;

    switch (active_layer_index)
    {
        case  0 : { color.h = 0; color.s = 0; color.b =  0; break; }
        case  1 : { color.h = 28; color.s = 100; color.b =  15; break; }
        case  2 : { color.h = 175; color.s = 80; color.b =  15; break; }
        case  3 : { color.h = 0; color.s = 100; color.b =  15; break; }
        case  4 : { color.h = 340; color.s = 90; color.b =  15; break; }
        default : { color.h = 0; color.s = 0; color.b =  1; break; }
    }

    if (color.b == 0)
    {
        set_rgb_enabled_both_split(false);
    }
    else
    {
        //zmk_rgb_underglow_set_hsb(color);
        set_rgb_color_both_split(color);
        set_rgb_enabled_both_split(true);
    }
}

static int zmk_rgb_layer_status_init(const struct device* _arg)
{
    zmk_rgb_underglow_off();
    return 0;
}

int layer_status_listener(const zmk_event_t* eh)
{
    struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
    #ifdef CONFIG_ZMK_SPLIT_BLE_ROLE_CENTRAL 
    set_layer_color();
    #endif
    return 0;
}

ZMK_LISTENER(rgb_layer_status, layer_status_listener)
ZMK_SUBSCRIPTION(rgb_layer_status, zmk_layer_state_changed);

SYS_INIT(zmk_rgb_layer_status_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);