#define NOMINMAX

#include <thread>

#include "Gui/GuiApp.h"
#include "imgui.h"

#include "imfilebrowser.h"

#include "Core/Core.h"
#include "Util/Environment.h"
#include "Util/Math.h"

namespace OperaThemeMaker
{
	static ImVec2 g_WindowDimensions = { 800, 700 };
	static const std::filesystem::path outputPath = std::filesystem::current_path().append("output");
	static bool processingTheme = false;

	void CreateTheme();

	namespace GUIComponents
	{
		static struct
		{
			int a = 1, b = 2, c = 3, d = 4;

			void A(int newValue)
			{
				if (b == newValue) std::swap(a, b);
				else if (c == newValue) std::swap(a, c);
				else if (d == newValue) std::swap(a, d);
			}

			void B(int newValue)
			{
				if (a == newValue) std::swap(b, a);
				else if (c == newValue) std::swap(b, c);
				else if (d == newValue) std::swap(b, d);
			}

			void C(int newValue)
			{
				if (b == newValue) std::swap(c, b);
				else if (a == newValue) std::swap(c, a);
				else if (d == newValue) std::swap(c, d);
			}

			void D(int newValue)
			{
				if (a == newValue) std::swap(d, a);
				else if (c == newValue) std::swap(d, c);
				else if (b == newValue) std::swap(d, b);
			}

		} g_FourIntCache;

		static ImVec4 fgColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
		static ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		static ImGui::FileBrowser fileDialog;
		static ImGui::FileBrowser fileDialogTheme(ImGuiFileBrowserFlags_SelectDirectory);
		static bool toPicture = true;

		static char themeName[512];
		static char authorName[512];
		static int version = 1;

		static char picturePath[512];
		static char videoPath[512];


		static OperaThemeMaker::MonoChrome monoChrome;
		static OperaThemeMaker::BoxBlur boxBlur;
		static OperaThemeMaker::Vignette vignette;
		static OperaThemeMaker::EdgeDetection edgeDetection;

		static bool chkBoxBlur = false;
		static bool chkVignette = false;
		static bool chkEdgeDetection = false;
		static bool chkMonochrome = false;

		static int boxBlurIntensity = 1;
		static int vignettePiDivisor = 4;

		static int videoEffectListSelectedItem;

		static bool chkToThemes = true;
		static char operaThemePath[512];
		static VideoEffects videoEffects;

		void MainConfig()
		{
			ImGui::Begin("Main Configuration");
			ImGui::InputText("Theme Name", themeName, 512);
			ImGui::InputText("Author", authorName, 512);
			if (ImGui::InputInt("Version", &version))
			{
				if (version < 1)
				{
					version = 1;
				}
			}
			if (ImGui::Button("...###BrowsePic"))
			{
				fileDialog.Open();
				toPicture = true;
			}
			ImGui::SameLine();
			ImGui::InputText("Picture Path", picturePath, 512);

			if (ImGui::Button("...###BrowseVid"))
			{
				fileDialog.Open();
				toPicture = false;
			}
			ImGui::SameLine();
			ImGui::InputText("Video Path (Optional)", videoPath, 512);


			fileDialog.Display();

			if (fileDialog.HasSelected())
			{
				if (toPicture)
					strcpy_s(picturePath, fileDialog.GetSelected().string().c_str());
				else
					strcpy_s(videoPath, fileDialog.GetSelected().string().c_str());
				fileDialog.ClearSelected();
			}

			ImGui::ColorEdit3("Text Color", reinterpret_cast<float*>(&fgColor));
			ImGui::ColorEdit3("Text Shadow", reinterpret_cast<float*>(&bgColor));
			if (ImGui::Button("...###BrowseThemeFolder"))
			{
				fileDialogTheme.Open();
			}
			fileDialogTheme.Display();

			if (fileDialogTheme.HasSelected())
			{
				strcpy_s(operaThemePath, fileDialogTheme.GetSelected().string().c_str());
				fileDialogTheme.ClearSelected();
			}

			ImGui::SameLine();
			ImGui::InputText("Opera Themes Folder", operaThemePath, 512);
			ImGui::End();
		}

