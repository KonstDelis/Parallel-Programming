import java.math.BigDecimal;

public class Forces {
    public BigDecimal fx, fy;

    public Forces(){
        reset();
    }

    public void reset(){
        fx = new BigDecimal(0);
        fy = new BigDecimal(0);
    }
}
