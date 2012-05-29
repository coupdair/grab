#ifndef GRAB_FACTORY
#define GRAB_FACTORY

#include "grab.h"

class Cgrab_factory
{
public:
  //! class name for debug only
#if cimg_debug>1
  std::string class_name;
#endif
  //! constructor
  /**
   *
  **/
  Cgrab_factory()
  {
#if cimg_debug>1
    class_name="Cgrab_factory";
#endif
  }//constructor

  //! create a grad device of a specific \c type
  /** 
   *
   * @param[in] type: type of port (e.g. type==1 <=> \c Cserial_system type)
   *
   * @return 
   */
  Cgrab* create(int type)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"("<<type<<")\n"<<std::flush;
#endif
   switch(type)
    {
      case 0: return new Cgrab;        break;
      case 1: return new Cgrab_WGet;   break;
//      case 2: return new Cgrab_OpenCV; break;
//      case 3: return new Cgrab_RTSP;   break;
      default:
      {
        std::cerr<<class_name<<"::"<<__func__<<": error: serial type="<<type<<" is unknown\n"<<std::flush;
        return NULL; break;
      }
    }
  }//create
  //! create a grad device of a specific \c type
  /** 
   *
   * @param[in] type: type of port (e.g. type=="serial_system" <=> \c Cserial_system type)
   *
   * @return 
   */
  Cgrab* create(std::string type_name)
  {
#if cimg_debug>1
std::cerr<<class_name<<"::"<<__func__<<"(\""<<type_name<<"\")\n"<<std::flush;
#endif
    if(type_name=="grab_image_file") return create(0);
    else if(type_name=="grab_WGet") return create(1);
//    else if(type_name=="grab_OpenCV") return create(2);
//    else if(type_name=="grab_RTSP") return create(3);
    else
    {
      std::cerr<<class_name<<"::"<<__func__<<": error: grab type="<<type_name<<" not handled.\n"<<std::flush;
      return NULL;
    }
  }//create
//  Cserial* create(CNetCDFParameter &fp)



};//Cgrab_factory class

#endif //GRAB_FACTORY

