//
//  NeuralNet.h
//  Tetris
//
//  Created by Kohei Kumazaki on 2017/02/24.
//
//

#ifndef NeuralNet_h
#define NeuralNet_h
#include <vector>

using namespace std;
class Neuron;


/*
	ニューラルネットワークの構成
	例) {3, 2, 1} : 2層のネットワーク.
		入力層の Neuron : 3
 		隠れ層の Neuron : 2
 		出力層の Neuron : 1
 */
typedef vector<int> Topology;


// ***************** Layer ***********************
//	ニューラルネットワークの層. Neuronの集まり.
typedef std::vector<Neuron> NeuralLayer;


// ***************** class Neuron ***********************
class Neuron {
	
public:
	Neuron(int outputNum, int id);
	~Neuron(){}
	
	void	forward( const NeuralLayer &prevLayer );	// 入力値から出力値を計算.
	
	void	calcOutputGradient( double targetVal );
	void	calcHiddenGradient( NeuralLayer prevlayer );
	void	updateWeight( NeuralLayer& prevlayer );
	void	SetOutputValue( double val ) { m_outputValue = val; }
	double	GetOutputValue() const { return m_outputValue; }
	void	SetWeight(int id, double weight);
	double	GetWeight(int id) const;
	
private:
	double	TransferFunction( double val );
	double	TransferFunctionDerivative( double val );
private:
	int	m_id;
	
	vector<double> m_weight;	// 次の層の Neuron 間の重み.
	double m_outputValue;		// 出力値.
	double m_gradient;			// 勾配.
	
};

// ***************** class NeuralNet ***********************
class CNeuralNet {

public:
	CNeuralNet( Topology &topology );
	~CNeuralNet(){}

	bool forward( vector<double> &inputValue );		// 順伝播.
	void backward( vector<double> &targetValue );	// 逆伝播による学習.
	int  getResult( vector<double> &resultValue );	// 結果の取得.

	double	GetError(){ return m_error; }
	int		GetWeights( vector<double> &weight ) const;
	int		GetOutputs( vector<double> &output ) const;
	int		SetWeights( const vector<double> &weight );

	int		GetHiddenLayerNum();
	int		GetInputNeuronNum();
	int		GetHiddenNeuronNum(int depth);
	int		GetOutputNeuronNum();
	
protected:
	vector<NeuralLayer> m_Layers;
	double m_error;

};

#endif /* NeuralNet_h */
