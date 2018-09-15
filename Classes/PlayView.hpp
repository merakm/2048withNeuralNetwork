//
//  PlayView.hpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/04.
//
//

#ifndef PlayView_hpp
#define PlayView_hpp
USING_NS_CC;
#include "2048.hpp"
#include "GameAIManager.hpp"

class PlayView : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	void update(float delta);
	
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// implement the "static create()" method manually
	CREATE_FUNC(PlayView);
	
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	
private:
	CGameManager*	m_game;
};
#endif /* PlayView_hpp */
