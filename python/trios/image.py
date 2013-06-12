"""
TRIOS uses numpy 2D arrays as images. The read_image/write_image are used
to read/write images to/from the disk.
"""

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
