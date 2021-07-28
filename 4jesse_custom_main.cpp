/*
   - TODO(Jesse):
- Function Definition Tooltips under cursor
- Quick code peek definitions
- Change color of recently pasted stuff to cut down on copy paste errors
- Macro unrolling?
- 
*/

#if !defined(FCODER_DEFAULT_BINDINGS_CPP)
#define FCODER_DEFAULT_BINDINGS_CPP

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "4coder_default_include.cpp"
#pragma warning(disable : 4189)

#include "4jesse_custom/4jesse_include_for_all_files.h"

#include "4jesse_custom/4jesse_small_functions.h"
#include "4jesse_custom/4jesse_omnibuffer.h"
#include "4jesse_custom/4jesse_error_annotations.h"
#include "4jesse_custom/4jesse_scope_annotations.h"
#include "4jesse_custom/4jesse_cursor_annotations.h"

#include "4jesse_custom/4jesse_modal_mode.h"
#include "4jesse_custom/4jesse_custom_hooks.cpp"


// NOTE(allen): Users can declare their own managed IDs here.

#if !defined(META_PASS)
#include "generated/managed_id_metadata.cpp"
#endif


static void
J_SetCustomHooks(Application_Links* app) {
    
    set_custom_hook(app, HookID_BufferViewerUpdate, default_view_adjust);
    
    set_custom_hook(app, HookID_ViewEventHandler, default_view_input_handler);
    set_custom_hook(app, HookID_Tick, default_tick);
    set_custom_hook(app, HookID_RenderCaller, J_DefaultRenderCaller);
    set_custom_hook(app, HookID_WholeScreenRenderCaller, J_DefaultWholeScreenRenderCaller);
    
    set_custom_hook(app, HookID_DeltaRule, fixed_time_cubic_delta);
    set_custom_hook_memory_size(app, HookID_DeltaRule,
                                delta_ctx_size(fixed_time_cubic_delta_memory_size));
    
    set_custom_hook(app, HookID_BufferNameResolver, default_buffer_name_resolution);
    
    set_custom_hook(app, HookID_BeginBuffer, J_DefaultBeginBuffer);
    set_custom_hook(app, HookID_EndBuffer, end_buffer_close_jump_list);
    set_custom_hook(app, HookID_NewFile, default_new_file);
    set_custom_hook(app, HookID_SaveFile, default_file_save);
    set_custom_hook(app, HookID_BufferEditRange, default_buffer_edit_range);
    set_custom_hook(app, HookID_BufferRegion, default_buffer_region);
    set_custom_hook(app, HookID_ViewChangeBuffer, default_view_change_buffer);
    
    set_custom_hook(app, HookID_Layout, layout_unwrapped);
    
    implicit_map_function = J_ImplicitMapFunction;
    //set_custom_hook(app, HookID_Layout, layout_wrap_anywhere);
    //set_custom_hook(app, HookID_Layout, layout_wrap_whitespace);
    //set_custom_hook(app, HookID_Layout, layout_virt_indent_unwrapped);
    //set_custom_hook(app, HookID_Layout, layout_unwrapped_small_blank_lines);
} 

void
custom_layer_init(Application_Links *app){
    
    Thread_Context *tctx = get_thread_context(app);
    
    // NOTE(allen): setup for default framework
    default_framework_init(app);
    
    // NOTE(allen): default hooks and command maps
    set_all_default_hooks(app);
    J_SetCustomHooks(app);
    mapping_init(tctx, &framework_mapping);
    
    J_SetupEssentialMapping(&framework_mapping);
    
}

#endif //FCODER_DEFAULT_BINDINGS

// BOTTOM