import sys
import Image
from math import *

fontImage = 0
fontHeight = 0
fontWidth = 0
fontPadding = 0

FONT_LOAD_BY_ROWS_MSB_RL = 1
FONT_LOAD_BY_COLS_MSB_RL = 2
FONT_LOAD_BY_ROWS_LSB_RL = 3
FONT_LOAD_BY_COLS_LSB_RL = 4
FONT_LOAD_BY_ROWS_MSB_LR = 5
FONT_LOAD_BY_COLS_MSB_LR = 6
FONT_LOAD_BY_ROWS_LSB_LR = 7
FONT_LOAD_BY_COLS_LSB_LR = 8

fontLoadOrder = FONT_LOAD_BY_ROWS_LSB_RL

# For loading Justin's font: dumpArray(loadFontImage('./...', 8, 16, 1, 5), 8, 16, 0)

def loadFontImage(imgpath, fwidth, fheight, fpadding, floadorder):    
    fontImage = Image.open(imgpath)
    fontImage.load()
    
    fontWidth = fwidth
    fontHeight = fheight
    fontPadding = fpadding
    fontLoadOrder = floadorder

    if((fwidth+fpadding) > fontImage.size[0] or fheight > fontImage.size[1]):
        print "Invalid parameters!"
        return

    fontVals = []

    for i in range(0, fontImage.size[0], fwidth + fpadding):
        tempBinData = 0
        if(floadorder == FONT_LOAD_BY_ROWS_MSB_LR):
            for r in range(0, fheight):
                for c in range(0, fwidth):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((c + i, r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_ROWS_LSB_LR):
            for r in range(0, fheight):
                for c in range(0, fwidth):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((((fwidth-1) - c) + i, r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_COLS_MSB_LR):
            for c in range(0, fwidth):
                for r in range(0, fheight):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((c + i, r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_COLS_LSB_LR):
            for c in range(0, fwidth):
                for r in range(0, fheight):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((c + i, (fheight-1) - r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_ROWS_MSB_RL):
            for r in range(0, fheight):
                for c in range(0, fwidth):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((c + i, (fheight-1) - r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_ROWS_LSB_RL):
            for r in range(0, fheight):
                for c in range(0, fwidth):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((((fwidth-1) - c) + i, (fheight-1) - r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_COLS_MSB_RL):
            for c in range(0, fwidth):
                for r in range(0, fheight):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((((fwidth-1) - c) + i, r)))/3 < 128):
                        tempBinData |= 1
        if(floadorder == FONT_LOAD_BY_COLS_LSB_RL):
            for c in range(0, fwidth):
                for r in range(0, fheight):
                    tempBinData <<= 1
                    if(sum(fontImage.getpixel((((fwidth-1) - c) + i, (fheight-1) - r)))/3 < 128):
                        tempBinData |= 1
        fontVals.append(tempBinData)
    return fontVals

UNROLL_FORWARD = 1
UNROLL_BACKWARD = 0

def dumpArray(fv, width, height, unrolldir):
    sys.stdout.write("{\n")
    for i in range(0, len(fv)):
        for j in range(0, int(ceil((width*height)/8.0))):
            if(unrolldir == UNROLL_FORWARD):
                sys.stdout.write('0x%02x' % (int((fv[i] >> (j*8)) & 0xFF)))
            if(unrolldir == UNROLL_BACKWARD):
                sys.stdout.write('0x%02x' % (int((fv[i] >> (((width*height)/8 - 1 - j)*8)) & 0xFF)))
            if((i != (len(fv) - 1)) or (j != (int(ceil((width*height)/8.0)) - 1))):
                sys.stdout.write(", ")
            if(j == (int(ceil((width*height)/8.0)) - 1)):
                sys.stdout.write(" // Line %d\n" % i)
        #if(i != (len(fv) - 1)):
         #   sys.stdout.write(", ")
    sys.stdout.write("}\n")
        
def generateTemplate(fwidth, fheight, fpadding, fname, numchars):
    fontImage = Image.new('RGB', ((fwidth + fpadding) * numchars, fheight), 'white')
    pix = fontImage.load()
    for i in range(0, fheight, 2):
        for j in range(fwidth, (fwidth + fpadding)*numchars, fwidth + fpadding):
            for k in range(0, fpadding):
                pix[j + k, i + (k % 2)] = (255, 0, 0)
    fontImage.save(fname)
    
