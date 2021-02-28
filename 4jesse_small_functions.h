/* date = February 27th 2021 10:55 am */

#ifndef FOUR_JESSE_SMALL_FUNCTIONS_H
#define FOUR_JESSE_SMALL_FUNCTIONS_H

/*
*  Small Functions for 4coder that should be able to be dropped in without any moditification of other parts of the code
*
*
*
*/

CUSTOM_COMMAND_SIG(J_DeleteBoundaryWhitespace) {
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Forward,
                                 push_boundary_list(scratch, boundary_non_whitespace));
}

CUSTOM_COMMAND_SIG(J_BackspaceBoundaryWhitespace) {
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Backward,
                                 push_boundary_list(scratch, boundary_non_whitespace));
}

CUSTOM_COMMAND_SIG(J_DeleteBoundaryAlphaNumericOrCamel) {
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Forward,
                                 push_boundary_list(scratch, boundary_alpha_numeric_camel));
}

CUSTOM_COMMAND_SIG(J_BackspaceBoundaryAlphaNumericOrCamel) {
    Scratch_Block scratch(app);
    current_view_boundary_delete(app, Scan_Backward,
                                 push_boundary_list(scratch, boundary_alpha_numeric_camel));
}



#endif //4JESSE_SMALL_FUNCTIONS_H
