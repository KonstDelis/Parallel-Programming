import java.io.PrintWriter;
import java.util.Scanner;

public class Utility {
    public static double G = 6.67*Math.pow(10,-11);
    public static int iterations=0, thread_no=4;
    public static double spaceSize;
    public static Planet[] planets;
    public static double getDistance(Point p1, Point p2){
        //get sides
        double x_side = Math.pow(Math.abs(p1.x-p2.x), 2);
        double y_side = Math.pow(Math.abs(p1.y-p2.y), 2);
        //use pythagorean theorem
        return Math.sqrt(x_side+y_side);
    }

    public static void getInput(Scanner fin){
        int planetNo = fin.nextInt();
        Utility.planets = new Planet[planetNo];
        Utility.spaceSize = fin.nextDouble();
        System.out.println("Space size = " + Utility.spaceSize);
        fin.nextLine();
        for(int i=0; i<planetNo; i++){
            String[] line = fin.nextLine().split(" ");

            planets[i] = new Planet(line[5], new Point(Double.parseDouble(line[0]), Double.parseDouble(line[1])),Double.parseDouble(line[2]),
                    Double.parseDouble(line[3]), Double.parseDouble(line[4]));

        }
        fin.close();
    }

    public static void writeOutput(PrintWriter fout){
        fout.println(planets.length);
        fout.println(Utility.spaceSize);
        for (Planet planet : Utility.planets) {
            fout.println(planet.position.x + " " +
                    planet.position.x + " " +
                    planet.position.y + " " +
                    planet.vx + " " +
                    planet.vy + " " +
                    planet.mass + " " +
                    planet.name
            );
        }
        fout.close();
    }

    public static void paramError(){
        System.err.println("""
                Error: wrong parameters. Try parameters:\s
                \t'-in filepath.txt': input file (necessary)
                \t'-i iteration_num(int)': number of iterations(necessary)
                \t'-t thread_no(int)': number of threads (optional, default is 4)
                \t'-out filepath.txt': output file (optional, default is 'test5serial.txt')
                """);
        System.exit(-1);
    }
}
