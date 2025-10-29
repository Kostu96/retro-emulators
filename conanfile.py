from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class RetroEmulatorsConan(ConanFile):
    name = "retro-emulators"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    
    def requirements(self):
        self.requires("gtest/[>1.17.0]")
        self.requires("portable-file-dialogs/0.1.0")
        self.requires("glfw/3.4")

    
    def layout(self):
        cmake_layout(self, generator="Ninja Multi-Config")
    
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.ctest(cli_args=["--output-on-failure"])
