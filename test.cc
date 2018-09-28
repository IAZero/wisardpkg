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
  cout << wp::__version__ << "\n\n" << w.json() << endl;

  auto out = w.classify(X);
  cout << "class: " << out[0] << endl;
}
