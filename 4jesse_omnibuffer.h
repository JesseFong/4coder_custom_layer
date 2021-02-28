/* date = February 26th 2021 4:08 pm */

#ifndef FOUR_JESSE_OMNIBUFFER_H
#define FOUR_JESSE_OMNIBUFFER_H

/*
*  This file adds a constant buffer to the bottom of 4coder that is universally used for compilation errors, search and code peek
*  - Everything should work with just this file EXCEPT RENDERING you need to call J_OmnibufferDraw(app, frame_info); in your
 *        (default_whole_screen_render_caller) Hook.
*
*  - Disable OMNIBUFFER_DRAW_FULL_SCREEN_WIDTH if you would rather it draw to the normal width of your views
*
*/

#define OMNIBUFFER_DRAW_FULL_SCREEN_WIDTH 1

#define MAX_DUMMY_VIEW_COUNT 128
global View_ID dummy_views[MAX_DUMMY_VIEW_COUNT] = {};
global u32 dummy_view_count = 0;

#define OMNIBUFFER_SMALL_RATIO 0.05f
#define OMNIBUFFER_LARGE_RATIO 0.9f
#define OMNIBUFFER_MAX_BUFFER_COUNT 16
#define OMNIBUFFER_BIN_HEIGHT 15

struct omnibuffer 
{
    View_ID last_view_id = 0;
    View_ID view_id = 0;
    b32 is_open = 0;
    b32 is_large = 0;
};
global omnibuffer GLOBALOmnibuffer = {}; 

internal b32
J_OmnibufferIsOpen() {
    b32 result = false;
    if(!GLOBALOmnibuffer.view_id == 0) {
        result = true;
    }
    return result;
}

internal View_ID
J_OmnibufferGetViewID() {
    View_ID result = GLOBALOmnibuffer.view_id;
    return result;
}

internal View_ID
J_OmnibufferOpenView(Application_Links *app, View_ID active_view) {
    View_ID new_view = open_view(app, active_view, ViewSplit_Bottom);
    new_view_settings(app, new_view);
    Buffer_ID buffer = view_get_buffer(app, new_view, Access_Always);
    view_set_buffer(app, new_view, buffer, 0);
    view_set_passive(app, new_view, true);
    return(new_view);
}

internal void
J_OmnibufferOpenDummyViews(Application_Links* app, View_ID active_view) {
    View_ID next_view = get_next_view_looped_primary_panels(app, active_view, Access_Always);
    while(next_view != active_view) {
        dummy_views[dummy_view_count] = J_OmnibufferOpenView(app, next_view);
        next_view = get_next_view_looped_primary_panels(app, next_view, Access_Always);
        dummy_view_count++;
    }
}

internal View_ID
J_OmnibufferOpenInternal(Application_Links* app) {
    View_ID result = {};
    
    View_ID view = get_active_view(app, Access_Always);
    if(!J_OmnibufferIsOpen()) {
        GLOBALOmnibuffer.view_id = J_OmnibufferOpenView(app, view);
#if OMNIBUFFER_DRAW_FULL_SCREEN_WIDTH
        J_OmnibufferOpenDummyViews(app, view);
#endif 
    }
    
    if(GLOBALOmnibuffer.view_id != view) {
        GLOBALOmnibuffer.last_view_id = view;
    }
    
    result = GLOBALOmnibuffer.view_id;
    return result;
}

internal void
J_OmnibufferSetDummyViewSize(Application_Links *app) {
    for(u32 view_index = 0;
        view_index < dummy_view_count;
        view_index++) {
        View_ID view = dummy_views[view_index];
        view_set_split_proportion(app, view, GLOBALOmnibuffer.is_large ? OMNIBUFFER_LARGE_RATIO : OMNIBUFFER_SMALL_RATIO);
    }
}

internal void
J_OmnibufferSetSizeInternal(Application_Links* app) {
    View_ID view = J_OmnibufferOpenInternal(app);
    view_set_split_proportion(app, view, GLOBALOmnibuffer.is_large ? OMNIBUFFER_LARGE_RATIO : OMNIBUFFER_SMALL_RATIO);
    
#if OMNIBUFFER_DRAW_FULL_SCREEN_WIDTH
    J_OmnibufferSetDummyViewSize(app);
#endif 
}


