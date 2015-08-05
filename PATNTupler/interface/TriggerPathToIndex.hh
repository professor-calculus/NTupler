#ifndef TRIGGERPATHTOINDEX_HH
#define TRIGGERPATHTOINDEX_HH

#include <vector>
#include <string>
#include <unordered_map>

using std::vector;
using std::string;
using std::unordered_map;


namespace ran{
  class TriggerPathToIndex{

  public:
    TriggerPathToIndex(){}
    TriggerPathToIndex(const vector<string> addedTriggerPaths){
      triggerPaths_.insert(triggerPaths_.begin(),addedTriggerPaths.begin(),addedTriggerPaths.end());
      for (unsigned int i = 0, imax = triggerPaths_.size(); i < imax; ++i){ 
	pathToIndexMap_.insert(make_pair(triggerPaths_[i],i));
      }
    }

    size_t size(){ return triggerPaths_.size();}
    unsigned int getTrigIndex(const string triggerPath) {return pathToIndexMap_[triggerPath];}//WARNING, if triggerPath doesn't exist it will be added and I don't know what the associated int will be. Need to take care of the situation where the trigger path doesn't exist

  private:

    vector<string> triggerPaths_;
    unordered_map<string, unsigned int> pathToIndexMap_;


  };
}
#endif
