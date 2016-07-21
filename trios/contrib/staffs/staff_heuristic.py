from trios import WOperator
from gamera import *
from gamera.core import *
from gamera.toolkits.musicstaves import musicstaves_rl_simple, musicstaves_rl_roach_tatem
from gamera.toolkits.musicstaves.musicstaves import *
from gamera.toolkits.musicstaves.musicstaves_skeleton import MusicStaves_skeleton
from trios.feature_extractors import RAWFeatureExtractor, CombinationPattern

import numpy as np
import inspect 
#from numpy to gamera
from gamera.plugins import numpy_io

#from gamera to numpy
from gamera.plugins import numpy_io
from numpy import fft

# pode fazer essa classe usando python mesmo

class StaffHeuristic(WOperator):

    nomeMetodo = 'skeleton'
    ms = None 
    window = np.zeros((1,1),dtype=np.uint8)
    extractor = RAWFeatureExtractor(window)
    
    def __init__(self,nomeMetodo=None):
        self.nomeMetodo = nomeMetodo

        
#usar os metodos do music staves para desenvolver 

    def apply(self, image_in_np, mask=None,batch=False):
    #transformar imagem para o formato do music staves
          
        
        init_gamera()
        image_gamera = numpy_io.from_numpy(image_in_np)
        image_gamera.to_onebit()
        image_gamera.invert()

        if self.nomeMetodo == 'skeleton':
             ms = MusicStaves_skeleton(image_gamera)
        else:
            if self.nomeMetodo == 'rl_simple':
                ms = musicstaves_rl_simple.MusicStaves_rl_simple(image_gamera)
            else:
                if self.nomeMetodo == 'rl_roach':
                    ms = musicstaves_rl_roach_tatem.MusicStaves_rl_roach_tatem(image_gamera)
    
        ms.remove_staves(crossing_symbols = 'bars')
        
        image_out =  ms.image.image_copy()
    
        image_out_new =  image_out.to_numpy()
         
        ###pegar cada ponto do imageOut para o newArray 
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
        obj_dict['nomeMetodo'] = self.nomeMetodo
        
    def set_state(self, obj_dict):
        self.nomeMetodo = obj_dict['nomeMetodo']

