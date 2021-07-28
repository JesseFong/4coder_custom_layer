#ifndef FOUR_JESSE_MODAL_MODE_H
#define FOUR_JESSE_MODAL_MODE_H

/*
*  A pretty small an straightforward modal mode in 4coder
*  
*  USAGE: add "keys_edit" to your bindings.4coder file
 *   and call J_SetupEssentialMapping(): instead of setup_essential_mapping(); in default_4coder_initialize();
*   if you want to change the theme during macro recording you will need to call J_UpdateThemeForMode() during the macro functions (or use the functions below)
*  
*
*
*
*  In your default_begin_buffer hook, add this:
*
*  Command_Map_ID map_id = (treat_as_code)?(code_map_id):(file_map_id); <-----Search for this and add the following:
*

    *  String_ID file_map_id = vars_save_string_lit("keys_file");
    *  String_ID code_map_id = vars_save_string_lit("keys_code");
    *  String_ID edit_map_id = vars_save_string_lit("keys_edit");
    *
    *  Command_Map_ID map_id = file_map_id;
    *
    *  if(treat_as_code) {
        *     if(global_edit_mode) {
            *        map_id = edit_map_id;
        *     } else {
            *        map_id = code_map_id;
        *     }
    *  }
*  
*
*
*/

static b32 global_edit_mode = false;

CUSTOM_ID(colors, defcolor_indexnote_type);
CUSTOM_ID(colors, defcolor_indexnote_function);
CUSTOM_ID(colors, defcolor_indexnote_macro);
CUSTOM_ID(colors, defcolor_indexnote_4codercommand);

