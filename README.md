# Real-time Deformation with Coupled Cages and Skeletons
[Corda Fabrizio](http://www.fabriziocorda.com), [Jean-Marc Thiery](https://perso.telecom-paristech.fr/jthiery/), [Marco Livesu](http://pers.ge.imati.cnr.it/livesu/), [Enrico Puppo](https://www.disi.unige.it/person/PuppoE/), [Tamy Boubekeur](https://perso.telecom-paristech.fr/boubek/), [Riccardo Scateni](http://people.unica.it/riccardoscateni/)

[PDF](http://pers.ge.imati.cnr.it/livesu/papers/CTLPBS20/CTLPBS20.pdf) 

Skeleton-based and cage-based deformation techniques represent the two most popular approaches to control real-time deformations of digital shapes and are, to a vast extent, complementary to one another. Despite their complementary roles, high-end modeling packages do not allow for seamless integration of such control structures, thus inducing a considerable burden on the user to maintain them synchronized. In this paper, we propose a framework that seamlessly combines rigging skeletons and deformation cages, granting artists with a real-time deformation system that operates using any smooth combination of the two approaches. By coupling the deformation spaces of cages and skeletons, we access a much larger space, containing poses that are impossible to obtain by acting solely on a skeleton or a cage. Our method is oblivious to the specific techniques used to perform skinning and cage-based deformation, securing it compatible with pre-existing tools. We demonstrate the usefulness of our hybrid approach on a variety of examples. 

## SCA2020 Video Presentation

[![Trailer](https://img.youtube.com/vi/bfwVCidbX1s/0.jpg)](https://www.youtube.com/watch?v=bfwVCidbX1s)

[SCA2020 Trailer](https://www.youtube.com/watch?v=bfwVCidbX1s)

[SCA2020 Full Presentation](https://www.youtube.com/watch?v=HNuNZP_RMN8)

## Installation
SuperCages can be compiled using the *supercages.pro* qmake file placed in the project root.
SuperCages has been tested with: 
 - Intel-based MacOS 11.1
 - Apple-silicon MacOS 11.1
 - Ubuntu

#### MacOS configuration
Install [Qt](https://www.qt.io/download) and [libqglviewer](http://libqglviewer.com/download.html) using the installers provided on the respective websites.
Use [brew](https://brew.sh/) to install *gsl*, *glm*, and *Eigen* using:

    brew install gsl
    brew install glm
    brew install eigen
    
#### Ubuntu configuration
Install Qt using these [instructions](https://wiki.qt.io/Install_Qt_5_on_Ubuntu).
Use *apt* to install *libqglviewer*, its dependencies, *gsl*, *glm* and *Eigen* using:

    apt install libglew-dev
    apt install libqglviewer-dev-qt5
    apt install freeglut3-dev
    apt install libgsl-dev
    apt install libglm-dev
    apt install libeigen3-dev
 
## Example Usage
In the *_DataExamples* folder you can find the **Arm** rig example.

Press the ***Load Rig*** button placed in the left side of the SuperCages UI to open the file selection dialog window. 

Choose the ***arm.obj*** file to load the skin geometry. SuperCages will load automatically the *arm.ska* and *arm.skw* files that contain the skeleton topology and weights.  

Then, a second dialog window will show up: choose the ***arm_cage.obj*** to load the cage geometry, the *arm_cage_cageWeights.txt* cage weights file, and the *arm_cage.maxvol* maxvol data.

Press the ***Load Skel Anim*** button and select ***arm_skelAnim.txt*** to load the provided skeleton animation. 

Press the ***Load Cage Anim*** button and select ***arm_cageAnim.txt*** to load the provided skeleton animation. 

Click on the main canvas and press the ***P*** keyboard button to play the loaded animations.

You can obtain additional information about the file composition opening each file in a text editor.

## Acknowldegment
If you use SuperCages in your academic projects, please cite us using the following 
BibTeX entry:

```bibtex  
@article{doi:10.1111/cgf.13900,
  author  = {Corda, Fabrizio and Thiery, Jean Marc and Livesu, Marco and Puppo, Enrico and Boubekeur, Tamy and Scateni, Riccardo},
  title   = {Real-Time Deformation with Coupled Cages and Skeletons},
  journal = {Computer Graphics Forum},
  volume  = {39},
  number  = {6},
  pages   = {19-32},
  doi     = {10.1111/cgf.13900},
  url     = {https://onlinelibrary.wiley.com/doi/abs/10.1111/cgf.13900},
  eprint  = {https://onlinelibrary.wiley.com/doi/pdf/10.1111/cgf.13900},
  year    = {2020}}
  ```