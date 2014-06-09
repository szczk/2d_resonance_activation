#ifndef __SIMULATION_HH__
#define __SIMULATION_HH__



/**
 * Top level class for dichotomic process simulation 
 * 
 */
#include "../core/Randoms.hh"
#include "../core/Settings.hh"
#include "Potential.hh"

#include "SubordinatedTrajectoryGenerator.hh"

class Simulation {
  
private:
  
  Randoms * rand;
  Settings * settings;
  double gamma;
  double e_minus;
  double e_plus; 
  double T;
  double dt;
  double L;
  double x0;
  double gauss_sigma;
  
  
  /**
   * Check if all parameters are set correctly
   */
  bool parametersOk();
  
  /**
   * Initialize anything that needs to be initialized
   */
  void init();
  
  /**
   *  Close anything that needs to be closed.
   */
  void destroy();
  
  SubordinatedTrajectoryGenerator * sbrd;
  Potential * v;
  
  
public:
  
  /**
   * Default constructor
   */
  Simulation(Randoms *, Settings * );
  
  ~Simulation();
  
  /**
   * Reset all and prepare for new simulations
   */
  void reset();
  
  void setGamma(double g) { if(g>0.0) this->gamma = g; }
  void setEminus(double em) { this->e_minus = em; }
  void setEplus(double ep) { this->e_plus = ep; }
  void setEplusEminus(double eplus,double eminus) { this->setEminus(eminus); this->setEplus(eplus); }
  void setDt(double d_t) { this->dt = d_t; }
  void setT(double t) { this->T = t; }
  void setL(double l) { this->L = l; }
  void setGaussSigma(double gs) { this->gauss_sigma = gs; }
  void setX0(double xz) { this->x0 = xz; }
  
  double getGamma() { return this->gamma; }
  double getEminus() { return this->e_minus; }
  double getEplus() { return this->e_plus; }
  double getT() { return this->T; }
  double getL() { return this->L; }
  double getGaussSigma() { return this->gauss_sigma; }
  double getX0() { return this->x0; }
  double getDt() { return this->dt; } 
  
  
  double run();
  
  
  void print();
  
  
  
};

#endif