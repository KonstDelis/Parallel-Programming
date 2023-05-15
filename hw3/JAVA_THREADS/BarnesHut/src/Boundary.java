import java.math.BigDecimal;

public class Boundary {
    public Point minPoint, maxPoint;

    public Boundary(Point minPoint, Point maxPoint){
        this.minPoint = minPoint;
        this.maxPoint = maxPoint;
    }
    public Boundary(){
        System.err.println("Warning creating empty boundary");
        this.minPoint = new Point();
        this.maxPoint = new Point();
    }
    public BigDecimal getMiddleRow(){
        BigDecimal addition = minPoint.x.add(maxPoint.x);
        return addition.divide(new BigDecimal(2));
    }
    public BigDecimal getMiddleColumn(){
        BigDecimal addition = minPoint.y.add(maxPoint.y);
        return addition.divide(new BigDecimal(2));
    }
    public SplitBoundary split(){
        Point midPoint = new Point(getMiddleRow(), getMiddleColumn());
        return new SplitBoundary(minPoint, midPoint, maxPoint);
    }
    public boolean pointIsInBoundary(Point p){
        if(p.x.compareTo(minPoint.x)<=0 || p.x.compareTo(maxPoint.x)>0 || p.y.compareTo(minPoint.y)<=0 || p.y.compareTo(maxPoint.y)>0){
            return false;
        }
        return true;
    }
    BigDecimal getMinSize(){
        BigDecimal size_x = maxPoint.x.subtract(minPoint.x);
        BigDecimal size_y = maxPoint.y.subtract(minPoint.y);
        return size_x.compareTo(size_y)<0 ? size_y : size_x;
    }
}
