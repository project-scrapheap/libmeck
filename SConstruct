
import os
import re
import subprocess

AddOption(
	"--optimize",
	dest="optimize",
	default="2",
	type="string",
	nargs=1,
	action="store",
	metavar="FLAG",
	help="optimization, one of {0, 1, 2, 3, g}"
)

def GetOptimize():
	optimize = GetOption("optimize")
	optimizations = {
		"0": "-O0 -ggdb",
		"1": "-O1",
		"2": "-O2",
		"3": "-O3",
		"g": "-Og -ggdb"
	}
	if not optimize in optimizations:
		raise ValueError("invalid optimization flag given")
	return optimizations[optimize]

env = Environment(
	CCFLAGS=" ".join([
		"-Wall",
		"-Wextra",
		"-std=c++14",
		"-D_FILE_OFFSET_BITS=64",
		"-DCATCH_CONFIG_RUNNER",
		GetOptimize()
	])
)

packages = [
	"sdl2",
	"SDL2_image",
	"SDL2_ttf",
	"SDL2_mixer"
]

env.Append(LIBPATH=[
	"#/"
])

env.Append(CPPPATH=[
	"#/include/"
])

env.Append(LIBS=[
	"m",
	"rt",
	"pthread",
	"meck",
	"boost_filesystem",
	"boost_program_options",
	"boost_regex",
	"boost_system",
	"boost_thread"
])

env.StaticLibrary("meck", [
	"source/application.cpp",
	"source/controller.cpp",
	"source/font.cpp",
	"source/keycode.cpp",
	"source/point.cpp",
	"source/rect.cpp",
	"source/renderer.cpp",
	"source/screenshot.cpp",
	"source/surface.cpp",
	"source/texture.cpp",
	"source/typeid.cpp",
	"source/window.cpp",
	
	"source/ui/ascii_textbox.cpp",
	"source/ui/block.cpp",
	"source/ui/button.cpp",
	"source/ui/checkbox.cpp",
	"source/ui/container.cpp",
	"source/ui/image.cpp",
	"source/ui/label.cpp",
	"source/ui/overlay.cpp",
	"source/ui/text.cpp",
	"source/ui/theme.cpp",
	"source/ui/utf8_textbox.cpp",
	"source/ui/validator.cpp",
	"source/ui/widget.cpp"
])

for pkg in packages:
	env.ParseConfig("pkg-config --cflags --libs {0}".format(pkg))

env.Program("test/test_app", [
	"test/test_app.cpp",
	"test/test_app_configure.cpp",
	"test/test_app_font.cpp",
	"test/test_app_keycode.cpp",
	"test/test_app_renderer.cpp",
	"test/test_app_ui_block.cpp",
	"test/test_app_ui_theme.cpp"
])

