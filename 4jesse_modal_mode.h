#ifndef FOUR_JESSE_MODAL_MODE_H
#define FOUR_JESSE_MODAL_MODE_H

/*
*  A pretty small an straightforward modal mode in 4coder
*  
*  USAGE: add "keys_edit" to the command_map and call J_SetupMapping(); at the end of your custom_layer_init function
*
*  In your custom_layer_init, setup should look like the following:
*
*  mapping_init(tctx, &framework_mapping);
    *  String_ID global_map_id = vars_save_string_lit("keys_global");
    *  String_ID file_map_id = vars_save_string_lit("keys_file");
    *  String_ID code_map_id = vars_save_string_lit("keys_code");
*
    *  String_ID edit_map_id = vars_save_string_lit("keys_edit"); <------Line to add  
    *  J_SetupMapping(&framework_mapping, global_map_id, file_map_id, code_map_id, edit_map_id); <------Line To Add
*
*
*  In your default_begin_buffer hook, add this line:

*  Command_Map_ID map_id = (treat_as_code)?(code_map_id):(file_map_id); <-----Search for this and add the following:
*
    *    if(global_edit_mode) {
        *        map_id = vars_save_string_lit("keys_edit");
    *    }
*  
*
*
*/

static b32 global_edit_mode = false;

CUSTOM_ID(colors, defcolor_indexnote_type)
CUSTOM_ID(colors, defcolor_indexnote_function)
CUSTOM_ID(colors, defcolor_indexnote_macro)
CUSTOM_ID(colors, defcolor_indexnote_4codercommand)

internal void
J_UpdateThemeForMode(Application_Links* app) {
    Color_Table *table = &active_color_table;
    Arena *arena = &global_theme_arena;
    linalloc_clear(arena);
    *table = make_color_table(app, arena);
    
    u32 EditModeRed = 0xFFEC0111;
    u32 TextModeBlue = 0xFF26AAC6;
    u32 RecordingMacroColor = 0xFF8a1aaa;
    
    u32 BarColor = 0xFFCACACA;
    
    
    if(global_keyboard_macro_is_recording) {
        
        table->arrays[defcolor_cursor]                = make_colors(arena, RecordingMacroColor);
        table->arrays[defcolor_mark]                  = make_colors(arena, RecordingMacroColor);
        table->arrays[defcolor_bar]                   = make_colors(arena, 0xFFCACACA); 
        
        table->arrays[defcolor_margin_hover]          = make_colors(arena, RecordingMacroColor); 
        table->arrays[defcolor_margin_active]         = make_colors(arena, RecordingMacroColor); 
        
    }else if(global_edit_mode) {
        table->arrays[defcolor_cursor]                = make_colors(arena, EditModeRed);
        table->arrays[defcolor_mark]                  = make_colors(arena, EditModeRed);
        table->arrays[defcolor_bar]                   = make_colors(arena, 0xFFCACACA); 
        
        table->arrays[defcolor_margin_hover]          = make_colors(arena, EditModeRed); 
        table->arrays[defcolor_margin_active]         = make_colors(arena, EditModeRed); 
    } else {
        table->arrays[defcolor_cursor]                = make_colors(arena, TextModeBlue);
        table->arrays[defcolor_mark]                  = make_colors(arena, TextModeBlue);
        table->arrays[defcolor_bar]                   = make_colors(arena, 0xFFCACACA);
        
        table->arrays[defcolor_margin_hover]          = make_colors(arena, TextModeBlue); 
        table->arrays[defcolor_margin_active]         = make_colors(arena, TextModeBlue); 
        
    }
    
    
    table->arrays[defcolor_at_cursor]             = make_colors(arena, 0xFF000000);
    table->arrays[defcolor_at_highlight]          = make_colors(arena, 0xFF000000);
    table->arrays[defcolor_margin]                = make_colors(arena, 0xFF181818);
    table->arrays[defcolor_base]                  = make_colors(arena, 0xFF000000);
    table->arrays[defcolor_pop1]                  = make_colors(arena, 0xFF03CF0C);
    table->arrays[defcolor_pop2]                  = make_colors(arena, 0xFFFF0000);
    table->arrays[defcolor_back]                  = make_colors(arena, 0xFF272822);
    table->arrays[defcolor_list_item]             = make_colors(arena, 0xFF262626);
    table->arrays[defcolor_list_item_active]      = make_colors(arena, 0xFFCACACA); 
    table->arrays[defcolor_list_item_hover]       = make_colors(arena, 0xFF000000); 
    table->arrays[defcolor_highlight_cursor_line] = make_colors(arena, 0xFF22221E);
    table->arrays[defcolor_highlight]             = make_colors(arena, 0xFF703419);
    table->arrays[defcolor_text_default]          = make_colors(arena, 0xFFA08563);
    table->arrays[defcolor_comment]               = make_colors(arena, 0xFF7D7D7D);
    table->arrays[defcolor_comment_pop]           = make_colors(arena, 0xff2ab34f, 0xFFdb2828);
    table->arrays[defcolor_keyword]               = make_colors(arena, 0xFFCD950C);
    table->arrays[defcolor_str_constant]          = make_colors(arena, 0xFF6B8E23);
    table->arrays[defcolor_char_constant]         = table->arrays[defcolor_str_constant];
    table->arrays[defcolor_int_constant]          = table->arrays[defcolor_str_constant];
    table->arrays[defcolor_float_constant]        = table->arrays[defcolor_str_constant];
    table->arrays[defcolor_bool_constant]         = table->arrays[defcolor_str_constant];
    table->arrays[defcolor_preproc]               = make_colors(arena, 0xFF26AAC6);
    table->arrays[defcolor_include]               = make_colors(arena, 0xFF6B8E23);
    table->arrays[defcolor_special_character]     = make_colors(arena, 0xFFFF0000);
    table->arrays[defcolor_ghost_character]       = make_colors(arena, 0xFF5B4D3C);
    table->arrays[defcolor_highlight_junk]        = make_colors(arena, 0xFF3A0000);
    table->arrays[defcolor_highlight_white]       = make_colors(arena, 0xFF003A3A);
    table->arrays[defcolor_paste]                 = make_colors(arena, 0xFFFFBB00);
    table->arrays[defcolor_undo]                  = make_colors(arena, 0xFF80005D);
    table->arrays[defcolor_back_cycle]            = make_colors(arena, 0xFF272822, 0xFF29302D, 0xFF313431, 0xFF46413A);
    table->arrays[defcolor_text_cycle]            = make_colors(arena, 0xFFA81B23, 0xFF9BC781, 0xFF0030B0, 0xFFA0A000);
    table->arrays[defcolor_line_numbers_back]     = make_colors(arena, 0xFF101010);
    table->arrays[defcolor_line_numbers_text]     = make_colors(arena, 0xFF404040);
    
    
    table->arrays[defcolor_indexnote_type]       = table->arrays[defcolor_keyword];
    table->arrays[defcolor_indexnote_function]   = make_colors(arena, 0xFF9BC781);
    table->arrays[defcolor_indexnote_macro]      = make_colors(arena, 0xffa46391);
    table->arrays[defcolor_indexnote_4codercommand] = make_colors(arena, 0xffffffff);
}



