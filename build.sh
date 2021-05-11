#!/usr/bin/env sh
nix run nixpkgs.clang_11 -c clang++ --std=c++14 -Wall -Wextra -Werror -Wno-unused-const-variable format.cpp
