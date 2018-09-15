//
//  2048.hpp
//  2048
//
//  Created by Kohei Kumazaki on 2017/03/03.
//
//

#ifndef _2048_hpp
#define _2048_hpp

USING_NS_CC;
using namespace std;

#include <vector>

#define DefailtTileColor Color3B(255, 255, 255)

struct STile {
	int		number;
	Sprite* sprite;
	Label*	label;
	STile() {
		number = 0;
		sprite = NULL;
	}
	
	void Update() {
		// int alpha = 255 * min(number, 10) / 10;
		int colormax = 246, colormin = 186;
		float slope = (colormax - colormin) / 5;
		int red, green, blue;
		if ( 1 <= number && number < 5 ) {
			red = colormax;
			blue = colormin;
			green = (int)(colormin + slope * number);
		}
		else if ( 5 <= number && number < 10 ) {
			int x = number - 5;
			red = (int)(colormax - slope * x);
			blue = colormin;
			green = colormax;
		}
		else if ( 10 <= number && number < 15 ) {
			int x = number - 5;
			red = colormin;
			blue = (int)(colormin + slope * x);
			green = colormax;
		}
		else {
			red = colormax;
			blue = colormax;
			green = colormax;
		}
		
		if( sprite ) {
			sprite->setColor(Color3B(red, green, blue));
			sprite->setOpacity( number == 0 ? 0 : 255 );
		}
		if ( label ) {
			std::string str = StringUtils::format("%d", (int)pow(2, number));
			label->setString(str);
		}
	}
};

class CGameManager {
public:
	enum {
		ROW_NUM = 4,
		COLUMN_NUM = 4,
		TILE_NUM = ROW_NUM * COLUMN_NUM,
		
		TILE_SIZE = 100,
	};
	
	enum eDIRECTION {
		eDIRECTION_START = 0,
		eDIRECTION_LEFT = eDIRECTION_START,
		eDIRECTION_RIGHT,
		eDIRECTION_UP,
		eDIRECTION_DOWN,
		
		eDIRECTION_NUM,
		eDIRECTION_INVALID = -1,
	};
	
public:
	CGameManager( Layer* pParent, Point pos );
	~CGameManager();
	
	void Start();
	void Update();
	
	void menuCloseCallback(cocos2d::Ref* pSender);
	
	bool IsGameOver(){ return m_bGameOver; };
	void CheckGameOver();
	
	int	  getTileNum( vector<int> &tile );
	int	  getScore(){ return m_score; }
	int	  getMaxNumber();
	
public:
#if _MOVE_REQUEST
	void Move( eDIRECTION dir );
#else
	bool Move( eDIRECTION dir, bool bTest = false, int* score = NULL );
#endif
	
private:
#if _MOVE_REQUEST
	bool Move();
#endif	
	bool CanMove(int i );
	bool MoveLeft(bool bTest = false, int* score = NULL);
	bool MoveRight(bool bTest = false, int* score = NULL);
	bool MoveUp(bool bTest = false, int* score = NULL);
	bool MoveDown(bool bTest = false, int* score = NULL);
	
private:
	int		GenerateRandom();
	bool	IsInRange( int row, int column ){
		return ( 0 <= row && row < ROW_NUM) && ( 0 <= column && column < COLUMN_NUM);
	};
	int		CalcScore(int n){ return pow(2, n); };
	
private:
	STile		m_tile[ROW_NUM*COLUMN_NUM];
	eDIRECTION	m_dirReq;
	bool		m_bGameOver;
	int			m_score;
};

#endif /* _2048_hpp */
