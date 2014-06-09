#include "TrajectoryGenerator.hh"





  
TrajectoryGenerator:: TrajectoryGenerator()
{
  this->start_time = 0.0;
//   this->end_time = 0.0;
  this->npoints = 1;
  this->x0 = 0.0;
  
  
  this->jumpsParameter = 2.0; // alpha, (superdiffusion parameter)
  this->waitingTimesParameter = 1.0; // beta/mu, (subdfiffusion parameter)
  
  this->rand = NULL;
}


TrajectoryGenerator:: ~TrajectoryGenerator() 
{

}




void TrajectoryGenerator:: setSettings(Settings & s)
{
  this->setStartTime( s.getStartTime() );
  this->setNpoints( s.getNpoints() );
  this->setDt( s.getDt() );
  this->setAlphaBeta( s.getJumpsParameter() , s.getWaitingTimesParameter() );
  this->setX0( s.getX0() );

}