internal void
J_OmnibufferCloseDummyViews(Application_Links *app) {
    for(u32 view_index = 0;
        view_index < dummy_view_count;
        view_index++) {
        View_ID view = dummy_views[view_index];
        if(view_exists(app, view)) {
            view_close(app, view);
        }
    }
    dummy_view_count = 0;
}

internal void
J_OmnibufferCloseInternal(Application_Links* app) {
    if(view_exists(app, GLOBALOmnibuffer.view_id)) {
        view_close(app, GLOBALOmnibuffer.view_id);
    }
    GLOBALOmnibuffer.view_id = 0;
    
#if OMNIBUFFER_DRAW_FULL_SCREEN_WIDTH
    J_OmnibufferCloseDummyViews(app);
#endif
}


internal void
J_OmnibufferSetActive(Application_Links* app) {
    View_ID view = J_OmnibufferOpenInternal(app);
    view_set_active(app, view);
}

internal void
J_OmnibufferSetPassive(Application_Links* app) {
    View_ID view = J_OmnibufferOpenInternal(app);
    view_set_passive(app, view, true);
    view_set_active(app, GLOBALOmnibuffer.last_view_id);
}


internal void
J_OmnibufferDisplayAndMaybeSwitch(Application_Links *app, b32 switch_to_omnibuffer) {
    
    View_ID last_view = get_active_view(app, Access_Always);
    View_ID omnibuffer_view = J_OmnibufferOpenInternal(app);
    
    J_OmnibufferSetSizeInternal(app);
    
    if(switch_to_omnibuffer) {
        view_set_active(app, omnibuffer_view);
    } else {
        view_set_active(app, last_view);
    }
}

internal void
J_OmnibufferAddBufferToView(Application_Links* app, Buffer_ID buffer_id, i64 cursor_pos) {
    if(J_OmnibufferIsOpen()) {
        View_ID view = GLOBALOmnibuffer.view_id;
        view_set_buffer(app, view, buffer_id, 0);
        view_set_cursor_and_preferred_x(app, view, seek_pos(cursor_pos));
    }
}


CUSTOM_COMMAND_SIG(J_OmnibufferOpen) {
    J_OmnibufferDisplayAndMaybeSwitch(app, false);
}

CUSTOM_COMMAND_SIG(J_OmnibufferOpenAndSwitch) {
    J_OmnibufferDisplayAndMaybeSwitch(app, true);
}

CUSTOM_COMMAND_SIG(J_OmnibufferSetSmall) {
    if(GLOBALOmnibuffer.is_large) {
        GLOBALOmnibuffer.is_large = false;
    }
    J_OmnibufferOpen(app);
}

CUSTOM_COMMAND_SIG(J_OmnibufferSetLarge) {
    if(!GLOBALOmnibuffer.is_large) {
        GLOBALOmnibuffer.is_large = true;
    }
    J_OmnibufferOpen(app);
}

CUSTOM_COMMAND_SIG(J_OmnibufferToggleSize) {
    GLOBALOmnibuffer.is_large = !GLOBALOmnibuffer.is_large;
    J_OmnibufferOpen(app);
}


CUSTOM_COMMAND_SIG(J_OmnibufferClose) {
    J_OmnibufferCloseInternal(app);
}

CUSTOM_COMMAND_SIG(J_OmnibufferSetLargeAndActive) {
    GLOBALOmnibuffer.is_large = true;
    J_OmnibufferOpenAndSwitch(app);
}

CUSTOM_COMMAND_SIG(J_OmnibufferSetSmallAndInactive) {
    GLOBALOmnibuffer.is_large = false;
    J_OmnibufferOpen(app);
    J_OmnibufferSetPassive(app);
}


CUSTOM_COMMAND_SIG(J_OmnibufferOpenActiveViewInOmnibuffer) {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    i64 pos = view_get_cursor_pos(app, view);
    
    J_OmnibufferOpen(app);
    J_OmnibufferAddBufferToView(app, buffer, pos);
}

