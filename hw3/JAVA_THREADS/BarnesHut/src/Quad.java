import java.math.BigDecimal;
import java.math.MathContext;
import java.util.ArrayList;

public class Quad {
    public Planet planet=null;
    public Boundary boundary=null;
    public Point centerOfMass=null;
    public double cluster_mass=0.0;
    public Quad [] children = new Quad[4];

    private void calculateMassCenter(){
        double mass_sum = 0.0;
        double x_sum = 0.0;
        double y_sum = 0.0;

        for (int i = 0; i < 4; i++)
        {
            if (children[i].hasChildren() || children[i].planet != null)
            { // subtree contains at least one planet leaf
                mass_sum += children[i].cluster_mass;
                x_sum +=  children[i].cluster_mass * children[i].centerOfMass.x;
                y_sum += children[i].cluster_mass * children[i].centerOfMass.y;
            }
        }
        cluster_mass = mass_sum;
        centerOfMass = new Point(x_sum/mass_sum, y_sum/mass_sum);
    }

    public Quad(){
    }
    public Quad(Boundary b){
        boundary=b;
    }

    public boolean hasChildren(){
        return (children[0]!=null || children[1]!=null || children[2]!=null|| children[3]!=null);
    }
    public void insertPlanet(Planet p){
        if (p==null)
        {
            System.err.println("Error: Cannot insert NULL planet");
            return;
        }
        if (!boundary.pointIsInBoundary(p.position))
        {
            System.err.println("Error: Cannot insert '"+p.name+"' planet that is outside of the space that is examined");
            return;
        }

        if (!hasChildren() && planet == null)
        {
            planet = p;
            cluster_mass = p.mass;
            centerOfMass = p.position;
            return;
        }

        if (!hasChildren())
        {
            if (p.position.x==planet.position.x && p.position.y==planet.position.y) {
                System.err.println("Cannot insert planets at the same point");
                return;
            }
            SplitBoundary split = boundary.split();
            children[0] = new Quad(split.topLeft);
            children[1] = new Quad(split.topRight);
            children[2] = new Quad(split.bottomLeft);
            children[3] = new Quad(split.bottomRight);

            for (int i = 0; i < 4; i++)
            {
                if (children[i].boundary.pointIsInBoundary(planet.position))
                { // add old planet to the new leaf
                    children[i].insertPlanet(planet);
                    planet = null;
                    break;
                }
            }

            for (int i = 0; i < 4; i++)
            {
                if (children[i].boundary.pointIsInBoundary(p.position))
                { // use recursion to add the new planet
                    children[i].insertPlanet(p);
                    calculateMassCenter();
                    return;
                }
            }
            System.err.println("Unexpected error: line "+Thread.currentThread().getStackTrace()[2].getLineNumber());
        }

        if (hasChildren())
        {
            for (int i = 0; i < 4; i++)
            {
                if (children[i].boundary.pointIsInBoundary(p.position))
                { // use recursion to add the new planet
                    children[i].insertPlanet(p);
                    calculateMassCenter();
                    return;
                }
            }
        }
    }
    public void calculateForce(Planet p){
        if(planet != null && planet==p){
            //Reached the planet
            return;
        }
        //if we reached a leaf directly use planet
        if(planet != null){
            double d = Utility.getDistance(p.position, planet.position);
            double f = Utility.G * planet.mass * p.mass / Math.pow(d,2) ;
            p.forces.fx += f * (planet.position.x - p.position.x) / d;
            p.forces.fy += f * (planet.position.y - p.position.y) / d;
            return ;
        }
        //if it is far away enough, use cluster
        if(!boundary.pointIsInBoundary(p.position) && hasChildren() && Utility.getDistance(p.position, centerOfMass)>=boundary.getMinSize()){
            double d = Utility.getDistance(p.position, centerOfMass);
            double f = ( Utility.G* cluster_mass * p.mass / Math.pow(d,2) );
            p.forces.fx += f * (centerOfMass.x - p.position.x) / d;
            p.forces.fy += f * (centerOfMass.y - p.position.y) / d;
            return ;
        }

        //else recursively call for all children (if they exist)
        if(children[0]!=null) children[0].calculateForce(p);
        if(children[1]!=null) children[1].calculateForce(p);
        if(children[2]!=null) children[2].calculateForce(p);
        if(children[3]!=null) children[3].calculateForce(p);
        return ;
    }

    public static void main(String[] args){
        ArrayList<Planet> planets = new ArrayList<Planet>();
        planets.add(new Planet("A", new Point(15, 15), 10, 1, 80));
        planets.add(new Planet("B", new Point(5, 15), 10, 1, 80));
        planets.add(new Planet("C", new Point(15, 5), 10, 1, 80));
        planets.add(new Planet("D", new Point(2.5, 2.5), 10, 1, 20));
        planets.add(new Planet("E", new Point(7.5, 2.5), 10, 1, 20));
        planets.add(new Planet("F", new Point(2.5, 7.5), 10, 1, 20));
        planets.add(new Planet("G1", new Point(6.25, 6.25), 10, 1, 5));
        planets.add(new Planet("G2", new Point(8.75, 6.25), 10, 1, 5));
        planets.add(new Planet("G3", new Point(6.25, 8.75), 10, 1, 5));
        planets.add(new Planet("G4", new Point(8.75, 8.75), 10, 1, 5));
        BHtree q = new BHtree(new Boundary(new Point(), new Point(20, 20)));

        for(Planet planet : planets){
            System.out.println("Adding "+planet.name);
            q.insertPlanet(planet);
        }

        System.out.println("Root: sum mass = "+q.root.cluster_mass+ " || center of mass = ("+ q.root.centerOfMass.x+", "+ q.root.centerOfMass.y+")");
        System.out.println("TL: sum mass = "+q.root.children[0].cluster_mass+ " || center of mass = ("+ q.root.children[0].centerOfMass.x+", "+ q.root.children[0].centerOfMass.y+")");
        System.out.println("TR: sum mass = "+q.root.children[1].cluster_mass+ " || center of mass = ("+ q.root.children[1].centerOfMass.x+", "+ q.root.children[1].centerOfMass.y+")");
        System.out.println("BL: sum mass = "+q.root.children[2].cluster_mass+ " || center of mass = ("+ q.root.children[2].centerOfMass.x+", "+ q.root.children[2].centerOfMass.y+")");
        System.out.println("BR: sum mass = "+q.root.children[3].cluster_mass+ " || center of mass = ("+ q.root.children[3].centerOfMass.x+", "+ q.root.children[3].centerOfMass.y+")");

    }
}
