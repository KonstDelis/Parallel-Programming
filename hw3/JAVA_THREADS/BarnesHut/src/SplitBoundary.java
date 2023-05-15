public class SplitBoundary {
    public Boundary topLeft, topRight, bottomLeft, bottomRight;

    public SplitBoundary(Point minPoint, Point midPoint, Point maxPoint){
        topLeft = (minPoint.x.compareTo(maxPoint.x)==0 && minPoint.y.compareTo(maxPoint.y)==0 ) ?
                null : new Boundary(new Point(minPoint.x, minPoint.y), new Point(midPoint.x, midPoint.y));
        topRight = (maxPoint.x.compareTo(minPoint.x)==0) ?
                null : new Boundary(new Point(midPoint.x, minPoint.y), new Point(maxPoint.x, midPoint.y));
        bottomLeft = (maxPoint.y.compareTo(minPoint.y)==0) ?
                null : new Boundary(new Point(minPoint.x, midPoint.y), new Point(midPoint.x, maxPoint.y));
        bottomRight = (maxPoint.x.compareTo(minPoint.x)==0 || maxPoint.y.compareTo(minPoint.y)==0) ?
                null : new Boundary(new Point(midPoint.x, midPoint.y), new Point(maxPoint.x, maxPoint.y));
    }
}
