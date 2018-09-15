//
//  NeuralNet.cpp
//  Tetris
//
//  Created by Kohei Kumazaki on 2017/02/24.
//
//

#include "NeuralNet.h"
#define eta 0.15
#define limit 0.00000001
// ***************** class Neuron ***********************
Neuron::Neuron(int outputNum, int id) {
	
	for (int i = 0; i < outputNum; ++i) {
		m_weight.push_back(1.0 - 2.0 * rand() / double(RAND_MAX));	// weight の初期化.
		// m_weight.push_back(rand() % 3 - 1);	// weight の初期化.
	}
	m_id = id;
	m_outputValue = 1.0f;
	m_gradient = 0.0f;
}


void Neuron::forward( const NeuralLayer &prevLayer ) {
	
	double sum = 0.0f;
	for (int i = 0; i < prevLayer.size(); ++i) {
		sum += prevLayer[i].GetOutputValue() * prevLayer[i].GetWeight(m_id);
	}
	
	m_outputValue = TransferFunction(sum);
	assert( m_outputValue != 0);
}

void Neuron::calcOutputGradient(double targetVal) {
	
	double delta = targetVal - m_outputValue;
	m_gradient = delta * TransferFunctionDerivative(m_outputValue);
	
}

void Neuron::calcHiddenGradient( NeuralLayer prevlayer ) {
	
	double dow = 0;
	for (int i = 0; i < prevlayer.size() - 1; ++i) {
		dow += prevlayer[i].m_gradient * m_weight[i];
	}
	
	m_gradient = dow * TransferFunctionDerivative(m_outputValue);
	
}

void  Neuron::updateWeight( NeuralLayer& prevlayer ) {
	
	for (int nNeuron = 0; nNeuron < prevlayer.size(); ++nNeuron) {
		Neuron &curNeuron = prevlayer[nNeuron];
		//if (curNeuron.GetOutputValue() != 0) {
			curNeuron.m_weight[m_id] += eta * curNeuron.GetOutputValue() * m_gradient;
			//assert( fabs(curNeuron.m_weight[m_id]) > limit);
		//}
	}
}

void Neuron::SetWeight(int id, double weight) {
	
	if (0 <= id && id < m_weight.size() ) {
		m_weight[id] = weight;
	}
	
}

double Neuron::GetWeight(int id) const {
	
	if (0 <= id && id < m_weight.size() ) {
		return m_weight[id];
	}
	
	return 0.0f;
}

double Neuron::TransferFunction( double val ) {
	
	return tanh(val);
}

double Neuron::TransferFunctionDerivative(double val) {
	
	return 1.0 - val * val;
}
// ***************** class NeuralNet ***********************

CNeuralNet::CNeuralNet( Topology &topology )
: m_error(0.0)
{
	
	int nNumLayer = topology.size();
	for (int nLayer = 0; nLayer < nNumLayer; ++nLayer) {
		// ネットワークへ Layer 追加.
		m_Layers.push_back(NeuralLayer());
		
		// Layer へ Neuron を追加.
		int nOutNum = (nLayer == nNumLayer - 1) ? 0 : topology[nLayer+1];
		int nNumNeuron = topology[nLayer] + 1;	// +1 is from bias.
		for (int nNeuron = 0; nNeuron < nNumNeuron; ++nNeuron) {
			m_Layers.back().push_back(Neuron( nOutNum, nNeuron));
		}
		m_Layers.back().back().SetOutputValue(1.0);
	}
}


bool CNeuralNet::forward( vector<double> &inputValue ) {
	
	// 入力データのサイズと、入力層のサイズが一致しているはず.
	if (inputValue.size() != m_Layers[0].size() - 1 /*bias の分だけ引く*/) {
		assert(false);
		return false;
	}
	
	// 入力層へデータを入れる.
	for (int nNeuron = 0; nNeuron < inputValue.size(); ++nNeuron) {
		m_Layers[0][nNeuron].SetOutputValue(inputValue[nNeuron]);
	}
	
	// Forward propagation.
	for (int nLayer = 1; nLayer < m_Layers.size(); ++nLayer) {
		NeuralLayer &prevLayer = m_Layers[nLayer - 1];
		for (int nNeuron = 0; nNeuron < m_Layers[nLayer].size() - 1; ++nNeuron) {
			m_Layers[nLayer][nNeuron].forward(prevLayer);
		}
	}

	return true;
	
}


