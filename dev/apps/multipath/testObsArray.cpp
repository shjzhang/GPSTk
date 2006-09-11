#include <iostream>
#include <set>

#include "DayTime.hpp"
#include "ObsArray.hpp"
#include "ValarrayUtils.hpp"


using namespace std;
using namespace gpstk;
using namespace ValarrayUtils;

int main(int argc, char* argv[])
{
   try {

      ObsArray oa;

      cout << oa.add(RinexObsHeader::C1) << endl;
      cout << oa.add(RinexObsHeader::P2) << endl;
      cout << oa.add("C1-P2") << endl;
      cout << oa.add("P1-wl1*L1-1/(1-gamma)*(wl1*L1-wl2*L2)");
      
      cout << "There are " << oa.getNumObsTypes() << " obs indices." << endl;

      cout << "Reading input files." << endl;
//      oa.load("netrs027.06o.30s","netrs027.06n");
      oa.load("arl2810.06o","arl2810.06n");
      cout << "Done reading." << endl;

      cout << "Values directly using operator(size_t, size_t): " << endl;
      for (int j=0; j <oa.getNumObsTypes(); j++)
         for (int i=0; i<12; i++)
         {
            cout << "(" << i << "," << j << ")";
            cout << setprecision(12) << oa(i,j) << endl;
         }

      cout << "Getting values with a slice: " << endl;
      slice myslice(0,20,1);  
      valarray <double> copyObs = oa.observation[myslice];
      slice_array <double> obsSample = oa.observation[myslice];
      cout << "There are " << copyObs.size() << " elements in this slice." << endl;
      for (int i=0; i<20; i++)
         cout << copyObs[i] << endl;

      cout << "Operations on a slice: " << endl;
      cout << "Mean value is: " << copyObs.sum() / copyObs.size() << endl;
      //cout << "Mean value is: " << oa.observation[myslice].sum() / oa.observation[myslice].size() << endl;

      RinexPrn thisPrn(9,systemGPS);
      valarray<bool> prnIdx = (oa.satellite==thisPrn);
      valarray<double> prnObs = oa.observation[prnIdx];
      valarray<DayTime> prnTime = oa.epoch[prnIdx];
      cout << "Data for PRN 9:" << endl;
      for (int i=0; i<12; i++)
      {
         cout << prnTime[i].GPSfullweek() << " ";
         cout << prnTime[i].GPSsow() << " ";
         cout << prnObs[i] << endl;
      }
      
      set<DayTime> allepochs = unique(oa.epoch);  
      cout << "Unique epochs:" << endl << allepochs << endl;
      
      set<RinexPrn> allprns = unique(oa.satellite);
      cout << "Unique satellites: " << endl << allprns << endl;

      set<long> allpasses = unique(oa.pass);
      cout << "Unique passes: " << endl << allpasses << endl;
      
      exit(0);
   }
   catch(Exception& ex)
   {
      cerr << "Exception caught" << endl;
      cerr << ex << endl;
   }
}
