#ifndef IMPORTFILES_H
#define IMPORTFILES_H

#include <vector>
#include <geom/transform.h>
#include "skinning/weights.h"
#include "skinning/sparseWeights.h"

void loadMesh (const char                * filename,
               std::vector<double>        & vertices,
               std::vector<double>        & uv      ,
               std::vector<int>           & faces   );

void loadOBJ  (const char                * filename,
               std::vector<double>        & vertices,
               //std::vector<double>      & uv      ,
               std::vector<int>           & faces   );

void loadPLY  (const char                * filename,
               std::vector<double>        & vertices,
               std::vector<int>           & faces   );

void loadWeights (const char    * filename,
                  Weights       * weights);

void loadSparseWeights (const char     * filename,
                        SparseWeights  & weights);

void loadAnimation (const char                               * filename,
                    std::vector<double>                      & t,
                    std::vector<std::vector<double>>         & cageKeyframes,
                    std::vector<std::vector<cg3::Transform>> & restSkelKeyframes,
                    std::vector<std::vector<cg3::Transform>> & deformedSkelKeyframes);

void loadOFF (const char            * filename,
              std::vector<double>    & vertices,
              std::vector<int>       & faces   );

void loadMAXVOL (const char         * filename,
                 std::vector<int>   & selectedvertices );

void loadSkeleton (const char                  * filename,
                   std::vector<cg3::Vec3d> & joints, std::vector<cg3::Vec3d> & jointsRotations,
                   std::vector<int>            & fathers,
                   std::vector<std::string>    & names);

void loadSkelAnimation (const char                               * filename,
                        std::vector<double>                      & t,
                        std::vector<std::vector<cg3::Transform>> & skelKeyframes);

void loadCageAnimation (const char                       * filename,
                        std::vector<double>              & t,
                        std::vector<std::vector<double>> & cageKeyframes);

void loadDoubleVec (const char * filename,
                    std::vector<double> & params);

#endif // IMPORTFILES_H