void CNeuralNet::backward(vector<double> &targetValue) {
	
	
	// 誤差推定.
	m_error = 0.0;
	NeuralLayer &outputLayer = m_Layers.back();
	assert(targetValue.size() == outputLayer.size() - 1);

	for (int n = 0; n < outputLayer.size() - 1; ++n) {
		double delta = targetValue[n] - outputLayer[n].GetOutputValue();
		m_error += delta * delta;
	}
	
	m_error /= outputLayer.size() - 1;
	m_error = sqrt(m_error);
	
	cocos2d::log("error : %f", m_error);
	// 出力層の勾配を計算.
	for (int i = 0; i < outputLayer.size(); ++i) {
		outputLayer[i].calcOutputGradient(targetValue[i]);
	}
	
	// 隠れ層の勾配を計算.
	for (int nHiddenLayer = m_Layers.size() - 2; nHiddenLayer > 0; --nHiddenLayer) {
		NeuralLayer &hiddenLayer = m_Layers[nHiddenLayer];
		NeuralLayer &nextLayer = m_Layers[nHiddenLayer+1];
		
		for (int nNeuron = 0; nNeuron < hiddenLayer.size() - 1; ++nNeuron) {
			hiddenLayer[nNeuron].calcHiddenGradient(nextLayer);
		}
	}
	
	// weightの更新.
	for (int nLayer = m_Layers.size()-1; nLayer > 0; --nLayer) {
		NeuralLayer &curLayer = m_Layers[nLayer];
		NeuralLayer &nextLayer = m_Layers[nLayer-1];
		for (int nNeuron = 0; nNeuron < curLayer.size(); ++nNeuron) {
			curLayer[nNeuron].updateWeight(nextLayer);
		}
	}
}

int CNeuralNet::getResult(vector<double> &resultValue){
	
	resultValue.clear();
	NeuralLayer layer = m_Layers.back();
	for (int nOutputNeuron = 0; nOutputNeuron < layer.size() - 1; ++nOutputNeuron) {
		resultValue.push_back(layer[nOutputNeuron].GetOutputValue());
	}
	
	return resultValue.size();
}

int CNeuralNet::GetWeights( vector<double> &weightOutput ) const {
	
	weightOutput.clear();
	for (int nLayer = 0; nLayer < m_Layers.size() - 1 /*出力層のweightはいらない*/; ++nLayer) {
		NeuralLayer lay = m_Layers[nLayer];
		for (int nNeuron = 0; nNeuron < lay.size(); ++nNeuron) {
			Neuron neuron = lay[nNeuron];
			int nNextNeuronNum = m_Layers[nLayer + 1].size();
			for (int n = 0; n < nNextNeuronNum - 1; ++n) {
				double weight = neuron.GetWeight(n);
				weightOutput.push_back(weight);
			}
		}
	}
	
	return weightOutput.size();
}

int CNeuralNet::SetWeights( const vector<double> &weightInput ) {

	int nCnt = 0;
	for (int nLayer = 0; nLayer < m_Layers.size() - 1 /*出力層のweightはいらない*/; ++nLayer) {
		NeuralLayer lay = m_Layers[nLayer];
		for (int nNeuron = 0; nNeuron < m_Layers[nLayer].size() - 1; ++nNeuron) {
			Neuron neuron = lay[nNeuron];
			int nNextNeuronNum = m_Layers[nLayer + 1].size();
			for (int n = 0; n < nNextNeuronNum - 1; ++n) {
				double weight = weightInput[nCnt];
				neuron.SetWeight(n, weight);
			}
			nCnt++;
		}
	}
	return nCnt;
}

int CNeuralNet::GetOutputs( vector<double> &output ) const {
	
	output.clear();
	
	for (int nLayer = 0; nLayer < m_Layers.size(); ++nLayer) {
		NeuralLayer lay = m_Layers[nLayer];
		for (int nNeuron = 0; nNeuron < lay.size(); ++nNeuron) {
			Neuron neuron = lay[nNeuron];
			double dOutput = neuron.GetOutputValue();
			output.push_back(dOutput);
		}
	}
	
	return output.size();
	
}


int	CNeuralNet::GetHiddenLayerNum() {
	return m_Layers.size() - 2;	// inputとoutputは除く.
}

int	CNeuralNet::GetInputNeuronNum(){
	return m_Layers.front().size();
}

int	CNeuralNet::GetHiddenNeuronNum(int depth) {
	
	int nLayerNum = GetHiddenLayerNum();
	if ( 0 <= depth && depth < nLayerNum ) {
		return m_Layers[depth+1].size();
	}
	return 0;
}

int	CNeuralNet::GetOutputNeuronNum() {
	return m_Layers.back().size() - 1;
}
