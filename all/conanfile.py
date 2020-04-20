from conans import ConanFile, CMake

class ConanMgs(ConanFile):
    name = "mgs"
    version = "0.1.0"
    generators = "cmake"

    def requirements(self):
        self.requires("mgs_base16/%s" % self.version)
        self.requires("mgs_base32/%s" % self.version)
        self.requires("mgs_base32hex/%s" % self.version)
        self.requires("mgs_base64/%s" % self.version)
        self.requires("mgs_base64url/%s" % self.version)
        self.requires("mgs_base_n/%s" % self.version)
        self.requires("mgs_meta/%s" % self.version)
        self.requires("mgs_exceptions/%s" % self.version)
        self.requires("mgs_codecs/%s" % self.version)
        self.requires("mgs_config/%s" % self.version)
