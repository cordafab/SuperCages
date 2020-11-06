#include "environment.h"


Environment::Environment()
{
   initController();
}

Environment::~Environment()
{

}

void Environment::initController()
{
   Controller * controller = Controller::get();

   controller->character               =   & character;
   controller->cage                    =   & cage;
   controller->skeleton                =   & skeleton;

   controller->cageWeights             =   & cageWeights;

   controller->nc                      =   & nc;
   controller->mvc                     =   & mvc;

   controller->skeletonWeights         =   & skeletonWeights;

   controller->lbs                     =   & lbs;
   controller->dqs                     =   & dqs;
   controller->cor                     =   & cor;

   controller->corWeights              =   & corWeights;

   controller->asyncAnimator           =   & asyncAnimator;

   controller->cageTranslator          =   & cageTranslator;
   controller->cageUpdater             =   & cageUpdater;
   controller->skeletonUpdater         =   & skeletonUpdater;

   controller->skeletonUpdaterWeights  =   & skeletonUpdaterWeights;
}
