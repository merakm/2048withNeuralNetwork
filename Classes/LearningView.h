//
//  LearningView.h
//  2048
//
//  Created by Kohei Kumazaki on 2017/02/18.
//
//

#ifndef LearningView_hpp
#define LearningView_hpp
USING_NS_CC;
#include "2048.hpp"
#include "GameAIManager.hpp"

class NeuralNetView : public LayerColor
{
public:
	
	CREATE_FUNC(NeuralNetView);
	
	virtual bool init();
	void update(float delta);
	void set_ai( CNeuralNet* pai );
	void updateNeuron();

private:
	void updateNeuronPos();
	
	Color4F getNodeColor(double value);
	Color4F getLineColor(double value);
	
private:
	CNeuralNet*			m_pAI;
	DrawNode*	m_pDrawNeuron;
	DrawNode*	m_pDrawLine;
	vector<Point>	inputNodePos;
	vector<Point>	hiddenNodePos;
	vector<Point>	outputNodePos;
};

class LearningView : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	void update(float delta);
	
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(LearningView);
	
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	
private:
	void updateAI();
	void updateUI();
	
private:
	//CTetris* m_pTetris;
	CGameManager*	m_game;
	CGameAIManager	m_AI;
	int				m_nNumFault;
	int				m_maxScore;
	int				m_maxNumber;
	int				m_prevMove;
	
	
public:
	enum eTEXT {
		eTEXT_START = 0,
		eTEXT_GENERATION = eTEXT_START,
		eTEXT_SPECIES,
		eTEXT_SCORE,
		eTEXT_ELITE_SCORE,
		eTEXT_MUTATION,
		eTEXT_MOVE,
		
		eTEXT_NUM,
	};
	
	enum eBUTTON {
		eBUTTON_START = 0,
		eBUTTON_PLAY = eBUTTON_START,
		eBUTTON_STEP,
		eBUTTON_NUM,
	};
	
public:
	void buttonCallback(cocos2d::Ref* pSender, int btnid);

public:
	bool m_bStep;
	bool m_bPlay;
private:
	int  m_dirCnt[CGameManager::eDIRECTION_NUM];
	
private:
	/* UI parts */
	Label*	m_apText[eTEXT_NUM];
	NeuralNetView* m_pNetworkView;
};


#endif /* LearningView_hpp */
