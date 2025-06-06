/*
  SimpleGlitchV2 Plugin for Nuke
  ------------------------------
  Copyright (c) 2024 Gonzalo Rojas
  This plugin is free to use, modify, and distribute.
  Provided "as is" without any warranty.
*/

#ifndef SIMPLEGLITCH_H
#define SIMPLEGLITCH_H

#include "DDImage/Iop.h"
#include "DDImage/NukeWrapper.h"

using namespace DD::Image;
#include "DDImage/Format.h"
#include "DDImage/Knobs.h"
#include "DDImage/Row.h"
#include "DDImage/Tile.h"

static const char *const CLASS = "SimpleGlitch2";
static const char *const HELP =
    "This node applies a horizontal glitch effect to the image lines.";

class SimpleGlitchIop : public Iop {
  float noise_seed;
  float noise_height;
  float noise_intensity;
  float noise_freq;
  float noise_offset;
  bool solo_effect;
  int _bbox;

public:
  // constructor
  SimpleGlitchIop(Node *node);

  void knobs(Knob_Callback f);

  void _validate(bool);

  void _request(int x, int y, int r, int t, ChannelMask channels, int count);

  void engine(int y, int l, int r, ChannelMask channels, Row &row);

  static const Iop::Description d;

  const char *Class() const { return d.name; }
  const char *node_help() const { return HELP; }
};

#endif // SIMPLEGLITCH_H