import java.math.BigDecimal;

public class Planet {
    public Point position;
    public BigDecimal vx,vy,mass;
    public String name;
    public Forces forces;
    Planet(String name, Point position, BigDecimal vx, BigDecimal vy, BigDecimal mass){
        this.name = name;
        this.position = position;
        this.vx = vx;
        this.vy = vy;
        this.mass = mass;
        this.forces = new Forces();
    }

}
