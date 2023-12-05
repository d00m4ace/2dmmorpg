GUI_ELEM* gui_element(void)
{
	GUI_ELEM* elm = CALLOC(1, sizeof(GUI_ELEM));

	elm->data = NULL;

	elm->elem_id = GUI_ELEM_NONE;
	elm->elem_type = GUI_ELEM_NONE;

	elm->size = (p32){ 0,0 };

	elm->style = GUI_STYLE_EMPTY;

	elm->calc_size = gui_element_calc_size;
	elm->calc_pos = gui_element_calc_pos;
	elm->show = gui_element_show;
	elm->on_input = gui_element_on_input;
	elm->on_update = gui_element_on_update;
	elm->on_free = gui_element_on_free;

	return elm;
}

void gui_element_calc_size(GUI_ELEM* elm)
{
	elm->flags &= ~GUI_FLG_DIRT;
}
void gui_element_calc_pos(GUI_ELEM* elm, p32 offs)
{
	elm->pos = p32_add(offs, elm->origin);
}

void gui_element_show(GUI_ELEM* elm, p32 offs)
{
	GUI_STYLE* style = gui_style_get(elm->style);

	rd2c color_border = gui_color(style->color_border[elm->gui_state]);
	rd2c color_background = gui_color(style->color_background[elm->gui_state]);

	p32 pos = p32_add(elm->pos, offs);

	switch(style->background)
	{
		case GUI_BACKGROUND_BOX:
		rd2_box(pos.x, pos.y, elm->size.x, elm->size.y, color_background, color_border);
		break;
		case GUI_BACKGROUND_TILE:
		rd2_tile(pos.x, pos.y, elm->size.x, elm->size.y, color_background, color_border);
		break;
		case GUI_BACKGROUND_BUBBLE:
		rd2_bubble(pos.x, pos.y, elm->size.x, elm->size.y, color_background, color_border);
		break;
	}
}

void gui_element_on_input(GUI_ELEM* elm)
{
	aabb32 elm_aabb = aabb32_from_ps(elm->pos, elm->size);

	if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
	{
		if(gui_input_is_pressed())
		{
			if(gui_input_elm_pressed() == NULL && gui_input_pressed_frames() == 1)
			{
				if(aabb32_to_p32(elm_aabb, gui_input_pos_start()))
					gui_input_set_elm_pressed(elm);
			}
		}

		if(elm == gui_input_elm_pressed())
		{
			if(aabb32_to_p32(elm_aabb, gui_input_pos()))
			{
				if((elm->flags & GUI_FLG_FOCUSABLE))
					gui_input_set_elm_focused(elm);

				if(gui_input_is_released())
				{
					gui_input_set_elm_released(elm);

					if((elm->flags & GUI_FLG_SELECTABLE))
					{
						if(elm->flags & GUI_FLG_SELECTED)
							elm->flags &= ~GUI_FLG_SELECTED;
						else
							elm->flags |= GUI_FLG_SELECTED;
					}
				}
			}

			gui_input_set_processed(true);
		}
	}
}

void gui_element_on_update(GUI_ELEM* elm)
{
	if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
	{
		if(elm->flags & GUI_FLG_SELECTED)
		{
			elm->gui_state = GUI_ELEM_STATE_SELECTED;
			elm->gui_state_fade_frames = GUI_STATE_FADE_FRAMES;
		}

		if(elm == gui_input_elm_focused())
		{
			elm->gui_state = GUI_ELEM_STATE_FOCUSED;
			elm->gui_state_fade_frames = GUI_STATE_FADE_FRAMES;
		}

		if(gui_element_to_p32(elm, gui_input_pos()))
		{
			if(elm == gui_input_elm_pressed())
			{
				elm->gui_state = GUI_ELEM_STATE_PRESSED;
				elm->gui_state_fade_frames = GUI_STATE_FADE_FRAMES;
			}

			if(elm == gui_input_elm_released())
			{
				elm->gui_state = GUI_ELEM_STATE_RELEASED;
				elm->gui_state_fade_frames = GUI_STATE_FADE_FRAMES;
			}
		}

		if(elm->gui_state_fade_frames > 0)
			elm->gui_state_fade_frames--;

		if(elm->gui_state_fade_frames == 0)
			elm->gui_state = GUI_ELEM_STATE_NORMAL;
	}
	else
	{
		elm->gui_state = GUI_ELEM_STATE_NORMAL;

		if(elm->flags & GUI_FLG_DISABLED)
			elm->gui_state = GUI_ELEM_STATE_DISABLED;
	}
}

void gui_element_on_free(GUI_ELEM* elm)
{
	CFREE(elm->data);
	elm->data = NULL;
	CFREE(elm->user_data);
	elm->user_data = NULL;
	CFREE(elm);
}
//--------------------------------------------------------------------------------------
bool gui_element_to_p32(GUI_ELEM* elm, p32 p)
{
	return aabb32_to_p32(aabb32_from_ps(elm->pos, elm->size), p);
}

aabb32 gui_element_get_aabb32(GUI_ELEM* elm)
{
	return aabb32_from_ps(elm->pos, elm->size);
}
//--------------------------------------------------------------------------------------
void gui_element_set_style_and_size_min(GUI_ELEM* elm, GUI_STYLE_ENUM style)
{
	elm->style = style;
	elm->size = gui_style_size_min(style);
}

void gui_element_set_style_and_size_max(GUI_ELEM* elm, GUI_STYLE_ENUM style)
{
	elm->style = style;
	elm->size = gui_style_size_max(style);
}
//--------------------------------------------------------------------------------------
