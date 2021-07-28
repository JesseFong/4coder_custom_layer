/* date = July 26th 2021 7:30 pm */

#ifndef FOURJESSE_CURSOR_ANNOTATIONS_H
#define FOURJESSE_CURSOR_ANNOTATIONS_H

//NOTE(Jesse): Pretty easy to use, call J_RenderTorkenOccurances in your render_buffer() hook

CUSTOM_ID( colors, J_token_match_color);

function void
J_RenderTokenRangeHighlight(Application_Links* app, View_ID view, Text_Layout_ID text_layout_id, Range_i64 range) {
    Rect_f32 range_start_rect = text_layout_character_on_screen(app, text_layout_id, range.start);
    Rect_f32 range_end_rect = text_layout_character_on_screen(app, text_layout_id, range.end-1);
    Rect_f32 total_range_rect = {0};
    total_range_rect.x0 = Min(range_start_rect.x0, range_end_rect.x0);
    total_range_rect.y0 = Min(range_start_rect.y0, range_end_rect.y0);
    total_range_rect.x1 = Max(range_start_rect.x1, range_end_rect.x1);
    total_range_rect.y1 = Max(range_start_rect.y1, range_end_rect.y1);
    
    total_range_rect.y0 = total_range_rect.y1 - 1.f;
    total_range_rect.y1 += 1.f;
    
    draw_rectangle(app, total_range_rect, 4.f, fcolor_resolve(fcolor_id(J_token_match_color)));
}

function void
J_RenderTokenOccurances(Application_Links* app, Buffer_ID buffer, View_ID view, Text_Layout_ID text_layout_id, Token_Array token_array, Range_i64 visible_range) {
    
    Buffer_ID active_buffer = view_get_buffer(app, view, Access_Always);
    
    if(active_buffer != buffer) {
        return;
    }
    
    
    u64 cursor_pos = view_get_cursor_pos(app, view);
    
    Token_Array buffer_tokens = get_token_array_from_buffer(app, buffer);
    Token* cursor_token = token_from_pos(&buffer_tokens, cursor_pos);
    
    if(cursor_token) {
        if(cursor_token->kind == TokenBaseKind_Identifier) {
            Scratch_Block scratch(app);
            String_Const_u8 query = push_token_or_word_under_pos(app, scratch, buffer, cursor_pos);
            
            i64 first_index = token_index_from_pos(&token_array, visible_range.first);
            Token_Iterator_Array it = token_iterator_index(0, &token_array, first_index);
            
            while(1) {
                Token* token = token_it_read(&it);
                if(!token || token->pos >= visible_range.one_past_last) {
                    return;
                }
                
                if(token->kind == TokenBaseKind_Identifier) {
                    
                    Range_i64 token_range = Ii64(token);
                    //Skip cursor token
                    if(!range_contains(token_range, cursor_pos)) {
                        String_Const_u8 token_string = push_buffer_range(app, scratch, buffer, token_range);
                        
                        if(string_match(token_string, query)) {
                            J_RenderTokenRangeHighlight(app, view, text_layout_id, token_range);
                        }
                    }
                }
                if(!token_it_inc_non_whitespace(&it))
                {
                    break;
                }
            }
        }
    }
}

#endif //4JESSE_CURSOR_ANNOTATIONS_H
