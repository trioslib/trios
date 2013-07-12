"""
Detects TRIOS command line tools. The path is stored in TRIOS_PATH.
"""

import os
import os.path

TRIOS_PATH = ''

def detect_trios():
    """
    Looks for TRIOS command line tools in some paths.
    """
    pass

def call(cmd):
    """
    Calls a command line tool from TRIOS.
    """
    print(os.path.join(TRIOS_PATH, cmd))
    return os.system(os.path.join(TRIOS_PATH, cmd))

detect_trios()
    
