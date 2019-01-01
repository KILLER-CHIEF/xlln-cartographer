#pragma once
#include <map>
#include <vector>

typedef struct {
	int lang_base;
	int lang_variant;
	bool other;
	char* lang_name;
	char* font_table_filename;
	std::map<int, std::map<int, char*>>* label_map;
} custom_language;

BOOL InitCustomLanguage();
BOOL UninitCustomLanguage();

void CLSetGameLanguage();

char* add_cartographer_label(int label_menu_id, int label_id, char* label, bool is_dynamic);
char* add_cartographer_label(int label_menu_id, int label_id, const char* label, bool is_dynamic);
char* add_cartographer_label(int label_menu_id, int label_id, char* label);
char* add_cartographer_label(int label_menu_id, int label_id, const char* label);
char* add_cartographer_label(int label_menu_id, int label_id, int labelBufferLen, bool is_dynamic);
char* add_cartographer_label(int label_menu_id, int label_id, int labelBufferLen);
char* get_cartographer_label(int label_menu_id, int label_id, __int8 is_dynamic);
char* get_cartographer_label(int label_menu_id, int label_id);
char* get_custom_label(custom_language* language, int label_menu_id, int label_id, char* label);
char* H2CustomLanguageGetLabel(int label_menu_id, int label_id);
void setCustomLanguage(int, int);
void setCustomLanguage(int);
bool CLReloadCustomLanguages();
void CLSaveCustomLanguages();
void combineCartographerLabels(int menuId, int lbl1, int lbl2, int lblCmb);

extern std::vector<custom_language*> custom_languages;
extern custom_language* current_language;
