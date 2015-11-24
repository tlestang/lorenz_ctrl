#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

/*This code is designed to perform a control run of the Lorenz-63 model. The correlation time tau_c is fixed from previosu data analysis (lorenz.cpp). 
  First of all there is a transient regime over a certain number of tau_c. Then the actual control run starts over typically 1e6*tau_c. 
  The Lorenz equations are just simulated through a first order euler scheme and the points of the trajectory are just written in an output file at EACH timesteps. 
  4Output is BINARY.
*/

int main()
{
  /*Parameters of the Lorenz-63 model*/
  double sigma = 10.0;
  double rho = 28.0;
  double beta = 8./3.;

  /*Parameters of the run*/
  /*Correlation time*/
  double tau_c = 10.0;
  /*Timestep for euler method*/
  double dt = 1.0e-3;
  /*Time on which you average x in units of correlation time tau_c*/
  int sampleTime = 100;
  /*Convert to nb of timesteps*/
  sampleTime = floor((sampleTime*tau_c)/dt);
  /*Total length of the transient regime in units of correlation time tau_c*/
  double transientDuration = 200.0;
  /*Total length of the chunk in units of correlation time tau_c*/
  long int runChunk = 1e3;
  /*Number of times you repeat the chunk*/
  long int nbOfChunks = 2e3;
  long int totNbOfTimesteps = floor((runChunk*tau_c)/dt);
  long int transientNbOfTimeSteps = floor((transientDuration*tau_c)/dt);
  /*Misc*/
  double xIn, yIn ,zIn, xOut, yOut, zOut;
  int k=0;
  /*Pointers of FILE for output*/
  ofstream x_time_serie, y_time_serie, z_time_serie;
  ofstream x_accumulated, y_accumulated, z_accumulated;

  /* -------------------------------------------------*/
  
  /*Transient regime prior to run*/
  xIn = drand48();
  yIn = drand48();
  zIn = drand48();
  /*Loop on timesteps for the transient regime*/
  for(int i=0;i<transientNbOfTimeSteps;i++)
    {
      /*Lorenz-63 equations*/
      xOut = xIn + dt*sigma*(yIn-xIn);
      yOut = yIn + dt*(rho*xIn - yIn - xIn*zIn);
      zOut = zIn + dt*(xIn*yIn - beta*zIn);
      /*Switch for next timestep*/
      xIn = xOut; yIn = yOut; zIn = zOut;
    }

  /*Actual control run*/
  /*Open output files for binary writing*/
  /*Instantaneous trajectories*/
  x_time_serie.open("x_time_serie.datout");
  y_time_serie.open("y_time_serie.datout");
  z_time_serie.open("z_time_serie.datout");
  /*Accumulated values of the coordinates*/
  x_accumulated.open("x_accumulated.datout");
  y_accumulated.open("y_accumulated.datout");
  z_accumulated.open("z_accumulated.datout");
  
  /*Define and initialize accumulated coordinates*/
  double accX = 0.0; double accY = 0.0; double accZ = 0.0;
  /*Start looping on timesteps*/
  /*First loop on chunks of timesteps*/
  for(int chunkID=0;chunkID<nbOfChunks;chunkID++)
    {
      /*Display percentage on chunks*/
      if(chunkID%(nbOfChunks/100)==0){k++; cout << k << "%\r"; fflush(stdout);}
      /*Then on timesteps for the current chunk*/
      for(int i=0;i<totNbOfTimesteps;i++)
	{

	  /*Simple percentage display*/

	  /*Lorenz-63 equations*/
	  xOut = xIn + dt*sigma*(yIn-xIn);
	  yOut = yIn + dt*(rho*xIn - yIn - xIn*zIn);
	  zOut = zIn + dt*(xIn*yIn - beta*zIn);

	  accX += xOut; accY += yOut; accZ += zOut;

	  /*Write to disk every 100 iterations*/
	  /*Comment the following lign 
	    along with l. 78 to write evevry timesteps*/

	  if(i%sampleTime==0){
	    /*Write coordinates in file (binary output) 
	      every 100 iterations
	    x_time_serie.write((char*)&xOut, sizeof(double));
	    y_time_serie.write((char*)&yOut, sizeof(double));
	    z_time_serie.write((char*)&zOut, sizeof(double));*/
	    
	    /*Write coordinates in file (formatted output)*/
	    // x_time_serie << xOut << endl;
	    // y_time_serie << yOut << endl;
	    // z_time_serie << zOut << endl;

	    /*Average*/
	    accX /= sampleTime; accY /= sampleTime; accZ /= sampleTime;
	  /*Write accumulated coordinates over sampleTime iterations 
	    in file (binary output)*/
	    x_accumulated.write((char*)&accX, sizeof(double));
	    y_accumulated.write((char*)&accY, sizeof(double));
	    z_accumulated.write((char*)&accZ, sizeof(double));
	    
	    /*Write accumulated coordinates in file (formatted output)*/
	    // x_accumulated << accX << endl;
	    // y_accumulated << accY << endl;
	    // z_accumulated << accZ << endl;
	    
	    accX = 0.0; accY=0.0; accZ=0.0;
	    }
	  /*Switch for next timestep*/
	  xIn = xOut; yIn = yOut; zIn = zOut;
      
	}
    }
}
  
