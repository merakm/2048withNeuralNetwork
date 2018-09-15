//
//  GameAIManager.hpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/03.
//
//

#ifndef GameAIManager_hpp
#define GameAIManager_hpp

#include "NeuralNet.h"
using namespace std;

typedef vector<double> Weights;
class CGameAIManager {
	
public:
	
public:
	CGameAIManager();
	~CGameAIManager(){};
	
	void Init( int num, Topology topology );
	
public:
	bool	NextSpecies();	// 同世代の次の個体.
	void	NextGeneration();	// 次世代への進化.
	int		Predict( vector<double> input, vector<double> &output );	// 個体の反応取得.
	void	Calibration(vector<double> answer);
	void	SetScore( double score );
	
	int		GetCurrentSpecies()		{ return m_currentSpecies; }
	int		GetCurrentGeneration()	{ return m_currentGeneration; }
	int		GetMutationNum()		{ return m_mutationNum; }
	CNeuralNet* GetCurrentAI()		{ return &ai[m_currentSpecies]; }
	
private:
	void	Evolution();
	void	Mating();
	int		Selection(int scoreSum);
	void	Crossover(int father, int mother, Weights &child1, Weights &child2);
	void	Mutation(Weights &gene);
	
private:
	void	WriteWeight();
	
private:
	vector<CNeuralNet>	ai;
	vector<Weights>		gene;
	vector<int>			score;
	int		m_currentSpecies;
	int		m_currentGeneration;
	int		m_mutationNum;
};

#endif /* GameAIManager_hpp */
