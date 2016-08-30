import os

if __name__ == '__main__':
    for file in os.listdir(path='.'):
        if file.endswith('test.py'):
            print('Executando', file, end=' ')
            with open(file) as script:
                try:
                    exec(script.read())
                except Exception as e:
                    print('- FAIL!\nUm erro ocorreu em', file)
                    print(e)
                else:
                    print('- OK')
        