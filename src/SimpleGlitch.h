/*
  SimpleGlitchV2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2024 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.
*/

// #include "DDImage/Iop.h"
#include "DDImage/Iop.h"
#include "DDImage/NukeWrapper.h"
using namespace DD::Image;
#include "DDImage/Row.h"
#include "DDImage/Tile.h"
#include "DDImage/Knobs.h"
#include "DDImage/Format.h"

static const char *const CLASS = "SimpleGlitchV2";
static const char *const HELP = "";

class SimpleGlitchIop : public Iop
{
    float glitch_seed;
    float glitch_block_height;
    float glitch_intensity;
    float glitch_freq;
public:
    //constructor
    SimpleGlitchIop(Node *node);

    void knobs(Knob_Callback f);

    void _validate(bool);

    void _request(int x, int y, int r, int t, ChannelMask channels, int count);

    void engine(int y, int l, int r, ChannelMask channels, Row& row);

    static const Iop::Description d;

    const char *Class() const { return d.name; }
    const char *node_help() const { return HELP; }
};