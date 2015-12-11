from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext



from Cython.Compiler.Options import directive_defaults

directive_defaults['linetrace'] = True
directive_defaults['binding'] = True

import numpy as np

includes = [np.get_include(), 'C:/Users/Igor/Miniconda3/includes']
libs = ['C:/Users/Igor/Miniconda3/libs']
extra_compile_args = ['-fopenmp']
extra_link_args = ['-fopenmp']

extensions = [
        
            Extension('trios.ISI.isi', ['trios/ISI/isi.pyx', 
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

                    'trios/ISI/train_operator.c',
                    # ISI algorithm
                    'trios/ISI/dp_isi.c',
                    'trios/ISI/ip_isi.c',
                    'trios/ISI/partition.c',
                    'trios/ISI/ip_mincover.c', 
                    ], include_dirs=['trios/v1', 'trios/ISI']),
            Extension('trios.WOperator', [
                    'trios/WOperator.pyx']),
            Extension('trios.feature_extractors.raw', [
                    'trios/feature_extractors/raw.pyx']),
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
    cmdclass = {'build_ext': build_ext},
    ext_modules = cythonize(extensions),
    packages = ['trios', 'trios.feature_extractors', 'trios.ISI', 'trios.classifiers', 'trios.v1', 'trios.legacy', 'trios.window_determination', 'trios.shortcuts']
)
