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



class Simulation {


private:
     Settings * settings;
     Randoms * rand;
     Potential2D * potential;


     double x0 ;
     double y0 ;
     double r ;
     double r_squared;


     double gamma;
      
    
     bool verbose;


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



public:
     Simulation ( Settings * );
     ~Simulation();

     void setGamma(double g) { if(g>0.0) this->gamma = g; }
     void setRadius ( double r ) {
          if ( r>0.0 )  {
            this->r = r;
            this->r_squared = r*r;
          }
          
     }

     /**
      * symulacja az opusci okrag i zwraca t jaki uplynal
      */
     double run ( );


     void setVerbose ( bool v ) {
          this->verbose = v;
     }


};

#endif
