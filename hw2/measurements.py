import subprocess
import os

threads = ['1', '2', '4']
iterations = ["100", "1000"]
tests = ["tests/glid1.txt", "tests/glid2.txt"]

with open("measure.txt", "w+") as fout:
    for test in tests:
        for iteration in iterations:
            fout.write("\n\n###########################################"+"Serial "+iteration+" "+test+"###########################################\n\n")
            fout.flush()
            for i in  range(1,6):
                    fout.write("\nMeasurement "+str(i)+"\n")
                    fout.flush()
                    subprocess.run(["time","./hw2serial.out",test, iteration, "out.txt"], stdout=fout, stderr=fout)
                    fout.flush()
            
            
            for thread in threads:
                fout.write("\n\n###########################################"+"Threads:"+thread+" "+iteration+" "+test+"###########################################\n\n")
                fout.flush()
                for i in  range(1,6):
                    fout.write("\nMeasurement "+str(i)+"\n")
                    fout.flush()
                    subprocess.run(["time","./hw2parallel.out",test, iteration, "out.txt"], stdout=fout, stderr=fout, env=dict(os.environ, OMP_NUM_THREADS=thread))
                    fout.flush()