//
//  GameAIManager.cpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/03.
//
//

#include "GameAIManager.hpp"

int randLessThan(const int &n) {
	int answer = n;
	while(answer == n) answer = ((double)rand() / RAND_MAX) * n;
	return answer;
}

CGameAIManager::CGameAIManager() {
	
	m_currentSpecies = 0;
	m_currentGeneration = 0;
}

void CGameAIManager::Init(int num, Topology topology) {
	
	ai.clear();
	gene.clear();
	score.clear();
	ai.reserve(num);
	for (int i = 0; i < num; ++i) {
		ai.push_back(CNeuralNet(topology));
		gene.push_back(Weights());
		ai.back().GetWeights(gene[i]);
		score.push_back(0.0);
	}
	m_currentSpecies = 0;
	m_currentGeneration = 0;
	m_mutationNum = 0;
}

void CGameAIManager::SetScore( double dscore ) {
	
	score[m_currentSpecies] = dscore;
}


int CGameAIManager::Predict( vector<double> input, vector<double> &output ){
	
	CNeuralNet* net = &ai[m_currentSpecies];
	net->forward(input);		// データ渡し.
	net->getResult(output);	// 出力取得.
	
	return output.size();
}

void CGameAIManager::Calibration(vector<double> answer) {

	CNeuralNet* net = &ai[m_currentSpecies];
	net->backward(answer);
	
}


bool CGameAIManager::NextSpecies() {
	
	if (m_currentSpecies == ai.size() - 1) {
		return false;
	}
	m_currentSpecies++;

	ai[m_currentSpecies].SetWeights(gene[m_currentSpecies]);
	return true;
}


void CGameAIManager::NextGeneration() {
	
	Evolution();
	m_currentGeneration++;
	m_currentSpecies = 0;
	
}

void CGameAIManager::Evolution() {

	// Mating();	// 交配.
}

void CGameAIManager::Mating() {

	// スコアの平均と最大, 最小を出す.
	int dsum = 0.0, dave, dmax = 0.0, dmin = score[0];
	int nNum = score.size();
	for (int n = 0; n < nNum; ++n) {
		dsum += score[n];
		dmax = max(dmax, score[n]);
		dmin = min(dmin, score[n]);
	}
	dave = dsum / nNum;
	
	vector<Weights> nextWeight;
	for (int n = 0; n < nNum / 2; ++n) {
		
		int nFather = 0, nMother = 0;
		Weights child1, child2;
		
		// Selection.
		while (nFather == nMother) {
			nFather = Selection(dsum);
			nMother = Selection(dsum);
		}
		
		// Crossover.
		Crossover(nFather, nMother, child1, child2);
		
		// Mutation.
		Mutation( child1 );
		Mutation( child2 );
		
		nextWeight.push_back(child1);
		nextWeight.push_back(child2);
	}
	
	assert(nextWeight.size() == nNum);
}

int	CGameAIManager::Selection(int scoreSum) {

	/* ルーレット法 */
	int roulette = randLessThan(scoreSum);

	double acc = 0.0;
	for (int n = 0; n < score.size(); ++n) {
		acc += score[n];
		// ルーレットで得られた値を越えたらその時の個体.
		if ( acc  > roulette ) return n;
	}
	
	assert(false);
	return -1;
}

void CGameAIManager::Crossover( int father, int mother, Weights &child1, Weights &child2 ) {
	
	/*一様交叉*/
	child1.clear(); child2.clear();
	
	int nNum = gene[father].size();
	
	for (int n = 0; n < nNum; ++n) {
		int random = rand() % 2;
		child1.push_back(gene[random == 0 ? father : mother][n]);
		child2.push_back(gene[random == 1 ? father : mother][n]);
	}
}

void CGameAIManager::Mutation(Weights &gene) {
	
	int nNum = gene.size();
	for (int n = 0; n < nNum; ++n) {
		if ( randLessThan(100) < 1 /*1%*/ ) {
			gene[n] = 1.0 - 2.0 * rand() / double(RAND_MAX);
			// gene[n] = rand() % 3 - 1;
			m_mutationNum++;
		}
	}
	
}

void CGameAIManager::WriteWeight() {
	
	// ファイル出力.
	cocos2d::FileUtils* fileUtil = cocos2d::FileUtils::getInstance();
	string path = fileUtil->getWritablePath();
	string filePath = path + "sample.txt";
	FILE* fp = fopen(filePath.c_str(), "w");
	
	// 現世代が何世代目か.
	fwrite(&m_currentGeneration, sizeof(m_currentGeneration), 1, fp);
	int nNumSpecies = gene.size();
	fwrite(&nNumSpecies, sizeof(nNumSpecies), 1, fp);
	for (int nSpecies = 0; nSpecies < nNumSpecies; ++nSpecies) {
		int nWeightNum = gene[nSpecies].size();
		fwrite(&nWeightNum, sizeof(nWeightNum), 1, fp);
		for (int nConnect = 0; nConnect < nWeightNum; ++nConnect) {
			fwrite(&gene[nSpecies][nConnect], sizeof(double), 1, fp);
		}
	}
	
	fclose(fp);
	
}
