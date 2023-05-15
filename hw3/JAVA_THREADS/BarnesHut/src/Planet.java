public class Planet {
    public Point position;
    public double vx,vy,mass;
    public String name;
    public Forces forces;
    Planet(String name, Point position, double vx, double vy, double mass){
        this.name = name;
        this.position = position;
        this.vx = vx;
        this.vy = vy;
        this.mass = mass;
        this.forces = new Forces();
    }

}
