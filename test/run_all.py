import os
import download_test_images
import gc

if __name__ == '__main__':
    download_test_images.download_images()
    for file in os.listdir(path='.'):
        if file.endswith('test.py'):

            print('Executando ' + str(file))

            with open(file) as script:
                try:
                    exec(script.read())
                    gc.collect()
                except Exception as e:
                    print('- FAIL!\nUm erro ocorreu em', file)
                    print(e)
                else:
                    print('- OK')
        