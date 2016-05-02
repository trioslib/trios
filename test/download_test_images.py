# -*- coding: utf-8 -*-
"""
Created on Tue Oct 27 15:50:50 2015

@author: igordsm
"""

import urllib.request
import tarfile

import io

if __name__ == "__main__":
    with urllib.request.urlopen(
        "http://vision.ime.usp.br/projects/trios/datasets/dataset-character.tar.gz"
        ) as images:

        
        with tarfile.open(fileobj=images, mode="r|gz") as tgz:
            tgz.extractall(path='images')
            
        