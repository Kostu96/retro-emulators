@echo off

conan install . -s build_type=Debug --build=missing
conan install . -s build_type=Release --build=missing

cmake --preset conan-default
