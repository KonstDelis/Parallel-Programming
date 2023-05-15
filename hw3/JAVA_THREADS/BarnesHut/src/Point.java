import java.math.BigDecimal;

public class Point {
    public BigDecimal x;
    public BigDecimal y;

    Point(BigDecimal x, BigDecimal y){
        this.x = x;
        this.y = y;
    }
    Point(){
        x=new BigDecimal("0.0");
        y=new BigDecimal("0.0");
    }
}
