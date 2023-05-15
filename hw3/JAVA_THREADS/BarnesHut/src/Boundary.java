public class Boundary {
    public Point minPoint, maxPoint;

    public Boundary(Point minPoint, Point maxPoint){
        this.minPoint = minPoint;
        this.maxPoint = maxPoint;
    }

    public double getMiddleRow(){
        double addition = minPoint.x+maxPoint.x;
        return addition/2.0;
    }
    public double getMiddleColumn(){
        double addition = minPoint.y+maxPoint.y;
        return addition/2.0;
    }
    public SplitBoundary split(){
        Point midPoint = new Point(getMiddleRow(), getMiddleColumn());
        return new SplitBoundary(minPoint, midPoint, maxPoint);
    }
    public boolean pointIsInBoundary(Point p){
        return !(p.x <= minPoint.x) && !(p.x > maxPoint.x) && !(p.y <= minPoint.y) && !(p.y > maxPoint.y);
    }
    double getMinSize(){
        double size_x = maxPoint.x-minPoint.x;
        double size_y = maxPoint.y-minPoint.y;
        return Math.max(size_x, size_y);
    }
}
