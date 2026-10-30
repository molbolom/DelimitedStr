from setuptools import setup, Extension

setup(
    name="delimitedstr",
    version="0.0.1",
    ext_modules=[Extension("delimitedstr", sources=["src/DelimitedStr.c"])],
)
