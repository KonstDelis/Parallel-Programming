import subprocess
import os

threads = ['1', '2', '4']
iterations = ["5000", "50000"]
tests = ["InputTests/input1.txt","InputTests/input2.txt", "InputTests/input3.txt", "InputTests/input3.txt", "InputTests/input4.txt", "InputTests/input5.txt"]


with open("measureCpp.txt", "w+") as fout:
    for test in tests:
        for iteration in iterations:            
            for thread in threads:
                fout.write("\n\n###########################################"+"Threads:"+thread+" "+iteration+" "+test+"###########################################\n\n")
                fout.flush()
                for i in  range(1,6):
                    fout.write("\nMeasurement "+str(i)+"\n")
                    fout.flush()
                    subprocess.run(["time","./TBB/barnes_hut.out","-in",test,"-i",iteration, "-t",thread], stdout=fout, stderr=fout)
                    fout.flush()
fout.close()
print("Finally done")