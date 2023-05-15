import java.math.BigDecimal;

public class Quad {
    public Planet planet=null;
    public Boundary boundary=null;
    public Point centerOfMass=null;
    public BigDecimal cluster_mass=null;
    public Quad topLeft=null, topRight=null, bottomLeft=null, bottomRight=null;

    private void calculateMassCenter(){
        BigDecimal mass_sum = new BigDecimal(0);
        BigDecimal x_sum = new BigDecimal(0);
        BigDecimal y_sum = new BigDecimal(0);
        Quad[] children = {topLeft, topRight, bottomLeft, bottomRight};
        for (int i = 0; i < 4; i++)
        {
            if (children[i].hasChildren() || children[i].planet != null)
            { // subtree contains at least one planet leaf
                mass_sum = mass_sum.add(children[i].cluster_mass);
                x_sum = x_sum.add( ( children[i].cluster_mass.multiply(children[i].centerOfMass.x) ) );
                y_sum = y_sum.add( ( children[i].cluster_mass.multiply(children[i].centerOfMass.y) ) );
            }
        }
        cluster_mass = mass_sum;
        centerOfMass = new Point(x_sum.divide(mass_sum), y_sum.divide(mass_sum));
    }

    public Quad(){
    }
    public Quad(Boundary b){
        boundary=b;
    }

    public boolean hasChildren(){
        return (topLeft!=null || bottomLeft!=null ||bottomRight!=null||topRight!=null);
    }
    public void insertPlanet(Planet p){
        if (p==null)
        {
            System.err.println("Error: Cannot insert NULL planet");
            return;
        }
        if (boundary.pointIsInBoundary(p.position))
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
        Quad[] children = {topLeft, topRight, bottomLeft, bottomRight};
        if (!hasChildren())
        {
            if (p.position.x.compareTo(planet.position.x)==0 && p.position.y.compareTo(planet.position.y)==0) {
                System.err.println("Cannot insert planets at the same point");
                return;
            }
            SplitBoundary split = boundary.split();
            topLeft = new Quad(split.topLeft);
            topRight = new Quad(split.topRight);
            bottomLeft = new Quad(split.bottomLeft);
            bottomRight = new Quad(split.bottomRight);

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
            if(planet==p) return ; //don't calculate force from self
            BigDecimal d = Utility.getDistance(p.position, planet.position);
            BigDecimal f = planet.mass.multiply(p.mass).multiply( d.pow(2) ).multiply(new BigDecimal(Utility.G));
            p.forces.fx = p.forces.fx.add( planet.position.x.subtract(p.position.x).divide(d).multiply(f) );
            p.forces.fy = p.forces.fy.add( planet.position.y.subtract(p.position.y).divide(d).multiply(f) );
            return ;
        }
        //if it is far away enough, use cluster
        if(!boundary.pointIsInBoundary(p.position) && hasChildren() && Utility.getDistance(p.position, centerOfMass).compareTo(boundary.getMinSize()) >= 0){
            BigDecimal d = Utility.getDistance(p.position, centerOfMass);
            BigDecimal f = cluster_mass.multiply(p.mass).divide( d.pow(2)).multiply(new BigDecimal(Utility.G));
            p.forces.fx = p.forces.fx.add( centerOfMass.x.subtract(p.position.x).divide(d).multiply(f) );
            p.forces.fy = p.forces.fy.add( centerOfMass.y.subtract(p.position.y).divide(d).multiply(f) );
            return ;
        }

        //else recursively call for all children (if they exist)
        if(topLeft!=null) topLeft.calculateForce(p);
        if(topRight!=null) topRight.calculateForce(p);
        if(bottomLeft!=null) bottomLeft.calculateForce(p);
        if(bottomRight!=null) bottomRight.calculateForce(p);
        return ;
    }

    static void main(String[] args){

    }
}
