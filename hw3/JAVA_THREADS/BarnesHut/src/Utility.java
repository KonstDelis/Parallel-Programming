import java.math.BigDecimal;
import java.math.MathContext;

public class Utility {
    public static double threshold = 0.5;
    public static double G = 6.67*Math.pow(10,-11);
    public static BigDecimal getDistance(Point p1, Point p2){
        //get sides
        BigDecimal x_side = (p1.x.subtract(p2.x)).abs().pow(2);
        BigDecimal y_side = (p1.y.subtract(p2.y)).abs().pow(2);
        //use pythagorean theorem
        return (x_side.add(y_side)).sqrt(MathContext.DECIMAL128);
    }
}
