import trios
import trios.shortcuts.persistence as p

if __name__ == '__main__':
    # Imageset can be defined directly in code, like below
    images = trios.Imageset([
        ('input.png', 'output.png', None),
        ('input2.png', 'output2.png', 'mask.png')
    ])
    # Definitions in code can be put in a module and imported like regular
    # Python code.
    
    
    # It can also be saved to a gziped file using the persistence
    p.save_gzip(images, 'imageset.gz')
    
    # And loaded back
    images2 = p.load_gzip('imageset.gz')
    assert images[0] == images2[0]
    assert images[1] == images2[1]
    assert len(images) == len(images2)
    
    # Or saved to a text-only format using the read and write methods.
    images.write('imageset-text.txt')
    
    images3 = trios.Imageset.read('imageset-text.txt')
    assert images[0] == images3[0]
    assert images[1] == images3[1]
    assert len(images) == len(images3)
