

#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include "SimulationParameters.h"
#include "BarnesHutParticleSystem.h"

using namespace std;

bool checkCmdLineFlag(const int argc, char** argv, const char* string)
{
	bool flag = false;
	if(argc > 1){
		for(int i=1;i<argc;i++){
			if(strcmp(&argv[i][1], string) == 0){
				flag = true;
				break;
			}
		}
	}

	return flag;
}



int getCmdLineParameterInt(const int argc, char** argv, const char* string, const int defaultValue)
{
	int parameter = defaultValue;
	if(argc > 1){
		for(int i=1;i<argc;i++){
			char* str = &argv[i][1];
			char* eqs = strchr(str, '=');

			if(eqs != NULL){
				int index = (int)(eqs-str);
				char substring[50];
				strncpy(substring, str, index);
				substring[index] = '\0';

				if(strcmp(substring, string)==0){
					parameter = atoi(&eqs[1]);
					break;
				}
			}
		}
	}
	return parameter;
}


float getCmdLineParameterFloat(const int argc, char** argv, const char* string, const float defaultValue)
{
	float parameter = defaultValue;
	if(argc > 1){
		for(int i=1;i<argc;i++){
			char* str = &argv[i][1];
			char* eqs = strchr(str, '=');

			if(eqs != NULL){
				int index = (int)(eqs-str);
				char substring[50];
				strncpy(substring, str, index);
				substring[index] = '\0';

				if(strcmp(substring, string)==0){
					parameter = atof(&eqs[1]);
					break;
				}
			}

		}
	}
	return parameter;
}

void displayDeviceProperties()
{
	// Set up CUDA device 
	cudaDeviceProp properties;

	cudaGetDeviceProperties(&properties,0);

	int fact = 1024;
	int driverVersion, runtimeVersion;

	cudaDriverGetVersion(&driverVersion);
    cudaRuntimeGetVersion(&runtimeVersion);

	std::cout << "************************************************************************" << std::endl;
	std::cout << "                          GPU Device Properties                         " << std::endl;
	std::cout << "************************************************************************" << std::endl;
	std::cout << "Name:                                    " << properties.name << std::endl;
	std::cout << "CUDA driver/runtime version:             " << driverVersion/1000 << "." << (driverVersion%100)/10 << "/" << runtimeVersion/1000 << "." << (runtimeVersion%100)/10 << std::endl;
	std::cout << "CUDA compute capabilitiy:                " << properties.major << "." << properties.minor << std::endl;
	std::cout << "Number of multiprocessors:               " << properties.multiProcessorCount << std::endl;                           
	std::cout << "GPU clock rate:                          " << properties.clockRate/fact << " (MHz)" << std::endl;
	std::cout << "Memory clock rate:                       " << properties.memoryClockRate/fact << " (MHz)" << std::endl;
	std::cout << "Memory bus width:                        " << properties.memoryBusWidth << "-bit" << std::endl;
	std::cout << "Theoretical memory bandwidth:            " << (properties.memoryClockRate/fact*(properties.memoryBusWidth/8)*2)/fact <<" (GB/s)" << std::endl;
	std::cout << "Device global memory:                    " << properties.totalGlobalMem/(fact*fact) << " (MB)" << std::endl;
	std::cout << "Shared memory per block:                 " << properties.sharedMemPerBlock/fact <<" (KB)" << std::endl;
	std::cout << "Constant memory:                         " << properties.totalConstMem/fact << " (KB)" << std::endl;
	std::cout << "Maximum number of threads per block:     " << properties.maxThreadsPerBlock << std::endl;
	std::cout << "Maximum thread dimension:                [" << properties.maxThreadsDim[0] << ", " << properties.maxThreadsDim[1] << ", " << properties.maxThreadsDim[2] << "]" << std::endl;
	std::cout << "Maximum grid size:                       [" << properties.maxGridSize[0] << ", " << properties.maxGridSize[1] << ", " << properties.maxGridSize[2] << "]" << std::endl;
	std::cout << "**************************************************************************" << std::endl;
	std::cout << "                                                                          " << std::endl;
	std::cout << "**************************************************************************" << std::endl;
}




int main(int argc, char** argv)
{
	int numbodies = 512*256;  // must be a power of 2!!
	SimulationParameters parameters;

	if(checkCmdLineFlag(argc, argv, "plummer") == true){
		parameters.model = plummer_model;
	}
	else if(checkCmdLineFlag(argc, argv, "colliding-disks") == true){
		parameters.model = colliding_disk_model;
	}
	else{
		parameters.model = disk_model;
	}

	parameters.opengl = checkCmdLineFlag(argc, argv, "opengl");
	parameters.debug = checkCmdLineFlag(argc, argv, "debug");
	parameters.benchmark = checkCmdLineFlag(argc, argv, "benchmark");
	parameters.fullscreen = checkCmdLineFlag(argc, argv, "fullscreen");
	parameters.iterations = getCmdLineParameterInt(argc, argv, "iterations", 50);
	parameters.timestep = getCmdLineParameterFloat(argc, argv, "timestep", 0.001);
	parameters.gravity = getCmdLineParameterFloat(argc, argv, "gravity", 1.0);
	parameters.dampening = getCmdLineParameterFloat(argc, argv, "dampening", 1.0);


	displayDeviceProperties();


	BarnesHutParticleSystem *particles = new BarnesHutParticleSystem(parameters, numbodies);
	particles->reset();

	for(int i = 0 ; i < parameters.iterations ; i++){
		particles->update();
	}
}