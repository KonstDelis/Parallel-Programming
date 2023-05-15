import java.util.ArrayList;

public class BHtree {
    Quad root;

    public BHtree(Boundary initialSpace){
        root = new Quad(initialSpace);
    }
    public BHtree(Boundary initialSpace, Planet[] planets){
        root = new Quad(initialSpace);
        for(Planet planet : planets){
            insertPlanet(planet);
        }
    }
    public void insertPlanet(Planet planet){
        root.insertPlanet(planet);
    }
    public void calculateForce(Planet p){
        root.calculateForce(p);
    }
}
