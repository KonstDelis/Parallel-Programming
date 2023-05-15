import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.ForkJoinTask;

public class BarnesHut {
    public static double Dt = 1;
    public static void changePosition(Planet p){
        double a_x = p.forces.fx/p.mass, a_y = p.forces.fy/p.mass;
        p.vx = p.vx + (a_x * Dt);
        p.vy = p.vy + (a_y * Dt);
        p.position.x = p.position.x + (p.vx * Dt);
        p.position.y = p.position.y + (p.vy * Dt);
        //System.out.println(p.name+": New Position = ("+p.position.x+", "+p.position.y+")");
        p.forces.reset();
    }

    public static void simulate() throws InterruptedException {
        ForkJoinPool forkJoinPool = new ForkJoinPool(Utility.thread_no);
        for(int i=0; i<Utility.iterations; i++){
            //Construct Tree
            BHtree bhtree= new BHtree(new Boundary(
                    new Point( Utility.spaceSize*(-1)-1,Utility.spaceSize*(-1)-1),
                    new Point(Utility.spaceSize+1, Utility.spaceSize+1)),
                    Utility.planets);
            //Parallel calculate forces
            ForkJoinTask<?> forceCalculation_t = forkJoinPool.submit(() -> Arrays.stream(Utility.planets).parallel().forEach(planet -> {
                bhtree.calculateForce(planet);
            }));
            // Wait for task1 to complete before starting task2
            forceCalculation_t.join();
            //Parallel change to new position
            ForkJoinTask<?> changePosition_t = forkJoinPool.submit(() -> Arrays.stream(Utility.planets).parallel().forEach(planet -> {
                changePosition(planet);
            }));
            // Wait for both tasks to complete
            changePosition_t.join();
        }
    }

    public static void main(String[] args) {
        Utility.thread_no = 4;
        String foutpath = "test5serial.txt";
        String finpath = "";

        //--------------------------Handle Args-------------------------
        if(args.length != 4 && args.length != 6 && args.length != 8)
            Utility.paramError();

        for(int i=0; i<args.length; i+=2){
            if(args[i].equals("-in"))
                finpath=args[i+1];
            if(args[i].equals("-out"))
                foutpath=args[i+1];
            if(args[i].equals("-t"))
                Utility.thread_no=Integer.parseInt(args[i+1]);
            if(args[i].equals("-i"))
                Utility.iterations=Integer.parseInt(args[i+1]);
        }
        if(Utility.iterations<1){
            System.err.println("Error: number of iterations should be a positive integer");
            Utility.paramError();
        }
        if(Utility.thread_no<1 || Utility.thread_no>4){
            System.err.println("Error: threads must be a number between 1 and 4");
            System.exit(-1);
        }
        if(finpath.isEmpty()){
            System.err.println("Error: '-in inputfile.txt' is a neccessary parameter");
            System.exit(-1);
        }
        Scanner fin = null;
        try {
            fin = new Scanner(new File(finpath));
        } catch (FileNotFoundException e) {
            System.err.println("Error: unable to open input file '"+finpath+"'");
            System.exit(-1);
        }

        PrintWriter fout = null;
        try {
            fout = new PrintWriter(foutpath);
        } catch (FileNotFoundException e) {
            System.err.println("Error: unable to open output file '"+foutpath+"'");
            System.exit(-1);
        }
        //---------------------------------------------------

        System.out.println("Thread no = "+Utility.thread_no+"\nIterations = "+Utility.iterations);
        Utility.getInput(fin);

        try {
            simulate();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        Utility.writeOutput(fout);

    }
}