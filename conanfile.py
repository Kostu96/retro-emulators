from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class RetroEmulatorsConan(ConanFile):
    name = "retro-emulators"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = ("gtest/[>1.17.0]",
                "sdl/3.2.20",
                "portable-file-dialogs/0.1.0")
    
    
    def layout(self):
        cmake_layout(self)
    
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.ctest(cli_args=["--output-on-failure"])
