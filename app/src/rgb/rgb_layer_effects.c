/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/rgb_layer_effects.h>
#include <zmk/rgb_common.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <init.h>
#include <zmk/keymap.h>
#include <zmk/behavior.h>
#include <dt-bindings/zmk/rgb.h>

#include <zmk/split/bluetooth/central.h>

#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/sensor_event.h>

#define STRIP_NUM_PIXELS DT_PROP(DT_CHOSEN(zmk_underglow), chain_length)

#define BRT_MAX 100

void layer_to_rgb_matrix(int active_layer_index, struct zmk_led_hsb* color_matrix) {
    struct zmk_led_hsb color = zmk_rgb_underglow_get_color();
    float brightness = color.b / ((float)BRT_MAX);

    switch (active_layer_index)
    {
        case  0 : { break; }
        case  1 : { color.h = 28; color.s = 100; color.b =  5; break; }
        case  2 : { color.h = 175; color.s = 80; color.b =  5; break; }
        case  3 : { color.h = 0; color.s = 100; color.b =  5; break; }
        case  4 : { color.h = 340; color.s = 90; color.b =  5; break; }
        case  5 : { color.h = 110; color.s = 75; color.b =  5; break; }
        //default : { color.h = 0; color.s = 0; color.b =  0; break; }
    }

    for (int i = 0; i < STRIP_NUM_PIXELS; i++)
    {
        color_matrix[i] = color;
    }

    switch (active_layer_index)
    {
        case  0 : { break; }
        case  1 : { // 
            // esdf
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[12].b = 70;
            color_matrix[14].b = 70;
            break;
        }
        case  2 : { 
            // home row numbers
            color_matrix[5].b = 70;
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[14].b = 70;
            color_matrix[17].b = 70;
            break;
            }
        case  3 : {
            // esdf
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[12].b = 70;
            color_matrix[14].b = 70;
            break;
            }
        case  4 : {
            // bt 1,2,3
            color_matrix[18].b = 70; color_matrix[18].h = 175; color_matrix[18].s = 100;
            color_matrix[13].b = 70; color_matrix[13].h = 175; color_matrix[13].s = 100;
            color_matrix[12].b = 70; color_matrix[12].h = 175; color_matrix[12].s = 100;
            color_matrix[7].b = 70;  color_matrix[7].h = 175;  color_matrix[7].s = 100;
            color_matrix[6].b = 80;  color_matrix[6].h = 175;  color_matrix[6].s = 100;

            // br + -
            color_matrix[5].b = 70; color_matrix[5].s = 0;
            color_matrix[4].b = 70; color_matrix[4].s = 0;
            // hue
            color_matrix[8].b = 70; color_matrix[8].s = 20; color_matrix[11].h = 359;
            color_matrix[9].b = 70; color_matrix[9].s = 20; color_matrix[10].h = 359;
            // sat + -
            color_matrix[10].b = 70; color_matrix[10].s = 0; //color_matrix[14].h = 359;
            color_matrix[11].b = 70; color_matrix[11].s = 0; //color_matrix[15].h = 359;
            break; 
        }
        case  5 : {
            // Z zen
            color_matrix[16].b = 70;

            // R run
            color_matrix[7].b = 70;

            // space
            color_matrix[2].b = 70;
            break;
            }
        default : { }
    }
}

void layer_to_rgb_matrix_slave(int active_layer_index, struct zmk_led_hsb* color_matrix) {
    struct zmk_led_hsb color = zmk_rgb_underglow_get_color();
    float brightness = color.b / ((float)BRT_MAX);

    switch (active_layer_index)
    {
        case  0 : { break; }
        case  1 : { color.h = 28; color.s = 100; color.b =  6; break; }
        case  2 : { color.h = 175; color.s = 80; color.b =  6; break; }
        case  3 : { color.h = 0; color.s = 100; color.b =  6; break; }
        case  4 : { color.h = 340; color.s = 90; color.b =  6; break; }
        case  5 : { color.h = 110; color.s = 75; color.b = brightness * 50; break; }
        //default : { color.h = 0; color.s = 0; color.b =  0; break; }
    }

    for (int i = 0; i < STRIP_NUM_PIXELS; i++)
    {
        color_matrix[i] = color;
    }

    switch (active_layer_index)
    {
        case  0 : { break; }
        case  1 : { 
            // esdf
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[12].b = 70;
            color_matrix[14].b = 70;
            break;
        }
        case  2 : { 
            // home row numbers
            color_matrix[5].b = 70;
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[14].b = 70;
            color_matrix[17].b = 70;
            break;
            }
        case  3 : {
            // esdf
            color_matrix[8].b = 70;
            color_matrix[11].b = 70;
            color_matrix[12].b = 70;
            color_matrix[14].b = 70;
            break;
            }
        case  4 : {
            // vol up
            color_matrix[18].b = 70; color_matrix[18].s = 0; //color_matrix[14].h = 359;
            // vol down
            color_matrix[17].b = 70; color_matrix[17].s = 0; //color_matrix[15].h = 359;
            break; 
        }
        default : { }
    }
}

void sned_rgb_layer_effect_to_slave(int layer) {
    // invoke slave
    struct zmk_behavior_binding binding = {
            .param1 = RGB_LAYER_EFF_CMD,
            .param2 = layer,
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

static int zmk_rgb_layer_status_init(const struct device* _arg)
{
    zmk_rgb_underglow_off();
    return 0;
}


int update_layer_rgb()
{
    #ifdef CONFIG_ZMK_SPLIT_BLE_ROLE_CENTRAL 
    // get current layer
    int active_layer_index = zmk_keymap_highest_layer_active();
    LOG_DBG("Layer changed to %i", active_layer_index);

    // create matrix
    struct zmk_led_hsb color_matrix[STRIP_NUM_PIXELS];
    layer_to_rgb_matrix(active_layer_index, color_matrix);

    // set matrix locally for central
    zmk_rgb_underglow_set_hsb_matrix(color_matrix);

    // send layer change to split 
    sned_rgb_layer_effect_to_slave(active_layer_index);
    #endif
    return 0;
}

int layer_status_listener(const zmk_event_t* eh)
{
    struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);
    #ifdef CONFIG_ZMK_SPLIT_BLE_ROLE_CENTRAL 
    update_layer_rgb();
    #endif
    return 0;
}

// global for handling split
int rgb_set_layer_effect(int active_layer_index) {
    struct zmk_led_hsb color_matrix[STRIP_NUM_PIXELS];
    layer_to_rgb_matrix_slave(active_layer_index, color_matrix); // TODO: split for left/right half
    zmk_rgb_underglow_set_hsb_matrix(color_matrix);
    return 0;
}

ZMK_LISTENER(rgb_layer_status, layer_status_listener)
ZMK_SUBSCRIPTION(rgb_layer_status, zmk_layer_state_changed);

SYS_INIT(zmk_rgb_layer_status_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);