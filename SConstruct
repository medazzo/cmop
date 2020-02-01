import glob
env = Environment(CPPPATH = ['.'])


print "========================================="
print "Building Library CMOPO  .. "
print "========================================="
sources = glob.glob("src/*.cpp")
#print "Find source files  "
#for s in sources:
#    print "  "+s

Repository('./src', './include')
env.Library('cmopo',sources)

# Later for tests 
#Library('foo', ['f1.c', 'f2.c', 'f3.c'])
#Program('prog.c', LIBS=['foo', 'bar'], LIBPATH='.')

#Deciding If a File Has Changed Using Both MD Signatures and Time Stamps
Decider('MD5-timestamp')
      