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
    default_for_package = os.path.split(os.path.realpath(__file__))[0]
    default_for_package += '/../../bin/bin'
    
    if os.system('trios_build > null') == 0:
        TRIOS_PATH = ''
    elif os.system('./trios_build > null') == 0:
        TRIOS_PATH = os.getcwd()
    elif os.system(os.path.join(pth, 'trios_build > null')) == 0:
        TRIOS_PATH = pth
    elif os.system(os.path.join(default_for_package, 'trios_build > null')) == 0:
        TRIOS_PATH = default_for_package
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
    
