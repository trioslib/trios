from trios.legacy._legacy import lib as v1
from trios.legacy import io as lio
from trios.WOperator import Classifier
import numpy as np

class ISI(Classifier):
	'''
	description here
	'''
	
	def __init__(self, win=None):
		super(ISI, self).__init__()
		self.set_win(win)

	def set_win(self, win):
		self.win = win
		if win != None:
			self.win_old = lio.win_to_old(win)
			self.wzip = sum(win != 0)
		
	def train(self, dataset, kw):
		assert type(dataset) == dict
		

	def apply(self, pat):
		return 0
		
	
		
	def write_state(self, obj_dict):
		pass
	
	def set_state(self, obj_dict):
		pass