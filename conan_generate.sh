#!/usr/bin/env bash

conan install . -s build_type=Debug --build=missing
conan install . -s build_type=Release --build=missing

cmake --preset conan-debug
cmake --preset conan-release
