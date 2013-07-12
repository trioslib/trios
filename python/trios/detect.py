import os
import os.path

TRIOS_PATH = ''



def detect_trios(pth=''):
    if os.system('trios_build') >= 0:
        TRIOS_PATH = ''
    elif os.system('./trios_build') >= 0:
        TRIOS_PATH = os.getcwd()
    elif os.system(os.path.join(pth, 'trios_build')) >= 0:
        TRIOS_PATH = pth
    else:
        raise OSError(-1, 'TRIOS not found.')
        
    


def call(cmd):
    print(os.path.join(TRIOS_PATH, cmd))
    return os.system(os.path.join(TRIOS_PATH, cmd))

detect_trios()
    
