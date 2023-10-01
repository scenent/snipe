#include "script/interpreter.h"

#define CHECK_EXIT if (glfwWindowShouldClose(root->window) == true || force_return){ return;}

std::queue<int> savedChoiceQueue{};
std::queue<int> choiceQueue{};
std::queue<std::wstring> savedInputQueue{};
std::queue<std::wstring> inputQueue{};
int currentGlobalIndex = 0;
int goalGlobalIndex = -1;

static std::vector<Scene2D*> tempSceneList{};
static std::vector<Camera2D*> tempCameraList{};
static std::vector<Dialog*> tempDialogList{};
static std::vector<ChoiceBox*> tempChoiceBoxList{};
static std::vector<HistoryContainer*> tempHistoryContainerList{};

static std::vector<Object2D*> tempObjectListForContainer{};

void interpret_screen(std::wstring src, Scene2D* currentScopeScene, bool isContainerScope) {
	std::vector<std::wstring> lines = split(src, L'\n');
	for (int i = 0; i < lines.size(); i++) {
		if (startsWith(lines[i], L"    ") || startsWith(lines[i], L"#")) continue;
		if (lines[i] == L"") continue;
		std::vector<Token> tokens = tokenize(lines[i]);
		if (tokens[0].type == TokenKind::New) {
			if (tokens[1].raw_data == L"root") {
				std::wstring CURRENT_SCENE_NAME = L"";
				root = new Root();
				std::wstring root_src= L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { root_src += erasedFront(lines[j], 4) + L"\n";}}
				std::vector<std::wstring> root_lines = split(root_src, L'\n');
				for (int j = 0; j < root_lines.size(); j++) {
					if (startsWith(root_lines[j], L"    ") || startsWith(root_lines[j], L"#") || root_lines[j] == L"") continue;
					std::vector<Token> root_tokens = tokenize(root_lines[j]);
					if (root_tokens[0].type == TokenKind::New) break;
					if (root_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < root_tokens.size(); q++) value += root_tokens[q].data();
						if (root_tokens[1].raw_data == L"window_position") { root->WINDOW_XPOS = toVec2(value).x; root->WINDOW_YPOS = toVec2(value).y; }
						else if (root_tokens[1].raw_data == L"window_size") { root->WINDOW_WIDTH = toVec2(value).x; root->WINDOW_HEIGHT = toVec2(value).y; utils::WINDOW_WIDTH_ORIGIN = toVec2(value).x; utils::WINDOW_HEIGHT_ORIGIN = toVec2(value).y; }
						else if (root_tokens[1].raw_data == L"window_title") { root->WINDOW_TITLE = multibyte2utf8(toString(value)); }
						else if (root_tokens[1].raw_data == L"window_fullscreen") { root->WINDOW_FULLSCREEN = toBool(value); }
						else if (root_tokens[1].raw_data == L"window_allow_hidpi") { root->WINDOW_ALLOW_HIDPI = toBool(value); }
						else if (root_tokens[1].raw_data == L"window_icon") { root->WINDOW_ICON_PATH = ws2s(checkPath(toString(value))); }
						else if (root_tokens[1].raw_data == L"window_quit_on_esc") { root->WINDOW_QUIT_ON_ESC = toBool(value); }
						else if (root_tokens[1].raw_data == L"window_target_fps") { root->WINDOW_TARGET_FPS = clamp(toFloat(value), 1.0f, 60.0f); }
						else if (root_tokens[1].raw_data == L"current_scene") { CURRENT_SCENE_NAME = toString(value); }
					}
				}
				glfwInit();
				glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
				glfwWindowHint(GLFW_SCALE_TO_MONITOR, root->WINDOW_ALLOW_HIDPI);
				root->window = glfwCreateWindow(root->WINDOW_WIDTH, root->WINDOW_HEIGHT, root->WINDOW_TITLE.c_str(), NULL, NULL);
				if (root->window == NULL) alert(L"GLFW load error");
				glfwMakeContextCurrent(root->window);
				if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))alert(L"GLAD load error");
				for (int j = 0; j < root_lines.size(); j++) {
					if (startsWith(root_lines[j], L"    ") || startsWith(root_lines[j], L"#") || root_lines[j] == L"") continue;
					std::vector<Token> root_tokens = tokenize(root_lines[j]);
					if (root_tokens[0].type == TokenKind::New) {
						std::wstring another_src = L"";
						another_src += root_lines[j] + L"\n";
						for (int k = j + 1; k < root_lines.size(); k++) { if (startsWith(root_lines[k], L"    ")) { another_src += root_lines[k] + L"\n"; } else break; }
						interpret_screen(another_src);
					}
				}
				for (auto& p : tempSceneList) { if (p->name == CURRENT_SCENE_NAME) { root->currentScene = p; break; } }
			}
			else if (tokens[1].raw_data == L"scene") {
				Scene2D* scene = root->addScene2D(L"");
				tempSceneList.push_back(scene);
				std::wstring CURRENT_CAMERA_NAME = L"";
				std::wstring CURRENT_DIALOG_NAME = L"";
				std::wstring CURRENT_CHOICEBOX_NAME = L"";
				std::wstring CURRENT_HISTORY_NAME = L"";

				std::wstring scene_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { scene_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> scene_lines = split(scene_src, L'\n');
				for (int j = 0; j < scene_lines.size(); j++) {
					if (startsWith(scene_lines[j], L"    ") || startsWith(scene_lines[j], L"#") || scene_lines[j] == L"") continue;
					std::vector<Token> scene_tokens = tokenize(scene_lines[j]);
					if (scene_tokens[0].type == TokenKind::New) break;
					if (scene_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < scene_tokens.size(); q++) value += scene_tokens[q].data();
						if (scene_tokens[1].raw_data == L"name") { scene->name = toString(value); }
						else if (scene_tokens[1].raw_data == L"current_camera") { CURRENT_CAMERA_NAME = toString(value); }
						else if (scene_tokens[1].raw_data == L"current_dialog") { CURRENT_DIALOG_NAME = toString(value); }
						else if (scene_tokens[1].raw_data == L"current_choicebox") { CURRENT_CHOICEBOX_NAME = toString(value); }
						else if (scene_tokens[1].raw_data == L"current_history") { CURRENT_HISTORY_NAME = toString(value); }
					}
				}
				for (int j = 0; j < scene_lines.size(); j++) {
					if (startsWith(scene_lines[j], L"    ") || startsWith(scene_lines[j], L"#") || scene_lines[j] == L"") continue;
					std::vector<Token> scene_tokens = tokenize(scene_lines[j]);
					if (scene_tokens[0].type == TokenKind::New) {
						std::wstring another_src = L"";
						another_src += scene_lines[j] + L"\n";
						for (int k = j + 1; k < scene_lines.size(); k++) { if (startsWith(scene_lines[k], L"    ")) { another_src += scene_lines[k] + L"\n"; } else break; }
						interpret_screen(another_src, scene);
					}
				}
				for (auto& p : tempCameraList) { if (p->name == CURRENT_CAMERA_NAME) { scene->currentCamera = p; break; } }
				for (auto& p : tempDialogList) { if (p->name == CURRENT_DIALOG_NAME) { scene->currentDialog = p; break; } }
				for (auto& p : tempChoiceBoxList) { if (p->name == CURRENT_CHOICEBOX_NAME) { scene->currentChoiceBox = p; break; } }
				for (auto& p : tempHistoryContainerList) { if (p->name == CURRENT_HISTORY_NAME) { scene->currentHistory = p; break; } }
			}
			else if (tokens[1].raw_data == L"camera") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new camera");
				Camera2D* camera = currentScopeScene->addCamera2D(L"");
				tempCameraList.push_back(camera);
				std::wstring camera_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { camera_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> camera_lines = split(camera_src, L'\n');
				for (int j = 0; j < camera_lines.size(); j++) {
					if (startsWith(camera_lines[j], L"    ") || startsWith(camera_lines[j], L"#") || camera_lines[j] == L"") continue;
					std::vector<Token> camera_tokens = tokenize(camera_lines[j]);
					if (camera_tokens[0].type == TokenKind::New) break;
					if (camera_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < camera_tokens.size(); q++) value += camera_tokens[q].data();
						if (camera_tokens[1].raw_data == L"name") { camera->name = toString(value); }
						else if (camera_tokens[1].raw_data == L"position") { camera->position = toVec2(value); }
						else if (camera_tokens[1].raw_data == L"zoom") { camera->zoom = toVec2(value); }
						else if (camera_tokens[1].raw_data == L"modify_movement") { camera->modify_movement = toBool(value); }
						else if (camera_tokens[1].raw_data == L"offset") { camera->offset = toVec2(value); }
						else if (camera_tokens[1].raw_data == L"rotation") { camera->rotation = toFloat(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"sprite") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new sprite");
				Sprite2D* sprite = currentScopeScene->addSprite2D(L"");
				if (isContainerScope == true) tempObjectListForContainer.push_back(sprite);
				std::wstring sprite_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { sprite_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> sprite_lines = split(sprite_src, L'\n');
				for (int j = 0; j < sprite_lines.size(); j++) {
					if (startsWith(sprite_lines[j], L"    ") || startsWith(sprite_lines[j], L"#") || sprite_lines[j] == L"") continue;
					std::vector<Token> sprite_tokens = tokenize(sprite_lines[j]);
					if (sprite_tokens[0].type == TokenKind::New) break;
					if (sprite_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < sprite_tokens.size(); q++) value += sprite_tokens[q].data();
						if (sprite_tokens[1].raw_data == L"name") { sprite->name = toString(value); }
						else if (sprite_tokens[1].raw_data == L"tag") { sprite->tag = toString(value); }
						else if (sprite_tokens[1].raw_data == L"position") { sprite->position = toVec2(value); }
						else if (sprite_tokens[1].raw_data == L"pivot_offset") { sprite->pivot_offset = toVec2(value); }
						else if (sprite_tokens[1].raw_data == L"rotation") { sprite->rotation = toFloat(value); }
						else if (sprite_tokens[1].raw_data == L"scale") { sprite->scale = toVec2(value); }
						else if (sprite_tokens[1].raw_data == L"file_path") { sprite->filePath = checkPath(toString(value)); sprite->load(sprite->filePath); }
						else if (sprite_tokens[1].raw_data == L"pixel_filter") { sprite->pixel_filter = toBool(value); sprite->load(sprite->filePath); }
						else if (sprite_tokens[1].raw_data == L"z_order") { sprite->z_order = toInt(value); }
						else if (sprite_tokens[1].raw_data == L"visible") { sprite->visible = toBool(value); }
						else if (sprite_tokens[1].raw_data == L"modulate") { sprite->modulate = toVec4(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"vbox" || tokens[1].raw_data == L"vboxcontainer") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new vbox");
				VboxContainer* vbox = currentScopeScene->addVboxContainer(L"");
				std::wstring vbox_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { vbox_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> vbox_lines = split(vbox_src, L'\n');
				for (int j = 0; j < vbox_lines.size(); j++) {
					if (startsWith(vbox_lines[j], L"    ") || startsWith(vbox_lines[j], L"#") || vbox_lines[j] == L"") continue;
					std::vector<Token> vbox_tokens = tokenize(vbox_lines[j]);
					if (vbox_tokens[0].type == TokenKind::New) break;
					if (vbox_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < vbox_tokens.size(); q++) value += vbox_tokens[q].data();
						if (vbox_tokens[1].raw_data == L"name") { vbox->name = toString(value); }
						else if (vbox_tokens[1].raw_data == L"position") { vbox->position = toVec2(value); }
						else if (vbox_tokens[1].raw_data == L"size") { vbox->size = toVec2(value); }
						else if (vbox_tokens[1].raw_data == L"separation") { vbox->separation = toFloat(value); }
						else if (vbox_tokens[1].raw_data == L"align_mode") { vbox->alignMode = ContainerAlignMode(toInt(value)); }
						else if (vbox_tokens[1].raw_data == L"visible") { vbox->visible = toBool(value); }
						else if (vbox_tokens[1].raw_data == L"z_order") { vbox->z_order = toInt(value); }
					}
				}
				for (int j = 0; j < vbox_lines.size(); j++) {
					if (startsWith(vbox_lines[j], L"    ") || startsWith(vbox_lines[j], L"#") || vbox_lines[j] == L"") continue;
					std::vector<Token> vbox_tokens = tokenize(vbox_lines[j]);
					if (vbox_tokens[0].type == TokenKind::New) {
						std::wstring another_src = L"";
						another_src += vbox_lines[j] + L"\n";
						for (int k = j + 1; k < vbox_lines.size(); k++) { if (startsWith(vbox_lines[k], L"    ")) { another_src += vbox_lines[k] + L"\n"; } else break; }
						interpret_screen(another_src, currentScopeScene, true);
					}
				}
				vbox->objectList = tempObjectListForContainer;
				tempObjectListForContainer.clear();
			}
			else if (tokens[1].raw_data == L"dialog") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new dialog");
				Dialog* dialog = currentScopeScene->addDialog(L"");
				tempDialogList.push_back(dialog);
				std::wstring dialog_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { dialog_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> dialog_lines = split(dialog_src, L'\n');
				for (int j = 0; j < dialog_lines.size(); j++) {
					if (startsWith(dialog_lines[j], L"    ") || startsWith(dialog_lines[j], L"#") || dialog_lines[j] == L"") continue;
					std::vector<Token> dialog_tokens = tokenize(dialog_lines[j]);
					if (dialog_tokens[0].type == TokenKind::New) break;
					if (dialog_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < dialog_tokens.size(); q++) value += dialog_tokens[q].data();
						if (dialog_tokens[1].raw_data == L"name") { dialog->name = toString(value); }
						else if (dialog_tokens[1].raw_data == L"namebox_file_path") { dialog->namebox->filePath = checkPath(toString(value)); dialog->namebox->load(dialog->namebox->filePath); }
						else if (dialog_tokens[1].raw_data == L"background_file_path") { dialog->background->filePath = checkPath(toString(value)); dialog->background->load(dialog->background->filePath); }
						else if (dialog_tokens[1].raw_data == L"namebox_scale") { dialog->namebox->scale = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"background_scale") { dialog->background->scale = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"position") { dialog->position = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"pivot_offset") { dialog->pivot_offset = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"rotation") { dialog->rotation = toFloat(value); }
						else if (dialog_tokens[1].raw_data == L"namelabel_position") { dialog->nameLabelPos = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"namebox_position") {dialog->namebox->position = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"background_position") {dialog->background->position = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"size") { dialog->size = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"namelabel_size") { dialog->nameLabel->size = toVec2(value); }
						else if (dialog_tokens[1].raw_data == L"font_path") { dialog->fontPath = checkPath(toString(value));}
						else if (dialog_tokens[1].raw_data == L"namelabel_font_path") { dialog->nameFontPath = checkPath(toString(value));}
						else if (dialog_tokens[1].raw_data == L"speed") { dialog->speed = toFloat(value); }
						else if (dialog_tokens[1].raw_data == L"visible") { dialog->visible = toBool(value); }
						else if (dialog_tokens[1].raw_data == L"font_size") { dialog->fontSize = toInt(value); }
						else if (dialog_tokens[1].raw_data == L"namelabel_font_size") { dialog->nameFontSize = toInt(value); }
						else if (dialog_tokens[1].raw_data == L"font_color") { dialog->textLabel->color = toVec4(value); }
						else if (dialog_tokens[1].raw_data == L"outline_size") { dialog->textLabel->outline_size = toFloat(value); }
						else if (dialog_tokens[1].raw_data == L"outline_color") { dialog->textLabel->outline_color = toVec4(value); }
						else if (dialog_tokens[1].raw_data == L"wrap_text") { dialog->textLabel->wrap_text = toBool(value);}
						else if (dialog_tokens[1].raw_data == L"wrap_by_word") { dialog->textLabel->wrap_mode = toBool(value) == true ? LabelWrapMode::Word : LabelWrapMode::Normal;}
						else if (dialog_tokens[1].raw_data == L"z_order") { dialog->z_order = toInt(value);}
					}
				}
			}
			else if (tokens[1].raw_data == L"choicebox") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new choicebox");
				ChoiceBox* choice = currentScopeScene->addChoiceBox(L"");
				tempChoiceBoxList.push_back(choice);
				std::wstring choice_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { choice_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> choice_lines = split(choice_src, L'\n');
				for (int j = 0; j < choice_lines.size(); j++) {
					if (startsWith(choice_lines[j], L"    ") || startsWith(choice_lines[j], L"#") || choice_lines[j] == L"") continue;
					std::vector<Token> choice_tokens = tokenize(choice_lines[j]);
					if (choice_tokens[0].type == TokenKind::New) break; 
					if (choice_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < choice_tokens.size(); q++) value += choice_tokens[q].data();
						if (choice_tokens[1].raw_data == L"name") { choice->name = toString(value); }
						else if (choice_tokens[1].raw_data == L"position") { choice->position = toVec2(value); }
						else if (choice_tokens[1].raw_data == L"pivot_offset") { choice->pivot_offset = toVec2(value); }
						else if (choice_tokens[1].raw_data == L"rotation") { choice->rotation = toFloat(value); }
						else if (choice_tokens[1].raw_data == L"size") { choice->size = toVec2(value); }
						else if (choice_tokens[1].raw_data == L"separation") { choice->separation = toFloat(value); }
						else if (choice_tokens[1].raw_data == L"each_box_size") { choice->eachBoxSize = toFloat(value); }
						else if (choice_tokens[1].raw_data == L"font_size") { choice->fontSize = toFloat(value); }
						else if (choice_tokens[1].raw_data == L"font_path") { choice->fontPath = checkPath(toString(value)); }
						else if (choice_tokens[1].raw_data == L"font_color") { choice->fontColor = toVec4(value); }
						else if (choice_tokens[1].raw_data == L"outline_size") { choice->outline_size = toFloat(value); }
						else if (choice_tokens[1].raw_data == L"outline_color") { choice->outline_color = toVec4(value); }
						else if (choice_tokens[1].raw_data == L"visible") { choice->visible = toBool(value); }
						else if (choice_tokens[1].raw_data == L"z_order") { choice->z_order = toInt(value); }
						else if (choice_tokens[1].raw_data == L"idle_file_path") { choice->idleFilePath = checkPath(toString(value)); }
						else if (choice_tokens[1].raw_data == L"hover_file_path") { choice->hoverFilePath = checkPath(toString(value)); }
						else if (choice_tokens[1].raw_data == L"down_file_path") { choice->downFilePath = checkPath(toString(value)); }

					}
				}
			}
			else if (tokens[1].raw_data == L"button") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new button");
				Button* button = currentScopeScene->addButton(L"");
				std::wstring button_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { button_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> button_lines = split(button_src, L'\n');
				for (int j = 0; j < button_lines.size(); j++) {
					if (startsWith(button_lines[j], L"    ") || startsWith(button_lines[j], L"#") || button_lines[j] == L"") continue;
					std::vector<Token> button_tokens = tokenize(button_lines[j]);
					if (button_tokens[0].type == TokenKind::New) break;
					if (button_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < button_tokens.size(); q++) value += button_tokens[q].data();
						if (button_tokens[1].raw_data == L"name") { button->name = toString(value); }
						else if (button_tokens[1].raw_data == L"position") { button->position = toVec2(value); }
						else if (button_tokens[1].raw_data == L"pivot_offset") { button->pivot_offset = toVec2(value); }
						else if (button_tokens[1].raw_data == L"rotation") { button->rotation = toFloat(value); }
						else if (button_tokens[1].raw_data == L"size") { button->size = toVec2(value); }
						else if (button_tokens[1].raw_data == L"font_size") { button->fontSize = toFloat(value); button->load(button->fontPath); }
						else if (button_tokens[1].raw_data == L"font_path") { button->fontPath = checkPath(toString(value)); button->load(button->fontPath); }
						else if (button_tokens[1].raw_data == L"text") { button->text = toString(value); button->load(button->fontPath); }
						else if (button_tokens[1].raw_data == L"idle_color") { button->idleColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"hover_color") { button->hoverColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"down_color") { button->downColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"callback") { button->callbackList.push_back(toString(value)); }
						else if (button_tokens[1].raw_data == L"visible") { button->visible = toBool(value); }
						else if (button_tokens[1].raw_data == L"z_order") { button->z_order = toInt(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"text_button") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new textbutton");
				TextButton* button = currentScopeScene->addTextButton(L"");
				std::wstring button_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { button_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> button_lines = split(button_src, L'\n');
				for (int j = 0; j < button_lines.size(); j++) {
					if (startsWith(button_lines[j], L"    ") || startsWith(button_lines[j], L"#") || button_lines[j] == L"") continue;
					std::vector<Token> button_tokens = tokenize(button_lines[j]);
					if (button_tokens[0].type == TokenKind::New) break;
					if (button_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < button_tokens.size(); q++) value += button_tokens[q].data();
						if (button_tokens[1].raw_data == L"name") { button->name = toString(value); }
						else if (button_tokens[1].raw_data == L"position") { button->position = toVec2(value); }
						else if (button_tokens[1].raw_data == L"pivot_offset") { button->pivot_offset = toVec2(value); }
						else if (button_tokens[1].raw_data == L"rotation") { button->rotation = toFloat(value); }
						else if (button_tokens[1].raw_data == L"size") { button->size = toVec2(value); }
						else if (button_tokens[1].raw_data == L"font_size") { button->fontSize = toFloat(value); button->load(button->fontPath);}
						else if (button_tokens[1].raw_data == L"font_path") { button->fontPath = checkPath(toString(value)); button->load(button->fontPath);}
						else if (button_tokens[1].raw_data == L"outline_size") { button->textLabel->outline_size = toFloat(value); button->load(button->fontPath); }
						else if (button_tokens[1].raw_data == L"outline_color") { button->textLabel->outline_color = toVec4(value); }
						else if (button_tokens[1].raw_data == L"text") { button->text = toString(value); button->load(button->fontPath);}
						else if (button_tokens[1].raw_data == L"idle_outline_color") { button->idleOutlineColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"idle_color") { button->idleColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"hover_outline_color") { button->hoverOutlineColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"hover_color") { button->hoverColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"down_outline_color") { button->downOutlineColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"down_color") { button->downColor = toVec4(value); }
						else if (button_tokens[1].raw_data == L"callback") { button->callbackList.push_back(toString(value)); }
						else if (button_tokens[1].raw_data == L"visible") { button->visible = toBool(value); }
						else if (button_tokens[1].raw_data == L"z_order") { button->z_order = toInt(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"textedit") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new textedit");
				TextEdit* edit = currentScopeScene->addTextEdit(L"");
				std::wstring edit_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { edit_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> edit_lines = split(edit_src, L'\n');
				for (int j = 0; j < edit_lines.size(); j++) {
					if (startsWith(edit_lines[j], L"    ") || startsWith(edit_lines[j], L"#") || edit_lines[j] == L"") continue;
					std::vector<Token> edit_tokens = tokenize(edit_lines[j]);
					if (edit_tokens[0].type == TokenKind::New) break;
					if (edit_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < edit_tokens.size(); q++) value += edit_tokens[q].data();
						if (edit_tokens[1].raw_data == L"name") { edit->name = toString(value); }
						else if (edit_tokens[1].raw_data == L"position") { edit->position = toVec2(value); }
						else if (edit_tokens[1].raw_data == L"pivot_offset") { edit->pivot_offset = toVec2(value); }
						else if (edit_tokens[1].raw_data == L"size") { edit->size = toVec2(value); }
						else if (edit_tokens[1].raw_data == L"fast_backspace") { edit->fast_backspace = toBool(value); }
						else if (edit_tokens[1].raw_data == L"font_path") { edit->fontPath = checkPath(toString(value)); }
						else if (edit_tokens[1].raw_data == L"font_size") { edit->fontSize = toInt(value); }
						else if (edit_tokens[1].raw_data == L"font_color") { edit->textLabel->color = toVec4(value); }
						else if (edit_tokens[1].raw_data == L"z_order") { edit->z_order = toInt(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"color_rect") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new color rect");
				ColorRect* cr = currentScopeScene->addColorRect(L"");
				std::wstring cr_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { cr_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> cr_lines = split(cr_src, L'\n');
				for (int j = 0; j < cr_lines.size(); j++) {
					if (startsWith(cr_lines[j], L"    ") || startsWith(cr_lines[j], L"#") || cr_lines[j] == L"") continue;
					std::vector<Token> cr_tokens = tokenize(cr_lines[j]);
					if (cr_tokens[0].type == TokenKind::New) break;
					if (cr_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < cr_tokens.size(); q++) value += cr_tokens[q].data();
						if (cr_tokens[1].raw_data == L"name") { cr->name = toString(value); }
						else if (cr_tokens[1].raw_data == L"position") { cr->position = toVec2(value); }
						else if (cr_tokens[1].raw_data == L"pivot_offset") { cr->pivot_offset = toVec2(value); }
						else if (cr_tokens[1].raw_data == L"rotation") { cr->rotation = toFloat(value); }
						else if (cr_tokens[1].raw_data == L"size") { cr->size = toVec2(value); }
						else if (cr_tokens[1].raw_data == L"visible") { cr->visible = toBool(value); }
						else if (cr_tokens[1].raw_data == L"z_order") { cr->z_order = toInt(value); }
						else if (cr_tokens[1].raw_data == L"color") { cr->color = toVec4(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"hslider") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new hslider");
				Hslider* slider = currentScopeScene->addHslider(L"");
				std::wstring slider_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { slider_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> slider_lines = split(slider_src, L'\n');
				for (int j = 0; j < slider_lines.size(); j++) {
					if (startsWith(slider_lines[j], L"    ") || startsWith(slider_lines[j], L"#") || slider_lines[j] == L"") continue;
					std::vector<Token> slider_tokens = tokenize(slider_lines[j]);
					if (slider_tokens[0].type == TokenKind::New) break;
					if (slider_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < slider_tokens.size(); q++) value += slider_tokens[q].data();
						if (slider_tokens[1].raw_data == L"name") { slider->name = toString(value); }
						else if (slider_tokens[1].raw_data == L"position") { slider->position = toVec2(value); }
						else if (slider_tokens[1].raw_data == L"pivot_offset") { slider->pivot_offset = toVec2(value); }
						else if (slider_tokens[1].raw_data == L"rotation") { slider->rotation = toFloat(value); }
						else if (slider_tokens[1].raw_data == L"size") { slider->size = toVec2(value); }
						else if (slider_tokens[1].raw_data == L"visible") { slider->visible = toBool(value); }
						else if (slider_tokens[1].raw_data == L"z_order") { slider->z_order = toInt(value); }
						else if (slider_tokens[1].raw_data == L"percent") { slider->percent = toFloat(value); }
						else if (slider_tokens[1].raw_data == L"idle_color") { slider->idleColor = toVec4(value); }
						else if (slider_tokens[1].raw_data == L"hover_color") { slider->hoverColor = toVec4(value); }
						else if (slider_tokens[1].raw_data == L"down_color") { slider->downColor = toVec4(value); }
						else if (slider_tokens[1].raw_data == L"grabber_size") { slider->grabber_size = toVec2(value); }
						else if (slider_tokens[1].raw_data == L"callback") { slider->callbackList.push_back(toString(value)); }
						else if (slider_tokens[1].raw_data == L"callback_value") { slider->callbackValue = toString(value); }
						else if (slider_tokens[1].raw_data == L"background_color") { slider->backgroundColor = toVec4(value); }
					}
				}
			}
			else if (tokens[1].raw_data == L"label") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new label");
				Label* label = currentScopeScene->addLabel(L"");
				std::wstring label_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { label_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> label_lines = split(label_src, L'\n');
				for (int j = 0; j < label_lines.size(); j++) {
					if (startsWith(label_lines[j], L"    ") || startsWith(label_lines[j], L"#") || label_lines[j] == L"") continue;
					std::vector<Token> label_tokens = tokenize(label_lines[j]);
					if (label_tokens[0].type == TokenKind::New) break;
					if (label_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < label_tokens.size(); q++) value += label_tokens[q].data();
						if (label_tokens[1].raw_data == L"name") { label->name = toString(value); }
						else if (label_tokens[1].raw_data == L"text") { label->text = toString(value); label->load(label->fontPath);}
						else if (label_tokens[1].raw_data == L"font_path") { label->fontPath = checkPath(toString(value)); label->load(label->fontPath); }
						else if (label_tokens[1].raw_data == L"percent_visible") { label->percent_visible = toFloat(value); }
						else if (label_tokens[1].raw_data == L"color") { label->color = toVec4(value); }
						else if (label_tokens[1].raw_data == L"outline_color") { label->outline_color = toVec4(value); label->load(label->fontPath);}
						else if (label_tokens[1].raw_data == L"outline_size") { label->outline_size = toFloat(value); label->load(label->fontPath);}
						else if (label_tokens[1].raw_data == L"position") { label->position = toVec2(value); }
						else if (label_tokens[1].raw_data == L"size") { label->size = toVec2(value); }
						else if (label_tokens[1].raw_data == L"pivot_offset") { label->pivot_offset = toVec2(value); }
						else if (label_tokens[1].raw_data == L"font_size") { label->fontSize = toInt(value); label->load(label->fontPath);}
						else if (label_tokens[1].raw_data == L"y_separation") { label->y_separation = toFloat(value);}
						else if (label_tokens[1].raw_data == L"shaking") { label->shaking = toFloat(value);}
						else if (label_tokens[1].raw_data == L"z_order") { label->z_order = toInt(value);}
						else if (label_tokens[1].raw_data == L"wrap_text") { label->wrap_text = toBool(value);}
						else if (label_tokens[1].raw_data == L"visible") { label->visible = toBool(value);}
						else if (label_tokens[1].raw_data == L"wrap_mode") { label->wrap_mode = LabelWrapMode(toInt(value));}

					}
				}
			}
			else if (tokens[1].raw_data == L"audio") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new audio");
				Audio* ad = currentScopeScene->addAudio(L"");
				std::wstring ad_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { ad_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> ad_lines = split(ad_src, L'\n');
				for (int j = 0; j < ad_lines.size(); j++) {
					if (startsWith(ad_lines[j], L"    ") || startsWith(ad_lines[j], L"#") || ad_lines[j] == L"") continue;
					std::vector<Token> ad_tokens = tokenize(ad_lines[j]);
					if (ad_tokens[0].type == TokenKind::New) break; 
					if (ad_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < ad_tokens.size(); q++) value += ad_tokens[q].data();
						if (ad_tokens[1].raw_data == L"name") { ad->name = toString(value); }
						else if (ad_tokens[1].raw_data == L"file_path") { ad->load(checkPath(toString(value))); }
						else if (ad_tokens[1].raw_data == L"volume") { ad->volume = clamp(toFloat(value), 0.0f, 1.0f); }
					}
				}
			}
			else if (tokens[1].raw_data == L"history" || tokens[1].raw_data == L"historycontainer") {
				if (currentScopeScene == nullptr) alert(L"fatal error : out of scope : cannot make new history container");
				HistoryContainer* h = currentScopeScene->addHistoryContainer(L"");
				tempHistoryContainerList.push_back(h);
				std::wstring h_src = L"";
				for (int j = i + 1; j < lines.size(); j++) { if (startsWith(lines[j], L"    ")) { h_src += erasedFront(lines[j], 4) + L"\n"; } }
				std::vector<std::wstring> h_lines = split(h_src, L'\n');
				for (int j = 0; j < h_lines.size(); j++) {
					if (startsWith(h_lines[j], L"    ") || startsWith(h_lines[j], L"#") || h_lines[j] == L"") continue;
					std::vector<Token> h_tokens = tokenize(h_lines[j]);
					if (h_tokens[0].type == TokenKind::New) break;
					if (h_tokens[0].type == TokenKind::Define) {
						std::wstring value = L"";
						for (int q = 3; q < h_tokens.size(); q++) value += h_tokens[q].data();
						if (h_tokens[1].raw_data == L"name") { h->name = toString(value); }
						else if (h_tokens[1].raw_data == L"position") { h->position = toVec2(value); }
						else if (h_tokens[1].raw_data == L"size") { h->size = toVec2(value); }
						else if (h_tokens[1].raw_data == L"separation") { h->separation = toFloat(value); }
						else if (h_tokens[1].raw_data == L"space") { h->space = toFloat(value); }
						else if (h_tokens[1].raw_data == L"align_mode") { h->alignMode = ContainerAlignMode(toInt(value)); }
						else if (h_tokens[1].raw_data == L"visible") { h->visible = toBool(value); }
						else if (h_tokens[1].raw_data == L"z_order") { h->z_order = toInt(value); }
						else if (h_tokens[1].raw_data == L"font_path") { h->fontPath = checkPath(toString(value)); }
						else if (h_tokens[1].raw_data == L"font_size") { h->fontSize = toInt(value); }
					}
				}
			}
		}
	}

}

void save_persistent(std::wstring filePath, std::wstring varName, std::wstring content) {
	std::map<std::wstring, std::wstring> currentMap{};
	if (fileExists(ws2s(filePath)) == true) {
		std::vector<std::vector<Token>> arr{};
		std::vector<std::wstring> splitted = split(readFileW(filePath), L'\n');
		for (auto& p : splitted) { arr.push_back(tokenize(p)); }
		for (auto& p : arr) { assert(p.size() == 2);  currentMap[p[0].raw_data] = p[1].data(); }
	}
	currentMap[varName] = content;
	std::wstring finalContent = L"";
	std::map<std::wstring, std::wstring>::iterator iter = currentMap.begin();
	while (iter != currentMap.end()) {
		finalContent += L'\"' + (*iter).first + L'\"' + L" " + (*iter).second + L"\n";
		iter++;
	}
	writeFile(ws2s(filePath), multibyte2utf8(finalContent));
}

std::wstring load_persistent(std::wstring filePath, std::wstring varName) {
	if (!fileExists(ws2s(filePath))) alert(L"cannot load file :: " + filePath);
	std::map<std::wstring, std::wstring> currentMap{};
	if (fileExists(ws2s(filePath)) == true) {
		std::vector<std::vector<Token>> arr{};
		std::vector<std::wstring> splitted = split(readFileW(filePath), L'\n');
		for (auto& p : splitted) { arr.push_back(tokenize(p)); }
		for (auto& p : arr) { currentMap[p[0].raw_data] = p[1].data(); }
	}
	return currentMap[varName];
}

void interpret_script(std::wstring src, bool isMainScope, bool isIfScope, bool isWhileScope, bool isCallback) {
	Dialog* currentDialog = ((Scene2D*)root->currentScene)->currentDialog;
	ChoiceBox* currentChoice = ((Scene2D*)root->currentScene)->currentChoiceBox;
	int choiceResult = -1;
	std::vector<std::wstring> src_lines = eraseLineBreak(split(src, L'\n'));
	std::vector<std::wstring>::iterator iter = src_lines.begin();
	while (iter != src_lines.end()) {
		if (startsWith((*iter), L"#")) iter = src_lines.erase(iter);
		else iter++;
	}
	for (int i = 0; i < src_lines.size(); i++) {
		if (startsWith(src_lines[i], L"    ") || startsWith(src_lines[i], L"#") || src_lines[i] == L"") continue;
		std::vector<Token> src_tokens = tokenize(src_lines[i]);
		if (src_tokens[0].type == TokenKind::Include) {
			src_lines.erase(src_lines.begin() + i);
			std::wstring include_src = readFileW(checkPath(src_tokens[1].raw_data));
			std::vector<std::wstring> include_lines = eraseLineBreak(split(include_src, L'\n'));
			std::vector<std::wstring>::iterator include_iter = include_lines.begin();
			while (include_iter != include_lines.end()) {
				if (startsWith((*include_iter), L"#")) include_iter = include_lines.erase(include_iter);
				else include_iter++;
			}
			src_lines.insert(src_lines.begin() + i, include_lines.begin(), include_lines.end());
			i += max(0, include_lines.size() - 1);
		}
	}
	for (int index = 0; index < src_lines.size(); index++) {
		CHECK_EXIT;
		if (startsWith(src_lines[index], L"    #") == true) continue;
		std::vector<Token> cur_tokens = tokenize(src_lines[index]);
		if (cur_tokens.size() == 0) continue;
		if (cur_tokens[0].type == TokenKind::Label) {
			Token identifier = cur_tokens[1];
			std::wstring labelSource = L"";
			for (int i = index + 1; i < src_lines.size(); i++) {
				if (startsWith(src_lines[i], L"    ")) {
					labelSource += erasedFront(src_lines[i], 4) + L"\n";
				}
				else break;
			}
			Sgt::instance().labelMap[identifier.raw_data] = labelSource;
		}
	}
	for (int index = 0; index < src_lines.size(); index++) {
		if (isCallback == false) {
			if (currentGlobalIndex < goalGlobalIndex) {
				is_skipping_to_last = true;
			}
			else {
				goalGlobalIndex = -1;
				is_skipping_to_last = false;
			}
		}
		if (is_returned_by_ifstate == true) {
			if (isIfScope == false && isWhileScope == false) {
				is_returned_by_ifstate = false;
			}
			return;
		}
		if (is_breaked_by_ifstate) {
			return;
		}
		if (startsWith(src_lines[index], L"    ") || startsWith(src_lines[index], L"#") || src_lines[index] == L"") continue;
		std::vector<Token> cur_tokens = tokenize(src_lines[index]);
		if (cur_tokens.size() == 0) continue;
		if (cur_tokens[0].type == TokenKind::Print) {
			std::wstring src = L"";
			for (int i = 1; i < cur_tokens.size(); i++) {
				if (cur_tokens[i].type == TokenKind::String) {
					src += cur_tokens[i].data();
				}
				else {
				src += cur_tokens[i].raw_data;
				}
			}
			std::wcout << eval(src, false).raw_data << "\n";
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::IntDef) {
			int k = 1;
			while (k < cur_tokens.size()-1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring integer");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = std::to_wstring(toInt(cur_tokens[k + 1].raw_data));
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::FloatDef) {
			int k = 1;
			while (k < cur_tokens.size() - 1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring float");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = std::to_wstring(toFloat(cur_tokens[k + 1].raw_data));
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::StringDef) {
			int k = 1;
			while (k < cur_tokens.size() - 1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring string");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = cur_tokens[k + 1].raw_data;
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::CharDef) {
			Token identifier = cur_tokens[1];
			Token name = cur_tokens[2];
			if (cur_tokens.size() > 3) {
				Token color = cur_tokens[3];
				Sgt::instance().charMap[identifier.raw_data] = character(name.raw_data, toVec4(color.raw_data));
			}
			else {
				Sgt::instance().charMap[identifier.raw_data] = character(name.raw_data);
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::BoolDef) {
			int k = 1;
			while (k < cur_tokens.size() - 1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring boolean");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = toBool(cur_tokens[k + 1].raw_data) == true ? L"true" : L"false";
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::ArrDef) {
			Token identifier = cur_tokens[1];
			std::wstring arr_src = L"[";
			for (int i = 2; i < cur_tokens.size(); i++) {
				arr_src += cur_tokens[i].data();
			}
			arr_src += L"]";
			Sgt::instance().varMap[identifier.raw_data] = arr_src;
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::Vector2Def) {
			int k = 1;
			while (k < cur_tokens.size() - 1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring vector2");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = cur_tokens[k + 1].raw_data;
				assert(getVariantType(cur_tokens[k].raw_data) == TokenKind::Vector2);
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::Vector4Def) {
			int k = 1;
			while (k < cur_tokens.size() - 1) {
				if (cur_tokens[k].type == TokenKind::Comma) k++;
				if (k >= cur_tokens.size()) alert(L"you misused comma while declaring vector4");
				Sgt::instance().varMap[cur_tokens[k].raw_data] = cur_tokens[k + 1].raw_data;
				assert(getVariantType(cur_tokens[k].raw_data) == TokenKind::Vector4);
				k += 2;
				if (cur_tokens.size() > k + 1 && cur_tokens[k + 1].type == TokenKind::Comma) {
					k++;
				}
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::Char) {
			character c = toChar(cur_tokens[0].raw_data);
			if (not is_skipping_to_last
				) {
				currentDialog->nameLabel->color = c.color;
				currentDialog->say(c.full_name, replaceVariable(cur_tokens[1].raw_data));
				if (not is_skipping_to_last) {
					while (currentDialog->progressed == false) {
						CHECK_EXIT;
					}
					currentDialog->state = DialogState::Idle;
				}
			}
			if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
				((Scene2D*)root->currentScene)->currentHistory->add_history(c.full_name, replaceVariable(cur_tokens[1].raw_data));
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::String) {
			if (cur_tokens.size() == 1) {
				if (is_skipping_to_last == false) {
					currentDialog->say(L"", replaceVariable(cur_tokens[0].raw_data));
					if (not is_skipping_to_last) {
						while (currentDialog->progressed == false) {
							CHECK_EXIT;
						}
						currentDialog->state = DialogState::Idle;
					}
				}
				if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
					((Scene2D*)root->currentScene)->currentHistory->add_history(L"", replaceVariable(cur_tokens[0].raw_data));
				}
				if (isCallback == false) currentGlobalIndex += 1;
			}
			else {
				currentGlobalIndex += 1;
				std::vector<Token> modified_tokens = tokenize(src_lines[index], true);
				if (modified_tokens.size() < 3) {
					continue;
				}
				if (modified_tokens[1].type != TokenKind::Assign) alert(L"please write assign opeator '=' in code " + src_lines[index]);
				std::wstring targetVarName = modified_tokens[0].raw_data;
				std::wstring targetAssignedValue = L"";
				for (int i = 2; i < modified_tokens.size(); i++) {
					targetAssignedValue += modified_tokens[i].data() + L" ";
				}
				if (Sgt::instance().varExists(targetVarName) == false) alert(L"there is no variable called " + targetVarName);
				Sgt::instance().varMap[targetVarName] = eval(targetAssignedValue, false).raw_data;
			}
		}
		else if (cur_tokens[0].type == TokenKind::Choice) {
			assert(choiceResult == -1);
			if (not is_skipping_to_last) {
				if (cur_tokens.size() > 1) {
					assert(Sgt::instance().varExists(cur_tokens[1].raw_data));
					assert(Sgt::instance().varExists(cur_tokens[2].raw_data));
					std::wstring targetStringVarName = (cur_tokens[1].raw_data);
					std::vector<Token> inputArray = tokenize(eraseBeginEnd(Sgt::instance().varMap[cur_tokens[2].raw_data]));
					std::vector<std::wstring> choiceVec{};
					std::pair < character, std::wstring> saySrc{};
					for (int i = 0; i < inputArray.size(); i+=2) { choiceVec.push_back(inputArray[i].raw_data); }
					if (index + 1 < src_lines.size() && startsWith(src_lines[index + 1], L"    ")) {
						std::vector<Token> say_tokens = tokenize(erasedFront(src_lines[index + 1], 4));
						if (say_tokens.size() == 1) {
							saySrc = std::pair<character, std::wstring>(character(), say_tokens[0].raw_data);
						}
						else {
							saySrc = std::pair<character, std::wstring>(toChar(say_tokens[0].raw_data), say_tokens[1].raw_data);
						}
					}
					if (choiceVec.size() != 0) {
						if (currentChoice == nullptr || currentDialog == nullptr){
							return;
						}
						currentChoice->choice(choiceVec, &choiceResult);
						if (saySrc.second != L"") {
							currentDialog->nameLabel->color = saySrc.first.color;
							currentDialog->say(saySrc.first.full_name, replaceVariable(saySrc.second), false);
							if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
								((Scene2D*)root->currentScene)->currentHistory->add_history(saySrc.first.full_name, replaceVariable(saySrc.second));
							}
						}
						while (choiceResult == -1) {
							CHECK_EXIT;
						}
						savedChoiceQueue.push(choiceResult);
						if (saySrc.second != L"") currentDialog->state = DialogState::Idle;
						if (isCallback == false) currentGlobalIndex++;
						Sgt::instance().varMap[targetStringVarName] = choiceVec[choiceResult];
						choiceResult = -1;
					}
				}
				else {
					int k = index + 1;
					std::vector<std::wstring> caseAndSaySrc{};
					std::pair<character, std::wstring> saySrc{};
					std::vector<std::pair<std::wstring, std::wstring>> caseSrc{};

					while (k < src_lines.size() && startsWith(src_lines[k], L"    ") == true) { caseAndSaySrc.push_back(erasedFront(src_lines[k], 4)); k++; }
					for (int i = 0; i < caseAndSaySrc.size(); i++) {
						if (startsWith(caseAndSaySrc[i], L"    ") == true) continue;
						if (startsWith(caseAndSaySrc[i], L"#") == true) continue;
						std::vector<Token> cas_tokens = tokenize(caseAndSaySrc[i]);
						if (cas_tokens.size() == 0) continue;
						if (cas_tokens[0].type == TokenKind::Char && saySrc.second == L"") {
							saySrc = std::pair<character, std::wstring>(toChar(cas_tokens[0].raw_data), cas_tokens[1].raw_data);
						}
						else if (cas_tokens[0].type == TokenKind::String && saySrc.second == L"") {
							saySrc = std::pair<character, std::wstring>(character(), cas_tokens[0].raw_data);
						}
						else if (cas_tokens[0].type == TokenKind::Case) {
							std::wstring info = cas_tokens[1].raw_data;
							std::wstring source = L"";
							int j = i + 1;
							while (j < caseAndSaySrc.size() && startsWith(caseAndSaySrc[j], L"    ") == true) { source += erasedFront(caseAndSaySrc[j], 4) + L"\n"; j++; }
							caseSrc.push_back({ info, source });
						}
					}
					if (caseSrc.size() != 0) {
						if (currentChoice == nullptr || currentDialog == nullptr) {
							return;
						}
						std::vector<std::wstring> choiceVec{};
						for (auto& p : caseSrc) choiceVec.push_back(p.first);
						currentChoice->choice(choiceVec, &choiceResult);
						if (saySrc.second != L"") {
							currentDialog->nameLabel->color = saySrc.first.color;
							currentDialog->say(saySrc.first.full_name, replaceVariable(saySrc.second), false);
							if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
								((Scene2D*)root->currentScene)->currentHistory->add_history(saySrc.first.full_name, replaceVariable(saySrc.second));
							}
						}
						while (choiceResult == -1) {
							CHECK_EXIT;
						}
						savedChoiceQueue.push(choiceResult);
						if (saySrc.second != L"") currentDialog->state = DialogState::Idle;
						if (isCallback == false) currentGlobalIndex++;
						int tempChoiceResult = choiceResult;
						choiceResult = -1;
						interpret_script(caseSrc[tempChoiceResult].second, false, TRUE, false, isCallback);
					}
				}
			}
			else {
				if (cur_tokens.size() > 1) {
					assert(Sgt::instance().varExists(cur_tokens[1].raw_data));
					assert(Sgt::instance().varExists(cur_tokens[2].raw_data));
					std::wstring targetStringVarName = (cur_tokens[1].raw_data);
					std::vector<Token> inputArray = tokenize(eraseBeginEnd(Sgt::instance().varMap[cur_tokens[2].raw_data]));
					std::pair < character, std::wstring> saySrc{};
					std::vector<std::wstring> choiceVec{};
					for (int i = 0; i < inputArray.size(); i += 2) { choiceVec.push_back(inputArray[i].raw_data); }
					if (index + 1 < src_lines.size() && startsWith(src_lines[index + 1], L"    ")) {
						std::vector<Token> say_tokens = tokenize(erasedFront(src_lines[index + 1], 4));
						if (say_tokens.size() == 1) {
							saySrc = std::pair<character, std::wstring>(character(), say_tokens[0].raw_data);
						}
						else {
							saySrc = std::pair<character, std::wstring>(toChar(say_tokens[0].raw_data), say_tokens[1].raw_data);
						}
					}
					assert(choiceQueue.empty() == false);
					if (saySrc.second != L"") {
						if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
							((Scene2D*)root->currentScene)->currentHistory->add_history(saySrc.first.full_name, replaceVariable(saySrc.second));
						}
					}
					if (choiceQueue.empty()) alert(L"choice queue is empty");
					if (isCallback == false) currentGlobalIndex++;
					Sgt::instance().varMap[targetStringVarName] = choiceVec[choiceQueue.front()];
					choiceQueue.pop();
				}
				else {
					int k = index + 1;
					std::vector<std::wstring> caseAndSaySrc{};
					std::pair<character, std::wstring> saySrc{};
					std::vector<std::pair<std::wstring, std::wstring>> caseSrc{};
					while (k < src_lines.size() && startsWith(src_lines[k], L"    ") == true) { caseAndSaySrc.push_back(erasedFront(src_lines[k], 4)); k++; }
					for (int i = 0; i < caseAndSaySrc.size(); i++) {
						if (startsWith(caseAndSaySrc[i], L"    ") == true) continue;
						if (startsWith(caseAndSaySrc[i], L"#") == true) continue;
						std::vector<Token> cas_tokens = tokenize(caseAndSaySrc[i]);
						if (cas_tokens.size() == 0) continue;
						if (cas_tokens[0].type == TokenKind::Char && saySrc.second == L"") {
							saySrc = std::pair<character, std::wstring>(toChar(cas_tokens[0].raw_data), cas_tokens[1].raw_data);
						}
						else if (cas_tokens[0].type == TokenKind::String && saySrc.second == L"") {
							saySrc = std::pair<character, std::wstring>(character(), cas_tokens[0].raw_data);
						}
						else if (cas_tokens[0].type == TokenKind::Case) {
							std::wstring info = cas_tokens[1].raw_data;
							std::wstring source = L"";
							int j = i + 1;
							while (j < caseAndSaySrc.size() && startsWith(caseAndSaySrc[j], L"    ") == true) { source += erasedFront(caseAndSaySrc[j], 4) + L"\n"; j++; }
							caseSrc.push_back({ info, source });
						}
					}
					if (saySrc.second != L"") {
						if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
							((Scene2D*)root->currentScene)->currentHistory->add_history(saySrc.first.full_name, replaceVariable(saySrc.second));
						}
					}
					if (choiceQueue.empty()) alert(L"choice queue is empty");
					if (isCallback == false) currentGlobalIndex++;
					int tempChoice = choiceQueue.front();
					choiceQueue.pop();
					interpret_script(caseSrc[tempChoice].second, false, TRUE, false, isCallback);
				}
			}
		}
		else if (cur_tokens[0].type == TokenKind::Case) {
		}
		else if (cur_tokens[0].type == TokenKind::Label) {
		}
		else if (cur_tokens[0].type == TokenKind::Jump) {
			if (Sgt::instance().labelExists(cur_tokens[1].raw_data) == false) alert(L"there's no label called " + cur_tokens[1].raw_data);
			std::wstring another_src = Sgt::instance().labelMap[cur_tokens[1].raw_data];
			interpret_script(another_src, isMainScope, false, false, isCallback);
			return;
		}
		else if (cur_tokens[0].type == TokenKind::Call) {
			if (Sgt::instance().labelExists(cur_tokens[1].raw_data) == false) alert(L"there's no label called " + cur_tokens[1].raw_data);
			std::wstring another_src = Sgt::instance().labelMap[cur_tokens[1].raw_data];
			interpret_script(another_src, isMainScope, false, false, isCallback);
		}
		else if (cur_tokens[0].type == TokenKind::JumpMain) {
			if (Sgt::instance().labelExists(cur_tokens[1].raw_data) == false) alert(L"there's no label called " + cur_tokens[1].raw_data);
			std::wstring another_src = Sgt::instance().labelMap[cur_tokens[1].raw_data];
			interpret_script(another_src, isMainScope, false, false, false);
			return;
		}
		else if (cur_tokens[0].type == TokenKind::CallMain) {
			if (Sgt::instance().labelExists(cur_tokens[1].raw_data) == false) alert(L"there's no label called " + cur_tokens[1].raw_data);
			std::wstring another_src = Sgt::instance().labelMap[cur_tokens[1].raw_data];
			interpret_script(another_src, isMainScope, false, false, false);
		}
		else if (cur_tokens[0].type == TokenKind::Return) {
			if (isIfScope == true) {
				is_returned_by_ifstate = true;
				is_breaked_by_ifstate = true;
			}
			else if (isWhileScope == true) {
				is_returned_by_ifstate = true;
				is_breaked_by_ifstate = true;
			}
			if (isCallback == false) currentGlobalIndex += 1;
			return;
		}
		else if (cur_tokens[0].type == TokenKind::If) {
			std::wstring judgeTarget = erasedFront(src_lines[index], 3);
			if (eval(judgeTarget).type == TokenKind::True) {
				std::wstring ifSrc = L"";
				int k = index + 1;
				while (k < src_lines.size() && startsWith(src_lines[k], L"    ") == true) { 
					ifSrc += erasedFront(src_lines[k], 4) + L"\n";  
					k++;
				}
				if (isCallback == false) currentGlobalIndex += 1;
				interpret_script(ifSrc, false, TRUE, isWhileScope, isCallback);
			}
			else {
				std::vector<std::pair<std::wstring, std::wstring>> elifList{};
				int k = index + 1;
				while (k < src_lines.size() && startsWith(src_lines[k], L"if ") == false) {
					if (!startsWith(src_lines[k], L"    ")) {
						std::vector<Token> temp_tokens = tokenize(src_lines[k]);
						if (k < src_lines.size() && temp_tokens.size() > 0 && temp_tokens[0].type == TokenKind::Elif) {
							std::wstring elifJudgeTarget = erasedFront(src_lines[k], 5);
							std::wstring elifSrc = L"";
							k++;
							while (k < src_lines.size() && startsWith(src_lines[k], L"    ")) {
								elifSrc += erasedFront(src_lines[k], 4) + L"\n";
								k++;
							}
							k--;
							elifList.push_back({ elifJudgeTarget, elifSrc });
						}
					}
					k++;
				}
				bool elif_sucess = false;
				for (auto& element : elifList) {
					if (eval(element.first).type == TokenKind::True) {
						if (isCallback == false) currentGlobalIndex += 1;
						interpret_script(element.second, false, TRUE, isWhileScope, isCallback);
						elif_sucess = true;
						break;
					}
				}
				if (elif_sucess == false) {
					int j = index + 1;
					while (j < src_lines.size() && startsWith(src_lines[j], L"if ") == false) {
						if (!startsWith(src_lines[j], L"    ")) {
							std::vector<Token> temp_tokens = tokenize(src_lines[j]);
							if (temp_tokens.size() > 0 && temp_tokens[0].type == TokenKind::Else) {
								j++;
								std::wstring elseSrc = L"";
								while (j < src_lines.size() && startsWith(src_lines[j], L"    ")) {
									elseSrc += erasedFront(src_lines[j], 4) + L"\n";
									j++;
								}
								j--;
								if (isCallback == false) currentGlobalIndex += 1;
								interpret_script(elseSrc, false, TRUE, isWhileScope, isCallback);
								break;
							}
						}
						j++;
					}
				}
			}


		}
		else if (cur_tokens[0].type == TokenKind::Elif) {
		}
		else if (cur_tokens[0].type == TokenKind::Else) {
		}
		else if (cur_tokens[0].type == TokenKind::Show) {
			if (isCallback == false) currentGlobalIndex += 1;
			std::wstring imageName = cur_tokens[1].raw_data;
			std::wstring imageTag = cur_tokens[2].raw_data;
			Sprite2D* sprite = ((Scene2D*)root->currentScene)->getSprite2D(imageName, imageTag);
			std::vector<std::wstring> withSrc{};
			int k = index + 1;
			while (k < src_lines.size() && startsWith(src_lines[k], L"    with")) {
				withSrc.push_back(erasedFront(src_lines[k], 4));
				k++;
			}
			sprite->visible = true;
			for (auto& p : withSrc) {
				std::vector<Token> with_tokens = tokenize(p);
				assert(with_tokens.size() > 1);
				if (with_tokens[1].type == TokenKind::Await) {
					if (with_tokens[2].raw_data == L"fade") {
						bool result = false;
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 0), vec4(1, 1, 1, 1), toFloat(with_tokens[3].raw_data), &result);
						if (not is_skipping_to_last) while (result == false);
					}
					else if (with_tokens[2].raw_data == L"dissolve") {
						bool result = false;
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 0), vec4(1, 1, 1, 1), toFloat(with_tokens[3].raw_data), &result);
						for (auto& p : ((Scene2D*)root->currentScene)->getSprite2DListExcept(imageName, imageTag)) {
							((Scene2D*)root->currentScene)->currentTween->linear_interpolate(p, L"modulate", vec4(1, 1, 1, 1), vec4(1, 1, 1, 0), toFloat(with_tokens[3].raw_data), &result);
						}
						if (not is_skipping_to_last) while (result == false);
					}
					else if (with_tokens[2].raw_data == L"move") {
						bool result = false;
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"position", toVec2(with_tokens[4].raw_data), toVec2(with_tokens[5].raw_data), toFloat(with_tokens[3].raw_data), &result);
						if (not is_skipping_to_last) while (result == false);
					}
				}
				else {
					if (with_tokens[1].raw_data == L"fade") {
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 0), vec4(1, 1, 1, 1), toFloat(with_tokens[2].raw_data), nullptr);
					}
					else if (with_tokens[1].raw_data == L"dissolve") {
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 0), vec4(1, 1, 1, 1), toFloat(with_tokens[2].raw_data), nullptr);
						for (auto& p : ((Scene2D*)root->currentScene)->getSprite2DListExcept(imageName, imageTag)) {
							((Scene2D*)root->currentScene)->currentTween->linear_interpolate(p, L"modulate", vec4(1, 1, 1, 1), vec4(1, 1, 1, 0), toFloat(with_tokens[2].raw_data), nullptr);
						}
					}
					else if (with_tokens[1].raw_data == L"move") {
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"position", toVec2(with_tokens[3].raw_data), toVec2(with_tokens[4].raw_data), toFloat(with_tokens[2].raw_data), nullptr);
					}
				}
			}
			for (auto& p : ((Scene2D*)root->currentScene)->getSprite2DListExcept(imageName, imageTag)) {
				p->visible = false;
			}
		}
		else if (cur_tokens[0].type == TokenKind::Hide) {
			if (isCallback == false) currentGlobalIndex += 1;
			std::wstring imageName = cur_tokens[1].raw_data;
			std::wstring imageTag = cur_tokens[2].raw_data;
			Sprite2D* sprite = ((Scene2D*)root->currentScene)->getSprite2D(imageName, imageTag);
			std::vector<std::wstring> withSrc{};
			int k = index + 1;
			while (k < src_lines.size() && startsWith(src_lines[k], L"    with")) {
				withSrc.push_back(erasedFront(src_lines[k], 4));
				k++;
			}
			sprite->visible = true;
			for (auto& p : withSrc) {
				std::vector<Token> with_tokens = tokenize(p);
				assert(with_tokens.size() > 1);
				if (with_tokens[1].type == TokenKind::Await) {
					if (with_tokens[2].raw_data == L"fade") {
						bool result = false;
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 1), vec4(1, 1, 1, 0), toFloat(with_tokens[3].raw_data), &result);
						if (not is_skipping_to_last) while (result == false);
					}
					else if (with_tokens[2].raw_data == L"move") {
						bool result = false;
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"position", toVec2(with_tokens[4].raw_data), toVec2(with_tokens[5].raw_data), toFloat(with_tokens[3].raw_data), &result);
						if (not is_skipping_to_last) while (result == false);
					}
				}
				else {
					if (with_tokens[1].raw_data == L"fade") {
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"modulate", vec4(1, 1, 1, 1), vec4(1, 1, 1, 0), toFloat(with_tokens[2].raw_data), nullptr);
					}
					else if (with_tokens[1].raw_data == L"move") {
						((Scene2D*)root->currentScene)->currentTween->linear_interpolate(sprite, L"position", toVec2(with_tokens[3].raw_data), toVec2(with_tokens[4].raw_data), toFloat(with_tokens[2].raw_data), nullptr);
					}
				}
			}
			sprite->visible = false;
		}
		else if (cur_tokens[0].type == TokenKind::Await) {
			if (not is_skipping_to_last) {
				if (cur_tokens[1].type == TokenKind::Float) {
					double curTime = root->currentTime;
					float waitTime = toFloat(cur_tokens[1].raw_data);
					double endTime = curTime + (waitTime);
					while (root->currentTime < endTime);
				}
			}
		}
		else if (cur_tokens[0].type == TokenKind::Scene) {
			if (isCallback == false) currentGlobalIndex += 1;
			for (auto& p : ((Scene2D*)root->currentScene)->audioList) {
				p->stop();
				ma_device_uninit(&p->device);
				ma_decoder_uninit(&p->decoder);
				p->load(s2ws(p->filePath));
			}
			for (auto& p : ((Scene2D*)root->currentScene)->objectList) {
				if (p->type == L"Tween") {
					((Tween*)p)->processingList.clear();
				}
			}
			root->currentScene->currentFocused = nullptr;
			std::wstring newSceneName = cur_tokens[1].raw_data;
			root->currentScene = root->getScene2D(newSceneName);
			root->currentScene->currentFocused = nullptr;
			currentDialog = ((Scene2D*)root->currentScene)->currentDialog;
			currentChoice = ((Scene2D*)root->currentScene)->currentChoiceBox;
		}
		else if (cur_tokens[0].type == TokenKind::Input) {
			if (is_skipping_to_last == true) {
				std::wstring targetVarName = cur_tokens[2].raw_data;
				Sgt::instance().varMap[targetVarName] = inputQueue.front();
				inputQueue.pop();
			}
			else {
				std::wstring result = L"\n";
				std::wstring editName = cur_tokens[1].raw_data;
				std::wstring targetVarName = cur_tokens[2].raw_data;
				int limit = toInt(cur_tokens[3].raw_data);
				TextEdit* t = (TextEdit*)((Scene2D*)root->currentScene)->getObject(L"TextEdit", editName);
				t->input(&result, limit);
				while (result == L"\n");
				assert(Sgt::instance().varExists(targetVarName));
				Sgt::instance().varMap[targetVarName] = result;
				savedInputQueue.push(result);
			}
			if (isCallback == false) currentGlobalIndex += 1;
		}
		else if (cur_tokens[0].type == TokenKind::Quit) {
			glfwSetWindowShouldClose(root->window, true);
			return;
		}
		else if (cur_tokens[0].type == TokenKind::Thread) {
			assert(Sgt::instance().labelExists(cur_tokens[1].raw_data));
			if (isCallback == false) currentGlobalIndex += 1;
			std::thread t(interpret_script, Sgt::instance().labelMap[cur_tokens[1].raw_data], isMainScope, false, false, false);
			t.detach();
			if (t.joinable()) t.join();
		}
		else if (cur_tokens[0].type == TokenKind::Break) {
			if (isCallback == false) currentGlobalIndex += 1;
			if ((isIfScope == true && isWhileScope == true) ||(isIfScope == false && isWhileScope == true)) {
				is_breaked_by_ifstate = true;
				return;
			}
			else {
				alert(L"you misused the break keyword : please use it when you want to break while loop");
			}
		}
		else if (cur_tokens[0].type == TokenKind::While) {
			std::wstring condition = L"";
			for (int k = 1; k < cur_tokens.size(); k++) {
				condition += tokenize(src_lines[index])[k].raw_data + L" ";
			}
			std::wstring source = L"";
			int k = index + 1;
			while (k < src_lines.size() && startsWith(src_lines[k], L"    ") == true) {
				source += erasedFront(src_lines[k], 4) + L"\n";
				k++;
			}
			while (eval(condition).type == TokenKind::True) {
				CHECK_EXIT;
				if (isCallback == false) currentGlobalIndex += 1;
				interpret_script(source, false, isIfScope, true, isCallback);
				if (is_returned_by_ifstate == true) {
					if (isIfScope == false && isWhileScope == false) {
						is_returned_by_ifstate = false;
					}
					return;
				}
				if (is_breaked_by_ifstate == true) {
					is_breaked_by_ifstate = false;
					break;
				}
				condition = L"";
				for (int q = 1; q < cur_tokens.size(); q++) {
					condition += tokenize(src_lines[index])[q].raw_data + L" ";
				}
			}
		}
		else if (cur_tokens[0].type == TokenKind::Command) {
			if (isCallback == false) currentGlobalIndex += 1;
			assert(cur_tokens.size() > 1);
			std::vector<Token> values = cur_tokens; 
			if (cur_tokens.size() > 2) {
				values.erase(values.begin()); values.erase(values.begin());
			}
			std::vector<Token> params = cur_tokens; 
			if (cur_tokens.size() > 3) {
				params.erase(params.begin()); params.erase(params.begin()); params.erase(params.begin());
			}
			if (cur_tokens[1].raw_data == L"set_visible") {
				bool flag = toBool(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data)->visible = flag;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_visible") {
				bool flag = toBool(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->visible = flag;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_position") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->position = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_position") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->position = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_scale") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->scale = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_size") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->size = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_scale") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->scale = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_rotation") {
				float f = toFloat(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->rotation = f;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_rotation") {
				float f = toFloat(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->rotation = f;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_pivot_offset") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->pivot_offset = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_pivot_offset") {
				vec2 v = toVec2(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->pivot_offset = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_z_order") {
				float ii = toInt(values[0].raw_data);
				for (int i = 0; i < params.size(); i++) {
					((Object2D*)((Scene2D*)root->currentScene)->getObjectN(params[i].raw_data))->z_order = ii;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_z_order") {
				float ii = toInt(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->z_order = ii;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_camera_zoom") {
				vec2 v = toVec2(values[0].raw_data);
				((Camera2D*)((Scene2D*)root->currentScene)->currentCamera)->zoom = v;
			}
			else if (cur_tokens[1].raw_data == L"set_sprite_modulate") {
				vec4 v = toVec4(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((Scene2D*)root->currentScene)->getSprite2D(params[i].raw_data, params[i + 1].raw_data)->modulate = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"set_color_rect_color") {
				vec4 v = toVec4(values[0].raw_data);
				for (int i = 0; i < params.size(); i += 2) {
					((ColorRect*)((Scene2D*)root->currentScene)->getObject(L"ColorRect", params[i].raw_data))->color = v;
				}
			}
			else if (cur_tokens[1].raw_data == L"open_url") {
				ShellExecute(0, 0, (LPCWSTR)checkPath(cur_tokens[2].raw_data).c_str(), 0, 0, SW_SHOW);
			}
			else if (cur_tokens[1].raw_data == L"set_window_fullscreen") {
				root->setWindowFullscreen(toBool(cur_tokens[2].raw_data));
			}
			else if (cur_tokens[1].raw_data == L"set_dialog_speed") {
				if (((Scene2D*)root->currentScene)->currentDialog != nullptr)
					((Scene2D*)root->currentScene)->currentDialog->speed = toFloat(cur_tokens[2].raw_data);
			}
			else if (cur_tokens[1].raw_data == L"set_hslider_percent") {
				if (((Scene2D*)root->currentScene)->getObject(L"Hslider", cur_tokens[2].raw_data) != nullptr) {
					((Hslider*)((Scene2D*)root->currentScene)->getObject(L"Hslider", cur_tokens[2].raw_data))->percent = clamp(toFloat(cur_tokens[3].raw_data), 0.0, 1.0);
				}
			}
			else if (cur_tokens[1].raw_data == L"reset_dialog") {
				if (((Scene2D*)root->currentScene)->currentDialog != nullptr)
					((Scene2D*)root->currentScene)->currentDialog->reset();
			}
			else if (cur_tokens[1].raw_data == L"reset_choice") {
				if (((Scene2D*)root->currentScene)->currentChoiceBox != nullptr)
					((Scene2D*)root->currentScene)->currentChoiceBox->reset();
			}
			else if (cur_tokens[1].raw_data == L"reset_textedit") {
				if (((Scene2D*)root->currentScene)->getObject(L"TextEdit", cur_tokens[2].raw_data) != nullptr) {
					((TextEdit*)((Scene2D*)root->currentScene)->getObject(L"TextEdit", cur_tokens[2].raw_data))->reset();
				}
			}
			else if (cur_tokens[1].raw_data == L"reset_charmap") {
				Sgt::instance().charMap.clear();
			}
			else if (cur_tokens[1].raw_data == L"reset_varmap") {
				Sgt::instance().varMap.clear();
			}
			else if (cur_tokens[1].raw_data == L"reset_history") {
				if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
					((Scene2D*)root->currentScene)->currentHistory->reset_history();
				}
			}
			else if (cur_tokens[1].raw_data == L"reset_current_focused") {
				if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
					((Scene2D*)root->currentScene)->currentHistory->reset_history();
				}
			}
			else if (cur_tokens[1].raw_data == L"reset_all") {
				force_return = true;
				if (((Scene2D*)root->currentScene)->currentHistory != nullptr) {
					((Scene2D*)root->currentScene)->currentHistory->reset_history();
				}
				if (((Scene2D*)root->currentScene)->currentChoiceBox != nullptr){
					((Scene2D*)root->currentScene)->currentChoiceBox->reset();
				}
				if (((Scene2D*)root->currentScene)->currentDialog != nullptr) {
					((Scene2D*)root->currentScene)->currentDialog->reset();
				}
				savedChoiceQueue = std::queue<int>();
				choiceQueue = std::queue<int>();
				savedInputQueue = std::queue<std::wstring>();
				inputQueue = std::queue<std::wstring>();
				goalGlobalIndex = -1;
				force_return = false;
			}
			else if (cur_tokens[1].raw_data == L"add_history") {
				((Scene2D*)root->currentScene)->currentHistory->add_history(cur_tokens[2].raw_data, cur_tokens[3].raw_data);
			}
			else if (cur_tokens[1].raw_data == L"pause_dialog") {
				((Scene2D*)root->currentScene)->dialogPaused = toBool(cur_tokens[2].raw_data);
			}
		}
		else if (cur_tokens[0].type == TokenKind::Tween) {
			if (isCallback == false) currentGlobalIndex += 1;
			assert(cur_tokens.size() > 4);
			int ptr = 0;
			if (cur_tokens[1].type == TokenKind::Await) ptr = 1;
			std::wstring propertyName = cur_tokens[ptr + 1].raw_data;
			std::wstring objectName = cur_tokens[ptr + 2].raw_data;
			if (((Scene2D*)root->currentScene)->getObjectN(objectName) == nullptr) { currentGlobalIndex++; continue; }
			if (propertyName == L"position:x") {
				Object2D* object = (Object2D*)((Scene2D*)root->currentScene)->getObjectN(objectName);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 3].raw_data);
				float from = toFloat(cur_tokens[ptr + 4].raw_data);
				float to = toFloat(cur_tokens[ptr + 5].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", vec2(from, object->position.y), vec2(to, object->position.y), duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position.x = to;
				}
			}
			else if (propertyName == L"position:y") {
				Object2D* object = (Object2D*)((Scene2D*)root->currentScene)->getObjectN(objectName);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 3].raw_data);
				float from = toFloat(cur_tokens[ptr + 4].raw_data);
				float to = toFloat(cur_tokens[ptr + 5].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", vec2(object->position.x, from), vec2(object->position.x, to), duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position.y = to;
				}
			}
			else if (propertyName == L"position") {
				Object2D* object = (Object2D*)((Scene2D*)root->currentScene)->getObjectN(objectName);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 3].raw_data);
				vec2 from = toVec2(cur_tokens[ptr + 4].raw_data);
				vec2 to = toVec2(cur_tokens[ptr + 5].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", from, to, duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position = to;
				}
			}
			else if (propertyName == L"sprite_position:x") {
				Sprite2D* object = ((Scene2D*)root->currentScene)->getSprite2D(objectName, cur_tokens[ptr + 3].raw_data);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 4].raw_data);
				float from = toFloat(cur_tokens[ptr + 5].raw_data);
				float to = toFloat(cur_tokens[ptr + 6].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", vec2(from, object->position.y), vec2(to, object->position.y), duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position.x = to;
				}
			}
			else if (propertyName == L"sprite_position:y") {
				Sprite2D* object = ((Scene2D*)root->currentScene)->getSprite2D(objectName, cur_tokens[ptr + 3].raw_data);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 4].raw_data);
				float from = toFloat(cur_tokens[ptr + 5].raw_data);
				float to = toFloat(cur_tokens[ptr + 6].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", vec2(object->position.x, from), vec2(object->position.x, to), duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position.y = to;
				}
			}
			else if (propertyName == L"sprite_position") {
				Sprite2D* object = ((Scene2D*)root->currentScene)->getSprite2D(objectName, cur_tokens[ptr + 3].raw_data);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 4].raw_data);
				vec2 from = toVec2(cur_tokens[ptr + 5].raw_data);
				vec2 to = toVec2(cur_tokens[ptr + 6].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"position", from, to, duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->position = to;
				}
			}
			else if (propertyName == L"sprite_scale") {
				Sprite2D* object = ((Scene2D*)root->currentScene)->getSprite2D(objectName, cur_tokens[ptr + 3].raw_data);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 4].raw_data);
				vec2 from = toVec2(cur_tokens[ptr + 5].raw_data);
				vec2 to = toVec2(cur_tokens[ptr + 6].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"scale", from, to, duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->scale = to;
				}
			}
			else if (propertyName == L"color_rect_color") {
				ColorRect* object = (ColorRect*)((Scene2D*)root->currentScene)->getObject(L"ColorRect", objectName);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 3].raw_data);
				vec4 from = toVec4(cur_tokens[ptr + 4].raw_data);
				vec4 to = toVec4(cur_tokens[ptr + 5].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"color", from, to, duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->color = to;
				}
			}
			else if (propertyName == L"sprite_modulate") {
				Sprite2D* object = ((Scene2D*)root->currentScene)->getSprite2D(objectName, cur_tokens[ptr + 3].raw_data);
				if (object == nullptr) { currentGlobalIndex++; continue; }
				float duration = toFloat(cur_tokens[ptr + 4].raw_data);
				vec4 from = toVec4(cur_tokens[ptr + 5].raw_data);
				vec4 to = toVec4(cur_tokens[ptr + 6].raw_data);
				if (is_skipping_to_last == false) {
					bool finished = false;
					((Scene2D*)root->currentScene)->currentTween->linear_interpolate(object, L"modulate", from, to, duration, (cur_tokens[1].type == TokenKind::Await) ? &finished : nullptr);
					if (cur_tokens[1].type == TokenKind::Await) while (finished == false);
				}
				else {
					object->modulate = to;
				}
			}
		}
		else if (cur_tokens[0].type == TokenKind::Audio) {
			if (isCallback == false) currentGlobalIndex += 1;
			std::wstring cmd = cur_tokens[1].raw_data;
			Audio* ad = ((Scene2D*)root->currentScene)->getAudio(cur_tokens[2].raw_data);
			if (cmd == L"play") {
				if (is_skipping_to_last == false) {
					ma_device_uninit(&ad->device);
					ma_decoder_uninit(&ad->decoder);
					ad->load(s2ws(ad->filePath));
					ad->play();
				}
			}
			else if (cmd == L"play_loop") {
				ma_device_uninit(&ad->device);
				ma_decoder_uninit(&ad->decoder);
				ad->load(s2ws(ad->filePath));
				ad->playLoop();
			}
			else if (cmd == L"play_fade") {
				if (is_skipping_to_last == false) {
					ma_device_uninit(&ad->device);
					ma_decoder_uninit(&ad->decoder);
					ad->load(s2ws(ad->filePath));
					ad->playFade(toFloat(cur_tokens[3].raw_data));
				}
			}
			else if (cmd == L"play_loop_fade") {
				ma_device_uninit(&ad->device);
				ma_decoder_uninit(&ad->decoder);
				ad->load(s2ws(ad->filePath));
				ad->playLoopFade(toFloat(cur_tokens[3].raw_data));
			}
			else if (cmd == L"stop") {
				ad->stop();
			}
			else if (cmd == L"stop_fade") {
				ad->stopFade(toFloat(cur_tokens[3].raw_data));
			}
			else if (cmd == L"set_volume") {
				ad->setVolume(toFloat(cur_tokens[3].raw_data));
			}
		}
		else if (cur_tokens[0].type == TokenKind::SaveStart) {
			currentGlobalIndex = 0;
			((Scene2D*)root->currentScene)->currentDialog->reset();
		}
		else if (cur_tokens[0].type == TokenKind::Save) {
			std::wstring filePath = checkPath(cur_tokens[1].raw_data);
			std::wstring INDEX = std::to_wstring(currentGlobalIndex);
			std::wstring CHOICE_QUEUE = L"";
			std::wstring INPUT_QUEUE = L"";
			std::queue<int> tempChoice = savedChoiceQueue;
			while (tempChoice.empty() == false) {
				CHOICE_QUEUE += std::to_wstring(tempChoice.front()) + L" ";
				tempChoice.pop();
			}
			std::queue<std::wstring> tempInput = savedInputQueue;
			while (tempInput.empty() == false) {
				INPUT_QUEUE += L"\"" + tempInput.front() + L"\"";
				tempInput.pop();
			}
			writeFile(ws2s(filePath), INDEX + L"\n" + CHOICE_QUEUE + L"\n" + INPUT_QUEUE);
		}
		else if (cur_tokens[0].type == TokenKind::Load) {
			std::wstring filePath = checkPath(cur_tokens[1].raw_data);
			if (fileExists(ws2s(filePath))) {
				currentGlobalIndex = 0;
				goalGlobalIndex = std::stoi(split(readFileW(filePath), L'\n')[0]);
				if (split(readFileW(filePath), L'\n').size() == 2) {
					std::vector<Token> TOKENS = tokenize(split(readFileW(filePath), L'\n')[1]);
					choiceQueue = std::queue<int>();
					savedChoiceQueue = std::queue<int>();
					for (auto& p : TOKENS) {
						savedChoiceQueue.push(toInt(p.raw_data));
						choiceQueue.push(toInt(p.raw_data));
					}
				}
				else if (split(readFileW(filePath), L'\n').size() == 3) {
					std::vector<Token> TOKENS = tokenize(split(readFileW(filePath), L'\n')[1]);
					choiceQueue = std::queue<int>();
					savedChoiceQueue = std::queue<int>();
					for (auto& p : TOKENS) {
						savedChoiceQueue.push(toInt(p.raw_data));
						choiceQueue.push(toInt(p.raw_data));
					}
					TOKENS = tokenize(split(readFileW(filePath), L'\n')[2]);
					inputQueue = std::queue<std::wstring>();
					savedInputQueue = std::queue<std::wstring>();
					for (auto& p : TOKENS) {
						savedInputQueue.push(p.raw_data);
						inputQueue.push(p.raw_data);
					}
				}
			}
			else {
				alert(L"there is no save file at " + filePath);
			}
		}
		else if (cur_tokens[0].type == TokenKind::Persistent) {
			std::vector<Token> modified_tokens = tokenize(src_lines[index], true);
			if (modified_tokens.size() > 3 == false) alert(L"persistent command needs 4 arguments at least");
			std::wstring filePath = checkPath(modified_tokens[1].raw_data);
			std::wstring mode = modified_tokens[2].raw_data;
			std::wstring varName = modified_tokens[3].raw_data;
			if (mode == L"load") {
				if (eraseLineBreak(eraseWhiteSpace(readFileW(filePath))) != L"") {
					Sgt::instance().varMap[varName] = load_persistent(filePath, varName);
				}
			}
			else if (mode == L"save") {
				std::vector<Token> tempTokens = modified_tokens;
				tempTokens.erase(tempTokens.begin());
				tempTokens.erase(tempTokens.begin());
				tempTokens.erase(tempTokens.begin());
				std::wstring realContent = L"";
				for (auto& p : tempTokens) { realContent += p.data() + L" "; }
				save_persistent(filePath, varName, eval(realContent).data());
			}
		}
		else {
			currentGlobalIndex += 1;
			std::vector<Token> modified_tokens = tokenize(src_lines[index], true);
			if (modified_tokens.size() < 3) {
				continue;
			}
			if (modified_tokens[1].type != TokenKind::Assign) alert(L"please write assign opeator '=' in code " + src_lines[index]);
			std::wstring targetVarName = modified_tokens[0].raw_data;
			std::wstring targetAssignedValue = L"";
			for (int i = 2; i < modified_tokens.size(); i++) {
				targetAssignedValue += modified_tokens[i].data() + L" ";
			}
			if (Sgt::instance().varExists(targetVarName) == false) alert(L"there is no variable called " + targetVarName);
			Sgt::instance().varMap[targetVarName] = eval(targetAssignedValue, false).raw_data;
		}
	}
}
