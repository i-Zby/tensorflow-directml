# Description:
# TensorBoard module containing volatile or experimental code.

# For platform specific build config
load("//tensorflow/core/platform:default/build_config.bzl", "tf_proto_library")
load("//tensorflow:tensorflow.bzl", "py_test")

package(
    default_visibility = ["//tensorflow:internal"],
    licenses = ["notice"],  # Apache 2.0
)

exports_files(["LICENSE"])

tf_proto_library(
    name = "protos_all",
    srcs = glob(["**/*.proto"]),
    visibility = ["//visibility:public"],
)

# API methods in `tf.contrib.tensorboard` package.
py_library(
    name = "tensorboard",
    srcs = ["__init__.py"],
    srcs_version = "PY2AND3",
    deps = [":plugins"],
)

# API methods in `tf.contrib.tensorboard.plugins` package.
py_library(
    name = "plugins",
    srcs = ["plugins/__init__.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":projector",
    ],
)

# API methods and protos in `tf.contrib.tensorboard.plugins.projector` package.
py_library(
    name = "projector",
    srcs = ["plugins/projector/__init__.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":protos_all_py",
        "//tensorflow/python:lib",
    ],
)

py_test(
    name = "projector_api_test",
    size = "small",
    srcs = ["plugins/projector/projector_api_test.py"],
    python_version = "PY2",
    srcs_version = "PY2AND3",
    deps = [
        ":projector",
        ":protos_all_py",
        "//tensorflow/python:client_testlib",
        "//tensorflow/python:platform",
        "//tensorflow/python:summary",
    ],
)
