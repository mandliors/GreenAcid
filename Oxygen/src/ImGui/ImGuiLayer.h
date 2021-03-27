#pragma once

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#ifdef PYTHON_INTERPRETER
#include <C:/Users/mandl/AppData/Local/Programs/Python/Python39/include/Python.h>
#endif
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "../Events/Event.h"

namespace ox {


	class ImGuiLayer
	{
		friend class GameWindow;
		friend class EventManager;

	public:
		ImGuiLayer(const ImGuiLayer&) = delete;
		static void BlockEvents(bool value);

	private:
		ImGuiLayer() { }
		static void __Init();
		static void __Update();
		static void __Render();
		static void __HandleEvent(Event& event);
		static void __Shutdown();


		static float s_Time;
		static bool s_BlockEvents;
	};

#ifdef PYTHON_INTERPRETER
	struct PythonConsole
	{
		char                  InputBuf[256];
		ImVector<char*>       Items;
		ImVector<char*>       History;
		int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
		bool                  AutoScroll;
		bool                  ScrollToBottom;

		PythonConsole()
		{
			ClearLog();
			memset(InputBuf, 0, sizeof(InputBuf));
			HistoryPos = -1;
			AutoScroll = true;
			ScrollToBottom = false;
		}
		~PythonConsole()
		{
			ClearLog();
			for (int i = 0; i < History.Size; i++)
				free(History[i]);
		}

		// Portable helpers
		static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
		static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
		static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
		static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }

		void ClearLog()
		{
			for (int i = 0; i < Items.Size; i++)
				free(Items[i]);
			Items.clear();
		}

		void AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			// FIXME-OPT
			char buf[1024];
			va_list args;
			va_start(args, fmt);
			vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
			buf[IM_ARRAYSIZE(buf) - 1] = 0;
			va_end(args);
			Items.push_back(Strdup(buf));
		}

		void Draw(const char* title, bool* p_open)
		{
			ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin(title, p_open))
			{
				ImGui::End();
				return;
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Close python interpreter"))
					*p_open = false;
				ImGui::EndPopup();
			}

			// Reserve enough left-over height for 1 separator + 1 input text
			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable("Clear")) ClearLog();
				ImGui::EndPopup();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
			for (int i = 0; i < Items.Size; i++)
				ImGui::TextWrapped(Items[i]);

			if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
				ImGui::SetScrollHereY(1.0f);
			ScrollToBottom = false;

			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();

			// Command-line
			bool reclaim_focus = false;
			ImVec2 size = ImGui::GetContentRegionAvail();
			ImGui::PushItemWidth(size.x);
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText(" ", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
			{
				char* s = InputBuf;
				Strtrim(s);
				if (s[0])
					ExecCommand(s);
				strcpy(s, "");
				reclaim_focus = true;
			}
			ImGui::PopItemWidth();

			// Auto-focus on window apparition
			ImGui::SetItemDefaultFocus();
			if (reclaim_focus)
				ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

			ImGui::End();
		}

		void Trim(std::string& str)
		{
			str.erase(0, 1);
			str.erase(str.size() - 1, 1);
			bool go = true;
			while (go)
			{
				go = false;
				for (int i = 0; i < str.size(); i++)
				{
					if (str[i] == '\\' && i < str.size() - 1 || str[i + 1] == '\n')
					{
						str.erase(i, 2);
						go = true;
						break;
					}
				}
			}
		}

		void ExecCommand(const char* command_line)
		{
			
			std::string temp(command_line);
			if ((temp.find("exit(") != std::string::npos || temp.find("quit(") != std::string::npos) && temp.find(")") != std::string::npos) return;

			AddLog("> %s\n", command_line);

			static std::string stdOutErr =
				"import sys\n\
class CatchOutErr:\n\
    def __init__(self):\n\
        self.value = ''\n\
    def write(self, txt):\n\
        self.value += txt\n\
catchOutErr = CatchOutErr()\n\
sys.stdout = catchOutErr\n\
sys.stderr = catchOutErr\n\
";
			PyObject* pModule = PyImport_AddModule("__main__");
			PyRun_SimpleString(stdOutErr.c_str());
			PyRun_SimpleString(command_line);
			PyObject* catcher = PyObject_GetAttrString(pModule, "catchOutErr");
			PyObject* output = PyObject_GetAttrString(catcher, "value");
			PyObject* repr = PyObject_Repr(output);
			PyObject* str = PyUnicode_AsEncodedString(repr, "utf-8", "~E~");
			std::string result = { PyBytes_AS_STRING(str) };
			Trim(result);

			if (result != "")
				AddLog(result.c_str());

			HistoryPos = -1;
			for (int i = History.Size - 1; i >= 0; i--)
				if (Stricmp(History[i], command_line) == 0)
				{
					free(History[i]);
					History.erase(History.begin() + i);
					break;
				}
			History.push_back(Strdup(command_line));

			ScrollToBottom = true;
		}

		// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
		static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
		{
			PythonConsole* console = (PythonConsole*)data->UserData;
			return console->TextEditCallback(data);
		}

		int TextEditCallback(ImGuiInputTextCallbackData* data)
		{
			switch (data->EventFlag)
			{
				case ImGuiInputTextFlags_CallbackHistory:
				{
					// Example of HISTORY
					const int prev_history_pos = HistoryPos;
					if (data->EventKey == ImGuiKey_UpArrow)
					{
						if (HistoryPos == -1)
							HistoryPos = History.Size - 1;
						else if (HistoryPos > 0)
							HistoryPos--;
					}
					else if (data->EventKey == ImGuiKey_DownArrow)
					{
						if (HistoryPos != -1)
							if (++HistoryPos >= History.Size)
								HistoryPos = -1;
					}

					// A better implementation would preserve the data on the current input line along with cursor position.
					if (prev_history_pos != HistoryPos)
					{
						const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
						data->DeleteChars(0, data->BufTextLen);
						data->InsertChars(0, history_str);
					}
				}
			}
			return 0;
		}
	};
#endif
}