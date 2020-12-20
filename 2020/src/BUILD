load("@rules_cc//cc:defs.bzl", "cc_test")
load("cc_tests.bzl", "gen_cc_tests")

cc_library(
    name = "lib",
    hdrs = [
        "lib.h",
    ],
    copts = ["-std=c++2a"],
)


gen_cc_tests([
    "day-01",
    "day-02",
    "day-03",
    "day-04",
    "day-05",
    "day-06",
    "day-07",
    "day-08",
    "day-09",
    "day-10",
])
