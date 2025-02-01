#include "Time.h"

namespace mb {
namespace helpers {

void millisecondsToSecondsMicroseconds(int milliseconds, int& seconds, int& microseconds) {
   int total_microseconds = milliseconds * 1000; 
   seconds = total_microseconds / 1000000; 
   microseconds = total_microseconds % 1000000;
}

} // helpers
} // mb