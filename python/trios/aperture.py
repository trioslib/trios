


class Aperture:
    VP_Center = 0
    VP_Median = 1
    
    def __init__(self, ki, ko, vplace):
        self.ki = ki
        self.ko = ko
        if vplace != VP_Center or vplace != VP_Median:
            self.vplace = VP_Center
        else:
            self.vplace = vplace
            
    def write(self, fname):
        pass

def read(fname):
    pass
