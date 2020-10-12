import os
import cv2 as cv
from PIL import Image

def splitimage(src, rownum, colnum, dstpath):
    img = Image.open(src)
    w, h = img.size
    if rownum <= h and colnum <= w:
        print('Original image info: %sx%s, %s, %s' % (w, h, img.format, img.mode))
        print('start split...')

        s = os.path.split(src)
        if dstpath == '':
            dstpath = s[0]
        fn = s[1].split('.')
        basename = fn[0]
        ext = fn[-1]

        num = 0
        rowheight = h // rownum
        colwidth = w // colnum
        for r in range(rownum):
            path = os.path.join(dstpath, str(r))
            isExists = os.path.exists(path)
            if not isExists:
                os.makedirs(path)
            for c in range(colnum):
                box = (c * colwidth, r * rowheight, (c + 1) * colwidth, (r + 1) * rowheight)
                img.crop(box).save(os.path.join(dstpath, str(r) + '/' + str(c) + '.' + ext), ext)
                num = num + 1
    else:
    	print('invalid')

src = input('img path: ')
if os.path.isfile(src):
    dstpath = input('out file dir: ')
    if (dstpath == '') or os.path.exists(dstpath):
        row = int(input('rows'))
        col = int(input('cols'))
        if row > 0 and col > 0:

            splitimage(src, row, col, dstpath)
        else:
            print('error')
    else:
        print('no exist %s file' % dstpath)
else:
    print('no exist %s fil' % src)
    
    
    
    import cv2 as cv
 

img = cv.imread('d:/temp/terrain.png')
 
x, y = img.shape[0:2]

cout = 0

while x > 2 and y > 2:
	img_test1 = cv.resize(img, (int(x / 2), int(y / 2)))
	imgname = 'd:/temp/' + str(cout) + '.png'
	cv.imwrite(imgname,img_test1)
	del img	
	img = img_test1
	x /= 2
	y /= 2
	cout += 1
