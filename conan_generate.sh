#!/usr/bin/env bash

conan install . --settings=build_type=Debug --build=missing
conan install . --settings=build_type=Release --build=missing

cmake --preset conan-debug
cmake --preset conan-release
