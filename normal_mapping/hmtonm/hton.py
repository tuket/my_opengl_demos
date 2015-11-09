import sys
import math
from sys import argv

if len(argv) == 2:
	inFile = argv[1]
	outFile = argv[1]
elif len(argv) == 3:
	inFile = argv[1]
	outFile = argv[2]
else:
	print("Error");
	sys.exit(1)
	
from PIL import Image

def sumTup(tup1, tup2):
	res = tup1
	for i in xrange(len(tup1)):
		res[i] += tup2[i]
	return res

def getLength(tup):
	res = 0;
	for x in tup:
		res += x*x
	res = math.sqrt(res)
	return res

im = Image.open(inFile)
width, height = im.size;
dat = im.getdata()

gs = [0] * len(dat)
for i in xrange(len(dat)):
	gs[i] = int( (dat[i][0]+dat[i][1]+dat[i][2])/3 )

nm = [(0,0,0)] * len(dat)
for y in xrange(1, height-1):
	for x in xrange(1, width-1):
		
		v = [0, 0, 0]
		for xx in xrange(-1, 2):
			for yy in xrange(-1, 2):
				t = (xx*255, yy*255, gs[(y+yy)*width+x+xx])
				v = sumTup(v, t)
		
		l = getLength(v)
		for i in xrange(len(v)):
			v[i] /= l
		print(v);
		
