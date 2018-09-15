#include "TitleView.h"
#include "PlayView.hpp"
#include "LearningView.h"

USING_NS_CC;

static const std::string btntext[TitleView::eBUTTON_NUM] = {
	"Play", //  eBUTTON_PLAY.
	"Training", //  eBUTTON_TRAINING.
};

Scene* TitleView::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = TitleView::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitleView::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
	
	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
										   "CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(TitleView::menuCloseCallback, this));
	
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
								origin.y + closeItem->getContentSize().height/2));
	
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	/////////////////////////////
	// 3. add your codes below...
	
	// add a label shows "Hello World"
	// create and initialize a label
	
	auto label = Label::createWithTTF("2048", "fonts/Marker Felt.ttf", 24);
	
	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
							origin.y + visibleSize.height - label->getContentSize().height));
	
	// add the label as a child to this layer
	this->addChild(label, 1);
	
	// ボタン.
	Vec2 BtnOrigin( origin.x + visibleSize.width/2, origin.y + visibleSize.height/2 );
	for (int i = eBUTTON_START; i < eBUTTON_NUM; ++i) {
		Vec2 pos( BtnOrigin.x, BtnOrigin.y - 20 - 25*(i+1) );
		Label* pbtnlabel = Label::createWithTTF(btntext[i], "fonts/arial.ttf", 20);
		pbtnlabel->setTag(i);
		auto pBtn = MenuItemLabel::create(pbtnlabel, CC_CALLBACK_1(TitleView::playStartCallback, this, i));
		
		Menu* pMenu = Menu::create(pBtn, NULL);
		pMenu->setPosition(pos);
		this->addChild(pMenu, 1);
	}
	
	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("identicon_2048.png");
	
	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	
	// add the sprite as a child to this layer
	this->addChild(sprite, 0);
	
	return true;
}


void TitleView::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



// 画面遷移ボタン.
void TitleView::playStartCallback(cocos2d::Ref* pSender, int btnid)
{
	
	log(btntext[btnid].c_str());
	switch (btnid) {
		case eBUTTON_PLAY:
		{
			auto scene = PlayView::createScene();
			Director::getInstance()->pushScene(scene);
		}
			break;
		case eBUTTON_TRAINING:
		{
			auto scene = LearningView::createScene();
			Director::getInstance()->pushScene(scene);
		}
			break;

		default:
			break;
	}
}