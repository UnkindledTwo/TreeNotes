#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QColor>

struct app_config{
    bool notetree_select_new_items = true;
    bool notetree_alternating_row_colors = true;
    int notetree_indentation_size = 20;
    int opacity = 100; //100 for solid, 0 for transparent
    bool notetree_animated = false;
    bool doubleClickToEditMessage = true;
    unsigned short int layoutMargin = 6;
    unsigned short int splitter_handle_width = 6;
    bool confirm_delete = true;
    bool line_wrapping = true;
    bool pair_completion = true;
    bool notetree_drag_drop = false;
    int maximum_backups = 20;
    bool highlight_current_line = true;
    bool use_native_theme = false;
    bool dark_mode = true;
};

#endif // APPCONFIG_H
