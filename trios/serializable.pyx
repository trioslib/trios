# -*- coding: utf-8 -*-
"""
Created on Sun May  3 10:23:16 2015

@author: igor
"""

import json
try:
    import cPickle as pickle
except ImportError:
    import pickle
import importlib

def rebuild_serializable(obj_dict):
    return Serializable.read(obj_dict)

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
        
        try:
            obj_dict['bibtex_citation'] = self.bibtex_citation
        except AttributeError:
            pass
        
        obj_dict['state'] = {}
        self.write_state(obj_dict['state'])
        
        if type(fname) == str:
            with open(fname, 'wb') as f:
                pickle.dump(obj_dict, f, -1)
        elif fname == None:
            return obj_dict
        else:
            json.dump(obj_dict, fname)
            
    def __reduce__(self):
        obj_dict = self.write(None)
        return (rebuild_serializable, (obj_dict,) )
    
    @staticmethod
    def read(fname):
        if type(fname) == str or type(fname) == unicode:
            with open(fname, 'rb') as f:
                obj_dict = pickle.load(f)
        else:
            obj_dict = fname
        
        module = importlib.import_module(obj_dict['class_module'])
        class_obj = getattr(module, obj_dict['class_name'])
        obj = class_obj()

        if 'bibtex_citation' in obj_dict:
            try:
                obj.bibtex_citation = obj_dict['bibtex_citation']
            except AttributeError:
                pass

        obj.set_state(obj_dict['state'])
        
        return obj