#ifndef PAINTERENGINE_APPLICATION_H
#define PAINTERENGINE_APPLICATION_H

#include "PainterEngine_Startup.h"
#include "./architecture/PainterEngine_Console.h"
#include "./architecture/PainterEngine_MessageBox.h"
#include "platform/modules/px_audio.h"



typedef struct
{
	PX_SoundPlay soundplay;
	PX_Json ui_json;
	PX_UI ui;
	PX_FontModule fontmodule;
	PX_Runtime runtime;
	PX_MessageBox messagebox;
	
	PX_Object *root,*label_1000hz,*slider_1000hz,*label_1100hz,*slider_1100hz;
}PX_Application;

extern PX_Application App;

px_bool PX_ApplicationInitialize(PX_Application *App,px_int screen_Width,px_int screen_Height);
px_void PX_ApplicationUpdate(PX_Application *App,px_dword elpased);
px_void PX_ApplicationRender(PX_Application *App,px_dword elpased);
px_void PX_ApplicationPostEvent(PX_Application *App,PX_Object_Event e);

#endif
