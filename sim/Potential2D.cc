#include "Potential2D.hh"





Potential2D:: Potential2D(Randoms * rand, double upToDownRate, double downToUpRate, double topValue, double bottomValue) :  timeOfNextSwitch(0.0) {
    this->top_value = topValue;
    this->bottom_value = bottomValue;
    this->gamma_up = downToUpRate;
    this->gamma_down = upToDownRate;

    this->randoms = rand;
//     cout << "creating potential"<<endl;
    this->init();
}



Potential2D:: ~Potential2D() {

    this->cleanUp();
}



void Potential2D::init()
{
//     cout << " draw initial barrier state"<<endl;

    double f = randoms->getUniform(0.0, 1.0);
    double gamma = 0.0;
// 	cout << "f= " << f <<endl;
    if (f <=0.5)
    {
        this->state = barrier_state::up_state;
        gamma = gamma_down;
    }
    else
    {
        this->state = barrier_state::down_state;
        gamma = gamma_up;
    }
    
//     cout << "gamma = " <<gamma <<endl;
    timeOfNextSwitch = randoms->getSpecialDist1(gamma); //getExponential( 1.0/gamma );
    
    
//     cout << " init at t= 0";
//     cout <<"\t\t new state: "<< (( state == barrier_state::up_state) ? " up" : "down ");
//     cout << "\t\t new time to switch = " << timeOfNextSwitch <<endl;
}


void Potential2D::cleanUp()
{
    //if (this->randoms!=NULL) delete this->randoms;
}

void Potential2D::switchBarrier(long double &currentTime)
{
    double gamma = 0.0;
    if ( state == barrier_state::up_state)
    {
        // barrier is in up state, switch do down state
        state = barrier_state:: down_state;
        // get rate of switching to up state
        gamma = gamma_up;
    }
    else
    {
        // is in down state, switch to up state
        state = barrier_state:: up_state;
        // get rate of switching to down state
        gamma = gamma_down;
    }

    
    timeOfNextSwitch = currentTime + randoms->getSpecialDist1(gamma); //getExponential( 1.0/gamma );


//     cout << " barier switch at t=" << currentTime ;
//     cout <<"\t\t new state: "<< (( state == barrier_state::up_state) ? " up" : "down ");
//     cout <<"\t\t new time to switch = " << timeOfNextSwitch <<endl;
}


double Potential2D::getValue(double &x, long double &t)
{
    double value = 0.0;

   

    /*
     * simple V(x,t) = A x
     * 
     * this actually returns V'(x,t)  
     * so  V'(x,t) = A;
     */
    if ( t >= timeOfNextSwitch )
    {
        this->switchBarrier(t);
    }

    if ( this->state == barrier_state:: up_state )
    {
        value = top_value;
    }
    else // if (this->state = barrier_state :: down_state )
    {
        value = bottom_value;
    }

//      cout << "get V( x = " << x << ", t = " << t << ") =  "<< value  <<endl;
    return value;
}



