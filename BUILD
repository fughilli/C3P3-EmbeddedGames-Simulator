load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

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
