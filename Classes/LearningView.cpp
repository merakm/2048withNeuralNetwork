//
//  LearningView.cpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/02/18.
//
//
USING_NS_CC;

#include "LearningView.h"

bool NeuralNetView::init() {

	if ( !LayerColor::init() )
	{
		return false;
	}
	//this->scheduleUpdate();
	m_pAI = NULL;
	
	m_pDrawNeuron = DrawNode::create();
	//m_pDrawNeuron->retain();
	addChild(m_pDrawNeuron, 1);
	m_pDrawLine = DrawNode::create();
	//m_pDrawLine->retain();
	addChild(m_pDrawLine, 0);
	
	return true;
}

void NeuralNetView::update(float delta) {
	
	updateNeuron();
}

Color4F NeuralNetView::getNodeColor(double value) {
	
	GLfloat green = 0;
	GLfloat red = min(1.0*((GLfloat)value + 1.0), 1.0);
	GLfloat blue = min(1.0*(-(GLfloat)value + 1.0), 1.0);
	Color4F color = Color4F(red, green, blue, (GLfloat)1.0);
	
	return color;
	
}

Color4F NeuralNetView::getLineColor(double value){

	GLfloat blue = 0;
	GLfloat green = min(1.0*((GLfloat)value + 1.0), 1.0);
	GLfloat red = min(1.0*(-(GLfloat)value + 1.0), 1.0);
	Color4F color = Color4F(red, green, blue, (GLfloat)1.0);
	
	return color;
}


void NeuralNetView::updateNeuron() {

	if ( !m_pAI ) return;
	//if (!m_pDrawNeuron) return;
	vector<double> output;
	int nOutNum = m_pAI->GetOutputs(output);
	
	int nCnt = 0;
	
	int nInputNeuronNum  = m_pAI->GetInputNeuronNum();
	int nHiddenLayerNum  = m_pAI->GetHiddenLayerNum();
	int nOutputNeuronNum = m_pAI->GetOutputNeuronNum();
	
	m_pDrawNeuron->clear();
	
	float rad = ( inputNodePos[1].y - inputNodePos[0].y ) / 2.0 * 0.8;
	
	// input.
	for (int n = 0; n < nInputNeuronNum; ++n, ++nCnt) {
		if ( output[nCnt] == 0 ) continue;
		Color4F color = getNodeColor(output[nCnt]);
		m_pDrawNeuron->drawDot(inputNodePos[n], rad, color);
	}

	// hidden.
	int nHidden = 0;
	for (int nLayer = 0; nLayer < nHiddenLayerNum; ++nLayer) {
		int nHiddenNum = m_pAI->GetHiddenNeuronNum(nLayer);
		for (int nNeuron = 0; nNeuron < nHiddenNum; ++nNeuron, ++nHidden, ++nCnt) {
		Color4F color = getNodeColor(output[nCnt]);
		m_pDrawNeuron->drawDot(hiddenNodePos[nHidden], rad, color);
		}
	}
	// output.
	for (int n = 0; n < nOutputNeuronNum; ++n, ++nCnt) {
		Color4F color = getNodeColor(output[nCnt]);
		m_pDrawNeuron->drawDot(outputNodePos[n], rad, color);
	}

	
	
}

