from conans import ConanFile, CMake, tools


class AntigrainConan(ConanFile):
    name = "antigrain"
    version = "2.6"
    license = "MIT"
    author = "Maxim Shemanarev"
    url = "https://github.com/ghaerr/agg-2.6"
    description = "Open Source, 2D vector graphics library"
    topics = ("vector graphics")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    requires = ["expat/2.2.10","freetype/2.10.4","xorg/system"]
    exports_sources = "include/*", "src/*", "myapp/*", "examples/*", "font_freetype/*", "gpc/*", "font_win32_tt/*", "agg2d/*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("copying", dst="licence")
        self.copy("README.md")
        self.copy("*.h", dst="include", src="include", keep_path=True)
        self.copy("*.hpp", dst="include", src="include", keep_path=True)
        self.copy("*antigrain.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["antigrain"]

