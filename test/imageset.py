# -*- coding: utf-8 -*-


import unittest
import trios

class ImagesetTest(unittest.TestCase):
    def test_creation_empty(self):
        img = trios.Imageset()
        self.assertEqual(img.image_list, [])
        self.assertEqual(img.input_preffix, '')
        self.assertEqual(img.output_preffix, '')
        self.assertEqual(img.mask_preffix, '')
        
    def test_append(self):
        img = trios.Imageset()
        with self.assertRaises(TypeError) as context:
            img.append('image_name')
        with self.assertRaises(TypeError) as context:
            img.append(('image_name',))
            
        with self.assertRaises(TypeError) as context:
            img.append(('image_name', 'out_name'))
            
        img.append(('image_name', 'out_name', 'mask_name'))
        self.assertEqual(img.image_list[-1], ('image_name', 'out_name', 'mask_name'))
        self.assertEqual(img[-1], ('image_name', 'out_name', 'mask_name'))

    def test_indexing(self):
        img = trios.Imageset([('a', 'b', None), ('c', 'b', None), ('d', 'b', None), ('e', 'b', None)])
        self.assertEqual(img[0], ('a', 'b', None))
        self.assertEqual(img[-1], ('e', 'b', None))
    
    def test_slice(self):
        img = trios.Imageset([('a', 'b', None), ('c', 'b', None), ('d', 'b', None), ('e', 'b', None)])
        img2 = img[:2]
        self.assertEqual(type(img2), trios.Imageset)
        self.assertEqual(img2.input_preffix, img.input_preffix)
        self.assertEqual(img2.output_preffix, img.output_preffix)
        self.assertEqual(img2.mask_preffix, img.mask_preffix)
        
    def test_len(self):
        imgset = trios.Imageset()
        self.assertEqual(len(imgset), 0)
        imgset.append(('asdl', '1sdf', 'ldkasl'))
        self.assertEqual(len(imgset), 1)
        
    def test_write_read(self):
        img = trios.Imageset([('a', 'b', None), ('c', 'b', 'er')])
        img.write('test-imageset.txt')
        img2 = trios.Imageset.read('test-imageset.txt')
        
        self.assertEqual(len(img), len(img2))
        self.assertEqual(img[0], img2[0])
        self.assertEqual(img[1], img2[1])
        
        
                
    
if __name__ == '__main__':
    unittest.main()
