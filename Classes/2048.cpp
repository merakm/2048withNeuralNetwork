//
//  2048.cpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/03.
//
//

#include "2048.hpp"

CGameManager::CGameManager( Layer* pParent, Point pos ) {

	// 初期化.
	float height = TILE_SIZE * ( ROW_NUM - 1 );
	for (int i = 0; i < TILE_NUM; ++i) {
		int row = i / COLUMN_NUM, column = i % COLUMN_NUM;
		m_tile[i].number = 0;
		Sprite *p = Sprite::create();
		p->retain();
		Rect rect(0, 0, TILE_SIZE - 5, TILE_SIZE - 5 );
		p->setTextureRect(rect);
		p->ignoreAnchorPointForPosition(true);
		p->setPosition(Point(pos.x + column*TILE_SIZE, pos.y + height - row*TILE_SIZE ));
		p->setColor(Color3B::BLACK);
		pParent->addChild(p);
		m_tile[i].sprite = p;
		
		Label* pl = Label::createWithTTF("2", "fonts/arial.ttf", 40);
		m_tile[i].label = pl;
		pl->ignoreAnchorPointForPosition(true);
		pl->setPosition(Point(pos.x + column*TILE_SIZE, pos.y + height - row*TILE_SIZE ));
		pParent->addChild(pl);
		pl->retain();
	}
	m_bGameOver = false;
	m_score = 0;
}

CGameManager::~CGameManager() {	
	
}

void CGameManager::Start() {
	
	// 初期化.
	for (int i = 0; i < TILE_NUM; ++i) {
		m_tile[i].number = 0;
		m_tile[i].Update();
	}
	
	for (int i = 0; i < 2 /*最初は2個*/; ++i) {
		int nTileID = GenerateRandom();
		srand (time(NULL));
		int nRand = arc4random() % 4;
		m_tile[nTileID].number = nRand == 0 ? 2 : 1;
	}
	
	m_bGameOver = false;
	m_score = 0;
	
}

void CGameManager::Update(){
	
	for (int i = 0; i < TILE_NUM; ++i) {
		m_tile[i].Update();
	}
#if _MOVE_REQUEST
	Move();
#endif
	CheckGameOver();
}

void CGameManager::CheckGameOver() {
	
	for (int i = 0; i < TILE_NUM; ++i) {
		// ひとつでも空白があればGame overじゃない./
		if (m_tile[i].number == 0) return;
	}
	
	for (int i = 0; i < TILE_NUM; ++i) {
		if ( CanMove(i)) return;
	}
	
 	m_bGameOver = true;
	
}

int	CGameManager::getTileNum( vector<int> &tile ) {
	
	tile.clear();
	for (int n = 0; n < TILE_NUM; ++n) {
		tile.push_back(m_tile[n].number);
	}
	
	return tile.size();
}

int CGameManager::getMaxNumber() {

	int nRet = 0;
	for (int n = 0; n < TILE_NUM; ++n) {
		nRet = max(nRet, m_tile[n].number);
	}
	return nRet;
	
}

bool CGameManager::CanMove( int id ) {
	
	int nVal = m_tile[id].number;
	if (nVal == 0) return true;
	
	int nColumn = id % COLUMN_NUM;
	int nRow = id / COLUMN_NUM;
	
	// up.
	int nNextID = id - COLUMN_NUM;
	int nNextColumn = nColumn;
	int nNextRow = nRow - 1;
	if ( IsInRange(nNextRow, nNextColumn) && m_tile[nNextID].number == nVal ) return true;
	
	// left.
	nNextID = id - 1;
	nNextColumn = nColumn - 1;
	nNextRow = nRow;
	if ( IsInRange(nNextRow, nNextColumn) && m_tile[nNextID].number == nVal ) return true;

	// right.
	nNextID = id + 1;
	nNextColumn = nColumn + 1;
	nNextRow = nRow;
	if ( IsInRange(nNextRow, nNextColumn) && m_tile[nNextID].number == nVal ) return true;
	
	// down.
	nNextID = id + COLUMN_NUM;
	nNextColumn = nColumn + 1;
	nNextRow = nRow;
	if ( IsInRange(nNextRow, nNextColumn) && m_tile[nNextID].number == nVal ) return true;
	return false;
	
}

#if _MOVE_REQUEST
void CGameManager::Move( eDIRECTION dir ) {

	if ( m_dirReq == eDIRECTION_INVALID ) {
		m_dirReq = dir;
	}
}
#endif

