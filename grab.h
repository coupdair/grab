#ifndef GRAB
#define GRAB

#include <sstream>
//CImg Library with additional image formats
#if version_cimg < 130
  #define cimg_display_type  cimg_display
  #include "../CImg.Tool/useCImg.h"
#else
  #include "../CImg/CImg.h"
#endif
#ifdef cimg_use_netcdf
#include "../CImg.Tool/CImg_NetCDF.h"
#endif

//serial needed for AandDEE, only.
#include "../rs232/serial_factory.h"

class Cgrab
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif
  //! class (or library) version for information only
  std::string class_version;
  //! specify if class is grabing or loading image
  bool grabed_image;
  //! temporary image path (e.g. "bimg.jpg" for Elphel camera or "/media/data/temp/image_%05d.imx" for FlowMaster camera)
  std::string temporary_image_path;
  //! temporary image index
  int temporary_image_index;
//! temporary image name from temporary_image_path and temporary_image_index
std::string temporary_image_name;
  //! image sequence i.e. several image record (e.g. for same scene statistics)
  int sequence_number;
  int sequence_index;
  virtual bool sequence_initialisation(int size)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<size<<")\n"<<std::flush;
#endif
    sequence_number=size;
    sequence_index=-1;
    return true;
  }//sequence_initialisation

private:
  //! grab type: wget or AandDEEserial or OpenCV or rtsp
//! \todo implement v wget or . AandDEEserial or _ OpenCV or _ rtsp
  std::string device_path;
public:
  //! constructor
  /**
   *
  **/
  Cgrab()
  {
#if cimg_debug>1
    class_name="Cgrab";
#endif
    class_version=GRAB_VERSION;
    ///grab/load image flag
    grabed_image=true;//generally, class is a camera (i.e. grab)
  }//constructor

  //! Open grab device
  /** 
   *
   * @param[in] device_path_name: path of camera (e.g. 192.168.0.9)
   *
   * @return 
   */
  virtual bool open(const std::string& device_path_name)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"(\""<<device_path_name<<"\")\n"<<std::flush;
#endif
    ///check \c device_path validity
    if(device_path_name.empty()) return false;
    device_path=device_path_name;
    ///set temporary image index
//! \todo [low] may start at a different number (e.g. setting 0 here, will start at 1) for image file name.
    temporary_image_index=0;
    sequence_number=-1;
    sequence_index=-1;
    ///grab/load image flag
    //grabed_image=false;//for load_from_file class
    return true;
  }//open

  //! convert value to string (might be fixed size with specific character fill)
  /**
   * format value to string, string size can be fixed and filled with a specific character
  **/
  template<typename T> std::string valueToString(const T& value,int size=-1,char c='0')
  {
    std::ostringstream stream;
    if(size>0)
    {
      stream.width(size);
      stream.fill(c);
    }
    stream << value;
    return stream.str();
  }//valueToString

  //! set numbered image file name from file path format
  /**
   * 
   * \param [out] file_name: image file name with numbers from i  (e.g. ./img_i000.png)
   * \param [in] file_path_format: format for the image file name (e.g. ./img_i%03d.png)
   * \param [in] i: dimension index for current file name (e.g. 0)
  **/
  bool image_file_name(std::string &file_name,const std::string &file_path_format, int i)
  {
    char fileAsCA[512];
    std::sprintf((char*)fileAsCA/*.c_str()*/,file_path_format.c_str(),i);//e.g. file_path_format="./img_i%03d.png"
    file_name=fileAsCA;
    return true;
  }//image_file_name

  //!check image folder
  bool check_image_folder(const std:: string &image_path)
  {
    std::string path;path.reserve(image_path.size());
#if version_cimg < 130
    cimg_library::cimg::filename_path(image_path.c_str(),(char*)path.c_str());
#else
//! \bug for CImg version 1.4.9 no filename_path function, try to use basename instead !!
    //cimg_library::cimg::basename(image_path.c_str());
std::cerr<<"warning: "<<image_path.c_str()<<" not checked, yet !\n"<<std::flush;
return true;
#endif
    std::string ls="ls "+path;
    int error=std::system(ls.c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to access to folder \""<<path<<"\" deduced from \""<<image_path<<"\" image file name format (i.e. std::system error code="<<error<<").\n";//e.g. ls /media/data/temp
      return false;
    }
    return true;
  }//check_image_folder

  //! grab one image
  /** 
   *
   * @param[out] image = image of grabing content
   * @param[in;option] image_path = file name of grabed image (might be saved depending of object type)
   * \note on the contrary in this class, image is loaded from the \c image_path file name.
   *
   * @return 
   */
