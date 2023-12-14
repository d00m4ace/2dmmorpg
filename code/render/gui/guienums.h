//--------------------------------------------------------------------------------------
typedef enum GUI_STYLE_ENUM
{
	GUI_STYLE_NONE,

	GUI_STYLE_BUTTON,
	GUI_STYLE_BUTTON_TILE,
	GUI_STYLE_BUTTON_LONG,
	GUI_STYLE_BUTTON_KEYBOARD,

	GUI_STYLE_PROGRESSBAR,

	GUI_STYLE_SPINNER,
	GUI_STYLE_SPINNER_BUTTON,

	GUI_STYLE_EMPTY,
	GUI_STYLE_EMPTY_TEXT_THIN,
	GUI_STYLE_EMPTY_TEXT_SMALL,

	GUI_STYLE_TEXT,
	GUI_STYLE_TEXT_SMALL,

	GUI_STYLE_TEXT_BORDER,
	GUI_STYLE_TEXT_SMALL_BORDER,

	GUI_STYLE_TEXT_INPUT,
	GUI_STYLE_TEXT_INPUT_LONG,

	GUI_STYLE_PLANE,

	GUI_STYLE_ENUM_COUNT,
} GUI_STYLE_ENUM;
//--------------------------------------------------------------------------------------
typedef enum GUI_BACKGROUND_ENUM
{
	GUI_BACKGROUND_NONE,
	GUI_BACKGROUND_BOX,
	GUI_BACKGROUND_TILE,
	GUI_BACKGROUND_BUBBLE,

	GUI_BACKGROUND_ENUM_COUNT,
} GUI_BACKGROUND_ENUM;
//--------------------------------------------------------------------------------------
typedef enum GUI_ELEM_STATE_ENUM
{
	GUI_ELEM_STATE_NORMAL,
	GUI_ELEM_STATE_PRESSED,
	GUI_ELEM_STATE_RELEASED,
	GUI_ELEM_STATE_FOCUSED,
	GUI_ELEM_STATE_SELECTED,
	GUI_ELEM_STATE_DISABLED,

	GUI_ELEM_STATE_ENUM_COUNT,
} GUI_ELEM_STATE_ENUM;
//--------------------------------------------------------------------------------------
//HORIZONTAL&VERTICAL ALIGN!
typedef enum
{
	VALIGN_NONE,
	VALIGN_TOP,
	VALIGN_MIDDLE,
	VALIGN_BOTTOM,
} VERTICAL_ALIGN_ENUM;
typedef enum
{
	HALIGN_NONE,
	HALIGN_LEFT,
	HALIGN_CENTER,
	HALIGN_RIGHT,
} HORIZONTAL_ALIGN_ENUM;
//--------------------------------------------------------------------------------------
typedef enum
{
	GUI_ELEM_NONE,

	GUI_ELEM_GROUP,
	GUI_ELEM_LIST,

	GUI_ELEM_BUTTON,

	GUI_ELEM_PROGRESSBAR,
	GUI_ELEM_SPINNER,

	GUI_ELEM_COMPOSITE,

	GUI_ELEM_TABS,
	GUI_ELEM_TOGGLE,

	GUI_ELEM_TEXT_INPUT,

	GUI_ELEM_SPACE,
	GUI_ELEM_SPACE_NEW_LINE,

	GUI_ELEM_ENUM_COUNT,
} GUI_ELEM_ENUM;
//--------------------------------------------------------------------------------------
typedef enum
{
	GUI_ALIGN_NONE,

	GUI_ALIGN_VERTICAL,
	GUI_ALIGN_HORIZONTAL,

	GUI_ALIGN_ENUM_COUNT,
} GUI_ALIGN_ENUM;
//--------------------------------------------------------------------------------------