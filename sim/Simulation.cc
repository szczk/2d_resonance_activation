#include "Simulation.hh"

inline ostream& operator<< ( ostream &out, point &cPoint )
{
     // Since operator<< is a friend of the Point class, we can access
     // Point's members directly.
     out << "(" << cPoint.x << ", " <<  cPoint.y << ")";
     return out;
}



Simulation::Simulation ( Settings * set )  : rand ( nullptr ) , x0 ( 0.0 ),y0 ( 0.0 ), r ( 4.0 ), dump_gnuplot ( false ), verbose ( false )
{

     this->settings = set;

     this->init();
}


Simulation::~Simulation()
{
     this->destroy();
}



void Simulation::destroy()
{
     cout << "destroying simulation"<<endl;

     if ( rand!=nullptr ) delete rand;

}


void Simulation::init()
{
     cout << "initializing simulation"<<endl;
     this->rand = new Randoms();
     this->reset();
     this->r_squared = r*r;

     this->angle_start = 0.0;
}


void Simulation::reset()
{
     this->rand->reset();

}




double Simulation::run ( double angle_end )
{

//   this->reset();

//      bool dump_gnuplot = false;

     double dt = this->settings->getDt();
     double t = this->settings->getStartTime(); //t0


     double alpha = this->settings->getJumpsParameter();
     double dL = exp ( log ( dt ) /alpha );

     double sigma = this->settings->getNoiseIntensity();
     

//   double endTime = 10.0;

     point current_point;

     current_point.x = this->settings->getX0();
     current_point.y = this->settings->getX0();

     ofstream gnuplot;

     if ( dump_gnuplot ) {
          gnuplot.open ( "test_traj.gnu",ios_base::out );

          gnuplot<<"reset\n";
          gnuplot<<"set term gif enhanced animate delay 1\n";
          gnuplot<<"set output \"trajectory.gif\"\n";
          gnuplot<<"set title '{/Symbol a} = "<< alpha <<"'\n";

          gnuplot << "set size square 1,1\n";
          gnuplot<<"set xrange [-4.2:4.2]\n";
          gnuplot<<"set yrange [-4.2:4.2]\n";
          gnuplot<<"set xlabel 'x'\n";
          gnuplot<<"set ylabel 'y'\n";

          gnuplot << " # Parametric functions for a circle\n";
          gnuplot << "set parametric\n";
          gnuplot << "set trange [0:2*pi]\n";
          gnuplot << "fx(t) = "<< x0 <<" +"<< r<<"*cos(t)\n";
          gnuplot << "fy(t) = "<< y0 <<" +"<< r<<"*sin(t)\n";
          gnuplot << "plot fx(t),fy(t)\n";
          gnuplot << "unset parametric\n";

          gnuplot << "plot \"< echo '"<< current_point.x << " "<< current_point.y << "'\" w linespoints pt 7 lc rgb 'blue' notitle;\n";
     }

     bool absorbed = false;

     while ( true ) {



//        cout << "t="<<t<<endl
          double * v = rand->getAlphaStableVector (  alpha, sigma );

          point target_point;
          target_point.x = current_point.x + v[0]*dL;
          target_point.y = current_point.y + v[1]*dL;

          //cout << "t="<< t << "\t" << current_point <<" \t => "<< target_point << endl;;

          if ( dump_gnuplot ) {
               gnuplot<<"set title \"{/Symbol a} = "<< alpha <<" t = "<< t << "\\n d = "<< getDistance ( current_point,target_point ) <<"\"\n";
               gnuplot << "set multiplot\n";
               gnuplot << " # Parametric functions for a circle\n";
               gnuplot << "set parametric\n";
               gnuplot << "set trange [0:2*pi]\n";
               gnuplot << "fx(t) = "<< x0 <<" +"<< r<<"*cos(t)\n";
               gnuplot << "fy(t) = "<< y0 <<" +"<< r<<"*sin(t)\n";
               gnuplot << "plot fx(t),fy(t) notitle lc rgb 'black'\n";
               gnuplot << "unset parametric\n";

               gnuplot << "set parametric\n";
               gnuplot << "set trange ["<<angle_start <<":"<< angle_end<<"]\n";
               gnuplot << "fx(t) = "<< x0 <<" +"<< r<<"*cos(t)\n";
               gnuplot << "fy(t) = "<< y0 <<" +"<< r<<"*sin(t)\n";
               gnuplot << "plot fx(t),fy(t) lw 3 lc rgb 'red' notitle\n";
               gnuplot << "unset parametric\n";
          }


          // funkcja liczaca odbicia moze rzucic wyjatkiem (a w zasadzie intem)
          // w przypadku problemow, typu 0 dlugosc odbicia, NaN-y itd.
          // wtedy wystarczy go wylapac i po prostu calkowicie zignorowac caly krok
          // i wylosowac kolejny wektor losowy nie zwiekszajac czasu t
          try {

               if ( ! inCircle ( target_point ) ) {
                    //odbicie
// 	    cout << "out"<<endl;
                    point new_target = target_point;
                    point last_punkt_odbicia = current_point;
                    point punkt_odbicia ;
                    while ( !inCircle ( new_target ) ) {


                         point target = this->getNewTarget ( last_punkt_odbicia, new_target, punkt_odbicia, gnuplot );


                         // przejscie przez luk absorbujacy
                         if ( this->pointOnArc ( angle_end, punkt_odbicia ) ) {
                              absorbed = true;
//                          cout << "absorbed!"<<endl;

                              if ( dump_gnuplot ) {
                                   gnuplot << "plot \"< echo '"<< punkt_odbicia.x <<" "<< punkt_odbicia.y <<"'\" w linespoints pt 7 lc rgb 'blue' notitle;\n";
                                   gnuplot << "set arrow from "<<last_punkt_odbicia.x<<","<<last_punkt_odbicia.y<<" to "<<punkt_odbicia.x<<","<<punkt_odbicia.y<<"  front lw 2 lc rgb 'red' # dr\n";
                              }
                              last_punkt_odbicia = punkt_odbicia;
                              break;
                         }

                         if ( dump_gnuplot ) {
                              gnuplot << "set arrow from "<<last_punkt_odbicia.x<<","<<last_punkt_odbicia.y<<" to "<<punkt_odbicia.x<<","<<punkt_odbicia.y<<"  front lw 2 lc rgb 'blue' # dr\n";
                         }

                         last_punkt_odbicia = punkt_odbicia;
                         new_target = target;

                    };
                    target_point = new_target;


                    if ( !absorbed && dump_gnuplot ) {
                         gnuplot << "set arrow from "<<last_punkt_odbicia.x<<","<<last_punkt_odbicia.y<<" to "<<target_point.x<<","<<target_point.y<<"  front lw 2 lc rgb 'blue' # dr\n";
                    }


               } else {

                    if ( dump_gnuplot ) {
                         gnuplot << "set arrow from "<<current_point.x<<","<<current_point.y<<" to "<<target_point.x<<","<<target_point.y<<"  front lw 2 lc rgb 'blue' # dr\n";
                    }
               }
               if ( dump_gnuplot ) {
                    gnuplot << "unset multiplot\n";
                    gnuplot << flush;
               }
               current_point = target_point;

               delete[] v;
               t+= dt;



               if ( absorbed ) break;

          } catch ( int j ) {
               clog <<"exception caught! " << j << endl;

          }

     }//while(t)

     if ( gnuplot.is_open() ) {
          gnuplot.close();
     }
     return t;
}


