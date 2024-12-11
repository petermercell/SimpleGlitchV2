/*
  SimpleGlitchV2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2024 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.

    TODO: Controlar el tamaño en x de cada bloque.

*/

#include "SimpleGlitch.h"
#include <cmath>
#include <algorithm>

// Macros
constexpr int BBOX_SIZE = 1;
constexpr float NOISE_MULT = 2.0f;

inline float fract(float x)
{
    return x - std::floor(x);
}

// Random Noise
inline float randomNoise(int y, float noise_seed)
{
    return fract(std::sin((float)y * 12.9898f + noise_seed) * 43758.5453f);
}

// block index
inline auto blockIndex(int current, int final_pos, int size)
{
    return (current - final_pos) / size;
}

SimpleGlitchIop::SimpleGlitchIop(Node *node) : Iop(node)
{
    noise_seed = 235.0f;
    noise_height = 4.0f;
    noise_intensity = 2.0f;
    _bbox = 1;
    noise_freq = 1.0f;
    noise_offset = 0.5f;
    solo_effect = false;
}

void SimpleGlitchIop::knobs(Knob_Callback f)
{
    Float_knob(f, &noise_seed, "seed");
    Tooltip(f, "Random seed for the glitch pattern.");
    SetRange(f, 1, 1000);
    Bool_knob(f, &solo_effect, "effect only");
    Tooltip(f, "Outputs only the glitch effect.");
    Float_knob(f, &noise_height, "noise_height", "noise height");
    Tooltip(f, "Height in lines for each glitch block.");
    SetRange(f, 1, 20);
    Float_knob(f, &noise_intensity, "intensity");
    Tooltip(f, "Intensity of the horizontal displacement.");
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
    _bbox = (int)noise_intensity;

    copy_info();
    info_.pad(_bbox);
    set_out_channels(Mask_All);
}

void SimpleGlitchIop::_request(int x, int y, int r, int t, ChannelMask channels, int count)
{
    _bbox = (int)noise_intensity;
    x -= _bbox;
    r += _bbox;
    y -= _bbox;
    t += _bbox;
    input(0)->request(x, y, r, t, channels, count);
}

void SimpleGlitchIop::engine(int y, int x, int r, ChannelMask channels, Row &out)
{
    const Box imageFormat = info().format();
    const int im_width = imageFormat.w();
    const int im_height = imageFormat.h();
    const int im_y = imageFormat.y();
    const int im_x = imageFormat.x();

    // make a tile for the current row
    _bbox = (int)noise_intensity;
    Tile tile(input0(), x - _bbox, y - _bbox, r + _bbox, y + _bbox, channels);

    if (aborted())
    {
        return;
    }
    // Set the size of the glitch block in y
    int block_index = blockIndex(y, im_y, noise_height);
    // Set the Noise
    float lineNoise = randomNoise(block_index, noise_seed);

    // Get original pixel values from the current row
    Row in(x, r);
    in.get(input0(), y, x, r, channels);

    // Iterate each Channel
    foreach (z, channels)
    {
        const float *inptr = in[z];
        float *outptr = out.writable(z);

        // Noise Frequency (threshold)
        if (lineNoise < noise_freq)
        {
            int offset = (int)((lineNoise - noise_offset) * noise_intensity * NOISE_MULT); // block offset

            for (int X = x; X < r; X++)
            {
                int newX = tile.clampx(X + offset); // avoid weird pixel values
                float row_off = newX == X ? 0.0f : inptr[newX];
                outptr[X] = solo_effect ? row_off : inptr[newX];
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