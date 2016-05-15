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

#define BALL_SIZE  48
int ballX = SCREEN_WIDTH/2 - BALL_SIZE/2;
int ballY = SCREEN_HEIGHT - PLAYER_HEIGHT - BALL_SIZE;
int ballMoveX = 1;
int ballMoveY = -1;

#define BLOCK_SIZE 32
int matrizRow = 4;
int matrizColumn = 5;



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

void Collision_Ball_Screen(){
	if(ballX < 0)
		ballMoveX = -ballMoveX;
	if(ballX + BALL_SIZE >= SCREEN_WIDTH)
		ballMoveX = -ballMoveX;
	if(ballY < 0)
		ballMoveY = -ballMoveY;
	if(ballY + BALL_SIZE > SCREEN_HEIGHT)
		ballMoveY = -ballMoveY; //Lose
}

void Collision_Ball_Player()
{
	if(ballX < playerX + PLAYER_WIDTH && ballX + BALL_SIZE > playerX &&
	   ballY < playerY + PLAYER_HEIGHT && ballY + BALL_SIZE > playerY){
		ballMoveY = -ballMoveY;
	}
}

void StartComponents(){

		
}

int main(void){

	int quit = 0;

	SDL_Init( SDL_INIT_EVERYTHING);

	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	rectangle = load_image("Rectangle.jpg");
	ball = load_image("Ball.png");
	block = load_image("Block.png");

	apply_surface(playerX, playerY, rectangle, screen);
	apply_surface(ballX, ballY, ball, screen);

	int i=0;
	int j=0;
	while(i<matrizRow){
		while(j<matrizColumn){
			apply_surface(i * BLOCK_SIZE, j * BLOCK_SIZE, block, screen);
			j++;
		}
		j = 0;
		i++;
	}

	SDL_Flip( screen );


	while(quit == 0){
		while( SDL_PollEvent(  &event ) != 0){

			switch(event.type){
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDL_QUIT:
							quit = 1;
							break;
						case SDLK_ESCAPE:
							 quit = 1;
							 break;
						case SDLK_LEFT:
							 playerMoveX = -1;
							 continue; 
						case SDLK_RIGHT:
							 playerMoveX = 1;
							 break; 	
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym){
						case SDLK_LEFT:
							//playerMoveX = 0;
							 break;
						case SDLK_RIGHT:
							//playerMoveX = 0;
							 break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		}

		playerX = playerX + playerMoveX;
		ballX = ballX + ballMoveX;
		ballY = ballY + ballMoveY;

		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
		apply_surface(playerX, playerY, rectangle, screen);
		apply_surface(ballX, ballY, ball, screen);
		

		int i=0;
	int j=0;
	while(i<matrizRow){
		while(j<matrizColumn){
			apply_surface(j * (BLOCK_SIZE + 5) + 230, i * (BLOCK_SIZE + 5) + 50, block, screen);
			j++;
		}
		j = 0;
		i++;
	}


		SDL_Flip( screen );
		Collision_Ball_Screen();
		Collision_Ball_Player();

	
	}

	CleanUp();
		
	return 0;
}
