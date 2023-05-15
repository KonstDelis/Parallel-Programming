#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

extern long double threshold;
extern long double G;

class Point {
    public:
    long double x;
    long double y;

    Point(long double _x, long double _y);
    Point();

    std::string toString();
};

class Boundary;

struct SplitBoundary{
    Boundary *top_left, *top_right, *bottom_left, *bottom_right;

    SplitBoundary(Point minPoint, Point midPoint, Point maxPoint);
    ~SplitBoundary();

    std::string toString();
};

class Boundary{
    public:
    Point minPoint, maxPoint;

    Boundary(Point minPoint, Point maxPoint);
    Boundary();

    long double getMiddleRow();
    long double getMiddleColumn();
    SplitBoundary* split();
    bool pointIsInBoundary(Point p);
    long double getMinSize();
    std::string toString();
};

struct Forces{
    long double fx;
    long double fy;
    Forces();
    void reset();
};

class Planet{
    public:
    Point position;
    long double vx, vy, mass;
    std::string name;
    Forces forces;

    Planet(std::string name, Point position, long double vx, long double vy, long double mass);
};

class Quad{
    private:
    void calculate_mass_center();

    public: 
    Boundary *boundary;
    Planet *planet;
    bool devided;
    Point center_of_mass;
    long double cluster_mass;
    Quad *children[4];

    Quad();
    Quad(Boundary* b);
    ~Quad();

    bool hasChildren();
    void insertPlanet(Planet* p);
    void calculateForce(Planet* p);
};

class BHtree{
    public:
    Quad *root;
    int maxDepth;
    int nodes;
    int planetNodes;

    BHtree(Boundary *initialSpace);
    BHtree(Boundary *initialSpace, std::vector<Planet*>& planets);
    ~BHtree();
    void insertPlanet(Planet* planet);
    void calculateForce(Planet* p);
};

long double getDistance(Point p1, Point p2);