		void VisualEffectsConfig()
		{
			ImGui::Begin("Video Effects");
			
			ImGui::Checkbox("Box Blur", &chkBoxBlur);
			ImGui::PushItemWidth(80);
			ImGui::InputInt("Box Blur Intensity", &boxBlurIntensity);
			if (boxBlurIntensity < 1) boxBlurIntensity = 1;
			if (boxBlurIntensity > 10) boxBlurIntensity = 10;
			int orderA = g_FourIntCache.a;
			ImGui::InputInt("Box Blur Priority Rank", &orderA);
			if (orderA < 1) orderA = 1;
			if (orderA > 4) orderA = 4;
			g_FourIntCache.A(orderA);
			ImGui::PopItemWidth();

			ImGui::Separator();

			ImGui::Checkbox("Vignette", &chkVignette);
			ImGui::PushItemWidth(80);
			ImGui::InputInt("Vignette Pi Divisor", &vignettePiDivisor);
			int orderB = g_FourIntCache.b;
			ImGui::InputInt("Vignette Priority Rank", &orderB);
			if (orderB < 1) orderB = 1;
			if (orderB > 4) orderB = 4;
			g_FourIntCache.B(orderB);
			ImGui::PopItemWidth();
			vignette.SetPiDivisor(vignettePiDivisor);
			if (vignettePiDivisor < 1) vignettePiDivisor = 1;
			if (vignettePiDivisor > 6) vignettePiDivisor = 6;

			ImGui::Separator();

			ImGui::Checkbox("EdgeDetection", &chkEdgeDetection);
			ImGui::PushItemWidth(80);
			int orderC = g_FourIntCache.c;
			ImGui::InputInt("Edge Detection Priority Rank", &orderC);
			if (orderC < 1) orderC = 1;
			if (orderC > 4) orderC = 4;
			g_FourIntCache.C(orderC);
			ImGui::PopItemWidth();

			ImGui::Separator();

			ImGui::Checkbox("Monochrome", &chkMonochrome);
			ImGui::PushItemWidth(80);
			int orderD = g_FourIntCache.d;
			ImGui::InputInt("Monochrome Priority Rank", &orderD);
			if (orderD < 1) orderD = 1;
			if (orderD > 4) orderD = 4;
			g_FourIntCache.D(orderD);
			ImGui::PopItemWidth();

			ImGui::End();
		}

		void ReviewAndDeliver()
		{
			ImGui::Begin("Active Video Effects Priority List");

			// TODO: Better handling of videoEffects vector. Remove constant resizing and assignment per loop

			videoEffects.resize(4);
			videoEffects[g_FourIntCache.a - 1] = (chkBoxBlur == true) ? &boxBlur : nullptr;
			videoEffects[g_FourIntCache.b - 1] = (chkVignette == true) ? &vignette : nullptr;
			videoEffects[g_FourIntCache.c - 1] = (chkEdgeDetection == true) ? &edgeDetection : nullptr;
			videoEffects[g_FourIntCache.d - 1] = (chkMonochrome == true) ? &monoChrome : nullptr;
		
			std::vector<std::string> videoEffectsStr;

			for (VideoEffect* videoEffect : videoEffects)
			{
				if (dynamic_cast<BoxBlur*>(videoEffect) != nullptr)
				{
					if (chkBoxBlur) 
						videoEffectsStr.push_back(std::basic_string<char>(*videoEffect));
				}

				else if (dynamic_cast<Vignette*>(videoEffect) != nullptr)
				{
					if (chkVignette)
						videoEffectsStr.push_back(std::basic_string<char>(*videoEffect));
				}

				else if (dynamic_cast<EdgeDetection*>(videoEffect) != nullptr)
				{
					if (chkEdgeDetection)
						videoEffectsStr.push_back(std::basic_string<char>(*videoEffect));
				}

				else if (dynamic_cast<MonoChrome*>(videoEffect) != nullptr)
				{
					if (chkMonochrome)
						videoEffectsStr.push_back(std::basic_string<char>(*videoEffect));
				}
			}

			auto VectorOfStringGetter = [](void* data, int idx, const char** outText)->bool
			{
				auto* strSaves = static_cast<std::string*>(data);
				std::string& strSave = strSaves[idx];
				*outText = strSave.c_str();
				return true;
			};

			ImGui::ListBox(
				"###Video Filter Order",
				&videoEffectListSelectedItem,
				VectorOfStringGetter,
				videoEffectsStr.data(),
				videoEffectsStr.size()
			);

			ImGui::Separator();

			ImGui::Checkbox("Automatically Add To Opera Themes", &chkToThemes);

			if (ImGui::Button("Create Theme"))
			{
				std::thread themeThread(CreateTheme);
				processingTheme = true;
				ImGui::OpenPopup("Creating Theme");
				themeThread.detach();
			}

			ImGui::SetNextWindowSize(ImVec2(300, 150));
			if (ImGui::BeginPopupModal("Creating Theme"))
			{
				ImGui::Text("Creating Theme...\n(May take up to 10 minutes)");
				if (!processingTheme) 
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			ImGui::End();
		}

	}

