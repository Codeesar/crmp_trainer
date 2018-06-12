/*
		AUTHOR: SlonoBoyko
		LiteHack.Ru
		21.05.2018
*/

#include <menu.h>
#include "main.h"

struct cheats				menu;
struct interface_colors		menu_colors;
 
namespace ImGui
{
	void RenderWindowBackground(int x, int y, int size_x, int size_y, int alpha)
	{
		D3DCOLOR first_color = D3DCOLOR_ARGB(ini.color.bgnd_first.alpha, ini.color.bgnd_first.red, ini.color.bgnd_first.green, ini.color.bgnd_first.blue);
		D3DCOLOR second_color = D3DCOLOR_ARGB(ini.color.bgnd_second.alpha, ini.color.bgnd_second.red, ini.color.bgnd_second.green, ini.color.bgnd_second.blue);
		if (alpha) {
			first_color = D3DCOLOR_ARGB(alpha, ini.color.bgnd_first.red, ini.color.bgnd_first.green, ini.color.bgnd_first.blue);
			second_color = D3DCOLOR_ARGB(alpha, ini.color.bgnd_second.red, ini.color.bgnd_second.green, ini.color.bgnd_second.blue);
		}
		pD3DHook->pRender->D3DGradient(x, y, size_x, size_y, first_color, second_color);
	}

	void SetupColors()
	{
		ImVec4 *colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = menu_colors.colorBgndGlobal;
		colors[ImGuiCol_ChildWindowBg] = menu_colors.colorBgndGlobal;
		colors[ImGuiCol_PopupBg] = menu_colors.colorBgndGlobal;

		colors[ImGuiCol_Separator] = menu_colors.colorElements;
		colors[ImGuiCol_Border] = menu_colors.colorElements;
		colors[ImGuiCol_ScrollbarGrab] = menu_colors.colorElements;
		colors[ImGuiCol_SliderGrab] = menu_colors.colorElements;
		colors[ImGuiCol_Button] = menu_colors.colorElements;
		colors[ImGuiCol_Header] = menu_colors.colorElements;
		colors[ImGuiCol_ResizeGrip] = menu_colors.colorElements;
		colors[ImGuiCol_TextSelectedBg] = menu_colors.colorElements;

		colors[ImGuiCol_FrameBg] = menu_colors.colorBgndElements;
		colors[ImGuiCol_ComboBg] = menu_colors.colorBgndElements;

		colors[ImGuiCol_FrameBgHovered] = menu_colors.colorHovered;
		colors[ImGuiCol_ScrollbarGrabHovered] = menu_colors.colorHovered;
		colors[ImGuiCol_ButtonHovered] = menu_colors.colorHovered;
		colors[ImGuiCol_HeaderHovered] = menu_colors.colorHovered;
		colors[ImGuiCol_ResizeGripHovered] = menu_colors.colorHovered;
		colors[ImGuiCol_CloseButtonHovered] = menu_colors.colorHovered;

		colors[ImGuiCol_FrameBgActive] = menu_colors.colorActive;
		colors[ImGuiCol_ScrollbarGrabActive] = menu_colors.colorActive;
		colors[ImGuiCol_SliderGrabActive] = menu_colors.colorActive;
		colors[ImGuiCol_ButtonActive] = menu_colors.colorActive;
		colors[ImGuiCol_HeaderActive] = menu_colors.colorActive;
		colors[ImGuiCol_ResizeGripActive] = menu_colors.colorActive;
		colors[ImGuiCol_CloseButtonActive] = menu_colors.colorActive;
		colors[ImGuiCol_CheckMark] = menu_colors.colorActive;

		colors[ImGuiCol_Text] = menu_colors.colorText;
		colors[ImGuiCol_TitleBg] = menu_colors.colorTitle;
		colors[ImGuiCol_TitleBgActive] = menu_colors.colorTitle;

		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	};
};

