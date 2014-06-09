#ifndef __SUBORDINATED_TRAJECTORY_GENERATOR__
#define __SUBORDINATED_TRAJECTORY_GENERATOR__

#include "TrajectoryGenerator.hh"
#include <vector>
#include <limits>
#include <fstream>
#include "Potential.hh"

using namespace std;

/**
 *  Subordinated trajectories generator
 * 
 *  from Magdziarz/Weron publication
 * 
 * 
 */

class SubordinatedTrajectoryGenerator : public TrajectoryGenerator {
  
private:
 
 
  resultPrecisionType F(resultPrecisionType x, resultPrecisionType t);
  
  resultPrecisionType filter(resultPrecisionType);
  paramsPrecisionType noise_D; // noise intensity

  void addValue(resultPrecisionType , resultPrecisionType * , unsigned int);

  bool verbose;
  bool doResetRandomNumberGenerator;
  double L;
  
  Potential * potential;
  
public: 
  SubordinatedTrajectoryGenerator();
  ~SubordinatedTrajectoryGenerator();
  
  void setNoiseIntensity(paramsPrecisionType d) { this->noise_D = d; }
  
  double simulateTrajectory();
    
  
  
  void setVerbose(bool v) { this->verbose = v;}
  void setSettings(Settings&);   
    
  void printParameters();
  
  void setDoResetRandomNumberGenerator(bool doReset) { this-> doResetRandomNumberGenerator = doReset; }
  
  void setPotential(Potential *v) { this->potential = v; }
  void setL(double l) {this->L = l;}
};



#endif
