//
//  PlayView.cpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/04.
//
//

#include "PlayView.hpp"

Scene* PlayView::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = PlayView::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool PlayView::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	this->scheduleUpdate();			// update関数呼び出し許可.
	{
		this->setKeyboardEnabled(true);	// キーボード使用許可.
		auto listener = EventListenerKeyboard::create();
		listener->onKeyPressed = CC_CALLBACK_2(PlayView::onKeyPressed, this);
		listener->onKeyReleased = CC_CALLBACK_2(PlayView::onKeyReleased, this);
		
		//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
	
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
										   "CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(PlayView::menuCloseCallback, this));
	
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
								origin.y + closeItem->getContentSize().height/2));
	
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	LayerColor::initWithColor(Color4B(255,255,255,255));
	
	Point gamepos;
	gamepos.x = origin.x + 50;
	gamepos.y = origin.y + 50;
	
	m_game = new CGameManager(this, gamepos);
	m_game->Start();

	return true;
}


void PlayView::update(float delta) {
	
	m_game->Update();
	if (m_game->IsGameOver()) {
		// ゲーム終了.
		m_game->Start();
	}
}

void PlayView::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void PlayView::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_A:
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			m_game->Move( CGameManager::eDIRECTION_LEFT );
			break;
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			m_game->Move( CGameManager::eDIRECTION_RIGHT );
			break;
		case EventKeyboard::KeyCode::KEY_W:
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			m_game->Move( CGameManager::eDIRECTION_UP );
			break;
		case EventKeyboard::KeyCode::KEY_S:
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			m_game->Move( CGameManager::eDIRECTION_DOWN );
			break;
			
		default:
			break;
	}
	CCLOG("Key with keycode %d pressed", keyCode);
}