void ImGui_Teleport()
{ 
	if (*(int *)0xBA6774 != 0)
	{
		float coords[3];
		if (gta_map_marker_get(coords)) {
			pCRMP->getChat()->addMessageToChat(COLOR_MSG_SUCCESS, 
				"�� ���� ��������������� �� ����������: %0.2f %0.2f %0.2f",
				coords[0], coords[1], coords[2]);
			gta_teleport(coords);
		}
	} else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "������! ��������� ����� �� �����.");
}

void ImGui_ObjectDump()
{
	FILE *file; SYSTEMTIME time;
	GetLocalTime(&time);

	char filename[MAX_PATH];
	sprintf(filename, "%s\\CRMP_Objects.txt", szWorkingDirectory);

	if (file = fopen(filename, "a")) {
		fprintf(file, "\n\n\nPlayer: %s[%d]\n", pCRMP->getPlayers()->strLocalPlayerName.c_str(), pCRMP->getPlayers()->sLocalPlayerID);
		fprintf(file, "Server: %s [%s:%d]\n", pCRMP->getInfo()->szHostname, pCRMP->getInfo()->szIP, pCRMP->getInfo()->ulPort);
		fprintf(file, "Date: %0.2d.%0.2d.%0.2d Time: %0.2d:%0.2d:%0.2d\n", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
		fprintf(file, "Position: %.2f, %.2f, %.2f\n\n\n", pPedSelf->GetPosition()->fX, pPedSelf->GetPosition()->fY, pPedSelf->GetPosition()->fZ);

		for (int i = 0; i < SAMP_MAX_OBJECTS; i++) {
			if (pCRMP->getObjects()->iIsListed[i] != 1) continue;
			if (pCRMP->getObjects()->object[i] == NULL) continue;
			if (vect3_near_zero(pCRMP->getObjects()->object[i]->fPos)) continue;
			stObject *pObject = pCRMP->getObjects()->object[i];
			fprintf(file, "CreateObject(%d, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f);\n", pObject->pGTA_Entity->model_alt_id,
				pObject->fPos[0], pObject->fPos[1], pObject->fPos[2], pObject->fRot[0], pObject->fRot[1], pObject->fRot[2]);
		}
		fclose(file);
	}
	return pCRMP->getChat()->addMessageToChat(COLOR_MSG_SUCCESS, "��� ������� ������� ������������ � ���� {FF9900}CRMP_Objects.txt");
}

void ImGui_Suicide()
{
	pPedSelf->SetHealth(0.f);
	pPedSelf->DetachPedFromEntity();
	pCRMP->getPlayers()->pLocalPlayer->sendDeath();
}

void ImGui_Unfrese()
{
	pCRMP->getMisc()->toggleSAMPCursor(true, CURSORMODE_NO_LOCKING);

//	pCRMP->getPlayers()->pLocalPlayer->toggleControllable(true);
	pPedSelf->SetIsLocked(false);
	GTAfunc_TogglePlayerControllable(false);

	if (trainer.state == CHEAT_STATE_ACTOR)
		GTAfunc_DisembarkInstantly();

	pGame->GetCamera()->RestoreWithJumpCut();
	pCRMP->getMisc()->toggleSAMPCursor(true);
}

void ImGui_RepairVehicle()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			GTAfunc_RepairVehicle(vinfo);
			vinfo->m_nVehicleFlags.bIsDamaged = false;
		}
	} else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "������! � ��� ��� ����������.");
}

void ImGui_ToggleEngine()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			vinfo->m_nVehicleFlags.bEngineOn ^= true;
			int iVehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vinfo);
			if (!pCRMP->getVehicles()->isBadVehicle(iVehicleID))
				pCRMP->getVehicles()->pSAMP_Vehicle[iVehicleID]->iIsMotorOn = vinfo->m_nVehicleFlags.bEngineOn;
		}
	} else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "������! � ��� ��� ����������.");

}

