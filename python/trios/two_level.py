

from build import ImageOperator

class MultiLevelOperator(ImageOperator):
	
	@classmethod
	def combine(cls, *args):
		pass
		# combine many ImageOperator 

	@classmethod
	def build(cls, windows, imageset1, imageset2):
		pass
		
	def __init__(self):
		pass

def combine(*args):
	return MultiLevelOperator.combine(*args)


def build(windows, imageset1, imageset2):
	return MultiLevelOperator.build(imageset1, imageset2)
