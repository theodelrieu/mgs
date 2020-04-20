from conans import ConanFile, CMake

class ConanMgsExceptions(ConanFile):
    name = "mgs_exceptions"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt"
    settings = "os", "build_type", "arch", "compiler"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s" % self.version)

    def requirements(self):
        self.requires("mgs_config/%s" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"

        cmake.configure()
        cmake.build()
        cmake.install()

    def package_id(self):
        self.info.header_only()
