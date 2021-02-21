cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    linkopts = ["-lSDL2"],
    deps = [
        "//game",
        "//gamelib",
        "//gamelib/compat:compat_main",
        "//gamelib/compat:sdl_display_interface",
        "//gamelib/display:screen",
        "//gamelib/display:tvout",
        "//gamelib/input:remote",
    ],
)
