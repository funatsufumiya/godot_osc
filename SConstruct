#!/usr/bin/env python
from glob import glob
from pathlib import Path

# define tests
AddOption(
    "--tests",
    dest="tests",
    default="no",
    help="Enable tests (yes/no).",
)

# TODO: Do not copy environment after godot-cpp/test is updated <https://github.com/godotengine/godot-cpp/blob/master/test/SConstruct>.
env = SConscript("godot-cpp/SConstruct")

# Add source files.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

# # Add lz4 source files.
# env.Append(CPPPATH=["lz4/lib/"])
# sources += Glob("lz4/lib/*.c")

# Find gdextension path even if the directory or extension is renamed (e.g. project/addons/example/example.gdextension).
(extension_path,) = glob("project/addons/*/*.gdextension")

# Find the addon path (e.g. project/addons/example).
addon_path = Path(extension_path).parent

# Find the project name from the gdextension file (e.g. example).
project_name = Path(extension_path).stem

# TODO: Cache is disabled currently.
# scons_cache_path = os.environ.get("SCONS_CACHE")
# if scons_cache_path != None:
#     CacheDir(scons_cache_path)
#     print("Scons cache enabled... (path: '" + scons_cache_path + "')")

# Create the library target (e.g. libexample.linux.debug.x86_64.so).
debug_or_release = "release" if env["target"] == "template_release" else "debug"
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "{0}/bin/lib{1}.{2}.{3}.framework/{1}.{2}.{3}".format(
            addon_path,
            project_name,
            env["platform"],
            debug_or_release,
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "{}/bin/lib{}.{}.{}.{}{}".format(
            addon_path,
            project_name,
            env["platform"],
            debug_or_release,
            env["arch"],
            env["SHLIBSUFFIX"],
        ),
        source=sources,
    )

# if tests=yes, TESTS_ENABLED is defined in the source code.
if GetOption("tests") == "yes":
    env.Append(CPPPATH=["test/"])
    sources = Glob("test/*.cpp")
    
    # run test_main
    test_main = env.Program("test/test_main", sources)
    env.Depends(test_main, library)
    env.AlwaysBuild(test_main)
    env.AddPostAction(test_main, "./test/test_main")

    Default([library, test_main])

else:
    Default(library)
