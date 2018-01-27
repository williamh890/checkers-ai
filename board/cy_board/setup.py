from distutils.core import setup
from Cython.Build import cythonize

setup(
    name = "checkers components",
    ext_modules = cythonize("*.pyx")
)
