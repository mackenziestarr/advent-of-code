load("@rules_cc//cc:defs.bzl", "cc_test")

filegroup(
    name = "input_data",
    srcs = [
        "day-01.input",
        "day-02.input",
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

cc_test(
    name = "day-02",
    srcs = ["day-02.cc"],
    data = [":input_data"],
    copts = ["-std=c++2a"],
    deps = [
        "@catch2//:catch2_main"
    ]
)


