#include "PainterEngine_Application.h"

PX_Application App;


px_int sampleIndex=0;
px_void PX_APP_UserRead(px_void *ptr,px_byte *pBuffer,px_int readSize)
{
	PX_Application *pApp=(PX_Application *)ptr;
	px_double amp1,amp2;
	px_int  readSampleCount=readSize/4;
	px_int i;
	px_short *pPCM=(px_short *)pBuffer;
	amp1=PX_Object_SliderBarGetValue(pApp->slider_1000hz);
	amp2=PX_Object_SliderBarGetValue(pApp->slider_1100hz);

	for (i=0;i<readSampleCount;i++)
	{
		px_double a=0;
		a+=amp1*PX_sind(1000*2*PX_PI*sampleIndex/44100);
		a+=amp2*PX_sind(1100*2*PX_PI*sampleIndex/44100);

		pPCM[i*2]=(px_short)a;
		pPCM[i*2+1]=(px_short)a;
		sampleIndex++;
	}
}

px_void PX_APP_OnSlider1000hzChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	px_char content[32];
	PX_Application *pApp=(PX_Application *)ptr;
	PX_sprintf1(content,sizeof(content),"1000HZ:%1",PX_STRINGFORMAT_INT(PX_Object_SliderBarGetValue(pApp->slider_1000hz)));
	PX_Object_LabelSetText(pApp->label_1000hz,content);
}


px_void PX_APP_OnSlider1100hzChanged(PX_Object *pObject,PX_Object_Event e,px_void *ptr)
{
	px_char content[32];
	PX_Application *pApp=(PX_Application *)ptr;
	PX_sprintf1(content,sizeof(content),"1100HZ:%1",PX_STRINGFORMAT_INT(PX_Object_SliderBarGetValue(pApp->slider_1100hz)));
	PX_Object_LabelSetText(pApp->label_1100hz,content);
}

px_bool PX_ApplicationInitialize(PX_Application *pApp,px_int screen_width,px_int screen_height)
{
	PX_memset(pApp,0,sizeof(PX_Application));
	PX_ApplicationInitializeDefault(&pApp->runtime, screen_width, screen_height);
	if(!PX_SoundPlayInitialize(&pApp->runtime.mp_game,&pApp->soundplay)) return PX_FALSE;
	if(!PX_AudioInitialize(&pApp->soundplay))return PX_FALSE;
	if(!PX_FontModuleInitialize(&pApp->runtime.mp_resources,&pApp->fontmodule))return PX_FALSE;
	if (!PX_LoadFontModuleFromFile(&pApp->fontmodule,"assets/font18.pxf")) return PX_FALSE;
	if(!PX_UIInitialize(&pApp->runtime.mp_game,&pApp->runtime.mp_ui,&pApp->ui,&pApp->fontmodule))return PX_FALSE;
	do 
	{
		if(!PX_JsonInitialize(&pApp->runtime.mp_resources,&pApp->ui_json)) return PX_FALSE;
		if(!PX_LoadJsonFromFile(&pApp->ui_json,"assets/ui_main.json"))return PX_FALSE;
		pApp->root=PX_UICreate(&pApp->ui,PX_NULL,&pApp->ui_json.rootValue,pApp->runtime.surface_width,pApp->runtime.surface_height);
		if (!pApp->root)
		{
			return PX_FALSE;
		}

	} while (0);


	pApp->label_1000hz=PX_UIGetObjectByID(&pApp->ui,"label_1000hz");
	pApp->slider_1000hz=PX_UIGetObjectByID(&pApp->ui,"slider_1000hz");
	pApp->label_1100hz=PX_UIGetObjectByID(&pApp->ui,"label_1100hz");
	pApp->slider_1100hz=PX_UIGetObjectByID(&pApp->ui,"slider_1100hz");

	PX_ObjectRegisterEvent(pApp->slider_1000hz,PX_OBJECT_EVENT_VALUECHANGED,PX_APP_OnSlider1000hzChanged,pApp);
	PX_ObjectRegisterEvent(pApp->slider_1100hz,PX_OBJECT_EVENT_VALUECHANGED,PX_APP_OnSlider1100hzChanged,pApp);

	PX_SoundPlaySetUserRead(&pApp->soundplay,PX_APP_UserRead,pApp);

	return PX_TRUE;
}

px_void PX_ApplicationUpdate(PX_Application *pApp,px_dword elpased)
{
	elpased>100?100:elpased;
	PX_ObjectUpdate(pApp->root,elpased);
	PX_MessageBoxUpdate(&pApp->messagebox,elpased);
}

px_void PX_ApplicationRender(PX_Application *pApp,px_dword elpased)
{
	px_surface *pRenderSurface=&pApp->runtime.RenderSurface;
	PX_RuntimeRenderClear(&pApp->runtime,PX_OBJECT_UI_DEFAULT_BACKGROUNDCOLOR);
	if (pApp->messagebox.show)
	{
		PX_MessageBoxRender(pRenderSurface,&pApp->messagebox,elpased);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	PX_ObjectRender(pRenderSurface,pApp->root,elpased);
}

px_void PX_ApplicationPostEvent(PX_Application *pApp,PX_Object_Event e)
{
	PX_ApplicationEventDefault(&pApp->runtime, e);
	if (e.Event==PX_OBJECT_EVENT_WINDOWRESIZE)
	{
		PX_UIUpdateObjectsPostions(&pApp->ui,pApp->root,&pApp->ui_json.rootValue,pApp->runtime.surface_width,pApp->runtime.surface_height);
	}
	if (pApp->messagebox.show)
	{
		PX_MessageBoxPostEvent(&pApp->messagebox,e);
		return;
	}
	PX_ObjectPostEvent(pApp->root,e);
}

