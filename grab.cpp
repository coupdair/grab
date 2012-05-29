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
 *  This is the reference documentation of <a href="http://www.meol.cnrs.fr/">serial</a>, from the <a href="http://www.univ-lille1.fr/lml/">LML</a>.\n\n
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

//! \todo serial_put doxygen user page

//standard library
#include <iostream>
//CImg Library
#include "../CImg/CImg.h"
//grab library
#include "grab.h"

int main(int argc, char *argv[])
{ 
//commmand line options
 ///usage
  cimg_usage(std::string("grab program of the Laboratory of Mechanics in Lille (LML) is intended to image acquisition from camera device, \
it uses different GNU libraries (see --info option)\n\n \
usage: ./grab -h -I\n \
       ./grab -n 10 --device-type Elphel\n \
version: "+std::string(VERSION)+"\n compilation date: " \
            ).c_str());//cimg_usage
  ///information and help
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help;//same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option
  ///device
//  const std::string DeviceType=cimg_option("--device-type","Elphel","type of grab device (e.g. ArduinoTTL or Elphel_wget or Elphel_OpenCV or Elphel_rtsp).");
  const std::string DevicePath=cimg_option("--device-path","192.168.0.9","path of grab device.");
  ///image
//  const int ImageNumber=cimg_option("-n",10,"number of images to acquire.");
  const std::string ImagePath=cimg_option("-o","image.jpg","path for image(s).");
  ///stop if help requested
  if(show_help) {/*print_help(std::cerr);*/return 0;}
//grab device object
  Cgrab_wget grab;
//open
  if(!grab.open(DevicePath/*,DeviceType*/)) return 1;
//get
  cimg_library::CImg<int> image;
  if(!grab.grab(image,ImagePath)) return 1;
  image.channel(0);//set to grey level, only
//display 2D image
  image.display(ImagePath.c_str());
//display 1D profile on maximum along x
  {
  cimg_library::CImg<float> stat=image.get_stats();
  int ymax=stat[9];
//stat.print("stat");
//std::cerr<<"ymax="<<ymax<<".\n"<<std::flush;
  cimg_library::CImg<int> profile;profile=image.get_crop(0,ymax,image.width()-1,ymax);
  profile.display_graph(std::string(ImagePath+" profile @ ymax").c_str());
  }
//close
  grab.close();
  return 0;
}//main

