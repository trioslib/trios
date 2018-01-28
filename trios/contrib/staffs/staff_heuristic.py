raise NotImplementedError('Staff Heuristic is not supported in python3.')




"""
from trios import WOperator
import numpy as np
try:
    from gamera import *
    from gamera.core import *
    from gamera.toolkits.musicstaves import musicstaves_rl_simple, musicstaves_rl_roach_tatem, musicstaves_linetracking, musicstaves_rl_carter
    from gamera.toolkits.musicstaves.musicstaves import *
    from gamera.toolkits.musicstaves.musicstaves_skeleton import MusicStaves_skeleton
    
from trios.feature_extractors import RAWFeatureExtractor


from gamera.plugins import numpy_io

class StaffHeuristic(WOperator):

    all_methods = ['skeleton', 'rl_simple', 'rl_roach', 'rl_carter', 'ltc', 'lth']

    def __init__(self,nomeMetodo=None):
        
        raise NotImplementedError('Staff Heuristic is not supported in python3.')
        self.nome_metodo = nomeMetodo
        self.window = np.zeros((1, 1), dtype=np.uint8)
        init_gamera()

    def apply(self, image_in_np, mask=None,batch=False):
        image_gamera = numpy_io.from_numpy(image_in_np)
        image_gamera.to_onebit()
        image_gamera.invert()

        if self.nome_metodo == 'skeleton':
            ms = MusicStaves_skeleton(image_gamera)
        elif self.nome_metodo == 'rl_simple':
            ms = musicstaves_rl_simple.MusicStaves_rl_simple(image_gamera)
        elif self.nome_metodo == 'rl_roach':
            ms = musicstaves_rl_roach_tatem.MusicStaves_rl_roach_tatem(image_gamera)
        elif self.nome_metodo == 'rl_carter':
            ms = musicstaves_rl_carter.MusicStaves_rl_carter(image_gamera)
        elif self.nome_metodo == 'ltc':
            ms = musicstaves_linetracking.MusicStaves_linetracking(image_gamera)
        elif self.nome_metodo == 'lth':
            ms = musicstaves_linetracking.MusicStaves_linetracking(image_gamera)
        
        else:
            raise Exception('Method not found: %s', self.nome_metodo)

        if self.nome_metodo == 'ltc':
            ms.remove_staves(symbol_criterion='secondchord')
        else:
            ms.remove_staves()
        
        image_out =  ms.image.image_copy()
        image_out_new =  image_out.to_numpy()

        newArray = np.zeros(image_in_np.shape,dtype=np.uint8)
         
        height,width = image_in_np.shape
    
        if batch==False: 
            for i in range(height):
                for j in range(width):
                    if image_out_new[i,j] != 0:
                        newArray[i,j]=255
        else:
            height,width = mask.shape
            whitePoints = 0
            for i in range(height):
                for j in range(width):             
                    if mask[i,j] != 0:
                            whitePoints+=1
    
            batchArray = np.zeros(whitePoints)
            height,width = image_in_np.shape
            
            cont = 0
            for i in range(height):
                for j in range(width):             
                    if mask[i,j]!=0:
                        if image_out_new[i,j] != 0:
                            batchArray[cont]=255
                    cont = cont+1

        if batch : 
            return batchArray 
        else: 
            return newArray
    
    def train(self, imgset):
        pass

    def write_state(self, obj_dict):
        obj_dict['nome_metodo'] = self.nome_metodo
        
    def set_state(self, obj_dict):
        self.nome_metodo = obj_dict['nome_metodo']
"""
