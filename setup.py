from setuptools.extension import Extension
from setuptools import setup
import os
import platform


extensions = [
            Extension('trios.classifiers.base_classifier', [
                    'trios/classifiers/base_classifier.pyx']),                    
            Extension('trios.feature_extractors.base_extractor', [
                    'trios/feature_extractors/base_extractor.pyx']),
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
            Extension('trios.window_determination.relief', [
                    'trios/window_determination/relief.pyx']),
            Extension('trios.contrib.features.lbp',[ 
                    'trios/contrib/features/lbp.pyx']),
            Extension('trios.contrib.features.moments',[ 
                    'trios/contrib/features/moments.pyx']),
            Extension('trios.contrib.features.fourier',[ 
                    'trios/contrib/features/fourier.pyx']),
            Extension('trios.contrib.features.sobel',[ 
                    'trios/contrib/features/sobel.pyx']),
            Extension('trios.contrib.features.featurecombination', [
                    'trios/contrib/features/featurecombination.pyx']),
            Extension('trios.contrib.features.hog', [
                    'trios/contrib/features/hog.pyx'])]
try:
   
    import numpy as np
    includes = [np.get_include()]
    if platform.system() != 'Windows':
        extra_compile_args = ['-fopenmp']
        extra_link_args = ['-fopenmp']

    
    for extt in extensions:
            extt.include_dirs += includes
            if platform.system() != 'Windows':
                extt.extra_compile_args += extra_compile_args
                extt.extra_link_args += extra_link_args

    
    from Cython.Build import cythonize
    extensions = cythonize(extensions)
except ImportError:
    pass


setup(
    name='trios',
    version='2.1',
    author='Igor Montagner, Roberto Hirata Jr, Nina S. T. Hirata',
    author_email='igordsm+trios@gmail.com',
    url='http://trioslib.github.io',
    classifiers=['Development Status :: 5 - Production/Stable',
                 'Intended Audience :: Science/Research',
                 'License :: OSI Approved :: GNU Lesser General Public License v2 (LGPLv2)',
                 'Operating System :: POSIX :: Linux',
                 'Programming Language :: Python :: 3',
                 'Programming Language :: Cython'
                 ],
    packages = ['trios', 'trios.feature_extractors', 'trios.classifiers', 'trios.legacy', 
                'trios.window_determination', 'trios.shortcuts', 'trios.contrib', 'trios.contrib.kern_approx',
                'trios.contrib.nilc', 'trios.contrib.features', 'trios.contrib.staffs'
                ],
    setup_requires=[
            'cython==0.26.1',
            'numpy==1.13.3', 
            ],
    install_requires=[
        'cython==0.26.1',
        'cffi==1.10.0', 
        'numpy==1.13.3', 
        'scikit-learn==0.19.1', 
        'scipy==0.19.1', 
        'pillow', 
        'numba==0.35.0',
        'scikit-image==0.13.1', 
    ], 
    ext_modules = extensions,
    include_dirs = [np.get_include()],
    cffi_modules = ['trios/legacy/build_legacy.py:c_code'],
)
