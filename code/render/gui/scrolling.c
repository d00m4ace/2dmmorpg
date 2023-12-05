GUI_SCROLLING gui_scrolling(int32 drag_delta_scale, int32 mouse_wheel_scale)
{
	GUI_SCROLLING scr = { 0 };

	scr.drag_delta_scale = drag_delta_scale;
	scr.mouse_wheel_scale = mouse_wheel_scale;

	scr.flags = GUI_SCROLLING_FLG_V_ENABLE | GUI_SCROLLING_FLG_H_ENABLE | GUI_SCROLLING_FLG_WHEEL_ENABLE;

	//scr.flags |= GUI_SCROLLING_FLG_NO_LIMITS;

	return scr;
}

void gui_scrolling_calc_pos(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size)
{
	scr->scroll_to = d2v_add(scr->scroll_to, d2v_mul(scr->scroll_v, 0.1f));
	scr->scroll_v = d2v_mul(scr->scroll_v, 0.9f);

	scr->scroll_pos = d2v_add(scr->scroll_pos, d2v_mul(d2v_sub(scr->scroll_to, scr->scroll_pos), 0.1f));

	if(d2v_len2(d2v_sub(scr->scroll_to, scr->scroll_pos)) < 2)
	{
		scr->scroll_pos = scr->scroll_to;
		scr->scroll_v = (d2v){ 0 };
	}

	p32 size_diff = p32_sub(max_size, size);

	if(!(scr->flags & GUI_SCROLLING_FLG_NO_LIMITS))
	{
		if(scr->scroll_to.x < 0)
		{
			if(-scr->scroll_to.x > size_diff.x)
			{
				scr->scroll_to.x = -size_diff.x;
				scr->scroll_v.x = 0;
			}
		}
		else
		{
			if(scr->scroll_to.x > 0)
			{
				scr->scroll_to.x = 0;
				scr->scroll_v.x = 0;
			}
		}

		if(scr->scroll_to.y < 0)
		{
			if(-scr->scroll_to.y > size_diff.y)
			{
				scr->scroll_to.y = -size_diff.y;
				scr->scroll_v.y = 0;
			}
		}
		else
		{
			if(scr->scroll_to.y > 0)
			{
				scr->scroll_to.y = 0;
				scr->scroll_v.y = 0;
			}
		}
	}

	if(max_size.x < size.x || ABS(scr->scroll_to.x) < 2)
	{
		scr->scroll_pos.x = 0;
		scr->scroll_to.x = 0;
		scr->scroll_v.x = 0;
	}

	if(max_size.y < size.y || ABS(scr->scroll_to.y) < 2)
	{
		scr->scroll_pos.y = 0;
		scr->scroll_to.y = 0;
		scr->scroll_v.y = 0;
	}

	if(d2v_len2(d2v_sub(scr->scroll_to, scr->scroll_pos)) > 1)
		scr->show_scrollbar_frames = 255;
}

void gui_scrolling_show(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size)
{
	if(scr->show_scrollbar_frames > 0)
		scr->show_scrollbar_frames -= 3;

	if(scr->show_scrollbar_frames > 0)
	{
		rd2c scrollbar_clr = ARGB32_SET_ALPHA(gui_color(PAL_DARKGRAY), scr->show_scrollbar_frames);
		rd2c scroll_clr = ARGB32_SET_ALPHA(gui_color(PAL_GRAY), scr->show_scrollbar_frames);

		if(scr->flags & GUI_SCROLLING_FLG_V_ENABLE)
		{
			if(size.y < max_size.y)
			{
				int32 slider_h = (size.y * size.y) / max_size.y;
				int32 slider_y = (-scr->scroll_pos.y * size.y) / max_size.y;

				rd2_rect_fill(pos.x + size.x - 2, pos.y + 0, 2, size.y, scroll_clr);
				rd2_rect_fill(pos.x + size.x - 2, pos.y + slider_y, 2, slider_h, scrollbar_clr);
			}
		}

		if(scr->flags & GUI_SCROLLING_FLG_H_ENABLE)
		{
			if(size.x < max_size.x)
			{
				int32 slider_w = (size.x * size.x) / max_size.x;
				int32 slider_x = (-scr->scroll_pos.x * size.x) / max_size.x;

				rd2_rect_fill(pos.x + 0, pos.y + size.y - 2, size.x, 2, scroll_clr);
				rd2_rect_fill(pos.x + slider_x, pos.y + size.y - 2, slider_w, 2, scrollbar_clr);
			}
		}
	}
}

void gui_scrolling_on_input(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size)
{
	if(gui_input_is_pressed() && aabb32_to_p32(aabb32_from_ps(pos, size), gui_input_pos_start()))
	{
		p32 drag_delta = gui_input_drag_delta();

		if(scr->flags & GUI_SCROLLING_FLG_V_ENABLE)
		{
			scr->scroll_to.y += (int)drag_delta.y;

			if(gui_input_is_drag())
				scr->scroll_v.y += (int)drag_delta.y * scr->drag_delta_scale;
		}

		if(scr->flags & GUI_SCROLLING_FLG_H_ENABLE)
		{
			scr->scroll_to.x += (int)drag_delta.x;

			if(gui_input_is_drag())
				scr->scroll_v.x += (int)drag_delta.x * scr->drag_delta_scale;
		}
	}

	if(scr->flags & GUI_SCROLLING_FLG_WHEEL_ENABLE)
	{
		if(hal_mouse_wheel() != 0 && (scr->flags & GUI_SCROLLING_FLG_V_ENABLE))
		{
			if(aabb32_to_p32(aabb32_from_ps(pos, size), gui_input_pos()))
				scr->scroll_to.y += hal_mouse_wheel() * scr->mouse_wheel_scale;
		}
	}
}