bool Simulation::inCircle ( point& landing_point )
{
     return ( landing_point.x*landing_point.x + landing_point.y*landing_point.y < r_squared );
}


double Simulation::dotProduct ( const vec & v1, const vec & v2 )
{
     return v1.x*v2.x + v1.y*v2.y;

}


double Simulation::getDistance ( const point & p1, const point & p2 )
{
     return sqrt ( pow ( p2.x-p1.x,2.0 ) + pow ( p2.y-p1.y,2.0 ) );
}



void Simulation::norm ( vec & v )
{
     double d = sqrt ( pow ( v.x,2.0 ) + pow ( v.y,2.0 ) );
     v.x = v.x/d;
     v.y = v.y/d;
}


// MAGIC starts here
// funkcja rzuca wyjatki w wyjatkowych przypadkach
// kiedy nie wie co zrobic
point Simulation::getNewTarget ( const point & start_point ,const point & end_point, point & odbicie_ref , ofstream & gnuplot )
{
//      bool verbose = false;



     if ( verbose ) cout << "odbicie!"<<endl;




     if ( verbose ) cout << " z : " << start_point.x << "," << start_point.y << " \t do: "<< end_point.x << "," << end_point.y << endl;

     if ( start_point.x==end_point.x && start_point.y == end_point.y ) {
          cerr<< "start_point == end_point !!"<<endl;
          throw -1;
     }
     point new_end_point;

     // znajdujemy prosta przechodzaca przez dwa podane punkty

     // normalna prosta
     // y = a x + b
     double a = 0.0;
     double b = 0.0;
     bool pionowa_prosta = false;
     if ( ! ( end_point.x==start_point.x ) ) {
          a = ( end_point.y - start_point.y ) / ( end_point.x - start_point.x );
          b = start_point.y - a * start_point.x;
     } else {
          // jezeli oba punkty maja ten sam x
          // to jest prosta o rownaniu  x = b (pionowa)
          // jako 'b' ustawiamy wartosc x
          b = end_point.x;
          pionowa_prosta = true;
          clog << "pionowa prosta!"<<endl;
//   throw ""
     }

//   cout << "p.a = "<< a<<endl;
//   cout << "p.b = "<< b<<endl;


     // jezeli jeden lezy w srodku a drugi poza okregiem to prosta
     // musi przecinac okrag w dwoch miejscach

     point przeciecie1,przeciecie2;

     if ( ! pionowa_prosta ) {
          double delta = 4.0*a*a*b*b - 4.0 * ( 1.0 + a*a ) * ( b*b-r_squared );

          double x_r_1 = ( -2.0*a*b - sqrt ( delta ) ) / ( 2* ( 1.0 + a*a ) );
          double x_r_2 = ( -2.0*a*b + sqrt ( delta ) ) / ( 2* ( 1.0 + a*a ) );

          przeciecie1.x = x_r_1;
          przeciecie1.y = a*x_r_1 + b;

          przeciecie2.x = x_r_2;
          przeciecie2.y = a*x_r_2 + b;

          // wykrywanie NaN
          if ( przeciecie1.x!=przeciecie1.x ) {
// 	  cout <<"!!!!"<<endl;
// 	  cout << przeciecie1 <<"\t"<<przeciecie2<<endl;
// 	  cout << "delta="<<delta<<endl;
// 	  cout <<"a="<<a<<"\tb="<<b<<"\t"<<endl;
               clog << start_point.x <<","<<start_point.y << "\t"<< end_point.x << ","<<end_point.y<<endl;
               throw -1;
          }

     } else {
          // pionowa prosta
          clog << "pionowa prosta!"<<endl;

          przeciecie1.x = b;
          przeciecie1.y = sqrt ( r_squared - b*b );

          przeciecie2.x = b;
          przeciecie2.y = -sqrt ( r_squared - b*b );
          clog << przeciecie1 <<"\t"<<przeciecie2<<endl;
     }


     if ( verbose ) cout << "przeciecie 1 = "<< przeciecie1 <<endl;
     if ( verbose ) cout  << "przeciecie 2 = "<< przeciecie2 <<endl;

     // trzeba wybrac ktory z punktow jest blizej (czyli lezy pomiedzy wejsciowymi dwoma punktami
     // a nie po przeciwnej stronie okregu

     double d1 = getDistance ( end_point,przeciecie1 );
     double d2 = getDistance ( end_point,przeciecie2 );


     if ( verbose ) cout << "d1 = " << d1 << ", d2="<<d2<<endl;

     point punkt_odbicia;

//    gnuplot << "plot \"< echo '"<< przeciecie1.x <<" "<< przeciecie1.y <<"\\n"<< przeciecie2.x <<" "<< przeciecie2.y <<"'\" w linespoints pt 7 lc rgb 'gray' notitle;\n";

     if ( d1 < d2 ) {
          if ( verbose ) cout << "punkt 1 jest blizej! "<<endl;
          punkt_odbicia = przeciecie1;
     } else if ( d1 > d2 ) {
          if ( verbose ) cout << "punkt 2 jest blizej"<<endl;
          punkt_odbicia=przeciecie2;
     } else {
          cerr << "taka sama odleglosc!"<<endl;
          cerr << przeciecie1 << " vs " << przeciecie2<<endl;
          throw -1;
     }

     // mamy punkt odbicia, teraz wyznaczamy 3 wektory


     // wektor normalny do okregu z punktu przeciecia
     vec d_n;

     d_n.x = - ( punkt_odbicia.x - x0 );
     d_n.y = - ( punkt_odbicia.y - y0 );

     //normalizacja
     norm ( d_n );

     if ( verbose ) cout << "wektor normalny d_n: "<< d_n.x <<","<<d_n.y<<endl;

     // wektor od punktu przeciecia do punktu poczatkowego (d_i)
     vec d_i;
     d_i.x =  - ( punkt_odbicia.x - start_point.x );
     d_i.y =  - ( punkt_odbicia.y - start_point.y );

     if ( verbose ) {
          cout << "d_i.x= - ( "<< punkt_odbicia.x <<" - "<<start_point.x << ")"<<endl;
          cout << "d_i.y= - ( "<< punkt_odbicia.y <<" - "<<start_point.y << ")"<<endl;
     }

     //normalizacja
     norm ( d_i );
     if ( verbose )  cout << "wektor normalny d_i: "<< d_i.x <<","<<d_i.y<<endl;


     // teraz mozna wyliczyc wektor pokazujacy kierunek odbicia

     // wektor odbicia
     // d_s  = 2 ( d_n . d_i ) * d_n - d_i
     double dp = this->dotProduct ( d_n , d_i );

     vec d_s;
     d_s.x = ( 2.0*dp ) *d_n.x - d_i.x;
     d_s.y = ( 2.0*dp ) *d_n.y - d_i.y;

     if ( verbose ) cout << "wektor d_s: "<< d_s.x <<","<<d_s.y<<endl;

     norm ( d_s );

//      gnuplot << "set arrow from "<<punkt_odbicia.x<<","<<punkt_odbicia.y<<" to "<<punkt_odbicia.x + d_i.x<<","<<punkt_odbicia.y + d_i.y<<"  front lw 1 lc rgb 'red' # d_i\n";
//      gnuplot << "set arrow from "<<punkt_odbicia.x<<","<<punkt_odbicia.y<<" to "<<punkt_odbicia.x + d_n.x<<","<<punkt_odbicia.y + d_n.y<<"  front lw 1 lc rgb 'red' # d_n\n";
//      gnuplot << "set arrow from "<<punkt_odbicia.x<<","<<punkt_odbicia.y<<" to "<<punkt_odbicia.x + d_s.x<<","<<punkt_odbicia.y + d_s.y<<"  front lw 1 lc rgb 'red' # d_s\n";

     if ( verbose )  cout << "wektor normalny d_s: "<< d_s.x <<","<<d_s.y<<endl;

     // mamy wektor d_s pokazujacy kierunek odbicia

     // wyznaczamy prosta na ktorej lezy ten wektor
     // (przechodzaca przez punkt_odbicia)

     // drugi punkt, wyznaczony przez d_s
     point point2;
     point2.x = punkt_odbicia.x + d_s.x;
     point2.y = punkt_odbicia.y + d_s.y;

     // TODO: zoptymalizowac
     double a2 = 0.0;
     double b2 = 0.0;
     bool pionowa_prosta2 = false;
     if ( ! ( punkt_odbicia.x==point2.x ) ) {
          a2 = ( punkt_odbicia.y - point2.y ) / ( punkt_odbicia.x - point2.x );
          b2 = point2.y - a2 * point2.x;
     } else {
          b2 = punkt_odbicia.x;
          pionowa_prosta2 = true;
          clog << "pionowa prosta2!"<<endl;
     }

     if ( verbose ) cout << "prosta punkt_odbicia-new end = "<<a2 << ","<<b2 <<endl;


     //oryginalna odleglosc skoku
     double org_d = getDistance ( start_point,end_point );

     // ile z tego jest od startu do punktu odbicia
     double d_start_odbicie = getDistance ( start_point, punkt_odbicia );

     // wiec pozostaje tyle po odbiciu
     double d_odbicie_end = org_d - d_start_odbicie;

     if ( verbose ) cout << "org_d = " << org_d << ",\td_start_odbicie = " <<d_start_odbicie<< "\t d_odbicie_end="<<d_odbicie_end<<endl;

     //odleglosc d_odbicie_end musi lezec na prostej a2-b2 w kierunku srodka okregu

     //(x2-x1)^2 + (y2-y1)^2 = d_odbicie_end^2

     // punkt odbicia =  (x1,y1)

     // (x2-x1)^2 + (y2-y1)^2 = d^2
     //  y2 = a x2 + b

     // (x2-x1)^2 + (a x2 + b- y1)^2 = d^2
     // x2^2 - 2 x1 x2 + x1^2 +   (a x2)^2 +  2 ( a x2) (b-y1) + (b-y1)^2  = d^2
     // x2^2 + (a x2)^2 - 2 x1 x2 + 2 ( a x2) (b-y1)  + x1^2 + (b-y1)^2 = d^2
     // (1 +a^2) x2^2  -  (2x1 - 2a(b-y1) ) x2   +   x1^2 + (b-y1)^2 - d^2 = 0

     point endpoint1;
     point endpoint2;

     if ( !pionowa_prosta2 ) {
          double A2 = ( 1.0  + pow ( a2,2.0 ) );
          double B2 = - ( ( 2.0 * punkt_odbicia.x - 2.0* a2* ( b2-punkt_odbicia.y ) ) );
          double C2 = pow ( punkt_odbicia.x,2.0 ) + pow ( ( b2-punkt_odbicia.y ),2.0 ) - pow ( d_odbicie_end,2.0 );

          double delta2 = B2 * B2 - 4.0*A2*C2;

// 	cout << "delta2 : "<<delta2 << endl;



          endpoint1.x = ( -B2 - sqrt ( delta2 ) ) / ( 2.0*A2 );
          endpoint2.x = ( -B2 + sqrt ( delta2 ) ) / ( 2.0*A2 );

          endpoint1.y = a2 * endpoint1.x + b2;
          endpoint2.y = a2 * endpoint2.x + b2;

          if ( verbose ) {
               cout << "p1 : " << endpoint1.x << ","<<endpoint1.y <<"\td=" << getDistance ( punkt_odbicia,endpoint1 ) << endl;
               cout << "p2 : " << endpoint2.x << ","<<endpoint2.y <<"\td=" << getDistance ( punkt_odbicia,endpoint2 ) << endl;
          }

//           gnuplot << flush;

     } else {
          //implement me!
          clog <<  "not implemented! o_O" << endl;
          throw -1;
     }


     // wlasciwiy punkt bedzie blizej point2 (punktu wskazanego przez wektor d_s od punktu odbicia

     double d11 = getDistance ( point2,endpoint1 );
     double d22 = getDistance ( point2,endpoint2 );

//
     if ( d11 < d22 ) {
          if ( verbose ) cout << " p1 jest blizej " <<endl;
          new_end_point = endpoint1;

          if ( !inCircle ( endpoint1 ) && inCircle ( endpoint2 ) ) {
               new_end_point = endpoint2; //korekta
               cout << "korekta p1 -> p2"<<endl;
          }

     } else {
          if ( verbose ) cout << " p2 jest blizej " <<endl;
          new_end_point = endpoint2;

          if ( inCircle ( endpoint1 ) && !inCircle ( endpoint2 ) ) {
               new_end_point = endpoint1; //korekta
               cout << "korekta p2 -> p1"<<endl;
          }

     }

     if ( verbose ) cout << "new end point: " << new_end_point.x <<"," << new_end_point<<endl;

     odbicie_ref = punkt_odbicia;

     return new_end_point;
}


bool Simulation::pointOnArc ( double angle, point& p )
{
//      bool debug = false;

//      if ( inCircle ( p ) )
//      {
//        if(debug) cout << "pointOnArc: in circle = true, returning false"<<endl;
//        return false;
//      }

     if ( angle > 0.0 ) {

          double at = 0.0;
          if ( p.y > 0.0 ) {
               at = atan2 ( p.y,p.x );
          } else if ( p.y<-0.0 ) {
               at = atan2 ( -p.y,-p.x ) + M_PI;
          } else { // y=0
               if ( p.x>0.0 ) at = 0.0;
               else at = M_PI;
          }

//            if(debug) cout << "angle="<<at<<endl;

          //korekta na zero
          if ( at <1.0e-6 ) at = 0.0;

// 	   if(debug) cout << "angle="<<at<< " < " << angle << endl;
// 	   if(debug) cout << at << angle << endl;
//
          return at < angle;

     } else {
//            if(debug) cout << "angle <= 0.0"<<endl;
          return false;
     }
}
