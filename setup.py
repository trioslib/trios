from setuptools.extension import Extension
from setuptools import setup
from Cython.Build import cythonize
from Cython.Distutils import build_ext

import numpy as np
from IPython.core.release import author_email

includes = [np.get_include()]#, 'C:/Users/Igor/Miniconda3/includes']
libs = []#['C:/Users/Igor/Miniconda3/libs', 'legacy_stdio_definitions.lib']
extra_compile_args = ['-fopenmp']
extra_link_args = ['-fopenmp']

extensions = [
        
            Extension('trios.classifiers.ISI.isi', ['trios/classifiers/ISI/isi.pyx', 
                    'trios/v1/definitions.pxd',
                    'trios/v1/basic_mtm.c',
                    'trios/v1/basic_itv.c',
                    'trios/v1/basic_common.c',
                    'trios/v1/basic_error.c',
                    'trios/v1/basic_win.c',
                    'trios/v1/io_header.c',
                    'trios/v1/io_itv.c',
                    'trios/v1/io_win.c',
                    'trios/v1/io_mtm.c',                    

                    'trios/classifiers/ISI/train_operator.c',
                    # ISI algorithm
                    'trios/classifiers/ISI/dp_isi.c',
                    'trios/classifiers/ISI/ip_isi.c',
                    'trios/classifiers/ISI/partition.c',
                    'trios/classifiers/ISI/ip_mincover.c', 
                    ], include_dirs=['trios/v1', 'trios/classifiers/ISI']),
            Extension('trios.WOperator', [
                    'trios/WOperator.pyx']),
            Extension('trios.feature_extractors.raw', [
                    'trios/feature_extractors/raw.pyx']),
            Extension('trios.feature_extractors.aperture', [
                    'trios/feature_extractors/aperture.pyx']),        
            Extension('trios.feature_extractors.combination', [
                    'trios/feature_extractors/combination.pyx']),            
            Extension('trios.wop_matrix_ops', [
                    'trios/wop_matrix_ops.pyx']),
            Extension('trios.util', [
                    'trios/util.pyx']),
            Extension('trios.serializable', [
                    'trios/serializable.pyx']),
            Extension('trios.legacy.io', [
                      'trios/legacy/io.pyx',
                        'trios/v1/definitions.pxd',
                        'trios/v1/basic_mtm.c',
                        'trios/v1/basic_itv.c',
                        'trios/v1/basic_common.c',
                        'trios/v1/basic_error.c',
                        'trios/v1/basic_win.c',
                        'trios/v1/io_header.c',
                        'trios/v1/io_itv.c',
                        'trios/v1/io_win.c',
                        'trios/v1/io_mtm.c',                    
                      ], include_dirs=['trios/v1/']),
            Extension('trios.window_determination.relief', [
                    'trios/window_determination/relief.pyx'
                    ])]

for ext in extensions:
    ext.include_dirs += includes
    ext.library_dirs += libs
    ext.extra_compile_args += extra_compile_args
    ext.extra_link_args += extra_link_args

setup(
    name='trios',
    version='2.0.0',
    author='Igor Montagner, Roberto Hirata Jr, Nina S. T. Hirata',
    author_email='igordsm+trios@gmail.com',
    url='http://trioslib.sf.net',
    classifiers=['Development Status :: 3 - Alpha',
                 'Programming Language :: Python :: 2',
                 'Programming Language :: Python :: 3'],
    
    cmdclass = {'build_ext': build_ext},
    ext_modules = cythonize(extensions),
    packages = ['trios', 'trios.feature_extractors', 'trios.classifiers.ISI', 'trios.classifiers', 'trios.v1', 'trios.legacy', 'trios.window_determination', 'trios.shortcuts',]# 'trios.contrib']
)