	void GUIMain()
	{
		auto& guiInstance = WrapGui::GuiApp::Get();
		ImGui::SetNextWindowSize(OperaThemeMaker::g_WindowDimensions);
		ImGui::Begin("Opera Theme Maker", guiInstance.GetRunStatePtr(),
			ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		// Setup DockSpaces
		ImGui::DockSpace(ImGui::GetID("DockUp"), ImVec2(g_WindowDimensions.x, g_WindowDimensions.y / 2 - 80));
		ImGui::DockSpace(ImGui::GetID("DockDownA"), ImVec2(g_WindowDimensions.x/2, g_WindowDimensions.y / 2 + 80));
		ImGui::SameLine();
		ImGui::DockSpace(ImGui::GetID("DockDownB"), ImVec2(g_WindowDimensions.x / 2, g_WindowDimensions.y / 2 + 80));

		// Setup MenuBar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					guiInstance.Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Main Config
		ImGui::SetNextWindowDockID(ImGui::GetID("DockUp"), ImGuiCond_FirstUseEver | ImGuiConfigFlags_DockingEnable);
		GUIComponents::MainConfig();

		// Visual Effect Config
		ImGui::SetNextWindowDockID(ImGui::GetID("DockDownA"), ImGuiCond_FirstUseEver | ImGuiConfigFlags_DockingEnable);
		GUIComponents::VisualEffectsConfig();
		
		// Visual Effect Priority List and Create Button
		ImGui::SetNextWindowDockID(ImGui::GetID("DockDownB"), ImGuiCond_FirstUseEver | ImGuiConfigFlags_DockingEnable);
		GUIComponents::ReviewAndDeliver();

		ImGui::End();
	}

	void CreateTheme()
	{
		using namespace GUIComponents;
		// Check if all required configurations are filled
		if (*authorName == 0 || *themeName == 0 || *picturePath == 0)
		{
			processingTheme = false;
			throw std::exception("Some Required Are Fields Left Blank!");
		}

		// Verify Output Path Exists
		if (!exists(outputPath)) create_directory(outputPath);
		// Create persona
		Persona persona;
		persona.info = { std::string(themeName), std::string(authorName), static_cast<unsigned>(version) };

		if (*videoPath != 0)
			persona.startPage = { std::filesystem::path(std::string(videoPath)).replace_extension(".webm").string(),
			std::string("#") + Util::U32ToHex(ImGui::ColorConvertFloat4ToU32(fgColor)),
				std::string("#") + Util::U32ToHex(ImGui::ColorConvertFloat4ToU32(bgColor)),
			std::string(picturePath)};
		else
			persona.startPage = { std::string(picturePath), std::string("#") + Util::U32ToHex(ImGui::ColorConvertFloat4ToU32(fgColor)),
			Util::U32ToHex(ImGui::ColorConvertFloat4ToU32(bgColor)), "" };

		Core::CreatePersonaIni(persona);

		// Process Video (Optional)
		std::filesystem::path processedVideoPath;
		if (*videoPath != 0)
		{
			processedVideoPath = Core::ProcessVideo(std::filesystem::path(videoPath), videoEffects);
		}

		// Create Archive
		if (*videoPath != 0)
			Core::Archive(picturePath, "output/persona.ini", processedVideoPath, std::string(themeName) + ".zip",
				(chkToThemes == true) ? std::filesystem::path(std::string(operaThemePath)) : std::filesystem::current_path().append("output"));
		else
			Core::Archive(picturePath, "output/persona.ini", std::string(themeName) + ".zip",
				(chkToThemes == true) ? std::filesystem::path(std::string(operaThemePath)) : std::filesystem::current_path().append("output"));

		processingTheme = false;
	}

}

int main()
{
	WrapGui::GuiApp::InitGui({ "", 1, 1 });
	auto& guiInstance = WrapGui::GuiApp::Get();
	guiInstance.HideWindow();
	guiInstance.SetMain(OperaThemeMaker::GUIMain);

	// Pre-run setups
	strcpy_s(OperaThemeMaker::GUIComponents::operaThemePath, OperaThemeMaker::Util::GetAppDataRoaming().append("Opera Software").append("Opera Stable").append("themes").string().c_str());
	OperaThemeMaker::GUIComponents::fileDialogTheme.SetPwd(OperaThemeMaker::GUIComponents::operaThemePath);

	guiInstance.Run();
}