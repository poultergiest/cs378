#include "CTimer.h"
using namespace std;
//---------------------- default constructor ------------------------------
//
//-------------------------------------------------------------------------





CTimer::CTimer() : m_FPS(0), m_TimeElapsed(0.0f)
				  
{
	//how many ticks per sec do we get
	//QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	timespec res;
	clock_getres(CLOCK_REALTIME, &res);
	m_PerfCountFreq = res.tv_nsec*1000000;
	m_TimeScale = 1.0f/(m_PerfCountFreq);
}

//---------------------- constructor -------------------------------------
//
//	use to specify FPS
//
//-------------------------------------------------------------------------

CTimer::CTimer(float fps): m_FPS(fps), m_TimeElapsed(0.0f)
{

	//how many ticks per sec do we get
	//QueryPerformanceFrequency( (LARGE_INTEGER*) &m_PerfCountFreq);
	timespec res;
	clock_getres(CLOCK_REALTIME, &res);
	m_PerfCountFreq = res.tv_nsec*1000000;
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
	timespec temp;
	clock_gettime(CLOCK_REALTIME, &temp);
	m_LastTime = temp.tv_sec*1000000 + temp.tv_nsec;
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

	cout << "HERE" << endl;
	if (!m_FPS)
  {
    //MessageBox(NULL, "No FPS set in timer", "Doh!", 0);
  	assert(false);
    return false;
  }
  
  //QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
  timespec temp;
  clock_gettime(CLOCK_REALTIME, &temp);
  m_CurrentTime = temp.tv_sec*1000000 + temp.tv_nsec;
  	cout << "m_CurrentTime = " << m_CurrentTime << endl;
  	cout << "m_NextTime = " << m_NextTime << endl;

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
double CTimer::TimeElapsed()
{
	//QueryPerformanceCounter( (LARGE_INTEGER*) &m_CurrentTime);
	timespec temp;
  	clock_gettime(CLOCK_REALTIME, &temp);
  	m_CurrentTime = temp.tv_sec*1000000 + temp.tv_nsec;


	m_TimeElapsed	= (m_CurrentTime - m_LastTime) * m_TimeScale;
	
	m_LastTime		= m_CurrentTime;

	return m_TimeElapsed;
		
}


/*int main() {
	CTimer x;
	x.Start();
	for (int i = 0; i < 30; ++i)
	{
		sleep(1);
	}
	cout << x.TimeElapsed();
	return 0;
}*/