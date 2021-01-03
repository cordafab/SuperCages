QT += core gui opengl xml widgets

TARGET   = Supercages
TEMPLATE = app

CONFIG += c++14

##LIBS DIRECTORY

macx:{
    #On MacOS i suggest the use of Homebrew

    #MacOS Intel 
    equals(QMAKE_HOST.arch, "x86_64") {
        LIBSPATH = /usr/local/opt    
    }

    #MacOS ARM
    equals(QMAKE_HOST.arch, "arm64") { 
        LIBSPATH = /opt/homebrew/opt 
    }
}




## Eigen

unix:!macx{

    #apt install libeigen3-dev

    INCLUDEPATH += /usr/include/eigen3
    DEFINES += EIGEN_AVAILABLE
}

macx:{

   #brew install eigen

   INCLUDEPATH += $${LIBSPATH}/eigen/include/eigen3
   DEFINES += EIGEN_AVAILABLE
}




## glm

unix:!macx{
 
  # apt-get install libglm-dev

}

macx:{
 
   #brew install glm

    INCLUDEPATH += $${LIBSPATH}/glm/include
}




## gsl

unix:!macx{

   # apt-get install libgsl-dev

   LIBS+= -lgsl
   LIBS+= -lgslcblas
}

macx:{

    #brew install gsl

    INCLUDEPATH += $${LIBSPATH}/gsl/include
    LIBS += -L'$${LIBSPATH}/gsl/lib' -lgsl -lgslcblas
}




## libQGLViewer

unix:!macx{

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

macx:{

    # Follow the official install instructions provided
    # here http://libqglviewer.com/installUnix.html#mac
    # and edit the INCLUDEPATH and LIBS path accordingly
    # (In our build, we compiling it using qmake+make and placing
    # "QGLViewer.framework" in "~/Library/Frameworks")

    INCLUDEPATH += $$(HOME)/Library/Frameworks/QGLViewer.framework/Headers
    LIBS += -F$$(HOME)/Library/Frameworks -framework QGLViewer
    #DEFINES += CUSTOMSNAPSHOTQGL
}




## OpenMP

#unix:!macx{
#   QMAKE_CXXFLAGS += -fopenmp
#   QMAKE_LFLAGS +=  -fopenmp
#   QMAKE_CFLAGS_RELEASE += -fopenmp
#   LIBS += -lgomp -lpthread
#}

#macx{
#   QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
#   INCLUDEPATH +='/libs/include/libomp/libomp'
#   LIBS += -L'/libs/lib/libomp/libomp' -lomp
#}




## Project files

HEADERS = \
    GUI/glCanvas.h \
    GUI/mainWindow.h \
    mesh/trimesh.h \
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
    geom/dualQuaternion.h \
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
    geom/quaternion.h \
    geom/ray.h \
    skinning/dualQuaternionSkinning.h \
    skinning/noCageSkinning.h \
    common/animatorOperations.h \
    GUI/restPoseCanvas.h \
    GUI/rigPanel.h \
    common/rigOperations.h \
    animation/asyncAnimator.h \
    GUI/asyncAnimatorPanel.h \
    skinning/corSkinning.h \
    environment.h \
    skinning/sparseWeights.h \
    _external/JM/MEC.h \
    _external/JM/point3.h \
    _external/JM/MVCoordinates3D.h \
    _external/cinolib/textures/textures.h \
    _external/cinolib/textures/texture_hsv.h \
    _external/cinolib/textures/texture_hsv_w_isolines.h \
    _external/cinolib/textures/texture_parula.h \
    _external/cinolib/textures/texture_parula_w_isolines.h \
    _external/cinolib/serialize_2D.h \
    _external/cinolib/cino_inline.h \
    _external/cinolib/color.h \
    _external/cinolib/pi.h \
    _external/cinolib/ANSI_color_codes.h \
    _external/cinolib/geometry/vec3.h \
    _external/cinolib/ipair.h \
    _external/cinolib/io/read_OBJ.h \
    _external/cinolib/string_utilities.h \
    _external/cinolib/cut_along_seams.h


SOURCES = \
    main.cpp \
    GUI/glCanvas.cpp \
    GUI/mainWindow.cpp \
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
    mesh/trimesh.cpp \
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
    skinning/sparseWeights.cpp \
    _external/cinolib/textures/textures.cpp \
    _external/cinolib/color.cpp \
    _external/cinolib/serialize_2D.cpp \
    _external/cinolib/ipair.cpp \
    _external/cinolib/io/read_OBJ.cpp \
    _external/cinolib/string_utilities.cpp \
    _external/cinolib/cut_along_seams.cpp

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
