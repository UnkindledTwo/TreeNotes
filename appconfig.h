#ifndef APPCONFIG_H
#define APPCONFIG_H

struct app_config {
    unsigned short tab_width = 4;
    bool notetree_select_new_items = true;
    bool notetree_alternating_row_colors = true;
    unsigned short notetree_icon_size = 20;
    int notetree_indentation_size = 20;
    int opacity = 100;  // 100 for solid, 0 for transparent
    bool notetree_animated = false;
    bool doubleClickToEditMessage = true;
    unsigned short int layoutMargin = 6;
    unsigned short int splitter_handle_width = 6;
    bool confirm_delete = true;
    bool line_wrapping = true;
    bool pair_completion = true;
    bool notetree_drag_drop = false;
    int maximum_backups = 20;
    bool use_native_theme = false;
    bool dark_mode = true;
    bool colorful_highlight = true;
};

#endif  // APPCONFIG_H
