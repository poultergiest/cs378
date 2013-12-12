#include "CTimer.h"
using namespace std;
//---------------------- default constructor ------------------------------
//
//-------------------------------------------------------------------------





CTimer::CTimer() : m_FPS(0), m_TimeElapsed(0.0f), m_FrameTime(0),
				   m_LastTime(0), m_PerfCountFreq(0)
				  
{
	//how many ticks per sec do we get
	//QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	
	m_PerfCountFreq = 1000000;
	m_TimeScale = 1.0f/(m_PerfCountFreq);
}

//---------------------- constructor -------------------------------------
//
//	use to specify FPS
//
//-------------------------------------------------------------------------

CTimer::CTimer(float fps): m_FPS(fps), m_TimeElapsed(0.0f), m_LastTime(0),
						   m_PerfCountFreq(0)
{

	//how many ticks per sec do we get
	//QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);

	m_PerfCountFreq = 1000000;
	m_TimeScale = 1.0f/m_PerfCountFreq;

	//calculate ticks per frame
	m_FrameTime = (long long)(m_PerfCountFreq / m_FPS);
}


//------------------------Start()-----------------------------------------
//
//	call this immediately prior to game loop. Starts the timer (obviously!)
//
//--------------------------------------------------------------------------
void CTimer::Start()
{
	//get the time
	//QueryPerformanceCounter( (LARGE_INTEGER*) &m_LastTime);
	timespec start;
	clock_gettime(CLOCK_MONOTONIC  , &start);
	m_LastTime = start.tv_sec*1000000 + start.tv_nsec;
	//update time to render next frame
	m_NextTime = m_LastTime + m_FrameTime;

	return;
}

//-------------------------ReadyForNextFrame()-------------------------------
//
//	returns true if it is time to move on to the next frame step. To be used if
//	FPS is set.
//
//----------------------------------------------------------------------------
bool CTimer::ReadyForNextFrame()
{
	if (!m_FPS)
  {
    //MessageBox(NULL, "No FPS set in timer", "Doh!", 0);
  	assert(false);
    return false;
  }
  
  //QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
  timespec ready;
  clock_gettime(CLOCK_MONOTONIC  , &ready);
  m_CurrentTime = ready.tv_sec*1000000 + ready.tv_nsec;
/*  	cout << "m_CurrentTime " << m_CurrentTime << endl;
*/
	if (m_CurrentTime > m_NextTime)
	{	

		m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
		m_LastTime		= m_CurrentTime;

		//update time to render next frame
		m_NextTime = m_CurrentTime + m_FrameTime;

		return true;
	}

	return false;
}

//--------------------------- TimeElapsed --------------------------------
//
//	returns time elapsed since last call to this function. Use in main
//	when calculations are to be based on dt.
//
//-------------------------------------------------------------------------
/*double CTimer::TimeElapsed()
{
	//QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	timespec elapsed;
  	clock_gettime(CLOCK_MONOTONIC  , &elapsed);
  	m_CurrentTime = elapsed.tv_sec*1000000 + elapsed.tv_nsec;


	m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
	
	m_LastTime		= m_CurrentTime;

	return m_TimeElapsed;
		
}*/

/*int main () {
	CTimer x(1);


	return 0;
}*/