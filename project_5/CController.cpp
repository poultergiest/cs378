#include "CController.h"


//these hold the geometry of the sweepers and the mines
const int	 NumSweeperVerts = 16;
const SPoint sweeper[NumSweeperVerts] = {SPoint(-1, -1),
                                         SPoint(-1, 1),
                                         SPoint(-0.5, 1),
                                         SPoint(-0.5, -1),

                                         SPoint(0.5, -1),
                                         SPoint(1, -1),
                                         SPoint(1, 1),
                                         SPoint(0.5, 1),
                                         
                                         SPoint(-0.5, -0.5),
                                         SPoint(0.5, -0.5),

                                         SPoint(-0.5, 0.5),
                                         SPoint(-0.25, 0.5),
                                         SPoint(-0.25, 1.75),
                                         SPoint(0.25, 1.75),
                                         SPoint(0.25, 0.5),
                                         SPoint(0.5, 0.5)};



const int NumMineVerts = 4;
const SPoint mine[NumMineVerts] = {SPoint(-1, -1),
                                   SPoint(-1, 1),
                                   SPoint(1, 1),
                                   SPoint(1, -1)};



//---------------------------------------constructor---------------------
//
//	initilaize the sweepers, their brains and the GA factory
//
//-----------------------------------------------------------------------
CController::CController(): m_NumSweepers(CParams::iNumSweepers), 
					m_pGA(NULL),
					m_bFastRender(false),
					m_iTicks(0),
					m_NumMines(CParams::iNumMines),
					m_iGenerations(0),
					cxClient(CParams::WindowWidth),
					cyClient(CParams::WindowHeight)
{
	//let's create the mine sweepers
	for (int i=0; i<m_NumSweepers; ++i)
	{
		m_vecSweepers.push_back(CMinesweeper());
	}

	//get the total number of weights used in the sweepers
	//NN so we can initialise the GA
	m_NumWeightsInNN = m_vecSweepers[0].GetNumberOfWeights();

	//initialize the Genetic Algorithm class
	m_pGA = new CGenAlg(m_NumSweepers,
                      CParams::dMutationRate,
	                    CParams::dCrossoverRate,
	                    m_NumWeightsInNN);

	//Get the weights from the GA and insert into the sweepers brains
	m_vecThePopulation = m_pGA->GetChromos();
	int i = 0;
	for (i=0; i<m_NumSweepers; i++)
	
		m_vecSweepers[i].PutWeights(m_vecThePopulation[i].vecWeights);

	//initialize mines in random positions within the application window
	for (i=0; i<m_NumMines; ++i)
	{
		m_vecMines.push_back(SVector2D(RandFloat() * cxClient,
                                   RandFloat() * cyClient));
	}

	//create a pen for the graph drawing
	m_BluePen.red = 0;
	m_BluePen.green = 0;
	m_BluePen.blue = 255;
	m_RedPen.red = 255;
	m_RedPen.green = 0;
	m_RedPen.blue = 0;
	m_GreenPen.red = 0;
	m_GreenPen.green = 175;
	m_GreenPen.blue = 0;
	m_BlackPen.red = 0;
	m_BlackPen.green = 0;
	m_BlackPen.blue = 0;

	//fill the vertex buffers
	for (i=0; i<NumSweeperVerts; ++i)
	{
		m_SweeperVB.push_back(sweeper[i]);
	}

	for (i=0; i<NumMineVerts; ++i)
	{
		m_MineVB.push_back(mine[i]);
	}
	m_bFastRender = true;
}


//--------------------------------------destructor-------------------------------------
//
//--------------------------------------------------------------------------------------
CController::~CController()
{
	if(m_pGA)
	{
		delete m_pGA;
	}
}


//---------------------WorldTransform--------------------------------
//
//	sets up the translation matrices for the mines and applies the
//	world transform to each vertex in the vertex buffer passed to this
//	method.
//-------------------------------------------------------------------
void CController::WorldTransform(vector<SPoint> &VBuffer, SVector2D vPos)
{
	//create the world transformation matrix
	C2DMatrix matTransform;

	//scale
	matTransform.Scale(CParams::dMineScale, CParams::dMineScale);

	//translate
	matTransform.Translate(vPos.x, vPos.y);

	//transform the ships vertices
	matTransform.TransformSPoints(VBuffer);
}