void ImGui_ToggleLight()
{
	if (trainer.state == CHEAT_STATE_VEHICLE) {
		vehicle_info *vinfo = vehicle_info_get(VEHICLE_SELF);
		if (vinfo != NULL) {
			vinfo->m_nVehicleFlags.bLightsOn ^= true;
			int VehicleID = pCRMP->getVehicles()->getSAMPVehicleID(vinfo);
			if (!pCRMP->getVehicles()->isBadVehicle(VehicleID))
				pCRMP->getVehicles()->pSAMP_Vehicle[VehicleID]->iIsLightsOn = vinfo->m_nVehicleFlags.bLightsOn;
		}
	} else pCRMP->getChat()->addMessageToChat(COLOR_MSG_ERROR, "������! � ��� ��� ����������.");
}

void ImGui_SpawnCars()
{
	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (pCRMP->getVehicles()->iIsListed[i] != 1)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr)
			continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->isSelfVehicle())
			continue;

		pCRMP->getRakClient()->SendVehicleDestroyed(i);
		Sleep(5);
	}

	return;
}

void ImGui_OpenVehicles()
{
	if (pCRMP->getVehicles() == nullptr) return;

	for (int i = 0; i < SAMP_MAX_VEHICLES; i++)
	{
		if (pCRMP->getVehicles()->iIsListed[i] != 1) continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i] == nullptr) continue;
		if (pCRMP->getVehicles()->pSAMP_Vehicle[i]->pGTA_Vehicle == nullptr) continue;
		if (i == pCRMP->getPlayers()->pLocalPlayer->sCurrentVehicleID) continue;

		pCRMP->getVehicles()->pSAMP_Vehicle[i]->setDoorState(false);
	}
	return;
}

