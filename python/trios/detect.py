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
    global TRIOS_PATH
    if os.system('trios_build > null') == 0:
        TRIOS_PATH = ''
    elif os.system('./trios_build > null') == 0:
        TRIOS_PATH = os.getcwd()
    elif os.system(os.path.join(pth, 'trios_build > null')) == 0:
        TRIOS_PATH = pth
        print pth
        print TRIOS_PATH
    else:
        print('TRIOS not found.')
        
def call(cmd):
    """
    Calls a command line tool from TRIOS.
    """
    global TRIOS_PATH
    print(os.path.join(TRIOS_PATH, cmd))
    return os.system(os.path.join(TRIOS_PATH, cmd))

detect_trios()
    
