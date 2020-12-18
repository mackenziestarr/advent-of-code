def gen_cc_tests(names, visibility=None):
    for name in names:
        native.cc_test(
            name = name,
            srcs = [name + ".cc"],
            data = ["//input:data"],
            copts = ["-std=c++2a"],
            deps = [
                "@catch2//:catch2_main"
            ]
        )
