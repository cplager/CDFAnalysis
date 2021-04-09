// -*- C++ -*-
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
   const int kSize = 6;   
   double array[ kSize ] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 };

   std::vector< double > dVec( array, array + kSize );

   cout << "size " << dVec.size() << endl;
   for (int loop = 0; loop < dVec.size(); ++loop)
   {
      cout << dVec[loop] << " ";
   }
   cout << endl;
}
