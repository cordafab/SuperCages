#include "geom/trimesh.h"

//METHODS DEFINITION

Trimesh::Trimesh()
{

}

Trimesh::Trimesh(const std::vector<double>    & _vertices ,
                 const std::vector<int>       & _tris     )
{
   create(_vertices, _tris);
}

bool Trimesh::create(const std::vector<double>  &_vertices,
                     const std::vector<int>     &_tris)
{
    clear();

    vertices = _vertices;
    tris = _tris;

    init();

    return true;
}

void Trimesh::init()
{
   trisNorm.resize(getNumTriangles()*3);
   verticesNorm.resize(getNumVertices()*3);

   buildAdjacency();
   updateNormals();
   updateBoundingBox();
}

void Trimesh::clear()
{
   vertices.clear();
   tris.clear();
   verticesNorm.clear();
   trisNorm.clear();
   _v2t.clear();
   _v2v.clear();
   _t2t.clear();
   boundingBox.clear();
}

void Trimesh::buildAdjacency()
{
   //CODE FROM CAGELAB
   _v2v.clear();
   _v2t.clear();
   _t2t.clear();

   _v2v.resize(getNumVertices());
   _v2t.resize(getNumVertices());
   _t2t.resize(getNumTriangles());

   std::set<edge>     edges;
   std::map<edge,int> edge2tri;

   for(int tId=0; tId<getNumTriangles(); ++tId)
   {
      int tIdPtr = tId * 3;
      for(int i=0; i<3; ++i)
      {
         int vId = tris[tIdPtr + i];
         _v2t[vId].push_back(tId);

         int adj = tris[tIdPtr + (i+1)%3];
         edge e = uniqueEdge(vId,adj);
         edges.insert(e);

         std::map<edge,int>::iterator query = edge2tri.find(e);
         if (query == edge2tri.end())
         {
            edge2tri[e] = tId;
         }
         else
         {
            int nbrTri = query->second;
            _t2t[tId].push_back(nbrTri);
            _t2t[nbrTri].push_back(tId);
         }
      }
   }
   //OLD
   for(std::set<edge>::iterator it=edges.begin(); it!=edges.end(); ++it)
   {
      edge e = *it;
      _v2v[e.first].push_back(e.second);
      _v2v[e.second].push_back(e.first);
   }

   //NEW
   /*for(int v=0; v<getNumVertices(); ++v) //per ogni punto v
   {
      std::vector<int> adjTris = _v2t[v]; //prendo i triangoli adiacenti al punto v
      std::vector<edge> unorderedEdgeLoop;
      unorderedEdgeLoop.reserve(adjTris.size());
      for(int t : adjTris)
      {
         //prendo l'edge opposto al vertice v
         int tPtr = t * 3;
         if(tris[tPtr+0] != v && tris[tPtr+1] != v) unorderedEdgeLoop.push_back(edge(tris[tPtr+0], tris[tPtr+1])); else
         if(tris[tPtr+1] != v && tris[tPtr+2] != v) unorderedEdgeLoop.push_back(edge(tris[tPtr+1], tris[tPtr+2])); else
         if(tris[tPtr+2] != v && tris[tPtr+0] != v) unorderedEdgeLoop.push_back(edge(tris[tPtr+2], tris[tPtr+0]));
      }

      //riordino gli edge opposti a v in modo che l'endpoint di un edge corrisponda con lo start del successivo
      std::vector<edge> orderedEdgeLoop;
      orderedEdgeLoop.reserve(adjTris.size());
      orderedEdgeLoop.push_back(unorderedEdgeLoop[0]);
      while(orderedEdgeLoop.size() != unorderedEdgeLoop.size())
      {
         int i = orderedEdgeLoop.size()-1;
         bool found = false;
         int j=0;
         while(!found)
         {
            if(orderedEdgeLoop[i].second == unorderedEdgeLoop[j].first)
            {
               found = true;
               orderedEdgeLoop.push_back(unorderedEdgeLoop[j]);
            }
            else
            {
               ++j;
            }
         }
      }

      //scorro gli edge e li inserisco nelle adiacenze
      for(int i = 0; i < orderedEdgeLoop.size(); ++i)
      {
         _v2v[v].push_back(orderedEdgeLoop[i].second);
      }
   }
   */

}

