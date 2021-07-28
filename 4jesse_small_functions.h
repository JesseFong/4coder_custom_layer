/* date = February 27th 2021 10:55 am */

#ifndef FOUR_JESSE_SMALL_FUNCTIONS_H
#define FOUR_JESSE_SMALL_FUNCTIONS_H

/*
*  Small Functions for 4coder that should be able to be dropped in without any moditification of other parts of the code
*
*
*
*/


CUSTOM_COMMAND_SIG(J_DeleteBoundaryWhitespace)
CUSTOM_DOC("Delete until whitespace"){
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Forward,
                                 push_boundary_list(scratch, boundary_non_whitespace));
}

CUSTOM_COMMAND_SIG(J_BackspaceBoundaryWhitespace)
CUSTOM_DOC("Backspace until whitespace"){
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Backward,
                                 push_boundary_list(scratch, boundary_non_whitespace));
}

CUSTOM_COMMAND_SIG(J_DeleteBoundaryAlphaNumericOrCamel)
CUSTOM_DOC("Delete until Alphanumeric or Camel"){
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Forward,
                                 push_boundary_list(scratch, boundary_alpha_numeric_camel));
}

CUSTOM_COMMAND_SIG(J_BackspaceBoundaryAlphaNumericOrCamel)
CUSTOM_DOC("Backspace Until Alphanumeric or Camel"){
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Backward,
                                 push_boundary_list(scratch, boundary_alpha_numeric_camel));
}

CUSTOM_COMMAND_SIG(J_WriteImportant)
CUSTOM_DOC("At the cursor, insert a '//IMPORTANT' comment, includes user name if it was specified in config.4coder.")
{
    write_named_comment_string(app, "IMPORTANT");
}

CUSTOM_COMMAND_SIG(J_WriteBreakpoint)
CUSTOM_DOC("At the cursor, insert a breakpoint line")
{
    Scratch_Block scratch(app);
    String_Const_u8 str = push_u8_stringf(scratch, "int BreakHere = 0;");
    write_string(app, str);
}

#endif //4JESSE_SMALL_FUNCTIONS_H
