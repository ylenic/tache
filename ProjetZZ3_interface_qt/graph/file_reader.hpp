#ifndef ____FILE_READER_HPP__
#define ____FILE_READER_HPP__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "graph.hpp"
#include "../exceptions/ReaderException.hpp"
#include "../utils/logger.hpp"
class File_reader{
   protected:
      std::string filename;
      // std::vector<int> peers;
      std::vector< std::vector<int> > peers;
      std::map<int,int> peersMap;
      std::vector<int> peersVector;
   public:
      void change_file_name(std::string);
      File_reader(std::string f):filename(f), peers(), peersMap(), peersVector(){;};
      void parse(Graph &);
      int getPeerASIndex(int);
   private:
      void addEdge(int, int, std::string, bool &, edge_descriptor &, Graph &);
      void addToPeersVector(int, int);
};

#endif