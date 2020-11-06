#ifndef EXPORTFILES_H
#define EXPORTFILES_H

#include "skinning/weights.h"
#include "geom/transform.h"
#include <vector>

void saveMesh (const char                * filename,
               const std::vector<double> & vertices,
               const std::vector<int>    & faces   );

void saveOBJ (const char                * filename,
              const std::vector<double> & vertices,
              const std::vector<int>    & faces   );

void savePLY (const char                * filename,
              const std::vector<double> & vertices,
              const std::vector<int>    & faces   );

void saveWeights (const char    * filename,
                  const Weights * weights);

void saveAnimation (const char                                     * filename,
                    const std::vector<double>                      & t,
                    const std::vector<std::vector<double> >        & cageKeyframes,
                    const std::vector<std::vector<cg3::Transform>> & restSkelKeyframes,
                    const std::vector<std::vector<cg3::Transform>> & deformedSkelKeyframes);

void saveSkeleton (const char                        * filename,
                   const std::vector<cg3::Vec3d>     & joints,
                   const std::vector<int>            & fathers,
                   const std::vector<std::string>    & names);

void saveSkeleton_old (const char                            * filename,
                       const std::vector<cg3::Vec3d>         & joints,
                       const std::vector<std::pair<int,int>> & bones);

void saveSkelAnimation (const char                                     * filename,
                        const std::vector<double>                      & t,
                        const std::vector<std::vector<cg3::Transform>> & skelKeyframes);

void saveCageAnimation (const char                              * filename,
                        const std::vector<double>               & t,
                        const std::vector<std::vector<double> > & cageKeyframes);

void saveDoubleVec (const char * filename,
                    const std::vector<double> & params);

#endif // EXPORTFILES_H
