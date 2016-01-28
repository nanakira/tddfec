#ifndef D_tddfecBuildTime_H
#define D_tddfecBuildTime_H

///////////////////////////////////////////////////////////////////////////////
//
//  tddfecBuildTime is responsible for recording and reporting when
//  this project library was built
//
///////////////////////////////////////////////////////////////////////////////

class tddfecBuildTime
  {
  public:
    explicit tddfecBuildTime();
    virtual ~tddfecBuildTime();
    
    const char* GetDateTime();

  private:
      
    const char* dateTime;

    tddfecBuildTime(const tddfecBuildTime&);
    tddfecBuildTime& operator=(const tddfecBuildTime&);

  };

#endif  // D_tddfecBuildTime_H