#if _MOVE_REQUEST
bool CGameManager::Move() {
	eDIRECTION dir = m_dirReq;
#else
bool CGameManager::Move(eDIRECTION dir, bool bTest /*= false*/, int* score /*= NULL*/) {
#endif
	
	bool bRet = false;
	switch (dir) {
		case eDIRECTION_DOWN:
			bRet = MoveDown(bTest, score);
		break;
		case eDIRECTION_UP:
			bRet = MoveUp(bTest, score);
			break;
		case eDIRECTION_LEFT:
			bRet = MoveLeft(bTest, score);
			break;
		case eDIRECTION_RIGHT:
			bRet = MoveRight(bTest, score);
			break;
		default:
			break;
	}
	
	if (!bTest && bRet) {
		int nNext = GenerateRandom();
		m_tile[nNext].number = (arc4random() % 4 == 0 ) ? 2 : 1;
	}
	
	m_dirReq = eDIRECTION_INVALID;
	
	return bRet;
}

bool CGameManager::MoveLeft(bool bTest /*= false*/, int* score /* = NULL */) {
	
	int check[] = {0, 4, 8, 12};
	bool bMove = false;// 動いたかどうか.
	for (int i = 0; i < ROW_NUM; ++i) {
		int nCheck = check[i];
		
		for (int j = 1; j < COLUMN_NUM; ++j) {
			int index = nCheck + j;
			int val = m_tile[index].number;
			if ( val == 0 ) continue;
			
			while (nCheck != index) {
				int prevIndex = index - 1; // ひとつ前.
				int curVal = m_tile[prevIndex].number; // ひとつ前の値.
				if ( curVal != 0 ) {
					// 値があった.
					if ( val == curVal ) {
						// 値が一致すれば足す.
						if (!bTest) {
							m_tile[prevIndex].number += 1;
							m_tile[index].number = 0;
							m_score += CalcScore(m_tile[prevIndex].number);
						}
						if ( score ) *score += CalcScore(m_tile[prevIndex].number*2);
						bMove = true;
					}
					break;
				}
				else {
					// 値がなければそこへ移動.
					if (!bTest) {
						m_tile[prevIndex].number = m_tile[index].number;
						m_tile[index].number = 0;
					}
					bMove = true;
				}
				index = prevIndex;
			}
		}
	}
	return bMove;
}

bool CGameManager::MoveRight(bool bTest /*= false*/, int* score /* = NULL */) {
	
	int check[] = {3, 7, 11, 15};
	
	bool bMove = false;// 動いたかどうか.
	for (int i = 0; i < ROW_NUM; ++i) {
		int nCheck = check[i];
		
		for (int j = 1; j < COLUMN_NUM; ++j) {
			int index = nCheck - j;
			int val = m_tile[index].number;
			if ( val == 0 ) continue;
			
			while (nCheck != index) {
				int prevIndex = index + 1; // ひとつ前.
				int curVal = m_tile[prevIndex].number; // ひとつ前の値.
				if ( curVal != 0 ) {
					// 値があった.
					if ( val == curVal ) {
						// 値が一致すれば足す.
						if (!bTest) {
							m_tile[prevIndex].number += 1;
							m_tile[index].number = 0;
							m_score += CalcScore(m_tile[prevIndex].number);
						}
						if ( score ) *score += CalcScore(m_tile[prevIndex].number*2);
						bMove = true;
					}
					break;
				}
				else {
					// 値がなければそこへ移動.
					if (!bTest) {
						m_tile[prevIndex].number = m_tile[index].number;
						m_tile[index].number = 0;
					}
					bMove = true;
				}
				index = prevIndex;
			}
		}
	}
	
	return bMove;
}

bool CGameManager::MoveUp(bool bTest /*= false*/, int* score /* = NULL */) {

	int check[] = {0, 1, 2, 3};
	
	bool bMove = false;// 動いたかどうか.
	for (int i = 0; i < ROW_NUM; ++i) {
		int nCheck = check[i];
		
		for (int j = 1; j < COLUMN_NUM; ++j) {
			int index = nCheck + COLUMN_NUM * j;
			int val = m_tile[index].number;
			if ( val == 0 ) continue;
			
			while (nCheck != index) {
				int prevIndex = index - COLUMN_NUM; // ひとつ前.
				int curVal = m_tile[prevIndex].number; // ひとつ前の値.
				if ( curVal != 0 ) {
					// 値があった.
					if ( val == curVal ) {
						// 値が一致すれば足す.
						if (!bTest) {
							m_tile[prevIndex].number += 1;
							m_tile[index].number = 0;
							m_score += CalcScore(m_tile[prevIndex].number);
						}
						if ( score ) *score += CalcScore(m_tile[prevIndex].number*2);
						bMove = true;
					}
					break;
				}
				else {
					// 値がなければそこへ移動.
					if (!bTest) {
						m_tile[prevIndex].number = m_tile[index].number;
						m_tile[index].number = 0;
					}
					bMove = true;
				}
				index = prevIndex;
			}
		}
	}
	return bMove;
}

bool CGameManager::MoveDown(bool bTest /*= false*/, int* score /* = NULL */) {
	
	int check[] = {12, 13, 14, 15};
	
	bool bMove = false;// 動いたかどうか.
	for (int i = 0; i < ROW_NUM; ++i) {
		int nCheck = check[i];
		
		for (int j = 1; j < COLUMN_NUM; ++j) {
			int index = nCheck - COLUMN_NUM * j;
			int val = m_tile[index].number;
			if ( val == 0 ) continue;
			
			while (nCheck != index) {
				int prevIndex = index + COLUMN_NUM; // ひとつ前.
				int curVal = m_tile[prevIndex].number; // ひとつ前の値.
				if ( curVal != 0 ) {
					// 値があった.
					if ( val == curVal ) {
						// 値が一致すれば足す.
						if (!bTest) {
							m_tile[prevIndex].number += 1;
							m_tile[index].number = 0;
							m_score += CalcScore(m_tile[prevIndex].number);
						}
						if ( score ) *score += CalcScore(m_tile[prevIndex].number*2);
						bMove = true;
					}
					break;
				}
				else {
					// 値がなければそこへ移動.
					if (!bTest) {
						m_tile[prevIndex].number = m_tile[index].number;
						m_tile[index].number = 0;
					}
					bMove = true;
				}
				index = prevIndex;
			}
		}
	}
	return bMove;
	
}

int CGameManager::GenerateRandom(){
	
	vector<int> il; il.clear();
	for (int i = 0; i < TILE_NUM; ++i) {
		if (m_tile[i].number == 0) {
			// 何もないタイルが候補.
			il.push_back(i);
		}
	}
	
	// シャッフル.
	srand (time(NULL));
	random_shuffle(il.begin(), il.end());
	
	return il.front();
}