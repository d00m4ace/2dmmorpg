GUI_ELEM* gui_progressbar(const char* txt, int32 val, int32 val_min, int32 val_max)
{
	GUI_ELEM* elm = gui_element();
	GUI_PROGRESSBAR* pb = CALLOC(1, sizeof(GUI_PROGRESSBAR));

	pb->val_min = val_min;
	pb->val_max = val_max;

	elm->data = pb;

	elm->elem_type = GUI_ELEM_PROGRESSBAR;
	elm->elem_id = -1;

	elm->style = GUI_STYLE_PROGRESSBAR;
	elm->size = gui_style_size_min(elm->style);

	elm->show = gui_progressbar_show;
	elm->on_input = gui_progressbar_on_input;

	elm->flags = 0;

	elm->flags |= GUI_FLG_MOUSELOCK;

	//elm->flags |= GUI_FLG_MOUSELOCK | GUI_FLG_LOCKED | GUI_FLG_DISABLED;

	elm->content_halign = HALIGN_CENTER;
	elm->content_valign = VALIGN_MIDDLE;

	gui_progressbar_set_txt(elm, txt);
	gui_progressbar_set_val(elm, val);

	return elm;
}

GUI_ELEM* gui_progressbar_fixed_size(const char* txt, int32 val, int32 val_min, int32 val_max, p32 fixed_size)
{
	GUI_ELEM* elm = gui_progressbar(txt, val, val_min, val_max);
	elm->size = fixed_size;
	return elm;
}

void gui_progressbar_show(GUI_ELEM* elm, p32 offs)
{
	GUI_PROGRESSBAR* pb = elm->data;
	GUI_STYLE* style = gui_style_get(elm->style);
	GUI_ELEM_STATE_ENUM gui_state = elm->gui_state;
	p32 size = elm->size;

	rd2_scr_stack_push(elm->size.x, elm->size.y, gui_color(PAL_TRANSPARENT));

	{
		///!HACK!///
		GUI_STYLE save_style = *style;

		style->color_background[gui_state] = style->color_border[gui_state];
		gui_element_show(elm, (p32) { -elm->pos.x, -elm->pos.y });

		if(pb->val > pb->val_min)
		{
			elm->size.x = (elm->size.x * (pb->val - pb->val_min)) / pb->val_max;

			if(elm->size.x < 3)
				elm->size.x = 3;

			if(style->background == GUI_BACKGROUND_BUBBLE && elm->size.x < 5)
				elm->size.x = 5;

			style->color_background[gui_state] = save_style.color_background[gui_state];
			style->color_border[gui_state] = PAL_TRANSPARENT;
			gui_element_show(elm, (p32) { -elm->pos.x, -elm->pos.y });
		}

		gui_style_set(elm->style, save_style);
		elm->size = size;
		///!HACK!///
	}

	char* txt = txt_format(pb->txt, pb->val);
	int txt_len = strlen(txt);

	if(txt_len)
	{
		p32 cs = gui_helper_txt_calc_content_size(txt, style, gui_state);
		cs = p32_min(cs, elm->size);

		int32 halgn = gui_style_calc_halign(elm->content_halign, elm->size.x, cs.x);
		int32 valgn = gui_style_calc_valign(elm->content_valign, elm->size.y, cs.y);

		RD2FONT* fnt = game_font_get(style->font_id[gui_state]);
		p32 offs = rd2_font_print(fnt, txt, halgn + 2, valgn, 1, 0, gui_color(style->color[gui_state]), FNT_FLG_DRAW);
	}

	rd2_scr_stack_pop_show(p32_add(elm->pos, offs));
}

void gui_progressbar_on_input(GUI_ELEM* elm)
{
	gui_element_on_input(elm);

	if(elm == gui_input_elm_pressed())
	{
		if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
		{
			if(gui_element_to_p32(elm, gui_input_pos()))
			{
				GUI_PROGRESSBAR* pb = elm->data;

				int32 val = pb->val_min + (((pb->val_max - pb->val_min) * (gui_input_pos().x - elm->pos.x)) / (elm->size.x - 2));
				gui_progressbar_set_val(elm, val);
			}
		}
	}
}

void gui_progressbar_set_txt(GUI_ELEM* elm, const char* txt)
{
	GUI_PROGRESSBAR* pb = elm->data;

	ASSERT((strlen(txt) + 1) < GUI_PROGRESSBAR_TXT_MAX_SIZE);

	strcpy(pb->txt, txt);

	elm->flags |= GUI_FLG_DIRT;
}

void gui_progressbar_set_val(GUI_ELEM* elm, int32 val)
{
	GUI_PROGRESSBAR* pb = elm->data;

	pb->val = val;

	pb->val = MAX(pb->val, pb->val_min);
	pb->val = MIN(pb->val, pb->val_max);

	elm->flags |= GUI_FLG_DIRT;
}

void gui_progressbar_add_val(GUI_ELEM* elm, int32 val)
{
	GUI_PROGRESSBAR* pb = elm->data;

	pb->val += val;

	pb->val = MAX(pb->val, pb->val_min);
	pb->val = MIN(pb->val, pb->val_max);

	elm->flags |= GUI_FLG_DIRT;
}

int32 gui_progressbar_get_val(GUI_ELEM* elm)
{
	GUI_PROGRESSBAR* pb = elm->data;
	return pb->val;
}

int32 gui_progressbar_get_val_min(GUI_ELEM* elm)
{
	GUI_PROGRESSBAR* pb = elm->data;
	return pb->val_min;
}

int32 gui_progressbar_get_val_max(GUI_ELEM* elm)
{
	GUI_PROGRESSBAR* pb = elm->data;
	return pb->val_max;
}
