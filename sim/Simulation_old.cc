#include "Simulation.hh"





Simulation::Simulation(Randoms * rand , Settings * set) //: gamma(0.0) , e_minus(0.0), e_plus(0.0), T(0.0), dt(0.0) , L(0.0), gauss_sigma(0.0)
{
    this->rand = rand;
    this->settings = set;
    this->init();
}


Simulation::~Simulation()
{
    this->destroy();
}



void Simulation::destroy()
{

}


void Simulation::init()
{
    this->reset();
    setDt( this->settings->getDt() );
}

void Simulation::reset()
{
    gamma = 0.0;
    e_minus = 0.0;
    e_plus = 0.0;
    //T = 0.0;
    //x0 = 0.0;
    //dt = 0.0;
    //L = 0.0;
    //gauss_sigma = 0.0;
//   cout << "reset"<<endl;
    v = NULL;

// 	cout << "reset2"<<endl;
    sbrd = new SubordinatedTrajectoryGenerator();

    sbrd->setRandomsGenerator ( rand );
    sbrd->setDoResetRandomNumberGenerator(false);
// 		cout << "reset2"<<endl;
    sbrd->setSettings ( *settings );
    sbrd->setVerbose(true);
    sbrd->setL(this->L);
    sbrd->setX0( this->settings->getX0() );
// 	cout << "reset3"<<endl;
    ;
    sbrd->setNoiseIntensity(T);

//   cout << "reset4"<<endl;
}


bool Simulation:: parametersOk()
{
    return ( (gamma>0.0) && (dt > 0.0) && (T > 0.0) );
}

double Simulation::run() {

    if(! parametersOk() )
    {
        cerr << "Simulation:: parameters not set!" << endl;
    }

//   long double t = 0.0;
//   double xInc = sqrt(2.0* T) * sqrt(dt);
//   double x = x0;
    if(v!=NULL) delete v;

    v = new Potential( this->rand, this->gamma, this->gamma, this->e_plus, this->e_minus );
    sbrd->setPotential( v );

// 	cout << "sim start" <<endl;
// 	sbrd->printParameters();

    return sbrd->simulateTrajectory();
}



// double Simulation::run()
// {
//   if(! parametersOk() )
//   {
// 	cerr << "Simulation:: parameters not set!" << endl;
//   }
//   long double t = 0.0;
//   double xInc = sqrt(2.0* T) * sqrt(dt);
//   double x = x0;
//
//     Potential V( this->rand, this->gamma, this->gamma, this->e_plus, this->e_minus );
//
//   do {
//     double functionValue = - V(x,t);
//     double xi = rand->getGauss(this->gauss_sigma);
//     x += functionValue * dt + xInc * xi;
//
//     if(x<0.0)
//     {
//       x = -x;
//     }
//
//     t += dt;
//
//   } while( x < L ) ;
//
//
//   return t;
// }



void Simulation:: print() {
    cout << "T = " << T <<"\tgamma = " << gamma << "\tE_+ = " << e_plus << "\tE_minus = " << e_minus << endl;
}
