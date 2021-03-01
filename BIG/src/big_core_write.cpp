#include "../include/big_core_write.hpp"

namespace big
{
    using half_float::half;
    using half_float::half_cast;
    BigCoreWrite::BigCoreWrite(const std::string& fileName,  uint64_t imageHeight, uint64_t imageWidth, uint64_t numberOfPlanes, const std::vector<ChunkIds>& dataOrder)
    {
        this->numberOfImages = 0;
        this->imageHeight = imageHeight;
        this->imageWidth = imageWidth;
        this->numberOfPlanes = numberOfPlanes;
        this->dataOrder = dataOrder;
        this->fileName = remove_extension(fileName);

        // prepare supporting structures
        initSupportingStructures();

        // open file
        file.open(fileName, std::ios_base::binary | std::ios_base::out);
        if (!file) throw "Unable to open file for writing!";

        // write magic number
        file.write(MAGIC.c_str(), CHUNK_LENGTH);

        // write meta-data, but do not write the data itself
        for (const auto& id : CoreChunkIds) {
            if (!writeChunk(id)) throw "Error while writing file!";
        }

        //create xml document
        createXMLFile();


    }

    BigCoreWrite::~BigCoreWrite()
    {
        file.seekp(CHUNK_LENGTH);
        if (!writeChunk(ChunkIds::NUMBER_OF_IMAGES)) throw "Error while writing file!";
        file.seekp(0, std::ios_base::end);
        if (!writeChunk(ChunkIds::XML)) throw "Error while writing file!";
        if (file.is_open()) file.close();
    }

    template<typename T>
    void BigCoreWrite::addEntity(std::shared_ptr<T> data, uint64_t index, DataTypes dataType)
    {
        numberOfImages++;
        /*file.seekp(CHUNK_LENGTH);
        if (!writeChunk(ChunkIds::NUMBER_OF_IMAGES)) throw "Error while writing file!";
        file.seekp(0, std::ios_base::end);*/

        if (index > numberOfImages)
            throw "Index out of bounds!";

        big::BigCoreBase::setEntitySizes(dataType); 

        const auto id = ChunkIds::DATA;
        file.write(reinterpret_cast<const char*>(&id), CHUNK_LENGTH);
        uint64_t complement = entitySizes[index] % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - entitySizes[index] % CHUNK_LENGTH : 0;
        uint64_t length = 2*CHUNK_LENGTH + entitySizes[index] + complement;
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(reinterpret_cast<const char*>(&index), sizeof(index));
        file.write(reinterpret_cast<const char*>(&dataType), sizeof(DataTypes)); //write data type
        file.write(reinterpret_cast<const char*>(data.get()), entitySizes[index]);
        const uint64_t zero = 0;
        file.write(reinterpret_cast<const char*>(&zero), complement);

    }

    template void BigCoreWrite::addEntity(std::shared_ptr<uint8_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<uint16_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<uint32_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<uint64_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<int8_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<int16_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<int32_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<int64_t> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<half> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<float> data, uint64_t index, DataTypes dataType);
    template void BigCoreWrite::addEntity(std::shared_ptr<double> data, uint64_t index, DataTypes dataType);

    template<typename T>
    void BigCoreWrite::pushEntity(std::shared_ptr<T> data, DataTypes dataType)
    {
        uint64_t index = numberOfImages;
        numberOfImages++;
        

        if (index > numberOfImages)
            throw "Index out of bounds!";

        big::BigCoreBase::setEntitySizes(dataType);

        const auto id = ChunkIds::DATA;
        file.write(reinterpret_cast<const char*>(&id), CHUNK_LENGTH);
        uint64_t complement = entitySizes[index] % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - entitySizes[index] % CHUNK_LENGTH : 0;
        uint64_t length = 2 * CHUNK_LENGTH + entitySizes[index] + complement;
        file.write(reinterpret_cast<const char*>(&length), sizeof(length));
        file.write(reinterpret_cast<const char*>(&index), sizeof(index));
        file.write(reinterpret_cast<const char*>(&dataType), sizeof(DataTypes)); //write data type
        file.write(reinterpret_cast<const char*>(data.get()), entitySizes[index]);
        const uint64_t zero = 0;
        file.write(reinterpret_cast<const char*>(&zero), complement);

    }
    template void BigCoreWrite::pushEntity(std::shared_ptr<uint8_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<uint16_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<uint32_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<uint64_t> data, DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<int8_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<int16_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<int32_t> data, DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<int64_t> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<half> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<float> data,  DataTypes dataType);
    template void BigCoreWrite::pushEntity(std::shared_ptr<double> data,  DataTypes dataType);

