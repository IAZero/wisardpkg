#include "include/wisardpkg.hpp"

namespace wp = wisardpkg;

using namespace std;

int main(int argc, const char * argv[]){

  string baseOut = wp::Base64::encode("Hello base64");
  cout << baseOut << endl;
  cout << wp::Base64::decode(baseOut) << endl;

  wp::Wisard w(3);
  vector<vector<short>> X(1);
  vector<string> y(1);

  X[0] = {1,1,1,0,0,0};
  y[0] = 'a';
  wp::DataSet ds(X,y);
  w.train(ds);
  cout << wp::__version__ << endl;

  auto out = w.classify(ds);
  cout << "class: " << out[0] << endl;

  cout << "json: " << w.json() << endl;

  wp::Discriminator d(3, 6, {
    {"base", 2}
  });

  d.train(ds);
  wp::BinInput b(X[0]);
  auto dout = d.classify(b);
  cout << "rams = " << dout[0] << " : " << dout[1] << endl;

  wp::ClusWisard clus(3, 0.1, 100, 3, {
    {"bleachingActivated", true}
  });

  clus.train(ds);
  auto clusout = clus.classify(ds);
  cout << "clus class: " << clusout[0] << endl;
}
