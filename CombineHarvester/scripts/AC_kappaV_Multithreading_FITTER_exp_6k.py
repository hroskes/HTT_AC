# Simple script to multithread the process of the KappaF, KappaV 2d grid
# Adjust the values below:
#  - nCores = number of cores running multithreaded process
#  - points = number of total points
#
# points / nCores = nToysPerJob you will need to run



from multiprocessing import Process
import subprocess
import math




nCores = 8 # Don't set this too high.  It can crash your machine
points = 5929 # 2000 gives very nice contours
cvMax = 2.0



def runFit(points, toys, j) :

    bottom = int(j)*toys
    top = ((int(j)+1)*toys)-1

    toRun = []
    toRun.append('combineTool.py')
    toRun.append('-n')
    toRun.append('CvAC_FITTER_exp_m0p1to0p1_6k.%i' % j)
    toRun.append('-M')
    toRun.append('MultiDimFit')
    toRun.append('-m')
    toRun.append('125')
    toRun.append('-t')
    toRun.append('-1')
    toRun.append('--expectSignal=1') 
    toRun.append('--preFitValue=1.') 

    toRun.append('--setPhysicsModelParameterRanges')
    toRun.append('muV=0.0,%s:muf=0.0,10.0:CMS_zz4l_fai1=-0.1,0.1' % cvMax)
    toRun.append('cmb/125/fa03_Interference_Workspace_cmb.root')
    toRun.append('--algo=grid')
    toRun.append('--points=%i' % points)

    toRun.append('--setPhysicsModelParameters')
    toRun.append('muV=1.,CMS_zz4l_fai1=0.,muf=1.') 
    #toRun.append('-t')
    #toRun.append('-1')
    #toRun.append('--expectSignal=1')
    toRun.append('--minimizerAlgoForMinos=Minuit2,Migrad')
    toRun.append('-P') 
    toRun.append('CMS_zz4l_fai1') 
    toRun.append('-P') 
    toRun.append('muV') 
    toRun.append('--floatOtherPOIs=1')
    toRun.append('--robustFit=1')
    toRun.append('--firstPoint')
    toRun.append('%i' % bottom)
    toRun.append('--lastPoint')
    toRun.append('%i' % top)
    #    toRun.append('-t') 
#    toRun.append('-1')
#    toRun.append('--expectSignal=1')
    toRun.append('--X-rtd') 
    toRun.append('FITTER_NEW_CROSSING_ALGO') 
    toRun.append('--minimizerToleranceForMinos=0.1') 
    toRun.append('--X-rtd')
    toRun.append('FITTER_NEVER_GIVE_UP') 
    toRun.append('--X-rtd') 
    toRun.append('FITTER_BOUND') 
    toRun.append('--minimizerAlgo=Minuit2') 
    toRun.append('--minimizerStrategy=0') 
    toRun.append('--minimizerTolerance=0.1') 
    toRun.append('--cminFallbackAlgo') 
    toRun.append('\"Minuit2,0:1.\"')

    #toRun.append('--fastScan') # This does not re-profile the nuisances, it is really fast, but produces low quality results
    #toRun.append('')

    subprocess.call( toRun )

# CH takes your nPointsIn and chooses the next largest integer
# value that is a perfect square as its input nPoints
# So we want to match that if we are multithreading
def findNPoints( nPointsIn ) :
    toSquare = math.sqrt( nPointsIn )
    print "Sqrt( %i ) = %.2f" % (nPointsIn, toSquare)
    return math.ceil( toSquare ) * math.ceil( toSquare )


nPoints = findNPoints( points )
print "nPoints ",nPoints
#jobs = int(nPoints/nToysPerJob)+1
nToysPerJob  = int(nPoints/nCores)+1 # +1 makes sure we get final points
print "nCores ",nCores
print "nToysPerJob ",nToysPerJob
processes = []

for i in range( nCores ) :
    processes.append( Process(target=runFit, args=(nPoints, nToysPerJob, i,) ) )
    processes[-1].start()

# Make sure all jobs finish
for process in processes :
    if process.is_alive() :
        print "Waiting for process pid: ",process.pid
        process.join()



print "\n\n"
print "Now Run:"
print "hadd higgsCombineCvCf.MultiDimFit.mH125.root higgsCombineCvCf.*.MultiDimFit.mH125.root"
print "python ../../../../scripts/plotMultiDimFit.py higgsCombineCvCf.MultiDimFit.mH125.root --sm-exp X"
#print "root -l higgsCombineCvCf.MultiDimFit.mH125.root ../../../../scripts/contours2D.cxx"
#print 'contour2D("CV",%i,0.0,%s,"CF",%i,0.0,2.0,1.,1.)' % (math.sqrt(nPoints), cvMax, math.sqrt(nPoints))
print "\n\n"



