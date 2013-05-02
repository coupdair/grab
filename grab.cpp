//grab
/*-----------------------------------------------------------------------
  File        : grab.cpp
  Description : frame grabber program on camera device of the Laboratory of Mechanics in Lille (LML)
  Authors     : Sebastien COUDERT
-----------------------------------------------------------------------*/

/*Additional documentation for the generation of the reference page (using doxygen)*/
/** \mainpage
 *
 *  index:
 *  \li \ref sectionCommandLine
 *  \li \ref sectionGrabDocumentation
 *  \li \ref sectionDoxygenSyntax
 *
 *  \section sectionCommandLine command line options
 *
 *  \verbinclude "grab.help.output"
 *  
 *  \section sectionGrabDocumentation documentation outline
 *  This is the reference documentation of <a href="http://www.meol.cnrs.fr/">grab</a>, from the <a href="http://www.univ-lille1.fr/lml/">LML</a>.\n\n
 *  frame grabber software. The main function is in <a href="grab_8cpp.html">grab.cpp</a> source file.\n\n
 *  This documentation has been automatically generated from the sources, 
 *  using the tool <a href="http://www.doxygen.org">doxygen</a>. It should be readed as HTML, LaTex and man page.\n
 *  It contains both
 *  \li a detailed description of all classes and functions
 *  \li TODO: a user guide (cf. \ref pages.html "related pages")
 *
 *  that as been documented in the sources.
 *
 *  \par Additional needed libraries:
 *
 *  \li <a href="http://cimg.sourceforge.net">the CImg Library</a> (v1.?.?) for command line options only.
 *
 *  \section sectionDoxygenSyntax make documentation using Doxygen syntax
 *  Each function in the source code should be commented using \b doxygen \b syntax in the same file.
 *  The documentation need to be written before the function.
 *  The basic syntax is presented in this part.
 *  \verbinclude "doxygen.example1.txt"
 *
 *  Two kind of comments are needed for \b declaration and \b explanation \b parts of the function:
 *  Standart documentation should the following (\b sample of code documentation):
 *  \verbinclude "doxygen.example2.txt"
 *
 *  In both declaration and explanation part, \b writting and \b highlithing syntax can be the following:\n\n
 *  \li \c \\code to get\n
 *
 *  \li \c \\n    a new line
 *  \li \c \\li   a list (dot list)
 *
 *  \li \c \\b    bold style
 *  \li \c \\c    code style
 *  \li \c \\e    enhanced style (italic)
 *
 *  For making \b shortcut please use:\n
 *  \li \c \\see to make a shortcut to a related function or variable
 *  \li \c \\link to make a shortcut to a file or a function
 *  \note this keyword needs to be closed using \c \\end*
 *
 *  \li \c \\todo to add a thing to do in the list of <a href="todo.html">ToDo</a> for the whole program
 *
 *  In explanation part, \b paragraph style can be the following:\n
 *  \li \c \\code for an example of the function use
 *  \li \c \\note to add a few notes
 *  \li \c \\attention for SOMETHING NOT FULLY DEFINED YET
 *  \li \c \\warning to give a few warning on the function
 *  \note these keywords need to be closed using \c \\end*
 *
 *  \verbinclude "doxygen.example3.txt"
 *
 *  Many other keywords are defined, so please read the documentation of <a href="http://www.doxygen.org/commands.html">doxygen</a>.
 *
**/

//standard library
#include <iostream>
//grab library (including CImg Library)
#include "grab_factory.h"