internal void
J_UpdateThemeForMode(Application_Links* app) {
    Color_Table *table = &active_color_table;
    Arena *arena = &global_theme_arena;
    linalloc_clear(arena);
    *table = make_color_table(app, arena);
    
    u32 EditModeColor = 0xFFEC0111;
    u32 TextModeColor = 0xFF26AAC6;
    u32 RecordingMacroColor = 0xFF8a1aaa;
    
    u32 BarColor = 0xFFCACACA;
    table->arrays[defcolor_bar]                   = make_colors(arena, BarColor); 
    
    if(global_keyboard_macro_is_recording) {
        table->arrays[defcolor_bar]                   = make_colors(arena, RecordingMacroColor); 
    }
    
    if(global_edit_mode) {
        table->arrays[defcolor_cursor]                = make_colors(arena, EditModeColor);
        table->arrays[defcolor_mark]                  = make_colors(arena, EditModeColor);
        
        table->arrays[defcolor_margin_hover]          = make_colors(arena, EditModeColor); 
        table->arrays[defcolor_margin_active]         = make_colors(arena, EditModeColor); 
    } else {
        table->arrays[defcolor_cursor]                = make_colors(arena, TextModeColor);
        table->arrays[defcolor_mark]                  = make_colors(arena, TextModeColor);
        
        table->arrays[defcolor_margin_hover]          = make_colors(arena, TextModeColor); 
        table->arrays[defcolor_margin_active]         = make_colors(arena, TextModeColor); 
        
    }
    
    u32 ColorAtCursor = 0xFF000000;
    u32 Comment = 0xFF7D7D7D;
    u32 TextDefault = 0xFFA08563;
    u32 BarelyNoticableColor = 0x887D7D7D;
    
    table->arrays[defcolor_at_cursor]             = make_colors(arena, ColorAtCursor);
    table->arrays[defcolor_at_highlight]          = make_colors(arena, ColorAtCursor);
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
    table->arrays[defcolor_text_default]          = make_colors(arena, TextDefault);
    table->arrays[defcolor_comment]               = make_colors(arena, Comment);
    table->arrays[defcolor_comment_pop]           = make_colors(arena, 0xff2ab34f, 0xFFdb2828, 0xFFF1FF00);
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
    
    table->arrays[J_defcolor_error_annotations] = make_colors(arena, BarelyNoticableColor);
    
    table->arrays[vertical_scope_annotation_text] = make_colors(arena, BarelyNoticableColor);
    table->arrays[J_token_match_color] = make_colors(arena, BarelyNoticableColor);
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

CUSTOM_COMMAND_SIG(J_KeyboardMacroToggleRecording)
CUSTOM_DOC("Toggle Macro Recording"){
    if(global_keyboard_macro_is_recording) {
        J_KeyboardMacroFinishRecording(app);
    } else {
        J_KeyboardMacroStartRecording(app);
    }
}

CUSTOM_COMMAND_SIG(J_SwitchToEditMode)
CUSTOM_DOC("Stop macro recording, do nothing if macro recording is not already started"){
    
    for(View_ID view = get_view_next(app, 0, Access_Always);
        view != 0;
        view = get_view_next(app, view, Access_Always)) {
        Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
        Managed_Scope scope = buffer_get_managed_scope(app, buffer);
        Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
        *map_id_ptr = vars_save_string_lit("keys_edit");
    }
    
    global_edit_mode = true;
    J_UpdateThemeForMode(app);
};

CUSTOM_COMMAND_SIG(J_SwitchToCodeMode) {
    
    for(View_ID view = get_view_next(app, 0, Access_Always);
        view != 0;
        view = get_view_next(app, view, Access_Always)) {
        Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
        Managed_Scope scope = buffer_get_managed_scope(app, buffer);
        Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
        *map_id_ptr = vars_save_string_lit("keys_code");
    }
    
    global_edit_mode = false;
    J_UpdateThemeForMode(app);
};

CUSTOM_COMMAND_SIG(J_SwitchModalModes)
CUSTOM_DOC("Toggle Between Code and Edit Bindings"){
    
    if(global_edit_mode) {
        J_SwitchToCodeMode(app);
    } else {
        J_SwitchToEditMode(app);
    };
}

function void
J_ModalModeStartup(Application_Links* app) {
    J_SwitchToCodeMode(app);
}

function void
J_SetupEssentialMapping(Mapping *mapping){
    
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    String_ID edit_map_id = vars_save_string_lit("keys_edit");
    
    // 4coder Essential Mapping
    MappingScope();
    SelectMapping(mapping);
    
    SelectMap(global_map_id);
    BindCore(J_DefaultStartup, CoreCode_Startup);
    BindCore(default_try_exit, CoreCode_TryExit);
    BindCore(clipboard_record_clip, CoreCode_NewClipboardContents);
    BindMouseWheel(mouse_wheel_scroll);
    BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
    
    SelectMap(file_map_id);
    ParentMap(global_map_id);
    BindTextInput(write_text_input);
    BindMouse(click_set_cursor_and_mark, MouseCode_Left);
    BindMouseRelease(click_set_cursor, MouseCode_Left);
    BindCore(click_set_cursor_and_mark, CoreCode_ClickActivateView);
    BindMouseMove(click_set_cursor_if_lbutton);
    
    SelectMap(code_map_id);
    ParentMap(file_map_id);
    BindTextInput(write_text_and_auto_indent);
    
    SelectMap(edit_map_id);
    ParentMap(file_map_id);
    //
    // Jesse Layer Essential Mapping
}


function Implicit_Map_Result
J_ImplicitMapFunction(Application_Links *app, String_ID lang, String_ID mode, Input_Event *event){
    Implicit_Map_Result result = {};
    
    View_ID view = get_this_ctx_view(app, Access_Always);
    
    Command_Map_ID map_id = default_get_map_id(app, view);
    
    Command_Map_ID code_map_id = vars_save_string_lit("keys_code");
    Command_Map_ID edit_map_id = vars_save_string_lit("keys_edit");
    
    if(map_id == code_map_id) {
        if(global_edit_mode) {
            map_id = edit_map_id;
        }
    }
    
    Command_Binding binding = map_get_binding_recursive(&framework_mapping, map_id, event);
    
    // TODO(allen): map_id <-> map name?
    result.map = 0;
    result.command = binding.custom;
    
    return(result);
}






#endif //FOUR_JESSE_MODAL_MODE_H

