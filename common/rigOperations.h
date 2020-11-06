#ifndef RIGOPERATIONS_H
#define RIGOPERATIONS_H

#import <iostream>

void importRig();
void exportRig();
void clearRig();
void importCharacter(const std::string & filename);
void importSkeleton(const std::string & filename);
void importSkeletonWeights(const std::string & filename);
void importCage(const std::string & filename);
void importCageWeights(const std::string & filename);
void importCoR(const std::string & filename);
void importMAXVOL(const std::string & filename);

#endif // RIGOPERATIONS_H
