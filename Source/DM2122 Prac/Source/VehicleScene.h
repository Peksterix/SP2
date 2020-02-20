#ifndef VEHICLESCENE_H
#define VEHICLESCENE_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "Entity.h"
#include "Vehicle.h"
#include "Animate.h"

class VehicleScene : public Scene
{
	enum GAME_STATES
	{
		S_LOADOUT = 0,
		S_CUSTOMISE,
		S_CUTSCENE,
		S_FREECAM,
		S_TOTAL,
	};

	enum ANIMATION_VALUES
	{
		ANI_CAMERA_POSITION_X = 0,
		ANI_CAMERA_POSITION_Y,
		ANI_CAMERA_POSITION_Z,
		ANI_CAMERA_TARGET_X,
		ANI_CAMERA_TARGET_Y,
		ANI_CAMERA_TARGET_Z,

		ANI_VEHICLE_POSITION,
		ANI_VEHICLE_ROTATION_Y,
		ANI_VEHICLE_ROTATION_Z,
		ANI_WHEEL_ROTATION,

		ANI_SKYBOX_ROTATION,
		ANI_SHIP_ROTATION,
		ANI_SHIP_BRIDGE_ROTATION,
		ANI_SHIP_TEETH_POSITION,

		ANI_CUSTOMISE_CHASSIS,
		ANI_CUSTOMISE_WHEEL,
		ANI_CUSTOMISE_WEAPON,

		ANI_SELECTED_UI,
		ANI_BUFFER,

		ANI_TOTAL,
	};

	enum ANIMATION_STATES
	{
		ANIS_ANY = 0,
		ANIS_RING,
		ANIS_LOADOUT_TO_CUSTOMISE,
		ANIS_CUSTOMISE_TO_LOADOUT,
		ANIS_CUSTOMISE_PARTS,

		ANIS_TOTAL,
	};

	enum DEBUG_VALUES
	{
		DEBUG_LIGHT_NUM = 0,
		DEBUG_TOTAL,
	};

	enum MENU_SELECTION
	{
		MENU_CUSTOMISATION_Y = 0,
		MENU_CUSTOMISATION_X,
		MENU_LOADOUT,
		MENU_CONIRMATION,

		MENU_TOTAL,
	};

	enum UI_SHEET
	{
		UI_CHANGE = 0,
		UI_EMPTY,
		UI_BACK,
		UI_ESCAPE,
		UI_SELECTED,
		UI_TEMP = UI_SELECTED + 4,
		UI_TOTAL,
	};
	
	enum WINDOW_TYPES
	{
		WINDOW_NONE = 0,
		WINDOW_CONFIRM,
		WINDOW_NOTIFY,
		WINDOW_INPUT,

		WINDOW_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_BOUNDINGBOX,
		GEO_LIGHTSPHERE,

		GEO_WAREHOUSE_SHELL,
		GEO_WAREHOUSE_RING,
		GEO_WAREHOUSE_CENTRE,
		GEO_WAREHOUSE_BRIDGE,
		GEO_WAREHOUSE_TEETH_TOP,
		GEO_WAREHOUSE_TEETH_BOTTOM,
		GEO_SKYSPHERE,

		GEO_SHOWCASEFLOOR,

		GEO_TEXT,
		GEO_UI,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,
		U_LIGHT4_TYPE,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,

		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,
		U_LIGHT5_TYPE,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,

		U_LIGHT6_POSITION,
		U_LIGHT6_COLOR,
		U_LIGHT6_POWER,
		U_LIGHT6_KC,
		U_LIGHT6_KL,
		U_LIGHT6_KQ,
		U_LIGHT6_TYPE,
		U_LIGHT6_SPOTDIRECTION,
		U_LIGHT6_COSCUTOFF,
		U_LIGHT6_COSINNER,
		U_LIGHT6_EXPONENT,

		U_LIGHT7_POSITION,
		U_LIGHT7_COLOR,
		U_LIGHT7_POWER,
		U_LIGHT7_KC,
		U_LIGHT7_KL,
		U_LIGHT7_KQ,
		U_LIGHT7_TYPE,
		U_LIGHT7_SPOTDIRECTION,
		U_LIGHT7_COSCUTOFF,
		U_LIGHT7_COSINNER,
		U_LIGHT7_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];
	MS modelStack, viewStack, projectionStack;

	Light light[8];
	Camera camera;
	Animate animate;
	
	Chassis* custChassis[5];
	Wheel* custWheel[5];
	Weapon* custWeapon[5];
	Vehicle* vehicle[8];

	bool animation[ANIS_TOTAL],
		 showDebugInfo,
		 showBoundingBox;

	int	screenSizeX,
		screenSizeY,
		state,
		inWindow,
		debugValues[DEBUG_TOTAL],
		menuSelected[MENU_TOTAL],
		vehiclePartSelect[3];

	float	aniVal[ANI_TOTAL],
			bounceTime[10];
	
	std::string textWindow,
				textInput;

	void renderScene();
	void renderSkysphere(int size);
	
	void moveLight(double dt, int lightNum);
	void renderLightPos(int lightNum);

	void CalculateLights();
	void RenderMesh(Mesh* mesh, bool enableLight, float BBSize = 0);
	void RenderText(Mesh* mesh, std::string text, Color color, int type = 0);
	void RenderSprite(Mesh* mesh, int frameCount, Color color = Color(1, 1, 1));
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, int type = 0);
	void RenderSpriteOnScreen(Mesh* mesh, int frameCount, float x, float y, float sizex, float sizey, Color color = Color(1, 1, 1));
	void CalculateFPS();

public:
	VehicleScene();
	~VehicleScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif