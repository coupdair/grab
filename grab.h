#ifndef GRAB
#define GRAB

#include "../CImg/CImg.h"

/*
class Cgrab //base class
{
virtualS
  //! Open grab device
  /** 
   *
   * @param[in] device_path_name: path of camera (e.g. 192.168.0.9)
   *
   * @return 
   * /
  bool open(const std::string& device_path_name)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<device_path_name<<")\n"<<std::flush;
#endif
    ///check \c device_path validity
    if(device_path_name.empty()) return false;
    device_path=device_path_name;
    return true;
  }//open

  //! grab one image
  /** 
   *
   * @param[in] message= string to send to serial port  
   *
   * @return 
   * /
  template<typename T>
  bool grab(cimg_library::CImg<T> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"(CImg<"<<image.pixel_type()<<">,"<<image_path<<")\n"<<std::flush;
#endif
    ///load image in CImg
    image.load(image_path.c_str());
    return true;
  }//grab

}//Cgrab
*/

class Cgrab
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif
  //! class (or library) version for information only
  std::string class_version;
private:
  //! grab type: wget or OpenCV or rtsp
//! \todo implement . wget or _ OpenCV or _ rtsp
  std::string device_path;
  std::string device_path_wget;
public:
  //! constructor
  /**
   *
  **/
  Cgrab()
  {
#if cimg_debug>1
    class_name="Cgrab_wget";
#endif
    class_version=VERSION;
  }//constructor

  //! Open grab device
  /** 
   *
   * @param[in] device_path_name: path of camera (e.g. 192.168.0.9)
   *
   * @return 
   */
  bool open(const std::string& device_path_name)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<device_path_name<<")\n"<<std::flush;
#endif
    ///check \c device_path validity
    if(device_path_name.empty()) return false;
    device_path=device_path_name;
    ///wget
    device_path_wget="wget http://"+device_path_name+":8081/bimg";
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"/device_path_wget=\""<<device_path_wget<<"\"\n"<<std::flush;
#endif
/**/
    ///check device validity
    int error=std::system(device_path_wget.c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to grab image from \""<<device_path_wget<<"\" (i.e. std::system error code="<<error<<").\n";//e.g. wget http://192.168.0.9:8081/bimg

      return false;
    }
    ///check device validity
    error=std::system("ls -lah bimg; rm bimg");
    if(error!=0)
    {
      std::cerr<<"error: Unable to list or remove image from \""<<device_path_wget<<"\" (i.e. bimg; std::system error code="<<error<<").\n";//e.g. rm bimg
      return false;
    }
    ///print availability
    std::cerr<<"device is available."<<std::endl;
/**/
    return true;
  }//open
  
  //! grab one image
  /** 
   *
   * @param[in] message= string to send to serial port  
   *
   * @return 
   */
  template<typename T>
  bool grab(cimg_library::CImg<T> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<": use system command execution (i.e. std::system() )\n"<<std::flush;
#endif
/**/
    ///get image
    int error=std::system(device_path_wget.c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to grab image from "<<device_path_wget<<" (i.e. std::system error code="<<error<<").\n";//e.g. wget http://192.168.0.9:123/bimg
      return false;
    }
    ///move image
    error=std::system(std::string("mv bimg "+image_path).c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to move image from bimg to "<<image_path<<" (i.e. std::system error code="<<error<<").\n";//e.g. mv bimg ...
      return false;
    }
/**/
    ///load image in CImg
    image.load(image_path.c_str());
    return true;
  }//grab

  //! Close grab
  /** 
   *
   * @return 
   */
  bool close()
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" empty\n"<<std::flush;
#endif
    return true;
  }//close

};//Cgrab class

#endif //GRAB

