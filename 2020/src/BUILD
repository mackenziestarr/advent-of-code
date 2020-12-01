load("@rules_cc//cc:defs.bzl", "cc_test")

filegroup(
    name = "input_data",
    srcs = [
        "day-01.input"
    ],
)

cc_test(
    name = "day-01",
    srcs = ["day-01.cc"],
    data = [":input_data"],
    copts = ["-std=c++2a -g"],
    deps = [
        "@catch2//:catch2_main"
    ]
)


