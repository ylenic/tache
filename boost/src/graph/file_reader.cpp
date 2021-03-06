#include "file_reader.hpp"

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

//--------------------------------------------
// METHODES PUBLIQUES
//--------------------------------------------

/// \brief Ajoute une arette
/// \param i1 index du premier point 
/// \param i2 index du deuxieme point
/// \param linkType descripteur du type d'arrete
/// \param found booleen resultat
/// \param e edge_descriptor de l'arrete
/// \param g double pointeur sur le Graph ou il faut ajouter la relation
/// \return le type de point
int addEdge(vertex_descriptor & v, vertex_descriptor & vv, std::string linkType, bool & found, edge_descriptor & e, Graph & g){
   int res = PEER;
   //si on veut mettre des poids
   // 		if(found) (**g)[e].weight = xx%13 * 2 + 5;
   if(linkType=="PEER"){
      // Pas besoin d'ajouter deux fois parce que les arcs en PEER sont presents en double
      boost::tie(e,found) = boost::add_edge( v,vv,g);
      res=PEER;
      //  boost::tie(e,found) = boost::add_edge(vv,v,g);
      // Peer vers Client
   }else if(linkType=="C2P"){
      boost::tie(e,found) = boost::add_edge(vv,v,g);
      res=C2P;
   }else if(linkType=="P2C"){
      boost::tie(e,found) = boost::add_edge(v,vv,g);
      res=P2C;
   }
   // Ajout a l'adjacence du currentVertex -> NON vous avez �a grace � la structure du graphe, suffit d'iterer dessus
   return res;
}


/// \brief Fonction de lecture du fichier d'adjacence
/// \param g Matrice d'adjacence à remplir
/// \throws ReaderException when an error occurs
void parse(std::string const & filename, Graph & g)
{
   int i1, i2;
   bool line_error = false, found= false;
   edge_descriptor e;
   std::string line, tempString, linkType;
   vertex_descriptor v,vv ;
   //mapping
   typedef  std::map< int ,  Graph::vertex_descriptor> as_number_to_vertex_type;
   as_number_to_vertex_type as_number_to_vertex;

   try
   {
      std::ifstream file(filename.c_str());

      if( file.is_open() )
      {
	 //remplissage de la matrice
	 while(std::getline(file, line)) {
	    // std::cout << "je lis la ligne" << file.tellg() << std::endl;
	    std::istringstream lineStream(line);
	    if(lineStream >> tempString) {
	       std::istringstream in1(tempString);
	       if(lineStream >> tempString) {
		  std::istringstream in2(tempString);
		  //Si tout rentre dans chacun des conteneurs...
		  if(lineStream >> linkType && in1 >> i1 && in2 >> i2) {
		     //
	//	     std::cout << i1 << "," << i2 << std::endl;  
		     as_number_to_vertex_type::const_iterator found_i1 =  as_number_to_vertex.find(i1);
		     if( found_i1 == as_number_to_vertex.end())
		     {
			v = boost::add_vertex(g);
			as_number_to_vertex[i1] = v;
			g[v].asn=i1;
		     }
		     else v = found_i1->second;

		     as_number_to_vertex_type::const_iterator found_i2 =  as_number_to_vertex.find(i2);
		     if( found_i2 == as_number_to_vertex.end())
		     {
			vv = boost::add_vertex(g);
			as_number_to_vertex[i2] = vv;
			g[vv].asn=i2;
		     }
		     else vv = found_i2->second;

		     int type = addEdge(v, vv, linkType, found, e, g);
		     if(!found)	  line_error=true;
		     else g[e].link_type = type;
		  }
		  else {
		     line_error=true;
		  }
	       }
	       else  line_error=true;
	    }
	    else  line_error=true;
	 }
	 file.close();
      }
      else{
	 throw ReaderException("Fichier inexistant ou non lisible", filename,ReaderException::CRITICAL);
      }
   }
   catch(const std::exception & e)
   {
      throw ReaderException("Erreur lors de la lecture du fichier", filename, ReaderException::CRITICAL);
   }

   if(line_error)
   {
      throw ReaderException("Erreur lors de la lecture d'une ou plusieurs ligne", filename, ReaderException::NON_BLOCKING);
   }
   std::cout << "num vertex:" << boost::num_vertices(g) << std::endl;

}

//--------------------------------------------
// METHODES PRIVEES
//--------------------------------------------

void read_triplet(std::string const & filename,std::set<std::size_t> &  transit_as)
{
   typedef boost::tokenizer< boost::char_separator<char> >   tokenizer;
   boost::char_separator<char> sep("\t");
   try
   {
      std::ifstream file(filename.c_str());

      if( file.is_open() )
      {
	 std::string line;
	 //remplissage de la matrice
	 while(std::getline(file, line))
	 {
	    tokenizer tokens(line, sep);
	    std::vector<std::string> parts(tokens.begin(), tokens.end());
	    if( parts.size() == 3)
	    {
	       transit_as.insert( boost::lexical_cast<std::size_t>( parts.at(1) ) );
	    }
	 }
      }
      file.close();
   }
   catch(const std::exception & e)
   {
      throw ReaderException("Erreur lors de la lecture du fichier", filename, ReaderException::CRITICAL);
   }
}
