import os
import cv2 as cv
from PIL import Image

Image.MAX_IMAGE_PIXELS = None

def splitimage(level,img, titleSize, dstpath):
    w, h = img.size
    if w >= titleSize  and h >= titleSize:
        print('Original image info: %sx%s, %s, %s' % (w, h, img.format, img.mode))
        print('start split...')

        levelPath = os.path.join(dstpath, str(level))
        isExists = os.path.exists(levelPath)
        if not isExists:
            os.makedirs(levelPath)

        num = 0
        rownum = h // titleSize
        colnum = w // titleSize
        for r in range(rownum):
            rowPath = os.path.join(levelPath, str(r))
            isExists = os.path.exists(rowPath)
            if not isExists:
                os.makedirs(rowPath)
            for c in range(colnum):
                box = (c * titleSize, r * titleSize, (c + 1) * titleSize, (r + 1) * titleSize)
                img.crop(box).save(os.path.join(rowPath, str(c) + '.png'))
                num = num + 1
    else:
    	print('invalid')



titleSize = 128

src = input('img path: ')
if os.path.isfile(src):
    dstpath = input('out file dir: ')
    if (dstpath == '') or os.path.exists(dstpath):
        if titleSize > 0:
            img = Image.open(src)
            w, h = img.size
            level = 0
            while w >= titleSize and h >= titleSize:
                splitimage(level, img, titleSize, dstpath)
                w /= 2
                h /= 2
                img = img.resize((int(w), int(h )))
                level += 1
        else:
            print('error')
    else:
        print('no exist %s file' % dstpath)
else:
    print('no exist %s fil' % src)
