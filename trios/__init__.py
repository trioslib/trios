from trios.imageset import *
from trios.woperator import WOperator

import platform

mp_support = platform.system() != 'Windows'
show_eval_progress = True