// -*-Mode: C++;-*-
//
//  Event manager
//
//  $Id: EventManager.hpp,v 1.3 2010/10/24 14:06:18 rishitani Exp $

#include "qlib.hpp"

#include "SingletonBase.hpp"
#include "LTypes.hpp"
#include "TimerEvent.hpp"

#ifndef QLIB_EVENT_MANAGER_HPP_
#define QLIB_EVENT_MANAGER_HPP_

namespace qlib {

  class LEvent;

  /// Base class of event casters implementing lock
  class QLIB_API LEventCasterBase
  {
  public:

    LEventCasterBase() : m_fLock(false) {}

    virtual void fireEvent(LEvent *pEvent) =0;

    /////////
    // Lock

    void lock() const {
      m_fLock = true;
    }

    void unlock() const {
      m_fLock = false;
    }

    bool isLocked() const {
      return m_fLock;
    }

  protected:
    mutable bool m_fLock;

  };

  /// Automatic event cast lock object
  class AutoEventCastLock
  {
  private:
    const LEventCasterBase *m_pCaster;

  public:
    AutoEventCastLock(const LEventCasterBase *pCaster)
         : m_pCaster(pCaster)
    {
      m_pCaster->lock();
    }

    ~AutoEventCastLock()
    {
      m_pCaster->unlock();
    }
  };

  ////////////////////////////////////////////////////////////////////

  /// Timer implementation interface
  class QLIB_API TimerImpl
  {
  public:
    TimerImpl() {}
    virtual ~TimerImpl();
    virtual time_value getCurrentTime() =0;
    virtual void start(qlib::time_value period) =0;
    virtual void stop() =0;
  };

  /// Timer entry data structure
  struct TimerTuple {
    time_value start;
    time_value end;
    TimerListener *pobj;
  };

  //inline bool operator < (const TimerTuple &a, const TimerTuple &b) {
  //return a.when<b.when;
  //}
  
  struct EMThreadImpl;

  ///
  /// Event/Timer manager
  ///
  class QLIB_API EventManager : public SingletonBase<EventManager>
  {
  private:
    EMThreadImpl *m_pthr;
    /*
#ifdef HAVE_BOOST_THREAD
    boost::thread::id m_mainthr;
    mutable boost::mutex m_mu;
#endif
    */

    /// Event entry data structure
    typedef std::pair<LEvent *, LEventCasterBase *> tuple_t;

    /// Pending event list
    std::list<tuple_t> m_pending;

    ///////////////////////

    /// Timer implementation
    TimerImpl *m_pImpl;

    /// Timer object table type
    typedef std::list<TimerTuple> TimerQueue;

    /// Timer object table
    TimerQueue m_timerq;

  public:
    EventManager();
    virtual ~EventManager();

    /////////////////////////////

    bool isMainThread() const;

    /// Delegate event notification to the main thread
    ///  Event object must be copy-safe.
    void delegateEventFire(const LEvent *pEvent, LEventCasterBase *pCaster);

    /// Main thread polling method
    ///  This method should be called periodically, when the main thread is idle.
    void messageLoop();

  public:
    
    void initTimer(TimerImpl *pimpl);
    void finiTimer();

    void setTimer(TimerListener *pobj, time_value dur_msec);
    void removeTimer(TimerListener *pobj);

    void checkTimerQueue();

    inline time_value getCurrentTime() {
      return m_pImpl->getCurrentTime();
    }

    inline static time_value sGetCurrentTime() {
      return getInstance()->getCurrentTime();
    }
    

  };

}

#endif