//-------------------------------------Update-----------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//	The comments should explain what is going on adequately.
//-------------------------------------------------------------------------
bool CController::Update()
{
	//run the sweepers through CParams::iNumTicks amount of cycles. During
	//this loop each sweepers NN is constantly updated with the appropriate
	//information from its surroundings. The output from the NN is obtained
	//and the sweeper is moved. If it encounters a mine its fitness is
	//updated appropriately,
	if (m_iTicks++ < CParams::iNumTicks)
	{
		for (int i=0; i<m_NumSweepers; ++i)
		{
			//update the NN and position
			if (!m_vecSweepers[i].Update(m_vecMines))
			{
				//error in processing the neural net
				/*MessageBox(m_hwndMain, "Wrong amount of NN inputs!", "Error", MB_OK);*/

				return false;
			}
				
			//see if it's found a mine
			int GrabHit = m_vecSweepers[i].CheckForMine(m_vecMines,
                                                  CParams::dMineScale);

			if (GrabHit >= 0)
			{
				//we have discovered a mine so increase fitness
				m_vecSweepers[i].IncrementFitness();

				//mine found so replace the mine with another at a random 
				//position
				m_vecMines[GrabHit] = SVector2D(RandFloat() * cxClient,
				RandFloat() * cyClient);
			}

			//update the chromos fitness score
			m_vecThePopulation[i].dFitness = m_vecSweepers[i].Fitness();

		}
	}
	//Another generation has been completed.
	//Time to run the GA and update the sweepers with their new NNs
	else
	{
		//update the stats to be used in our stat window
		m_vecAvFitness.push_back(m_pGA->AverageFitness());
		m_vecBestFitness.push_back(m_pGA->BestFitness());

		//increment the generation counter
		++m_iGenerations;

		//reset cycles
		m_iTicks = 0;

		//run the GA to create a new population
		m_vecThePopulation = m_pGA->Epoch(m_vecThePopulation);
			
		//insert the new (hopefully)improved brains back into the sweepers
		//and reset their positions etc
		for (int i=0; i<m_NumSweepers; ++i)
		{
			m_vecSweepers[i].PutWeights(m_vecThePopulation[i].vecWeights);
			m_vecSweepers[i].Reset();
		}
	}

	return true;
}

