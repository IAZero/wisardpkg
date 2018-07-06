#include <vector>

using namespace std;

class Entry{
public:
  const vector<int> features;
  const string label;

  Entry(){}
  Entry(const vector<int>& f):features(f){}
  Entry(const vector<int>& f, const string& l):features(f),label(l){}
};

class DataSet {
public:
  enum type {empty,supervised,semi,unsupervised};

  DataSet():dataset_type(DataSet::type::empty){}

  void append(const vector<int>& f){
    if(isEmpty()){
      dataset_type = DataSet::type::unsupervised;
    }
    if(isSupervised()){
      dataset_type = DataSet::type::semi;
    }
    entries.push_back(Entry(f));
  }

  void append(const vector<int>& f, const string& l){
    if(isEmpty()){
      dataset_type = DataSet::type::supervised;
    }
    if(isUnsupervised()){
      dataset_type = DataSet::type::semi;
    }
    entries.push_back(Entry(f,l));
  }

  const Entry& operator[](int i) const{
    return entries[i];
  }

  int size() const{
    return entries.size();
  }

  bool isEmpty() const{
    return entries.size() == 0;
  }

  bool isSupervised() const{
    return dataset_type == DataSet::type::supervised;
  }
  bool isSemiSupervised() const{
    return dataset_type == DataSet::type::semi;
  }
  bool isUnsupervised() const{
    return dataset_type == DataSet::type::unsupervised;;
  }
private:
  vector<Entry> entries;
  DataSet::type dataset_type;
};
