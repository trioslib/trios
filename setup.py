from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
from Cython.Distutils import build_ext

import numpy as np

setup(
    name='trios',
    cmdclass = {'build_ext': build_ext},
    ext_modules = cythonize([
        
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
                    ], include_dirs=['trios/v1', 'trios/ISI', np.get_include()], extra_compile_args = ['-fopenmp', '-g'], extra_link_args = ['-fopenmp']),
            Extension('trios.feature_extractors.raw', [
                    'trios/feature_extractors/raw.pyx'
                    ], include_dirs=[np.get_include()]),
            Extension('trios.wop_matrix_ops', [
                    'trios/wop_matrix_ops.pyx'
                    ], include_dirs=[np.get_include()]),
            Extension('trios.util', [
                    'trios/util.pyx'
                    ], include_dirs=[np.get_include()]),
            Extension('trios.serializable', [
                    'trios/serializable.pyx'
                    ], include_dirs=[np.get_include()]),
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
                      ], include_dirs=['trios/v1/', np.get_include()]),
            Extension('trios.window_determination.relief', [
                    'trios/window_determination/relief.pyx'
                    ], include_dirs=[np.get_include()]),
    ], gdb_debug=True, output_dir='.'),
    packages = ['trios', 'trios.feature_extractors', 'trios.ISI', 'trios.classifiers', 'trios.v1', 'trios.legacy', 'trios.window_determination']
)
