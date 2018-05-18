import sys
import cv2
import numpy as np

def Test(byteImg):
	frame = np.frombuffer(byteImg, dtype=np.uint8).reshape(288, 352, 3)
	cv2.imshow("PyShow",frame)
	
	ret_bbox = []
	box = [1,2,3,4]
	ret_bbox.append(box)
	
	return ret_bbox