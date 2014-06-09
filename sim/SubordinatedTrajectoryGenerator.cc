#include "SubordinatedTrajectoryGenerator.hh"





SubordinatedTrajectoryGenerator:: SubordinatedTrajectoryGenerator() : verbose(true)
{
    this->noise_D =  1.0;
//  cout << " SubordinatedTrajectoryGenerator created " << endl;

}



SubordinatedTrajectoryGenerator:: ~SubordinatedTrajectoryGenerator()
{

//   cout << " SubordinatedTrajectoryGenerator destroyed " << endl;
}

SubordinatedTrajectoryGenerator::resultPrecisionType SubordinatedTrajectoryGenerator:: filter(resultPrecisionType x)
{
    resultPrecisionType xf = 0.0;
    if (x>0.0) xf = 1.0;
    if (x<0.0) xf = -1.0;
    // if x==0, return 0.0;
    return xf;
}

// F(x,t) = - d/dx V(x,t)
SubordinatedTrajectoryGenerator::resultPrecisionType SubordinatedTrajectoryGenerator:: F(resultPrecisionType x, resultPrecisionType t)
{
    return -((*this->potential)(x,t));
    //return ( a*x - b*x*x*x  + A0 * cos( 2.0*M_PI *Q*t + phase ) );
}



void SubordinatedTrajectoryGenerator:: addValue(resultPrecisionType x, resultPrecisionType * resultArray, unsigned int index)
{
    resultArray[index] = x;
    //resultArray[index] = this->filter(x);
}

/**
 * generates subordinated trajectory
 *
 */

double SubordinatedTrajectoryGenerator:: simulateTrajectory()
{
  
    resultPrecisionType x = this->x0;
    resultPrecisionType t = this->start_time;
//  cout <<"reset rand " <<endl;
    if(this->doResetRandomNumberGenerator) this->rand->reset();
//      cout <<"reset rand " <<endl;
    this->potential->reset();

//      cout <<"new trajectory. x0 = " << x <<endl;

// 	end_time = end_time - dt;
//     resultPrecisionType * result = new resultPrecisionType[this->npoints];
//     unsigned int index = 0;

    if ( this->waitingTimesParameter > 0.0 && this->waitingTimesParameter < 1.0)
    {
        resultPrecisionType U_t = 0.0;
        resultPrecisionType tau = 0.0;
        resultPrecisionType dtau = 0.1*dt;

        resultPrecisionType taubar = 0.0;
        resultPrecisionType dtaubar = 0.1*dt;

        resultPrecisionType S_tn = 0.0;

        resultPrecisionType dUinc = pow(dtau,(1.0/this->waitingTimesParameter));


        resultPrecisionType T = start_time;

        resultPrecisionType noise_intensity = sqrt( 2.0* this->noise_D );

        resultPrecisionType dXinc = noise_intensity* sqrt( dtaubar );

// 	    while( T < end_time )
// 	    {
        unsigned long int n = 0;
        unsigned int tenPerc = (unsigned int)(npoints/10);
        while ( x < L  )
        {
            while (U_t < T)
            {
                resultPrecisionType du =  (resultPrecisionType) dUinc * this->rand->getSkewLevy(this->waitingTimesParameter);
                U_t += du;
                tau += dtau;
            }

            S_tn = tau;
            while (taubar < S_tn)
            {
                resultPrecisionType funcVal = F( x, U_t );
                resultPrecisionType stochastVal = (resultPrecisionType) this->rand->getLevyStable(this->jumpsParameter,0.0, 1.0 ,0.0);;
                resultPrecisionType sdx = funcVal * dtaubar + dXinc * stochastVal;

                x += sdx;
                taubar += dtaubar;

                if(x<0.0)
                {
                    x = -x;
                }

            }



            T += dt;
            t += dt;
            ++n;
        }
    }
    else    // diffusion
    {
        resultPrecisionType dTinc =  pow(dt,(1.0/this->jumpsParameter));


// 		double dTinc = sqrt(2.0* T) * sqrt(dt);

        resultPrecisionType noise_intensity = sqrt(2.0 * this->noise_D ) ; //pow(this->noise_D,(1.0/this->jumpsParameter));


        unsigned long int n = 0;

		double ONE_OVER_SQRT2 = 0.70710678118;
		
        while ( x < L  )
        {

            // save value
//             this->addValue(x, result, index++);

            resultPrecisionType dx = (resultPrecisionType) this->rand->getLevyStable(this->jumpsParameter ,0.0, ONE_OVER_SQRT2, 0.0);

            double forceValue =  F(x, t)*dt;
            double noiseValue =  noise_intensity * dTinc* dx;

// 			cout << "x(t="<<t<<")=" <<x<<"\t" << "F(x,t) = " << forceValue << " \t noise="<<noiseValue << endl;

            x += forceValue +  noiseValue;
            t += dt;

            if(x<0.0)
            {
                x = -x;
            }
            n++;
        }

//         cout << "n=" <<n <<endl;
    }


    return t;
}

void SubordinatedTrajectoryGenerator::  printParameters()
{
    cout << endl;
    cout <<" subordinated trajectory generator parameters:"<<endl;


    cout << "start time: " << start_time <<endl;
//   cout << "end time: " << end_time <<endl;
    cout << "dt: " << dt <<endl;
    cout << "npoints: " << npoints <<endl;
    cout << "x0: " << x0 <<endl;


    cout << "jumps parameter (superdiffusion) (alpha): " << this->jumpsParameter << endl;
    cout << "waiting times parameter ( subdiffusion ) (beta): " << this->waitingTimesParameter << endl;


    cout << "D = " << noise_D <<endl;




    cout <<endl;
    cout <<"-------------------------------"<<endl<<endl;
}

void SubordinatedTrajectoryGenerator:: setSettings(Settings& s)
{
    TrajectoryGenerator:: setSettings(s);

    this->setNoiseIntensity( s.getNoiseIntensity() );
}
