#include "id_generator.h"

using namespace Lefishe;

std::atomic<long> IDGenerator::counter = 0;
		
long IDGenerator::Generate(){
	return counter++;
}