//  template<typename T>
//  virtual bool grab(cimg_library::CImg<T> &image,const std::string &image_path)
  virtual bool grab(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"(CImg<"<<image.pixel_type()<<">,\""<<image_path<<"\")\n"<<std::flush;
#endif
    ///load image in CImg
    image.load(image_path.c_str());
    ///grab/load image flag
    grabed_image=false;
    return true;
  }//grab
/**/
  //! reference of save image
  /** 
   * image save reference function for derived classes.
   * @param[in] image = image to save
   * @param[in] image_path = file name of image to save
   * \note especially in this class, this function should never be used, but it might be used in derived classes.
   *
   * @return 
   */
  virtual bool save_image(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" image saving.\n"<<std::flush;
#endif
    image.save(image_path.c_str());//might throw exception on error
    return true;
  }//save_image
  //! save image
  /** 
   * save image.
   * @param[in] image = image to save
   * @param[in] image_path = file name of image to save
   * \note especially in this class, image is not saved as it fakes a saving (prior to this call it might have loaded the correponding file).
   *
   * @return 
   */
  virtual bool save(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" empty (i.e. fake image saving)\n"<<std::flush;
#endif
    //this->save_image(image_path.c_str());
    return true;
  }//save

  //! Close grab
  /** 
   *
   * @return 
   */
  virtual bool close()
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" empty\n"<<std::flush;
#endif
    return true;
  }//close

};//Cgrab

class Cgrab_WGet: public Cgrab
{
private:
  //! wget command line
  std::string device_path_wget;
public:
  //! constructor
  /**
   *
  **/
  Cgrab_WGet()
  {
    ///call parent open function
    Cgrab();
    ///set \c class_name
#if cimg_debug>1
    class_name="Cgrab_WGet";
#endif
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
    ///call parent open function
    Cgrab::open(device_path_name);
    ///wget
    device_path_wget="wget -e \"background=off\" http://"+device_path_name+":8081/bimg";
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"/device_path_wget=\""<<device_path_wget<<"\"\n"<<std::flush;
#endif
    ///check temporary image path format
#if version_cimg < 130
    const char *ext = cimg_library::cimg::filename_split(temporary_image_path.c_str());//,body);
#else
    const char *ext = cimg_library::cimg::split_filename(temporary_image_path.c_str());//,body);
#endif
    if(cimg_library::cimg::strncasecmp(ext,"jpg",3))
    {
//! \todo [low] replace bad extention ?!
std::cerr<<"error: bad file name extention (should be .JPG, but \""<<temporary_image_path<<"\" provided)\n";
      return false;
    }
    ///check temporary image folder
    if(!check_image_folder(temporary_image_path)) return false;
    ///check device validity
//! \todo [low] use temporary image file name (see \c temporary_image_path)
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
    return true;
  }//open
  
  //! grab one image
  /** 
   *
   * @param[in] message= string to send to serial port  
   *
   * @return 
   */
//  template<typename T>
//  bool grab(cimg_library::CImg<T> &image,const std::string &image_path)
  bool grab(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<": use system command execution (i.e. std::system() )\n"<<std::flush;
#endif
    ///file name
    std::string file;
#if version_cimg < 130
    const char *ext = cimg_library::cimg::filename_split(image_path.c_str());
#else
    const char *ext = cimg_library::cimg::split_filename(image_path.c_str());
#endif
    if(!cimg_library::cimg::strncasecmp(ext,"jpg",3))
    {//save native image (.JPG)
      file=image_path;//e.g. .JPG
      temporary_image_path.clear();//! empty \c temporary_image_path as it will not be used
    }
    else
    {//save native image (.JPG) as temporary image
      image_file_name(file,temporary_image_path,0);//extention JPG already checked in \c open
    }
    ///get image
    int error=std::system(device_path_wget.c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to grab image from "<<device_path_wget<<" (i.e. std::system error code="<<error<<").\n";//e.g. wget http://192.168.0.9:123/bimg
      return false;
    }
    ///move image
    error=std::system(std::string("mv bimg "+file).c_str());
    if(error!=0)
    {
      std::cerr<<"error: Unable to move image from bimg to "<<file<<" (i.e. std::system error code="<<error<<").\n";//e.g. mv bimg ...
      return false;
    }
    std::cerr<<"information: image saved in \""<<file<<"\" file.\n";
    ///load image in CImg
    image.load(file.c_str());
    return true;
  }//grab