    bool BigCoreWrite::writeChunk(ChunkIds id)
    {
        const uint64_t zero = 0;
        switch (id) {
        case ChunkIds::NUMBER_OF_IMAGES:
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&CHUNK_LENGTH), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&numberOfImages), sizeof(numberOfImages));
            file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH - sizeof(numberOfImages));
            break;
        case ChunkIds::IMAGE_HEIGHT:
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&CHUNK_LENGTH), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&imageHeight), sizeof(imageHeight));
            file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH - sizeof(imageHeight));
            break;
        case ChunkIds::IMAGE_WIDTH:
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&CHUNK_LENGTH), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&imageWidth), sizeof(imageWidth));
            file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH - sizeof(imageWidth));
            break;
        case ChunkIds::NUMBER_OF_PLANES:
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&CHUNK_LENGTH), CHUNK_LENGTH);
            file.write(reinterpret_cast<const char*>(&numberOfPlanes), sizeof(numberOfPlanes));
            file.write(reinterpret_cast<const char*>(&zero), CHUNK_LENGTH - sizeof(numberOfPlanes));
            break;
        case ChunkIds::DATA_ORDER:
        {
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            uint64_t l = dataOrder.size() * sizeof(DataTypes);
            uint64_t length = l + (l % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - l % CHUNK_LENGTH : 0);
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            for (const auto& i : dataOrder) {
                file.write(reinterpret_cast<const char*>(&i), sizeof(ChunkIds));
            }
            file.write(reinterpret_cast<const char*>(&zero), length - l);
            break;
        }
        case ChunkIds::XML:
        {
          std::string xmlFile = GetXMLString(); 
          uint64_t l = (xmlFile.size() + 1) * sizeof(char);
          uint64_t length = l + (l % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - l % CHUNK_LENGTH : 0);
          file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
          file.write(reinterpret_cast<const char*>(&length), sizeof(length));
          file.write(reinterpret_cast<const char*>(&xmlFile[0]), l);
          file.write(reinterpret_cast<const char*>(&zero), length - l);
          break;
        }
      /*  case ChunkIds::DATA_TYPE:
        {
            file.write(reinterpret_cast<char*>(&id), CHUNK_LENGTH);
            uint64_t l = dataType.size() * sizeof(DataTypes);
            uint64_t length = l + (l % CHUNK_LENGTH > 0 ? CHUNK_LENGTH - l % CHUNK_LENGTH : 0);
            file.write(reinterpret_cast<const char*>(&length), sizeof(length));
            for (const auto& i : dataType) {
                file.write(reinterpret_cast<const char*>(&i), sizeof(DataTypes));
            }
            file.write(reinterpret_cast<const char*>(&zero), length - l);
            break;
        }*/
        }
        return !file.fail();
    }
    std::string BigCoreWrite::remove_extension(const std::string& fileName) {
      
      size_t lastdot = fileName.find_last_of(".");
      if (lastdot == std::string::npos) return fileName;
      return fileName.substr(0, lastdot);
      
    }
    //create XML file
    void BigCoreWrite::createXMLFile() {
      if (!file_exist(xmlFileName)) {
        auto declarationNode = doc.append_child(pugi::node_declaration);
        declarationNode.append_attribute("version") = "1.0";
        declarationNode.append_attribute("encoding") = "ISO-8859-1";
        declarationNode.append_attribute("standalone") = "yes";
        // A valid XML doc must contain a single root node of any name
        auto root = doc.append_child("MyRoot");
        // Save XML tree to file.
        // Remark: second optional param is indent string to be used;
        // default indentation is tab character.
        //bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
        //if (!saveSucceeded)
          //throw "Xml save error";
      }
     
     
    }

    //write xml to string write
    struct xml_string_writer : pugi::xml_writer
    {
      std::string result;

      virtual void write(const void* data, size_t size)
      {
        result.append(static_cast<const char*>(data), size);
      }
    };

    //xml node to string
    std::string node_to_string(pugi::xml_node node)
    {
      xml_string_writer writer;
      node.print(writer);

      return writer.result;
    }

    //get string with XML document
    std::string BigCoreWrite::GetXMLString() {
      return node_to_string(doc.document_element());
    }

    //check if the file exist
    bool BigCoreWrite::file_exist(const std::string& name) {
      if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
      }
      else {
        return false;
      }
    }

    void BigCoreWrite::AddAttributeToXmlInt(std::string nameOfAtribute, int  in_intVal, std::string hint) {
      /*pugi::xml_parse_result result = doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      if (!result)
      {
        throw "Parse error , character pos= " + result.offset;
      }*/
      // A valid XML document must have a single root node
      pugi::xml_node root = doc.document_element();
      /// [load xml file]
      /// [add child elements]
      // Append some child elements below root
      // Add as last element
      pugi::xml_node nodeChild = root.append_child(nameOfAtribute.c_str());
      nodeChild.append_attribute("hint") = hint.c_str();
      nodeChild.append_attribute("intVal") = in_intVal;
     /* bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
    }

    void BigCoreWrite::AddAttributeToXmlBool(std::string nameOfAtribute, bool  in_boolVal, std::string hint) {
     /* pugi::xml_parse_result result = doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      if (!result)
      {
        throw "Parse error , character pos= " + result.offset;
      }*/
      pugi::xml_node root = doc.document_element();

      pugi::xml_node nodeChild = root.append_child(nameOfAtribute.c_str());
      nodeChild.append_attribute("hint") = hint.c_str();
      nodeChild.append_attribute("boolVal") = in_boolVal;
    /*  bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
    }

    void BigCoreWrite::AddAttributeToXmlDouble(std::string nameOfAtribute, double in_doubleVal, std::string hint) {
     /* pugi::xml_parse_result result = doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      if (!result)
      {
        throw "Parse error , character pos= " + result.offset;
      }*/
      pugi::xml_node root = doc.document_element();

      pugi::xml_node nodeChild = root.append_child(nameOfAtribute.c_str());
      nodeChild.append_attribute("hint") = hint.c_str();
      nodeChild.append_attribute("doubleVal") = in_doubleVal;
     /* bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
    }

    void BigCoreWrite::AddAttributeToXmlString(std::string nameOfAtribute, std::string in_stringVal, std::string hint) {
     /* pugi::xml_parse_result result = doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      if (!result)
      {
        throw "Parse error , character pos= " + result.offset;
      }*/
      pugi::xml_node root = doc.document_element();

      pugi::xml_node nodeChild = root.append_child(nameOfAtribute.c_str());
      nodeChild.append_attribute("hint") = hint.c_str();
      nodeChild.append_attribute("stringVal") = in_stringVal.c_str();
      /*bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
    }


    bool BigCoreWrite::RemoveNodesByName(std::string nameOfAtribute) {
      //doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      pugi::xml_node root = doc.document_element();

      bool deleted = false;
      while (root.remove_child(nameOfAtribute.c_str()))
      {
        deleted= true;
      }
     /* bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
      /// [r
      return deleted;
    }
    bool BigCoreWrite::RemoveNodeByName(std::string nameOfAtribute) {
      doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      pugi::xml_node root = doc.document_element();

      bool deleted = false;
      if (root.remove_child(nameOfAtribute.c_str()))
      {
        deleted = true;
      }
     /* bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
      /// [r
      return deleted;
    }

    void BigCoreWrite::RemoveAllNodes() {
      doc.load_file(xmlFileName.c_str(), pugi::parse_default | pugi::parse_declaration);
      pugi::xml_node root = doc.document_element();
      for (pugi::xml_node child = root.first_child(); child; )
      {
        // Get next child node before possibly deleting current child
        pugi::xml_node next = child.next_sibling();
        //remove node
        child.parent().remove_child(child);

        child = next;
      }
      /*bool saveSucceeded = doc.save_file(xmlFileName.c_str(), PUGIXML_TEXT("  "));
      if (!saveSucceeded)
        throw "Xml save error";*/
    }
}
