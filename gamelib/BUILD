package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gamelib",
    hdrs = ["gamelib.h"],
    deps = [
        ":gamelib_config",
        "//gamelib/display:screen",
        "//gamelib/display:tvout",
        "//gamelib/input:remote",
        "//gamelib/utils:pwm_timer",
        "//gamelib/utils/math:game_math",
    ],
)

cc_library(
    name = "gamelib_config",
    hdrs = ["gamelib_config.h"],
)
