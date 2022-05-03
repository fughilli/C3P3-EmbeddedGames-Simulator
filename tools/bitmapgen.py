import os, PIL.Image as Image, sys

ifilename = ''
img = 0

if(len(sys.argv) > 1):
    ifilename = sys.argv[1]
else:
    sys.stdout.write("Enter a file to convert to a bitmap: \n")
    ifilename = sys.stdin.readline().strip('\n\r')
    
try:
    img = Image.open(ifilename)
except:
    print("Error opening file %s\n" % ifilename)
    exit(1)

# do image stuff

ifnamenoext = os.path.split(ifilename)[-1]
ifnamenoext = os.path.splitext(ifnamenoext)[0]

ofnamenoext = os.path.splitext(ifilename)[0]

guardsym = '_BITMAP_' + ('_'.join(ifnamenoext.split(' '))).upper() + '_H_'

try:
    ohfile = open(ofnamenoext + '.h', 'w')
    ocfile = open(ofnamenoext + '.c', 'w')
except:
    print("Error opening output files\n")

imgwidth = img.size[0]
imgheight = img.size[1]

outputBytes = []
byteAccum = 0

def getBWPixel(xy):
    pix = img.getpixel(xy)
    pixAvg = 0
    if(isinstance(pix, tuple)):
        pixAvg = sum(pix)/len(pix)
    else:
        pixAvg = pix
    center = 1

    #print pixAvg

    return (1 if pixAvg >= center else 0)

def binString(val, length):
    binstr = bin(val)
    startstr = binstr[2:]
    if(length <= len(startstr)):
        return binstr

    return '0b' + '0'*(length - len(startstr)) + startstr

ohfile.write("""
#ifndef %s\n
#define %s\n
\n
#ifdef __cplusplus\nextern \"C\" {\n
#endif\n
\n
#include <stdint.h>\n
\n
extern const uint8_t %s[];\n
\n
#ifdef __cplusplus\n
}\n
#endif\n
\n
#endif // %s\n
""" % (guardsym, guardsym, ifnamenoext, guardsym))

ohfile.close()

ocfile.write("""
#include "%s"\n
\n
""" % (ifnamenoext + '.h',))

ocfile.write("""
const uint8_t %s[] = {\n
""" % (ifnamenoext,))


for i in range(imgheight):
    for j in range(imgwidth):
        byteAccum <<= 1
        byteAccum |= getBWPixel((j, i))
        if(not ((j + 1) % 8)):
            outputBytes.append(byteAccum)
            byteAccum = 0
    if(imgwidth % 8):
        outputBytes.append(byteAccum)
        byteAccum = 0

    for obyte in outputBytes:
        ocfile.write(binString(obyte, 8) + ', ')
    outputBytes = []
    ocfile.write('\n')

ocfile.write("""
};\n
\n
""")

ohfile.close()
ocfile.close()