CUSTOM_COMMAND_SIG(J_OmnibufferBuild) {
    J_OmnibufferOpen(app);
    View_ID view = J_OmnibufferGetViewID();
    Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*compilation*"), BufferCreate_AlwaysNew);
    standard_search_and_build(app, view, buffer);
    set_fancy_compilation_buffer_font(app);
    block_zero_struct(&prev_location);
    lock_jump_buffer(app, string_u8_litexpr("*compilation*"));
}


CUSTOM_COMMAND_SIG(J_OmnibufferIfReadOnlyGoToPosition) {
    View_ID view = get_active_view(app, Access_ReadVisible);
    View_ID omnibuffer_view = J_OmnibufferGetViewID();
    
    if_read_only_goto_position(app);
    
    if(view == omnibuffer_view) {
        J_OmnibufferSetSmall(app);
    }
}

internal void
J_OmnibufferListAllLocationsGeneric(Application_Links *app, String_Const_u8_Array needle, List_All_Locations_Flag flags) {
    
    J_OmnibufferSetLarge(app);
    
    if (needle.count > 0){
        View_ID target_view = J_OmnibufferGetViewID();//get_next_view_after_active(app, Access_Always);
        String_Match_Flag must_have_flags = 0;
        String_Match_Flag must_not_have_flags = 0;
        if (HasFlag(flags, ListAllLocationsFlag_CaseSensitive)){
            AddFlag(must_have_flags, StringMatch_CaseSensitive);
        }
        if (!HasFlag(flags, ListAllLocationsFlag_MatchSubstring)){
            AddFlag(must_not_have_flags, StringMatch_LeftSideSloppy);
            AddFlag(must_not_have_flags, StringMatch_RightSideSloppy);
        }
        print_all_matches_all_buffers_to_search(app, needle, must_have_flags, must_not_have_flags, target_view);
    }
}


internal void
J_OmnibufferListAllLocationsGeneric(Application_Links *app, String_Const_u8 needle, List_All_Locations_Flag flags){
    if (needle.size != 0){
        String_Const_u8_Array array = {&needle, 1};
        J_OmnibufferListAllLocationsGeneric(app, array, flags);
    }
}

internal void
J_OmnibufferListAllLocationsGenericQuery(Application_Links *app, List_All_Locations_Flag flags){
    Scratch_Block scratch(app);
    u8 *space = push_array(scratch, u8, KB(1));
    String_Const_u8 needle = get_query_string(app, "List Locations For: ", space, KB(1));
    J_OmnibufferListAllLocationsGeneric(app, needle, flags);
}

internal void
J_OmnibufferListAllLocationsGenericIdentifier(Application_Links *app, List_All_Locations_Flag flags){
    Scratch_Block scratch(app);
    String_Const_u8 needle = push_token_or_word_under_active_cursor(app, scratch);
    J_OmnibufferListAllLocationsGeneric(app, needle, flags);
}

internal void
J_OmnibufferListAllLocationsGenericViewRange(Application_Links *app, List_All_Locations_Flag flags){
    Scratch_Block scratch(app);
    String_Const_u8 needle = push_view_range_string(app, scratch);
    J_OmnibufferListAllLocationsGeneric(app, needle, flags);
}

CUSTOM_COMMAND_SIG(J_OmnibufferListAllLocations) {
    J_OmnibufferListAllLocationsGenericQuery(app, ListAllLocationsFlag_CaseSensitive);
}

CUSTOM_COMMAND_SIG(J_OmnibufferListAllLocationsCaseInsensitive) {
    J_OmnibufferListAllLocationsGenericQuery(app, 0);
}

CUSTOM_COMMAND_SIG(J_OmnibufferListAllLocationsOfIdentifier) {
    J_OmnibufferListAllLocationsGenericIdentifier(app, ListAllLocationsFlag_CaseSensitive);
}

