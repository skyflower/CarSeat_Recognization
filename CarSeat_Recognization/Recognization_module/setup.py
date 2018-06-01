__author__ = 'Administrator'
from distutils.core import setup
from Cython.Build import cythonize

from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

#setup(ext_modules = cythonize(["label_image_command_line.py"], language='c++'))

## compiler command [python setup.py build_ext]

ext_module = Extension(
                        "image_classify",
            ["label_image_command_line.py"],
            extra_compile_args=["/openmp", "/DEBUG"],
            extra_link_args=["/DEBUG"],
            )

setup(
    cmdclass = {'build_ext': build_ext},
        ext_modules = [ext_module],
)