from conans import ConanFile, CMake

class ConanMgsCMake(ConanFile):
    name = "mgs_cmake"
    version = "0.1.0"
    exports_sources = "*.cmake*"

    def package(self):
        self.copy("*.cmake*")

