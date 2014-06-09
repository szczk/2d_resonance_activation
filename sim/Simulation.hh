#ifndef __SIMULATION_HH__
#define __SIMULATION_HH__

#include "../core/Randoms.hh"
#include "../core/Settings.hh"
#include "Potential2D.hh"
// #include "../main.hh"
// #include <map>
// #include <gsl/gsl_histogram2d.h>

using namespace std;

struct  point {
     double x;
     double y;

     point ( double x=0.0, double y=0.0 )  : x ( x ), y ( y ) {
     }

     point operator+ ( const point& a ) const {
          return point ( a.x+x, a.y+y );
     }
};



struct vec {
     double x;
     double y;
};


class Simulation {


private:
     Settings * settings;
     Randoms * rand;


     double x0 ;
     double y0 ;
     double r ;
     double r_squared;


     bool dump_gnuplot;
     bool verbose;


     double angle_start;
//   double angle_end;


     /**
      * Initialize anything that needs to be initialized
      */
     void init();

     /**
      *  Close anything that needs to be closed.
      */
     void destroy();


     /**
      * Reset all and prepare for new simulations
      */
     void reset();

     /**
      * if the given landing point is still in circle
      */
     bool inCircle ( point& );


     /**
      *  vectors dot product
      */
     double dotProduct ( const vec & v1, const vec & v2 );

     /**
      * get distance between points
      */

     double getDistance ( const point & p1 ,const point & p2 );

     /**
      * normalize vector
      */
     void norm ( vec & v );

     /*
      * obliczanie odbicia, przy zalozeniu ze start_point lezy w okregu
      * a end_point za okregiem
      *
      */
     point getNewTarget ( const point & start_point ,const point & end_point , point& odbicie_ref , ofstream & );


     /**
      * sprawdzanie czy punkt lezy na luku [0, angle]
      *
      */
     bool pointOnArc ( double angle, point& );

public:
     Simulation ( Settings * );
     ~Simulation();


     void setRadius ( double r ) {
          if ( r>0.0 ) this->r = r;
     }

     /**
      * Simulate up to t = endTime and return value of X(t)
      */
     double run ( double angle_end );


     void setVerbose ( bool v ) {
          this->verbose = v;
     }


     void setDumpGnuplot ( bool dg ) {
          this->dump_gnuplot = dg;
     }


};

#endif