int main(int argc, char *argv[])
{ 
//commmand line options
 ///usage
  cimg_usage(std::string("grab program of the Laboratory of Mechanics in Lille (LML) is intended to image acquisition from camera device, \
it uses different GNU libraries (see --info option)\n\n \
usage: ./grab -h -I\n \
       ./grab -n 10 --device-type grab_WGet --device-path 192.168.0.9 -t tmp_image.jpg #ethernet camera such as Elphel\n \
       ./grab -n 10 --device-type grab_AandDEE_serial --device-path /dev/ttyUSB0 #TTL trigger for external camera driver\n \
       ./grab -n 10 --device-type grab_image_file #to do off line run or test\n \
version: "+std::string(GRAB_VERSION)+"\n compilation date: " \
            ).c_str());//cimg_usage
  ///information and help
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help;//same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option
  ///device
  const std::string DeviceType=cimg_option("--device-type","grab_WGet","type of grab device (e.g. grab_image_file or grab_WGet or grab_AandDEE_serial -or Elphel_OpenCV or Elphel_rtsp-).");
  const std::string DevicePath=cimg_option("--device-path","192.168.0.9","path of grab device (e.g. 192.168.0.9 or /dev/ttyUSB0).");
  ///image
  const int ImageNumber=cimg_option("-n",10,"number of images to acquire.");
  const std::string ImagePath=cimg_option("-o","image_%03d.cimg","path for image(s) (e.g. image_%03d.cimg for image_000.cimg).");
  const std::string TemporaryImagePath=cimg_option("-t","tmp_image.jpg","temporary path for image(s) (e.g. image_%05d.imx for image_000001.imx (using LaVision/DaVis) or tmp_image.jpg using Elphel).");
  const bool display=cimg_option("-X",true,"display image and graph (e.g. -X false for no display).");
    ///stop if help requested
  if(show_help) {/*print_help(std::cerr);*/return 0;}
//grab device object
  Cgrab_factory grab_factory;
  Cgrab *pGrab=grab_factory.create(DeviceType);
  pGrab->temporary_image_path=TemporaryImagePath;
//open
  if(!pGrab->open(DevicePath)) return 1;
//get
  cimg_library::CImgList<int> image(2);//index: 0 current, 1 previous
  //int ymax=0;
  //cimg_library::CImg<int> profile;
  //cimg_library::CImg<float> mean;
  //cimg_library::CImg<int> min;
  //cimg_library::CImg<int> max;
  std::string file;file.reserve(ImagePath.size()+64);
//! \todo [low] _ do the same with sequence framework of Cgrab, so have 2 possibilities for grabing a mean image.
  for(int i=0;i<ImageNumber;++i)
  {//do
    pGrab->image_file_name(file,ImagePath,i);
pGrab->temporary_image_index++;//DaVis and AandDEE reset
    //get next image with check (i.e. grab single time image)
    int loop=false;
    do
    {//get next image with check (i.e. grab single time image)
      if(loop) std::cerr<<"\n\nget next image (i.e. reload)\n\n"<<std::flush;
      if(!pGrab->grab(image[0],file.c_str())) return 1;//might use temporary image
      image[0].channel(0);//set to grey level, only
      //check
      if(image[1].is_empty())
      {//first loop
        image[1]=image[0];//copy current to previous
        //loop=false;
      }
      else
      {//check if any pixel value difference
        //cimg_forXY(image[0],x,y) if(image[0](x,y)==image[1](x,y)) loop=true; else loop=false;//loop on entire image
        cimg_for_inXY(image[0],1,1,4,4,x,y) if(image[0](x,y)==image[1](x,y)) loop=true; else loop=false;//loop on 3x3 pixel
      }
    }while(loop);
    //display 2D image
    if(display) image[0].display(file.c_str());
/*
    if(i==0)
    {//search maximum for first image only
      cimg_library::CImg<float> stat=image[0].get_stats();
      ymax=stat[9];
    }//max
    profile=image[0].get_crop(0,ymax,image[0].width()-1,ymax);//.get_line(ymax);
    if(i==0)
    {
      min=max=mean=profile;
    }
    else
    {
      mean+=profile;
      min=profile.get_min(min);
      max=profile.get_max(max);
    }
*/
    if(!pGrab->temporary_image_path.empty()) image[0].save(file.c_str());//use temporary image, so save final record
    //copy current to previous for next loop (i.e. fast swap previous/current)
    image[1].swap(image[0]);
  }//done
/*
  //mean
  mean/=ImageNumber;
//display last 2D image
  image.display(file.c_str());
//display 1D profile on maximum along x
  profile.display_graph(file.c_str());
  mean.display_graph("mean");
  min.display_graph("min");
  max.display_graph("max");
*/
//close
  pGrab->close();
  return 0;
}//main

