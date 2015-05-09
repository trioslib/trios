# -*- coding: utf-8 -*-
"""
Created on Sun May  3 10:23:16 2015

@author: igor
"""

import json
import importlib

cdef class Serializable:
    def __init__(self):
        pass
    
    def write_state(self, obj_dict):
        pass
    
    def set_state(self, obj_dict):
        pass
    
    def write(self, fname):
        obj_dict = {}
        obj_dict['class_name'] = self.__class__.__name__
        obj_dict['class_module'] = self.__class__.__module__
        
        obj_dict['state'] = {}
        self.write_state(obj_dict['state'])
        
        if type(fname) == str:
            with open(fname, 'w') as f:
                json.dump(obj_dict, f)
        elif fname == None:
            return json.dumps(obj_dict)
        else:
            json.dump(obj_dict, fname)
            
    
    @staticmethod
    def read(fname):
        if type(fname) == str or type(fname) == unicode:
            try:
                with open(fname) as f:
                    obj_dict = json.load(f)
            except IOError:
                obj_dict = json.loads(fname)
        else:
            obj_dict = json.load(fname)
        
        module = importlib.import_module(obj_dict['class_module'])
        class_obj = getattr(module, obj_dict['class_name'])
        obj = class_obj()
        obj.set_state(obj_dict['state'])
        
        return obj