void NeuralNetView::set_ai( CNeuralNet* pai ){
	
	m_pAI = pai;
	
	updateNeuronPos();
	
	int nInputNum = m_pAI->GetInputNeuronNum();
	int nHiddenLayerNum = m_pAI->GetHiddenLayerNum();
	int nOutputNum = m_pAI->GetOutputNeuronNum();
	
	////if(!m_pDrawLine) return;
		
	m_pDrawLine->clear();
	// weight.
	vector<double> weight;
	m_pAI->GetWeights(weight);
	int nCnt = 0;//nInputNum;
	
	// input to hidden 1st.
	for (int nInputNeuron = 0; nInputNeuron < nInputNum; ++nInputNeuron) {
		Vec2 posInput = inputNodePos[nInputNeuron];
		int nHiddenNum = m_pAI->GetHiddenNeuronNum(0);
		for (int nHiddenNeuron = 0; nHiddenNeuron < nHiddenNum - 1; ++nHiddenNeuron) {
			Point posHidden = hiddenNodePos[nHiddenNeuron];
			double d = weight[nCnt];
			if ( d != 0 ) {
				m_pDrawLine->drawLine(posInput, posHidden, getLineColor(d) );
			}
			nCnt++;
		}
	}
	
	// hidden to output;
	for (int nOutputNeuron = 0; nOutputNeuron < nOutputNum; ++nOutputNeuron) {
		Vec2 posOutput = outputNodePos[nOutputNeuron];
		int nHiddenNum = m_pAI->GetHiddenNeuronNum(0);
		for (int nHiddenNeuron = 0; nHiddenNeuron < nHiddenNum; ++nHiddenNeuron) {
			Vec2 posHidden = hiddenNodePos[nHiddenNeuron];
			double d = weight[nCnt];
			if ( d != 0 ) {
				m_pDrawLine->drawLine(posOutput, posHidden, getLineColor(d) );
			}
			else {
				assert(false);
			}
			nCnt++;
		}
	}
	
}

void NeuralNetView::updateNeuronPos() {
	
	int nInputNum = m_pAI->GetInputNeuronNum();
	int nHiddenLayerNum = m_pAI->GetHiddenLayerNum();
	int nOutputNum = m_pAI->GetOutputNeuronNum();
	
	inputNodePos.clear();
	hiddenNodePos.clear();
	outputNodePos.clear();
	
	Size size = getContentSize();
	Point inputbase = { size.width  / ( nHiddenLayerNum + 2/*input + output*/ + 1 ),
						size.height / ( nInputNum + 1 ) };

	// input.
	for (int i = 0; i < nInputNum; ++i) {
		inputNodePos.push_back( Point(inputbase.x, inputbase.y*(i+1)) );
	}
	
	// hidden.
	for (int nLayer = 0; nLayer < nHiddenLayerNum; ++nLayer) {
		int nHiddenNum = m_pAI->GetHiddenNeuronNum(nLayer);
		Point hiddenbase = { inputbase.x * ( nLayer + 2 ), size.height / ( nHiddenNum + 1 ) };
		for (int nNeuron = 0; nNeuron < nHiddenNum; ++nNeuron) {
			hiddenNodePos.push_back(Point(hiddenbase.x, hiddenbase.y*(nNeuron+1)));
		}
	}
	
	// output.
	Point outputbase = { size.width * ( nHiddenLayerNum + 1 + 1) / ( nHiddenLayerNum + 2 + 1 ),
						 size.height / ( nOutputNum  + 1 ) };
	
	for (int i = 0; i < nOutputNum; ++i) {
		outputNodePos.push_back( Point(outputbase.x, outputbase.y*(i + 1)) );
	}
	
}

static const std::string btntext[LearningView::eBUTTON_NUM] = {
	"Play", //  eBUTTON_PLAY.
	"Step", //  eBUTTON_STEP.
};

