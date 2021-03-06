from conans import ConanFile, CMake

class ConanMgsCodecsBase(ConanFile):
    name = "mgs_codecs"
    version = "0.1.0"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "test/*"
    settings = "os", "arch", "build_type", "compiler"

    def build_requirements(self):
        self.build_requires("mgs_cmake/%s" % self.version)
        if self.develop:
            self.build_requires("catch2/2.13.6")

    def requirements(self):
        self.requires("mgs_meta/%s" % self.version)
        self.requires("mgs_exceptions/%s" % self.version)
        self.requires("mgs_config/%s" % self.version)

    def build(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTING"] = "OFF"

        cmake.configure()
        cmake.build()
        cmake.install()

    def package_id(self):
        self.info.header_only()
