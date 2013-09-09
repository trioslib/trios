"""
Detects TRIOS command line tools. The path is stored in TRIOS_PATH.
"""

import os
import shlex
import subprocess
import os.path

TRIOS_PATH = ''

def detect_trios(pth=''):
    """
    Looks for TRIOS command line tools in some paths.
    """
    global TRIOS_PATH
    default_for_package = os.path.split(os.path.realpath(__file__))[0]
    default_source_package = default_for_package + '/../../bin/'
    default_for_package += '/../../bin/bin'
    
    paths = ['', './', pth, default_for_package, default_source_package]

    with open(os.devnull, 'w') as trash:    
        for p in paths:
            try:
                subprocess.call(os.path.join(p, 'trios_build'), stdout=trash, stderr=trash)
            except OSError:
                pass
            else:
                TRIOS_PATH = p
                print 'Detected path', TRIOS_PATH
                return
    """if os.system('trios_build > null') == 0:
        TRIOS_PATH = ''
    elif os.system('./trios_build > null') == 0:
        TRIOS_PATH = os.getcwd()
    elif os.system(os.path.join(pth, 'trios_build > null')) == 0:
        TRIOS_PATH = pth
    elif os.system(os.path.join(default_for_package, 'trios_build > null')) == 0:
        TRIOS_PATH = default_for_package
    elif os.system(os.path.join(default_source_package, 'trios_build > null')) == 0:
        TRIOS_PATH = default_source_package
    else:
        print('TRIOS not found.')
    """
    
def call(cmd, output=os.devnull):
    """
    Calls a command line tool from TRIOS.
    """
    global TRIOS_PATH
    cmd = shlex.split(cmd)
    cmd[0] = os.path.join(TRIOS_PATH, cmd[0])
    with open(output, 'w') as redir:
        with open(os.devnull, 'w') as trash:
            return subprocess.call(cmd, stdout=redir, stderr=trash)

detect_trios()
    