//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
void CController::Render(struct rgbData data[][WIDTH])
{
	//render the stats
	string s = "Generation:          " + itos(m_iGenerations);
	//TextOut(surface, 5, 0, s.c_str(), s.size());
	drawstring(data, 5, 0, s.c_str(), m_BlackPen);

	//do not render if running at accelerated speed
	if (!m_bFastRender)
	{
		//keep a record of the old pen
		m_OldPen = m_GreenPen;

		//render the mines
		int i = 0;
		for (i=0; i<m_NumMines; ++i)
		{
			//grab the vertices for the mine shape
			vector<SPoint> mineVB = m_MineVB;

			WorldTransform(mineVB, m_vecMines[i]);

			//draw the mines
			//MoveToEx(surface, (int)mineVB[0].x, (int)mineVB[0].y, NULL);
			drawcircle(data, m_vecMines[i].x, m_vecMines[i].y, 2, m_GreenPen);
			// for (int vert=1; vert<mineVB.size(); ++vert)
			// {
			// 	LineTo(surface, (int)mineVB[vert].x, (int)mineVB[vert].y);
			// }

			//LineTo(surface, (int)mineVB[0].x, (int)mineVB[0].y);
		}
       		
		//we want the fittest displayed in red
		//SelectObject(surface, m_RedPen);
		m_OldPen = m_RedPen;

		//render the sweepers
		for (i=0; i<m_NumSweepers; i++)
		{
			if (i == CParams::iNumElite)
			{
				//SelectObject(surface, m_OldPen);
				m_OldPen = m_BluePen;
			}
      
			//grab the sweeper vertices
			vector<SPoint> sweeperVB = m_SweeperVB;

			//transform the vertex buffer
			m_vecSweepers[i].WorldTransform(sweeperVB);

			//draw the sweeper left track
			//MoveToEx(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y, NULL);
			int oldx = (int)sweeperVB[0].x;
			int oldy = (int)sweeperVB[0].y;
			int vert = 1;
			for (vert=1; vert<4; ++vert)
			{
				//LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
				drawline(data, oldx, oldy, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y, m_OldPen);
				oldx = (int)sweeperVB[vert].x;
				oldy = (int)sweeperVB[vert].y;
			}

			//LineTo(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y);
			drawline(data, oldx, oldy, (int)sweeperVB[0].x, (int)sweeperVB[0].y, m_OldPen);

			//draw the sweeper right track
			//MoveToEx(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y, NULL);
			oldx = (int)sweeperVB[4].x;
			oldy = (int)sweeperVB[4].y;

			for (vert=5; vert<8; ++vert)
			{
				//LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
				drawline(data, oldx, oldy, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y, m_OldPen);
				oldx = (int)sweeperVB[vert].x;
				oldy = (int)sweeperVB[vert].y;
			}

			//LineTo(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y);
			drawline(data, oldx, oldy, (int)sweeperVB[4].x, (int)sweeperVB[4].y, m_OldPen);

			//MoveToEx(surface, (int)sweeperVB[8].x, (int)sweeperVB[8].y, NULL);
			oldx = (int)sweeperVB[8].x;
			oldy = (int)sweeperVB[8].y;
			//LineTo(surface, (int)sweeperVB[9].x, (int)sweeperVB[9].y);
			drawline(data, oldx, oldy, (int)sweeperVB[9].x, (int)sweeperVB[9].y, m_OldPen);

			//MoveToEx(surface, (int)sweeperVB[10].x, (int)sweeperVB[10].y, NULL);
			oldx = (int)sweeperVB[10].x;
			oldy = (int)sweeperVB[10].y;
			for (vert=11; vert<16; ++vert)
			{
				//LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
				drawline(data, oldx, oldy, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y, m_OldPen);
				oldx = (int)sweeperVB[vert].x;
				oldy = (int)sweeperVB[vert].y;
			}
		}
		//put the old pen back
		//SelectObject(surface, m_OldPen);

	}//end if
	else
	{
		//PlotStats(data);
	}

}

//--------------------------PlotStats-------------------------------------
//
//  Given a surface to draw on this function displays stats and a crude
//  graph showing best and average fitness
//------------------------------------------------------------------------
void CController::PlotStats(struct rgbData data[][WIDTH])
{
	string s = "Best Fitness:       " + ftos(m_pGA->BestFitness());
	
	//TextOut(surface, 5, 20, s.c_str(), s.size());
	drawstring(data, 5, 20, s.c_str(), m_BlackPen);

	s = "Average Fitness: " + ftos(m_pGA->AverageFitness());
	//TextOut(surface, 5, 40, s.c_str(), s.size());
	drawstring(data, 5, 40, s.c_str(), m_BlackPen);

	//render the graph
	float HSlice = (float)cxClient/(m_iGenerations+1);
	float VSlice = (float)cyClient/((m_pGA->BestFitness()+1)*2);

	//plot the graph for the best fitness
	float x = 0;

	//m_OldPen = (HPEN)SelectObject(surface, m_RedPen);
	m_OldPen = m_RedPen;

	//MoveToEx(surface, 0, cyClient, NULL);
	int oldx = 0;
	int oldy = cyClient;
	int i = 0;
	for (i=0; i<m_vecBestFitness.size(); ++i)
	{
		//LineTo(surface, x, cyClient - VSlice*m_vecBestFitness[i]);
		drawline(data, oldx, oldy, x, cyClient - VSlice*m_vecBestFitness[i], m_OldPen);
		oldx = x;
		oldy = cyClient - VSlice*m_vecBestFitness[i];
		x += HSlice;
	}

	//plot the graph for the average fitness
	x = 0;

	//SelectObject(surface, m_BluePen);

	//MoveToEx(surface, 0, cyClient, NULL);

	for (i=0; i<m_vecAvFitness.size(); ++i)
	{
		//LineTo(surface, (int)x, (int)(cyClient - VSlice*m_vecAvFitness[i]));

		x += HSlice;
	}

	//replace the old pen
	//SelectObject(surface, m_OldPen);
}
