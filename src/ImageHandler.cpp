
#include "ImageHandler.h"

ImageHandler::ImageHandler()
{
	LoadImages();	
}

ImageHandler::~ImageHandler()
{
	Clear();
}

void ImageHandler::LoadImages()
{
	// Global Image Loading class
	//						 Image State			File Name							Image Flags (Settings)
	//						 ------------			---------							----------------------
	Image* img_background = new Image("background", "/background/", SCREEN_WIDTH, SCREEN_HEIGHT);
	img_background->AddState("title",				"titlescreen.png",					0);
	img_background->AddState("helpfile_p0",			"helpscreen_p01.png",				0);
	img_background->AddState("helpfile_p1",			"helpscreen_p02.png",				0);
	img_background->AddState("helpfile_p2",			"helpscreen_p03.png",				0);
	img_background->AddState("helpfile_p3",			"helpscreen_p04.png",				0);
	img_background->AddState("helpfile_p4",			"helpscreen_p05.png",				0);
	img_background->AddState("helpfile_p5",			"helpscreen_p06.png",				0);
	img_background->AddState("level",				"levelselect.png",					0);
	img_background->AddState("scenario",			"sceneselect.png",					0);
	img_background->AddState("game",				"gamescreen.png",					0);
	ImportImage(img_background);

	Image* img_stickman = new Image("stickman", "/stickman/", 40, 40);
	img_stickman->AddState	("base",				"stickman_base.png",				IMGSET_USEANGLES);
	img_stickman->AddState	("base-right",			"stickman_base-right.png",			IMGSET_USEANGLES);
	img_stickman->AddState	("base-left",			"stickman_base-left.png",			IMGSET_USEANGLES);
	img_stickman->AddState	("jump",				"stickman_jump.png",				IMGSET_USEANGLES);
	img_stickman->AddState	("jump-right",			"stickman_jump-right.png",			IMGSET_USEANGLES);
	img_stickman->AddState	("jump-left",			"stickman_jump-left.png",			IMGSET_USEANGLES);
	img_stickman->AddState	("anchor",				"stickman_arm.png",					IMGSET_USEANGLES);
	img_stickman->AddState	("anchor-right",		"stickman_arm-right.png",			IMGSET_USEANGLES);
	img_stickman->AddState	("anchor-left",			"stickman_arm-left.png",			IMGSET_USEANGLES);
	ImportImage(img_stickman);

	Image* img_turret = new Image("turret", "/turret/", 40, 40);
	img_turret->AddState	("base",				"base.png",							IMGSET_USEANGLES);
	img_turret->AddState	("anchor",				"anchor.png",						IMGSET_USEANGLES);
	ImportImage(img_turret);

	Image* img_item = new Image("item", "/items/", 0, 0);
	img_item->AddState		("box",					"box.png",							IMGSET_USEANGLES);
	img_item->AddState		("medbox",				"bigbox.png",						IMGSET_USEANGLES);
	img_item->AddState		("bigbox",				"bigbox.png",						IMGSET_USEANGLES);
	img_item->AddState		("spikeball",			"spikeball.png",					IMGSET_USEANGLES);
	ImportImage(img_item);

	Image* img_terrain = new Image("tile", "/terrain/", TILE_WIDTH, TILE_HEIGHT);
	img_terrain->AddState	("door",				"door.png",							0);
	img_terrain->AddState	("doorexit",			"door_exit.png",					0);
	img_terrain->AddState	("wall",				"wall.png",							0);
	img_terrain->AddState	("bwall",				"bwall.png",						0);
	img_terrain->AddState	("dwall",				"darkwall.png",						0);
	img_terrain->AddState	("glass",				"glass.png",						0);
	img_terrain->AddState	("bglass",				"bglass.png",						0);
	img_terrain->AddState	("spike",				"spike.png",						0);
	img_terrain->AddState	("nspike",				"nspike.png",						0);
	ImportImage(img_terrain);

	Image* img_portal = new Image("portal", "/portal/", 96, 96);
	img_portal->AddState	("red-T",				"portal_redT.png",					0);
	img_portal->AddState	("red-B",				"portal_redB.png",					0);
	img_portal->AddState	("red-L",				"portal_redL.png",					0);
	img_portal->AddState	("red-R",				"portal_redR.png",					0);
	img_portal->AddState	("blue-T",				"portal_blueT.png",					0);
	img_portal->AddState	("blue-B",				"portal_blueB.png",					0);
	img_portal->AddState	("blue-L",				"portal_blueL.png",					0);
	img_portal->AddState	("blue-R",				"portal_blueR.png",					0);
	img_portal->AddState	("black-T",				"portal_blackT.png",				0);
	img_portal->AddState	("black-B",				"portal_blackB.png",				0);
	img_portal->AddState	("black-L",				"portal_blackL.png",				0);
	img_portal->AddState	("black-R",				"portal_blackR.png",				0);
	ImportImage(img_portal);

	Image* img_proj = new Image("projectile", "/projectile/", 16, 16);
	img_proj->AddState		("portal_red",			"projportal_red.png",				0);
	img_proj->AddState		("portal_blue",			"projportal_blue.png",				0);
	img_proj->AddState		("bullet",				"bullet.png",						0);
	ImportImage(img_proj);

	Image* img_hud = new Image("hud", "/hud/", 0, 0);
	img_hud->AddState		("panel",				"panel.png",						0);
	ImportImage(img_hud);

	Image* img_panel = new Image("panel", "/panel/", 0, 0);
	img_panel->AddState		("generic",				"generic.png",						0);
	img_panel->AddState		("prep",				"prep.png",							0);
	img_panel->AddState		("pause",				"pause.png",						0);
	img_panel->AddState		("flevel",				"flevel.png",						0);
	img_panel->AddState		("fscenario",			"fscenario.png",					0);
	img_panel->AddState		("star0",				"starr_00.png",						0);
	img_panel->AddState		("star1",				"starr_01.png",						0);
	img_panel->AddState		("star2",				"starr_02.png",						0);
	img_panel->AddState		("star3",				"starr_03.png",						0);
	img_panel->AddState		("star4",				"starr_04.png",						0);
	img_panel->AddState		("star5",				"starr_05.png",						0);
	img_panel->AddState		("star6",				"starr_06.png",						0);
	img_panel->AddState		("star7",				"starr_07.png",						0);
	img_panel->AddState		("star8",				"starr_08.png",						0);
	img_panel->AddState		("star9",				"starr_09.png",						0);
	img_panel->AddState		("star10",				"starr_10.png",						0);
	ImportImage(img_panel);

	Image* img_button = new Image("button", "/button/", 0, 0);
	img_button->AddState	("title_startgame",		"title_startgame.png",				0);
	img_button->AddState	("title_startgame-over","title_startgame_over.png",			0);
	img_button->AddState	("title_startgame-down","title_startgame_down.png",			0);
	img_button->AddState	("title_helpfile",		"title_helpfile.png",				0);
	img_button->AddState	("title_helpfile-over",	"title_helpfile_over.png",			0);
	img_button->AddState	("title_helpfile-down",	"title_helpfile_down.png",			0);
	img_button->AddState	("title_quit",			"title_quit.png",					0);
	img_button->AddState	("title_quit-over",		"title_quit_over.png",				0);
	img_button->AddState	("title_quit-down",		"title_quit_down.png",				0);

	img_button->AddState	("menu_start",			"menu_start.png",					0);
	img_button->AddState	("menu_start-over",		"menu_start_over.png",				0);
	img_button->AddState	("menu_start-down",		"menu_start_down.png",				0);
	img_button->AddState	("menu_enter",			"menu_enter.png",					0);
	img_button->AddState	("menu_enter-over",		"menu_enter_over.png",				0);
	img_button->AddState	("menu_enter-down",		"menu_enter_down.png",				0);
	img_button->AddState	("menu_back",			"menu_back.png",					0);
	img_button->AddState	("menu_back-over",		"menu_back_over.png",				0);
	img_button->AddState	("menu_back-down",		"menu_back_down.png",				0);
	img_button->AddState	("menu_retry",			"menu_retry.png",					0);
	img_button->AddState	("menu_retry-over",		"menu_retry_over.png",				0);
	img_button->AddState	("menu_retry-down",		"menu_retry_down.png",				0);
	img_button->AddState	("menu_end",			"menu_end.png",						0);
	img_button->AddState	("menu_end-over",		"menu_end_over.png",				0);
	img_button->AddState	("menu_end-down",		"menu_end_down.png",				0);
	img_button->AddState	("menu_quit",			"menu_quit.png",					0);
	img_button->AddState	("menu_quit-over",		"menu_quit_over.png",				0);
	img_button->AddState	("menu_quit-down",		"menu_quit_down.png",				0);
	img_button->AddState	("menu_nextlevel",		"menu_nextlevel.png",				0);
	img_button->AddState	("menu_nextlevel-over",	"menu_nextlevel_over.png",			0);
	img_button->AddState	("menu_nextlevel-down",	"menu_nextlevel_down.png",			0);
	img_button->AddState	("menu_mainmenu",		"menu_mainmenu.png",				0);
	img_button->AddState	("menu_mainmenu-over",	"menu_mainmenu_over.png",			0);
	img_button->AddState	("menu_mainmenu-down",	"menu_mainmenu_down.png",			0);
	img_button->AddState	("menu_quit",			"menu_quit.png",					0);
	img_button->AddState	("menu_quit-over",		"menu_quit_over.png",				0);
	img_button->AddState	("menu_quit-down",		"menu_quit_down.png",				0);

	img_button->AddState	("arrow_up",			"arrow_up.png",						0);
	img_button->AddState	("arrow_up-over",		"arrow_up_over.png",				0);
	img_button->AddState	("arrow_up-down",		"arrow_up_down.png",				0);
	img_button->AddState	("arrow_down",			"arrow_down.png",					0);
	img_button->AddState	("arrow_down-over",		"arrow_down_over.png",				0);
	img_button->AddState	("arrow_down-down",		"arrow_down_down.png",				0);
	img_button->AddState	("arrow_left",			"arrow_left.png",					0);
	img_button->AddState	("arrow_left-over",		"arrow_left_over.png",				0);
	img_button->AddState	("arrow_left-down",		"arrow_left_down.png",				0);
	img_button->AddState	("arrow_right",			"arrow_right.png",					0);
	img_button->AddState	("arrow_right-over",	"arrow_right_over.png",				0);
	img_button->AddState	("arrow_right-down",	"arrow_right_down.png",				0);
	ImportImage(img_button);

}

void ImageHandler::ImportImage(Image* image)
{
	// Set and initialize to portal map.  Let image handle its own states 
	// and further image importing (animation, autojoining tiles, angles, etc)
	images[image->getName()] = image;
}

void ImageHandler::Clear()
{
	// Free all images when game ends
	map<string, Image*>::iterator imgItr;
	for(imgItr = images.begin(); imgItr != images.end(); ++imgItr)
		delete imgItr->second;
}