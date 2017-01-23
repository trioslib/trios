"""
Contains Imageset related functions.
"""

import os.path
import numpy as np
import trios.shortcuts.persistence as p
import collections

ImagePair = collections.namedtuple('ImagePair', ['input', 'output', 'mask'])

class Imageset:
    """
An Imageset is a list of examples (tuples containing two or three strings). Each example contains the path of the input and ideal images and may contain a binary mask. Imageset is a subclass of list.

Every place that expects an Imageset will work with a simple list in the following format:

[ 
 ['input1', 'ideal1', 'mask1'],
 ['input2', 'ideal2'],
 ....
 ....
]

Also, Imageset([['input1', 'ideal1', 'mask1'], ['input2', 'ideal2'], ... ]) converts the list to an Imageset.
    """
    
    def __init__(self, initial_iter=None, input_preffix='', output_preffix='',
                 maks_preffix=''):
        self.image_list = []
        self.input_preffix = input_preffix
        self.output_preffix = output_preffix
        self.mask_preffix= maks_preffix
        if initial_iter != None:
            for pair in initial_iter:
                self.append(pair)
            
    
    def __getitem__(self, key):
        if isinstance(key, slice):
            return Imageset(self.image_list[key], self.input_preffix, 
                            self.output_preffix, self.mask_preffix)
        else:
            return self.image_list[key]
    
    def __len__(self):
        return len(self.image_list)
    
    def get_full_path(self, i):
        return (os.path.join(self.input_preffix, self.image_list[i][0]),
                os.path.join(self.output_preffix, self.image_list[i][1]),
                os.path.join(self.mask_preffix, self.image_list[i][2]))
            
    def append(self, example):
        """
        Adds an example to the Imageset. All examples must have the same number of images.
        """
        try:
            example = ImagePair(*example)
        except TypeError:
            raise TypeError('Object must be a tuple with three file names')
    
        self.image_list.append(example)
    
    def num_samples(self, win):
        total = 0
        for (_, _, m) in p.load_imageset(self, win):
            total += len(np.nonzero(m)[0])
        return total
    
    @staticmethod
    def __skip_blank(lines, count):
        while lines[count].strip() == "":
            count += 1
        return count
    
    @classmethod
    def read(cls, fname):
        """
        Reads Imageset from disk.
        """
        f = open(fname, 'r')
        imgset = Imageset()
        
        lines = f.readlines()
        
        n = int(lines[1].split()[1])
        ngroups = int(lines[2].split()[1])
        if ngroups != 2 and ngroups != 3:
            print('Number of groups must be 2 or 3')
            return None
        
        base_dir = [""] * 3
        base_dir[0] = lines[4].strip()
        base_dir[1] = lines[5].strip()
        
        pref = os.path.dirname(fname)
        
        if ngroups == 3:
            base_dir[2] = lines[6].strip()
            
        for i in range(3):
            if (base_dir[i].startswith('./') and 
                len(base_dir[i].split('/')) == 2):
                
                base_dir[i] = base_dir[i][2:]
            
        for i in range(3):
            base_dir[i] = os.path.join(pref, base_dir[i])
            
        line_count = 7
        for i in range(n):
            line_count = cls.__skip_blank(lines, line_count)
            input_img = base_dir[0] + lines[line_count].strip()
            ideal_img = base_dir[1] + lines[line_count+1].strip()
            if ngroups == 2:
                imgset.append( (input_img, ideal_img, None) )
            else:
                mask_img = base_dir[2] + lines[line_count+2].strip()
                imgset.append( (input_img, ideal_img, mask_img) )
            line_count += ngroups        
        
        f.close()
        return imgset


    def write(self, fname):
        """
        Writes Imageset to disk.
        """
        f = open(fname, 'w')
        f.write('IMGSET  ########################################################\n')
        
        n = len(self)
        ngroups = len(self[0])
        
        f.write('.n %d\n' % n)
        f.write('.f %d\n' % ngroups)
        f.write('.d\n' + './\n' * ngroups + '\n')
        
        for example in self:
            f.write('\n'.join(example))
            f.write('\n\n')
        
        f.close()