Scene* LearningView::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = LearningView::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool LearningView::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	// background color.
	LayerColor::initWithColor(Color4B(255,255,255,255));

	// init member.
	memset(m_apText, 0, sizeof(m_apText));

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	this->scheduleUpdate();			// update関数呼び出し許可.
	{
	this->setKeyboardEnabled(true);	// キーボード使用許可.
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(LearningView::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(LearningView::onKeyReleased, this);
	
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
										   "CloseNormal.png",
										   "CloseSelected.png",
										   CC_CALLBACK_1(LearningView::menuCloseCallback, this));
	
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
								origin.y + closeItem->getContentSize().height/2));
	
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	
	// text.
	Vec2 TextOrigin( 50, 650 );
	for (int i = eTEXT_START; i < eTEXT_NUM; ++i) {
		Vec2 pos( TextOrigin.x, TextOrigin.y - 20 - 25*(i+1) );
		m_apText[i] = Label::createWithTTF("", "fonts/arial.ttf", 20);
		m_apText[i]->retain();
		m_apText[i]->setPosition(pos);
		m_apText[i]->setColor(Color3B::BLACK);
		m_apText[i]->ignoreAnchorPointForPosition(true);
		this->addChild(m_apText[i]);
	}
	
	// button.
	Vec2 BtnOrigin( 50, 700 );
	for (int i = eBUTTON_START; i < eBUTTON_NUM; ++i) {
		Vec2 pos( BtnOrigin.x + 70*i, BtnOrigin.y );
		Label* pbtnlabel = Label::createWithTTF(btntext[i], "fonts/arial.ttf", 25);
		pbtnlabel->setColor(Color3B::BLACK);
		pbtnlabel->setTag(i);
		auto pBtn = MenuItemLabel::create(pbtnlabel, CC_CALLBACK_1(LearningView::buttonCallback, this, i));
		
		Menu* pMenu = Menu::create(pBtn, NULL);
		pMenu->setPosition(pos);
		this->addChild(pMenu, 1);
	}
	
	m_pNetworkView = NeuralNetView::create();
	m_pNetworkView->ignoreAnchorPointForPosition(true);
	m_pNetworkView->changeWidthAndHeight(600, 700);
	m_pNetworkView->setPosition(450, 50);
	addChild(m_pNetworkView);
	
	Point gamepos;
	gamepos.x = origin.x + 50;
	gamepos.y = origin.y + 50;
	
	m_game = new CGameManager(this, gamepos);
	m_game->Start();
	Topology topology = { 16, 10, 5, 2 };
	m_AI.Init(1, topology);
	
	m_pNetworkView->set_ai(m_AI.GetCurrentAI());
	m_nNumFault = 0;
	m_maxScore = 0;
	m_maxNumber = 0;
	
	m_bStep = false;
	m_bPlay = false;
	for (int n = 0; n < CGameManager::eDIRECTION_NUM; ++n) {
		m_dirCnt[n] = 0;
	}
	updateUI();
	return true;
}


void LearningView::update(float delta) {

	if ( !m_bStep && !m_bPlay ) return;
	
	m_bStep = false;
	updateAI();
	m_pNetworkView->updateNeuron();
	m_game->Update();
	if (m_game->IsGameOver() || m_nNumFault > 10) {
		// ゲーム終了.
		int nGen = m_AI.GetCurrentGeneration();
		int nSpe = m_AI.GetCurrentSpecies();
		int nScore = m_game->getScore();
		//log(" -- Finish Speceis %2d in Generation %d : Score %d -- ", nSpe, nGen, nScore);
		m_nNumFault = 0;
		m_AI.SetScore(nScore);
		bool isEnd = !m_AI.NextSpecies();
		if (isEnd) {
			m_AI.NextGeneration();
			nGen = m_AI.GetCurrentGeneration();
			//log("---------------------------------");
			//log(" -- Generation %2d Start", nGen);
			for (int n = 0; n < CGameManager::eDIRECTION_NUM; ++n) {
				m_dirCnt[n] = 0;
			}
		}

		m_game->Start();
	}
	m_pNetworkView->set_ai(m_AI.GetCurrentAI());
	updateUI();
	
}

