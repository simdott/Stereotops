/*
 * Stereotops LV2 Plugin - Simple stereo width plugin
 * Copyright (C) 2026 Simon Delaruotte
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include <lv2/core/lv2.h>
#include <math.h>
#include <stdlib.h>

#define STEREOTOPS_URI "urn:simdott:stereotops"

typedef enum {
    INPUT_LEFT = 0,
    INPUT_RIGHT = 1,
    OUTPUT_LEFT = 2,
    OUTPUT_RIGHT = 3,
    WIDTH = 4
} PortIndex;

typedef struct {
    const float* input_left;
    const float* input_right;
    const float* width;
    float* output_left;
    float* output_right;
    float prev_width;
} Stereotops;

static LV2_Handle instantiate(
    const LV2_Descriptor* descriptor,
    double sample_rate,
    const char* bundle_path,
    const LV2_Feature* const* features) {
    
    Stereotops* self = (Stereotops*)malloc(sizeof(Stereotops));
    if (!self) return NULL;
    
    self->prev_width = 0.0f;
    
    return (LV2_Handle)self;
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data) {
    Stereotops* self = (Stereotops*)instance;
    
    switch ((PortIndex)port) {
        case INPUT_LEFT: 
            self->input_left = (const float*)data; 
            break;
        case INPUT_RIGHT: 
            self->input_right = (const float*)data; 
            break;
        case OUTPUT_LEFT: 
            self->output_left = (float*)data; 
            break;
        case OUTPUT_RIGHT: 
            self->output_right = (float*)data; 
            break;
        case WIDTH:
            self->width = (const float*)data;
            break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples) {
    Stereotops* self = (Stereotops*)instance;
    
    float current_width = *(self->width);
    float width_factor = current_width / 100.0f;
    
    if (fabsf(width_factor - self->prev_width) < 0.0001f &&
        fabsf(width_factor) < 0.0001f) {
        for (uint32_t i = 0; i < n_samples; i++) {
            self->output_left[i] = self->input_left[i];
            self->output_right[i] = self->input_right[i];
        }
        return;
    }
    
    self->prev_width = width_factor;
    
    float mid_gain, side_gain;
    
    if (width_factor < 0.0f) {
        float mono_factor = 1.0f + width_factor;
        mid_gain = sqrtf(2.0f / (1.0f + mono_factor * mono_factor));
        side_gain = mid_gain * mono_factor;
    } else {
        float width_scaled = 1.0f + width_factor;
        mid_gain = sqrtf(2.0f / (1.0f + width_scaled * width_scaled));
        side_gain = mid_gain * width_scaled;
    }
    
    for (uint32_t i = 0; i < n_samples; i++) {
        float mid = (self->input_left[i] + self->input_right[i]) * 0.5f;
        float side = (self->input_left[i] - self->input_right[i]) * 0.5f;
        
        float mid_scaled = mid * mid_gain;
        float side_scaled = side * side_gain;
        
        self->output_left[i] = mid_scaled + side_scaled;
        self->output_right[i] = mid_scaled - side_scaled;
    }
}

static void cleanup(LV2_Handle instance) {
    free(instance);
}

static const LV2_Descriptor descriptor = {
    STEREOTOPS_URI,
    instantiate,
    connect_port,
    NULL,
    run,
    NULL,
    cleanup,
    NULL
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index) {
    return (index == 0) ? &descriptor : NULL;
}
