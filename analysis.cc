#include "main.hh"
#include <gsl/gsl_histogram2d.h>
#include <gsl/gsl_histogram.h>
#include <fstream>

#include "analysis/HistogramsProducer.hh"
#include "analysis/EDFProducer.hh"



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


     double maxT = settings.get ( "maxT" );
     double alpha = settings.getJumpsParameter();



//      HistogramsProducer * producer = new HistogramsProducer ( &settings );
//      producer->setTime ( maxT );

     EDFProducer * edfProducer = new EDFProducer ( &settings );

     edfProducer->setTime ( maxT );


     char dataFile[200];

     int potentialType = settings.get ( "POTENTIAL_TYPE" );
     int noiseType = settings.get ( "NOISE_TYPE" );

     DataSource * data = nullptr ;

     if ( settings.multipleOutputs() ) {
          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_output_{NUM}.dat",settings.getDataPath(),settings.getFilesSuffix(), alpha, potentialType, noiseType );

          // multiple inputs, we will use chained input files
          int filesNum = settings.getMultipleOutputFilenum();  // use this number as max-number indicator

          cout << "opening "<< filesNum << "chain files "<< dataFile << endl;
          data = new DatafilesChain ( dataFile ,1, filesNum );
     } else {

          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_output.dat",settings.getDataPath(), settings.getFilesSuffix(),alpha , potentialType, noiseType );

          cout << "opening single file '"<< dataFile<< "'" <<endl;
          data = Datafile::open ( dataFile );
     }


     if ( data->ok() ) {
          cout << "datafile ok"<<endl;
     } else {
          cout << "datafile not ok" <<endl;
          return 0;
     }
     int count = data->getCount() /2; //wektor 2 wartosci
     cout << "datafile contains " << count << " values"<<endl;
     int tenPerc = ( int ) ( count/10.0 );

     int c = 0;
     while ( data->hasNext() ) {
          if ( c%tenPerc==0 ) cout<< c << "/" << count <<endl;

          double x = data->read();
          double y = data->read();


//           producer->fill ( x,y );
          edfProducer->fill ( x, y );

          ++c;
     }

     data->close();


//      producer->close();
     edfProducer->close();

//      delete producer;

     delete edfProducer;




// ====================== t= 2


     HistogramsProducer * producer_t2 = new HistogramsProducer ( &settings );
     EDFProducer * edfProducer_t2 = new EDFProducer ( &settings );

     producer_t2->setTime ( 2.0 );
     
     edfProducer_t2->setTime ( 2.0 );



     if ( settings.multipleOutputs() ) {
          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_2.000_output_{NUM}.dat",settings.getDataPath(),settings.getFilesSuffix(), alpha, potentialType, noiseType );

          // multiple inputs, we will use chained input files
          int filesNum = settings.getMultipleOutputFilenum();  // use this number as max-number indicator

          cout << "opening "<< filesNum << "chain files "<< dataFile << endl;
          data = new DatafilesChain ( dataFile ,1, filesNum );
     } else {

          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_2.000_output.dat",settings.getDataPath(), settings.getFilesSuffix(),alpha , potentialType, noiseType );

          cout << "opening single file '"<< dataFile<< "'" <<endl;
          data = Datafile::open ( dataFile );
     }


     if ( data->ok() ) {
          cout << "datafile ok"<<endl;
     } else {
          cout << "datafile not ok" <<endl;
          return 0;
     }
     count = data->getCount() /2; //wektor 2 wartosci
     cout << "datafile contains " << count << " values"<<endl;
     tenPerc = ( int ) ( count/10.0 );

     c = 0;
     while ( data->hasNext() ) {
          if ( c%tenPerc==0 ) cout<< c << "/" << count <<endl;

          double x = data->read();
          double y = data->read();


//           producer_t2->fill ( x,y );
          edfProducer_t2->fill ( x,y );

          ++c;
     }

     data->close();



//      producer_t2->close();
     edfProducer_t2->close();
     
//      delete producer_t2;
     delete edfProducer_t2;

     //================================ t = 4

//      HistogramsProducer * producer_t4 = new HistogramsProducer ( &settings );
     EDFProducer * edfProducer_t4 = new EDFProducer ( &settings );
//      producer_t4->setTime ( 4.0 );
     edfProducer_t4->setTime ( 4.0 );


     if ( settings.multipleOutputs() ) {
          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_4.000_output_{NUM}.dat",settings.getDataPath(),settings.getFilesSuffix(), alpha, potentialType, noiseType );

          // multiple inputs, we will use chained input files
          int filesNum = settings.getMultipleOutputFilenum();  // use this number as max-number indicator

          cout << "opening "<< filesNum << "chain files "<< dataFile << endl;
          data = new DatafilesChain ( dataFile ,1, filesNum );
     } else {

          sprintf ( dataFile,"%s/%s_alpha_%1.2f_p%d_n%d_xy_t_4.000_output.dat",settings.getDataPath(), settings.getFilesSuffix(),alpha , potentialType, noiseType );

          cout << "opening single file '"<< dataFile<< "'" <<endl;
          data = Datafile::open ( dataFile );
     }


     if ( data->ok() ) {
          cout << "datafile ok"<<endl;
     } else {
          cout << "datafile not ok" <<endl;
          return 0;
     }
     count = data->getCount() /2; //wektor 2 wartosci
     cout << "datafile contains " << count << " values"<<endl;
     tenPerc = ( int ) ( count/10.0 );

     c = 0;
     while ( data->hasNext() ) {
          if ( c%tenPerc==0 ) cout<< c << "/" << count <<endl;

          double x = data->read();
          double y = data->read();


//           producer_t4->fill ( x,y );
          edfProducer_t4->fill ( x,y );
          ++c;
     }

     data->close();




//      producer_t4->close();
     edfProducer_t4->close();
//      delete producer_t4;
     delete edfProducer_t4;


     sys.finish();
     sys.printTimeSummary();

     return 0;
}