void LearningView::updateAI() {
	
	// get tile info.
	vector<int> tile;
	int nTile = m_game->getTileNum(tile);
	
	if (nTile == 0) {
		return;
	}
	// search max.
	int nMax = 0;
	for (int n = 0; n < nTile; ++n) {
		nMax = max( nMax, tile[n] );
	}
	
	vector<double> input;
	for (int n = 0; n < nTile; ++n) {
		double up = (double)tile[n];
		double down = (double)nMax;
		double tmp = up / down;
		input.push_back( tmp );
	}
	
	// get output.
	vector<double> output;
	m_AI.Predict(input, output);
	
	// control.
	int nControl = 0;
	for (int n = 0; n < output.size(); ++n) {
		nControl += (output[n] > 0.0) ? (n+1) : 0;
	}
	bool bSuccess = false;// m_game->Move((CGameManager::eDIRECTION)nControl);
	
	const string ctrl[ CGameManager::eDIRECTION_NUM ] = {
		"LEFT",
		"RIGHT",
		"UP",
		"DOWN",
	};

	if ( !bSuccess ) {
		int nScoreMax = 0;
		int nMaxDir = 0;
		for (int i = 0; i < CGameManager::eDIRECTION_NUM; ++i) {
			int nScoreTmp = 0;
			bool bmove = m_game->Move((CGameManager::eDIRECTION)i, true, &nScoreTmp);
			if ( nScoreTmp > nScoreMax ) {
				nMaxDir = i;
				nScoreMax = nScoreTmp;
			}
			else if ( bmove && nScoreMax == 0 ){
				nMaxDir = i;
			}
		}
		if ( m_prevMove == nMaxDir) {
			m_nNumFault++;
		}
		else {
			m_nNumFault = 0;
		}
		m_prevMove = nMaxDir;
		m_dirCnt[nControl]++;
		vector<double> answer;
		answer.push_back((nMaxDir%2) == 1 ? 1.0 : -1.0);
		answer.push_back((nMaxDir/2) == 1 ? 1.0 : -1.0);
		m_AI.Calibration(answer);
		m_game->Move((CGameManager::eDIRECTION)nControl);
		// log("%s illegal move", ctrl[nControl].c_str());
	}
	else {
		log(ctrl[nControl].c_str());
	}
	
	string tmpstr = StringUtils::format( "Prediction : %s -  Answer : %s", ctrl[nControl].c_str(), ctrl[m_prevMove].c_str() );
	m_apText[eTEXT_MOVE]->setString(tmpstr);
}

void LearningView::updateUI() {
	
	string tmpstr = StringUtils::format( "Generation : %d", m_AI.GetCurrentGeneration() );
	m_apText[eTEXT_GENERATION]->setString(tmpstr);
	
	tmpstr = StringUtils::format( "Species : %d", m_AI.GetCurrentSpecies() );
	m_apText[eTEXT_SPECIES]->setString(tmpstr);

	m_maxNumber = max( m_maxNumber, m_game->getMaxNumber());
	tmpstr = StringUtils::format( "Score : %d (%d)", m_game->getScore(), m_game->getMaxNumber() );
	m_apText[eTEXT_SCORE]->setString(tmpstr);

	m_maxScore = max( m_maxScore, m_game->getScore() );
	tmpstr = StringUtils::format( "Max Score : %d (%d)", m_maxScore, m_maxNumber );
	m_apText[eTEXT_ELITE_SCORE]->setString(tmpstr);

//	tmpstr = StringUtils::format( "Mutation Num : %d", m_AI.GetMutationNum() );
	tmpstr = StringUtils::format( " Left : %d, Right : %d, Up : %d, Down : %d", m_dirCnt[0], m_dirCnt[1], m_dirCnt[2], m_dirCnt[3]  );
	m_apText[eTEXT_MUTATION]->setString(tmpstr);
	
	
}

void LearningView::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void LearningView::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		break;
	case EventKeyboard::KeyCode::KEY_S:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		break;

	default:
			break;
	}
	CCLOG("Key with keycode %d pressed", keyCode);

}

void LearningView::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	CCLOG("Key with keycode %d released", keyCode);
}

void LearningView::buttonCallback(cocos2d::Ref* pSender, int btnid)
{
	
	log(btntext[btnid].c_str());
	switch (btnid) {
		case eBUTTON_PLAY:
		{
			m_bPlay = !m_bPlay;
		}
			break;
		case eBUTTON_STEP:
		{
			m_bStep = true;
		}
			break;
			
		default:
			break;
	}
}

