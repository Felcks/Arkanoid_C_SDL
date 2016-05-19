#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT  480
#define SCREEN_BPP 32


SDL_Surface* screen = NULL;
SDL_Surface* rectangle = NULL;
SDL_Surface* ball = NULL;
SDL_Surface* block = NULL;

SDL_Event event;

#define PLAYER_WIDTH 139
#define PLAYER_HEIGHT 31
int playerX = SCREEN_WIDTH/2 - PLAYER_WIDTH/2;
int playerY = SCREEN_HEIGHT - PLAYER_HEIGHT;
int playerMoveX = 0;
int playerMoveY = 0;

#define BALL_SIZE  35
float ballX = SCREEN_WIDTH/2 - BALL_SIZE/2;
float ballY = SCREEN_HEIGHT - PLAYER_HEIGHT - BALL_SIZE;
float ballMoveX = 0.5;
float ballMoveY = -0.5;

#define BLOCK_SIZE_X 50
#define BLOCK_SIZE_Y 25
int matrizRow = 4;
int matrizColumn = 10;
int matriz[4][10]; 
int offSetX = 10;
int offSetY = 10;
int startX = 25;
int startY = 50;


int quit = 0;



SDL_Surface *load_image( char fileName[]){
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	loadedImage = IMG_Load ( fileName );

	if(loadedImage != NULL){
		optimizedImage = SDL_DisplayFormat( loadedImage );

		SDL_FreeSurface(loadedImage);

		//Uint32 colorKey = SDL_MapRGB ( optimizedImage->format, 255,255,255);
		//SDL_SetColorKey (optimizedImage, SDL_SRCCOLORKEY, colorKey);
	}

	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination){
	SDL_Rect offSet;

	offSet.x = x;
	offSet.y = y;

	SDL_BlitSurface( source, NULL, destination, &offSet);
}

void CleanUp(){
	SDL_FreeSurface ( rectangle );
	SDL_Quit();
}

void Lose(){
	ballX = SCREEN_WIDTH/2 - BALL_SIZE/2;
	ballY = SCREEN_HEIGHT - PLAYER_HEIGHT - BALL_SIZE;
	ballMoveX = 0.5;
	ballMoveY = -.5;

	playerX = SCREEN_WIDTH/2 - PLAYER_WIDTH/2;
	playerY = SCREEN_HEIGHT - PLAYER_HEIGHT;

	int i=0;
	int j=0;
	while(i<matrizRow){
		while(j<matrizColumn){
			matriz[i][j] = 0;
			j++;
		}
		j = 0;
		i++;
	}

}

void Collision_Ball_Screen(){
	if(ballX < 0)
		ballMoveX = -ballMoveX;
	if(ballX + BALL_SIZE >= SCREEN_WIDTH)
		ballMoveX = -ballMoveX;
	if(ballY < 0)
		ballMoveY = -ballMoveY;
	if(ballY + BALL_SIZE > SCREEN_HEIGHT)
		Lose();
		//ballMoveY = -ballMoveY; //Lose
}

void Collision_Player_Screen(){
	if(playerX < 0)
		playerX = 0;
	if(playerX + PLAYER_WIDTH > SCREEN_WIDTH)
		playerX = SCREEN_WIDTH - PLAYER_WIDTH;
		//ballMoveY = -ballMoveY; //Lose
}


void Collision_Ball_Player()
{
	if(ballX < playerX + PLAYER_WIDTH && ballX + BALL_SIZE > playerX &&
	   ballY < playerY + PLAYER_HEIGHT && ballY + BALL_SIZE > playerY){
		ballMoveY = -ballMoveY;
	}
}

int getPosBlockX(int j){
	return j * (BLOCK_SIZE_X + offSetX) + startX;
}

int getPosBlockY(int i){
	return  i * (BLOCK_SIZE_Y + offSetY) + startY;
}

void Collision_Ball_Matrix(){
	int i=0;
	int j=0;
	//j * (BLOCK_SIZE + 5) + 230, i * (BLOCK_SIZE + 5) + 50, block, screen);
	while(i<matrizRow){
		while(j<matrizColumn){
			if(matriz[i][j] != 1){
			if(ballX < getPosBlockX(j) + BLOCK_SIZE_X && ballX + BALL_SIZE > getPosBlockX(j) &&
				ballY < getPosBlockY(i) + BLOCK_SIZE_Y && ballY + BALL_SIZE > getPosBlockY(i)){
					matriz[i][j] = 1;
					ballMoveX *= -1;
					ballMoveY *= -1;
					i =50;
					j = 50;
					break;
				}
			}
			j++;
		}
		j = 0;
		i++;
	}
}

void StartComponents(){

	SDL_Init( SDL_INIT_EVERYTHING);

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	rectangle = load_image("Rectangle.jpg");
	ball = load_image("Ball2.png");
	block = load_image("Block2.png");

	SDL_Flip( screen );

}

void UpdateKeyBoard(){
		Uint8 *keystates = SDL_GetKeyState( NULL );

		if( keystates[ SDLK_LEFT ] )
        {
            playerX -= 1;
        }
        if( keystates[ SDLK_RIGHT ] )
        {
            playerX += 1;
        }
        if(keystates[ SDLK_ESCAPE ]){
        	quit = 1;
        }
			
}

void UpdatePlayer(){
	playerX = playerX + playerMoveX;
}

void UpdateBall(){
	ballX = ballX + ballMoveX;
	ballY = ballY + ballMoveY;
}


void DrawMatrix(){
	int i=0;
	int j=0;
	while(i<matrizRow){
		while(j<matrizColumn){
			if(matriz[i][j] != 1)
				apply_surface(getPosBlockX(j),getPosBlockY(i), block, screen);

			j++;
		}
		j = 0;
		i++;
	}
}


void Draw(){
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	apply_surface(playerX, playerY, rectangle, screen);
	apply_surface(ballX, ballY, ball, screen);
	DrawMatrix();
}




int main(void){

	StartComponents();
	
	while(quit == 0){
		Draw();
		UpdateKeyBoard();
		UpdatePlayer();
		UpdateBall();

		SDL_Flip( screen );
		Collision_Player_Screen();
		Collision_Ball_Screen();
		Collision_Ball_Player();
		Collision_Ball_Matrix();
	}

	CleanUp(); 
		
	return 0;
}