void ImGui_Update_Window()
{
	if (gta_menu_active() || !trainer.update_available) return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 3.0f));

	if (trainer.update_available && !ImGui::IsPopupOpen(u8"����������")) {
		pCRMP->getMisc()->toggleSAMPCursor(trainer.imcursor = true);
		ImGui::OpenPopup(u8"����������");
	}
	if (ImGui::BeginPopupModal(u8"����������", &trainer.update_available, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::RenderWindowBackground(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"��������!");
		ImGui::Text(u8"����� ����� ���������� ������ ����.");
		ImGui::Text(u8"� ����� ���������� ��� ���� ����� �������.");
		ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
		ImGui::Text(u8"������� �������� �������?");
		if (ImGui::Button(u8"��������", ImVec2(130.0f, 0.0f))) {
			pCRMP->getMisc()->toggleSAMPCursor(trainer.imcursor = false);
			HWND hWnd = pD3DHook->pPresentParam->hDeviceWindow;
			SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			HMODULE hLibrary = LoadLibrary("shell32.dll");
			uint32_t ulBase = (uint32_t)GetProcAddress(hLibrary, "ShellExecuteA");
			func::stdcall(ulBase, hWnd, "open", PATH_TO_OFFICIAL_THREAD, 0, 0, SW_RESTORE);
			trainer.update_available = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"������", ImVec2(130.0f, 0.0f))) {
			pCRMP->getMisc()->toggleSAMPCursor(trainer.imcursor = false);
			trainer.update_available = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::Spacing(); 
		ImGui::Spacing();
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar(5);
}

void ImGui_Menu_Draw()
{
	ImGui_Update_Window();

	if (gta_menu_active() || !menu.active) return;
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 3.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.1f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 10.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(180.0f, 250.0f));
	ImGui::SetNextWindowSize(ImVec2(240.0f, 350.0f), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(300.0f, 350.0f), ImGuiCond_Once);

	static bool has_changed = false;

	if (ImGui::Begin(u8"CRMP Trainer by SlonoBoyko", &menu.active, ImGuiWindowFlags_NoCollapse/* | ImGuiWindowFlags_ShowBorders*/))
	{
		ImGui::RenderWindowBackground(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

		float fItemWidth = ImGui::GetWindowWidth() - 10.0f;
		if (ImGui::GetScrollMaxY() > 5.0f)
			fItemWidth -= ImGui::GetStyle().ScrollbarSize;

		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::CollapsingHeader(u8"�������"))
		{
			ImGui::Spacing();

			if (ImGui::Button(u8"���������", ImVec2(fItemWidth, 15.f))) menu.reconnect = true;
			ImGui::PushItemWidth(fItemWidth);
			if (ImGui::SliderInt(u8"�������� ����������", &ini.reconnect_delay, 0, 25, u8"��������: %.0f �.")) 
				has_changed = file::ini::set("reconnect_delay", ini.reconnect_delay);
			if (ImGui::Checkbox(u8"������� �������", &menu.fast_connect))
				has_changed = file::ini::set("enable_fast_connect", menu.fast_connect);		
			if(ImGui::Checkbox(u8"�������������", &menu.autoreconnect))
				has_changed = file::ini::set("enable_auto_reconnect", menu.autoreconnect);			
			if (ImGui::Checkbox(u8"���������", &menu.autologin))
				has_changed = file::ini::set("enable_autologin", menu.autologin);
			if (ImGui::InputText(u8"������", ini.autologin_pass, 32, ImGuiInputTextFlags_Password))
				has_changed = file::ini::set("autologin_pass", ini.autologin_pass);
			if (ImGui::Button(u8"������������", ImVec2(fItemWidth, 15.f)))
				pCRMP->getPlayers()->pLocalPlayer->sendSpawn();

			ImGui::Separator();

			if (ImGui::Button(u8"�������� �� �����", ImVec2(fItemWidth, 15.f))) ImGui_Teleport();

			ImGui::Separator();

			ImGui::Checkbox(u8"��������� �������", &menu.render.player);
			ImGui::Checkbox(u8"��������� �����������", &menu.render.vehicle);
			ImGui::Checkbox(u8"��������� �������", &menu.render.pickup);
			ImGui::Checkbox(u8"��������� ��������", &menu.render.object);
			ImGui::Checkbox(u8"��������� 3D �������", &menu.render.labels);

			ImGui::Separator();

			if (ImGui::Checkbox(u8"����������� �����", &menu.improved_overview))
				has_changed = file::ini::set("enable_improved_overview", menu.improved_overview);
			if (ImGui::SliderFloat(u8"���� ������", &ini.normal_fov, 50.0f, 140.0f, u8"����: %.0f �."))
				has_changed = file::ini::set("normal_fov", ini.normal_fov);
			
			ImGui::PopItemWidth();

			ImGui::Separator();

			if (ImGui::Checkbox(u8"������ ��������", &menu.clear_screen)) 
				has_changed = file::ini::set("enable_clear_screen", menu.clear_screen);
			if (ImGui::Checkbox(u8"�� ������� � ����", &menu.chat_ids)) 
				has_changed = file::ini::set("enable_chat_ids", menu.chat_ids);
			if (ImGui::Checkbox(u8"������ � ��������� ������", &menu.aafk)) 
				has_changed = file::ini::set("enable_anti_afk", menu.aafk);
	//		ImGui::Checkbox(u8"������ ������", &menu.dhider);

			ImGui::Separator();
			if (ImGui::TreeNode(u8"������"))
			{
				ImGui::Separator();
				ImGui::Checkbox(u8"�������� ������", &menu.flooder);
				ImGui::Separator();
				ImGui::PushItemWidth(fItemWidth - ImGui::GetStyle().IndentSpacing);
				if (ImGui::SliderFloat(u8"�������� �������", &ini.flood_delay, 0.1f, 20.f, u8"��������: %.1f �.", 1.1f))
					has_changed = file::ini::set("flood_delay", ini.flood_delay);

				ImGui::Separator();
			
				
				ini.flood_string.resize(64);
				vector<string> &vec = ini.flood_string; char data[288];
				for (size_t i = 0; i < ini.flood_string.size(); i++)
				{
					if (i > 0 && vec[i].empty() && vec[i - 1].empty())
						continue;
						
					strcpy_s(data, cp1251_to_utf8(vec[i]).data());
					string strName = u8"������: " + std::to_string(i);
					if (ImGui::InputText(strName.c_str(), data, 144)) {
						vec[i] = utf8_to_cp1251(data);
					}
					
				}
				ImGui::Separator();
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			
			ImGui::Spacing();
		}
		
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(u8"�����������"))
		{
			ImGui::Separator();
			ImGui::Checkbox(u8"���� ���������", &menu.invisible);
			ImGui::Checkbox(u8"���������� ���������", &menu.uinvisible);
			ImGui::Separator();
			ImGui::Checkbox(u8"������� �������", &menu.play_col);
			ImGui::Checkbox(u8"������� �����������", &menu.veh_col);
			ImGui::Checkbox(u8"������� ��������", &menu.obj_col);
			ImGui::Separator();
			ImGui::Checkbox(u8"�����������", &menu.pizdarvanka);
			ImGui::Checkbox(u8"������", &menu.slapper);
			ImGui::Separator();
			ImGui::Checkbox(u8"������ �� �����", &menu.skin_aim);
			ImGui::Checkbox(u8"������ �� �����", &menu.ping_aim);
			ImGui::Checkbox(u8"�������", &menu.autoshot);
			ImGui::Checkbox(u8"����������� ����", &menu.nospread);
			ImGui::Checkbox(u8"�������� ��� �����������", &menu.noreload);
			ImGui::Checkbox(u8"����� ������", &menu.skillmaster);
			ImGui::Checkbox(u8"���� +� ���", &menu.pluscbug);
			ImGui::Checkbox(u8"��������", &menu.antistun);
			ImGui::Separator();
			if (ImGui::Button(u8"���� ��������", ImVec2(fItemWidth, 15.f))) ImGui_ObjectDump();
			ImGui::Separator();
		}

		ImGui::Spacing();
		if (ImGui::CollapsingHeader(u8"�����"))
		{
			ImGui::Separator();
			if (ImGui::Button(u8"������������ ��������", ImVec2(fItemWidth, 15.f))) pPedSelf->SetHealth(100.0f);
			if (ImGui::Button(u8"������������ �����", ImVec2(fItemWidth, 15.f))) pPedSelf->SetArmor(100.0f);
			ImGui::Separator();
			ImGui::Checkbox(u8"����������", &menu.good_mode);
			if (ImGui::Button(u8"������������", ImVec2(fItemWidth, 15.f))) ImGui_Suicide();
			if (ImGui::Button(u8"����������", ImVec2(fItemWidth, 15.f))) ImGui_Unfrese();
			
			if (ImGui::Checkbox(u8"��������", &menu.antifreeze)) 
				has_changed = file::ini::set("enable_antifreeze", menu.antifreeze);
			if (ImGui::Checkbox(u8"�������", &menu.banyhop)) 
				has_changed = file::ini::set("enable_banyhop", menu.banyhop);
			
			ImGui::PushItemWidth(fItemWidth);
			if (pCRMP->getInfo() && pCRMP->getInfo()->pSettings)
				ImGui::SliderFloat(u8"��������� �����", &pCRMP->getInfo()->pSettings->fNameTagsDistance, 1.0f, 500.0f, u8"��������� �����: %.0f �.");
			ImGui::Separator();
			ImGui::Checkbox(u8"��� �����", &menu.air_break);
						
			if (ImGui::SliderFloat(u8"�������� ��� ������", &ini.air_break_speed, 1.0f, 250.0f, u8"��������: %.1f ��/�"))
				has_changed = file::ini::set("air_break_speed", ini.air_break_speed);
			
			ImGui::Separator();
			if (ImGui::Button(u8"����������� ��������", ImVec2(fItemWidth, 15.f))) ImGui::OpenPopup("actions");
			if (ImGui::BeginPopup("actions")) {
				int current, action = pCRMP->getPlayers()->pLocalPlayer->getSpecialAction();
				for (current = 0; current < COUNT_SPECIAL_ACTION; current++)
					if (ImSpecialAction[current].action_id == action) break;
				for (int i = 0; i < COUNT_SPECIAL_ACTION; i++)
					if (ImGui::Selectable(ImSpecialAction[i].action_name, current == i))
						pCRMP->getPlayers()->pLocalPlayer->setSpecialAction(ImSpecialAction[i].action_id);
				ImGui::EndPopup();
			}
			ImGui::PopItemWidth();
			ImGui::Separator();

		}

		ImGui::Spacing();
		if (ImGui::CollapsingHeader(u8"����������"))
		{
			ImGui::Separator();
			if (ImGui::Button(u8"������� ����������", ImVec2(fItemWidth, 15.f))) ImGui_RepairVehicle();
			ImGui::Checkbox(u8"����������", &menu.good_mode);
			ImGui::Separator();
			ImGui::Checkbox(u8"������", &menu.carshot);

			ImGui::PushItemWidth(fItemWidth);
			if (ImGui::SliderFloat(u8"�������� �������", &ini.carshot_speed, 1.0f, 200.0f, u8"�������� �������: %.1f ��/�"))
				has_changed = file::ini::set("carshot_speed", ini.carshot_speed);

			ImGui::Separator();
			if (ImGui::SliderFloat(u8"�������� ��������", &ini.max_speed, 1.0f, 200.0f, u8"�������� ��������: %.1f ��/�"))
				has_changed = file::ini::set("speedhack_speed", ini.max_speed);
			if (ImGui::SliderFloat(u8"������ ��������", &ini.acceleration, 1.0f, 50.0f, u8"������ ��������: %.1f �/�", 1.1f))
				has_changed = file::ini::set("speedhack_accel", ini.acceleration);
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::Checkbox(u8"�������� ��������", &menu.cargun);
			if (ImGui::Button(u8"���������� ��� ������", ImVec2(fItemWidth, 15.f))) ImGui_SpawnCars();
			ImGui::Separator();
			
			if(ImGui::Checkbox(u8"�� ������ � �����", &menu.bike_fall)) 
				has_changed = file::ini::set("enable_anti_bike_fall", menu.bottom_bar);
			
			ImGui::Checkbox(u8"�� ��������� �������� ����", &menu.noeject);
			ImGui::Checkbox(u8"�� ��������� ����������", &menu.always_engine_on);
			
			if (ImGui::Checkbox(u8"��������� ������", &menu.keep_trailer)) 
				has_changed = file::ini::set("enable_keep_trailer", menu.keep_trailer);
			
			if (ImGui::Button(u8"������� ���������", ImVec2(fItemWidth, 15.f))) ImGui_ToggleEngine();
			if (ImGui::Button(u8"�������� ����", ImVec2(fItemWidth, 15.f))) ImGui_ToggleLight();
			if (ImGui::Button(u8"������� ��� ������", ImVec2(fItemWidth, 15.f))) ImGui_OpenVehicles();
			ImGui::Separator();
			ImGui::Checkbox(u8"��������� ������", &menu.pulsewheels);
			ImGui::Checkbox(u8"��������� �����", &menu.pulsecolor);
			ImGui::Separator();
		}
		
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(u8"�����"))
		{
			ImGui::Separator();

			auto toggle_all = [](uint8_t type) {
				static bool state[5] = { false, false, false, false, false };
				for (size_t i = 0; i < ini.patch_cnt; i++) {
					if (ini.patch[i].type == type)
						ini.patch[i].enabled = !state[type];
				} state[type] ^= true;
			};

			for (int i = 1; i <= 4; i++)
			{
				static int iTreeId[4] = { false, false, false, false };
				if (ImGui::TreeNode(&iTreeId[i - 1], u8"%s %s", i % 2 ? u8"��������" : u8"���������", i > 2 ? u8"������" : u8"RPC"))
				{
					if (ImGui::Button(u8"����������� ���", ImVec2(fItemWidth - ImGui::GetStyle().IndentSpacing, 15.f)))
						toggle_all(i);

					ImGui::Spacing();
					 
					for (size_t x = 0; x < ini.patch_cnt; x++)
					{
						if (ini.patch[x].id && ini.patch[x].name.length() && ini.patch[x].type == i) {
							string strName = to_string(ini.patch[x].id) + ". " + ini.patch[x].name;
							ImGui::Checkbox(strName.c_str(), &ini.patch[x].enabled);
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::Separator();
		}
		
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(u8"�������"))
		{
			ImGui::Separator();
			
			ImGui::PushItemWidth(fItemWidth);
			static string name_buf = pCRMP->getPlayers()->strLocalPlayerName;
			
			char szPlayerName[SAMP_MAX_PLAYER_NAME_LENGHT];
			strncpy_s(szPlayerName, name_buf.data(), 24);
			ImGui::InputText(u8"��� ������", szPlayerName, SAMP_MAX_PLAYER_NAME_LENGHT);
			ImGui::PopItemWidth();
			ImGui::Separator();
			for (size_t i = 0; i < ini.servers.size(); i++)
			{
				if (ini.servers.empty()) break;
				if (ImGui::Button(cp1251_to_utf8(ini.servers[i].strHostname).c_str(), ImVec2(fItemWidth, 15.f))) {
					pCRMP->getInfo()->changeServer(ini.servers[i].strIP.data(), ini.servers[i].ulPort);
					pCRMP->getPlayers()->setLocalPlayerName(szPlayerName);
					name_buf = szPlayerName;
					menu.reconnect = true;
				}
			}
			ImGui::Separator();
		}
		
		ImGui::Spacing();	
		if (ImGui::CollapsingHeader(u8"���������")) {
			ImGui::Separator();

			if (ImGui::Checkbox(u8"������ ���", &menu.bottom_bar))
				has_changed = file::ini::set("bottom_bar_enable", menu.bottom_bar);
			if (ImGui::Checkbox(u8"��������� Aero Win7", &menu.restore_aero))
				has_changed = file::ini::set("restore_windows_aero", menu.restore_aero);
			
			ImGui::Separator();

			ImGuiIO& io = ImGui::GetIO();
			ImGui::PushItemWidth(35.0f);
			int flags = ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_CharsDecimal | 
				ImGuiInputTextFlags_AutoSelectAll;
			char buffer[16];

			auto AddKeySettingsItem = [&](char *label, string fparam, uint32_t &key) {
				strcpy_s(buffer, to_string(key).data());
				ImGui::InputText(label, buffer, 8, flags);

				for (size_t i = 0; i < ARRAYSIZE(io.KeysDown); i++) {
					if (ImGui::IsItemActive() && ImGui::IsKeyPressed(i) 
						&& i != 0x10 && i != 0x11 && i != 0x12) {
						has_changed = file::ini::set(fparam, i);
						io.KeysDown[VK_RETURN] = true;
						return key = i;
					} else if (ImGui::IsKeyReleased(key)) {
						io.KeysDown[VK_RETURN] = false;
					}
				}
				return key;
			};
			
			ImGui::Text(u8"��������� ������");
			AddKeySettingsItem(u8"����", "key_enable_menu", ini.key.menu);
			AddKeySettingsItem(u8"������������", "key_dialoghider", ini.key.dialoghider);
			AddKeySettingsItem(u8"������������", "key_good_mode", ini.key.good_mode);
			AddKeySettingsItem(u8"��� �����", "key_air_break", ini.key.air_break);
			AddKeySettingsItem(u8"������", "key_suicide", ini.key.suicide);
			AddKeySettingsItem(u8"�����", "key_spawn", ini.key.spawn);
			AddKeySettingsItem(u8"���������", "key_reconnect", ini.key.reconnect);
			AddKeySettingsItem(u8"���� ���", "key_ping_aimbot", ini.key.ping_aimbot);
			AddKeySettingsItem(u8"���� ���", "key_skin_aimbot", ini.key.skin_aimbot);
			AddKeySettingsItem(u8"�������", "key_speedhack", ini.key.speedhack);
			AddKeySettingsItem(u8"�������", "key_repair_vehicle", ini.key.repair);
			AddKeySettingsItem(u8"����������", "key_unfreze", ini.key.unfreze);
			AddKeySettingsItem(u8"����� �� ����", "key_fast_exit", ini.key.fast_exit);
			AddKeySettingsItem(u8"������� ������", "key_turn_right", ini.key.turn_right);
			AddKeySettingsItem(u8"������� � ����", "key_turn_left", ini.key.turn_left);
			AddKeySettingsItem(u8"������� �����", "key_turn_back", ini.key.turn_back);
			AddKeySettingsItem(u8"��������� ������", "key_unflip", ini.key.unflip);
			ImGui::PopItemWidth();
			
			ImGui::Separator();
			
			flags = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;
			auto AddColorEdit = [flags](char *label, string fparam, struct menu_color &fcolor)
			{
				ImVec4 color(float(fcolor.red) / 255.0f, float(fcolor.green) / 255.0f, float(fcolor.blue) / 255.0f, float(fcolor.alpha) / 255.0f);

				if (ImGui::ColorEdit4(label, (float*)&color, flags)) {
					fcolor.red		= static_cast<uint8_t>(color.x * 255.0f);
					fcolor.green	= static_cast<uint8_t>(color.y * 255.0f);
					fcolor.blue		= static_cast<uint8_t>(color.z * 255.0f);
					fcolor.alpha	= static_cast<uint8_t>(color.w * 255.0f);
					
					string buf = to_string(fcolor.alpha) + " " + to_string(fcolor.red)
						+ " " + to_string(fcolor.green) + " " + to_string(fcolor.blue);
					
					has_changed = file::ini::set(fparam, buf.data());
				}
				return color;
			};
			
			AddColorEdit(u8"���� ���� ���� 1", "color_background_first", ini.color.bgnd_first);
			AddColorEdit(u8"���� ���� ���� 2", "color_background_second", ini.color.bgnd_second);
			menu_colors.colorBgndGlobal		= AddColorEdit(u8"���������� ���� ����", "color_background_global", ini.color.bgnd_global);
			menu_colors.colorBgndElements	= AddColorEdit(u8"���� ���� �����", "color_background_items", ini.color.bgnd_items);
			
			menu_colors.colorText			= AddColorEdit(u8"���� ������", "color_menu_text", ini.color.text);
			menu_colors.colorTitle			= AddColorEdit(u8"���� ���������", "color_menu_title", ini.color.title);
			menu_colors.colorElements		= AddColorEdit(u8"���� ���������", "color_menu_elements", ini.color.elements);
			menu_colors.colorSeparator		= AddColorEdit(u8"���� ������������", "color_menu_separators", ini.color.separators);

			menu_colors.colorActive			= AddColorEdit(u8"���� ��� ��������������", "color_menu_active", ini.color.active);
			menu_colors.colorHovered		= AddColorEdit(u8"���� ��� ���������", "color_menu_hovered", ini.color.hovered);

			if (has_changed) ImGui::SetupColors();

			ImGui::Separator();

			if (ImGui::Checkbox(u8"�������������� ��������", &trainer.auto_save))
				has_changed = file::ini::set("auto_save_settings", trainer.auto_save);
			ImGui::Separator();
			if (ImGui::Button(u8"�������� ���������", ImVec2(fItemWidth, 15.0f))) {
				has_changed = false;
				file::ini::clear();
				DeleteFile(szIniFileName);
				get_ini_file_settings();
				
			}
			
			ImGui::Separator();
		}

		ImGui::Spacing();

		if (trainer.auto_save && has_changed) 
			has_changed = !file::ini::write();

		ImGui::End();		
		ImGui::PopStyleVar(8);

	}
	if (!menu.active) pCRMP->getMisc()->toggleSAMPCursor(trainer.imcursor = false);
}