  //! save image
  /** 
   * save image.
   * @param[in] image = image to save
   * @param[in] image_path = file name of image to save
   *
   * @return 
   */
  virtual bool save(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" image saving.\n"<<std::flush;
#endif
    this->save_image(image,image_path);
    return true;
  }//save

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
    ///call parent close function
    return Cgrab::close();
  }//close

};//Cgrab_WGet class

class Cgrab_AandDEE_serial: public Cgrab
{
//private:
public:
  //! AandDEE serial device path (e.g. /dev/ttyUSB0)
  //std::string device_path;//of Cgrab class
  //! communication with AandDEE as serial device (e.g. serial named /dev/ttyUSB0; \see device_path )
//! \todo [medium] _ communicate with AandDEE.
  Cserial* pComAandDEE;
  //! temporary image path (e.g. /media/data/temp/image_%06d.imx filled with an endless loop by rsync , scp, ...)
  //std::string temporary_image_path;//of Cgrab class
  //! temporary image index
  //int temporary_image_index;//of Cgrab class
public:
  //! constructor
  /**
   *
  **/
  Cgrab_AandDEE_serial()
  {
    ///call parent open function
    Cgrab();
    ///set \c class_name
#if cimg_debug>1
    class_name="Cgrab_AandDEE_serial";
#endif
  }//constructor

  //! Open grab device
  /** 
   *
   * @param[in] device_path_name: path of AandDEE serial as commander for a TTL camera (e.g. /dev/ttyUSB0)
   *
   * @return 
   */
  bool open(const std::string& device_path_name)
  {
    ///call parent open function
    Cgrab::open(device_path_name);
    ///check device validity
//! \todo [high] . open AandDEE serial
    ////serial object
    std::string SerialType="serial_system";//"serial_termios";
    Cserial_factory serial_factory;
    pComAandDEE=serial_factory.create(SerialType);
    ////open
    if(!pComAandDEE->opens(device_path_name)) return false;
//! \todo [medium] check AandDEE serial
    ///check temporary image folder
    check_image_folder(temporary_image_path);
    ///print availability
    std::cerr<<"device is available."<<std::endl;
    return true;
  }//open

  //! init a sequence record
  /** 
   *
   * @param[in] size = number of images to record in the sequence
   *
   * @return 
   */
  bool sequence_initialisation(int size)
  {
    Cgrab::sequence_initialisation(size);
    pComAandDEE->writes("grab");//actually, reset AandDEE !!
    cimg_library::cimg::wait(2000);
    temporary_image_index++;//DaVis and AandDEE reset
    return true;
  }//sequence_initialisation

  //! grab one image
  /** 
   *
   * @param[in] message= string to send to serial port  
   *
   * @return 
   */
//  template<typename T>
//  bool grab(cimg_library::CImg<T> &image,const std::string &image_path)
  bool grab(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<": use load image (e.g. image_12345.IMX )\n"<<std::flush;
#endif
    ///increment temporary image index
    temporary_image_index++;
    ///grab image
//! \todo [high] v single   grab image with AandDEE (i.e. serial call, no reset prior to the following one)
    if(sequence_number==-1)
    {
      pComAandDEE->writes("grab");//actually, reset AandDEE !!
      cimg_library::cimg::wait(2000);
    }//single image
    else
    {
      //DO NOT forget to call sequence_initialisation prior to the sequence loop.
      sequence_index++;
    }//image sequence
    ///set temporary image file name
    std::string file;
    image_file_name(file,temporary_image_path,temporary_image_index);
temporary_image_name=file;
    ///check/wait for image file (e.g. using rsync from a remote computer)
//! \todo [medium] set \c try_nb and \c wait_time as class members.
    int try_index=0,try_nb=12345,wait_time=500;
    std::string ls="ls "+file;
    while(std::system(ls.c_str())!=0)
    {
      if(++try_index>try_nb) break;//return false;
      std::cerr<<"information: wait for \""<<file<<"\" image file (try index="<<try_index<<"/"<<try_nb<<").\r"<<std::flush;
      cimg_library::cimg::wait(wait_time);
    }
    //just wait for finishing transfert
    cimg_library::cimg::wait(wait_time);
    ///load image in CImg
    image.load(file.c_str());//e.g. IMX
    return true;
  }//grab

  //! save image
  /** 
   * save image.
   * @param[in] image = image to save
   * @param[in] image_path = file name of image to save
   *
   * @return 
   */
  virtual bool save(cimg_library::CImg<int> &image,const std::string &image_path)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<" image saving.\n"<<std::flush;
#endif
    this->save_image(image,image_path);
    return true;
  }//save

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
    ///call parent close function
    return Cgrab::close();
  }//close

};//Cgrab_AandDEE_serial class

#endif //GRAB

