package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sdl_display_interface",
    srcs = ["SDL_DisplayInterface.cpp"],
    hdrs = ["SDL_DisplayInterface.h"],
)

cc_library(
    name = "compat_main",
    srcs = ["compat_main.cpp"],
    hdrs = ["compat_main.h"],
    deps = [":sdl_display_interface"],
)
