package(default_visibility = ["//visibility:public"])

cc_library(
    name = "tvout",
    srcs = ["tvout.cpp"],
    hdrs = [
        "tvout.h",
        "tvout_constants.h",
    ],
    deps = [
        "//gamelib:gamelib_config",
        "//gamelib/compat:compat_main",
        "//gamelib/compat:sdl_display_interface",
    ],
)

cc_library(
    name = "screen",
    srcs = ["screen.cpp"],
    hdrs = ["screen.h"],
    deps = [
        ":tvout",
        "//gamelib/utils/VectorLib:vectorlib",
    ],
)

cc_library(
    name = "disp3d",
    srcs = ["disp3d.cpp"],
    hdrs = ["disp3d.h"],
    deps = [
        ":screen",
        "//gamelib/utils/VectorLib:vectorlib",
    ],
)

cc_library(
    name = "selector_widget",
    hdrs = ["selector_widget.h"],
    deps = [
        ":screen",
        ":widget_util",
    ],
)

cc_library(
    name = "modal_widget",
    hdrs = ["modal_widget.h"],
    deps = [
        ":screen",
        ":widget_util",
    ],
)

cc_library(
    name = "widget_util",
    srcs = ["widget_util.cc"],
    hdrs = ["widget_util.h"],
    deps = [":screen"],
)
