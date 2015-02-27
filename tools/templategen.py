import os, PIL.Image as Image, sys

DIVIDER_COLOR = (225,225,225)
GRID_COLOR = (245,245,245)
BG_COLOR = (255,255,255)

GRID_SIZE = 2

ofilename = ''
img = 0
celldims = [0,0]
ncells = [0,0]

def readlineint():
    return int(sys.stdin.readline().strip('\n\r'))

sys.stdout.write("Enter a filename for the template: ")
ofilename = sys.stdin.readline().strip('\n\r')

sys.stdout.write("Enter the width of each cell: ")
celldims[0] = readlineint()

sys.stdout.write("Enter the height of each cell: ")
celldims[1] = readlineint()

sys.stdout.write("Enter the number cell columns: ")
ncells[0] = readlineint()

sys.stdout.write("Enter the number cell rows: ")
ncells[1] = readlineint()

imgdims = [0,0]

imgdims[0] = celldims[0] * ncells[0]
imgdims[1] = celldims[1] * ncells[1]

img = Image.new('RGB', imgdims)

pix = img.load()

for i in range(imgdims[0]):
    for j in range(imgdims[1]):
        pixcol = BG_COLOR

        if(i % celldims[0] == 0 or j % celldims[1] == 0):
            pixcol = DIVIDER_COLOR
        elif(i % GRID_SIZE == 0 or j % GRID_SIZE == 0):
            pixcol = GRID_COLOR

        pix[i,j] = pixcol       

img.save(ofilename + '.png', 'PNG')
