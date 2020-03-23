cc_library(
    name = "multi_resolution_graph",
    hdrs = glob([
        "include/multi_resolution_graph/*.h",
        "include/multi_resolution_graph/*.tpp",
    ]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "test",
    srcs = glob(["test/multi_resolution_graph/*.cpp"]),
    deps = [
        ":multi_resolution_graph",
        "@gtest//:gtest_main",
    ],
)
