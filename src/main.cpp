
/*

  CODENAME: PORTAL
  Wen-Hao Lue								 May 28th, 2007				
  ---------------------------------------------------------

  CODENAME: Portal is a single player puzzle game where you 
  control a player to complete the task of going to an exit 
  door.  You control a gun that can fire portals to 
  transport yourself to other places.  Players go through 
  levels which progress in difficulty.  Levels are grouped 
  in scenarios, and are divided in similarity.

*/

#include "main.h"

#include "Timer.h"
#include "Client.h"
#include "Game.h"
#include "GScnLoader.h"

#include "CObject.h"
#include "CDynamic.h"
#include "CMovable.h"
#include "CUnit.h"
#include "CPlayer.h"
#include "CTurret.h"

SDL_Surface* screen;
SDL_Event event;
Uint8* keys;
Uint32 counter;

bool done = false;

ImageHandler* imgHandler;
Game game;

void InitSDL();					// Initialize SDL functions
void QuitSDL();					// Free SDL functions
void InitData();				// Initialize data needed in the game
void LoadFonts();				// Load fonts
void LoadMusic();				// Load music and sound effects
void SetSDLScreen(int, int);	// Set screen dimensions
void GarbageCollect();			// Free unused objects added from DeleteObj()

int main(int argc, char* argv[])
{
	InitSDL();
	SetSDLScreen(800, 640);		// Set the screen to a 800x640 dimension (4:3)
	InitData();

	SDL_Surface* fpsImg = NULL;
	Client player;
	Timer fps;					// timer used to moderate frame rate
	int currTime = 0, prevTime = 0, frame = 0;	// variables used to determine frames
	game.setClient(&player);

	while(!done)
	{
		frame++;
		counter++;
		fps.Start();
		keys = SDL_GetKeyState(NULL);
		game.HandleDynamicEvents();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			// quit game when player presses "Escape" or clicks the X button on the window
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					done = true;
				break;
			}
			game.HandleEvents();
		}

		game.LifeCycle();
		game.Draw();
		if(SHOW_FPS) ApplySurface(fpsImg, 5, 5);	// Blit FPS counter in top-left part of screen

		SDL_Flip(screen);	// Page-flip the screen
		GarbageCollect();	// Delete unused items

		while(fps.getTicks() < (1000 / FPS))	// halt the frame until the game reaches maximum frame rate
			SDL_Delay(1);

		if(SHOW_FPS)
		{
			// calculate frame rate by counting how many frames appear in a second
			prevTime = currTime;		
			currTime = (int) time(NULL); 

			if(currTime - prevTime >= 1) 
			{
				// update and free the frame rate image displayed on the screen to prevent memory leaks
				SDL_FreeSurface(fpsImg);
				fpsImg = TTF_DrawText("fps", val2str(frame)+" fps", "black", 0);
				frame = 0;
			}
		}
	}

	QuitSDL();
	return 0;
}

void InitSDL()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);		// Initialize SDL with video and audio capability
	TTF_Init();										// Initialize SDL_ttf library functions
	SDL_WM_SetCaption(GAME_NAME.c_str(), NULL);		// Set window name
 	Mix_OpenAudio(SOUND_FREQ, SOUND_FORMAT, SOUND_CHANNELS, SOUND_CHUNKSIZE);	// Initialize SDL_mixer and sound capabilities
	srand(Uint32(time(NULL)));	// randomize
	cout << "CODENAME: Portal, created by Wen-Hao Lue" << endl;
	cout << "Version " << version << " - " << date << endl;
	cout << "------------------------------------------------------------" << endl;
}

void QuitSDL()
{
	// Free and quit SDL libraries
	SDL_FreeSurface(screen);
	delete imgHandler;
	Mix_CloseAudio();	// Close SDL_mixer
	TTF_Quit();			// Close SDL_ttf
	SDL_Quit();			// Close SDL library
}

void InitData()
{
	// Load and display loading image if compiler defined
#ifdef SHOW_LOADING_MESSAGE
	SDL_Surface* loading = SDL_LoadBMP("rsc/images/loading.bmp");
	if(loading == NULL) return;
	boxRGBA(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0, 255);
	ApplySurface(loading, SCREEN_WIDTH/2 - loading->w/2, SCREEN_HEIGHT/2 - loading->h/2);
	SDL_Flip(screen);
#endif

	// Load images using ImageHandler, and load scenarios using GScnLoader
	imgHandler = new ImageHandler();
	new GScnLoader();
	LoadFonts();
	LoadMusic();
	game.Initialize();

}

void LoadFonts()
{
	// Loading fonts
	fonts["fps"]			= TTF_OpenFont("rsc/fonts/verdana.ttf", 12);
	fonts["btitle"]			= TTF_OpenFont("rsc/fonts/arialbd.ttf", 42);
	fonts["title"]			= TTF_OpenFont("rsc/fonts/arial.ttf", 42);
	fonts["text"]			= TTF_OpenFont("rsc/fonts/arial.ttf", 36);
	fonts["htext"]			= TTF_OpenFont("rsc/fonts/arial.ttf", 28);

	// Loading font colours
	fontColors["black"]		= getSDLColor(0x00, 0x00, 0x00);
	fontColors["gray"]		= getSDLColor(0x10, 0x10, 0x10);
	fontColors["white"]		= getSDLColor(0xFF, 0xFF, 0xFF);
}

void LoadMusic()
{
	// background music
	Mix_Music* bgmusic = NULL;
	bgmusic = Mix_LoadMUS((SOUND_DIRECTORY + "/cyril.mid").c_str());
	if(bgmusic)	Mix_PlayMusic(bgmusic, -1);

	// sound effecst
	sfx["fall"]			= Mix_LoadWAV((SOUND_DIRECTORY + "/fall.wav").c_str());
	sfx["gunshot"]		= Mix_LoadWAV((SOUND_DIRECTORY + "/gunshot.wav").c_str());
	sfx["hit"]			= Mix_LoadWAV((SOUND_DIRECTORY + "/hit.wav").c_str());
	sfx["spike"]		= Mix_LoadWAV((SOUND_DIRECTORY + "/damage.wav").c_str());
	sfx["shatter"]		= Mix_LoadWAV((SOUND_DIRECTORY + "/shatter.wav").c_str());
	sfx["break"]		= Mix_LoadWAV((SOUND_DIRECTORY + "/break.wav").c_str());
	sfx["portal_shot"]	= Mix_LoadWAV((SOUND_DIRECTORY + "/portalshot.wav").c_str());
	sfx["portal_hit"]	= Mix_LoadWAV((SOUND_DIRECTORY + "/portalhit.wav").c_str());
}

void SetSDLScreen(int width, int height)
{
	if(screen) SDL_FreeSurface(screen);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_DOUBLEBUF|SDL_HWSURFACE);
}

void GarbageCollect()
{
	// delete all objects in the deletion vector
	if(!deletion.size()) return;
	vector<CObject*>::iterator f;
	for(f=deletion.begin(); f!=deletion.end(); ++f)
		delete *f;
	deletion.clear();
}