CUSTOM_COMMAND_SIG(J_KeyboardMacroStartRecording)
CUSTOM_DOC("J_Start macro recording"){
    if (global_keyboard_macro_is_recording ||
        get_current_input_is_virtual(app)){
        return;
    }
    
    Buffer_ID buffer = get_keyboard_log_buffer(app);
    global_keyboard_macro_is_recording = true;
    global_keyboard_macro_range.first = buffer_get_size(app, buffer);
    
    J_UpdateThemeForMode(app);
}


CUSTOM_COMMAND_SIG(J_KeyboardMacroFinishRecording)
CUSTOM_DOC("Stop macro recording, do nothing if macro recording is not already started")
{
    if (!global_keyboard_macro_is_recording ||
        get_current_input_is_virtual(app)){
        return;
    }
    
    Buffer_ID buffer = get_keyboard_log_buffer(app);
    global_keyboard_macro_is_recording = false;
    i64 end = buffer_get_size(app, buffer);
    Buffer_Cursor cursor = buffer_compute_cursor(app, buffer, seek_pos(end));
    Buffer_Cursor back_cursor = buffer_compute_cursor(app, buffer, seek_line_col(cursor.line - 1, 1));
    global_keyboard_macro_range.one_past_last = back_cursor.pos;
    
    J_UpdateThemeForMode(app);
}

CUSTOM_COMMAND_SIG(J_SwitchToEditMode) {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Managed_Scope scope = buffer_get_managed_scope(app, buffer);
    Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
    *map_id_ptr = vars_save_string_lit("keys_edit");
    global_edit_mode = true;
    J_UpdateThemeForMode(app);
};

CUSTOM_COMMAND_SIG(J_SwitchToCodeMode) {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Managed_Scope scope = buffer_get_managed_scope(app, buffer);
    Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
    *map_id_ptr = vars_save_string_lit("keys_code");
    global_edit_mode = false;
    J_UpdateThemeForMode(app);
};

