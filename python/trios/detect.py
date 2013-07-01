import os
import os.path

TRIOS_PATH = ''

def detect_trios():
	pass

def call(cmd):
    print(os.path.join(TRIOS_PATH, cmd))
    return os.system(os.path.join(TRIOS_PATH, cmd))

detect_trios()
    
