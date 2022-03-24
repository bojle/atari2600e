#!/usr/bin/bash

base_dir="$HOME/dev/atari2600e"
build_dir="$base_dir/build"
src_dir="$base_dir/src"
cmake --build "$build_dir" --target a "$src_dir"
