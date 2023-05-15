public class SplitBoundary {
    public Boundary topLeft, topRight, bottomLeft, bottomRight;

    public SplitBoundary(Point minPoint, Point midPoint, Point maxPoint){
        topLeft = (minPoint.x==(maxPoint.x) && minPoint.y==maxPoint.y ) ?
                null : new Boundary(new Point(minPoint.x, minPoint.y), new Point(midPoint.x, midPoint.y));
        topRight = (maxPoint.x==minPoint.x) ?
                null : new Boundary(new Point(midPoint.x, minPoint.y), new Point(maxPoint.x, midPoint.y));
        bottomLeft = (maxPoint.y==minPoint.y) ?
                null : new Boundary(new Point(minPoint.x, midPoint.y), new Point(midPoint.x, maxPoint.y));
        bottomRight = (maxPoint.x==minPoint.x || maxPoint.y==minPoint.y) ?
                null : new Boundary(new Point(midPoint.x, midPoint.y), new Point(maxPoint.x, maxPoint.y));
    }
}
