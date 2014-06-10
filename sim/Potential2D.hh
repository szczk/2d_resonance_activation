#ifndef __POTENTIAL2D__CC__
#define __POTENTIAL2D__CC__

#include "Randoms.hh"
#include "Settings.hh"

using namespace std;


/**
 *  Represents fluctuating potential 2D barrier 
 * 
 *  V(x,y) = H * r
 * 
 *
 *  barrier fluctuates from  top_value
 * to bottom_value
 *
 * with rates:
 *
 *  gamma_up  - rate of switching from down state to up state
 *  gamma_down - rate of switching from up state to down state
 *
 *
 *
 *
 */


class Potential2D {

private:

    long double timeOfNextSwitch;
  
    double gamma_up; //rate of down_state jumping to up_state

    double gamma_down;//rate of up_state jumping to down_state


    double top_value;
    double bottom_value;

    Randoms * randoms;


    enum barrier_state { up_state, down_state};

    /**
     *  Current barier state
     */
    barrier_state state;



    /**
     * Initialize random number generator
     * and barrier state.
     */
    void init();

    /**
     *  destroy and close
     */
    void cleanUp();


    /**
     * Switch barrier state and draw new waiting time
     */
    void switchBarrier(long double &);


public:
    Potential2D(Randoms * rand, double upToDownRate, double downToUpRate, double topValue, double bottomValue);

    ~Potential2D();

    /**
     * Return the value of V(x, y,t)
     */
    vec getValue(double &x, double &y, long double &t);

    // overloaded operator() for convinience
    vec operator()(double &x,double &y,  long double &t) {
        return this->getValue(x,y,t);
    }
    

//     double operator()(double x, double &y,  long double t) {
//         return this->getValue(x,t);
//     }
    
  //reset potential state and draw new initial value
	void reset() { this->init(); }

	
	     
    const char * toString() { return "V(x,y,t) = H * sqrt( x^2 + y^2) ";}


};


#endif
