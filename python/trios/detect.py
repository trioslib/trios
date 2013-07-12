"""
Detects TRIOS command line tools. The path is stored in TRIOS_PATH.
"""

import os
import os.path

TRIOS_PATH = ''

def detect_trios(pth=''):
    """
    Looks for TRIOS command line tools in some paths.
    """
    if os.system('trios_build') >= 0:
        TRIOS_PATH = ''
    elif os.system('./trios_build') >= 0:
        TRIOS_PATH = os.getcwd()
    elif os.system(os.path.join(pth, 'trios_build')) >= 0:
        TRIOS_PATH = pth
    else:
        raise OSError(-1, 'TRIOS not found.')
        
def call(cmd):
    """
    Calls a command line tool from TRIOS.
    """
    print(os.path.join(TRIOS_PATH, cmd))
    return os.system(os.path.join(TRIOS_PATH, cmd))

detect_trios()
    
