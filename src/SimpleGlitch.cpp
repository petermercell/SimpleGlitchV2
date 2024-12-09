/*
  SimpleGlitchV2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2024 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.

  TODO: poner en 0.0f todos los pixeles que esten fuera del bloque con offset
*/

#include "SimpleGlitch.h"
#include <cmath>
#include <algorithm>

// Macros
constexpr int BBOX_SIZE = 1;

inline float fract(float x)
{
    return x - std::floor(x);
}

// Random Noise
inline float randomNoise(int y, float noise_seed)
{
    return fract(std::sin((float)y * 12.9898f + noise_seed) * 43758.5453f);
}

SimpleGlitchIop::SimpleGlitchIop(Node *node) : Iop(node)
{
    noise_seed = 235.0f;
    noise_height = 4.0f;
    noise_intensity = 2.0f;
    noise_freq = 1.0f;
    noise_offset = 0.5f;
    noise_mult = 2.0f;
}

void SimpleGlitchIop::knobs(Knob_Callback f)
{
    Float_knob(f, &noise_seed, "seed");
    Tooltip(f, "Random seed for the glitch pattern.");
    SetRange(f, 1, 1000);
    Float_knob(f, &noise_height, "noise_height", "noise height");
    Tooltip(f, "Height in lines for each glitch block.");
    SetRange(f, 1, 20);
    Float_knob(f, &noise_intensity, "intensity");
    Tooltip(f, "Intensity of the horizontal displacement.");
    SetRange(f, 1, 10);
    Float_knob(f, &noise_mult, "multiply");
    Tooltip(f, "Intensity Multiplier.");
    SetRange(f, 1, 10);
    Float_knob(f, &noise_freq, "frequency");
    Tooltip(f, "Frequency at which glitch lines occur.");
    SetRange(f, 0, 1);
    Float_knob(f, &noise_offset, "offset");
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
    input0().request(x - BBOX_SIZE, y - BBOX_SIZE, r + BBOX_SIZE, t + BBOX_SIZE, channels, count);
}

void SimpleGlitchIop::engine(int y, int x, int r, ChannelMask channels, Row &out)
{
    // make a tile for the current row
    Tile tile(input0(), x - BBOX_SIZE, y - BBOX_SIZE, r + BBOX_SIZE, y + BBOX_SIZE, channels);
    if (aborted())
    {
        std::cerr << "Aborted!";
        return;
    }

    // Get original pixel values from the current row
    Row in(x, r);
    in.get(input0(), y, x, r, channels);

    // Set the size of the glitch block in y
    int blockIndex = y / noise_height;
    // Set the Noise
    float lineNoise = randomNoise(blockIndex, noise_seed);

    // Iterate each Channel
    foreach (z, channels)
    {
        const float *inptr = in[z];
        float *outptr = out.writable(z);

        // Noise Frequency (threshold)
        if (lineNoise < noise_freq)
        {
            int offset = (int)((lineNoise - noise_offset) * noise_intensity * noise_mult); // block offset

            for (int X = x; X < r; X++)
            {
                int newX = tile.clampx(X + offset);
                outptr[X] = inptr[newX];
            }
        }
        else
        {
            std::copy(in[z] + x, in[z] + r, out.writable(z) + x); // just copy the same pixel values to the current row (No pixel changes)
        }
    }
}

static Iop *build(Node *node) { return new NukeWrapper(new SimpleGlitchIop(node)); }
const Iop::Description SimpleGlitchIop::d("SimpleGlitch2", "Filter/SimpleGlitch2", build);