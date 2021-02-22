load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    linkopts = ["-lSDL2"],
    deps = [
        ":console",
        ":control_message_cc_proto",
        "//bitmaps:font5x7",
        "//gamelib",
        "//gamelib/compat:compat_main",
        "//gamelib/compat:sdl_display_interface",
        "//gamelib/display:screen",
        "//gamelib/display:tvout",
        "//utils:udp_server",
    ],
)

cc_proto_library(
    name = "control_message_cc_proto",
    srcs = ["control_message.proto"],
)

cc_binary(
    name = "test_server",
    srcs = ["test_server.cc"],
    deps = [
        "//utils:udp_server",
        ":control_message_cc_proto",
        #"@com_google_protobuf//:text_format",
    ],
)

cc_library(
    name = "console",
    srcs = ["console.cc"],
    hdrs = ["console.h"],
    deps = [
        ":control_message_cc_proto",
        ":dro_widget",
        "//gamelib/display:screen",
        "//gamelib/display:selector_widget",
    ],
)

cc_library(
    name = "dro_widget",
    hdrs = ["dro_widget.h"],
    deps = [
        "//gamelib/display:screen",
        "//gamelib/display:widget_util",
    ],
)