CUSTOM_COMMAND_SIG(J_SwitchModalModes) {
    
    if(global_edit_mode) {
        J_SwitchToCodeMode(app);
    } else {
        J_SwitchToEditMode(app);
    };
}

function void
J_SetupMapping(Mapping *mapping, i64 global_id, i64 file_id, i64 code_id, i64 edit_id){
    MappingScope();
    SelectMapping(mapping);
    
    SelectMap(global_id);
    {
        Bind(J_SwitchModalModes,            KeyCode_Space, KeyCode_Control);
        Bind(exit_4coder,                   KeyCode_F4, KeyCode_Alt); 
        Bind(J_KeyboardMacroStartRecording,   KeyCode_U, KeyCode_Control);
        Bind(J_KeyboardMacroFinishRecording, KeyCode_U, KeyCode_Control, KeyCode_Shift);
        Bind(keyboard_macro_replay,           KeyCode_U, KeyCode_Alt);
        Bind(change_active_panel,           KeyCode_Comma, KeyCode_Control);
        Bind(change_active_panel_backwards, KeyCode_Comma, KeyCode_Control, KeyCode_Shift);
        
        Bind(project_go_to_root_directory,  KeyCode_H, KeyCode_Control);
        Bind(change_to_build_panel,         KeyCode_Period, KeyCode_Alt);
        Bind(toggle_filebar,                KeyCode_B, KeyCode_Alt);
        Bind(execute_any_cli,               KeyCode_Z, KeyCode_Alt);
        Bind(execute_previous_cli,          KeyCode_Z, KeyCode_Alt, KeyCode_Shift);
        Bind(project_command_lister,        KeyCode_X, KeyCode_Alt, KeyCode_Shift);
        Bind(quick_swap_buffer,             KeyCode_BackwardSlash, KeyCode_Alt);
        Bind(jump_to_last_point,            KeyCode_P, KeyCode_Control);
        
        //Bind(project_fkey_command, KeyCode_F1);
        //Bind(project_fkey_command, KeyCode_F2);
        Bind(project_fkey_command, KeyCode_F3);
        Bind(project_fkey_command, KeyCode_F4);
        Bind(project_fkey_command, KeyCode_F5);
        Bind(project_fkey_command, KeyCode_F6);
        Bind(project_fkey_command, KeyCode_F7);
        Bind(project_fkey_command, KeyCode_F8);
        Bind(project_fkey_command, KeyCode_F9);
        Bind(project_fkey_command, KeyCode_F10);
        Bind(project_fkey_command, KeyCode_F11);
        Bind(project_fkey_command, KeyCode_F12);
        Bind(project_fkey_command, KeyCode_F13);
        Bind(project_fkey_command, KeyCode_F14);
        Bind(project_fkey_command, KeyCode_F15);
        Bind(project_fkey_command, KeyCode_F16);
        
        Bind(word_complete,              KeyCode_Tab);
        
        Bind(seek_end_of_line,       KeyCode_End);
        Bind(seek_beginning_of_line, KeyCode_Home);
        Bind(page_up,                KeyCode_PageUp);
        Bind(page_down,              KeyCode_PageDown);
        Bind(delete_char,            KeyCode_Delete);
        Bind(backspace_char,         KeyCode_Backspace);
        Bind(goto_beginning_of_file, KeyCode_PageUp, KeyCode_Control);
        Bind(goto_end_of_file,       KeyCode_PageDown, KeyCode_Control);
        
        Bind(move_up, KeyCode_Up);
        Bind(move_down, KeyCode_Down);
        Bind(move_left, KeyCode_Left);
        Bind(move_right, KeyCode_Right);
        
        Bind(move_up_to_blank_line_end, KeyCode_Up, KeyCode_Shift);
        Bind(page_up,                   KeyCode_Up, KeyCode_Control);
        //uBind(move_line_up,              KeyCode_Up, KeyCode_Alt);
        
        Bind(move_down_to_blank_line_end, KeyCode_Down, KeyCode_Shift);
        Bind(page_down,                   KeyCode_Down, KeyCode_Control);
        //Bind(move_line_down,              KeyCode_Down, KeyCode_Alt);
        
        
        Bind(move_left_alpha_numeric_or_camel_boundary,   KeyCode_Left, KeyCode_Shift);
        Bind(seek_beginning_of_line,                      KeyCode_Left, KeyCode_Alt);
        Bind(move_left_whitespace_boundary, KeyCode_Left, KeyCode_Control);
        
        Bind(move_right_alpha_numeric_or_camel_boundary,  KeyCode_Right, KeyCode_Shift);
        Bind(seek_end_of_line,                            KeyCode_Right, KeyCode_Alt);
        Bind(move_right_whitespace_boundary,              KeyCode_Right, KeyCode_Control); 
        
        Bind(backspace_char,                              KeyCode_Backspace);
        Bind(backspace_char,                              KeyCode_Backspace, KeyCode_Shift);
        Bind(J_BackspaceBoundaryWhitespace,           KeyCode_Backspace, KeyCode_Alt);
        Bind(J_BackspaceBoundaryAlphaNumericOrCamel,  KeyCode_Backspace, KeyCode_Control);
        
        Bind(delete_char,                               KeyCode_Delete);
        Bind(delete_char,                               KeyCode_Delete, KeyCode_Shift);
        Bind(J_DeleteBoundaryWhitespace,          KeyCode_Delete, KeyCode_Alt);
        Bind(J_DeleteBoundaryAlphaNumericOrCamel, KeyCode_Delete, KeyCode_Control);
        
        Bind(snipe_forward_whitespace_or_token_boundary, KeyCode_D, KeyCode_Alt);
        Bind(delete_range,                KeyCode_D, KeyCode_Control);
        
        Bind(J_OmnibufferIfReadOnlyGoToPosition,  KeyCode_Return);
        
        Bind(if0_off,                    KeyCode_I, KeyCode_Alt);
        
        Bind(command_lister,               KeyCode_F10);
    }
    
    
    SelectMap(file_id);
    {
        Bind(J_OmnibufferIfReadOnlyGoToPosition,  KeyCode_Return);
        //Bind(if_read_only_goto_position_same_panel, KeyCode_Return, KeyCode_Shift); 
        //Bind(view_buffer_other_panel,     KeyCode_Comma, KeyCode_Control);
        //Bind(swap_panels,                 KeyCode_Period, KeyCode_Control);
        //Bind(goto_line,                   KeyCode_G, KeyCode_Control);
        Bind(duplicate_line,              KeyCode_L, KeyCode_Control);
        Bind(center_view,                 KeyCode_E, KeyCode_Control);
        
        
        Bind(comment_line_toggle,        KeyCode_Semicolon, KeyCode_Control);
        
        Bind(auto_indent_range,          KeyCode_Tab, KeyCode_Control);
        Bind(auto_indent_line_at_cursor, KeyCode_Tab, KeyCode_Shift);
        Bind(word_complete_drop_down,    KeyCode_Tab, KeyCode_Shift, KeyCode_Control);
        Bind(write_block,                KeyCode_R, KeyCode_Alt);
        Bind(write_todo,                 KeyCode_T, KeyCode_Alt);
        Bind(write_note,                 KeyCode_Y, KeyCode_Alt);
        Bind(list_all_locations_of_type_definition,               KeyCode_D, KeyCode_Alt);
        Bind(list_all_locations_of_type_definition_of_identifier, KeyCode_T, KeyCode_Alt, KeyCode_Shift);
        Bind(open_long_braces,           KeyCode_LeftBracket, KeyCode_Control);
        Bind(open_long_braces_semicolon, KeyCode_LeftBracket, KeyCode_Control, KeyCode_Shift);
        Bind(open_long_braces_break,     KeyCode_RightBracket, KeyCode_Control, KeyCode_Shift);
        Bind(select_surrounding_scope,   KeyCode_LeftBracket, KeyCode_Alt);
        Bind(select_surrounding_scope_maximal, KeyCode_LeftBracket, KeyCode_Alt, KeyCode_Shift);
        Bind(select_prev_scope_absolute, KeyCode_RightBracket, KeyCode_Alt);
        Bind(select_prev_top_most_scope, KeyCode_RightBracket, KeyCode_Alt, KeyCode_Shift);
        Bind(select_next_scope_absolute, KeyCode_Quote, KeyCode_Alt);
        Bind(select_next_scope_after_current, KeyCode_Quote, KeyCode_Alt, KeyCode_Shift);
        //Bind(place_in_scope,             KeyCode_ForwardSlash, KeyCode_Alt);
        //Bind(delete_current_scope,       KeyCode_Minus, KeyCode_Alt);
        
        //Bind(open_file_in_quotes,        KeyCode_1, KeyCode_Alt);
        //Bind(open_matching_file_cpp,     KeyCode_2, KeyCode_Alt);
        //Bind(write_zero_struct,          KeyCode_0, KeyCode_Control);
        
        //Bind(left_adjust_view,            KeyCode_E, KeyCode_Control, KeyCode_Shift);
        //Bind(view_jump_list_with_lister,  KeyCode_Period, KeyCode_Control, KeyCode_Shift);
    }
    
    
    SelectMap(edit_id);
    ParentMap(file_id);
    ParentMap(global_id);
    {
        BindTextInput(write_text_input);
        
        
        Bind(J_OmnibufferBuild,                           KeyCode_F1);
        Bind(J_OmnibufferSetLargeAndActive,               KeyCode_1);
        Bind(J_OmnibufferSetSmallAndInactive,             KeyCode_2);
        
        
        Bind(set_mark,                    KeyCode_Space);
        Bind(cursor_mark_swap,            KeyCode_Space, KeyCode_Shift);
        
        Bind(interactive_open_or_new,     KeyCode_O);
        Bind(reopen,                      KeyCode_O, KeyCode_Control);
        Bind(open_in_other,               KeyCode_O, KeyCode_Shift);
        Bind(kill_buffer,                 KeyCode_K);
        Bind(interactive_kill_buffer,     KeyCode_K, KeyCode_Control);
        Bind(interactive_switch_buffer,   KeyCode_I);
        Bind(interactive_new,             KeyCode_N);
        
        Bind(open_matching_file_cpp,     KeyCode_I, KeyCode_Shift);
        Bind(open_file_in_quotes,        KeyCode_I, KeyCode_Control);
        
        Bind(replace_in_range,            KeyCode_A);
        Bind(query_replace,               KeyCode_Q);
        Bind(query_replace_selection,     KeyCode_Q, KeyCode_Control);
        Bind(query_replace_identifier,    KeyCode_Q, KeyCode_Shift);
        
        Bind(reverse_search,              KeyCode_R);
        Bind(search,                      KeyCode_F);
        Bind(search_identifier,           KeyCode_F, KeyCode_Shift);
        
        Bind(J_OmnibufferListAllLocations,         KeyCode_T);
        Bind(list_all_substring_locations_case_insensitive, KeyCode_T, KeyCode_Shift); 
        Bind(list_all_locations,                            KeyCode_T, KeyCode_Control);
        //Bind(omnibuffer_peek_definition_at_cursor,          KeyCode_T, KeyCode_Alt);
        
        Bind(save,                        KeyCode_S);
        Bind(save_all_dirty_buffers,      KeyCode_S, KeyCode_Control);
        Bind(copy,                        KeyCode_C);
        Bind(cut,                         KeyCode_X);
        Bind(redo,                        KeyCode_Y);
        Bind(undo,                        KeyCode_Z);
        Bind(paste_and_indent,            KeyCode_V);
        Bind(paste_next_and_indent,       KeyCode_V, KeyCode_Control);
        
        
        Bind(snippet_lister,              KeyCode_J);
        
        Bind(delete_line,                 KeyCode_D);
        
        Bind(jump_to_definition,  KeyCode_W, KeyCode_Shift);
        Bind(J_OmnibufferPeekDefinitionAtCursor, KeyCode_W);
        Bind(goto_next_jump,               KeyCode_F2);
        Bind(goto_prev_jump,               KeyCode_F3);
        Bind(goto_first_jump,              KeyCode_F4);
        
        
        //Bind(open_file_in_quotes,        KeyCode_1, KeyCode_Alt);
        //Bind(open_matching_file_cpp,     KeyCode_2, KeyCode_Alt);
        //Bind(write_zero_struct,          KeyCode_0, KeyCode_Control);
        
        
        
    }
    
    
    SelectMap(global_id);
    BindCore(J_DefaultStartup, CoreCode_Startup);
    BindCore(J_DefaultTryExit, CoreCode_TryExit);
    BindCore(clipboard_record_clip, CoreCode_NewClipboardContents);
    BindMouseWheel(mouse_wheel_scroll);
    BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
    
    SelectMap(file_id);
    ParentMap(global_id);
    BindTextInput(write_text_input);
    BindMouse(click_set_cursor_and_mark, MouseCode_Left);
    BindMouseRelease(click_set_cursor, MouseCode_Left);
    BindCore(click_set_cursor_and_mark, CoreCode_ClickActivateView);
    BindMouseMove(click_set_cursor_if_lbutton);
    
    SelectMap(code_id);
    ParentMap(file_id);
    BindTextInput(write_text_and_auto_indent);
    
    
}


function void
J_ModalModeStartup(Application_Links* app) {
    J_UpdateThemeForMode(app);
}


#endif //FOUR_JESSE_MODAL_MODE_H

