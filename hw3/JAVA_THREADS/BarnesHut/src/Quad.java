


public class Quad {
    public Planet planet=null;
    public Boundary boundary;
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
    }
}
