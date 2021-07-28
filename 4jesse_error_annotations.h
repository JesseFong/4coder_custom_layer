/* date = July 25th 2021 2:00 pm */

#ifndef FOUR_JESSE_ERROR_ANNOTATIONS_H
#define FOUR_JESSE_ERROR_ANNOTATIONS_H

//Basically Stolen from https://github.com/Dion-Systems/4coder_fleury/blob/master/4coder_fleury_error_annotations.cpp 
//but made easier to drop into any codebase

/*
 * NOTE(Jesse): This is SUPER UNOPTIMIZED and basically N^2 between the error list and the line highlighting 
* but I don't know enough about the 4coder code base yet to exctract exatly what i need. If you are reading this and know a good way to 
* extract jumps from the jump buffer by knowing ALREADY where the jump is, you can optimize this yourself and please email me fong.jes@gmail.com
*
*
*
*/

CUSTOM_ID(colors, J_defcolor_error_annotations);

function Sticky_Jump
J_GetJumpForBufferLineNumberFromArray(Application_Links* app, Buffer_ID buffer, i64 line_number,  Sticky_Jump_Array JumpArray) {
    Sticky_Jump Result = {};
    for(i32 JumpIndex = 0;
        JumpIndex < JumpArray.count;
        JumpIndex++) {
        
        Sticky_Jump* JumpToTest = &JumpArray.jumps[JumpIndex];
        if(JumpToTest->jump_buffer_id == buffer) {
            
            i64 jump_line = get_line_number_from_pos(app, buffer, JumpToTest->jump_pos);
            if(jump_line == line_number) {
                Result = *JumpToTest;
                break;
            }
        }
    }
    
    return Result;
}


function void
J_RenderErrorAnnotations(Application_Links *app, Buffer_ID buffer, Face_ID face_id,
                         Text_Layout_ID text_layout_id,
                         Buffer_ID jump_buffer)
{
    
#if 1
    Heap *heap = &global_heap;
    Scratch_Block scratch(app);
    
    Locked_Jump_State jump_state = {};
    {
        ProfileScope(app, "[Fleury] Error Annotations (Get Locked Jump State)");
        jump_state = get_locked_jump_state(app, heap);
    }
    
    Face_ID face = face_id;
    Face_Metrics metrics = get_face_metrics(app, face);
    
    if(jump_buffer != 0 && jump_state.view != 0)
    {
        Managed_Scope buffer_scopes[2];
        {
            ProfileScope(app, "[Fleury] Error Annotations (Buffer Get Managed Scope)");
            buffer_scopes[0] = buffer_get_managed_scope(app, jump_buffer);
            buffer_scopes[1] = buffer_get_managed_scope(app, buffer);
        }
        
        Managed_Scope comp_scope = 0;
        {
            ProfileScope(app, "[Fleury] Error Annotations (Get Managed Scope)");
            comp_scope = get_managed_scope_with_multiple_dependencies(app, buffer_scopes, ArrayCount(buffer_scopes));
        }
        
        Managed_Object *buffer_markers_object = 0;
        {
            ProfileScope(app, "[Fleury] Error Annotations (Scope Attachment)");
            buffer_markers_object = scope_attachment(app, comp_scope, sticky_jump_marker_handle, Managed_Object);
        }
        
        // NOTE(rjf): Get buffer markers (locations where jumps point at).
        i32 buffer_marker_count = 0;
        Marker *buffer_markers = 0;
        {
            ProfileScope(app, "[Fleury] Error Annotations (Load Managed Object Data)");
            buffer_marker_count = managed_object_get_item_count(app, *buffer_markers_object);
            buffer_markers = push_array(scratch, Marker, buffer_marker_count);
            managed_object_load_data(app, *buffer_markers_object, 0, buffer_marker_count, buffer_markers);
        }
        
        i64 last_line = -1;
        
        if(buffer_marker_count == 0) {
            return;
        }
        
        Sticky_Jump_Array jumps = parse_buffer_to_jump_array(app, scratch, jump_buffer);
        
        for(i32 i = 0; i < buffer_marker_count; i += 1)
        {
            ProfileScope(app, "[Fleury] Error Annotations (Buffer Loop)");
            
            i64 code_line_number = get_line_number_from_pos(app, buffer, buffer_markers[i].pos);
            
            Sticky_Jump jump = J_GetJumpForBufferLineNumberFromArray(app, buffer, code_line_number, jumps);
            
            if(code_line_number != last_line)
            {
                ProfileScope(app, "[Fleury] Error Annotations (Jump Line)");
                
                String_Const_u8 jump_line = push_buffer_line(app, scratch, jump_buffer, jump.list_line);
                String_Const_u8 jump_line_1 = push_buffer_line(app, scratch, jump_buffer, jump.list_line + 1);
                
                // NOTE(rjf): Remove file part of jump line.
                {
                    u64 index = string_find_first(jump_line, string_u8_litexpr("error"), StringMatch_CaseInsensitive);
                    if(index == jump_line.size)
                    {
                        index = string_find_first(jump_line, string_u8_litexpr("warning"), StringMatch_CaseInsensitive);
                        if(index == jump_line.size)
                        {
                            index = 0;
                        }
                    }
                    jump_line.str += index;
                    jump_line.size -= index;
                }
                
                // NOTE(rjf): Render annotation.
                {
                    Range_i64 line_range = Ii64(code_line_number);
                    
                    Range_f32 y1 = text_layout_line_on_screen(app, text_layout_id, line_range.min);
                    Range_f32 y2 = text_layout_line_on_screen(app, text_layout_id, line_range.max);
                    Range_f32 y = range_union(y1, y2);
                    Rect_f32 last_character_on_line_rect =
                        text_layout_character_on_screen(app, text_layout_id, get_line_end_pos(app, buffer, code_line_number)-1);
                    
                    if(range_size(y) > 0.f)
                    {
                        Rect_f32 region = text_layout_region(app, text_layout_id);
                        Vec2_f32 draw_position =
                        {
                            last_character_on_line_rect.x1 + 30,
                            y.min + (y.max-y.min)/2 - metrics.line_height/2,
                        };
                        
                        if(draw_position.x < last_character_on_line_rect.x1 + 30)
                        {
                            draw_position.x = last_character_on_line_rect.x1 + 30;
                        }
                        
                        draw_string(app, face, jump_line, draw_position, fcolor_id(J_defcolor_error_annotations));
                    }
                }
            }
            
            last_line = code_line_number;
        }
    }
#endif
}

#endif //4JESSE_ERROR_ANNOTATIONS_H
