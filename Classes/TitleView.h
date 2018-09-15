#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class TitleView : public cocos2d::Layer
{
public:
	enum eBUTTON {
		eBUTTON_START = 0,
		eBUTTON_PLAY = eBUTTON_START,
		eBUTTON_TRAINING,
		eBUTTON_NUM,
	};
	
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	// 画面遷移ボタン.
	void playStartCallback(cocos2d::Ref* pSender, int btnid);
	
	// implement the "static create()" method manually
	CREATE_FUNC(TitleView);
};
#endif // __HELLOWORLD_SCENE_H__
