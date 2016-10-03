from setuptools.extension import Extension
from setuptools import setup
import os
import platform

try:
    import Cython
    use_cython = True
except ImportError:
    use_cython = False

if os.path.exists('trios/WOperator.pyx'):
    ext = '.pyx'
else:
    ext = '.c'

import numpy as np

includes = [np.get_include()]#, 'C:/Users/Igor/Miniconda3/includes']
if platform.system() != 'Windows':
    extra_compile_args = ['-fopenmp']
    extra_link_args = ['-fopenmp']

extensions = [
            Extension('trios.WOperator', [
                    'trios/WOperator'+ext]),
            Extension('trios.feature_extractors.raw', [
                    'trios/feature_extractors/raw'+ext]),
            Extension('trios.feature_extractors.aperture', [
                    'trios/feature_extractors/aperture'+ext]),        
            Extension('trios.feature_extractors.combination', [
                    'trios/feature_extractors/combination'+ext]),            
            Extension('trios.wop_matrix_ops', [
                    'trios/wop_matrix_ops'+ext]),
            Extension('trios.util', [
                    'trios/util'+ext]),
            Extension('trios.serializable', [
                    'trios/serializable'+ext]),
            Extension('trios.window_determination.relief', [
                    'trios/window_determination/relief'+ext
                    ])]

for extt in extensions:
    extt.include_dirs += includes
    if platform.system() != 'Windows':
        extt.extra_compile_args += extra_compile_args
        extt.extra_link_args += extra_link_args

if use_cython:
    from Cython.Build import cythonize
    extensions = cythonize(extensions)

setup(
    name='trios',
    version='2.0.5',
    author='Igor Montagner, Roberto Hirata Jr, Nina S. T. Hirata',
    author_email='igordsm+trios@gmail.com',
    url='http://trioslib.sf.net',
    classifiers=['Development Status :: 4 - Beta',
                 'Programming Language :: Python :: 2',
                 'Programming Language :: Python :: 3'],
    install_requires=[
        'cython', 'cffi', 'numpy', 'scikit-learn', 'scipy', 'pillow'
    ], 
    ext_modules = extensions,
    cffi_modules = ['trios/legacy/build_legacy.py:c_code'],
    packages = ['trios', 'trios.feature_extractors', 'trios.classifiers', 'trios.legacy', 'trios.window_determination', 'trios.shortcuts',]# 'trios.contrib']
)
