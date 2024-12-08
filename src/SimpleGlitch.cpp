/*
  SimpleGlitchV2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2024 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.
*/

#include "SimpleGlitch.h"
#include <cmath>
#include <algorithm>

inline float fract(float x)
{
    return x - std::floor(x);
}

inline float randFromY(int y, float glitch_seed)
{
    return fract(std::sin((float)y * 12.9898f + glitch_seed) * 43758.5453f);
}

SimpleGlitchIop::SimpleGlitchIop(Node *node) : Iop(node)
{
    glitch_seed = 235.0f;
    glitch_block_height = 4.0f;
    glitch_intensity = 2.0f;
    glitch_freq = 1.0f;
    glitch_blocks_offset = 0.5f;
    glitch_intensity_mult = 2.0f;
}

void SimpleGlitchIop::knobs(Knob_Callback f)
{
    Float_knob(f, &glitch_seed, "seed");
    Tooltip(f, "Random seed for the glitch pattern.");
    SetRange(f, 1, 1000);
    Float_knob(f, &glitch_block_height, "block_height", "block height");
    Tooltip(f, "Height in lines for each glitch block.");
    SetRange(f, 1, 20);
    Float_knob(f, &glitch_intensity, "intensity");
    Tooltip(f, "Intensity of the horizontal displacement.");
    SetRange(f, 1, 10);
    Float_knob(f, &glitch_intensity_mult, "multiplier");
    Tooltip(f, "Intensity Multiplier.");
    SetRange(f, 1, 10);
    Float_knob(f, &glitch_freq, "frequency");
    Tooltip(f, "Frequency at which glitch lines occur.");
    SetRange(f, 0, 1);
    Float_knob(f, &glitch_blocks_offset, "offset");
    Tooltip(f, "General image Offset.");
    SetRange(f, 0, 1);
}

void SimpleGlitchIop::_validate(bool for_real)
{
    copy_info();
    set_out_channels(Mask_All);
}

void SimpleGlitchIop::_request(int x, int y, int r, int t, ChannelMask channels, int count)
{
    input0().request(x, y, r, t, channels, count);
}

void SimpleGlitchIop::engine(int y, int x, int r, ChannelMask channels, Row& out)
{
    Row in(x, r);
    in.get(input0(), y, x, r, channels);

    const int width = input0().info().w();
    int blockIndex = y / glitch_block_height;
    float lineNoise = randFromY(blockIndex, glitch_seed);

    foreach (z, channels)
    {
        const float *inptr = in[z];
        float *outptr = out.writable(z);

        if (lineNoise < glitch_freq)
        {
            float noiseVal = randFromY(blockIndex, glitch_seed);
            // int offset = (int)((noiseVal - 0.5f) * glitch_intensity * 2.0f);
            int offset = (int)((noiseVal - glitch_blocks_offset) * glitch_intensity * glitch_intensity_mult);

            for (int X = x; X < r; X++)
            {
                int newX = std::clamp(X + offset, 0, width - 1);
                outptr[X] = inptr[newX];
            }
        }
        else
        {
            std::copy(in[z] + x, in[z] + r, out.writable(z) + x);
        }
    }
}

static Iop *build(Node *node) { return new NukeWrapper(new SimpleGlitchIop(node)); }
const Iop::Description SimpleGlitchIop::d("SimpleGlitch2", "Filter/SimpleGlitch2", build);