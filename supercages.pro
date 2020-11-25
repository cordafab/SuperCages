QT += core gui opengl xml widgets

TARGET   = Supercages
TEMPLATE = app

CONFIG += c++17

#FLAG FOR CUSTOM LIGHTS
DEFINES += CUSTOM_LIGHTS



## libQGLViewer

unix:!macx{ # Linux

    #apt install libglew-dev
    #apt install libqglviewer-dev-qt5
    #apt install freeglut3-dev

    INCLUDEPATH += /usr/include/QGLViewer
    LIBS += /usr/lib/x86_64-linux-gnu/libQGLViewer-qt5.so

    LIBS+= -lGLU
    LIBS+= -lglut
    LIBS+= -lGLEW

    DEFINES += GL_GLEXT_PROTOTYPES
}

macx:{ # Mac
   INCLUDEPATH += /Users/Shared/libs/include/libqglviewer
   LIBS += -F/Library/Frameworks -framework QGLViewer
   #DEFINES += CUSTOMSNAPSHOTQGL
}




## Eigen

unix:!macx{ # Linux
    #apt install libeigen3-dev
    INCLUDEPATH    += /usr/include/eigen3
}

macx:{ # Mac
   #brew install eigen
   INCLUDEPATH += /usr/local/include/eigen3
   #INCLUDEPATH += /Users/Shared/libs/include/eigen
}




## glm
unix:!macx{ # Linux
   # apt-get install libglm-dev
}

macx:{ # Mac
    #brew install eigen
    INCLUDEPATH += /usr/local/include/glm
    #INCLUDEPATH += /Users/Shared/libs/include/glm
}




## gsl

unix:!macx{ # Linux
   # apt-get install libgsl-dev
   LIBS+= -lgsl
   LIBS+= -lgslcblas
}

macx:{ # Mac
   LIBS += -L'/Users/Shared/libs/lib/gsl' -lgsl -lgslcblas
   INCLUDEPATH += /Users/Shared/libs/include/gsl
}




##Cinolib

unix:!macx{ # Linux
   INCLUDEPATH    += /libs/include/cinolib
   DEPENDPATH     += /libs/include/cinolib # force recompilation if cinolib
}

macx:{ # Mac
   INCLUDEPATH    += /Users/Shared/libs/include/cinolib
   DEPENDPATH     += /Users/Shared/libs/include/cinolib # force recompilation if cinolib changes
}




## OpenMP

unix:!macx{ # Linux
#   QMAKE_CXXFLAGS += -fopenmp
#   QMAKE_LFLAGS +=  -fopenmp
#   QMAKE_CFLAGS_RELEASE += -fopenmp
#   LIBS += -lgomp -lpthread
}

macx{ # Mac
#   QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
#   INCLUDEPATH +='/libs/include/libomp/libomp'
#   LIBS += -L'/libs/lib/libomp/libomp' -lomp
}




## Project files

HEADERS = \
    GUI/glCanvas.h \
    GUI/mainWindow.h \
    geom/trimesh.h \
    geom/boundingBox.h \
    geom/vec3.h \
    geom/edge.h \
    drawables/drawableObject.h \
    drawables/drawableTrimesh.h \
    common/importFiles.h \
    controller.h \
    GUI/qtUtils.h \
    rigs/cage.h \
    drawables/drawableCage.h \
    GUI/glUtils.h \
    drawables/pickerController.h \
    drawables/pickableObject.h \
    geom/plane.h \
    math/dualQuaternion.h \
    rigs/skeleton.h \
    drawables/drawableSkeleton.h \
    geom/transform.h \
    common/types.h \
    operators/cageReverser.h \
    operators/cageUpdater.h \
    operators/skeletonUpdater.h \
    skinning/weights.h \
    skinning/skeletonSkinning.h \
    skinning/linearBlendSkinning.h \
    skinning/meanValueCoordinates.h \
    skinning/cageSkinning.h \
    GUI/clickConverter.h \
    common/cageOperations.h \
    common/skeletonOperations.h \
    GUI/cagePanel.h \
    GUI/skeletonPanel.h \
    common/texture.h \
    common/exportFiles.h \
    rigs/character.h \
    drawables/drawableCharacter.h \
    GUI/characterPanel.h \
    common/toolsOperation.h \
    GUI/toolsPanel.h \
    common/characterOperations.h \
    math/leastSquareSolver.h \
    math/quaternion.h \
    geom/ray.h \
    skinning/dualQuaternionSkinning.h \
    skinning/noCageSkinning.h \
    common/animatorOperations.h \
    GUI/restPoseCanvas.h \
    external/JM/MEC.h \
    external/JM/point3.h \
    external/JM/MVCoordinates3D.h \
    GUI/rigPanel.h \
    common/rigOperations.h \
    animation/asyncAnimator.h \
    GUI/asyncAnimatorPanel.h \
    skinning/corSkinning.h \
    environment.h \
    skinning/sparseWeights.h

SOURCES = \
    main.cpp \
    GUI/glCanvas.cpp \
    GUI/mainWindow.cpp \
    geom/boundingBox.cpp \
    drawables/drawableObject.cpp \
    drawables/drawableTrimesh.cpp \
    common/importFiles.cpp \
    controller.cpp \
    GUI/qtUtils.cpp \
    drawables/drawableCage.cpp \
    GUI/glUtils.cpp \
    drawables/pickableObject.cpp \
    drawables/pickerController.cpp \
    rigs/skeleton.cpp \
    drawables/drawableSkeleton.cpp \
    geom/transform.cpp \
    common/types.cpp \
    operators/cageReverser.cpp \
    operators/cageUpdater.cpp \
    operators/skeletonUpdater.cpp \
    skinning/weights.cpp \
    skinning/skeletonSkinning.cpp \
    skinning/linearBlendSkinning.cpp \
    skinning/meanValueCoordinates.cpp \
    skinning/cageSkinning.cpp \
    GUI/clickConverter.cpp \
    common/cageOperations.cpp \
    common/skeletonOperations.cpp \
    GUI/cagePanel.cpp \
    GUI/skeletonPanel.cpp \
    common/exportFiles.cpp \
    geom/trimesh.cpp \
    rigs/cage.cpp \
    rigs/character.cpp \
    drawables/drawableCharacter.cpp \
    GUI/characterPanel.cpp \
    common/toolsOperation.cpp \
    GUI/toolsPanel.cpp \
    common/characterOperations.cpp \
    math/leastSquareSolver.cpp \
    skinning/dualQuaternionSkinning.cpp \
    skinning/noCageSkinning.cpp \
    common/animatorOperations.cpp \
    GUI/restPoseCanvas.cpp \
    GUI/rigPanel.cpp \
    common/rigOperations.cpp \
    animation/asyncAnimator.cpp \
    GUI/asyncAnimatorPanel.cpp \
    skinning/corSkinning.cpp \
    environment.cpp \
    skinning/sparseWeights.cpp

FORMS *= \
    GUI/mainWindow.ui \
    GUI/cagePanel.ui \
    GUI/skeletonPanel.ui \
    GUI/characterPanel.ui \
    GUI/toolsPanel.ui \
    GUI/debugDialog.ui \
    GUI/rigPanel.ui \
    GUI/asyncAnimatorPanel.ui

RESOURCES += \
    icons.qrc
