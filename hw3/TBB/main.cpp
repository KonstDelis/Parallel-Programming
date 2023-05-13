#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string.h>
#include "BHtree.hpp"
using namespace std;

#define Dt 1

static long int iterations=-1;
static int planet_no=0;
static long double space_size=0;

vector<Planet *> planets;

void changePosition(Planet *p){
    long double a_x = p->forces.fx/p->mass, a_y = p->forces.fy/p->mass;
    p->vx = p->vx + (a_x * Dt);
    p->vy = p->vy + (a_y * Dt); 
    p->position.x = p->position.x + (p->vx * Dt);
    p->position.y = p->position.y + (p->vy * Dt);
    // cout<<p->name<<" | New Position("<<p->position.x<<", "<<p->position.y<<endl;
    p->forces.reset();
}

void getInput(ifstream& fin){
    fin>>planet_no;
    fin>>space_size;
    for(int i=0; i<planet_no && !fin.eof(); i++){
        long double x, y, vx, vy, mass;
        string name;
        fin>>x>>y>>vx>>vy>>mass>>name;
        planets.push_back(new Planet(name, Point(x,y), vx, vy, mass));
    }
    fin.close();
}
void writeResults(ofstream& fout){
    fout<<planet_no<<endl;
    fout<<space_size<<endl;
    for(int i=0; i<planet_no && !fout.eof(); i++){
        fout<<planets[i]->position.x<<" "<<planets[i]->position.y<<
            " "<<planets[i]->vx<<" "<<planets[i]->vy<<
            " "<<planets[i]->mass<<" "<<planets[i]->name<<
            endl;  
    }
    fout.close();
}

void simulate(){
    for(int iteration=0; iteration<iterations; iteration++){
        BHtree *q = new BHtree(new Boundary(Point(-1*space_size,-1*space_size), Point(space_size, space_size)), planets); 
        //parallelize
        for(int i=0; i<planets.size(); i++){
            q->calculateForce(planets[i]);
            //cout<<planets[i]->name<<": fx="<<planets[i]->forces.fx<<" | fy="<<planets[i]->forces.fy<<endl;
        }

        for(int i=0; i<planets.size(); i++){
            //cout<<"p size"<<planets.size()<<endl;
            changePosition(planets[i]);
        }

        // cout<<"------------------------------------\n";
        delete q;
    }
}

void paramError(){
    cerr<<"Error: wrong parameters. Try parameters: "<<endl<<
            "\t'-in filepath.txt': input file (neccessary)"<<endl<<
            "\t'-i iteration_num(int)': number of iterations(neccessary)"<<endl<<
            "\t'-t thread_no(int)': number of threads (optional, default is 4)"<<endl<<
            "\t'-out filepath.txt': output file (optional, default is 'output.txt')"<<endl;
    exit(-1);
}

int main(int argc, char* argv[])
{
    //-------------------HANDLE ARGS---------------------
    int thread_no = 4;
    string foutpath = "output.txt";
    string finpath = "";
    if(argc != 5 && argc != 7 && argc !=9) paramError();

    for(int i=1; i<argc; i+=2){
        if(strcmp(argv[i],"-in")==0)
            finpath=argv[i+1];
        if(strcmp(argv[i],"-out")==0)
            foutpath=argv[i+1];
        if(strcmp(argv[i],"-t")==0)
            thread_no=stoi(argv[i+1]);
        if(strcmp(argv[i],"-i")==0)
            iterations=stoi(argv[i+1]);
    }
    if(iterations<1){
        cerr<<"Error: number of iterations should be a positive integer"<<endl;
        paramError();
    }
    if(thread_no<1 || thread_no>4){
        cerr<<"Error: threads must be a number between 1 and 4"<<endl;
        return -1;
    }
    if(finpath.empty()){
        cerr<<"Error: '-in inputfile.txt' is a neccessary parameter"<<endl;
        return -1;
    }
    ifstream fin;
    fin.open(finpath, ios::in);
    if(!fin.is_open()){
        cerr<<"Error: unable to open input file '"<<finpath<<"'"<<endl;
    }
    ofstream fout;
    fout.open(foutpath, ios::out);
    if(!fout.is_open()){
        cerr<<"Error: unable to find output file '"<<foutpath<<"'"<<endl;
    }
    //---------------------------------------------------

    //gets input, places planets to "planets" vector
    getInput(fin);
    //simulation starts
    simulate();
    //write results in file
    writeResults(fout);
    //free planets
    for(int i=0; i<planets.size(); i++){
        if(planets[i]) delete planets[i];
    }
}