void Trimesh::updateNormals()
{
   updateTrisNormals();
   updateVerticesNormals();
}

void Trimesh::updateTrisNormals()
{
   #pragma omp parallel for schedule(static)
   for(unsigned long tId=0; tId<getNumTriangles(); ++tId)
   {
      const ulong tIdPtr = tId * 3;

      const ulong v0IdPtr = (tris[tIdPtr + 0]) * 3;
      const ulong v1IdPtr = (tris[tIdPtr + 1]) * 3;
      const ulong v2IdPtr = (tris[tIdPtr + 2]) * 3;

      const double v0x = vertices[v0IdPtr + 0];
      const double v0y = vertices[v0IdPtr + 1];
      const double v0z = vertices[v0IdPtr + 2];

      const double v1x = vertices[v1IdPtr + 0];
      const double v1y = vertices[v1IdPtr + 1];
      const double v1z = vertices[v1IdPtr + 2];

      const double v2x = vertices[v2IdPtr + 0];
      const double v2y = vertices[v2IdPtr + 1];
      const double v2z = vertices[v2IdPtr + 2];

      const double ux = (v1x - v0x);
      const double uy = (v1y - v0y);
      const double uz = (v1z - v0z);

      const double vx = (v2x - v0x);
      const double vy = (v2y - v0y);
      const double vz = (v2z - v0z);

      const double nx = uy * vz - uz * vy;
      const double ny = uz * vx - ux * vz;
      const double nz = ux * vy - uy * vx;

      const double lenghtN = std::sqrt( nx * nx + ny * ny + nz * nz );

      const double nxN = nx / lenghtN;
      const double nyN = ny / lenghtN;
      const double nzN = nz / lenghtN;

      trisNorm[tIdPtr + 0] = nxN;
      trisNorm[tIdPtr + 1] = nyN;
      trisNorm[tIdPtr + 2] = nzN;
   }
}

void Trimesh::updateVerticesNormals()
{
   #pragma omp parallel for schedule(static)
   for(ulong vId=0; vId<getNumVertices(); ++vId)
   {
      const std::vector<int> & neigh = v2t(vId);

      double sumx = 0.0;
      double sumy = 0.0;
      double sumz = 0.0;

      for(ulong i=0; i<neigh.size(); ++i)
      {
         const ulong neighIdPtr = neigh[i] * 3;

         sumx += trisNorm[neighIdPtr + 0];
         sumy += trisNorm[neighIdPtr + 1];
         sumz += trisNorm[neighIdPtr + 2];
      }

      sumx /= neigh.size();
      sumy /= neigh.size();
      sumz /= neigh.size();

      const double lenghtSum = std::sqrt( sumx * sumx + sumy * sumy + sumz * sumz );

      const double sumxN = sumx / lenghtSum;
      const double sumyN = sumy / lenghtSum;
      const double sumzN = sumz / lenghtSum;

      const ulong vIdPtr = vId * 3;

      verticesNorm[vIdPtr + 0] = sumxN;
      verticesNorm[vIdPtr + 1] = sumyN;
      verticesNorm[vIdPtr + 2] = sumzN;
   }
}

void Trimesh::updateBoundingBox()
{
   boundingBox.clear();
   for(int vId=0; vId<getNumVertices(); ++vId)
   {
      cg3::Vec3<double> v = getVertex(vId);
      boundingBox.min = boundingBox.min.min(v);
      boundingBox.max = boundingBox.max.max(v);
   }
}

void Trimesh::exportVerticesToEigen(Eigen::VectorXd & vx,
                                    Eigen::VectorXd & vy,
                                    Eigen::VectorXd & vz)
{
   unsigned int numVert = getNumVertices();
   vx.resize(numVert);
   vy.resize(numVert);
   vz.resize(numVert);

   for(unsigned int i=0; i < numVert; ++i)
   {
      vx[i] = vertices[i*3+0];
      vy[i] = vertices[i*3+1];
      vz[i] = vertices[i*3+2];
   }

   /*v = Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>
         (vertices.data(), vertices.size());*/
}
