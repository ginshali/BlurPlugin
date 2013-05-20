/********************************************************************************
 Copyright (C) 2013 Ginshali.info

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/

#include "stdafx.h"
#include "BlurEffectSource.h"
#include "resource.h"

extern "C" __declspec(dllexport) bool LoadPlugin();
extern "C" __declspec(dllexport) void UnloadPlugin();
extern "C" __declspec(dllexport) CTSTR GetPluginName();
extern "C" __declspec(dllexport) CTSTR GetPluginDescription();

HINSTANCE hinstMain = NULL;

struct SourceConfig
{
	XElement* data;
	CTSTR name;
};

INT_PTR CALLBACK ConfigureDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
	{
	case WM_INITDIALOG:
		{
			SourceConfig* config = (SourceConfig*)lParam;
			XElement *data = config->data;
			SetWindowLongPtr(hwnd, DWLP_USER, (LONG_PTR)lParam);

			HWND hwHorizSlider = GetDlgItem(hwnd, IDC_HORIZ_SLIDER);
			HWND hwVertSlider = GetDlgItem(hwnd, IDC_VERT_SLIDER);
			HWND hwHorizValueLabel = GetDlgItem(hwnd, IDC_HORIZ_VALUE);
			HWND hwVertValueLabel = GetDlgItem(hwnd, IDC_VERT_VALUE);

			float horizValue = data->GetFloat(TEXT("horizValue"), 1.0f);
			SendMessage(hwHorizSlider, TBM_CLEARTICS, FALSE, 0);
			SendMessage(hwHorizSlider, TBM_SETRANGE, FALSE, MAKELPARAM(1, 100));
			SendMessage(hwHorizSlider, TBM_SETTIC, 0, 10);
			SendMessage(hwHorizSlider, TBM_SETPOS, TRUE, (int)(horizValue * 10));
			SetWindowText(hwHorizValueLabel, FormattedString(TEXT("%.01f"), horizValue));
			
			float vertValue = data->GetFloat(TEXT("vertValue"), 1.0f);
			SendMessage(hwVertSlider, TBM_CLEARTICS, FALSE, 0);
			SendMessage(hwVertSlider, TBM_SETRANGE, FALSE, MAKELPARAM(1, 100));
			SendMessage(hwVertSlider, TBM_SETTIC, 0, 10);
			SendMessage(hwVertSlider, TBM_SETPOS, TRUE, (int)(vertValue * 10));
			SetWindowText(hwVertValueLabel, FormattedString(TEXT("%.01f"), vertValue));

			return TRUE;
		}
	case WM_HSCROLL:
		{
			switch (GetDlgCtrlID((HWND)lParam))
			{
			case IDC_HORIZ_SLIDER:
				{
					HWND hwHorizSlider = GetDlgItem(hwnd, IDC_HORIZ_SLIDER);
					HWND hwHorizValueLabel = GetDlgItem(hwnd, IDC_HORIZ_VALUE);

					float horizValue = (float)SendMessage(hwHorizSlider, TBM_GETPOS, 0, 0) / 10.0f;
					SetWindowText(hwHorizValueLabel, FormattedString(TEXT("%.01f"), horizValue));

					SourceConfig* config = (SourceConfig*)GetWindowLongPtr(hwnd, DWLP_USER);
					ImageSource* source = API->GetSceneImageSource(config->name);
					source->SetFloat(TEXT("horizValue"), horizValue);
					break;
				}
			case IDC_VERT_SLIDER:
				{
					HWND hwVertSlider = GetDlgItem(hwnd, IDC_VERT_SLIDER);
					HWND hwVertValueLabel = GetDlgItem(hwnd, IDC_VERT_VALUE);

					float vertValue = (float)SendMessage(hwVertSlider, TBM_GETPOS, 0, 0) / 10.0f;
					SetWindowText(hwVertValueLabel, FormattedString(TEXT("%.01f"), vertValue));

					SourceConfig* config = (SourceConfig*)GetWindowLongPtr(hwnd, DWLP_USER);
					ImageSource* source = API->GetSceneImageSource(config->name);
					source->SetFloat(TEXT("vertValue"), vertValue);
					break;
				}
			}
		}
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
				{
					HWND hwHorizSlider = GetDlgItem(hwnd, IDC_HORIZ_SLIDER);
					HWND hwVertSlider = GetDlgItem(hwnd, IDC_VERT_SLIDER);
					float horizValue = (float)SendMessage(hwHorizSlider, TBM_GETPOS, 0, 0) / 10.0f;
					float vertValue = (float)SendMessage(hwVertSlider, TBM_GETPOS, 0, 0) / 10.0f;

					SourceConfig* config = (SourceConfig*)GetWindowLongPtr(hwnd, DWLP_USER);
					XElement* data = config->data;
					data->SetFloat(TEXT("horizValue"), horizValue);
					data->SetFloat(TEXT("vertValue"), vertValue);
					

					EndDialog(hwnd, LOWORD(wParam));
				}
				break;

			case IDCANCEL:
				{
					SourceConfig* config = (SourceConfig*)GetWindowLongPtr(hwnd, DWLP_USER);
					XElement* data = config->data;
					data->SetFloat(TEXT("horizValue"), data->GetFloat(TEXT("horizValue"), 1.0f));
					data->SetFloat(TEXT("vertValue"), data->GetFloat(TEXT("vertValue"), 1.0f));
					
					ImageSource* source = API->GetSceneImageSource(config->name);
					source->SetFloat(TEXT("horizValue"), data->GetFloat(TEXT("horizValue"), 1.0f));
					source->SetFloat(TEXT("vertValue"), data->GetFloat(TEXT("vertValue"), 1.0f));

					EndDialog(hwnd, LOWORD(wParam));
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			EndDialog(hwnd, IDCANCEL);
		}
		break;
	}

	return 0;
}

bool STDCALL ConfigureBlurEffectSource(XElement *element, bool bCreating)
{
	if (!element)
	{
		AppWarning(TEXT("ConfigureBlurEffectSource: NULL element"));
		return false;
	}

	SourceConfig config;
	XElement* data = element->GetElement(TEXT("data"));
	if (!data)
	{
		data = element->CreateElement(TEXT("data"));
	}
	config.data = data;
	config.name = element->GetName();

	if (DialogBoxParam(
			hinstMain, 
			MAKEINTRESOURCE(IDD_CONFIG), 
			API->GetMainWindow(), 
			ConfigureDialogProc, 
			(LPARAM)&config) == IDOK)
	{
        UINT width, height;
        API->GetBaseSize(width, height);
		element->SetInt(TEXT("cx"), width);
		element->SetInt(TEXT("cy"), height);
		return true;
	}

    return false;
}

ImageSource* STDCALL CreateBlurEffectSource(XElement *data)
{
    BlurEffectSource *source = new BlurEffectSource;
    if(!source->Init(data))
    {
        delete source;
        return NULL;
    }

    return source;
}

bool LoadPlugin()
{
    API->RegisterImageSourceClass(
		TEXT("BlurPlugin"), 
		TEXT("BlurPlugin"), 
		(OBSCREATEPROC)CreateBlurEffectSource, 
		(OBSCONFIGPROC)ConfigureBlurEffectSource);

    return true;
}

void UnloadPlugin()
{
}

CTSTR GetPluginName()
{
    return TEXT("BlurPlugin");
}

CTSTR GetPluginDescription()
{
    return TEXT("Plugin to apply blur effects for scenes.");
}

BOOL CALLBACK DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpBla)
{
    if(dwReason == DLL_PROCESS_ATTACH)
        hinstMain = hInst;

    return TRUE;
}
