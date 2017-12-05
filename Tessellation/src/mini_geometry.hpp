/* File: mini_geometry.h
   Author: T.J. Jankun-Kelly <tjk@cse.msstate.edu>
   Geometry for the Mini Cooper model.

The Cooper model is formed of various groups. Each group has a name (the
"part") that determines which part it belongs to. For example, the "Roof" part
possesses geometry that belongs to the roof of the car. To get the
starting and ending indices of the faces belonging to the part, use the groups
function; this can be used to render only that face (or to walk over all):
    MiniGeometry mini();
    <load vertex buffer and index buffer from mini.vertexdata & mini.indices>
    list<PartEntry> groups = mini.groups();
    list<PartEntry>::const_iterator it;
    for(it = groups.begin(); it != groups.end(); ++it)
    {
        string name = entry.name;
        int offset = sizeof(GLushort) * mini.indicesPerFace() * entry.start;
        int count = mini.indicesPerFace() * (entry.end - entry.start);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, offsset);
    }
*/

// Had some strange issues with VS17 here.
// If you don't put the preprocessor before the included library you don't get
// proper syntax highlighting for those libraries and for the whole header file.
#pragma once

#include <string>
#include <list>
#include <fstream>

// Stores info for a part: Its name, the starting index of faces that belong
// to it, and their ending index
struct PartEntry
{
    std::string name;
    int start;
    int end;
};

// The actual geometry class. Reads data for you.
class MiniGeometry
{
    private:
        int _numVertices;           // Total num of vertices's and attributes
                                    // Note: There are 8 entries per vertex:
                                    //       xyz for position
                                    //       xyz for normal
                                    //       uv  for texture coordinate
        float *data;                // Float to hold the data
                                    // 8*numVertices elements
        int _numFaces;              // Number of faces
        unsigned int _idxCount;     // Number of indices per face; usually 3
        unsigned short *_indices;   // Indices of vertex data per face. 
                                    // _idxCount * _numFaces elemets
        std::list<PartEntry> _groups;    // Part names and start/end indices
        
        
    public:   
        MiniGeometry(bool adjacency = false)
        {
            /* Read in geometry
               Format:
                numVertices
                vx vy vz nx ny nz tu tv
                ...
                numFaces
                vertexIndex0 vertexIndex1 ... vertexIndexN (N is 3 or 6)
                ...
                numGroups
                faceStart faceEnd name 
            */
            std::string filename;
            if(!adjacency)
            {
                filename = "mini_geometry.txt";
                _idxCount = 3;
            }
            else
            {
                filename = "mini_geometry_adjacency.txt";
                _idxCount = 6;
            }
            std::ifstream geom(filename.c_str());
               
            // Store the vertex + attribute data in one array. Easier for
            // copying to the GPU later
            geom >> _numVertices;
            data = new float[8*_numVertices];
            for(int i = 0; i < _numVertices; ++i)
            {
                for(int j = 0; j < 8; ++j)
                {
                    geom >> data[8*i+j];
                }
            }
                    
            // Store the face indices in one array of unsigned shorts for easier
            // copying to the GPU later
            geom >> _numFaces;
            _indices = new unsigned short[_idxCount*_numFaces];
            for(int i = 0; i < _numFaces; ++i)
            {
                for(unsigned int j = 0; j < _idxCount; ++j)
                {
                    geom >> _indices[_idxCount*i+j];
                }
            }
                    
            // Store groups (and names) in and ordered dictionary
            int numParts;
            geom >> numParts;
            for(int i = 0; i < numParts; ++i)
            {
                PartEntry entry;
                geom >> entry.start >> entry.end;
                geom.get(); // clear the space
                std::getline(geom, entry.name);
                _groups.push_back(entry);
            }
            
            geom.close();
        }
        
        MiniGeometry(const MiniGeometry& other)
        {
            data = 0;
            _indices = 0;
            *this = other;
        }
        
        virtual ~MiniGeometry()
        {
            delete [] data;
            delete [] _indices;
        }
        
        MiniGeometry& operator=(const MiniGeometry& rhs)
        {
            if(&rhs != this)
            {
                if(data != 0) delete [] data;
                if(_indices != 0) delete [] _indices;

                _numVertices = rhs._numVertices;
                data = new float[8*_numVertices];
                for(int i = 0; i < _numVertices; ++i)
                {
                    for(int j = 0; j < 8; ++j)
                    {
                        data[8*i+j] = rhs.data[8*i+j];
                    }
                }

                _numFaces = rhs._numFaces;
                _idxCount = rhs._idxCount;
                _indices = new unsigned short[_idxCount*_numFaces];
                for(int i = 0; i < _numFaces; ++i)
                {
                    for(unsigned int j = 0; j < _idxCount; ++j)
                    {
                        _indices[_idxCount*i+j] = rhs._indices[_idxCount*i+j];
                    }
                }
                
                _groups = rhs._groups;
            }
            return *this;
        }

        // Data access members. 
        int numVertices() const {return _numVertices;}
        float const* vertexdata() const {return data;}
        int numFaces() const {return _numFaces;}
        int indicesPerFace() const {return _idxCount; }
        unsigned short const* indices() const {return _indices;}
        std::list<PartEntry> groups() const {return _groups;}
        std::list<std::string> parts() const
        {
            std::list<std::string> names;
            std::list<PartEntry>::const_iterator it;
            for(it = _groups.begin(); it != _groups.end(); ++it)
            {
                names.push_back(it->name);
            }
            
            return names;
        }
};


