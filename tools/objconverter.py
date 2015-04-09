import sys

objfile = 0

if(len(sys.argv) == 2):
    objfile = open(sys.argv[1]).read().split('\n')
else:
    sys.stdout.write("Enter filename of .obj file to read: ")
    objfile = open(sys.stdin.readline().translate(None, '\n'))

vlines = []
flines = []

ofile = open("./output.txt", "w")

for line in objfile:
    if(line == ""):
        continue
    if(line[0] == '#'):
        continue
    try:
        {
            'v':vlines,
            'f':flines
        }[line[0]].append(line[1:])
    except:
        print "Ignoring unknown .obj line specifier: %s\n" % line[0],

unproc_vbuf = []
unproc_fbuf = []

for line in vlines:
    unproc_vbuf.append([float(x) for x in line.translate(None, ',\n').split()])

for line in flines:
    unproc_fbuf.append([int(x)-1 for x in line.translate(None, ',\n').split()])

#sys.stdin.readline()

v_vals = set()

vbuf = []
fbuf = unproc_fbuf # eventually handle triangulation of faces

# Normalize the vector vertices to produce the integer coordinates
for line in unproc_vbuf:
    for val in line:
        if abs(val - int(val)) > 0.0001:
            v_vals.add(val)

minv_val = min(v_vals)/10

print "Factor: %f\n" % minv_val,
ofile.write("Factor: %f\n" % minv_val,)

for line in unproc_vbuf:
    vbuf.append([int(x/minv_val) for x in line])

#print fbuf
#print vbuf

print "Vertex table, length %d\n" % (3*len(vbuf)),
ofile.write("Vertex table, length %d\n" % (3*len(vbuf)),)
for line in vbuf:
    sys.stdout.write(", ".join([str(x) for x in line]))
    ofile.write(", ".join([str(x) for x in line]) + ",\n")
    print ","

print "Face list, length %d\n" % (3*len(fbuf)),
ofile.write("Face list, length %d\n" % (3*len(fbuf)),)
for line in fbuf:
    sys.stdout.write(", ".join([str(x) for x in line]))
    ofile.write(", ".join([str(x) for x in line]) + ",\n")
    print ","

ofile.close()

sys.stdin.readline()
