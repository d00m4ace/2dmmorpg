typedef enum GUI_TEXT_STYLE_ENUM
{
	GUI_TEXT_STYLE_NONE,

	GUI_TEXT_STYLE_NORMAL,
	GUI_TEXT_STYLE_SMALL,
	GUI_TEXT_STYLE_NORMAL_BORDER,
	GUI_TEXT_STYLE_SMALL_BORDER,

	GUI_TEXT_STYLE_ENUM_COUNT,
} GUI_TEXT_STYLE_ENUM;

GUI_ELEM* gui_text_(const char* txt, GUI_TEXT_STYLE_ENUM txt_style);

//max_width and max_height are optional, if you want to limit the size of the text
//if you don't want to limit the size, set max_width and max_height to -1
GUI_ELEM* gui_text_word_wrap_(const char* txt, GUI_TEXT_STYLE_ENUM txt_style, int max_width, int max_height);

#define gui_text(txt) gui_text_((txt),GUI_TEXT_STYLE_NORMAL)
#define gui_text_word_wrap(txt) gui_text_word_wrap_((txt),GUI_TEXT_STYLE_NORMAL,-1,-1)

#define gui_text_s(txt,style) gui_text_((txt),(style))
#define gui_text_word_wrap_s(txt,style) gui_text_word_wrap_((txt),(style),-1,-1)