CUSTOM_COMMAND_SIG(J_OmnibufferPeekDefinitionAtCursor) {
    
    View_ID view = get_active_view(app, Access_Visible);
    if (view != 0){
        Scratch_Block scratch(app);
        String_Const_u8 query = push_token_or_word_under_active_cursor(app, scratch);
        
        code_index_lock();
        for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
             buffer != 0;
             buffer = get_buffer_next(app, buffer, Access_Always)){
            Code_Index_File *file = code_index_get_file(buffer);
            if (file != 0){
                for (i32 i = 0; i < file->note_array.count; i += 1){
                    Code_Index_Note *note = file->note_array.ptrs[i];
                    if (string_match(note->text, query)){
                        
                        J_OmnibufferSetLargeAndActive(app);
                        View_ID omnibuffer_view = J_OmnibufferGetViewID();
                        point_stack_push_view_cursor(app, omnibuffer_view);
                        jump_to_location(app, omnibuffer_view, buffer, note->pos.first);
                        view_set_active(app, view);
                        
                        goto done;
                    }
                }
            }
        }
        done:;
        code_index_unlock();
    }
}

static Rect_f32
J_OmnibufferDrawFullscreenAndMargin(Application_Links* app, f32 width) {
    Rect_f32 screen_rect = global_get_screen_rectangle(app);
    
    View_ID omnibuffer_view = J_OmnibufferGetViewID();
    Rect_f32 omnibuffer_internal_rect = view_get_screen_rect(app, omnibuffer_view);
    omnibuffer_internal_rect.x0 = screen_rect.x0;
    omnibuffer_internal_rect.x1 = screen_rect.x1;
    Rect_f32 inner = rect_inner(omnibuffer_internal_rect, width);
    
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == omnibuffer_view);
    
    FColor margin_color = get_panel_margin_color(is_active_view?UIHighlight_Active:UIHighlight_None);
    FColor back_color = fcolor_id(defcolor_back);
    
    draw_rectangle(app, inner, 0.f, fcolor_resolve(back_color));
    if(width > 0.f) {
        draw_margin(app, omnibuffer_internal_rect, inner, fcolor_resolve(margin_color));
    }
    return(inner);
}

static void
J_OmnibufferDraw(Application_Links* app, Frame_Info frame_info) {
    
    
    if(!J_OmnibufferIsOpen()) {
        return;
    }
    View_ID view_id = J_OmnibufferGetViewID();
    
    Rect_f32 region = J_OmnibufferDrawFullscreenAndMargin(app, 3.f);
    Rect_f32 prev_clip = draw_set_clip(app, region);
    
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 line_height = face_metrics.line_height;
    //f32 digit_advance = face_metrics.decimal_digit_advance;
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if (view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) && showing_file_bar){
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        draw_file_bar(app, view_id, buffer, face_id, pair.min);
        region = pair.max;
    }
    
    Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
    
    Buffer_Point_Delta_Result delta = delta_apply(app, view_id,
                                                  frame_info.animation_dt, scroll);
    if (!block_match_struct(&scroll.position, &delta.point)){
        block_copy_struct(&scroll.position, &delta.point);
        view_set_buffer_scroll(app, view_id, scroll, SetBufferScroll_NoCursorChange);
    }
    if (delta.still_animating){
        animate_in_n_milliseconds(app, 0);
    }
    
    // NOTE(allen): query bars
    region = default_draw_query_bars(app, region, view_id, face_id);
    
    // NOTE(allen): FPS hud
    if (show_fps_hud){
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        draw_fps_hud(app, frame_info, face_id, pair.max);
        region = pair.min;
        animate_in_n_milliseconds(app, 1000);
    }
    
    
    // NOTE(allen): begin buffer render
    Buffer_Point buffer_point = scroll.position;
    Text_Layout_ID text_layout_id = text_layout_create(app, buffer, region, buffer_point);
    
    // NOTE(allen): draw the buffer
    default_render_buffer(app, view_id, face_id, buffer, text_layout_id, region);
    
    text_layout_free(app, text_layout_id);
    draw_set_clip(app, prev_clip);
}

static void
J_OmnibufferStartup(Application_Links* app) {
}

#endif //4JESSE_OMNIBUFFER_H
