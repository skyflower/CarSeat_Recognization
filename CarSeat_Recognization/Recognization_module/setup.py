__author__ = 'Administrator'
from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize(["label_image_command_line.py"], language='c++'))

## compiler command [python setup.py build_ext]

