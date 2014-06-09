#ifndef __TRAJECTORY_GENERATOR__
#define __TRAJECTORY_GENERATOR__


#include "Randoms.hh"
#include "Settings.hh"
#include <iostream>

// #include <gsl/gsl_rng.h>
// #include <gsl/gsl_randist.h>
 
using namespace std;

/**
 *  abstract class that represents 
 *  trajectory generator ( some algorithm that 
 *  generates Trajectory object -> set of x(t) values)
 * 
 * 
 */


class TrajectoryGenerator {

  
  
protected:
    
  typedef Settings::paramsType paramsPrecisionType;
  typedef Settings::resultType resultPrecisionType; //using floats = 1/2 less memory needed 
  
  
  paramsPrecisionType start_time;
//   paramsPrecisionType end_time;
  unsigned long int npoints;
  paramsPrecisionType dt;  // = (end_time - start_time)/npoints;
  
  paramsPrecisionType jumpsParameter; // alpha, (superdiffusion parameter)
  paramsPrecisionType waitingTimesParameter; // beta/mu, (subdfiffusion parameter)
  
  Randoms * rand;
  
  paramsPrecisionType x0; 
  
  void prepare();
  
public:
  
  TrajectoryGenerator();
  virtual ~TrajectoryGenerator();
  
  void setStartTime(paramsPrecisionType st) { this->start_time = st;  } 
  paramsPrecisionType getStartTime() { return this->start_time; }
/*  
  void setEndTime(paramsPrecisionType et) { this->end_time = et; }
  paramsPrecisionType getEndTime() { return this->end_time; }*/
  
  void setStartTimeNpoints(paramsPrecisionType st, unsigned long int np)
  {
    setStartTime(st); setNpoints(np);
  }
  
  void setNpoints(unsigned long int np) { if(np>0) this->npoints = np;  }
  unsigned long int getNpoints() { return this->npoints; }
  
  void setDt(paramsPrecisionType ddt) { if(ddt) this->dt=ddt;}
  paramsPrecisionType getDt() { return this->dt; }
  
  
  void setJumpsParameter(paramsPrecisionType alpha) { this->jumpsParameter = alpha; }
  void setWaitingTimesParameter(paramsPrecisionType mu) { this->waitingTimesParameter = mu; }
  
  void setAlphaBeta(paramsPrecisionType alpha, paramsPrecisionType beta) 
  {
    setJumpsParameter(alpha); setWaitingTimesParameter(beta);
  }
  
  void setRandomsGenerator(Randoms * rnd) { this->rand = rnd; }

  void setX0( paramsPrecisionType x) { this->x0 = x; }
  
  void setSettings( Settings &);
  
//   virtual resultPrecisionType* simulateTrajectory() = 0;
  
  
  
};


#endif
