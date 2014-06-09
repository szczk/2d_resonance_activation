#include "main.hh"
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_histogram.h>
#include <fstream>



// tmp: /mnt/lustre/scratch/people/ufszczep/";
// storage: /storage/ufszczep/
char filePrefix[200] = "";

int main ( int argc, char **argv )
{
     cout << endl << endl;

     System& sys = System::getInstance();
     sys.start();

     sys.printInfo();

     Settings& settings = Settings:: getInstance ( "settings.dat" );
     settings.readCommandLineParameters ( argc,argv );
     settings.printParameters();


     double alpha = settings.getJumpsParameter();
     //...

     Randoms * rand  = new Randoms();
     rand->reset();

     double maxT = settings.get ( "maxT" );
     double dt = settings.get("dt");

     double num = settings.getNtrajectories();
     int tenPerc = ( num>10 ) ? ( int ) ( num*0.1 ) : 1;


     int potentialType = settings.get ( "POTENTIAL_TYPE" );
     int noiseType = settings.get ( "NOISE_TYPE" );
     
     
     Simulation * sim = new Simulation ( &settings );

     char dataFile[200];
     if ( settings.multipleOutputs() ) {
          cout << " multiple outputs! generating file #" << settings.getMultipleOutputFilenum() <<endl;
          //wielokrotny output, wyjsciowy plik dat musi miec numerek przyslany z zewnatrz
          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_output_%d.dat",settings.getStoragePath(), settings.getFilesSuffix(), alpha, potentialType, noiseType, settings.getMultipleOutputFilenum() );
     } else {
          // wszystko do jednego pliku
          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_output.dat",settings.getStoragePath(), settings.getFilesSuffix(), alpha , potentialType, noiseType );
     }

     // korzystamy z Datafile
     // WARNING;  datafile jest tylko seria liczb, a tu zapisujemy wektor
     // wiec przy czytaniou trzeba pamietac o tym ze zapisujemy 2 skladowe po kolei
     // i potem trzeba to tak czytac po 2 naraz
     Datafile * data = Datafile::create ( dataFile );

//      ofstream output ( dataFile, ios_base::trunc | ios_base::out );

     
     // create dump times
     map<int, Datafile *> * dumps = new map<int, Datafile *>();
     double t = 0.0;
     for(int iter=0; t<maxT; iter++)  {
        
        
        if( !settings.isDumpTime(iter) ) {
          t+= dt;
          continue;
        }
        
//         cout << " dump file t=" << t << "\titeration="<<iter<<endl;
        
        if ( settings.multipleOutputs() ) {
            
            sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_%2.3f_output_%d.dat",settings.getStoragePath(), settings.getFilesSuffix(), alpha, potentialType, noiseType, t, settings.getMultipleOutputFilenum() );
        } else {
            sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_%2.3f_output.dat",settings.getStoragePath(), settings.getFilesSuffix(), alpha , potentialType, noiseType, t );
        }
        Datafile * d = Datafile::create ( dataFile );
        
//         cout <<"map key="<<iter<<endl;
        dumps->insert ( pair<int, Datafile*> ( iter , d ) );
        t+= dt;
     }
     
     
     
     
     for ( int i =0; i < num ; i++ ) {
          if ( i%tenPerc==0 ) {
               cout << i<<"/"<<num<<endl;
          }


          vec X = sim->run ( settings.getJumpsParameter() ,maxT , dumps );

//           output << X.x <<"\t"<<X.y<<"\n";
          data->write ( X.x );
//           data->write ( X.y );
     }

//      output.close();
     data->close();

     cout << "closing dumps"<<endl;

     for( auto it = dumps->begin(); it != dumps->end(); ++it) {
       (it->second)->close();
       delete (it->second);
     }
     
     delete dumps;




     delete rand;

     sys.finish();
     sys.printTimeSummary();

     return 0;
}


