#pragma once
#ifndef _BIG_CORE_WRITE_H_
#ifdef _BIG_CORE_WRITE_H_
#define BIGDLL  __declspec(dllexport)  
#else
#define BIGDLL  __declspec(dllimport)   
#endif
#define _BIG_CORE_WRITE_H_

#include "big_core_base.hpp"
#include "half.hpp"
#include "pugixml.hpp"

#include <fstream>
#include <memory>
#include <string>


namespace big
{
    class BIGDLL BigCoreWrite : public BigCoreBase
    {
    public:

        // Constructor. Constructs an empty container, sets dimensions of the container, data order and data type(s).
        // Opens the given file and writes meta-data, but does not write the data itself yet.
        BigCoreWrite(const std::string& fileName,  uint64_t imageHeight, uint64_t imageWidth, uint64_t numberOfPlanes = 1, const std::vector<ChunkIds>& dataOrder = defaultDataOrder);

        // Copy constructor forbidden.
        BigCoreWrite(const BigCoreWrite &) = delete;

        // Move constructor.
        BigCoreWrite(BigCoreWrite &&other) = default;

        // Destructor. 
        ~BigCoreWrite();

        // Copy-assignment operator forbidden.
        BigCoreWrite &operator=(const BigCoreWrite &) = delete;

        // Move-assignment operator.
        BigCoreWrite &operator=(BigCoreWrite &&rhs) = default;

      
        

    public:

        // Adds an entity to the container and writes it to the file, to selected index
        template<typename T>
        void addEntity(std::shared_ptr<T> data, uint64_t index, DataTypes dataType);

        // Adds an entity to the container and writes it to the file, to actual index
        template<typename T>
        void pushEntity(std::shared_ptr<T> data, DataTypes dataType);

        //Add int atribute to xml
        void AddAttributeToXmlInt(std::string nameOfAtribute, int  in_intVal, std::string hint);

        //Add bool atribute to xml
        void AddAttributeToXmlBool(std::string nameOfAtribute, bool in_boolVal, std::string hint);

        //Add  double to xml
        void AddAttributeToXmlDouble(std::string nameOfAtribute, double in_doubleVal, std::string hint);

        //Add  string to xml
        void AddAttributeToXmlString(std::string nameOfAtribute, std::string in_stringVal, std::string hint);

        //Get XML string from root node
        std::string GetXMLString();

        //remove all nodes with this name
        bool RemoveNodesByName(std::string nameOfAtribute);

        //remove first node with this name
        bool RemoveNodeByName(std::string nameOfAtribute);

        //remove all child nodes of myRoot
        void RemoveAllNodes();

    protected:

        // Write chunk's id, length and data.
        bool writeChunk(ChunkIds id);

        //create xml document
        void createXMLFile();

        //check if file exist
        bool file_exist(const std::string&name);

      

        std::string remove_extension(const std::string& filename);

    protected:

        std::ofstream file;             // attached file
       
    };

}

#endif // _BIG_CORE_WRITE_H_ 