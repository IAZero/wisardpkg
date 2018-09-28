#include "include/wisardpkg.hpp"

namespace wp = wisardpkg;

using namespace std;

int main(int argc, const char * argv[]){
  wp::Wisard w(3, {
    {"bleachingActivated", true}
  });
  vector<vector<int>> X(1);
  vector<string> y(1);

  X[0] = {1,1,1,0,0,0};
  y[0] = 'a';
  w.train(X,y);
  cout << wp::__version__ << endl;

  auto out = w.classify(X);
  cout << "class: " << out[0] << endl;

  wp::Discriminator d(3, 6, {
    {"base", 2}
  });

  d.train(X);
  auto dout = d.classify(X[0]);
  cout << "rams = " << dout[0] << " : " << dout[1] << endl;

  wp::ClusWisard clus(3, 0.1, 100, 3, {
    {"bleachingActivated", true}
  });

  clus.train(X, y);
  auto clusout = clus.classify(X);
  cout << "clus class: " << clusout[0] << endl;
}
